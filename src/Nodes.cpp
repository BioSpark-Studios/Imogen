// Creating a node graph editor for ImGui
// Quick demo, not production code! This is more of a demo of how to use ImGui to create custom stuff.
// Better version by @daniel_collin here https://gist.github.com/emoon/b8ff4b4ce4f1b43e79f2
// See https://github.com/ocornut/imgui/issues/306
// v0.03: fixed grid offset issue, inverted sign of 'scrolling'
// Animated gif: https://cloud.githubusercontent.com/assets/8225057/9472357/c0263c04-4b4c-11e5-9fdf-2cd4f33f6582.gif

#include "imgui.h"
#include "imgui_internal.h"
#include <math.h> // fmodf
#include "Nodes.h"
#include <vector>
#include <algorithm>
#include <assert.h>


UndoRedoHandler undoRedoHandler;

// NB: You can use math functions/operators on ImVec2 if you #define IMGUI_DEFINE_MATH_OPERATORS and #include "imgui_internal.h"
// Here we only declare simple +/- operators so others don't leak into the demo code.
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
static inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
static inline float Distance(ImVec2& a, ImVec2& b) { return sqrtf((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y)); }

static void GetConCount(const NodeGraphDelegate::MetaNode* metaNodes, int type, int &input, int &output)
{
	input = 0;
	output = 0;
	const NodeGraphDelegate::MetaNode& metaNode = metaNodes[type];
	for (int i = 0; i < NodeGraphDelegate::MaxCon; i++)
	{
		if (!metaNode.mInputs[i].mName)
			break;
		input++;
	}
	for (int i = 0; i < NodeGraphDelegate::MaxCon; i++)
	{
		if (!metaNode.mOutputs[i].mName)
			break;
		output++;
	}

}

// Dummy
struct Node
{
	int     mType;
	ImVec2  Pos, Size;
	int     InputsCount, OutputsCount;

	Node(int type, const ImVec2& pos, const NodeGraphDelegate::MetaNode* metaNodes)
	{
		mType = type;
		Pos = pos;
		GetConCount(metaNodes, type, InputsCount, OutputsCount);
	}

	ImVec2 GetInputSlotPos(int slot_no) const { return ImVec2(Pos.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)InputsCount + 1)); }
	ImVec2 GetOutputSlotPos(int slot_no) const { return ImVec2(Pos.x + Size.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)OutputsCount + 1)); }
};
struct NodeLink
{
	int     InputIdx, InputSlot, OutputIdx, OutputSlot;
	NodeLink() {}
	NodeLink(int input_idx, int input_slot, int output_idx, int output_slot) { InputIdx = input_idx; InputSlot = input_slot; OutputIdx = output_idx; OutputSlot = output_slot; }
	bool operator == (const NodeLink& other) const
	{
		return InputIdx == other.InputIdx && InputSlot == other.InputSlot && OutputIdx == other.OutputIdx && OutputSlot == other.OutputSlot;
	}
};

struct NodeOrder
{
	size_t mNodeIndex;
	size_t mNodePriority;
	bool operator < (const NodeOrder& other) const
	{
		return other.mNodePriority < mNodePriority; // reverse order compared to priority value: lower last
	}
};

size_t PickBestNode(const std::vector<NodeOrder>& orders)
{
	for (auto& order : orders)
	{
		if (order.mNodePriority == 0)
			return order.mNodeIndex;
	}
	// issue!
	assert(0);
	return -1;
}

void RecurseSetPriority(std::vector<NodeOrder>& orders, const ImVector<NodeLink> &links, size_t currentIndex, size_t currentPriority, size_t& undeterminedNodeCount)
{
	if (!orders[currentIndex].mNodePriority)
		undeterminedNodeCount--;

	orders[currentIndex].mNodePriority = std::max(orders[currentIndex].mNodePriority, currentPriority+1);
	for (auto & link : links)
	{
		if (link.OutputIdx == currentIndex)
		{
			RecurseSetPriority(orders, links, link.InputIdx, currentPriority + 1, undeterminedNodeCount);
		}
	}
}

std::vector<NodeOrder> ComputeEvaluationOrder(const ImVector<NodeLink> &links, size_t nodeCount)
{
	std::vector<NodeOrder> orders(nodeCount);
	for (size_t i = 0; i < nodeCount; i++)
	{
		orders[i].mNodeIndex = i;
		orders[i].mNodePriority = 0;
	}
	size_t undeterminedNodeCount = nodeCount;
	while (undeterminedNodeCount)
	{
		size_t currentIndex = PickBestNode(orders);
		RecurseSetPriority(orders, links, currentIndex, orders[currentIndex].mNodePriority, undeterminedNodeCount);
	};
	//
	return orders;
}


