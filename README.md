# Chimia lib




## Features

- [Graphics] Fog;
- [Graphics] Morph target animation;
  - [Rendering] VAO with multi buffer shader bindings;
- [Graphics] Transparency rework
  - No need to have different renderers;
  - Needs to do a two pass rendering still, as it's done currently;
  - Adjust the shader for handling both behaviors;
- [Core] Logging system
  - Include error level support;
- [General] Testing tooling
  - [Core] Printing things to a log;
  - [Utils] Comparing images + Comparing text files; 
- [Core] Create class for representing a model/mesh;
- [Graphics] Post processing effects: HDR, bloom, anti aliasing;
- [Graphics] Texture filtering; 

## Tech depts

- [Extras|Rendering] Move GetPixels functionality to Rendering module
- [Graphics] Include functions for adding/removing lights from the scene. See TODOs at Illumination.cpp
- [Graphics] Review object table structure. The initial size and growth factor should be configurable. See TODO at ObjectTable.h
- [Graphics] Buffers implementation code on Rendering module is quite messy.
- [Graphics] Model is not a good name for class Model;
- [Graphics] Remove tag from resources, and also store them in simple varianges, no containers needed;
- [General] Review all codebase and add diagnostics (error reporting, warnings);
- [General] update readme.
  - Choose a license for the project;
  - Create documentation for the architecture;
- [Rendering] The rendering module samples seem to be with wrong triangle winding. Try enabling depth test;
- [Tests] Reduce artifacts for retained triangles test (steps 2 and 3 aren't really needed);
- [Tests] Reduce artifacts for batching test (doesn't really need to test all vertex layouts);


## Transparency open problems:
- Flush by demand;
  - Include transparent pipeline setting wrap in shader configuration steps;
- Transparency is not enabled in models currently; (Will get resolved by the rework feature);

## Build Instructions
Run `cppm build` to build the project.

