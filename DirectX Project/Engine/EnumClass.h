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
	Transform = 0,
	MeshRenderer = 1,
	Camera = 2,
	Animator = 3,
	FixedComponentCount = 4,
	
	Script = 5
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