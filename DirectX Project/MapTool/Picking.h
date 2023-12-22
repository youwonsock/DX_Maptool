#pragma once

class SectionNode;

class Picking
{
public:
	std::vector<UINT> UpdateVertexIdxList;
	std::vector<SHORT> UpdateNodeIdxList;

public:
	void FindChangeVertex(Vector3 centerPos, float findRange, int leafNodeNum
							, std::shared_ptr<SectionNode>& pickNode
							, std::vector<PNCTVertex>& vertexList);

	std::shared_ptr<SectionNode>& FindPickFace(Ray& ray, std::vector<UINT>& leafNodeIdxList, std::map<int, std::shared_ptr<SectionNode>>& leafNodeMap
						, OUT Vector3& pickPoint);
};

