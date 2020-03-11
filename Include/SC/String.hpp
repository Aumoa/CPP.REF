#pragma once

namespace SC
{
	/// <summary> 지정된 형식에 따라 개체의 값을 문자열로 변환하여 새 문자열 개체를 생성합니다. </summary>
	/// <param name="formatText"> 합성 서식 텍스트입니다. </param>
	/// <param name="args"> 텍스트 형식으로 변환 가능한 형식 매개변수를 전달합니다. </param>
	template< class... _Args >
	String String::Format( String formatText, _Args&&... args )
	{
		using namespace std;

		vector<object> argsVector;
		Format_Helper( argsVector, args... );

		return Format( formatText, argsVector );
	}

	template< class... _Args >
	void String::Format_Helper( std::vector<object>& vec, object arg0, _Args&&... args )
	{
		using namespace std;

		vec.push_back( arg0 );
		arg0.Reset();
		Format_Helper( vec, args... );
	}

	void String::Format_Helper( std::vector<object>& vec, object arg0 )
	{
		using namespace std;

		vec.push_back( arg0 );
		arg0.Reset();
	}

	void String::Format_Helper( std::vector<object>& vec )
	{

	}
}