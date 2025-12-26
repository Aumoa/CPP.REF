// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShaderType.h"

namespace Ayla
{
	class ShaderCompilationTask
	{
		struct ParseContext
		{
			String Content;
			size_t Seekpos;

			const char_t& GetCurrent()
			{
				return Content[Seekpos];
			}

			void SkipWhiteSpace()
			{
				for (; Seekpos < Content.length(); ++Seekpos)
				{
					auto c = GetCurrent();
					if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
					{
						break;
					}
				}
			}

			void SkipWhiteSpaceExceptNewline()
			{
				for (; Seekpos < Content.length(); ++Seekpos)
				{
					auto c = GetCurrent();
					if (c != ' ' && c != '\t')
					{
						break;
					}
				}
			}
		};

		struct ParseScratch
		{
			std::vector<ShaderCompilationTask> Tasks;
		};

	private:
		String m_SourceFile;
		std::optional<ShaderType> m_Type;
		String m_OutputBasePath;
		std::vector<String> m_IncludePaths;

	public:
		static Task<std::vector<ShaderCompilationTask>> ParseMakefileAsync(String makeFileName, std::stop_token cancellationToken = {});

		// Getter methods
		const String& GetSourceFile() const noexcept { return m_SourceFile; }
		ShaderType GetType() const { return m_Type.value(); }
		bool HasType() const noexcept { return m_Type.has_value(); }
		const String& GetOutputBasePath() const noexcept { return m_OutputBasePath; }
		const std::vector<String>& GetIncludePaths() const noexcept { return m_IncludePaths; }

	private:
		static void ParseRoot(ParseContext& context, ParseScratch* scratch);
		static void ParseSingleTask(ParseContext& context, ParseScratch* scratch);
		static void SkipCommentLine(ParseContext& context);
		static String ReadToken(ParseContext& context);
	};
}