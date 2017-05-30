# ballin engine

## TODO
A whole lotta TODO

## Plans
* GL 3.3 CORE + maybe newer GL features if the card supports it
* BSP maps? (similar to quake 3 or something)
* Lightmaps + dynamic lightmap shadows
* Realtime dynamic lighting and shadows
* Forward+ sorta thing
* ODE physics
* Networked multiplayer
* MSAA
* VERY high framerate (400fps + on a modern card, will run at 60+ on a 8800gt while still looking good)

## Maybes
* Opengl 4.whatever advanced features (multi draw indirect, compute shaders, etc)
* Reflections
* PBR
* SSAO (or SSAO-like effect like SSDO), must be clean and not farcry3
* Fancy audio
* Motion blur (inter-frame, so its not making your expensive 144hz+ monitor a waste)

## Stuff im not planning for
The game artstyle that will be running on this engine is "clean and colorful", so that is the sort of effects im implementing
* Volumetrics
* Loads of alpha blended particles
* Loads of decals
* Really any CPU intensive effects
* VR

## Libraries used (So far)
* Darkplaces Matrixlib (some modifications)
* LibEpoxy (replaced my usual GLEW usage)
* GLFW
