// Copyright 2020 Aumoa.lib. All right reserved.

template<class T> requires TIsBaseOf<T, APawn>
T* AController::GetPawn() const
{
	return Cast<T>(GetPawn());
}