// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:Object;

import std.core;
import :PrimitiveTypes;
import :LogCore;
import :LogSystem;
import :LogVerbosity;

using enum ELogVerbosity;

/// <summary>
/// Supports all classes in the smart component hierarcy and provides low-level services to derived classes.
/// </summary>
export class Object
{
	std::atomic<int32> _ref = 0;
	Object* _outer = nullptr;

	std::vector<Object*> _subobjects;

public:
	/// <summary>
	/// Initialize new <see cref="Object"/> class instance.
	/// </summary>
	Object()
	{
	}

	virtual ~Object() noexcept
	{
		// Destroy all subobjects.
		for (size_t i = 0; i < _subobjects.size(); ++i)
		{
			delete _subobjects[i];
			_subobjects[i] = nullptr;
		}
	}

	/// <summary>
	/// Get a simple string representing this object.
	/// </summary>
	/// <param name="formatArgs"> The formatting args that use to std::format. </param>
	/// <returns> The string view instance. </returns>
	virtual std::wstring ToString(std::wstring_view formatArgs = L"") const
	{
		return L"Object";
	}

	/// <summary>
	/// Create subobject which linked outer to this.
	/// </summary>
	/// <typeparam name="T"> Type of subobject. </typeparam>
	/// <typeparam name="...TArgs"> The type sequence of constructor arguments. </typeparam>
	/// <param name="...args"> The constructor arguments. </param>
	/// <returns> The instantiated pointer. </returns>
	template<class T, class... TArgs>
	T* CreateSubobject(TArgs&&... args)
	{
		T*& ptr = _subobjects.emplace_back(new T(std::forward<TArgs>(args)...));
		ptr->_outer = this;
		return ptr;
	}

	/// <summary>
	/// Get outer that owner of this object.
	/// </summary>
	/// <returns> The outer object. </returns>
	Object* GetOuter() const
	{
		return _outer;
	}

	/// <summary>
	/// Change outer. Object will destroy when outer be destroyed.
	/// </summary>
	/// <param name="newOuter"> The new outer. </param>
	void SetOuter(Object* newOuter)
	{
		if (_outer != nullptr)
		{
			_outer->InternalDetachSubobject(this);
		}

		if (newOuter != nullptr)
		{
			newOuter->InternalAttachSubobject(this);
		}

		_outer = newOuter;
	}

	/// <summary>
	/// Destroy subobject.
	/// </summary>
	/// <param name="subobject"> The target object. </param>
	static void DestroySubobject(Object* subobject)
	{
		Object* outer = subobject->_outer;
		if (outer == nullptr)
		{
			LogSystem::Log(LogCore, Error, L"Request destroy subobject but target is not valid subobject. Outer is nullptr.");
			return;
		}

		outer->InternalDestroySubobject(subobject);
	}

private:
	void InternalDetachSubobject(Object* subobject)
	{
		for (size_t i = 0; i < _subobjects.size(); ++i)
		{
			if (_subobjects[i] == subobject)
			{
				// RemoveAtSwap
				std::swap(_subobjects[_subobjects.size() - 1], _subobjects[i]);
				_subobjects.resize(_subobjects.size() - 1);
				return;
			}
		}

		LogSystem::Log(LogCore, Error, L"Request destroy subobject but target is not valid subobject. Outer have not this subobject.");
	}

	void InternalAttachSubobject(Object* subobject)
	{
		_subobjects.emplace_back(subobject);
	}

	void InternalDestroySubobject(Object* subobject)
	{
		for (size_t i = 0; i < _subobjects.size(); ++i)
		{
			if (_subobjects[i] == subobject)
			{
				// Will remove all subobjects on destructor of object.
				delete _subobjects[i];

				// RemoveAtSwap
				std::swap(_subobjects[_subobjects.size() - 1], _subobjects[i]);
				_subobjects.resize(_subobjects.size() - 1);
				return;
			}
		}

		LogSystem::Log(LogCore, Error, L"Request destroy subobject but target is not valid subobject. Outer have not this subobject.");
	}
};