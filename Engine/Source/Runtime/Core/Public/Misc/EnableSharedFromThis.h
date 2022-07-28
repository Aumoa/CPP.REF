// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <memory>

template<class T>
class EnableSharedFromThis : public std::enable_shared_from_this<T>
{
public:
	template<class U>
	inline std::shared_ptr<U> SharedFromThis()
	{
		std::shared_ptr ptr = this->shared_from_this();
		return std::dynamic_pointer_cast<U>(ptr);
	}

	template<class U>
	inline std::shared_ptr<const U> SharedFromThis() const
	{
		std::shared_ptr ptr = this->shared_from_this();
		return std::dynamic_pointer_cast<const U>(ptr);
	}

	template<class U>
	inline std::weak_ptr<U> WeakFromThis()
	{
		std::shared_ptr ptr = this->shared_from_this();
		return std::dynamic_pointer_cast<U>(ptr);
	}

	template<class U>
	inline std::weak_ptr<const U> WeakFromThis() const
	{
		std::shared_ptr ptr = this->shared_from_this();
		return std::dynamic_pointer_cast<const U>(ptr);
	}
};