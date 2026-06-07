// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/DirectXMath.h"

namespace Ayla
{
	namespace GeometryHelper
	{
#if defined(_MSC_VER)
		template<class T>
		concept IsDirectXVector = std::convertible_to<T, DirectX::XMVECTOR> || std::convertible_to<T, DirectX::XMVECTORF32>;
#else
		template<class T>
		concept IsDirectXVector = std::convertible_to<T, DirectX::XMVECTORF32>;
#endif

		template<class T>
		concept IsMutableVector2 = requires (T & v)
		{
			{ v.x } -> std::convertible_to<float&>;
			{ v.y } -> std::convertible_to<float&>;
		} || requires (T & v)
		{
			{ v.X } -> std::convertible_to<float&>;
			{ v.Y } -> std::convertible_to<float&>;
		};

		template<class T>
		concept IsVector2 = IsMutableVector2<T> || IsDirectXVector<T>;

		template<class T>
		concept IsMutableVector3 = requires (T & v)
		{
			{ v.x } -> std::convertible_to<float&>;
			{ v.y } -> std::convertible_to<float&>;
			{ v.z } -> std::convertible_to<float&>;
		} || requires (T & v)
		{
			{ v.X } -> std::convertible_to<float&>;
			{ v.Y } -> std::convertible_to<float&>;
			{ v.Z } -> std::convertible_to<float&>;
		};

		template<class T>
		concept IsVector3 = IsMutableVector3<T> || IsDirectXVector<T>;

		template<class T>
		concept IsVertex = requires (T & v)
		{
			{ v.Position } -> IsMutableVector3;
			{ v.Normal } -> IsMutableVector3;
			{ v.Tex } -> IsMutableVector2;
		};

		template<class T>
		concept IsIndex = sizeof(T) >= sizeof(uint16) && std::convertible_to<T, uint16>;
	}

	class VertexHelper
	{
	public:
		template<class TVector>
		inline static constexpr float& GetX(TVector& v) requires requires { { v.x } -> std::convertible_to<float>; }
		{
			return v.x;
		}

		template<class TVector>
		inline static constexpr float& GetX(TVector& v) requires requires { { v.X } -> std::convertible_to<float>; }
		{
			return v.X;
		}

		template<class TVector>
		inline static constexpr const float& GetX(const TVector& v) requires requires { { v.x } -> std::convertible_to<float>; }
		{
			return v.x;
		}

		template<class TVector>
		inline static constexpr const float& GetX(const TVector& v) requires requires { { v.X } -> std::convertible_to<float>; }
		{
			return v.X;
		}

		inline static float GetX(const DirectX::FXMVECTOR v)
		{
			return DirectX::XMVectorGetX(v);
		}

		template<class TVector>
		inline static constexpr float& GetY(TVector& v) requires requires { { v.y } -> std::convertible_to<float>; }
		{
			return v.y;
		}

		template<class TVector>
		inline static constexpr float& GetY(TVector& v) requires requires { { v.Y } -> std::convertible_to<float>; }
		{
			return v.Y;
		}

		template<class TVector>
		inline static constexpr const float& GetY(const TVector& v) requires requires { { v.y } -> std::convertible_to<float>; }
		{
			return v.y;
		}

		template<class TVector>
		inline static constexpr const float& GetY(const TVector& v) requires requires { { v.Y } -> std::convertible_to<float>; }
		{
			return v.Y;
		}

		inline static float GetY(const DirectX::FXMVECTOR v)
		{
			return DirectX::XMVectorGetY(v);
		}

		template<class TVector>
		inline static constexpr float& GetZ(TVector& v) requires requires { { v.z } -> std::convertible_to<float>; }
		{
			return v.z;
		}

		template<class TVector>
		inline static constexpr float& GetZ(TVector& v) requires requires { { v.Z } -> std::convertible_to<float>; }
		{
			return v.Z;
		}

		template<class TVector>
		inline static constexpr const float& GetZ(const TVector& v) requires requires { { v.z } -> std::convertible_to<float>; }
		{
			return v.z;
		}

		template<class TVector>
		inline static constexpr const float& GetZ(const TVector& v) requires requires { { v.Z } -> std::convertible_to<float>; }
		{
			return v.Z;
		}

		inline static float GetZ(const DirectX::FXMVECTOR v)
		{
			return DirectX::XMVectorGetZ(v);
		}

		template<GeometryHelper::IsMutableVector2 TVector>
		inline static constexpr TVector& Set(TVector& v, float x, float y, float z = 0)
		{
			VertexHelper::GetX(v) = x;
			VertexHelper::GetY(v) = y;
			if constexpr (GeometryHelper::IsMutableVector3<TVector>)
			{
				VertexHelper::GetZ(v) = z;
			}
			return v;
		}

		template<GeometryHelper::IsMutableVector2 TVectorA, GeometryHelper::IsMutableVector2 TVectorB>
		inline static constexpr TVectorA& Set(TVectorA& v, TVectorB& assign)
		{
			GetX(v) = GetX(assign);
			GetY(v) = GetY(assign);
			if constexpr (GeometryHelper::IsMutableVector3<TVectorA> && GeometryHelper::IsMutableVector3<TVectorB>)
			{
				GetZ(v) = GetZ(assign);
			}
			return v;
		}

		template<GeometryHelper::IsMutableVector2 TVectorA>
		inline static constexpr TVectorA& Set(TVectorA& v, const DirectX::FXMVECTOR assign)
		{
			if constexpr (GeometryHelper::IsMutableVector3<TVectorA>)
			{
				DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&v), assign);
			}
			else if constexpr (GeometryHelper::IsMutableVector2<TVectorA>)
			{
				DirectX::XMStoreFloat2(reinterpret_cast<DirectX::XMFLOAT2*>(&v), assign);
			}
			else
			{
				static_assert(details::always_false<0>, "Unsupported vector type");
			}

			return v;
		}

		template<GeometryHelper::IsMutableVector2 TVectorA>
		inline static constexpr TVectorA& Set(TVectorA& v, const DirectX::XMVECTORF32& assign)
		{
			return Set(v, static_cast<DirectX::FXMVECTOR>(assign));
		}

		template<GeometryHelper::IsVertex TVertex, GeometryHelper::IsVector3 TPos, GeometryHelper::IsVector3 TNormal, GeometryHelper::IsVector2 TTex>
		inline static constexpr TVertex Make(const TPos& pos, const TNormal& normal, const TTex& tex)
		{
			TVertex v = {};
			Set(v.Position, pos);
			Set(v.Normal, normal);
			Set(v.Tex, tex);
			return v;
		}
	};
}
