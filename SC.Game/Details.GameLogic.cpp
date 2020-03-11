using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;
using namespace SC::Diagnostics;

using namespace std;

GameLogic::GameLogic() : Object()
{
	auto pDevice = GlobalVar.device->pDevice.Get();

	// 게임 시간을 정밀하게 측정할 타이머 도구를 생성합니다.
	timer = new StepTimer();
	physicsTimer = new StepTimer();
	physicsTimer->TargetElapsedTicksPerSecond = GlobalVar.pApp->AppConfig.PhysicsUpdatePerSeconds;
	physicsTimer->IsFixedTimeStep = true;

	// 명령 할당기 개체를 생성합니다.
	HR( pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &pCommandAllocator[0] ) ) );
	HR( pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &pCommandAllocator[1] ) ) );
	deviceContext = new CDeviceContext( GlobalVar.device, D3D12_COMMAND_LIST_TYPE_DIRECT );
	visibleViewStorage = new VisibleViewStorage( GlobalVar.device.Get() );

	// 스레드 대기를 위한 핸들을 생성합니다.
	waitingHandle = new Threading::Event();

	// 필요한 버퍼를 할당합니다.
	geometryBuffer = new GeometryBuffer( GlobalVar.device );
	hdrBuffer = new HDRBuffer( GlobalVar.device );
	hdrComputedBuffer = new HDRComputedBuffer( GlobalVar.device );
}

void GameLogic::Update()
{
	// 장면 전환 요청이 있을 경우 장면을 전환합니다.
	if ( currentScene.Get() != SceneManager::currentScene.Get() )
	{
		currentScene = SceneManager::currentScene;
	}

	// 타이머 상태를 갱신합니다.
	physicsTimer->Tick( [this]() { FixedUpdate(); } );

	// 장면 갱신 함수를 호출합니다.
	if ( currentScene->firstUpdate == false )
	{
		currentScene->Start();
		currentScene->firstUpdate = true;
	}
	currentScene->Update();

	// 장면에 포함된 모든 카메라 개체와 조명 개체를 가져옵니다.
	cameraCollection.clear();
	lightCollection.clear();

	auto iterator = currentScene->GetEnumerator();
	for ( auto i : iterator )
	{
		auto cameras = i->GetComponentsInChildren<Camera>();
		if ( cameras.size() != 0 )
		{
			cameraCollection.insert( cameraCollection.end(), cameras.begin(), cameras.end() );
		}

		auto lights = i->GetComponentsInChildren<Light>();
		if ( lights.size() != 0 )
		{
			lightCollection.insert( lightCollection.end(), lights.begin(), lights.end() );
		}
	}

	// 장면 늦은 갱신 함수를 호출합니다.
	currentScene->LateUpdate();

	// 매 프레임 업데이트가 끝난 후 스크롤 이동량을 초기화합니다.
	//scrollDelta = { 0, 0 };
}

void GameLogic::FixedUpdate()
{
	GlobalVar.fixedFrameIndex += 1;
	if ( GlobalVar.fixedFrameIndex >= 2 ) GlobalVar.fixedFrameIndex = 0;

	// 장면의 물리 갱신 함수를 호출합니다.
	currentScene->FixedUpdate();
}

