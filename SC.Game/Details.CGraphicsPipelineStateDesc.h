#pragma once

namespace SC::Game::Details
{
	struct CGraphicsPipelineStateDesc : public ValueType
	{
	private:
		ComPtr<ID3D12RootSignature> rootSignature;
		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{ };
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs;

	public:
		CGraphicsPipelineStateDesc( ComPtr<ID3D12RootSignature> rootSignatureInit = nullptr )
		{
			RootSignature = rootSignatureInit;

			desc.SampleMask = 0xFFFFFFFF;

			desc.BlendState =
			{
				FALSE,
				TRUE,
				{
					{ FALSE, FALSE, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_LOGIC_OP_CLEAR, D3D12_COLOR_WRITE_ENABLE_ALL },
					{ FALSE, FALSE, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_LOGIC_OP_CLEAR, D3D12_COLOR_WRITE_ENABLE_ALL },
					{ FALSE, FALSE, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_LOGIC_OP_CLEAR, D3D12_COLOR_WRITE_ENABLE_ALL },
					{ FALSE, FALSE, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_LOGIC_OP_CLEAR, D3D12_COLOR_WRITE_ENABLE_ALL },
					{ FALSE, FALSE, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_LOGIC_OP_CLEAR, D3D12_COLOR_WRITE_ENABLE_ALL },
					{ FALSE, FALSE, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_LOGIC_OP_CLEAR, D3D12_COLOR_WRITE_ENABLE_ALL },
					{ FALSE, FALSE, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_LOGIC_OP_CLEAR, D3D12_COLOR_WRITE_ENABLE_ALL },
					{ FALSE, FALSE, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_LOGIC_OP_CLEAR, D3D12_COLOR_WRITE_ENABLE_ALL }
				}

			};

			desc.RasterizerState =
			{
				D3D12_FILL_MODE_SOLID,
				D3D12_CULL_MODE_NONE,
			};

			desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			desc.SampleDesc = { 1, 0 };
		}

		D3D12_GRAPHICS_PIPELINE_STATE_DESC* Get()
		{
			return &desc;
		}

		void SetVertexShader( const void* shaderBytecode, size_t length )
		{
			desc.VS = { shaderBytecode, length };
		}

		void SetPixelShader( const void* shaderBytecode, size_t length )
		{
			desc.PS = { shaderBytecode, length };
		}

		void SetDomainShader( const void* shaderBytecode, size_t length )
		{
			desc.DS = { shaderBytecode, length };
		}

		void SetHullShader( const void* shaderBytecode, size_t length )
		{
			desc.HS = { shaderBytecode, length };
		}

		void SetGeometryShader( const void* shaderBytecode, size_t length )
		{
			desc.GS = { shaderBytecode, length };
		}

		void AddInputLayout( const D3D12_INPUT_ELEMENT_DESC& layout )
		{
			inputElementDescs.push_back( layout );
			desc.InputLayout.pInputElementDescs = inputElementDescs.data();
			desc.InputLayout.NumElements = ( UINT )inputElementDescs.size();
		}

		void AddInputLayout( LPCSTR semanticName, UINT semanticIndex, DXGI_FORMAT format, UINT inputSlot, UINT alignedByteOffset, D3D12_INPUT_CLASSIFICATION inputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, UINT instanceDataStepRate = 0 )
		{
			AddInputLayout( { semanticName, semanticIndex, format, inputSlot, alignedByteOffset, inputSlotClass, instanceDataStepRate } );
		}

		template< std::size_t Count >
		void AddInputLayout( const D3D12_INPUT_ELEMENT_DESC( &inputElements )[Count] )
		{
			for ( int i = 0; i < ( int )Count; ++i )
			{
				AddInputLayout( inputElements[i] );
			}
		}

