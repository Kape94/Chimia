# Chimia lib




## Features

- [Graphics] Fog;
- [Graphics] Morph target animation;
- [Core] Logging system
  - Include error level support;
- [General] Testing tooling
  - [Core] Printing things to a log;
  - [Utils] Comparing text files; 
- [Core] Create class for representing a model/mesh;
- [Graphics] Post processing effects: HDR, bloom, anti aliasing;
- [Graphics] Texture filtering; 
- [Rendering] Stateless API: Remove enable/disable functions (e.g depthTest, colorBlending)
  * This enable/disable mechanic should be replaced by the concept of a pipeline object;

## Tech depts

- [Extras|Rendering] Move GetPixels functionality to Rendering module
- [Graphics] Include functions for adding/removing lights from the scene. See TODOs at Illumination.cpp
- [Graphics] Review object table structure. The initial size and growth factor should be configurable. See TODO at ObjectTable.h
- [Graphics] All shader configuration functions from all renderers now are looking quite similar. Should them be dropped?
- [Graphics] Remove m_onRender functions from inside batching components, this should be coordinated externally.
- [Rendering] Buffers implementation code on Rendering module is quite messy.
- [Graphics] Model is not a good name for class Model;
- [Graphics] Remove tag from resources, and also store them in simple varianges, no containers needed;
- [Graphics] Re-architect the code around the concept of pipeline;
  - A pipeline should be able to receive a set of renderers, or even a single small component, and draw it;
  - The thing that is currently called "Pipeline" is not really a pipeline, it should be called "RenderingConfiguration", or something alike;
  - The pipeline then should be an object that could apply the rendering configs and orchestrate the rendering process;
    - ex: a full pipeline (supports transparency), could do the flow of setting api states for opaque draw, then flushing the renderers,
        then setting api state for blended draw, then re-flushing the renderers;
       
- [General] Review all codebase and add diagnostics (error reporting, warnings);
- [General] update readme.
  - Choose a license for the project;
  - Create documentation for the architecture;
- [Rendering] The rendering module samples seem to be with wrong triangle winding. Try enabling depth test;
- [Rendering] Texture binding is being done inside shader usage, it would be good to have it done inside RenderAction;
- [Rendering] The static object creation functions should be all centered at Chimia::Rendering;
- [Rendering] We should maybe swap the classes names? ShaderInstance should be called Shader, and the Shader could be called ShaderInstance or ShaderObject?
- [Tests][Rendering] Add test coverage for: 
  - data resize;
  - Multiple data bindings;
  - RenderData retargeting;
  - Different primitives;
- [Tests] Reduce artifacts for retained triangles test (steps 2 and 3 aren't really needed);
- [Tests] Reduce artifacts for batching test (doesn't really need to test all vertex layouts);


## Open problems:
- Containers/buffers expansion rate;
  - Performance problem arised while drawing many triangles in the first frame (many input buffer resizes);
- Adding many small retained triangle meshes is not showing good performance. A single huge mesh performs far better;

## Build Instructions
Run `cppm build` to build the project.

