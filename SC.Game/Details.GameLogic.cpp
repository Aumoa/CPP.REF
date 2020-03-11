using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;
using namespace SC::Diagnostics;

using namespace std;

GameLogic::GameLogic() : Object()
{
	auto pDevice = GlobalVar.device->pDevice.Get();

	// ���� �ð��� �����ϰ� ������ Ÿ�̸� ������ �����մϴ�.
	timer = new StepTimer();
	physicsTimer = new StepTimer();
	physicsTimer->TargetElapsedTicksPerSecond = GlobalVar.pApp->AppConfig.PhysicsUpdatePerSeconds;
	physicsTimer->IsFixedTimeStep = true;

	// ��� �Ҵ�� ��ü�� �����մϴ�.
	HR( pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &pCommandAllocator[0] ) ) );
	HR( pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &pCommandAllocator[1] ) ) );
	deviceContext = new CDeviceContext( GlobalVar.device, D3D12_COMMAND_LIST_TYPE_DIRECT );
	visibleViewStorage = new VisibleViewStorage( GlobalVar.device.Get() );

	// ������ ��⸦ ���� �ڵ��� �����մϴ�.
	waitingHandle = new Threading::Event();

	// �ʿ��� ���۸� �Ҵ��մϴ�.
	geometryBuffer = new GeometryBuffer( GlobalVar.device );
	hdrBuffer = new HDRBuffer( GlobalVar.device );
	hdrComputedBuffer = new HDRComputedBuffer( GlobalVar.device );
}

void GameLogic::Update()
{
	// ��� ��ȯ ��û�� ���� ��� ����� ��ȯ�մϴ�.
	if ( currentScene.Get() != SceneManager::currentScene.Get() )
	{
		currentScene = SceneManager::currentScene;
	}

	// Ÿ�̸� ���¸� �����մϴ�.
	physicsTimer->Tick( [this]() { FixedUpdate(); } );

	// ��� ���� �Լ��� ȣ���մϴ�.
	if ( currentScene->firstUpdate == false )
	{
		currentScene->Start();
		currentScene->firstUpdate = true;
	}
	currentScene->Update();

	// ��鿡 ���Ե� ��� ī�޶� ��ü�� ���� ��ü�� �����ɴϴ�.
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

	// ��� ���� ���� �Լ��� ȣ���մϴ�.
	currentScene->LateUpdate();

	// �� ������ ������Ʈ�� ���� �� ��ũ�� �̵����� �ʱ�ȭ�մϴ�.
	//scrollDelta = { 0, 0 };
}

void GameLogic::FixedUpdate()
{
	GlobalVar.fixedFrameIndex += 1;
	if ( GlobalVar.fixedFrameIndex >= 2 ) GlobalVar.fixedFrameIndex = 0;

	// ����� ���� ���� �Լ��� ȣ���մϴ�.
	currentScene->FixedUpdate();
}

