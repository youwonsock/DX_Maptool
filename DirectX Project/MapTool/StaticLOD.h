#pragma once

class SectionNode;

struct StaticLOD
{
public:
	DWORD					dwLevel;
	std::vector<std::vector<UINT>>	IndexList;
	std::vector<ComPtr<ID3D11Buffer>>	IndexBufferList;

private:

public:

	void Release();
};

