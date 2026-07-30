
Code drafts:

```cpp

Shader shader;
shader.Create(...);

VertexData positionData;
positionData.Create(...);

VertexData colorData;
colorData.Create(...);

InstancedData offsetsData;
offsetsData.Create(...);

RenderAction drawInstancedTriangles;
drawInstancedTriangles.Create(
  ePrimitive::TRIANGLE,
  shader, 
  {
    {positionData, "position", "a_pos"},
    {colorData, "color", "a_color"},
    {offsetsData, "offset", "a_offset"}
  });

```




Implementation steps:
  - Rename classes: Buffer -> RenderAction [DONE]
  - Rename source files: Buffer -> RenderAction [DONE]
  - Rename ReusableVertexBufferObject -> VertexBuffer [DONE]
  - Rename ReusableIndexedVertexBufferObject -> IndexedVertexBuffer [DONE]
  - Use VertexBuffer inside RenderAction; [DONE]
  - Use IndexedVertexBuffer inside IndexedRenderAction; [DONE]
  - Create Generic vertexbuffer and renderAction; [DONE]
  - Integrate generic impls on simple and indexed actions/buffers; [DONE]
  - Create class for holding instanced data; [DONE]
  - Create class VertexBuffer [DONE]
  - Create class InstancedDataBuffer [DONE]
  - Refactor GenericRenderAction implementation (lots of duplication); [DONE]
  - Create state binding manager; [DONE]
  - Rework state binding inside GenericeRenderAction implementation; [DONE]
  - Create class RenderData. GenericVertexBuffer -> RenderData; [DONE]
  - Create class IndexBuffer? Yes! [DONE]
  - Create class VertexBuffer; [NO]
  - Create shader binding structures; [DONE]
  - Test shader bindings with non-indexed draw and instanced draw (these 2 are very likely broken currently); [DONE]
  - Inside GenericRenderAction, store a container of reference datas; [DONE]
    * for non-indexed draw, the number of vertices should correspond to the smaller number across all buffers; [DONE]
    * for instanced draw, the number of instances should be the smaller across all instanced buffers; [DONE]
  - Rename InstancedDataBuffer -> InstancedData; [DONE]
  - Use shared_ptr for VertexData, IndexData and InstancedDataBuffer; [DONE]
  - Remove self owned datas from render actions. Datas should be explicitly created and managed; [DONE]
  - Create support for reference InstancedDataBuffer;  [DONE]
  - Implement dataLayout definition and named binding; [DONE]
  - Drop ShaderAttributes from the codebase; [DONE]
  - Drop RenderAction, IndexedRenderAction and InstancedRenderAction from the codebase; [DONE]
  - Rename GenericRenderAction to RenderAction; [DONE]
  - Hide ShaderBinding structure on Rendering module's impl;
    * Remove ShaderBindingsTemplate;
    * The binding definition will be specified directly in the render action (check example code); 
    * On Graphics module, we can create own structures for propagating the attributes from renderer to batching components;
  - Move IDataChangeListener definition to module's impl and also the Listeners structure;
  - Data clear should notify actions as well, we should have a separate funcion for internal clearing and user clearing;
  - We shouldn`t rely in reusing public methods inside other methods of the same object. See RenderActions, Data objects and Shader.
  - The renderAction could hold the reference to the shader it uses for rendering;
  - Use shared_ptr for Shader? RenderAction should hold a shader?
  - For shaderBinding, we should mention the shader only once. Doesn't makes any sense to link to different shaders in the same action;
  - "Use" methods on Shader, Texture, Framebuffer etc., should be private. The public API shouldn`t rely on state;
  - Create support for essential primitive types: triangles, lines and points. The render data should receive the primitive type as property;
