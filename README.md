# Chimia lib

TODO: update readme
TODO: Buffers implementation code on Rendering module is quite messy.
TODO: Model is not a good name for class Model;
TODO: Search TODO comments on code;  
TODO: Missing features
  - Post processing effects: HDR, bloom, anti aliasing;
  - Fog;
  - Shadowing and per vertex color shadowing;
  - Texture filtering; 

Transparency open problems:
- Flush by demand;
  - Include transparent pipeline setting wrap in shader configuration steps;
- Mixture color with alpha. Changing the mixture color from opaque to transparent might imply in moving batches from one renderer to another;
  - Allow only opaque mixture color, and then provide a transparency number as a separate resource;
- Transparency is not enabled in models currently;

## Build Instructions
Run `cppm build` to build the project.

