// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/SocketBuffer.h"
#include "Exceptions/NullReferenceException.h"
#include "Exceptions/InvalidOperationException.h"
#include "Exceptions/ArgumentOutOfRangeException.h"

const void* SocketBuffer::GetReadonlyBuffer() const
{
	if (_block)
	{
		return reinterpret_cast<const uint8*>(_block->_ptr) + _begin;
	}
	else
	{
		throw NullReferenceException();
	}
}

void* SocketBuffer::GetBuffer() const
{
	if (_block)
	{
		if (_block->_readonly)
		{
			throw InvalidOperationException(TEXT("Buffer is readonly, but try to get mutable buffer."));
		}
		return reinterpret_cast<uint8*>(_block->_ptr) + _begin;
	}
	else
	{
		throw NullReferenceException();
	}
}

size_t SocketBuffer::GetBufferSize() const
{
	if (_block)
	{
		return _end - _begin;
	}
	else
	{
		throw NullReferenceException();
	}
}

SocketBuffer SocketBuffer::Subbuffer(size_t pos, size_t length) const
{
	size_t limit = GetBufferSize();
	if (pos >= limit)
	{
		throw ArgumentOutOfRangeException();
	}

	SocketBuffer sb;
	sb._block = _block;
	sb._begin = _begin + pos;
	sb._end = std::max(sb._begin + length, _end);
	return sb;
}

std::span<uint8> SocketBuffer::AsSpan(size_t pos, size_t length) const
{
	size_t limit = GetBufferSize();
	if (pos >= limit)
	{
		throw ArgumentOutOfRangeException();
	}

	uint8* ptr = reinterpret_cast<uint8*>(GetBuffer());
	return std::span(ptr + pos, ptr + pos + length);
}

std::span<const uint8> SocketBuffer::AsReadonlySpan(size_t pos, size_t length) const
{
	size_t limit = GetBufferSize();
	if (pos >= limit)
	{
		throw ArgumentOutOfRangeException();
	}

	const uint8* ptr = reinterpret_cast<const uint8*>(GetReadonlyBuffer());
	return std::span(ptr + pos, ptr + pos + length);
}

SocketBuffer SocketBuffer::CopyReadonly(std::vector<uint8> buffer)
{
	SocketBuffer sb;
	auto* ptr = (sb._block = std::make_shared<_impl_class>()).get();
	ptr->_ptr = buffer.data();
	ptr->_length = buffer.size();
	ptr->_readonly = true;
	ptr->_buf = std::move(buffer);
	sb._begin = 0;
	sb._end = buffer.size();
	return sb;
}

SocketBuffer SocketBuffer::CopyReadonly(std::span<const uint8> buffer)
{
	std::vector<uint8> cloned(buffer.begin(), buffer.end());
	return CopyReadonly(std::move(cloned));
}

SocketBuffer SocketBuffer::WrapReadonly(std::span<const uint8> buffer)
{
	SocketBuffer sb;
	auto* ptr = (sb._block = std::make_shared<_impl_class>()).get();
	ptr->_ptr = const_cast<uint8*>(buffer.data());
	ptr->_length = buffer.size_bytes();
	ptr->_readonly = true;
	sb._begin = 0;
	sb._end = buffer.size();
	return sb;
}

SocketBuffer SocketBuffer::Alloc(size_t length)
{
	SocketBuffer sb;
	auto* ptr = (sb._block = std::make_shared<_impl_class>()).get();
	ptr->_buf.resize(length);
	ptr->_ptr = ptr->_buf.data();
	ptr->_length = ptr->_buf.size();
	ptr->_readonly = false;
	sb._begin = 0;
	sb._end = length;
	return sb;
}