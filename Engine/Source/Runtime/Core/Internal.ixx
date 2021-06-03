// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>
#include <DirectXMath.h>

export module SC.Runtime.Core.Internal;

using namespace DirectX;

export
{
	// Windows.h
	WINBASEAPI VOID WINAPI OutputDebugStringW(LPCWSTR lpOutputString);
	WINBASEAPI int WINAPI MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar);
	WINBASEAPI int WINAPI WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar);

	// DirectXMath.h
	namespace DirectX
	{
		typedef XMVECTOR XMVECTOR;
		typedef FXMVECTOR FXMVECTOR;
		typedef FXMMATRIX FXMMATRIX;

		struct XMMATRIX;
		struct XMFLOAT4X4;
		struct XMFLOAT3;
		struct XMFLOAT4;

		XMMATRIX    XM_CALLCONV     XMLoadFloat4x4(_In_ const XMFLOAT4X4* pSource);
		XMVECTOR    XM_CALLCONV     XMLoadFloat3(_In_ const XMFLOAT3* pSource);
		XMVECTOR    XM_CALLCONV     XMLoadFloat4(_In_ const XMFLOAT4* pSource);
		void        XM_CALLCONV     XMStoreFloat4x4(_Out_ XMFLOAT4X4* pDestination, _In_ FXMMATRIX M);
		void        XM_CALLCONV     XMStoreFloat3(_Out_ XMFLOAT3* pDestination, _In_ FXMVECTOR V);
		void        XM_CALLCONV     XMStoreFloat4(_Out_ XMFLOAT4* pDestination, _In_ FXMVECTOR V);
		XMVECTOR    XM_CALLCONV     XMMatrixDeterminant(FXMMATRIX M);
		XMMATRIX    XM_CALLCONV     XMMatrixInverse(_Out_opt_ XMVECTOR* pDeterminant, _In_ FXMMATRIX M);
		float       XM_CALLCONV     XMVectorGetX(FXMVECTOR V);
		bool        XM_CALLCONV     XMMatrixDecompose(_Out_ XMVECTOR* outScale, _Out_ XMVECTOR* outRotQuat, _Out_ XMVECTOR* outTrans, _In_ FXMMATRIX M);
		XMMATRIX    XM_CALLCONV     XMMatrixMultiply(FXMMATRIX M1, CXMMATRIX M2);
		XMVECTOR    XM_CALLCONV     XMVectorZero();
		XMVECTOR    XM_CALLCONV     XMVectorSet(float x, float y, float z, float w);
		XMMATRIX    XM_CALLCONV     XMMatrixAffineTransformation(FXMVECTOR Scaling, FXMVECTOR RotationOrigin, FXMVECTOR RotationQuaternion, GXMVECTOR Translation);
	}
}