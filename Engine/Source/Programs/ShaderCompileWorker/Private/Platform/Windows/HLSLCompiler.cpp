// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Platform/Windows/HLSLCompiler.h"

#if PLATFORM_WINDOWS

#include "Exceptions/TerminateException.h"
#include "Platform/Windows/DxcIncludeHandler.h"

NHLSLCompiler::NHLSLCompiler()
{
	HR(DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&pLibrary)));
	HR(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler)));
	pIncludeHandler = new DxcIncludeHandler(GetIncludeDirectory());
}

Task<> NHLSLCompiler::CompileVertexShaderAsync(String InName, String ShaderCode, const CancellationToken& cancellationToken)
{
	return CompileShaderAsync(InName, ShaderCode, TEXT("vs_6_1"), cancellationToken);
}

Task<> NHLSLCompiler::CompilePixelShaderAsync(String InName, String ShaderCode, const CancellationToken& cancellationToken)
{
	return CompileShaderAsync(InName, ShaderCode, TEXT("ps_6_1"), cancellationToken);
}

std::span<const byte> NHLSLCompiler::GetCompileResults() const
{
	return { reinterpret_cast<const byte*>(pCompiledResults->GetBufferPointer()), pCompiledResults->GetBufferSize() };
}

std::span<const FileReference> NHLSLCompiler::GetCompilerIncludes() const
{
	return pIncludeHandler->GetHandledIncludeFiles();
}

Task<> NHLSLCompiler::CompileShaderAsync(String InName, String ShaderCode, String InModelName, const CancellationToken& cancellationToken)
{
	std::string SrcContentMultiByte = ShaderCode.AsCodepage();

	ComPtr<IDxcBlobEncoding> pEncoding;
	HR(pLibrary->CreateBlobWithEncodingFromPinned(SrcContentMultiByte.c_str(), (UINT32)SrcContentMultiByte.length(), 0, &pEncoding));

	ComPtr<IDxcOperationResult> pResult;
	HRESULT hCompileResult;
	co_await Task<>::Run([&]()
	{
		hCompileResult = pCompiler->Compile(pEncoding.Get(),
			InName.c_str(),
			TEXT("main").c_str(),
			InModelName.c_str(),
			NULL, 0,
			NULL, 0,
			pIncludeHandler.Get(),
			&pResult
		);
	});

	HR(hCompileResult);
	pResult->GetStatus(&hCompileResult);

	ComPtr<IDxcBlobEncoding> pErrorBlob;
	if (SUCCEEDED(pResult->GetErrorBuffer(&pErrorBlob)) && pErrorBlob && pErrorBlob->GetBufferPointer())
	{
		String Message = String::FromCodepage({ (const char*)pErrorBlob->GetBufferPointer(), (size_t)pErrorBlob->GetBufferSize() });
		Console::Error.WriteLine(Message);
	}

	if (FAILED(hCompileResult))
	{
		throw TerminateException(TerminateException::EKnownErrorCodes::CompilerError);
	}

	HR(pResult->GetResult(&pCompiledResults));
}

#endif