
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

Case 7: RenderData draft

```cpp
std::vector<float> vertexData {...};
std::vector<unsigned> indexData {...};

std::vector<Vec3> instancedData1 {...};
std::vector<Mat4> instancedData2 {...};

RenderData data1;
data1.Create(vertexData, indexData, instancedData1);

RenderAction render1;
render1.Create(data1, shader, eRenderPrimitive::TRIANGLES);


```

```cpp
std::vector<float> vertexData {...};
std::vector<unsigned> indexData {...};

std::vector<Vec3> instancedData1 {...};
std::vector<Mat4> instancedData2 {...};

RenderAction render1;
render1.Create(vertexData, indexData, instancedData1, shader, eRenderPrimitive::TRIANGLES);


```

```cpp
std::vector<float> vertexData {...};
std::vector<unsigned> trianglesIndexData {...};
std::vector<unsigned> linesIndexData {...};

VertexBuffer vertexBuffer;
vertexBuffer.Create(vertexData, vertexLayout);

IndexBuffer triangleIndexBuffer;
triangleIndexBuffer.Create(trianglesIndexData);

IndexBuffer linesIndexBuffer;
linesIndexBuffer.Create(linexIndexData);

RenderData triangleData(vertexBuffer, triangleIndexBuffer);
RenderData linesData(vertexBuffer, linesIndexBuffer);

RenderAction renderTriangle;
renderTriangle.Create(triangleData, shader, eRenderPrimitive::TRIANGLES);

RenderAction renderLine;
renderLine.Create(linesData, shader, eRenderPrimitive::LINES);


```



Render data API:

```cpp

void Create(const VertexBuffer& buffer,
            const int primitiveType);

void Create(const RawDataView& vertexData, 
            const size_t layoutSize,
            const int primitiveType);

void Create(const VertexBuffer& buffer,
            const IndexBuffer& indexBuffer);

void Create(const RawDataView& vertexData,
            const size_t layoutSize
            const RawArrayView& indexData,
            const int primitiveType);

void CreateInstanced(const VertexBuffer& buffer,
                     const int primitiveType,
                     const RawArrayView& instancesData,
                     const size_t instanceSize);

void CreateInstanced(const RawDataView& vertexData,
                      const size_t layoutSize,
                      const RawArrayView& instancesData,
                      const size_t instanceSize);

void CreateInstanced(const VertexBuffer& buffer,
                     const IndexBuffer& indexBuffer,
                     const RawArrayView& instancesData,
                     const size_t instanceSize);

void CreateInstanced(const RawDataView& vertexData,
                     const size_t layoutSize,
                     const RawArrayView& indexData,
                     const int primitiveType,
                     const RawArrayView& instancesData,
                     const size_t instanceSize);

void CreateInstanced(const VertexBuffer& buffer,
                     const int primitiveType,
                     const InstancedDataBuffer& instanceBuffer);

void CreateInstanced(const RawDataView& vertexData,
                     const size_t layoutSize,
                     const InstancedDataBuffer& instanceBuffer);

void CreateInstanced(const VertexBuffer& buffer,
                     const IndexBuffer& indexBuffer,
                     const InstancedDataBuffer& instanceBuffer);

void CreateInstanced(const RawDataView& vertexData,
                     const size_t layoutSize,
                     const RawArrayView& indexData,
                     const int primitiveType,
                     const InstancedDataBuffer& instanceBuffer);

void LoadVertexData(const RawDataView& vertexData);
void LoadIndexData(const RawArrayView& indexData);
void LoadInstancedData(const RawArrayView& instancesData);

// Need to relink attributes after recreation
void RecreateInstancedBuffer(
  const RawArrayView& instancesData
);

void Clear();

```



Render action API:

```cpp

void Create(
  const ePrimitiveType primitive,
  const Shader& shader
);

void AddVertexData(
  const VertexBuffer& buffer,
  const ShaderAttributes& shaderAttrs
);

void AddVertexData(
  const RawDataView& vertexData,
  const ShaderAttributes& shaderAttrs
);

void AddVertexData(
  const VertexBuffer& buffer,
  const RawArrayView& indexData,
  const ShaderAttributes& shaderAttrs
);

void AddVertexData(
  const RawDataView& vertexData,
  const RawArrayView& indexData,
  const ShaderAttributes& shaderAttrs
);

void AddInstancedData(
  const InstancedDataBuffer& instancedData,
  const ShaderAttributes& shaderAttrs
);

void AddInstancedData(
  const RawArrayView& instancedData,
  const ShaderAttributes& shaderAttrs
);

void Clear();

void Render() const;

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
  - Create state binding manager; [NO]
  - Rework state binding inside GenericeRenderAction implementation; [DONE]
  - Create class RenderData. GenericVertexBuffer -> RenderData;
  - Create class IndexBuffer? Yes!
  - Create class VertexBuffer;
  - Create support for reference InstancedDataBuffer;
  - Create shader binding structures;
  - Create support for essential primitive types: triangles, lines and points.
