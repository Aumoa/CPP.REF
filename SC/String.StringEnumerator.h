#pragma once

namespace SC
{
	class String::StringEnumerator : virtual public Object, virtual public IEnumerator<wchar_t>
	{
		int indexOf = -1;
		const wchar_t* bufferPtr = nullptr;
		int length = 0;

	public:
		StringEnumerator( const wchar_t* bufferPtr, int length );

		bool MoveNext() override;
		void Reset() override;
		wchar_t Current_get() override;
	};
}