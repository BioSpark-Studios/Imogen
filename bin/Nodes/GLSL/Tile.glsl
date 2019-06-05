layout (std140) uniform TileBlock
{
	vec2 offset0;
	vec2 offset1;
	vec2 overlap;
	float scale;
} TileParam;

vec4 GetTile0(vec2 uv)
{
    if (uv.x > 1.0 || uv.y > 1.0 || uv.x <0.0 || uv.y<0.0)
        return vec4(0.0);
    
    return texture(Sampler0, uv);
}

vec2 GetOffset(vec2 uv)
{
	float quincunxY = float(int(floor(uv.y))&1);
	float quincunxX = float(int(floor(uv.x))&1);
	float oX = mix(TileParam.offset0.x, TileParam.offset1.x, quincunxY);
	float oY = mix(TileParam.offset0.y, TileParam.offset1.y, quincunxX);
	return vec2(oX,oY);
}

vec4 GetTile(vec2 uv)
{
	vec2 cellSize = vec2(1.0) - TileParam.overlap;

	vec4 c = vec4(0.0);

	for (int y = -1;y<2;y++)
	{
		for (int x = -1;x<2;x++)
		{
			vec2 cell0 = uv - (fract(uv/cellSize) + vec2(float(x), float(y))) * cellSize;
			vec2 cell1 = (floor(uv/cellSize) + vec2(float(x), float(y)));
			vec2 ofs = GetOffset(cell1);
			vec4 multiplier = vec4(1.0);
			if (EvaluationParam.inputIndices[0].y > -1)
			{
				vec2 uvMul = (floor(uv + ofs) + vec2(0.5))/ TileParam.scale - (ofs*vec2(0.5))/ TileParam.scale;
				//return vec4(uvMul, 0., 1.); // debug!
				multiplier = texture(Sampler1, uvMul);
			}
			c += GetTile0(uv - cell0 + ofs) * vec4(multiplier.xyz, 1.0);
		}
	}

	return c;
}

vec4 Tile()
{
	vec2 nuv = vUV * TileParam.scale;
	return GetTile(nuv);
}