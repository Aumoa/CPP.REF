// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/TransformInterface/Translate2D.h"
#include "Numerics/TransformInterface/Scale2D.h"
#include "Numerics/TransformInterface/Complex.h"

struct Transform2D
{
    Translate2D Translation;
    Scale2D Scale = Scale2D(1.0f);
    Complex Rotation = Complex::Identity();

    constexpr Transform2D()
    {
    }

    constexpr Transform2D(const Translate2D& T, const Scale2D& S, const Complex& C)
        : Translation(T)
        , Scale(S)
        , Rotation(C)
    {
    }

    constexpr Transform2D(const Transform2D& T)
        : Translation(T.Translation)
        , Scale(T.Scale)
        , Rotation(T.Rotation)
    {
    }

    static constexpr bool NearlyEquals(const Transform2D& TL, const Transform2D& TR, float Epsilon)
    {
        return TL.Translation.NearlyEquals(TR.Translation, Epsilon)
            && TL.Scale.NearlyEquals(TR.Scale, Epsilon)
            && TL.Rotation.NearlyEquals(TR.Rotation, Epsilon);
    }

    constexpr bool NearlyEquals(const Transform2D& T, float Epsilon) const
    {
        return NearlyEquals(*this, T, Epsilon);
    }

    static String ToString(const Transform2D& T)
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
    static constexpr Transform2D Inverse(const Transform2D& T)
    {
        return
        {
            -T.Translation,
            1.0f / T.Scale,
            T.Rotation.Inverse()
        };
    }

    constexpr Transform2D Inverse() const
    {
        return Inverse(*this);
    }

    static Transform2D Concatenate(const Transform2D& TL, const Transform2D& TR)
    {
        return Multiply(TL, TR);
    }

    Transform2D Concatenate(const Transform2D& T) const
    {
        return Concatenate(*this, T);
    }

    static constexpr Transform2D Identity()
    {
        return Transform2D(Translate2D::Identity(), Scale2D::Identity(), Complex::Identity());
    }

    template<TIsVector<float, 2> IVector>
    static constexpr IVector TransformPoint(const Transform2D& T, const IVector& V)
    {
        const IVector Scaled = V * T.Scale;
        const IVector Rotated = T.Rotation.TransformPoint(Scaled);
        return Rotated + T.Translation;
    }

    template<TIsVector<float, 2> IVector>
    constexpr IVector TransformPoint(const IVector& V) const
    {
        return TransformPoint(*this, V);
    }

    template<TIsVector<float, 2> INormal>
    static constexpr INormal TransformNormal(const Transform2D& T, const INormal& N)
    {
        const INormal Scaled = N * T.Scale;
        return T.Rotation.TransformVector(Scaled);
    }

    template<TIsVector<float, 2> INormal>
    constexpr INormal TransformNormal(const INormal& N) const
    {
        return TransformNormal(*this, N);
    }

public:
    static constexpr Transform2D GetRelativeTransform(const Transform2D& LT, const Transform2D& RT)
    {
        Scale2D RS = RT.Scale.Inverse();
        Complex InvR = RT.Rotation.Inverse();

        return Transform2D
        {
            InvR.TransformPoint(LT.Translation - RT.Translation) * RS,
            LT.Scale * RS,
            InvR * LT.Rotation
        };
    }

    constexpr Transform2D GetRelativeTransform(const Transform2D& T) const
    {
        return GetRelativeTransform(*this, T);
    }

    static Transform2D Multiply(const Transform2D& TL, const Transform2D& TR)
    {
        Matrix3x2 M = Matrix<>::Multiply(TL.GetMatrix(), TR.GetMatrix());
        Transform2D T;
        Matrix3x2::Decompose(M, T.Translation, T.Scale, T.Rotation);
        return T;
    }

    Transform2D Multiply(const Transform2D& T) const
    {
        return Multiply(*this, T);
    }

    template<TIsMatrix<float, 3, 2> IMatrix = Matrix3x2>
    static constexpr IMatrix GetMatrix(const Transform2D& T)
    {
        return Matrix3x2::AffineTransformation<IMatrix>(T.Translation, T.Scale, T.Rotation);
    }

    template<TIsMatrix<float, 3, 2> IMatrix = Matrix3x2>
    constexpr IMatrix GetMatrix() const
    {
        return GetMatrix<IMatrix>(*this);
    }

    constexpr auto operator <=>(const Transform2D& T) const = default;
};