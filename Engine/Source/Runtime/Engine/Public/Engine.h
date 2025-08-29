// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.gen.h"

namespace Ayla
{
	class Graphics;
	class GenericActivity;
	class GenericWindowSwapchainExtension;
	class RenderThread;
	class GameInstance;
	class CommandLineParser;

	ACLASS()
	class ENGINE_API Engine : public Object
	{
		GENERATED_BODY()

	private:
		std::shared_ptr<GenericActivity> m_MainActivity;
		std::shared_ptr<Graphics> m_Graphics;
		std::vector<std::shared_ptr<GenericWindowSwapchainExtension>> m_SwapchainExtensions;
		std::unique_ptr<RenderThread> m_RenderThread;

		APROPERTY()
		PPtr<GameInstance> m_GameInstance;

	public:
		Engine();
		virtual ~Engine() noexcept override;

		virtual void PreInitialize();
		virtual void Initialize(const CommandLineParser* args);
		virtual void Shutdown();

		virtual void Tick();

		template<TransferFunction F>
		constexpr void Transfer(F&& transfer)
		{
			using class_t = reflexpr(Engine);
			using members_t = std::reflect::get_data_members_t<class_t>;
			Transfer2(std::forward<F>(transfer), std::make_index_sequence<std::tuple_size_v<members_t>>{});
		}

		template<TransferFunction F, std::size_t... I>
		constexpr void Transfer2(F&& transfer, std::index_sequence<I...>&&)
		{
			using class_t = reflexpr(Engine);
			using members_t = std::reflect::get_data_members_t<class_t>;
			((transfer.template Transfer<std::tuple_element_t<I, members_t>>(), 0) + ...);
		}

	private:
		void InitializeActivity();
		void InitializeGraphics();
		void InitializeGame(const CommandLineParser* args);
		void PostInitialized();
	};
}