void GameLogic::Render()
{
	int frameIndex = GlobalVar.frameIndex;
	auto directQueue = GlobalVar.device->DirectQueue[0].Get();

	// 렌더링을 실행하기 전 장치를 초기화합니다.
	visibleViewStorage->Reset();

	HR( pCommandAllocator[frameIndex]->Reset() );
	deviceContext->Reset( GlobalVar.device->DirectQueue[0].Get(), pCommandAllocator[frameIndex].Get(), nullptr );
	deviceContext->SetVisibleViewStorage( visibleViewStorage );

	auto pCommandList = deviceContext->pCommandList;

	if ( !cameraCollection.empty() )
	{
		bool readyLight = false;

		// 각 라이트 객체에 대해 렌더링을 수행합니다.
		for ( auto i : lightCollection )
		{
			if ( i->IsShadowCast )
			{
				if ( readyLight == false )
				{
					ShaderBuilder::ShadowCastShader_get().SetAll( deviceContext );
					readyLight = true;
				}

				i->BeginDraw( deviceContext );
				i->SetGraphicsRootShaderResources( deviceContext );
				currentScene->Render( deviceContext );
				i->EndDraw( deviceContext );
			}
		}

		// 각 카메라 개체에 대해 렌더링을 수행합니다.
		for ( auto i : cameraCollection )
		{
			// 기하 버퍼를 렌더 타겟으로 설정합니다.
			geometryBuffer->OMSetRenderTargets( deviceContext );

			if ( auto skybox = i->ClearMode.TryAs<CameraClearModeSkybox>(); skybox )
			{
				// 장면이 스카이박스를 사용할 경우 우선 스카이박스 렌더링을 진행합니다.
				ShaderBuilder::SkyboxShader_get().SetAll( deviceContext );

				i->SetGraphicsRootConstantBufferView( deviceContext );
				deviceContext->SetGraphicsRootShaderResources( deviceContext->Slot["Texture"], skybox->SkyboxTexture->pShaderResourceView );
				skyboxMesh->DrawIndexed( deviceContext );
			}

			// 기하 버퍼 셰이더를 선택합니다.
			ShaderBuilder::ColorShader_get().SetAll( deviceContext );

			// 장면에 포함된 모든 개체를 렌더링합니다.
			i->SetGraphicsRootConstantBufferView( deviceContext );
			currentScene->Render( deviceContext );

			// 기하 버퍼 사용을 종료합니다.
			geometryBuffer->EndDraw( deviceContext );

			// HDR 버퍼를 렌더 타겟으로 설정합니다.
			hdrBuffer->OMSetRenderTargets( deviceContext );

			// HDR 렌더링을 수행합니다.
			ShaderBuilder::HDRShader_get().SetAll( deviceContext );

			// 매개변수를 입력합니다.
			i->SetGraphicsRootConstantBufferView( deviceContext );
			geometryBuffer->SetGraphicsRootShaderResourcesForLayer( deviceContext );

			// 각 조명에 대해 조명 렌더 패스를 진행합니다.
			pCommandList->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
			Material::SetGraphicsRootShaderResources( deviceContext );

			for ( auto i : lightCollection )
			{
				i->SetGraphicsRootShaderResources( deviceContext );
				pCommandList->DrawInstanced( 4, 1, 0, 0 );
			}

			// HDR 색상 렌더링을 수행합니다.
			ShaderBuilder::HDRColorShader_get().SetAll( deviceContext );

			// 매개변수를 입력합니다.
			geometryBuffer->SetGraphicsRootShaderResourcesForColor( deviceContext );

			// HDR 버퍼에 색상 렌더링을 수행합니다.
			pCommandList->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
			pCommandList->DrawInstanced( 4, 1, 0, 0 );

			// HDR 버퍼 사용을 종료합니다.
			hdrBuffer->EndDraw( deviceContext );

			// 시간 경과를 조회합니다.
			timer->Tick();
			double t = timer->ElapsedSeconds;

			// 매개변수를 입력합니다.
			ShaderBuilder::HDRComputeShader_get( 0 ).SetAll( deviceContext );
			hdrComputedBuffer->SetComputeRootTimestamp( deviceContext, t );
			hdrBuffer->SetComputeRootShaderResources( deviceContext );
			
			// 렌더 패스를 수행합니다.
			for ( int i = 0; i < 3; ++i )
			{
				// 셰이더를 선택합니다.
				if ( i != 0 ) ShaderBuilder::HDRComputeShader_get( i ).SetAll( deviceContext );
				hdrComputedBuffer->Compute( deviceContext, i );
			}
		}
	}

	// 현재 후면 버퍼의 인덱스를 가져옵니다.
	auto idx = GlobalVar.swapChain->Index;
	auto pBackBuffer = GlobalVar.swapChain->ppBackBuffers[idx].Get();
	auto rtvHandle = GlobalVar.swapChain->RTVHandle[idx];

	// 스왑 체인의 후면 버퍼를 렌더 타겟으로 설정합니다.
	deviceContext->TransitionBarrier( pBackBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET, 0 );
	pCommandList->OMSetRenderTargets( 1, &rtvHandle, FALSE, nullptr );

	// 후면 버퍼를 단색으로 초기화합니다.
	constexpr float clear[4]{ 0.0f };
	pCommandList->ClearRenderTargetView( rtvHandle, clear, 0, nullptr );

	if ( !cameraCollection.empty() )
	{
		// 톤 매핑 렌더링을 수행합니다.
		ShaderBuilder::ToneMappingShader_get().SetAll( deviceContext );

		// HDR 원본 텍스처 및 계산된 HDR 상수를 설정하고 렌더링을 수행합니다.
		//geometryBuffer->SetGraphicsRootShaderResourcesForColor( deviceContext );
		hdrBuffer->SetGraphicsRootShaderResources( deviceContext );
		hdrComputedBuffer->SetGraphicsRootConstantBuffers( deviceContext );
		pCommandList->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
		pCommandList->DrawInstanced( 4, 1, 0, 0 );
	}

	// 명령 목록을 닫고 푸쉬합니다.
	deviceContext->Close();
	directQueue->Execute( deviceContext );

	// 마지막 명령 번호를 저장합니다.
	lastPending[frameIndex++] = directQueue->Signal();

	// 프레임 인덱스를 한정합니다.
	if ( frameIndex >= 2 ) frameIndex -= 2;
}

void GameLogic::ResizeBuffers( uint32 width, uint32 height )
{
	// 필요한 각종 버퍼에게 크기 변경 요청을 수행합니다.
	geometryBuffer->ResizeBuffers( width, height );
	hdrBuffer->ResizeBuffers( width, height );
	hdrComputedBuffer->ResizeBuffers( width, height );
}