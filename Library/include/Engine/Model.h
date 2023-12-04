#pragma once

struct ModelBone;
struct ModelMesh;
struct ModelAnimation;
class Material;

class Model : public std::enable_shared_from_this<Model>
{
private:
	std::wstring modelPath = L"../../Res/Models/";
	std::wstring texturePath = L"../../Res/Textures/";
	
	std::shared_ptr<ModelBone> root;
	std::vector<std::shared_ptr<Material>> materials;
	std::vector<std::shared_ptr<ModelBone>> bones;
	std::vector<std::shared_ptr<ModelMesh>> meshes;
	std::vector<std::shared_ptr<ModelAnimation>> animations;
private:
	void BindCacheInfo();

public:
	Model();
	~Model();

	void ReadMaterial(std::wstring filename);
	void ReadModel(std::wstring filename);
	void ReadAnimation(std::wstring filename);

	UINT GetMaterialCount() { return static_cast<UINT>(materials.size()); }
	std::vector<std::shared_ptr<Material>>& GetMaterials() { return materials; }
	std::shared_ptr<Material> GetMaterialByIndex(UINT index) { return materials[index]; }
	std::shared_ptr<Material> GetMaterialByName(const std::wstring& name);

	UINT GetMeshCount() { return static_cast<UINT>(meshes.size()); }
	std::vector<std::shared_ptr<ModelMesh>>& GetMeshes() { return meshes; }
	std::shared_ptr<ModelMesh> GetMeshByIndex(UINT index) { return meshes[index]; }
	std::shared_ptr<ModelMesh> GetMeshByName(const std::wstring& name);

	UINT GetBoneCount() { return static_cast<UINT>(bones.size()); }
	std::vector<std::shared_ptr<ModelBone>>& GetBones() { return bones; }
	std::shared_ptr<ModelBone> GetBoneByIndex(UINT index) { return (index < 0 || index >= bones.size() ? nullptr : bones[index]); }
	std::shared_ptr<ModelBone> GetBoneByName(const std::wstring& name);

	UINT GetAnimationCount() { return animations.size(); }
	std::vector<std::shared_ptr<ModelAnimation>>& GetAnimations() { return animations; }
	std::shared_ptr<ModelAnimation> GetAnimationByIndex(UINT index) { return (index < 0 || index >= animations.size()) ? nullptr : animations[index]; }
	std::shared_ptr<ModelAnimation> GetAnimationByName(const std::wstring& name);
};