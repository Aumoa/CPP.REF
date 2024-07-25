// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:StreamWriter;

export import :TextWriter;
export import :Stream;

export class CORE_API StreamWriter : public TextWriter
{
private:
	std::shared_ptr<Stream> CurrentStream_Capture;
	Stream* CurrentStream = nullptr;

public:
	StreamWriter(std::shared_ptr<Stream> InStream);
	StreamWriter(Stream* InStreamPtr);

	virtual void Write(String Val) override;
	virtual Task<> WriteAsync(String Val) override;
};