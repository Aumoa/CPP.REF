// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include <vector>

namespace Ayla
{
	/// <summary>
	/// Represents a mutable string of characters for efficient string building operations.
	/// </summary>
	class StringBuilder
	{
		std::vector<String> m_Chunks;
		size_t m_TotalLength = 0;

	public:
		/// <summary>
		/// Initializes a new instance of the StringBuilder class.
		/// </summary>
		inline StringBuilder() noexcept = default;

		/// <summary>
		/// Initializes a new instance of the StringBuilder class with the specified initial string.
		/// </summary>
		/// <param name="initial">The initial string value.</param>
		explicit inline StringBuilder(String initial)
		{
			if (initial)
			{
				m_TotalLength = initial.length();
				m_Chunks.emplace_back(std::move(initial));
			}
		}

		/// <summary>
		/// Initializes a new instance of the StringBuilder class with the specified capacity.
		/// </summary>
		/// <param name="capacity">The initial capacity for the number of string chunks.</param>
		explicit inline StringBuilder(size_t capacity)
		{
			m_Chunks.reserve(capacity);
		}

		/// <summary>
		/// Appends a string to the end of the current StringBuilder object.
		/// </summary>
		/// <param name="value">The string to append.</param>
		/// <returns>A reference to this instance after the append operation has completed.</returns>
		inline StringBuilder& Append(String value)
		{
			if (value)
			{
				m_TotalLength += value.length();
				m_Chunks.emplace_back(std::move(value));
			}
			return *this;
		}

		/// <summary>
		/// Appends a character to the end of the current StringBuilder object.
		/// </summary>
		/// <param name="ch">The character to append.</param>
		/// <returns>A reference to this instance after the append operation has completed.</returns>
		inline StringBuilder& Append(char_t ch)
		{
			return Append(String(ch, 1));
		}

		/// <summary>
		/// Appends a substring to the end of the current StringBuilder object.
		/// </summary>
		/// <param name="value">The string that contains the substring to append.</param>
		/// <param name="startIndex">The starting position of the substring within value.</param>
		/// <param name="count">The number of characters in value to append.</param>
		/// <returns>A reference to this instance after the append operation has completed.</returns>
		inline StringBuilder& Append(const String& value, size_t startIndex, size_t count)
		{
			if (value && startIndex < value.length())
			{
				return Append(value.Substring(startIndex, count));
			}
			return *this;
		}

		/// <summary>
		/// Appends a specified number of copies of the string representation of a character to this instance.
		/// </summary>
		/// <param name="ch">The character to append.</param>
		/// <param name="repeatCount">The number of times to append ch.</param>
		/// <returns>A reference to this instance after the append operation has completed.</returns>
		inline StringBuilder& Append(char_t ch, size_t repeatCount)
		{
			if (repeatCount > 0)
			{
				return Append(String(ch, repeatCount));
			}
			return *this;
		}

		/// <summary>
		/// Appends the string representation of a specified object to the end of the current StringBuilder object.
		/// </summary>
		/// <typeparam name="T">The type of the object to append.</typeparam>
		/// <param name="value">The object to append.</param>
		/// <returns>A reference to this instance after the append operation has completed.</returns>
		template<class T>
		inline StringBuilder& Append(const T& value) requires requires { { value.ToString() } -> std::convertible_to<String>; }
		{
			return Append(value.ToString());
		}

		/// <summary>
		/// Appends a formatted string to the end of the current StringBuilder object.
		/// </summary>
		/// <typeparam name="...TArgs">The types of the format arguments.</typeparam>
		/// <param name="format">The format string.</param>
		/// <param name="...args">The format arguments.</param>
		/// <returns>A reference to this instance after the append operation has completed.</returns>
		template<class... TArgs>
		inline StringBuilder& AppendFormat(String format, TArgs&&... args)
		{
			return Append(String::Format(format, std::forward<TArgs>(args)...));
		}

