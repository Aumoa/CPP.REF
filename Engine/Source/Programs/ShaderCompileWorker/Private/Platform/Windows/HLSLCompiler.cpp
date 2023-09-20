// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Platform/Windows/HLSLCompiler.h"

#if PLATFORM_WINDOWS

#include "Exceptions/TerminateException.h"
#include "Platform/Windows/DxcIncludeHandler.h"

NHLSLCompiler::NHLSLCompiler()
{
	HR(DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&pLibrary)));
	HR(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler)));
}

Task<std::vector<byte>> NHLSLCompiler::CompileVertexShaderAsync(String InShaderFile, std::stop_token InCancellationToken)
{
	String SrcContent = co_await File::ReadAllTextAsync(InShaderFile, InCancellationToken);
	std::string SrcContentMultiByte = SrcContent.AsCodepage();



	ComPtr<IDxcBlobEncoding> pEncoding;
	HR(pLibrary->CreateBlobWithEncodingFromPinned(SrcContentMultiByte.c_str(), (UINT32)SrcContentMultiByte.length(), 0, &pEncoding));

	ComPtr<IDxcOperationResult> pResult;
	HRESULT hCompileResult;
	ComPtr<IDxcIncludeHandler> pIncludeHandler = new DxcIncludeHandler(Path::GetDirectoryName(InShaderFile));
	co_await Task<>::Run([&]()
	{
		hCompileResult = pCompiler->Compile(pEncoding.Get(),
			InShaderFile.c_str(),
			TEXT("main").c_str(),
			TEXT("vs_6_1").c_str(),
			NULL, 0,
			NULL, 0,
			pIncludeHandler.Get(),
			&pResult
			);
	});

	HR(hCompileResult);
	pResult->GetStatus(&hCompileResult);

	ComPtr<IDxcBlobEncoding> pErrorBlob;
	if (SUCCEEDED(pResult->GetErrorBuffer(&pErrorBlob)) && pErrorBlob)
	{
		std::string_view View = (const char*)pErrorBlob->GetBufferPointer();
		Console::Error.WriteLine(String(View));
	}

	if (FAILED(hCompileResult))
	{
		throw TerminateException(TerminateException::EKnownErrorCodes::CompilerError);
	}

	ComPtr<IDxcBlob> pCode;
	HR(pResult->GetResult(&pCode));

	std::vector<byte> Bytes(pCode->GetBufferSize());
	memcpy(Bytes.data(), pCode->GetBufferPointer(), pCode->GetBufferSize());

	co_return Bytes;
}

Task<std::vector<byte>> NHLSLCompiler::CompilePixelShaderAsync(String InShaderFile, std::stop_token InCancellationToken)
{
	String SrcContent = co_await File::ReadAllTextAsync(InShaderFile, InCancellationToken);
	std::string SrcContentMultiByte = SrcContent.AsCodepage();

	ComPtr<IDxcBlobEncoding> pEncoding;
	HR(pLibrary->CreateBlobWithEncodingFromPinned(SrcContentMultiByte.c_str(), (UINT32)SrcContentMultiByte.length(), 0, &pEncoding));

	ComPtr<IDxcOperationResult> pResult;
	HRESULT hCompileResult;
	co_await Task<>::Run([&]()
		{
			hCompileResult = pCompiler->Compile(pEncoding.Get(),
			InShaderFile.c_str(),
			TEXT("main").c_str(),
			TEXT("vs_6_1").c_str(),
			NULL, 0,
			NULL, 0,
			NULL,
			&pResult
			);
		});
	if (FAILED(hCompileResult))
	{
		if (pResult)
		{
			ComPtr<IDxcBlobEncoding> pErrorBlob;
			HR(pResult->GetErrorBuffer(&pErrorBlob));
			std::string_view View = (const char*)pErrorBlob->GetBufferPointer();
			Console::Error.WriteLine(String(View));
		}
		HR(hCompileResult);
	}

	ComPtr<IDxcBlob> pCode;
	HR(pResult->GetResult(&pCode));

	std::vector<byte> Bytes(pCode->GetBufferSize());
	memcpy(Bytes.data(), pCode->GetBufferPointer(), pCode->GetBufferSize());

	co_return Bytes;
}

#endif