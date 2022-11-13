// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/Packet.h"

Packet::Builder::Builder(std::shared_ptr<Packet> p)
	: _p(std::move(p))
	, _header{}
{
}

Packet::Builder::~Builder() noexcept
{
}

std::shared_ptr<Packet> Packet::Builder::Build()
{
	check(IsCompleted());
	return std::move(_p);
}

size_t Packet::Builder::Append(std::span<const uint8> bytes)
{
	check(_p);

	size_t reads = 0;

	if (_pos < HEADER_SIZE)
	{
		size_t header = HEADER_SIZE - _pos;
		size_t size = std::min(header, bytes.size_bytes());

		memcpy(_header.data() + _pos, bytes.data(), size);
		_pos += size;
		reads += size;
		
		if (_pos >= HEADER_SIZE)
		{
#pragma pack(push, 1)
			struct _Header
			{
				uint16 messageId;
				int32 length;
				int32 correlationId;
			} *header = (_Header*)_header.data();
#pragma pack(pop)

			_p->_buf.resize((size_t)HEADER_SIZE + header->length);
			memcpy(_p->_buf.data(), header, HEADER_SIZE);
		}
	}

	if (_pos >= HEADER_SIZE && (bytes.size_bytes() - reads) > 0)
	{
		int32 readp = (int32)_pos - HEADER_SIZE;
		int32 length = _p->GetLength();
		int32 remain = length - readp;
		int32 size = std::min(remain, (int32)(bytes.size_bytes() - reads));
		memcpy(_p->_buf.data() + _pos, bytes.data() + reads, size);
		_pos += size;
		reads += size;
	}

	return reads;
}

bool Packet::Builder::IsCompleted() const
{
	return _pos >= _p->GetLength() + HEADER_SIZE;
}

std::atomic<int32> Packet::s_CorreationId = 0;

Packet::Packet()
{
}

Packet::Packet(uint16 messageId)
	: Packet(messageId, ++s_CorreationId)
{
}

Packet::Packet(uint16 messageId, int32 correlationId)
	: _buf(1024)
{
	_Set_message_id(messageId);
	_Set_corrleation_id(correlationId);
}

uint16 Packet::GetMessageId() const noexcept
{
	return *reinterpret_cast<const uint16*>(_buf.data() + 0);
}

int32 Packet::GetLength() const noexcept
{
	return *reinterpret_cast<const int32*>(_buf.data() + 2);
}

int32 Packet::GetCorrelationId() const noexcept
{
	return *reinterpret_cast<const int32*>(_buf.data() + 6);
}

std::span<uint8 const> Packet::GetBuffer() const noexcept
{
	return { _buf.data(), HEADER_SIZE + GetLength() };
}

std::shared_ptr<Packet> Packet::AllocACK(uint16 messageId)
{
	return std::make_shared<Packet>(messageId, GetCorrelationId());
}

auto Packet::CreateBuilder() -> Builder
{
	return Builder(std::make_unique<Packet>());
}

void Packet::_Set_message_id(uint16 messageId) noexcept
{
	memcpy(_buf.data() + 0, &messageId, 2);
}

void Packet::_Set_length(int32 length) noexcept
{
	memcpy(_buf.data() + 2, &length, 4);
}

void Packet::_Set_corrleation_id(int32 id) noexcept
{
	memcpy(_buf.data() + 6, &id, 4);
}

size_t Packet::_Acquire(size_t length) noexcept
{
	check(_seekpos + length <= _buf.size());
	size_t p = _seekpos + HEADER_SIZE;
	_seekpos += length;
	return p;
}

size_t Packet::_Extend(size_t length) noexcept
{
	size_t l = GetLength();
	if (length + HEADER_SIZE > _buf.size())
	{
		_buf.resize(_buf.size() * 2);
	}

	_Set_length((int32)(l + length));
	return l + HEADER_SIZE;
}

std::vector<uint8> Packet::_Read_bytes()
{
	int32 length = Read<int32>();
	size_t off = _Acquire(length);
	std::vector<uint8> bytes(length);
	memcpy(bytes.data(), _buf.data() + off, (size_t)length);
	_seekpos += length;
	return bytes;
}

String Packet::_Read_string()
{
	std::vector<uint8> bytes = _Read_bytes();
	return String(std::string_view((const char*)bytes.data(), bytes.size()));
}