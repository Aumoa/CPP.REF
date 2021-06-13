// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:SubclassOf;

import std.core;
import SC.Runtime.Core;

using namespace std;

export
{
	/// <summary>
	/// Represents subclass of base type.
	/// </summary>
	/// <typeparam name="TBase"> The base class. </typeparam>
	template<class TBase>
	class SubclassOf
	{
		template<class>
		friend class SubclassOf;

		inline static function<Object*(Object*)> _myctor;

		size_t _hash = 0;
		function<Object*(Object*)> _ctor;

	public:
		/// <summary>
		/// Initialize new <see cref="SubclassOf"/> instance.
		/// </summary>
		inline SubclassOf()
		{
		}

		/// <summary>
		/// Initialize new <see cref="SubclassOf"/> instance.
		/// </summary>
		inline SubclassOf(const SubclassOf& rhs)
			: _hash(rhs._hash)
			, _ctor(rhs._ctor)
		{
		}

		/// <summary>
		/// Initialize new <see cref="SubclassOf"/> instance.
		/// </summary>
		inline SubclassOf(SubclassOf&& rhs)
			: _hash(rhs._hash)
			, _ctor(move(rhs._ctor))
		{
		}

		/// <summary>
		/// Initialize new <see cref="SubclassOf"/> instance.
		/// </summary>
		template<derived_from<TBase> TOther>
		inline SubclassOf(const SubclassOf<TOther>& rhs)
			: _hash(rhs._hash)
			, _ctor(rhs._ctor)
		{
		}

		/// <summary>
		/// Initialize new <see cref="SubclassOf"/> instance.
		/// </summary>
		template<derived_from<TBase> TOther>
		inline SubclassOf(SubclassOf<TOther>&& rhs)
			: _hash(rhs._hash)
			, _ctor(move(rhs._ctor))
		{
		}

		/// <summary>
		/// Get identifier hash code.
		/// </summary>
		inline size_t GetHashCode() const { return _hash; }

		/// <summary>
		/// Represents this is valid state.
		/// </summary>
		inline bool IsValid() const { return (bool)_ctor; }

		/// <summary>
		/// Instantiate saved class as base class.
		/// </summary>
		/// <param name="outer"> Outer object of instance. </param>
		inline TBase* Instantiate(Object* outer) const
		{
			if (!IsValid())
			{
				return nullptr;
			}
			return dynamic_cast<TBase*>(_ctor(outer));
		}

		inline SubclassOf& operator =(const SubclassOf& rhs)
		{
			_hash = rhs._hash;
			_ctor = rhs._ctor;
			return *this;
		}

		inline SubclassOf& operator =(SubclassOf&& rhs)
		{
			_hash = rhs._hash;
			_ctor = move(rhs._ctor);
			return *this;
		}

		template<derived_from<TBase> TOther>
		inline SubclassOf& operator =(const SubclassOf<TOther>& rhs)
		{
			_hash = rhs._hash;
			_ctor = rhs._ctor;
			return *this;
		}

		template<derived_from<TBase> TOther>
		inline SubclassOf& operator =(SubclassOf<TOther>&& rhs)
		{
			_hash = rhs._hash;
			_ctor = move(rhs._ctor);
			return *this;
		}

		inline bool operator ==(const SubclassOf& rhs) const
		{
			return _hash == rhs._hash;
		}

		inline bool operator !=(const SubclassOf& rhs) const
		{
			return _hash != rhs._hash;
		}

		/// <summary>
		/// Get <see cref="SubclassOf"/> instance with current base class.
		/// </summary>
		/// <returns></returns>
		inline static SubclassOf StaticClass()
		{
			if (!_myctor)
			{
				_myctor = [](Object* outer)
				{
					return static_cast<Object*>(outer->CreateSubobject<TBase>());
				};
			}

			SubclassOf ins;
			ins._hash = UniqueType<TBase>::HashCode;
			ins._ctor = _myctor;
			return ins;
		}
	};
}