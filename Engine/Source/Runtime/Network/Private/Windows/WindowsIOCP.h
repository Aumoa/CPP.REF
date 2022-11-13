// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"

//#if PLATFORM_WINDOWS
//
//#include "WindowsMinimal.h"
//#include "WindowsIOCP.gen.h"
//
//class ThreadGroup;
//
//SCLASS()
//class WindowsIOCP : public IOCompletionPort
//{
//	GENERATED_BODY()
//
//private:
//	HANDLE _hComp = NULL;
//	std::unique_ptr<ThreadGroup> _threads;
//	WSAData _data;
//
//public:
//	WindowsIOCP();
//	virtual ~WindowsIOCP() noexcept;
//
//protected:
//	virtual void HandleChainSocket(ImplSocket* impl) override;
//
//private:
//	void IOCPThreadStart();
//};
//
//#endif