void GameLogic::Render()
{
	int frameIndex = GlobalVar.frameIndex;
	auto directQueue = GlobalVar.device->DirectQueue[0].Get();

	// �������� �����ϱ� �� ��ġ�� �ʱ�ȭ�մϴ�.
	visibleViewStorage->Reset();

	HR( pCommandAllocator[frameIndex]->Reset() );
	deviceContext->Reset( GlobalVar.device->DirectQueue[0].Get(), pCommandAllocator[frameIndex].Get(), nullptr );
	deviceContext->SetVisibleViewStorage( visibleViewStorage );

	auto pCommandList = deviceContext->pCommandList;

	if ( !cameraCollection.empty() )
	{
		bool readyLight = false;

		// �� ����Ʈ ��ü�� ���� �������� �����մϴ�.
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

		// �� ī�޶� ��ü�� ���� �������� �����մϴ�.
		for ( auto i : cameraCollection )
		{
			// ���� ���۸� ���� Ÿ������ �����մϴ�.
			geometryBuffer->OMSetRenderTargets( deviceContext );

			if ( auto skybox = i->ClearMode.TryAs<CameraClearModeSkybox>(); skybox )
			{
				// ����� ��ī�̹ڽ��� ����� ��� �켱 ��ī�̹ڽ� �������� �����մϴ�.
				ShaderBuilder::SkyboxShader_get().SetAll( deviceContext );

				i->SetGraphicsRootConstantBufferView( deviceContext );
				deviceContext->SetGraphicsRootShaderResources( deviceContext->Slot["Texture"], skybox->SkyboxTexture->pShaderResourceView );
				skyboxMesh->DrawIndexed( deviceContext );
			}

			// ���� ���� ���̴��� �����մϴ�.
			ShaderBuilder::ColorShader_get().SetAll( deviceContext );

			// ��鿡 ���Ե� ��� ��ü�� �������մϴ�.
			i->SetGraphicsRootConstantBufferView( deviceContext );
			currentScene->Render( deviceContext );

			// ���� ���� ����� �����մϴ�.
			geometryBuffer->EndDraw( deviceContext );

			// HDR ���۸� ���� Ÿ������ �����մϴ�.
			hdrBuffer->OMSetRenderTargets( deviceContext );

			// HDR �������� �����մϴ�.
			ShaderBuilder::HDRShader_get().SetAll( deviceContext );

			// �Ű������� �Է��մϴ�.
			i->SetGraphicsRootConstantBufferView( deviceContext );
			geometryBuffer->SetGraphicsRootShaderResourcesForLayer( deviceContext );

			// �� ���� ���� ���� ���� �н��� �����մϴ�.
			pCommandList->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
			Material::SetGraphicsRootShaderResources( deviceContext );

			for ( auto i : lightCollection )
			{
				i->SetGraphicsRootShaderResources( deviceContext );
				pCommandList->DrawInstanced( 4, 1, 0, 0 );
			}

			// HDR ���� �������� �����մϴ�.
			ShaderBuilder::HDRColorShader_get().SetAll( deviceContext );

			// �Ű������� �Է��մϴ�.
			geometryBuffer->SetGraphicsRootShaderResourcesForColor( deviceContext );

			// HDR ���ۿ� ���� �������� �����մϴ�.
			pCommandList->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
			pCommandList->DrawInstanced( 4, 1, 0, 0 );

			// HDR ���� ����� �����մϴ�.
			hdrBuffer->EndDraw( deviceContext );

			// �ð� ����� ��ȸ�մϴ�.
			timer->Tick();
			double t = timer->ElapsedSeconds;

			// �Ű������� �Է��մϴ�.
			ShaderBuilder::HDRComputeShader_get( 0 ).SetAll( deviceContext );
			hdrComputedBuffer->SetComputeRootTimestamp( deviceContext, t );
			hdrBuffer->SetComputeRootShaderResources( deviceContext );
			
			// ���� �н��� �����մϴ�.
			for ( int i = 0; i < 3; ++i )
			{
				// ���̴��� �����մϴ�.
				if ( i != 0 ) ShaderBuilder::HDRComputeShader_get( i ).SetAll( deviceContext );
				hdrComputedBuffer->Compute( deviceContext, i );
			}
		}
	}

	// ���� �ĸ� ������ �ε����� �����ɴϴ�.
	auto idx = GlobalVar.swapChain->Index;
	auto pBackBuffer = GlobalVar.swapChain->ppBackBuffers[idx].Get();
	auto rtvHandle = GlobalVar.swapChain->RTVHandle[idx];

	// ���� ü���� �ĸ� ���۸� ���� Ÿ������ �����մϴ�.
	deviceContext->TransitionBarrier( pBackBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET, 0 );
	pCommandList->OMSetRenderTargets( 1, &rtvHandle, FALSE, nullptr );

	// �ĸ� ���۸� �ܻ����� �ʱ�ȭ�մϴ�.
	constexpr float clear[4]{ 0.0f };
	pCommandList->ClearRenderTargetView( rtvHandle, clear, 0, nullptr );

	if ( !cameraCollection.empty() )
	{
		// �� ���� �������� �����մϴ�.
		ShaderBuilder::ToneMappingShader_get().SetAll( deviceContext );

		// HDR ���� �ؽ�ó �� ���� HDR ����� �����ϰ� �������� �����մϴ�.
		//geometryBuffer->SetGraphicsRootShaderResourcesForColor( deviceContext );
		hdrBuffer->SetGraphicsRootShaderResources( deviceContext );
		hdrComputedBuffer->SetGraphicsRootConstantBuffers( deviceContext );
		pCommandList->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
		pCommandList->DrawInstanced( 4, 1, 0, 0 );
	}

	// ��� ����� �ݰ� Ǫ���մϴ�.
	deviceContext->Close();
	directQueue->Execute( deviceContext );

	// ������ ��� ��ȣ�� �����մϴ�.
	lastPending[frameIndex++] = directQueue->Signal();

	// ������ �ε����� �����մϴ�.
	if ( frameIndex >= 2 ) frameIndex -= 2;
}

void GameLogic::ResizeBuffers( uint32 width, uint32 height )
{
	// �ʿ��� ���� ���ۿ��� ũ�� ���� ��û�� �����մϴ�.
	geometryBuffer->ResizeBuffers( width, height );
	hdrBuffer->ResizeBuffers( width, height );
	hdrComputedBuffer->ResizeBuffers( width, height );
}