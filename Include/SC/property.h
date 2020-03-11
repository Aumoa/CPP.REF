#pragma once

#if defined( _VISUAL_STUDIO_EDITOR_ )

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

#else

#define vs_property_get( Type, Name )

#define vs_property_set( Type, Name )

#define vs_property( Type, Name )

#endif