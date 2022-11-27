// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Services/FileLogger.h"
#include "Services/IServiceProvider.h"

LogCategory LogFileLogger(TEXT("LogFileLogger"));

FileLogger::FileWriterImpl::FileWriterImpl(FileLogger* logger)
	: _logger(logger)
{
}

void FileLogger::FileWriterImpl::Installed()
{
	InstallFileWriter();
	Log::Debug(LogFileLogger, TEXT("FileWriter installed."));
}

void FileLogger::FileWriterImpl::Uninstalled()
{
}

void FileLogger::FileWriterImpl::TraceOne(LogEntry& entry)
{
	static auto l2s = [](ELogLevel l)
	{
		switch (l)
		{
		case ELogLevel::Debug:
			return TEXT("dbug");
		case ELogLevel::Info:
			return TEXT("info");
		case ELogLevel::Warning:
			return TEXT("warn");
		case ELogLevel::Error:
			return TEXT("erro");
		case ELogLevel::Fatal:
			return TEXT("ftal");
		default:
			std::unreachable();
		}
	};

	String categoryName = entry.Category->GetCategoryName();
	_buffer += String::Format(TEXT("{}: {}: {}: {}\n"), entry.LogDate.ToString(TEXT("%H:%M:%S")), l2s(entry.Level), categoryName, entry.Message);
}

void FileLogger::FileWriterImpl::Flush()
{
	if (_buffer.IsEmpty())
	{
		return;
	}

	std::string utf8 = std::exchange(_buffer, TEXT("")).AsCodepage(65001);

	auto lock = std::unique_lock(_lock);
	_logFile->Write({ (const uint8*)utf8.c_str(), utf8.length() });
}

void FileLogger::FileWriterImpl::InstallFileWriter()
{
	String composedFilename = String::Format(TEXT("{}-{:%Y-%m-%d}"), _logger->_config.Filename, DateTime::Now().GetTimePoint());
	composedFilename = Path::ChangeExtension(composedFilename, TEXT(".log"));

	{
		auto lock = std::unique_lock(_lock);
		_logFile = std::make_unique<FileStream>(Path::Combine(_logger->_config.Path, composedFilename), EFileAccessMode::Append, EFileSharedMode::Read, EFileMode::OpenOrCreate);
	}

	auto now = DateTime::Now();
	DateTime refreshTimer = (now + TimeSpan::FromDays(1)).GetFloor<std::chrono::days>();
	Task<>::Delay(refreshTimer - now).ContinueWith([this](auto)
	{
		InstallFileWriter();
	});
}

FileLogger::FileLogger(IServiceProvider* sp)
{
	_config = sp->GetConfig<Configuration>().value_or(Configuration());
}

FileLogger::~FileLogger() noexcept
{
}

Task<> FileLogger::StartAsync(std::stop_token cancellationToken)
{
	if (_config.Filename.IsEmpty())
	{
		_config.Filename = Environment::GetExecutingApp();
		_config.Filename = Path::GetFileNameWithoutExtension(_config.Filename);
	}

	if (_config.Path.IsEmpty())
	{
		_config.Path = TEXT("Saved/Logs");
	}

	if (Directory::Exists(_config.Path) == false)
	{
		Directory::CreateDirectory(_config.Path);
	}

	auto impl = std::make_unique<FileWriterImpl>(this);
	_impl = impl.get();
	Log::InstallExtension(std::move(impl));

	return Task<>::CompletedTask();
}

Task<> FileLogger::StopAsync(std::stop_token cancellationToken)
{
	return Task<>::CompletedTask();
}