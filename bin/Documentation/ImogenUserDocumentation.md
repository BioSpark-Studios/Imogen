# Imogen - User documentation


1. [Nodes](#Nodes)
1. [Default Hot Keys](#Default-Hot-Keys)


# Nodes
## Generator
![node picture](Pictures/Circle.png)|![node picture](Pictures/Checker.png)|![node picture](Pictures/Sine.png)|![node picture](Pictures/CircleSplatter.png)|![node picture](Pictures/NGon.png)|![node picture](Pictures/GradientBuilder.png)
-|-|-|-|-|-
[Circle](#Circle)|[Checker](#Checker)|[Sine](#Sine)|[CircleSplatter](#CircleSplatter)|[NGon](#NGon)|[GradientBuilder](#GradientBuilder)
![node picture](Pictures/ReactionDiffusion.png)|![node picture](Pictures/Disolve.png)||||
[ReactionDiffusion](#ReactionDiffusion)|[Disolve](#Disolve)||||


## Transform
![node picture](Pictures/Transform.png)|![node picture](Pictures/Pixelize.png)|![node picture](Pictures/Tile.png)|![node picture](Pictures/PolarCoords.png)|![node picture](Pictures/Swirl.png)|![node picture](Pictures/Crop.png)
-|-|-|-|-|-
[Transform](#Transform)|[Pixelize](#Pixelize)|[Tile](#Tile)|[PolarCoords](#PolarCoords)|[Swirl](#Swirl)|[Crop](#Crop)
![node picture](Pictures/Warp.png)|![node picture](Pictures/EdgeDetect.png)|![node picture](Pictures/Kaleidoscope.png)|![node picture](Pictures/Palette.png)|![node picture](Pictures/ChannelPacker.png)|![node picture](Pictures/Lens.png)
[Warp](#Warp)|[EdgeDetect](#EdgeDetect)|[Kaleidoscope](#Kaleidoscope)|[Palette](#Palette)|[ChannelPacker](#ChannelPacker)|[Lens](#Lens)
![node picture](Pictures/Distance.png)|||||
[Distance](#Distance)|||||


## Filter
![node picture](Pictures/SmoothStep.png)|![node picture](Pictures/Blur.png)|![node picture](Pictures/NormalMap.png)|![node picture](Pictures/Invert.png)|![node picture](Pictures/Ramp.png)|![node picture](Pictures/Clamp.png)
-|-|-|-|-|-
[SmoothStep](#SmoothStep)|[Blur](#Blur)|[NormalMap](#NormalMap)|[Invert](#Invert)|[Ramp](#Ramp)|[Clamp](#Clamp)
![node picture](Pictures/AO.png)|||||
[AO](#AO)|||||


## Material
![node picture](Pictures/LambertMaterial.png)|![node picture](Pictures/PBR.png)|![node picture](Pictures/TerrainPreview.png)|![node picture](Pictures/PathTracer.png)|![node picture](Pictures/PBR2.png)
-|-|-|-|-
[LambertMaterial](#LambertMaterial)|[PBR](#PBR)|[TerrainPreview](#TerrainPreview)|[PathTracer](#PathTracer)|[PBR2](#PBR2)


## Blend
![node picture](Pictures/MADD.png)|![node picture](Pictures/Blend.png)|![node picture](Pictures/NormalMapBlending.png)
-|-|-
[MADD](#MADD)|[Blend](#Blend)|[NormalMapBlending](#NormalMapBlending)


## None
![node picture](Pictures/Color.png)
-
[Color](#Color)


## Noise
![node picture](Pictures/iqnoise.png)|![node picture](Pictures/PerlinNoise.png)|![node picture](Pictures/Voronoi.png)
-|-|-
[iqnoise](#iqnoise)|[PerlinNoise](#PerlinNoise)|[Voronoi](#Voronoi)


## File
![node picture](Pictures/ImageRead.png)|![node picture](Pictures/ImageWrite.png)|![node picture](Pictures/Thumbnail.png)|![node picture](Pictures/SVG.png)|![node picture](Pictures/SceneLoader.png)|![node picture](Pictures/GLTFRead.png)
-|-|-|-|-|-
[ImageRead](#ImageRead)|[ImageWrite](#ImageWrite)|[Thumbnail](#Thumbnail)|[SVG](#SVG)|[SceneLoader](#SceneLoader)|[GLTFRead](#GLTFRead)


## Paint
![node picture](Pictures/Paint2D.png)|![node picture](Pictures/Paint3D.png)
-|-
[Paint2D](#Paint2D)|[Paint3D](#Paint3D)


## Cubemap
![node picture](Pictures/PhysicalSky.png)|![node picture](Pictures/CubemapView.png)|![node picture](Pictures/EquirectConverter.png)|![node picture](Pictures/CubeRadiance.png)
-|-|-|-
[PhysicalSky](#PhysicalSky)|[CubemapView](#CubemapView)|[EquirectConverter](#EquirectConverter)|[CubeRadiance](#CubeRadiance)


## Fur
![node picture](Pictures/FurGenerator.png)|![node picture](Pictures/FurDisplay.png)|![node picture](Pictures/FurIntegrator.png)
-|-|-
[FurGenerator](#FurGenerator)|[FurDisplay](#FurDisplay)|[FurIntegrator](#FurIntegrator)


## Circle
![node picture](Pictures/Circle.png)

Category : Generator
### Description
Renders a perfect Circle center in the view port.
### Parameters
1. Radius
Clip-space radius.
1. T
Interpolation factor between full white cirle (0.) and height of the hemisphere (1.).

### Example
![node example](Examples/Example_Circle.png)

## Transform
![node picture](Pictures/Transform.png)

Category : Transform
### Description
Transform every source texel using the translation, rotation, scale.
### Parameters
1. Translate
2D vector translation.
1. Scale
2D vector scale.
1. Rotation
Angle in degrees. Center of rotation is the center of the source.

### Example
![node example](Examples/Example_Transform.png)

## Checker
![node picture](Pictures/Checker.png)

Category : Generator
### Description
Renders a 4 square black and white checker. Use a Transform node to scale it to any number of squares.
### Parameters
No parameter for this node.

### Example
![node example](Examples/Example_Checker.png)

## Sine
![node picture](Pictures/Sine.png)

Category : Generator
### Description
Renders a one directioned sine as a greyscale value.
### Parameters
1. Frequency
Basically, the number of bars.
1. Angle
Angle in degrees of the so called bars.

### Example
![node example](Examples/Example_Sine.png)

## SmoothStep
![node picture](Pictures/SmoothStep.png)

Category : Filter
### Description
Performs a smoothstep operation. Hermite interpolation between 0 and 1 when Low < x < high. This is useful in cases where a threshold function with a smooth transition is desired.
### Parameters
1. Low
Lower value for the Hermite interpolation.
1. High
Higher value for the Hermite interpolation. Result is undertimined when high value < low value.

### Example
![node example](Examples/Example_SmoothStep.png)

## Pixelize
![node picture](Pictures/Pixelize.png)

Category : Transform
### Description
Lower the resolution of the image using nearest filter.
### Parameters
1. scale
Number of pixels on a side.

### Example
![node example](Examples/Example_Pixelize.png)

## Blur
![node picture](Pictures/Blur.png)

Category : Filter
### Description
Performs a Directional of Box blur filter. Directional blur is a gaussian pass with 16 pixels. Box is 16x16.
### Parameters
1. Type
Selection of the Blur type.
1. angle
angle in degrees for the directional blur.
1. strength
Defines how wide the blur pass will be. The bigger, the larger area each pixel will cover.
1. passCount
Multiple passes are supported by this node.

### Example
![node example](Examples/Example_Blur.png)

## NormalMap
![node picture](Pictures/NormalMap.png)

Category : Filter
### Description
Computes a normal map using the Red component of the source as the height.
### Parameters
1. spread
The bigger, the stronger the resulting normal will be.
1. Invert
Change the direction of the XY components of the normal.

### Example
![node example](Examples/Example_NormalMap.png)

## LambertMaterial
![node picture](Pictures/LambertMaterial.png)

Category : Material
### Description
simental node.
### Parameters
1. view


### Example
![node example](Examples/Example_LambertMaterial.png)

## MADD
![node picture](Pictures/MADD.png)

Category : Blend
### Description
For each source texel, multiply and and a color value.
### Parameters
1. Mul Color
The color to multiply the source with.
1. Add Color
The color to add to the source.

### Example
![node example](Examples/Example_MADD.png)

## Blend
![node picture](Pictures/Blend.png)

Category : Blend
### Description
Blends to source together using a built-in operation. Each source can also be masked and multiplied by a value.
### Parameters
1. A
Color/mask to multiply A source with.
1. B
Color/mask to multiply A source with.
1. Operation
Built-ins operation used for blending. Check the examples below.

![node picture](Examples/Example_Blend_Add.png)|![node picture](Examples/Example_Blend_Multiply.png)|![node picture](Examples/Example_Blend_Darken.png)
-|-|-
Mode Add|Mode Multiply|Mode Darken
![node picture](Examples/Example_Blend_Lighten.png)|![node picture](Examples/Example_Blend_Average.png)|![node picture](Examples/Example_Blend_Screen.png)
Mode Lighten|Mode Average|Mode Screen
![node picture](Examples/Example_Blend_Color_Burn.png)|![node picture](Examples/Example_Blend_Color_Dodge.png)|![node picture](Examples/Example_Blend_Soft_Light.png)
Mode Color Burn|Mode Color Dodge|Mode Soft Light
![node picture](Examples/Example_Blend_Subtract.png)|![node picture](Examples/Example_Blend_Difference.png)|![node picture](Examples/Example_Blend_Inverse_Difference.png)
Mode Subtract|Mode Difference|Mode Inverse Difference
![node picture](Examples/Example_Blend_Exclusion.png)||
Mode Exclusion||


## Invert
![node picture](Pictures/Invert.png)

Category : Filter
### Description
Performs a simple color inversion for each component. Basically, for R source value, outputs 1.0 - R.
### Parameters
No parameter for this node.

### Example
![node example](Examples/Example_Invert.png)

## CircleSplatter
![node picture](Pictures/CircleSplatter.png)

Category : Generator
### Description
Renders a bunch of circle with interpolated position and scales. For N circles the interpolation coefficient will be between [0/N....N/N].
### Parameters
1. Distance
First and Last value to interpolate between for the distance to the center of the viewport.
1. Radius
First and Last value for the interpolated circle radius.
1. Angle
First and Last value for the Angle. The circle position is computed using the angle and the distance.
1. Count
The total number of circles to render.

### Example
![node example](Examples/Example_CircleSplatter.png)

## Ramp
![node picture](Pictures/Ramp.png)

Category : Filter
### Description
Performs a Ramp on the source components. For each source value (X coord on the ramp graph), retrieve an intensity value (Y on the ramp graph). Optionnaly use an image instead of the editable graph.
### Parameters
1. Ramp
Graph that can be edited.

### Example
![node example](Examples/Example_Ramp.png)

## Tile
![node picture](Pictures/Tile.png)

Category : Transform
### Description
Generate a tile map of the source image. With optional overlap. An optional Color input can be used to modulate the tiles color. Color is uniform per tile and picked at its center in the output image.
### Parameters
1. Offset 0
X,Y offset applied to even tiles. Offset in clipspace.
1. Offset 1
X,Y offset applied to odd tiles. Offset in clipspace.
1. Overlap
Amount of overlap between odd and even tiles.
1. Scale
The number of tiles in X and Y in the output.

### Example
![node example](Examples/Example_Tile.png)

## Color
![node picture](Pictures/Color.png)

Category : None
### Description
Single plain color.
### Parameters
1. Color
Single plain color.

### Example
![node example](Examples/Example_Color.png)

## NormalMapBlending
![node picture](Pictures/NormalMapBlending.png)

Category : Blend
### Description
Blend two normal maps into a single one. Choose the Technique that gives the best result.
### Parameters
1. Technique
Different techniques for blending. Check the examples below to see the differences.

### Example
![node example](Examples/Example_NormalMapBlending.png)

## iqnoise
![node picture](Pictures/iqnoise.png)

Category : Noise
### Description
Generate noise based on work by Inigo Quilez.
### Parameters
1. Translation
Translate the noise seeds so you can have virtualy infinite different noise.
1. Size

1. U
Interpolate between centered seed (checker) to jittered random position.
1. V
Interpolation factor between full color to distance-like color per cell.

### Example
![node example](Examples/Example_iqnoise.png)

## PerlinNoise
![node picture](Pictures/PerlinNoise.png)

Category : Noise
### Description

### Parameters
1. Translation
Translate the noise seeds so you can have virtualy infinite different noise.
1. Octaves
The number of noise with different scale for each.
1. lacunarity
Geoemtric scale applied for each octave.
1. gain
Intensity factor applied to each octave.

### Example
![node example](Examples/Example_PerlinNoise.png)

## PBR
![node picture](Pictures/PBR.png)

Category : Material
### Description
Experimental Node.
### Parameters
1. View

1. Displacement Factor

1. Geometry


### Example
![node example](Examples/Example_PBR.png)

## PolarCoords
![node picture](Pictures/PolarCoords.png)

Category : Transform
### Description
Transform the source using polar coordinates.
### Parameters
1. Type
Change to direction of the transformation.

### Example
![node example](Examples/Example_PolarCoords.png)

## Clamp
![node picture](Pictures/Clamp.png)

Category : Filter
### Description
Performs a clamp for each component of the source. Basically, sets the min and max of each component.
### Parameters
1. Min
The minimal value for each source component.
1. Max
The maximal value for each source component.

### Example
![node example](Examples/Example_Clamp.png)

## ImageRead
![node picture](Pictures/ImageRead.png)

Category : File
### Description
Imports a file from the disk. Major formats are supported. Cubemaps can be imported using one image for each face of using a cubemap .DDS/.KTX. MP4 movies can be read as well.
### Parameters
1. File name
Single image frame or DDS/KTX cubemap.
1. +X File name
Cubemap image face for +X direction.
1. -X File name
Cubemap image face for -X direction.
1. +Y File name
Cubemap image face for +Y direction (Top).
1. -Y File name
Cubemap image face for -Y direction (Bottom).
1. +Z File name
Cubemap image face for +Z direction.
1. -Z File name
Cubemap image face for -Z direction.

### Example
![node example](Examples/Example_ImageRead.png)

## ImageWrite
![node picture](Pictures/ImageWrite.png)

Category : File
### Description

### Parameters
1. File name

1. Format

1. Quality

1. Width

1. Height

1. Mode

1. Export


### Example
![node example](Examples/Example_ImageWrite.png)

## Thumbnail
![node picture](Pictures/Thumbnail.png)

Category : File
### Description
Create a thumbnail picture from the source input and applies it to the thumbnail library view.
### Parameters
1. Make
Force the evaluation without building the graph.

### Example
![node example](Examples/Example_Thumbnail.png)

## Paint2D
![node picture](Pictures/Paint2D.png)

Category : Paint
### Description
Paint in the parameter viewport using the connected brush. Paint picture is saved in the graph.
### Parameters
1. Size
Size of the output in pixels.

### Example
![node example](Examples/Example_Paint2D.png)

## Swirl
![node picture](Pictures/Swirl.png)

Category : Transform
### Description
Performs a rotation on source based on distance to the viewport center.
### Parameters
1. Angles
Rotation for the inner pixels (closer to the center) and the outter pixels. Angles in degrees.

### Example
![node example](Examples/Example_Swirl.png)

## Crop
![node picture](Pictures/Crop.png)

Category : Transform
### Description
Set the output as a rectangle in the source.
### Parameters
1. Quad
X/Y Position and width/height of the selection rectangle.

### Example
![node example](Examples/Example_Crop.png)

## PhysicalSky
![node picture](Pictures/PhysicalSky.png)

Category : Cubemap
### Description
Generate a physical sky cubemap.
### Parameters
1. ambient
Ambient aka minimal color within the cubemap. Ambient alpha is used to modulate it with the sky.
1. lightdir
Light direction vector.
1. Kr
Kr component value.
1. rayleigh brightness
Rayleigh brightness
1. mie brightness
Mie brightness factor.
1. spot brightness
Spot/sun brightness.
1. scatter strength
Scatter strength.
1. rayleigh strength
Rayleigh strength.
1. mie strength
Mie strength.
1. rayleigh collection power
Rayleigh collection power.
1. mie collection power
Mie collection power.
1. mie distribution
Mie distribution.
1. Size
Size of the cubemap face width in pixels.

### Example
![node example](Examples/Example_PhysicalSky.png)

## CubemapView
![node picture](Pictures/CubemapView.png)

Category : Cubemap
### Description
Used to display a cubemap using various techniques.
### Parameters
1. view
Used by the Camera technique to rotate the eye direction.
1. Mode
Techniques for display. See below for examples.
1. LOD
Use a particular LOD (mipmap) for display. Radiance cube node can help produce cubemap mipmaps.

### Example
![node example](Examples/Example_CubemapView.png)

## EquirectConverter
![node picture](Pictures/EquirectConverter.png)

Category : Cubemap
### Description
Converts an equirect source (one single picture containing all environment) into a cubemap output. The inverse (cubemap -> equirect) can also be performed with this node.
### Parameters
1. Mode
Select to operation to perform.
1. Size
Size fo the ouput in pixels.

### Example
![node example](Examples/Example_EquirectConverter.png)

## NGon
![node picture](Pictures/NGon.png)

Category : Generator
### Description
Compute N plans and color the texels behind every plan accordingly. Texels in front of any plan will be black.
### Parameters
1. Sides
Number of uniformly distributed plans.
1. Radius
Distance from the plan to the center of the viewport.
1. T
Interpolation factor between full white color and distance to the nearest plan.

### Example
![node example](Examples/Example_NGon.png)

## GradientBuilder
![node picture](Pictures/GradientBuilder.png)

Category : Generator
### Description
Computes a linear gradient based on key values(position/color).
### Parameters
1. Gradient
Double click to add a click. Click and drag a key to move it position. Modify the color for the selected key.

### Example
![node example](Examples/Example_GradientBuilder.png)

## Warp
![node picture](Pictures/Warp.png)

Category : Transform
### Description
Displace each source texel using the Warp input.
### Parameters
1. Strength
How strong is the displacement. Clip-space value.
1. Mode
One of 2 modes. XY offset : R and G channels are used for X and Y displacement. Rotation-Distance: R is used as an angle (0..1 -> 0..2pi) and G is the length of the displacement.

### Example
![node example](Examples/Example_Warp.png)

## TerrainPreview
![node picture](Pictures/TerrainPreview.png)

Category : Material
### Description
Experimental node.
### Parameters
1. Camera


### Example
![node example](Examples/Example_TerrainPreview.png)

## AO
![node picture](Pictures/AO.png)

Category : Filter
### Description
Compute ambient occlusion based on an input heightmap.
### Parameters
1. strength
Strength of the occlusion. The higher value, the stronger dark you'll get.
1. area
Area size used to compute the AO. The higher value, the bigger area.
1. falloff
How much each sample influence the AO.
1. radius
Radius in clipspace used for the computation.

### Example
![node example](Examples/Example_AO.png)

## FurGenerator
![node picture](Pictures/FurGenerator.png)

Category : Fur
### Description
Experimental node.
### Parameters
1. Hair count

1. Length factor


### Example
![node example](Examples/Example_FurGenerator.png)

## FurDisplay
![node picture](Pictures/FurDisplay.png)

Category : Fur
### Description
Experimental node.
### Parameters
1. Camera


### Example
![node example](Examples/Example_FurDisplay.png)

## FurIntegrator
![node picture](Pictures/FurIntegrator.png)

Category : Fur
### Description
Experimental node.
### Parameters
No parameter for this node.

### Example
![node example](Examples/Example_FurIntegrator.png)

## SVG
![node picture](Pictures/SVG.png)

Category : File
### Description
Import an SVG vector graphics image and rasterize it to an image output.
### Parameters
1. File name
Relative or absolute filepath of the SVG file.
1. DPI
Resolution used for rendering the SVG. The higher value, the bigger the image will be.

### Example
![node example](Examples/Example_SVG.png)

## SceneLoader
![node picture](Pictures/SceneLoader.png)

Category : File
### Description
Experimental node.
### Parameters
1. File name


### Example
![node example](Examples/Example_SceneLoader.png)

## PathTracer
![node picture](Pictures/PathTracer.png)

Category : Material
### Description
Experimental node.
### Parameters
1. Mode

1. Camera


### Example
![node example](Examples/Example_PathTracer.png)

## EdgeDetect
![node picture](Pictures/EdgeDetect.png)

Category : Transform
### Description
Performs an edge detection on the source. Texels that are close in intensity with the neighbours will be white. Black if the difference is strong.
### Parameters
1. Radius
The radius size in clipspace used for detection. The higher value, the broader the search is.

### Example
![node example](Examples/Example_EdgeDetect.png)

## Voronoi
![node picture](Pictures/Voronoi.png)

Category : Noise
### Description
Generates a Voronoi texture based on random seeds.
### Parameters
1. Point Count
The number of seeds.
1. Seed
The seeds random position base value.
1. Distance Blend
Distance computation type interpolate between Euclydean distance (0.) and Manhattan Distance (1.)
1. Square Width
Size of each seed in clipspace size.

### Example
![node example](Examples/Example_Voronoi.png)

## Kaleidoscope
![node picture](Pictures/Kaleidoscope.png)

Category : Transform
### Description
Duplicates portion of the source using rotation and symetry. Basically, computes N plans and duplicate what's in front of the plane to the other with or without symetry.
### Parameters
1. Center
Center of the operation in clipspace coordinates [0..1]
1. Start Angle
Angle in degrees of the first plan. Total sum of plan angle is 360 deg.
1. Splits
How many split plans to use.
1. Symetry
Enable symetry for even plans.

### Example
![node example](Examples/Example_Kaleidoscope.png)

## Palette
![node picture](Pictures/Palette.png)

Category : Transform
### Description
Find the closest color inside the predefined palette for each texel in the source. Using optional dithering.
### Parameters
1. Palette
Predefined palette. Check examples below for results.
1. Dither Strength
Bayer dithering strength (0 = none, 1 = full dither).

![node picture](Examples/Example_Palette_CGA0.png)|![node picture](Examples/Example_Palette_CGA1.png)
-|-
Mode CGA0|Mode CGA1
![node picture](Examples/Example_Palette_CGA2.png)|![node picture](Examples/Example_Palette_CGA3.png)
Mode CGA2|Mode CGA3
![node picture](Examples/Example_Palette_CGA4.png)|![node picture](Examples/Example_Palette_CGA5.png)
Mode CGA4|Mode CGA5
![node picture](Examples/Example_Palette_EGA.png)|![node picture](Examples/Example_Palette_Gameboy(mono).png)
Mode EGA|Mode Gameboy(mono)
![node picture](Examples/Example_Palette_PICO-8.png)|![node picture](Examples/Example_Palette_C64.png)
Mode PICO-8|Mode C64


## ReactionDiffusion
![node picture](Pictures/ReactionDiffusion.png)

Category : Generator
### Description
Use multipass to compute a Reaction Diffusion generative synthesis from a source image. Pass count must be a multiple of 3. First 2 passes are used to blur the image.
### Parameters
1. boost
Component boost
1. divisor
Reaction divisor.
1. colorStep
Component color step.
1. passCount
Multiple passes are supported by this node.
1. Size
Size of the output in pixels

### Example
![node example](Examples/Example_ReactionDiffusion.png)

## Disolve
![node picture](Pictures/Disolve.png)

Category : Generator
### Description
Apply a fluid dynamic-like process on the source image. A noise is computed and is used to displace the source.
### Parameters
1. passCount
Multiple passes are supported by this node.
1. Frequency
Noise frequency. The higher, the more noise you'll get.
1. Strength
Noise strength.
1. Randomization
How much randomization is applied.
1. VerticalShift
How much in clip-space is moved to the top. Somekind of force applied to texels.

### Example
![node example](Examples/Example_Disolve.png)

## GLTFRead
![node picture](Pictures/GLTFRead.png)

Category : File
### Description
Experimental node.
### Parameters
1. File name

1. Camera


### Example
![node example](Examples/Example_GLTFRead.png)

## Paint3D
![node picture](Pictures/Paint3D.png)

Category : Paint
### Description
Experimental node.
### Parameters
1. Size

1. Camera


### Example
![node example](Examples/Example_Paint3D.png)

## CubeRadiance
![node picture](Pictures/CubeRadiance.png)

Category : Cubemap
### Description
Compute cubemap radiance or irradiance and generate a cubemap with optional mipmaps
### Parameters
1. Mode
Radiance or Irradiance.
1. Size
Size of the output cubemap face width in pixels.
1. Sample Count
The samples count used for the blur. The more samplem the higher quality.

### Example
![node example](Examples/Example_CubeRadiance.png)

## PBR2
![node picture](Pictures/PBR2.png)

Category : Material
### Description
Experimental node.
### Parameters
1. View

1. Depth factor


### Example
![node example](Examples/Example_PBR2.png)

## ChannelPacker
![node picture](Pictures/ChannelPacker.png)

Category : Transform
### Description
Pack channels from 1 to 4 inputs into a new output. This can be used to pack normal/roughness/metallic into a 4 channels image.
### Parameters
1. R
The source image and its channel used for Red in the output.
1. G
The source image and its channel used for Green in the output.
1. B
The source image and its channel used for Blue in the output.
1. A
The source image and its channel used for Alpha in the output.

### Example
![node example](Examples/Example_ChannelPacker.png)

## Lens
![node picture](Pictures/Lens.png)

Category : Transform
### Description
Deforms the source using a lens computation with an optionnal vignette shading.
### Parameters
1. Factor
Lens factor strength.
1. Vignette
Vignette shading: Corners get darker. 0 to disable vignette.

### Example
![node example](Examples/Example_Lens.png)

## Distance
![node picture](Pictures/Distance.png)

Category : Transform
### Description
Generate a signed distance field from a black and white source image.
### Parameters
1. passCount


### Example
![node example](Examples/Example_Distance.png)

# Default Hot Keys

Action|Description|Hot key
-|-|-
Layout|Reorder nodes in a simpler layout|Ctrl + L
PlayPause|Play or Stop current animation|F5
AnimationFirstFrame|Set current time to the first frame of animation|V
AnimationNextFrame|Move to the next animation frame|N
AnimationPreviousFrame|Move to previous animation frame|B
MaterialExport|Export current material to a file|Ctrl + E
MaterialImport|Import a material file in the library|Ctrl + I
ToggleLibrary|Show or hide Libaray window|Ctrl + 1
ToggleNodeGraph|Show or hide Node graph window|Ctrl + 2
ToggleLogger|Show or hide Logger window|Ctrl + 3
ToggleSequencer|Show or hide Sequencer window|Ctrl + 4
ToggleParameters|Show or hide Parameters window|Ctrl + 5
MaterialNew|Create a new graph|Ctrl + N
ReloadShaders|Reload them|F7
DeleteSelectedNodes|Delete selected nodes in the current graph|Del
AnimationSetKey|Make a new animation key with the current parameters values at the current time|S
HotKeyEditor|Open the Hotkey editor window|Ctrl + K
NewNodePopup|Open the new node menu|Tab
Undo|Undo the last operation|Ctrl + Z
Redo|Redo the last undo|Ctrl + Shift +Z
Copy|Copy the selected nodes|Ctrl + C
Cut|Cut the selected nodes|Ctrl + X
Paste|Paste previously copy/cut nodes|Ctrl + V
BuildMaterial|Build current material|Ctrl + B
MouseState|Show Mouse State as a tooltip|Ctrl + M

