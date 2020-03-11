#pragma once

namespace SC
{
	/// <summary> ������ ���Ŀ� ���� ��ü�� ���� ���ڿ��� ��ȯ�Ͽ� �� ���ڿ� ��ü�� �����մϴ�. </summary>
	/// <param name="formatText"> �ռ� ���� �ؽ�Ʈ�Դϴ�. </param>
	/// <param name="args"> �ؽ�Ʈ �������� ��ȯ ������ ���� �Ű������� �����մϴ�. </param>
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