		/// <summary>
		/// Appends a string followed by a line terminator to the end of the current StringBuilder object.
		/// </summary>
		/// <param name="value">The string to append.</param>
		/// <returns>A reference to this instance after the append operation has completed.</returns>
		inline StringBuilder& AppendLine(String value = String::GetEmpty())
		{
			if (value)
			{
				Append(value);
			}
			return Append(TEXT("\n"));
		}

		/// <summary>
		/// Appends a formatted string followed by a line terminator to the end of the current StringBuilder object.
		/// </summary>
		/// <typeparam name="...TArgs">The types of the format arguments.</typeparam>
		/// <param name="format">The format string.</param>
		/// <param name="...args">The format arguments.</param>
		/// <returns>A reference to this instance after the append operation has completed.</returns>
		template<class... TArgs>
		inline StringBuilder& AppendLineFormat(String format, TArgs&&... args)
		{
			return AppendLine(String::Format(format, std::forward<TArgs>(args)...));
		}

		/// <summary>
		/// Appends a string that is produced by joining an array of strings.
		/// </summary>
		/// <param name="separator">The string to use as a separator.</param>
		/// <param name="values">An array of strings to join.</param>
		/// <returns>A reference to this instance after the append operation has completed.</returns>
		template<std::ranges::input_range TStringArray>
			requires std::convertible_to<std::ranges::range_value_t<TStringArray>, String>
		inline StringBuilder& AppendJoin(const String& separator, const TStringArray& values)
		{
			bool bFirst = true;
			for (const auto& value : values)
			{
				if (!bFirst)
				{
					Append(separator);
				}
				Append(value);
				bFirst = false;
			}
			return *this;
		}

		/// <summary>
		/// Appends a string that is produced by joining a sequence of values using a separator.
		/// </summary>
		/// <typeparam name="...TArgs">The types of the values to join.</typeparam>
		/// <param name="separator">The string to use as a separator.</param>
		/// <param name="...values">The values to join.</param>
		/// <returns>A reference to this instance after the append operation has completed.</returns>
		template<std::convertible_to<String>... TArgs>
		inline StringBuilder& AppendJoin(const String& separator, const TArgs&... values)
		{
			return AppendJoin(separator, std::array<String, sizeof...(TArgs)>{ (String)values... });
		}

		/// <summary>
		/// Inserts a string into the current StringBuilder object at the specified character position.
		/// </summary>
		/// <param name="index">The position in this instance where insertion begins.</param>
		/// <param name="value">The string to insert.</param>
		/// <returns>A reference to this instance after the insert operation has completed.</returns>
		inline StringBuilder& Insert(size_t index, String value)
		{
			if (!value)
			{
				return *this;
			}

			// Convert to string, insert, and rebuild
			String current = ToString();
			String newValue = current.Insert(index, value);
			
			m_Chunks.clear();
			m_TotalLength = newValue.length();
			m_Chunks.emplace_back(std::move(newValue));
			
			return *this;
		}

		/// <summary>
		/// Removes the specified range of characters from the current StringBuilder object.
		/// </summary>
		/// <param name="startIndex">The zero-based position in this instance where removal begins.</param>
		/// <param name="length">The number of characters to remove.</param>
		/// <returns>A reference to this instance after the removal has completed.</returns>
		inline StringBuilder& Remove(size_t startIndex, size_t length)
		{
			if (startIndex >= m_TotalLength)
			{
				return *this;
			}

			// Convert to string, remove, and rebuild
			String current = ToString();
			String newValue = current.Remove(startIndex, length);
			
			m_Chunks.clear();
			m_TotalLength = newValue.length();
			if (m_TotalLength > 0)
			{
				m_Chunks.emplace_back(std::move(newValue));
			}
			
			return *this;
		}

