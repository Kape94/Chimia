
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
  - Hide ShaderBinding structure on Rendering module's impl; [DONE]
    * Remove ShaderBindingsTemplate; [DONE]
    * The binding definition will be specified directly in the render action (check example code); [DONE]
    * On Graphics module, we can create own structures for propagating the attributes from renderer to batching components; [DONE]
  - Move IDataChangeListener definition to module's impl and also the Listeners structure; [DONE]
  - For shaderBinding, we should mention the shader only once. Doesn't makes any sense to link to different shaders in the same action; [DONE]
  - We shouldn`t rely in reusing public methods inside other methods of the same object. See RenderActions, Data objects and Shader. [DONE]
    * Guess we could try moving the clear functions to private; [DONE]
    * Clear will only be called if we try to re-created a already existing data; [DONE]
  - Adopt m_ prefix for Framebuffer and Texture members; [DONE]
  - Data clear should notify actions as well, we should have a separate funcion for internal clearing and user clearing; [DONE]
    * Does it makes sense to clear the data before its scope ends? [DONE]
    * I don't think so, we're currently doing only for data resize, but we should get public functions in the datas for that; [DONE]
  - Move Texture::GetId functionality to private section; [DONE]
  - "Use" methods on Shader, Texture, Framebuffer etc., should be private. The public API shouldn`t rely on state; [DONE]
    * For Texture::Use: The texture should be passed altogether in the Shader::SetUniform call; [DONE]
    * For Framebuffer::Use, the framebuffer could optionally be provided to RenderAction. Or, a new structure called Target could be 
        created, to hold a framebuffer and a shader, and the RenderAction could reference it; [DONE]
    * For Shader::Use, the RenderAction should hold a reference to it, or it should be passed to RenderAction::Render call as parameter; [DONE]
  - Shader shouldn't bind the texture right away when the uniform is setted, it should do that when it's Use method is called; [DONE]
  - The renderAction could hold the reference to the shader it uses for rendering; [DONE]
  - Create uniform location cache inside Shader; [NO]
    * When applied, it led to a performance regression; [NO]
  
  - Use shared_ptr for Shader!
    * Probably yes. We do need to support shader re-targeting though;
  - Create Texture2DInstance as well!
    * Will be held by shader inUseTextures table;
  - Don't forget to create the RenderAction retargeting function, it should be pretty easy!  
  - VertexData::Create and VertexData::New should be combined into the same function;
    * Same applies to InstancedData and IndexData;
  - The static object creation functions should be all centered at Chimia::Rendering;
  - We should maybe swap the classes names? ShaderInstance should be called Shader, and the Shader could be called ShaderInstance or ShaderObject?
  - Create support for essential primitive types: triangles, lines and points. The render data should receive the primitive type as property;
