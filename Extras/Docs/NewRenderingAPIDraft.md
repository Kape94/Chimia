
Case 1: Self owned buffer

```cpp
const std::vector<float> vertexData = ...;
const ShaderAttributes attributes = ...;

Renderable renderable;

renderable.Create(vertexData, attributes);

renderable.Render();

renderable.Clear();
```

Case 2: Self owned vertex and index buffers

```cpp
const std::vector<float> vertexData = ...;
const std::vector<unsigned> indexData = ...;
const ShaderAttributes attributes = ...;

Renderable renderable;

renderable.Create(vertexData, indexData, attributes);

renderable.Render();
```

Case 3: Content update

```cpp
const std::vector<float> vertexData = ...;
const std::vector<unsigned> indexData = ...;
const ShaderAttributes attributes = ...;

Renderable renderable;

renderable.Create(vertexData, indexData, attributes);

renderable.LoadVertexData(...);
renderable.LoadIndexData(...);
```

Case 4: Reuse buffers

```cpp
const std::vector<float> vertexData = ...;
const std::vector<unsigned> indexData = ...;

Buffer buffer(vertexData, indexData);

const ShaderAttributes attributes = ...;

Renderable renderable;

renderable.Create(buffer, attributes);

renderable.LoadVertexData(...); // Do nothing
renderable.LoadIndexData(...); // Do nothing
```

Case 5: Instancing

```cpp
const std::vector<float> vertexData = ...;
const std::vector<unsigned> indexData = ...;

const ShaderAttributes attributes = ...;

const std::vector<void> instancesData = ...;
const ShaderAttributes instancedAttributes = ...;

InstancedRenderable renderable;

renderable.Create(vertexData, indexData, attributes, instancedDatas, instancedAttributes);

renderable.RecreateInstancedData(...);
```

Case 6: Multiple binding

```cpp
Buffer positions(...);
Buffer colors(...);

Renderable renderable;

renderable.Create(
    { {positions, ShaderAttribute::Float(0/*position*/, 3)}, 
      {colors, ShaderAttribute::Float(1/*color*/, 4)},
    }
);

// ooor...

Buffer model1(...);
Buffer model2(...);

Renderable renderable;

renderable.Create(
    { {model1, ShaderAttribute::Float(0/*position*/, 3)}, 
      {model1, ShaderAttribute::Float(1/*color*/, 4)},
      {model2, ShaderAttribute::Float(2/*targetPosition*/, 3)}, 
      {model2, ShaderAttribute::Float(3/*targetColor*/, 4)},
    }
);

```


Names:
  - Renderable
  - Drawable
  - Mesh
  - Geometry
  - RenderObject
  - Primitive
  - RenderAction



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
  - Create class IndexBuffer? No point!
  - Create class InstancedDataBuffer [DONE]
  - Refactor GenericRenderAction implementation (lots of duplication);
  - Rework state binding inside GenericeRenderAction implementation;
    * The buffers linked to the shader attributes and getting linked implicitly;
    * I want this binding to be very clear; 
  - Create support for reference InstancedDataBuffer;
  - Create state binding manager;
  - Create shader binding structures;
