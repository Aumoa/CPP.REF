// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/MatrixInterface/Matrix4x4.h"

namespace Ayla
{
    template<class T>
    struct Transform
    {
        Translate3D<T> Translation;
        Scale3D<T> Scale = Scale3D<T>(1.0);
        Quaternion<T> Rotation = Quaternion<T>::Identity();

        constexpr Transform()
        {
        }

        constexpr Transform(const Translate3D<T>& T, const Scale3D<T>& S, const Quaternion<T>& Q)
            : Translation(T)
            , Scale(S)
            , Rotation(Q)
        {
        }

        constexpr Transform(const Transform& T)
            : Translation(T.Translation)
            , Scale(T.Scale)
            , Rotation(T.Rotation)
        {
        }

        static constexpr bool NearlyEquals(const Transform& TL, const Transform& TR, float Epsilon)
        {
            return TL.Translation.NearlyEquals(TR.Translation, Epsilon)
                && TL.Scale.NearlyEquals(TR.Scale, Epsilon)
                && TL.Rotation.NearlyEquals(TR.Rotation, Epsilon);
        }

        constexpr bool NearlyEquals(const Transform& T, float Epsilon) const
        {
            return NearlyEquals(*this, T, Epsilon);
        }

        static String ToString(const Transform& T)
        {
            return String::Format(TEXT("{{T: {}, S: {}, R: {}}}"),
                T.Translation.ToString(),
                T.Scale.ToString(),
                T.Rotation.ToString());
        }

        String ToString()
        {
            return ToString(*this);
        }

    public:
        static constexpr Transform Inverse(const Transform& T)
        {
            return
            {
                -T.Translation,
                1.0f / T.Scale,
                T.Rotation.Inverse()
            };
        }

        constexpr Transform Inverse() const
        {
            return Inverse(*this);
        }

        static Transform Concatenate(const Transform& TL, const Transform& TR)
        {
            return Multiply(TL, TR);
        }

        Transform Concatenate(const Transform& T) const
        {
            return Concatenate(*this, T);
        }

        static constexpr Transform Identity()
        {
            return Transform(Translate3D<T>::Identity(), Scale3D<T>::Identity(), Quaternion<T>::Identity());
        }

        template<TIsVector<float, 3> IVector>
        static constexpr IVector TransformPoint(const Transform& T, const IVector& V)
        {
            const IVector Scaled = V * T.Scale;
            const IVector Rotated = T.Rotation.TransformPoint(Scaled);
            return Rotated + T.Translation;
        }

        template<TIsVector<float, 3> IVector>
        constexpr IVector TransformPoint(const IVector& V) const
        {
            return TransformPoint(*this, V);
        }

        template<TIsVector<float, 3> INormal>
        static constexpr INormal TransformNormal(const Transform& T, const INormal& N)
        {
            const INormal Scaled = N * T.Scale;
            return T.Rotation.TransformVector(Scaled);
        }

        template<TIsVector<float, 3> INormal>
        constexpr INormal TransformNormal(const INormal& N) const
        {
            return TransformNormal(*this, N);
        }

    public:
        static constexpr Transform GetRelativeTransform(const Transform& LT, const Transform& RT)
        {
            Scale3D<T> RS = RT.Scale.Inverse();
            Quaternion<T> InvR = RT.Rotation.Inverse();

            return Transform
            {
                InvR.TransformPoint(LT.Translation - RT.Translation) * RS,
                LT.Scale * RS,
                InvR * LT.Rotation
            };
        }

        constexpr Transform GetRelativeTransform(const Transform& T) const
        {
            return GetRelativeTransform(*this, T);
        }

        static Transform Multiply(const Transform& TL, const Transform& TR)
        {
            Matrix4x4 M = Matrix4x4::Multiply(TL.GetMatrix(), TR.GetMatrix());
            Transform T;
            Matrix4x4::Decompose(M, T.Translation, T.Scale, T.Rotation);
            return T;
        }

        Transform Multiply(const Transform& T) const
        {
            return Multiply(*this, T);
        }

        template<TIsMatrix<float, 4, 4> IMatrix = Matrix4x4>
        static IMatrix GetMatrix(const Transform& T)
        {
            return Matrix4x4::AffineTransformation(T.Translation, T.Scale, T.Rotation);
        }

        template<TIsMatrix<float, 4, 4> IMatrix = Matrix4x4>
        IMatrix GetMatrix() const
        {
            return GetMatrix<IMatrix>(*this);
        }

        constexpr auto operator <=>(const Transform& T) const = default;

    public:
        template<class ITransformL, class ITransformR, class... TTransforms>
        static constexpr auto Concatenate(const ITransformL& TL, const ITransformR& TR, const TTransforms&... Args)
        {
            if constexpr (sizeof...(TTransforms) > 0)
            {
                return Concatenate(ConcatenateTransparent(0, TL, TR), Args...);
            }
            else
            {
                return ConcatenateTransparent(0, TL, TR);
            }
        }

    private:
        template<class ITransformL, class ITransformR>
        static constexpr auto ConcatenateTransparent(int, const ITransformL& TL, const ITransformR& TR) requires requires
        {
            { std::declval<ITransformL>().Concatenate(std::declval<ITransformR>()) };
        }
        {
            return TL.Concatenate(TR);
        }

        template<class ITransformL, class ITransformR>
        static constexpr auto ConcatenateTransparent(short, const ITransformL& TL, const ITransformR& TR) requires requires
        {
            { ITransformR::Concatenate(std::declval<ITransformL>(), std::declval<ITransformR>()) };
        }
        {
            return ITransformR::Concatenate(TL, TR);
        }
    };
}