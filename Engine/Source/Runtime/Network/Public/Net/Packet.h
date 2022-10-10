// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Packet.gen.h"

SCLASS()
class NETWORK_API Packet : implements Object
{
	GENERATED_BODY()

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
	inline std::span<uint8 const> GetBuffer() const noexcept { return _buf; }

	std::shared_ptr<Packet> AllocACK(uint16 messageId);

	template<class T>
	std::vector<uint8> Read() requires std::same_as<T, std::vector<uint8>> { return _Read_bytes(); }
	template<class T>
	String Read() requires std::same_as<T, String> { return _Read_string(); }

#define DECLARE_GENERIC_READ(type) \
	template<class T> \
	int32 Read() requires std::same_as<T, type> { return _Generic_read<T>(); }

#define DECLARE_GENERIC_WRITE(type) \
	template<class T> \
	void Write(const T& value) requires std::same_as<T, type> { _Generic_write(value); }

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

#undef DECLARE_GENERIC_READ
#undef DECLARE_GENERIC_WRITE
#undef DECLARE_GENERIC_READ_WRITE

public:
	static Builder CreateBuilder();

private:
	void _Set_message_id(uint16 messageId) noexcept;
	void _Set_length(int32 length) noexcept;
	void _Set_corrleation_id(int32 id) noexcept;

	size_t _Acquire(size_t length) noexcept;
	size_t _Extend(size_t length) noexcept;

	std::vector<uint8> _Read_bytes();
	String _Read_string();

	template<class T>
	T _Generic_read()
	{
		size_t off = _Acquire(sizeof(T));
		T value;
		memcpy(&value, _buf.data() + off, sizeof(T));
		return value;
	}

	template<class T>
	void _Generic_write(const T& value)
	{
		size_t off = _Extend(sizeof(T));
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