static std::vector<NodeOrder> mOrders;
static ImVector<Node> nodes;
static ImVector<NodeLink> links;


void UpdateEvaluationOrder(NodeGraphDelegate *delegate)
{
	mOrders = ComputeEvaluationOrder(links, nodes.size());
	std::sort(mOrders.begin(), mOrders.end());
	std::vector<size_t> nodeOrderList(mOrders.size());
	for (size_t i = 0; i < mOrders.size(); i++)
		nodeOrderList[i] = mOrders[i].mNodeIndex;
	delegate->UpdateEvaluationList(nodeOrderList);
}

std::string encode(unsigned char* ptr, size_t size)
{
	std::string res = "";
	for (size_t i = 0; i < size; i++)
	{
		char tmps[3];
		sprintf(tmps, "%02x", ptr[i]);
		res += tmps;
	}
	return res;
}

void decode(const std::string & str, unsigned char *output)
{
	for (size_t i = 0; i < str.size(); i+=2)
	{
		int v;
		std::string sub;
		sub +=str[i];
		sub += str[i + 1];
		sscanf(sub.c_str(), "%02x", &v);
		*output = unsigned char(v);
		output++;
	}
}

void SaveNodes(const std::string &filename, NodeGraphDelegate *delegate)
{
	FILE * fp = fopen(filename.c_str(), "wt");
	if (!fp)
		return;

	char tmps[512];
	int index = 0;
	sprintf(tmps, "%d\n", nodes.size());
	fputs(tmps, fp);
	for (auto& node : nodes)
	{
		sprintf(tmps, "%d[%d,%d]\n", node.mType, int(node.Pos.x), int(node.Pos.y));
		fputs(tmps, fp);
		size_t paramBlockSize;
		unsigned char * paramBlock = delegate->GetParamBlock(index, paramBlockSize);
		fputs((encode(paramBlock, paramBlockSize)+"\n").c_str(), fp);
		index++;
	}
	sprintf(tmps, "%d\n", links.size());
	fputs(tmps, fp);
	for (auto& link : links)
	{
		sprintf(tmps, "%d:%d>%d:%d\n", link.InputIdx, link.InputSlot, link.OutputIdx, link.OutputSlot);
		fputs(tmps, fp);
	}

	fclose(fp);
}

void LoadNodes(const std::string &filename, NodeGraphDelegate *delegate)
{
	FILE * fp = fopen(filename.c_str(), "rt");
	if (!fp)
		return;

	int metaNodeCount;
	const NodeGraphDelegate::MetaNode* metaNodes = delegate->GetMetaNodes(metaNodeCount);

	int count;
	char tmps[512];
	unsigned char decodedBuf[1024];
	fgets(tmps, 512, fp);
	sscanf(tmps, "%d", &count);
	for (int i = 0; i < count; i++)
	{
		int nodeType, px, py;
		fgets(tmps, 512, fp);
		sscanf(tmps, "%d[%d,%d]", &nodeType, &px, &py);

		nodes.push_back(Node(nodeType, ImVec2(float(px), float(py)), metaNodes));
		delegate->AddNode(nodeType);


		fgets(tmps, 512, fp);
		decode(std::string(tmps), decodedBuf);
		delegate->SetParamBlock(i, decodedBuf);
	}
	fgets(tmps, 512, fp);
	sscanf(tmps, "%d", &count);
	for (int i = 0; i < count; i++)
	{
		fgets(tmps, 512, fp);
		NodeLink lnk;
		sscanf(tmps, "%d:%d>%d:%d", &lnk.InputIdx, &lnk.InputSlot, &lnk.OutputIdx, &lnk.OutputSlot);
		links.push_back(lnk);
		delegate->AddLink(lnk.InputIdx, lnk.InputSlot, lnk.OutputIdx, lnk.OutputSlot);
	}

	UpdateEvaluationOrder(delegate);
}


