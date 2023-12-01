#pragma once

template <typename T>
class Geometry
{
private:
	std::vector<T> vertices;
	std::vector<UINT> indices;

public:
	Geometry() {}
	~Geometry() {}

	UINT GetVertexCount() const { return vertices.size(); }
	UINT GetIndexCount() const { return indices.size(); }

	const std::vector<T>& GetVertices() const { return vertices; }
	const std::vector<UINT>& GetIndices() const { return indices; }

	void* GetVerticesData() const { return (void*)vertices.data(); }
	void* GetIndicesData() const { return (void*)indices.data(); }

	void AddVertex(const T& vertex) { vertices.push_back(vertex); }
	void AddVertices(const std::vector<T>& vertices) { this->vertices.insert(this->vertices.end(), vertices.begin(), vertices.end()); }
	void SetVertices(const std::vector<T>& vertices) { this->vertices = vertices; }

	void AddIndex(UINT index) { indices.push_back(index); }
	void AddIndices(const std::vector<UINT>& indices) { this->indices.insert(this->indices.end(), indices.begin(), indices.end()); }
	void SetIndices(const std::vector<UINT>& indices) { this->indices = indices; }
};