		void RemoveInputLayout( const char* semanticName )
		{
			for ( int i = 0; i < NumInputLayout; ++i )
			{
				if ( std::strcmp( inputElementDescs[i].SemanticName, semanticName ) == 0 )
				{
					inputElementDescs.erase( inputElementDescs.begin() + i );
				}
			}
			desc.InputLayout.pInputElementDescs = inputElementDescs.data();
			desc.InputLayout.NumElements = ( UINT )inputElementDescs.size();
		}

		void SetDepthStencilState( bool enable, D3D12_COMPARISON_FUNC depthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL )
		{
			desc.DepthStencilState =
			{
				( BOOL )enable,
				D3D12_DEPTH_WRITE_MASK_ALL,
				depthFunc,
				FALSE,
				0,
				0,
				{ },
				{ }
			};
		}

		void SetBlendState( int renderTargetIndex, bool enable, D3D12_BLEND srcBlend, D3D12_BLEND destBlend, D3D12_BLEND_OP blendOp = D3D12_BLEND_OP_ADD, D3D12_BLEND srcAlpha = D3D12_BLEND_ONE, D3D12_BLEND destAlpha = D3D12_BLEND_ZERO, D3D12_BLEND_OP alphaOp = D3D12_BLEND_OP_ADD )
		{
			D3D12_RENDER_TARGET_BLEND_DESC blendState
			{
				enable,
				FALSE,
				srcBlend,
				destBlend,
				blendOp,
				srcAlpha,
				destAlpha,
				alphaOp,
				D3D12_LOGIC_OP_CLEAR,
				D3D12_COLOR_WRITE_ENABLE_ALL
			};

			desc.BlendState.RenderTarget[renderTargetIndex] = blendState;
		}

		vs_property( ComPtr<ID3D12RootSignature>, RootSignature );
		vs_property( int, RTVCount );
		vs_property( DXGI_FORMAT, RTVFormats )[];
		vs_property_get( int, NumInputLayout );
		vs_property( D3D12_CULL_MODE, CullMode );
		vs_property( DXGI_FORMAT, DSVFormat );
		vs_property( double, SlopeScaledDepthBias );

		ComPtr<ID3D12RootSignature> RootSignature_get()
		{
			return rootSignature;
		}

		void RootSignature_set( ComPtr<ID3D12RootSignature> value )
		{
			rootSignature = value;
			desc.pRootSignature = rootSignature.Get();
		}

		int RTVCount_get()
		{
			return desc.NumRenderTargets;
		}

		void RTVCount_set( int value )
		{
#if defined( _DEBUG )
			if ( value < 0 && value >= 8 ) throw ArgumentOutOfRangeException();
#endif
			desc.NumRenderTargets = value;
		}

		DXGI_FORMAT RTVFormats_get( int index )
		{
#if defined( _DEBUG )
			if ( index < 0 && index >= 8 ) throw ArgumentOutOfRangeException();
#endif
			return desc.RTVFormats[index];
		}

		void RTVFormats_set( int index, DXGI_FORMAT value )
		{
#if defined( _DEBUG )
			if ( index < 0 && index >= 8 ) throw ArgumentOutOfRangeException();
#endif
			desc.RTVFormats[index] = value;
		}

		int NumInputLayout_get()
		{
			return ( int )inputElementDescs.size();
		}

		D3D12_CULL_MODE CullMode_get()
		{
			return desc.RasterizerState.CullMode;
		}

		void CullMode_set( D3D12_CULL_MODE value )
		{
			desc.RasterizerState.CullMode = value;
		}

		DXGI_FORMAT DSVFormat_get()
		{
			return desc.DSVFormat;
		}

		void DSVFormat_set( DXGI_FORMAT value )
		{
			desc.DSVFormat = value;
		}

		double SlopeScaledDepthBias_get()
		{
			return ( double )desc.RasterizerState.SlopeScaledDepthBias;
		}

		void SlopeScaledDepthBias_set( double value )
		{
			desc.RasterizerState.SlopeScaledDepthBias = ( float )value;
		}
	};
}