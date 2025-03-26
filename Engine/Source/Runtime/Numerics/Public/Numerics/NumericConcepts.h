// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	template<class TVector>
	concept TIsVectorBase = requires
	{
		{ std::declval<typename TVector::Type>() };
		{ TVector::Size() } -> std::same_as<size_t>;
		{ -std::declval<TVector>() };
		{ std::declval<TVector>()[std::declval<size_t>()] };
	};

	template<class TVector, class T>
	concept TIsVectorTyped = TIsVectorBase<TVector> && requires
	{
		{ TVector(std::declval<T>()) };
	};

	template<class TVector, size_t N>
	concept TIsVectorSized = TIsVectorBase<TVector> && (TVector::Size() >= N);

	template<class TVector, class T, size_t N>
	concept TIsVector = TIsVectorTyped<TVector, T>&& TIsVectorSized<TVector, N>;

	template<class VL, class VR>
	concept TIsCompatibleVector =
		TIsVectorBase<VL> &&
		TIsVectorBase<VR> &&
		std::same_as<typename VL::Type, typename VR::Type> &&
		(VL::Size() <= VR::Size());


	template<class TMatrix>
	concept TIsMatrixBase = TIsVectorBase<typename TMatrix::VectorType> && requires
	{
		{ std::declval<typename TMatrix::Type>() };
		{ TMatrix::Row() } -> std::same_as<size_t>;
		{ TMatrix::Column() } -> std::same_as<size_t>;
		{ -std::declval<TMatrix>() };
		{ std::declval<TMatrix>()[std::declval<size_t>()] };
		{ TMatrix::Identity() };
	};

	template<class TMatrix, class T>
	concept TIsMatrixTyped = TIsMatrixBase<TMatrix> && requires
	{
		{ TMatrix(std::declval<T>()) };
	};

	template<class TMatrix, size_t NRow, size_t NCol>
	concept TIsMatrixSized = TIsMatrixBase<TMatrix> && (TMatrix::Row() >= NRow) && (TMatrix::Column() >= NCol);

	template<class TMatrix, class T, size_t NRow, size_t NCol>
	concept TIsMatrix = TIsMatrixTyped<TMatrix, T>&& TIsMatrixSized<TMatrix, NRow, NCol>;

	template<class ML, class MR>
	concept TIsCompatibleMatrix =
		TIsMatrixBase<ML> &&
		TIsMatrixBase<MR> &&
		std::same_as<typename ML::Type, typename MR::Type> &&
		(ML::Row() <= MR::Row() && ML::Column() <= MR::Column());
}