#pragma once

// direct x header
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi1_2.h>

#include <dinput.h>

// direct 2d header
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

// direct x math header
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#ifndef XM_CONSTEXPR
#define XM_CONSTEXPR
#endif
using namespace DirectX;
using namespace DirectX::PackedVector;

//for ComPtr
#include <wrl.h>

// additional Lib header
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/DirectXTex.inl"
#include "FMOD/fmod.h"
#include "FMOD/fmod.hpp"
#include <FBXSDK/fbxsdk.h>

#include "SimpleMath.h"
#include "tinyxml2.h"

// link direct x library
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxgi.lib")

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

// link direct 2d library
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")

// link additional Lib library
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "FBXSDK\\debug\\libfbxsdk-md.lib")
#pragma comment(lib, "FBXSDK\\debug\\libxml2-md.lib")
#pragma comment(lib, "FBXSDK\\debug\\zlib-md.lib")
#else
#pragma comment(lib, "FBXSDK\\release\\libfbxsdk-md.lib")
#pragma comment(lib, "FBXSDK\\release\\libxml2-md.lib")
#pragma comment(lib, "FBXSDK\\release\\zlib-md.lib")
#endif

#pragma comment (lib, "FMOD\\fmod_vc.lib")
#pragma comment(lib, "Engine\\Engine.lib")

using namespace DirectX;						// for DirectXMath
using namespace Microsoft::WRL;					// for ComPtr
using namespace tinyxml2;						// for tinyxml2	
