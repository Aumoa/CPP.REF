// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreTypes/IntegralTypes.h"
#include <span>
#include <vector>
#include <memory>

class CORE_API SocketBuffer
{
	struct _impl_class
	{
		void* _ptr = nullptr;
		size_t _length = 0;
		bool _readonly = false;
		std::vector<uint8> _buf;
	};

	std::shared_ptr<_impl_class> _block;
	size_t _begin = 0;
	size_t _end = 0;

public:
	SocketBuffer() noexcept = default;
	SocketBuffer(const SocketBuffer&) = default;
	SocketBuffer(SocketBuffer&&) = default;

	const void* GetReadonlyBuffer() const;
	void* GetBuffer() const;
	size_t GetBufferSize() const;

	SocketBuffer Subbuffer(size_t pos, size_t length = -1) const;
	std::span<uint8> AsSpan(size_t pos, size_t length) const;
	std::span<const uint8> AsReadonlySpan(size_t pos, size_t length) const;

public:
	static SocketBuffer CopyReadonly(std::vector<uint8> buffer);
	static SocketBuffer CopyReadonly(std::span<const uint8> buffer);
	static SocketBuffer WrapReadonly(std::span<const uint8> buffer);
	static SocketBuffer Alloc(size_t length);
};