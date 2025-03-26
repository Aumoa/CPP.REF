// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IO/FileSystemReference.h"
#include "IO/Directory.h"
#include "InvalidOperationException.h"
#include "Environment.h"
#include "Path.h"
#include "Linq/Linq.ToVector.h"
#include "Linq/Linq.Select.h"

namespace Ayla
{
	class FileReference;

	class CORE_API DirectoryReference : public FileSystemReference
	{
	public:
		DirectoryReference() noexcept
		{
		}

		DirectoryReference(String InPath) : FileSystemReference(InPath)
		{
		}

		DirectoryReference(const DirectoryReference&) = default;
		DirectoryReference(DirectoryReference&&) noexcept = default;

		virtual bool IsExists() const override
		{
			return Directory::Exists(GetValue());
		}

		[[nodiscard]] DirectoryReference GetAbsolute() const
		{
			return DirectoryReference(GetAbsolutePath());
		}

		[[nodiscard]] DirectoryReference ToCurrentDirectoryBased() const
		{
			if (IsPathFullQualified())
			{
				throw InvalidOperationException();
			}

			return Path::Combine(Environment::GetCurrentDirectory(), GetValue());
		}

		[[nodiscard]] DirectoryReference GetChild(String InName) const
		{
			return Path::Combine(GetValue(), InName);
		}

		void Create() const
		{
			Directory::CreateDirectory(GetValue());
		}

		void Delete(bool bRecursive = false) const
		{
			if (IsExists())
			{
				Directory::Delete(GetValue(), bRecursive);
			}
		}

		void Clear() const
		{
			bool bExists = IsExists();
			Delete(true);
			if (bExists)
			{
				Create();
			}
		}

		[[nodiscard]] std::vector<DirectoryReference> GetDirectories(bool bRecursive) const
		{
			if (IsExists() == false)
			{
				return {};
			}

			SearchOption Option = bRecursive ? SearchOption::AllDirectories : SearchOption::TopDirectoryOnly;
			return Directory::GetDirectories(GetValue(), Option)
				| Linq::Select([](auto p) { return DirectoryReference(p); })
				| Linq::ToVector();
		}

		[[nodiscard]] DirectoryReference GetParent() const
		{
			return Path::Combine(GetValue(), TEXT(".."));
		}

		[[nodiscard]] std::vector<FileReference> GetFiles(bool bRecursive) const;
		[[nodiscard]] FileReference GetFile(String InName) const;

		DirectoryReference& operator =(const DirectoryReference& Rhs) noexcept = default;
		DirectoryReference& operator =(DirectoryReference&& Rhs) noexcept = default;
	};
}