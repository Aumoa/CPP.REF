#pragma once

/// <summary> 매개 변수를 가지 않는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define delegate_0( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this )

/// <summary> 매개 변수를 가지 않는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define callback_0( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer )

/// <summary> 한 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define delegate_1( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1 )

/// <summary> 한 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define callback_1( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1 )

/// <summary> 두 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define delegate_2( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1, std::placeholders::_2 )

/// <summary> 두 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define callback_2( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1, std::placeholders::_2 )

/// <summary> 세 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define delegate_3( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 )

/// <summary> 세 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define callback_3( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 )

/// <summary> 네 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define delegate_4( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 )

/// <summary> 네 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define callback_4( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 )

/// <summary> 다섯 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define delegate_5( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5 )

/// <summary> 다섯 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define callback_5( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5 )

/// <summary> 여섯 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define delegate_6( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6 )

/// <summary> 여섯 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define callback_6( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6 )

/// <summary> 일곱 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define delegate_7( function_name ) std::bind( &std::remove_reference_t< decltype( *this ) >:: ## function_name, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, std::placeholders::_7 )

/// <summary> 일곱 개의 매개 변수를 가지는 멤버 함수에 대한 대리자 등록을 제공합니다. </summary>
#define callback_7( pointer, function_name ) std::bind( &std::remove_reference_t< decltype( *pointer ) >:: ## function_name, pointer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, std::placeholders::_7 )