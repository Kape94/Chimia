# Chimia lib

TODO: update readme
TODO: Buffers implementation code on Rendering module is quite messy.
TODO: Review static models code
  - Instead of managing an internal batch, it could be just a single instance;
  - Then, we would have just a table of many static models;
  - However, the current way is more efficient, it leads to less state changes and draw calls;
  - We should review the static triangles implementation;
TODO: Models should not be tied up to the renderer;
TODO: Model is not a good name for class Model;
TODO: There should be 3 big components inside the renderer:
  - The model component;
  - The triangle mesh component, which supports static and dynamic;
  - The indexed triangle mesh component;

## Build Instructions
Run `cppm build` to build the project.

