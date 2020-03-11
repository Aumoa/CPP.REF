#pragma once

namespace SC::Game::Details
{
	struct CRootSignatureDesc : public ValueType
	{
	private:
		D3D12_ROOT_SIGNATURE_DESC desc{ };
		std::vector<D3D12_ROOT_PARAMETER> rootParameters;
		std::vector<D3D12_STATIC_SAMPLER_DESC> samplers;

	public:
		CRootSignatureDesc()
		{
			VertexShaderRootAccess = false;
			PixelShaderRootAccess = false;
			DomainShaderRootAccess = false;
			HullShaderRootAccess = false;
			GeometryShaderRootAccess = false;
		}

		D3D12_ROOT_SIGNATURE_DESC* Get()
		{
			return &desc;
		}

		void AddRoot32BitConstants( int shaderRegister, int num32Bits, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, int registerSpace = 0 )
		{
			D3D12_ROOT_PARAMETER param{ };
			param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
			param.Constants.ShaderRegister = shaderRegister;
			param.Constants.Num32BitValues = num32Bits;
			param.Constants.RegisterSpace = registerSpace;
			param.ShaderVisibility = shaderVisibility;
			AddRootParameter( param );
		}

		void AddConstantBufferView( int shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, int registerSpace = 0 )
		{
			D3D12_ROOT_PARAMETER param{ };
			param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			param.Descriptor.ShaderRegister = shaderRegister;
			param.Descriptor.RegisterSpace = registerSpace;
			param.ShaderVisibility = shaderVisibility;
			AddRootParameter( param );
		}

		void AddShaderResourceView( int shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, int registerSpace = 0 )
		{
			D3D12_ROOT_PARAMETER param{ };
			param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
			param.Descriptor.ShaderRegister = shaderRegister;
			param.Descriptor.RegisterSpace = registerSpace;
			param.ShaderVisibility = shaderVisibility;
			AddRootParameter( param );
		}

		template< size_t Count >
		void AddDescriptorTable( const D3D12_DESCRIPTOR_RANGE( &ranges )[Count], D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL )
		{
			D3D12_ROOT_PARAMETER param{ };
			param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			param.DescriptorTable.NumDescriptorRanges = ( UINT )Count;
			param.DescriptorTable.pDescriptorRanges = ranges;
			param.ShaderVisibility = shaderVisibility;
			AddRootParameter( param );
		}

		void AddStaticSampler( D3D12_FILTER filter, D3D12_TEXTURE_ADDRESS_MODE addressMode, uint32 maxAnisotropy = 0, int shaderRegister = 0 )
		{
			D3D12_STATIC_SAMPLER_DESC sampler{ };

			sampler.Filter = filter;
			sampler.AddressU = addressMode;
			sampler.AddressV = addressMode;
			sampler.AddressW = addressMode;
			sampler.MaxAnisotropy = maxAnisotropy;
			sampler.ShaderRegister = shaderRegister;

			samplers.push_back( sampler );
			desc.pStaticSamplers = samplers.data();
			desc.NumStaticSamplers = ( UINT )samplers.size();
		}

		void AddStaticComparisonSampler( D3D12_FILTER filter, D3D12_TEXTURE_ADDRESS_MODE addressMode, int shaderRegister = 0, D3D12_COMPARISON_FUNC comparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL )
		{
			D3D12_STATIC_SAMPLER_DESC sampler{ };

			sampler.Filter = filter;
			sampler.AddressU = addressMode;
			sampler.AddressV = addressMode;
			sampler.AddressW = addressMode;
			sampler.ComparisonFunc = comparisonFunc;
			sampler.ShaderRegister = shaderRegister;
			sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;

			samplers.push_back( sampler );
			desc.pStaticSamplers = samplers.data();
			desc.NumStaticSamplers = ( UINT )samplers.size();
		}

		vs_property( bool, InputAssemblerInputLayout );
		vs_property( bool, VertexShaderRootAccess );
		vs_property( bool, PixelShaderRootAccess );
		vs_property( bool, DomainShaderRootAccess );
		vs_property( bool, HullShaderRootAccess );
		vs_property( bool, GeometryShaderRootAccess );

		bool InputAssemblerInputLayout_get()
		{
			return ( desc.Flags & D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT ) != 0;
		}

		void InputAssemblerInputLayout_set( bool value )
		{
			if ( value )
			{
				desc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
			}
			else
			{
				desc.Flags &= ~D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
			}
		}

		bool VertexShaderRootAccess_get()
		{
			return ( desc.Flags & D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS ) == 0;
		}

		void VertexShaderRootAccess_set( bool value )
		{
			if ( value )
			{
				desc.Flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
			}
			else
			{
				desc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
			}
		}

		bool PixelShaderRootAccess_get()
		{
			return ( desc.Flags & D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS ) == 0;
		}

		void PixelShaderRootAccess_set( bool value )
		{
			if ( value )
			{
				desc.Flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
			}
			else
			{
				desc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
			}
		}

		bool DomainShaderRootAccess_get()
		{
			return ( desc.Flags & D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS ) == 0;
		}

		void DomainShaderRootAccess_set( bool value )
		{
			if ( value )
			{
				desc.Flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
			}
			else
			{
				desc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
			}
		}

		bool HullShaderRootAccess_get()
		{
			return ( desc.Flags & D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS ) == 0;
		}

		void HullShaderRootAccess_set( bool value )
		{
			if ( value )
			{
				desc.Flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
			}
			else
			{
				desc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
			}
		}

		bool GeometryShaderRootAccess_get()
		{
			return ( desc.Flags & D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS ) == 0;
		}

		void GeometryShaderRootAccess_set( bool value )
		{
			if ( value )
			{
				desc.Flags &= ~D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
			}
			else
			{
				desc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
			}
		}

	private:
		void AddRootParameter( const D3D12_ROOT_PARAMETER& param )
		{
			rootParameters.push_back( param );
			desc.NumParameters = ( UINT )rootParameters.size();
			desc.pParameters = rootParameters.data();
		}
	};
}