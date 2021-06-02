// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:Object;

import std.core;
import :PrimitiveTypes;

using namespace std;

/// <summary>
/// Supports all classes in the smart component hierarchy and provides low-level services to derived classes.
/// Represents unit that subobjects are binding and managed.
/// </summary>
export class Object
{
	atomic<int32> _ref = 0;
	Object* _outer = nullptr;
	set<Object*> _subobjects;

public:
	/// <summary>
	/// Initialize new <see cref="Object"/> class instance.
	/// </summary>
	Object();
	virtual ~Object() noexcept;

	/// <summary>
	/// Get a simple string representing this object.
	/// </summary>
	/// <param name="formatArgs"> The formatting args that use to std::format. </param>
	/// <returns> The string view instance. </returns>
	virtual wstring ToString(wstring_view formatArgs = L"") const;

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
		T* ptr = new T(forward<TArgs>(args)...);
		_subobjects.emplace(ptr);
		ptr->_outer = this;
		return ptr;
	}

	/// <summary>
	/// Get outer that owner of this object.
	/// </summary>
	/// <returns> The outer object. </returns>
	Object* GetOuter() const;

	/// <summary>
	/// Change outer. Object will destroy when outer be destroyed.
	/// </summary>
	/// <param name="newOuter"> The new outer. </param>
	void SetOuter(Object* newOuter);

	/// <summary>
	/// Destroy subobject.
	/// </summary>
	/// <param name="subobject"> The target object. </param>
	static void DestroySubobject(Object* subobject);

private:
	void InternalDetachSubobject(Object* subobject);
	void InternalAttachSubobject(Object* subobject);
	void InternalDestroySubobject(Object* subobject);
};