		/// <summary>
		/// Replaces all occurrences of a specified string in the current StringBuilder object with another specified string.
		/// </summary>
		/// <param name="oldValue">The string to replace.</param>
		/// <param name="newValue">The string that replaces oldValue.</param>
		/// <returns>A reference to this instance with all instances of oldValue replaced by newValue.</returns>
		inline StringBuilder& Replace(const String& oldValue, const String& newValue)
		{
			if (!oldValue)
			{
				return *this;
			}

			// Convert to string, replace, and rebuild
			String current = ToString();
			String replaced = current.Replace(oldValue, newValue);
			
			m_Chunks.clear();
			m_TotalLength = replaced.length();
			if (m_TotalLength > 0)
			{
				m_Chunks.emplace_back(std::move(replaced));
			}
			
			return *this;
		}

		/// <summary>
		/// Removes all characters from the current StringBuilder instance.
		/// </summary>
		inline StringBuilder& Clear() noexcept
		{
			m_Chunks.clear();
			m_TotalLength = 0;
			return *this;
		}

		/// <summary>
		/// Gets or sets the length of the current StringBuilder object.
		/// </summary>
		/// <returns>The number of characters in the current instance.</returns>
		[[nodiscard]] inline constexpr size_t Length() const noexcept
		{
			return m_TotalLength;
		}

		/// <summary>
		/// Sets the length of the current StringBuilder object.
		/// </summary>
		/// <param name="value">The new length.</param>
		inline void SetLength(size_t value)
		{
			if (value == 0)
			{
				Clear();
			}
			else if (value < m_TotalLength)
			{
				// Truncate
				String current = ToString();
				m_Chunks.clear();
				m_TotalLength = value;
				m_Chunks.emplace_back(current.Substring(0, value));
			}
			else if (value > m_TotalLength)
			{
				// Extend with spaces (C# behavior)
				size_t diff = value - m_TotalLength;
				Append(' ', diff);
			}
		}

		/// <summary>
		/// Gets a value indicating whether the current StringBuilder instance is empty.
		/// </summary>
		/// <returns>true if the StringBuilder is empty; otherwise, false.</returns>
		[[nodiscard]] inline constexpr bool IsEmpty() const noexcept
		{
			return m_TotalLength == 0;
		}

		/// <summary>
		/// Converts the value of this instance to a String.
		/// </summary>
		/// <returns>A string whose value is the same as this instance.</returns>
		[[nodiscard]] inline String ToString() const
		{
			if (m_Chunks.empty())
			{
				return String::GetEmpty();
			}

			if (m_Chunks.size() == 1)
			{
				return m_Chunks[0];
			}

			return String::Concat(m_Chunks);
		}

		/// <summary>
		/// Converts a substring of this instance to a String.
		/// </summary>
		/// <param name="startIndex">The starting position of the substring.</param>
		/// <param name="length">The number of characters in the substring.</param>
		/// <returns>A string whose value is the same as the specified substring of this instance.</returns>
		[[nodiscard]] inline String ToString(size_t startIndex, size_t length) const
		{
			String full = ToString();
			return full.Substring(startIndex, length);
		}

		/// <summary>
		/// Ensures that the capacity of this instance of StringBuilder is at least the specified value.
		/// </summary>
		/// <param name="capacity">The minimum capacity to ensure.</param>
		/// <returns>The new capacity of this instance.</returns>
		inline size_t EnsureCapacity(size_t capacity)
		{
			if (m_Chunks.capacity() < capacity)
			{
				m_Chunks.reserve(capacity);
			}
			return m_Chunks.capacity();
		}

		/// <summary>
		/// Gets the number of string chunks in the current StringBuilder instance.
		/// </summary>
		/// <returns>The number of chunks.</returns>
		[[nodiscard]] inline size_t GetChunkCount() const noexcept
		{
			return m_Chunks.size();
		}

		/// <summary>
		/// Copies the characters from a specified segment of this instance to a destination String.
		/// </summary>
		/// <param name="sourceIndex">The starting position in this instance where characters will be copied from.</param>
		/// <param name="length">The number of characters to copy.</param>
		/// <returns>A new string containing the copied characters.</returns>
		[[nodiscard]] inline String CopyTo(size_t sourceIndex, size_t length) const
		{
			return ToString(sourceIndex, length);
		}
	};
}
