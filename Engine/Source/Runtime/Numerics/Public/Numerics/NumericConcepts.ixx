// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Numerics:NumericConcepts;

export import Core;

export template<class TVector>
concept TIsVectorBase = requires
{
	{ std::declval<typename TVector::Type>() };
	{ TVector::Size() } -> std::same_as<size_t>;
	{ -std::declval<TVector>() };
	{ std::declval<TVector>()[std::declval<size_t>()] };
};

export template<class TVector, class T>
concept TIsVectorTyped = TIsVectorBase<TVector> && requires
{
	{ TVector(std::declval<T>()) };
};

export template<class TVector, size_t N>
concept TIsVectorSized = TIsVectorBase<TVector> && (TVector::Size() >= N);

export template<class TVector, class T, size_t N>
concept TIsVector = TIsVectorTyped<TVector, T> && TIsVectorSized<TVector, N>;

export template<class VL, class VR>
concept TIsCompatibleVector =
	TIsVectorBase<VL> &&
	TIsVectorBase<VR> &&
	std::same_as<typename VL::Type, typename VR::Type> &&
	(VL::Size() <= VR::Size());


export template<class TMatrix>
concept TIsMatrixBase = TIsVectorBase<typename TMatrix::VectorType> && requires
{
	{ std::declval<typename TMatrix::Type>() };
	{ TMatrix::Row() } -> std::same_as<size_t>;
	{ TMatrix::Column() } -> std::same_as<size_t>;
	{ -std::declval<TMatrix>() };
	{ std::declval<TMatrix>()[std::declval<size_t>()] };
	{ TMatrix::Identity() };
};

export template<class TMatrix, class T>
concept TIsMatrixTyped = TIsMatrixBase<TMatrix> && requires
{
	{ TMatrix(std::declval<T>()) };
};

export template<class TMatrix, size_t NRow, size_t NCol>
concept TIsMatrixSized = TIsMatrixBase<TMatrix> && (TMatrix::Row() >= NRow) && (TMatrix::Column() >= NCol);

export template<class TMatrix, class T, size_t NRow, size_t NCol>
concept TIsMatrix = TIsMatrixTyped<TMatrix, T> && TIsMatrixSized<TMatrix, NRow, NCol>;

export template<class ML, class MR>
concept TIsCompatibleMatrix =
	TIsMatrixBase<ML> &&
	TIsMatrixBase<MR> &&
	std::same_as<typename ML::Type, typename MR::Type> &&
	(ML::Row() <= MR::Row() && ML::Column() <= MR::Column());