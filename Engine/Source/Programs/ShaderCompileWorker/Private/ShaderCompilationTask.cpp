// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "ShaderCompilationTask.h"
#include "CompilationException.h"
#include "IO/File.h"

namespace Ayla
{
	Task<std::vector<ShaderCompilationTask>> ShaderCompilationTask::ParseMakefileAsync(String makeFileName, std::stop_token cancellationToken)
	{
		auto makefileText = co_await File::ReadAllTextAsync(makeFileName, cancellationToken);
		ParseContext context;
		context.Content = makefileText;
		context.Seekpos = 0;

		ParseScratch scratch;

		ParseRoot(context, &scratch);
		co_return std::move(scratch.Tasks);
	}

	void ShaderCompilationTask::ParseRoot(ParseContext& context, ParseScratch* scratch)
	{
		while (context.Seekpos < context.Content.length())
		{
			// Skip all whitespace including newlines
			context.SkipWhiteSpace();
			
			if (context.Seekpos >= context.Content.length())
			{
				break;
			}

			auto c = context.GetCurrent();
			
			// Skip comment lines
			if (c == '#')
			{
				SkipCommentLine(context);
				continue;
			}
			
			// Parse a single shader compilation task
			ParseSingleTask(context, scratch);
		}
	}

	void ShaderCompilationTask::ParseSingleTask(ParseContext& context, ParseScratch* scratch)
	{
		ShaderCompilationTask task;
		task.m_EntryPoint = TEXT("main");

		// Read source file path
		task.m_SourceFile = ReadToken(context);
		if (task.m_SourceFile.IsEmpty())
		{
			return;
		}

		// Parse arguments
		while (context.Seekpos < context.Content.length())
		{
			auto c = context.GetCurrent();

			// End of line
			if (c == '\n' || c == '\r')
			{
				++context.Seekpos;
				break;
			}

			// Read next token
			String token = ReadToken(context);
			if (token.IsEmpty())
			{
				break;
			}

			// Parse shader type (-t)
			if (token == TEXT("-t"))
			{
				String typeStr = ReadToken(context).ToLower();
				if (typeStr == TEXT("vs") || typeStr == TEXT("vertex"))
				{
					task.m_Type = ShaderType::Vertex;
				}
				else if (typeStr == TEXT("ps") || typeStr == TEXT("pixel"))
				{
					task.m_Type = ShaderType::Pixel;
				}
				else if (typeStr == TEXT("cs") || typeStr == TEXT("compute"))
				{
					task.m_Type = ShaderType::Compute;
				}
				else if (typeStr == TEXT("lib") || typeStr == TEXT("library"))
				{
					task.m_Type = ShaderType::Library;
				}
				else
				{
					throw CompilationException(
						String::Format(TEXT("Unknown shader type '{}'. Valid types are: vs/Vertex, ps/Pixel, cs/Compute, lib/Library"), typeStr),
						task.m_SourceFile
					);
				}
			}
			// Parse output path (-o)
			else if (token == TEXT("-o"))
			{
				task.m_OutputBasePath = ReadToken(context);
			}
			// Parse dependency file path (-d)
			else if (token == TEXT("-d"))
			{
				task.m_DependencyFile = ReadToken(context);
			}
			// Parse include path (-I)
			else if (token == TEXT("-I"))
			{
				String includePath = ReadToken(context);
				if (!includePath.IsEmpty())
				{
					task.m_IncludePaths.emplace_back(std::move(includePath));
				}
			}
			// Parse entry point (-e)
			else if (token == TEXT("-e"))
			{
				task.m_EntryPoint = ReadToken(context);
			}
			// Parse Vulkan flag (--vulkan)
			else if (token == TEXT("--vulkan"))
			{
				task.m_Vulkan = true;
			}
			else
			{
				throw CompilationException(
					String::Format(TEXT("Unknown token '{}' in shader compilation task."), token),
					task.m_SourceFile
				);
			}
		}

		// Add task if valid
		if (!task.m_SourceFile.IsEmpty() && task.m_Type.has_value() && !task.m_OutputBasePath.IsEmpty())
		{
			scratch->Tasks.emplace_back(std::move(task));
		}
	}

	void ShaderCompilationTask::SkipCommentLine(ParseContext& context)
	{
		for (; context.Seekpos < context.Content.length(); ++context.Seekpos)
		{
			if (context.GetCurrent() == '\n')
			{
				++context.Seekpos;
				break;
			}
		}
	}

	String ShaderCompilationTask::ReadToken(ParseContext& context)
	{
		// Skip leading whitespace (but not newlines)
		context.SkipWhiteSpaceExceptNewline();

		if (context.Seekpos >= context.Content.length())
		{
			return String();
		}

		size_t startPos = context.Seekpos;
		auto c = context.GetCurrent();

		// Handle quoted strings
		if (c == '"')
		{
			++context.Seekpos; // Skip opening quote
			startPos = context.Seekpos;

			for (; context.Seekpos < context.Content.length(); ++context.Seekpos)
			{
				c = context.GetCurrent();
				if (c == '"')
				{
					// Check for escaped quote ("")
					if (context.Seekpos + 1 < context.Content.length() && context.Content[context.Seekpos + 1] == '"')
					{
						++context.Seekpos; // Skip second quote
						continue;
					}
					
					// End of quoted string
					String result = context.Content.Substring(startPos, context.Seekpos - startPos);
					++context.Seekpos; // Skip closing quote
					
					// Replace "" with "
					result = result.Replace(TEXT(R"("")"), TEXT(R"(")"));
					return result;
				}
			}
			
			// Unterminated string - return what we have
			return context.Content.Substring(startPos, context.Seekpos - startPos);
		}

		// Handle non-quoted tokens (read until whitespace or special character)
		for (; context.Seekpos < context.Content.length(); ++context.Seekpos)
		{
			c = context.GetCurrent();
			if (std::isspace(c) || c == '\n' || c == '\r')
			{
				break;
			}
		}

		return context.Content.Substring(startPos, context.Seekpos - startPos);
	}
}
