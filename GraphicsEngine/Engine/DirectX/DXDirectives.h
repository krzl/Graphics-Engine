#pragma once

#ifdef _DEBUG

#define D3D_DEBUG_INFO
#include <d3d11.h>
#include <DxErr.h>

bool CheckDXError(HRESULT hr, const std::string& call, const std::string& filename, int line);

#define DXCALL_(x) CheckDXError(x, #x, __FILE__, __LINE__)
#define DXCALL_V(x) if (CheckDXError(x, #x, __FILE__, __LINE__)) return;
#define DXCALL(x) if (CheckDXError(x, #x, __FILE__, __LINE__)) return false;

#include <string>

#else

#include <d3d11.h>

#define DXCALL_(x) x;
#define DXCALL(x) x;
#define DXCALL_V(x) x;

#endif

#define DX_RELEASE(x) if(x) x->Release()