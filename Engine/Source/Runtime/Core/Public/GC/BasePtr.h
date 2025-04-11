// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

namespace Ayla
{
	class Object;
	class GC;

	class BasePtr
	{
		friend GC;

	protected:
		Object* m_Object = nullptr;

	public:
		inline bool operator ==(const BasePtr& rhs) const noexcept
		{
			return m_Object == rhs.m_Object;
		}

		inline bool operator ==(nullptr_t) const noexcept
		{
			return m_Object == nullptr;
		}

		inline auto operator <=>(const BasePtr& rhs) const noexcept
		{
			return m_Object <=> rhs.m_Object;
		}

		inline operator bool() const noexcept
		{
			return m_Object != nullptr;
		}
	};
}