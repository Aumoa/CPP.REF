// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "Exception.h"
#include <vector>
#include <ranges>
#include <map>
#include <optional>

namespace Ayla
{
	class CommandLineParser
	{
    public:
        class ParserErrorException : public Exception
        {
        public:
            ParserErrorException(String message) : Exception(message) {}
        };

    private:
        // key -> list of values (each occurrence appends one entry; flag-only is nullopt)
        std::map<String, std::vector<std::optional<String>>> m_Options;
        std::optional<String> m_Body;

        void AddOptionValue(const String& key, std::optional<String> value)
        {
            m_Options[key].emplace_back(std::move(value));
        }

    public:
        template<std::ranges::input_range R> requires std::convertible_to<std::ranges::range_value_t<R>, String>
        CommandLineParser(R&& args)
        {
            auto it  = std::ranges::begin(args);
            auto end = std::ranges::end(args);

            auto isValueToken = [](const String& s)
            {
                // Starts with a quote => treat explicitly as value.
                if (s.StartsWith(TEXT("\"")) || s.StartsWith(TEXT("'")))
                {
                    return true;
                }
                // Anything not starting with '-' is a value.
                return !s.StartsWith(TEXT("-"));
            };

            while (it != end)
            {
                String arg = *it;
                if (arg.StartsWith(TEXT("--")))
                {
                    String key = arg.Substring(2);
                    std::optional<String> value;
                    auto next = it;
                    ++next;
                    if (next != end && isValueToken(*next))
                    {
                        value = *next;
                        ++it; // skip consumed value
                    }
                    AddOptionValue(key, std::move(value));
                }
                else if (arg.StartsWith(TEXT("-")) && arg.length() > 1)
                {
                    String flags = arg.Substring(1);
                    if (flags.length() == 1)
                    {
                        String key = flags.Substring(0, 1);
                        std::optional<String> value;
                        auto next = it;
                        ++next;
                        if (next != end && isValueToken(*next))
                        {
                            value = *next;
                            ++it; // skip consumed value
                        }
                        AddOptionValue(key, std::move(value));
                    }
                    else
                    {
                        for (size_t i = 0; i < flags.length(); ++i)
                        {
                            String key = flags.Substring(i, 1);
                            AddOptionValue(key, std::nullopt);
                        }
                    }
                }
                else
                {
                    if (m_Body.has_value())
                    {
                        throw ParserErrorException(String::Format(TEXT("Multiple body arguments: '{}' & '{}'"), m_Body.value(), arg));
                    }

                    // Not an option token (or single '-') => body positional argument.
                    m_Body.emplace(arg);
                }
                ++it;
            }
		}

        // Split by whitespace; sections enclosed in single or double quotes become a single token.
        // Surrounding quotes are stripped (content only). Throws ParserErrorException if a quote is not closed.
        CommandLineParser(String args)
            : CommandLineParser(
                [&]()
                {
                    std::vector<String> tokens;
                    std::basic_string<char_t> current;
                    bool inQuote = false;
                    char_t quoteChar = 0;
                    size_t quoteStartIndex = 0;

                    auto flushCurrent = [&]()
                    {
                        if (!current.empty())
                        {
                            tokens.emplace_back(String(current));
                            current.clear();
                        }
                    };

                    for (size_t i = 0; i < args.length(); ++i)
                    {
                        char_t ch = args[i];
                        if (!inQuote)
                        {
                            if (ch == '\"' || ch == '\'')
                            {
                                // Start quoted segment (do NOT store the quote itself)
                                flushCurrent();
                                inQuote = true;
                                quoteChar = ch;
                                quoteStartIndex = i;
                                current.clear();
                            }
                            else if (ch == ' ' || ch == '\t')
                            {
                                flushCurrent();
                            }
                            else
                            {
                                current += ch;
                            }
                        }
                        else
                        {
                            if (ch == quoteChar)
                            {
                                // End quoted segment: store current as a token (quotes stripped)
                                tokens.emplace_back(String(current));
                                current.clear();
                                inQuote = false;
                            }
                            else
                            {
                                current += ch;
                            }
                        }
                    }

                    if (inQuote)
                    {
                        throw ParserErrorException(
                            String::Format(TEXT("Unclosed quote starting at index {} with {}"),
                                quoteStartIndex,
                                String(1, quoteChar))
                        );
                    }

                    flushCurrent();
                    return tokens;
                }())
        {
        }

        const std::optional<String>& Body() const noexcept
        {
            return m_Body;
        }

        const std::map<String, std::vector<std::optional<String>>>& Options() const noexcept
        {
            return m_Options;
        }
	};
}