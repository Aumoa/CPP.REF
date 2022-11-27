// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Services/IHostedService.h"
#include "Json/JsonUtility.h"

class IServiceProvider;

class NETWORK_API FileLogger : public IHostedService
{
	struct Configuration
	{
		String Path;
		String Filename;
		GENERATE_JSON_BODY(Configuration, Path, Filename);
	};

	class FileWriterImpl : public ILoggingExtension
	{
		FileLogger* _logger = nullptr;
		std::unique_ptr<FileStream> _logFile;
		Spinlock _lock;
		String _buffer;

	public:
		FileWriterImpl(FileLogger* logger);

		virtual void Installed() override;
		virtual void Uninstalled() override;

		virtual void TraceOne(LogEntry& entry) override;
		virtual void Flush() override;

		void InstallFileWriter();
	};

private:
	Configuration _config;
	FileWriterImpl* _impl = nullptr;

public:
	FileLogger(IServiceProvider* sp);
	virtual ~FileLogger() noexcept override;

	virtual Task<> StartAsync(std::stop_token cancellationToken) override;
	virtual Task<> StopAsync(std::stop_token cancellationToken) override;
};