void NodeGraph(NodeGraphDelegate *delegate)
{
	int metaNodeCount;
	const NodeGraphDelegate::MetaNode* metaNodes = delegate->GetMetaNodes(metaNodeCount);

	

	static bool editingNode = false;
	static ImVec2 editingNodeSource;
	static bool editingInput = false;
	static int editingNodeIndex;
	static int editingSlotIndex;


	static ImVec2 scrolling = ImVec2(0.0f, 0.0f);
	static bool show_grid = true;
	static int node_selected = -1;

	int node_hovered_in_list = -1;
	int node_hovered_in_scene = -1;
	bool open_context_menu = false;

	ImGui::BeginGroup();

	const float NODE_SLOT_RADIUS = 8.0f;
	const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

	ImGuiIO& io = ImGui::GetIO();
	// Create our child canvas
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	
	//style.Colors[] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(30, 30, 30, 200));

	//ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(230, 80, 30, 200));


	ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);

	ImVec2 offset = ImGui::GetCursorScreenPos() + scrolling;
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	// Display grid
	if (show_grid)
	{
		ImU32 GRID_COLOR = IM_COL32(100, 100, 100, 40);
		float GRID_SZ = 64.0f;
		ImVec2 win_pos = ImGui::GetCursorScreenPos();
		ImVec2 canvas_sz = ImGui::GetWindowSize();
		for (float x = fmodf(scrolling.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
			draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
		for (float y = fmodf(scrolling.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
			draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);
	}

	// Display links
	draw_list->ChannelsSplit(2);
	draw_list->ChannelsSetCurrent(0); // Background
	for (int link_idx = 0; link_idx < links.Size; link_idx++)
	{
		NodeLink* link = &links[link_idx];
		Node* node_inp = &nodes[link->InputIdx];
		Node* node_out = &nodes[link->OutputIdx];
		ImVec2 p1 = offset + node_inp->GetOutputSlotPos(link->InputSlot);
		ImVec2 p2 = offset + node_out->GetInputSlotPos(link->OutputSlot);
		draw_list->AddBezierCurve(p1, p1 + ImVec2(+50, 0), p2 + ImVec2(-50, 0), p2, IM_COL32(200, 200, 150, 255), 3.0f);
	}

	// edit node
	if (editingNode)
	{
		ImVec2 p1 = editingNodeSource;
		ImVec2 p2 = ImGui::GetIO().MousePos;
		draw_list->AddBezierCurve(p1, p1 + ImVec2(editingInput ?-50.f:+50.f, 0.f), p2 + ImVec2(editingInput ?50.f:-50.f, 0.f), p2, IM_COL32(200, 200, 100, 255), 3.0f);
	}

	int nodeToDelete = -1;
	// Display nodes
	for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
	{
		Node* node = &nodes[node_idx];
		ImGui::PushID(node_idx);
		ImVec2 node_rect_min = offset + node->Pos;

		// Display node contents first
		draw_list->ChannelsSetCurrent(1); // Foreground
		bool old_any_active = ImGui::IsAnyItemActive();
		ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);


		ImGui::InvisibleButton("canvas", ImVec2(100, 100));

		// Save the size of what we have emitted and whether any of the widgets are being used
		bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
		node->Size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
		ImVec2 node_rect_max = node_rect_min + node->Size;

		draw_list->AddRectFilled(node_rect_min, ImVec2(node_rect_max.x, node_rect_min.y + 20), metaNodes[node->mType].mHeaderColor, 2.0f);
		draw_list->AddText(node_rect_min+ImVec2(2,2), IM_COL32(0, 0, 0, 255), metaNodes[node->mType].mName);
		//ImGui::Text("%s [%d]", metaNodes[node->mType].mName, mOrders[node_idx].mNodePriority);
		
		/*
		ImGui::BeginGroup(); // Lock horizontal position
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, 0);
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, 0);
		ImGui::PushStyleColor(ImGuiCol_Border, node_selected?0xFF1040FF:0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2.0f);
		if (ImGui::TreeNodeEx(metaNodes[node->mType].mName))
		{
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
			delegate->EditNode(node_idx);
			ImGui::TreePop();
		}
		else
		{
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
		}
		ImGui::EndGroup();
		*/

		// Display node box
		draw_list->ChannelsSetCurrent(0); // Background
		ImGui::SetCursorScreenPos(node_rect_min);
		ImGui::InvisibleButton("node", node->Size);
		if (ImGui::IsItemHovered())
		{
			node_hovered_in_scene = node_idx;
			open_context_menu |= ImGui::IsMouseClicked(1);
		}
		bool node_moving_active = ImGui::IsItemActive();
		if (node_widgets_active || node_moving_active)
			node_selected = node_idx;
		if (node_moving_active && ImGui::IsMouseDragging(0))
			node->Pos = node->Pos + ImGui::GetIO().MouseDelta;

		bool currentSelectedNode = node_selected == node_idx;

		ImU32 node_bg_color = (node_hovered_in_list == node_idx || node_hovered_in_scene == node_idx || (node_hovered_in_list == -1 && currentSelectedNode)) ? IM_COL32(85, 85, 85, 255) : IM_COL32(60, 60, 60, 255);
		
		if (delegate->mBakeTargetIndex == node_idx)
		{
			//draw_list->AddRect(node_rect_min, node_rect_max, IM_COL32(230, 100, 30, 255) , 2.0f, 15, 4.f);
			draw_list->AddRectFilled(node_rect_min, node_rect_max, IM_COL32(230, 100, 30, 255), 2.0f);
		}
		else
			draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 2.0f);
		draw_list->AddRect(node_rect_min, node_rect_max, currentSelectedNode? IM_COL32(230, 230, 230, 255):IM_COL32(100, 100, 100, 0), 2.0f, 15, 2.f);

		//ImVec2 offsetImg = ImGui::GetCursorScreenPos();

		ImVec2 imgPos = node_rect_min + ImVec2(5, 25);
		ImVec2 imgSize = node_rect_max + ImVec2(-5, -5) - imgPos;
		float imgSizeComp = std::min(imgSize.x, imgSize.y);
		draw_list->AddImage(ImTextureID(delegate->GetNodeTexture(size_t(node_idx))), imgPos, imgPos + ImVec2(imgSizeComp, imgSizeComp));
		// draw/use inputs/outputs
		bool hoverSlot = false;
		for (int i = 0; i < 2; i++)
		{
			int slotCount[2];
			GetConCount(metaNodes, node->mType, slotCount[0], slotCount[1]);
			for (int slot_idx = 0; slot_idx < slotCount[i]; slot_idx++)
			{
				ImVec2 p = offset + (i? node->GetOutputSlotPos(slot_idx):node->GetInputSlotPos(slot_idx));
				bool overCon = !hoverSlot && (node_hovered_in_scene == -1 || editingNode) && Distance(p, ImGui::GetIO().MousePos) < NODE_SLOT_RADIUS*2.f;

				const NodeGraphDelegate::Con *con = i ? metaNodes[node->mType].mOutputs:metaNodes[node->mType].mInputs;
				const char *conText = con[slot_idx].mName;
				ImVec2 textSize;
				textSize = ImGui::CalcTextSize(conText);
				ImVec2 textPos = p + ImVec2(-NODE_SLOT_RADIUS*(i ? -1.f : 1.f)*(overCon ? 3.f : 2.f) - (i ? 0:textSize.x ), -textSize.y / 2);

				if (overCon)
				{
					hoverSlot = true;
					draw_list->AddCircleFilled(p, NODE_SLOT_RADIUS*2.f, IM_COL32(0, 0, 0, 200));
					draw_list->AddCircleFilled(p, NODE_SLOT_RADIUS*1.5f, IM_COL32(200, 200, 200, 200));
					draw_list->AddText(io.FontDefault, 16, textPos+ImVec2(1,1), IM_COL32(0, 0, 0, 255), conText);
					draw_list->AddText(io.FontDefault, 16, textPos, IM_COL32(250, 250, 250, 255), conText);
					bool inputToOutput = (!editingInput && !i) || (editingInput&& i);
					if (editingNode && !ImGui::GetIO().MouseDown[0])
						if (inputToOutput)
						{
							editingNode = false;
							NodeLink nl;
							if (editingInput)
								nl = NodeLink(node_idx, slot_idx, editingNodeIndex, editingSlotIndex);
							else
								nl = NodeLink(editingNodeIndex, editingSlotIndex, node_idx, slot_idx);

							bool alreadyExisting = false;
							for (int linkIndex = 0; linkIndex < links.size(); linkIndex++)
							{
								if (links[linkIndex] == nl)
								{
									alreadyExisting = true;
									break;
								}
							}
							// check already connected output
							for (int linkIndex = 0; linkIndex < links.size(); linkIndex++)
							{
								NodeLink& link = links[linkIndex];
								if (link.OutputIdx == node_idx && link.OutputSlot == slot_idx)
								{
									delegate->DelLink(link.OutputIdx, link.OutputSlot);
									links.erase(links.begin() + linkIndex);
									UpdateEvaluationOrder(delegate);
									break;
								}
							}

							if (!alreadyExisting)
							{
								links.push_back(nl);
								delegate->AddLink(nl.InputIdx, nl.InputSlot, nl.OutputIdx, nl.OutputSlot);
								UpdateEvaluationOrder(delegate);
							}
						}
					if (!editingNode && ImGui::GetIO().MouseDown[0])
					{
						editingNode = true;
						editingInput = i == 0;
						editingNodeSource = p;
						editingNodeIndex = node_idx;
						editingSlotIndex = slot_idx;
						if (editingInput)
						{
							// remove existing link
							for (int linkIndex = 0; linkIndex < links.size(); linkIndex++)
							{
								NodeLink& link = links[linkIndex];
								if (link.OutputIdx == node_idx && link.OutputSlot == slot_idx)
								{
									delegate->DelLink(link.OutputIdx, link.OutputSlot);
									links.erase(links.begin() + linkIndex);
									UpdateEvaluationOrder(delegate);
									break;
								}
							}
						}
					}
				}
				else
				{
					draw_list->AddCircleFilled(p, NODE_SLOT_RADIUS*1.2f, IM_COL32(0, 0, 0, 200));
					draw_list->AddCircleFilled(p, NODE_SLOT_RADIUS*0.75*1.2f, IM_COL32(160, 160, 160, 200));
					draw_list->AddText(io.FontDefault, 14, textPos + ImVec2(2, 2), IM_COL32(0, 0, 0, 255), conText);
					draw_list->AddText(io.FontDefault, 14, textPos, IM_COL32(150, 150, 150, 255), conText);
					
				}
			}
		}
		ImGui::PopID();
	}

	// release mouse anywhere but over a slot
	if (editingNode&&!ImGui::GetIO().MouseDown[0])
	{
		editingNode = false;
	}
	draw_list->ChannelsMerge();

	// Open context menu
	if (!ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && ImGui::IsMouseClicked(1))
	{
		node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
		open_context_menu = true;
	}
	if (open_context_menu)
	{
		ImGui::OpenPopup("context_menu");
		if (node_hovered_in_list != -1)
			node_selected = node_hovered_in_list;
		if (node_hovered_in_scene != -1)
			node_selected = node_hovered_in_scene;
	}

	// Draw context menu
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	if (ImGui::BeginPopup("context_menu"))
	{
		Node* node = node_selected != -1 ? &nodes[node_selected] : NULL;
		ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;
		if (node)
		{
			ImGui::Text(metaNodes[node->mType].mName);
			ImGui::Separator();
			//if (ImGui::MenuItem("Rename..", NULL, false, false)) {}
			if (ImGui::MenuItem("Delete", NULL, false)) 
			{
				if (delegate->mBakeTargetIndex == node_selected)
					delegate->mBakeTargetIndex = -1;

				for (int id = 0; id < links.size(); id++)
				{
					if (links[id].InputIdx == node_selected || links[id].OutputIdx == node_selected)
						delegate->DelLink(links[id].OutputIdx, links[id].OutputSlot);
				}
				auto iter = links.begin();
				for (; iter != links.end();)
				{
					if (iter->InputIdx == node_selected || iter->OutputIdx == node_selected)
						iter = links.erase(iter);
					else
						iter++;
				}
				// recompute link indices
				for (int id = 0; id < links.size(); id++)
				{
					if (links[id].InputIdx > node_selected)
						links[id].InputIdx--;
					if (links[id].OutputIdx > node_selected)
						links[id].OutputIdx--;
				}

				// inform delegate
				delegate->DeleteNode(node_selected);

				// delete links
				nodes.erase(nodes.begin() + node_selected);
				UpdateEvaluationOrder(delegate);
				node_selected = -1;
			}
			if (ImGui::MenuItem("Bake", NULL, false))
			{
				delegate->Bake(node_selected);
			}
			/*
			if (ImGui::MenuItem("Set as target", NULL, false))
			{
				delegate->mBakeTargetIndex = node_selected;
			}
			*/
		}
		else
		{
			static char inputText[64] = { 0 };
			ImGui::InputText("", inputText, sizeof(inputText));
			{ 
				//nodes.push_back(Node(nodes.Size, "New node", scene_pos, 0.5f, ImColor(100, 100, 200), 2, 2)); 
				for (int i = 0; i < metaNodeCount; i++)
				{
					const char *nodeName = metaNodes[i].mName;
					bool displayNode = !strlen(inputText) || ImStristr(nodeName, nodeName + strlen(nodeName), inputText, inputText + strlen(inputText));
					if (displayNode && ImGui::MenuItem(nodeName, NULL, false))
					{
						nodes.push_back(Node(i, scene_pos, metaNodes));
						delegate->AddNode(i);
						UpdateEvaluationOrder(delegate);
						node_selected = nodes.size() - 1;
					}
				}
				//ImGui::EndMenu();
			}
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();

	// Scrolling
	if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
		scrolling = scrolling + ImGui::GetIO().MouseDelta;

	ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::PopStyleColor(1);
	ImGui::PopStyleVar(2);

	delegate->mSelectedNodeIndex = node_selected;
	
	ImGui::EndGroup();
}