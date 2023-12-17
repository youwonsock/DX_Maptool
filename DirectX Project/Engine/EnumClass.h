#pragma once

enum ShaderScope
{
	SS_None = 0,
	SS_VertexShader = (1 << 0),
	SS_PixelShader = (1 << 1),
	SS_Both = SS_VertexShader | SS_PixelShader
};

enum class ComponentType
{
	Transform,
	MeshRenderer,
	ModelRenderer,
	Camera,
	Animator,
	Light,

	// temp for maptool
	Terrain,

	FixedComponentCount,
	
	Script
};

enum class ProjectionType
{
	Perspective = 0,
	Orthographic = 1
};

enum class ResourceType
{
	Unknown = -1,
	Mesh,
	Shader,
	Texture,
	Material,
	Animation,

	RecourcesCount
};