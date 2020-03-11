#pragma once

/// <summary> �Ű� ������ ���� �ʴ� ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define delegate_0( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this )

/// <summary> �Ű� ������ ���� �ʴ� ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define callback_0( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer )

/// <summary> �� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define delegate_1( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1 )

/// <summary> �� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define callback_1( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1 )

/// <summary> �� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define delegate_2( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1, std::placeholders::_2 )

/// <summary> �� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define callback_2( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1, std::placeholders::_2 )

/// <summary> �� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define delegate_3( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 )

/// <summary> �� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define callback_3( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 )

/// <summary> �� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define delegate_4( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 )

/// <summary> �� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define callback_4( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 )

/// <summary> �ټ� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define delegate_5( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5 )

/// <summary> �ټ� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define callback_5( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5 )

/// <summary> ���� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define delegate_6( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6 )

/// <summary> ���� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define callback_6( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6 )

/// <summary> �ϰ� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define delegate_7( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, std::placeholders::_7 )

/// <summary> �ϰ� ���� �Ű� ������ ������ ��� �Լ��� ���� �븮�� ����� �����մϴ�. </summary>
#define callback_7( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, std::placeholders::_7 )