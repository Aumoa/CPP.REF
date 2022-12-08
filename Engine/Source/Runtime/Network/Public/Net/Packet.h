// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <bit>

class NETWORK_API Packet
{
private:
	static constexpr size_t HEADER_SIZE = 10;

public:
	class Builder
	{
		friend class Packet;

	private:
		std::shared_ptr<Packet> _p;

		std::array<uint8, HEADER_SIZE> _header;
		size_t _pos = 0;

	private:
		Builder(std::shared_ptr<Packet> p);

	public:
		Builder(Builder&&) = default;
		~Builder() noexcept;

		std::shared_ptr<Packet> Build();
		size_t Append(std::span<const uint8> bytes);

		bool IsCompleted() const;

		Builder& operator =(Builder&&) = default;
	};

private:
	static std::atomic<int32> s_CorreationId;

	std::vector<uint8> _buf;
	size_t _seekpos = 0;

public:
	Packet();
	Packet(uint16 messageId);
	Packet(uint16 messageId, int32 corrleationId);

	uint16 GetMessageId() const noexcept;
	int32 GetLength() const noexcept;
	int32 GetCorrelationId() const noexcept;
	std::span<uint8 const> GetBuffer() const noexcept;

	std::shared_ptr<Packet> AllocACK(uint16 messageId);

	template<class T>
	String Read() requires std::same_as<T, String> { return _Read_string(); }
	void Write(String value) { _Write_string(value); }

#define DECLARE_GENERIC_READ(type) \
	template<class T> \
	T Read() requires std::same_as<T, type> { return _Generic_read<T>(); }

#define DECLARE_GENERIC_WRITE(type) \
	void Write(const type& value) { _Generic_write(value); }

#define DECLARE_GENERIC_READ_WRITE(type) \
	DECLARE_GENERIC_READ(type); \
	DECLARE_GENERIC_WRITE(type);

	DECLARE_GENERIC_READ_WRITE(bool);
	DECLARE_GENERIC_READ_WRITE(float);
	DECLARE_GENERIC_READ_WRITE(double);
	DECLARE_GENERIC_READ_WRITE(uint8);
	DECLARE_GENERIC_READ_WRITE(int8);
	DECLARE_GENERIC_READ_WRITE(uint16);
	DECLARE_GENERIC_READ_WRITE(int16);
	DECLARE_GENERIC_READ_WRITE(uint32);
	DECLARE_GENERIC_READ_WRITE(int32);
	DECLARE_GENERIC_READ_WRITE(uint64);
	DECLARE_GENERIC_READ_WRITE(int64);
	DECLARE_GENERIC_READ_WRITE(char_t);

#undef DECLARE_GENERIC_READ
#undef DECLARE_GENERIC_WRITE
#undef DECLARE_GENERIC_READ_WRITE

	template<class TVector>
	TVector Read() requires
		std::same_as<TVector, std::vector<typename TVector::value_type>> &&
		requires { std::declval<Packet>().template Read<typename TVector::value_type>(); }
	{
		using element_t = typename TVector::value_type;
		int32 length = Read<int32>();
		std::vector<element_t> values;
		values.reserve(length);
		for (int32 i = 0; i < length; ++i)
		{
			values.emplace_back(Read<element_t>());
		}
		return values;
	}

	template<class T>
	void Write(const T& values) requires
		requires { std::span(values); } &&
		requires { std::declval<Packet>().Write(std::span(values)); }
	{
		Write(std::span(values));
	}

	template<class T>
	void Write(std::span<T> values) requires
		requires { std::declval<Packet>().Write(std::declval<T>()); }
	{
		Write((int32)values.size());
		for (auto& v : values)
		{
			Write(v);
		}
	}

	template<class TSerializable>
	TSerializable Read() requires
		requires { TSerializable::Deserialize(std::declval<TSerializable&>(), std::declval<Packet&>()); }
	{
		TSerializable value;
		TSerializable::Deserialize(value, *this);
		return value;
	}

	template<class TSerializable>
	void Write(const TSerializable& value) requires
		requires { TSerializable::Serialize(std::declval<const TSerializable&>(), std::declval<Packet&>()); }
	{
		TSerializable::Serialize(value, *this);
	}

public:
	static Builder CreateBuilder();

private:
	void _Set_message_id(uint16 messageId) noexcept;
	void _Set_length(int32 length) noexcept;
	void _Set_corrleation_id(int32 id) noexcept;

	size_t _Acquire(size_t length) noexcept;
	size_t _Extend(size_t length) noexcept;

	std::vector<uint8> _Read_bytes();
	void _Write_bytes(std::span<const uint8> values);
	String _Read_string();
	void _Write_string(String value);

	template<class T>
	T _Generic_read()
	{
		static_assert(sizeof(T) == 1 || sizeof(T) % 2 == 0);
		size_t off = _Acquire(sizeof(T));
		T value;
		memcpy(&value, _buf.data() + off, sizeof(T));
		if constexpr (std::endian::native != std::endian::little && sizeof(T) > 1)
		{
			size_t sp = sizeof(T) / 2;
			uint8* p = reinterpret_cast<uint8*>(&value);
			for (size_t i = 0; i < sp; ++i)
			{
				std::swap(p[i], p[sizeof(T) - (i + 1)]);
			}
		}
		return value;
	}

	template<class T>
	void _Generic_write(T value)
	{
		static_assert(sizeof(T) == 1 || sizeof(T) % 2 == 0);
		size_t off = _Extend(sizeof(T));
		if constexpr (std::endian::native != std::endian::little && sizeof(T) > 1)
		{
			size_t sp = sizeof(T) / 2;
			uint8* p = reinterpret_cast<uint8*>(&value);
			for (size_t i = 0; i < sp; ++i)
			{
				std::swap(p[i], p[sizeof(T) - (i + 1)]);
			}
		}
		memcpy(_buf.data() + off, &value, sizeof(T));
	}
};

template<class T>
Packet& operator >>(Packet& p, T& o) requires requires { { p.Read<T>() }; }
{
	o = p.Read<T>();
	return p;
}

template<class T>
Packet& operator <<(Packet& p, const T& i) requires requires { { p.Write(i) }; }
{
	p.Write(i);
	return p;
}