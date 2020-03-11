#pragma once

namespace SC
{
	/// <summary> SC Ŭ���� ���Ŀ� ���Ե��� ���� ����Ƽ�� Ŭ���� ���Ŀ� ���� �����Դϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	template< class T >
	class NativeType final : public ValueType
	{
		T* ptr = nullptr;

	public:
		/// <summary> <see cref="NativeType"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="value"> ������ ������ ����Ƽ�� ���� �����մϴ�. </param>
		template< typename std::enable_if_t<!where<Object, T>> * = nullptr >
		inline NativeType( const T& value );
		inline ~NativeType() override;

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		inline String ToString() override;

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ������ ��ü�� ���� ��ü�� ������ Ȯ���մϴ�. </summary>
		/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
		inline bool Equals( object obj ) override;

		/// <summary> ����Ƽ�� �� ������ ��ڽ��մϴ�. </summary>
		inline T Unboxing();

	private:
		template< class O, std::enable_if_t<Details::HasOperator<O>::value>* = nullptr >
		inline String ToStringT()
		{
			std::wstringstream wss;
			wss << *ptr;
			return wss.str().c_str();
		}

		template< class O, std::enable_if_t<!Details::HasOperator<O>::value>* = nullptr >
		inline String ToStringT()
		{
			return String::Format( "SC.NativeType<{0}>", new String( typeid( O ).name() ) );
		}
	};
}