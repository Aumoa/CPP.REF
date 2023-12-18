// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Sources/SourceFile.h"

class SyntaxTree;
class AylaCxxSyntaxNode;

class HeaderSource : public SourceFile
{
protected:
	using Super = SourceFile;

private:
	std::unique_ptr<SyntaxTree> Syntaxes;

private:
	String PackageName;
	String FileId;

	struct CompiledACLASS
	{
		int32 BodyLine = 0;
		std::vector<String> ClassAttributes;
		String ClassName;
		String SuperName;
	};

	std::vector<CompiledACLASS> ACLASSes;

public:
	HeaderSource(String InPackageName, String InPath);
	virtual ~HeaderSource() noexcept override;

	virtual Task<bool> TryParseAsync(std::stop_token InCancellationToken) override;
	virtual std::vector<String> GetErrors() const override;
	virtual Task<bool> CompileAsync(std::stop_token InCancellationToken) override;
	virtual Task<> GenerateAsync(String IncludePath, String CSharpPath, std::stop_token InCancellationToken = {}) override;

private:
	bool CompileInACLASS(CompiledACLASS& Body, size_t& Index, std::vector<AylaCxxSyntaxNode*>& Nodes);
	void ParseClassAttributes(CompiledACLASS& Body, size_t& Index, std::vector<AylaCxxSyntaxNode*>& Nodes);
	bool ParseClassHeads(CompiledACLASS& Body, size_t& Index, std::vector<AylaCxxSyntaxNode*>& Nodes);

	std::vector<AylaCxxSyntaxNode*> CollectParenthesesAndStep(size_t& Index, std::vector<AylaCxxSyntaxNode*>& Nodes);

private:
	void ReportConsoleError(AylaCxxSyntaxNode* Node, String InMessage);
};