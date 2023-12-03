#include "pch.h"
#include "ModelAnimator.h"

#include "ModelMesh.h"
#include "Material.h"
#include "Model.h"

ModelAnimator::ModelAnimator(std::shared_ptr<Shader> shader) : Base(ComponentType::Animator), shader(shader)
{
}

ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::Update()
{
	// todo
}

void ModelAnimator::SetModel(std::shared_ptr<Model> model)
{
}
