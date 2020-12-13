// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#ifdef __SC_RUNTIME_CORE_API__
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

#ifndef interface
#define interface struct
#endif

#define vs_property_get( Type, Name )\
__declspec( property( get = Name##_get ) ) Type Name

#define vs_property_set( Type, Name )\
__declspec( property( put = Name##_set ) ) Type Name

#define vs_property( Type, Name )\
__declspec( property( get = Name##_get, put = Name##_set ) ) Type Name

#define vs_property_auto( Type, Name )\
vs_property( Type, Name );\
Type Name##_get();\
void Name##_set( Type value );

#define vs_property_get_auto( Type, Name )\
vs_property_get( Type, Name );\
Type Name##_get();

#define vs_property_set_auto( Type, Name )\
vs_property_set( Type, Name );\
void Name##_set( Type value );

using uint8 = unsigned char;
using int8 = signed char;
using uint16 = unsigned short;
using int16 = signed short;
using uint32 = unsigned int;
using int32 = signed int;
using uint64 = unsigned long long int;
using int64 = signed long long int;