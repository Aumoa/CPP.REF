using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

void Mesh::DrawIndexed( RefPtr<CDeviceContext>& deviceContext )
{
	vertexBuffer->Lock( deviceContext );
	indexBuffer->Lock( deviceContext );

	auto pCommandList = deviceContext->pCommandList.Get();

	if ( auto slot = deviceContext->Slot["IsSkinned"]; slot != -1 )
	{
		pCommandList->SetGraphicsRoot32BitConstant( ( UINT )slot, isSkinned, 0 );
	}

	D3D12_VERTEX_BUFFER_VIEW vbv{ };
	vbv.BufferLocation = vertexBuffer->pResource->GetGPUVirtualAddress();
	vbv.SizeInBytes = ( isSkinned ? sizeof( SkinnedVertex ) : sizeof( Vertex ) ) * numVertex;
	vbv.StrideInBytes = ( isSkinned ? sizeof( SkinnedVertex ) : sizeof( Vertex ) );

	vertexBuffer->Lock( deviceContext );
	pCommandList->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	pCommandList->IASetVertexBuffers( 0, 1, &vbv );

	if ( indexBuffer )
	{
		D3D12_INDEX_BUFFER_VIEW ibv{ };
		ibv.BufferLocation = indexBuffer->pResource->GetGPUVirtualAddress();
		ibv.SizeInBytes = sizeof( uint32 ) * numIndex;
		ibv.Format = DXGI_FORMAT_R32_UINT;

		indexBuffer->Lock( deviceContext );
		pCommandList->IASetIndexBuffer( &ibv );
		pCommandList->DrawIndexedInstanced( ( UINT )numIndex, 1, 0, 0, 0 );
	}
	else
	{
		pCommandList->DrawInstanced( ( UINT )numVertex, 1, 0, 0 );
	}
}

Mesh::Mesh( String name, const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer ) : Assets( name )
{
	this->vertexBuffer = new CBuffer(
		GlobalVar.device,
		sizeof( Vertex ) * vertexBuffer.size(),
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		D3D12_RESOURCE_FLAG_NONE,
		vertexBuffer.data()
	);

	this->indexBuffer = new CBuffer(
		GlobalVar.device,
		sizeof( uint32 ) * indexBuffer.size(),
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		D3D12_RESOURCE_FLAG_NONE,
		indexBuffer.data()
	);

	this->numVertex = ( int )vertexBuffer.size();
	this->numIndex = ( int )indexBuffer.size();
}

Mesh::Mesh( String name, const vector<SkinnedVertex>& vertexBuffer, const vector<uint32>& indexBuffer ) : Assets( name )
{
	this->vertexBuffer = new CBuffer(
		GlobalVar.device,
		sizeof( SkinnedVertex ) * vertexBuffer.size(),
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		D3D12_RESOURCE_FLAG_NONE,
		vertexBuffer.data()
	);

	this->indexBuffer = new CBuffer(
		GlobalVar.device,
		sizeof( uint32 ) * indexBuffer.size(),
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		D3D12_RESOURCE_FLAG_NONE,
		indexBuffer.data()
	);

	this->numVertex = ( int )vertexBuffer.size();
	this->numIndex = ( int )indexBuffer.size();

	isSkinned = true;
}

bool Mesh::IsSkinned_get()
{
	return isSkinned;
}

RefPtr<Mesh> Mesh::CreatePlane( String name, float texScaleX, float texScaleY, int gridCountX, int gridCountY )
{
	int vertexCountX = gridCountX + 1;
	int vertexCountY = gridCountY + 1;

	vector<Vertex> vertexBuffer( vertexCountX * vertexCountY );
	vector<uint32> indexBuffer( gridCountX * gridCountY * 6 );

	float xPosBeg = -1.0f;
	float xPosEnd = 1.0f;
	float xPosStride = 2.0f / gridCountX;
	float yPosStride = 2.0f / gridCountY;
	float xTexStride = texScaleX / gridCountX;
	float yTexStride = texScaleY / gridCountY;

	for ( int j = 0; j < vertexCountY; ++j )
	{
		for ( int i = 0; i < vertexCountX; ++i )
		{
			Vertex v;

			v.Pos = { xPosBeg + xPosStride * i, 0.0f, xPosEnd - xPosStride * j };
			v.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			v.Tex = { xTexStride * i, yTexStride * j };
			v.Normal = { 0.0f, 1.0f, 0.0f };
			v.Tangent = { 1.0f, 0.0f, 0.0f };

			vertexBuffer[j * vertexCountX + i] = v;
		}
	}

	for ( int j = 0; j < gridCountY; ++j )
	{
		for ( int i = 0; i < gridCountX; ++i )
		{
			int base = j * gridCountX + i;
			auto arr = ( int( & )[6] )indexBuffer[( j * gridCountX + i ) * 6];
			arr[0] = base;
			arr[1] = base + 1;
			arr[2] = base + vertexCountX;
			arr[3] = base + 1;
			arr[4] = base + vertexCountX + 1;
			arr[5] = base + vertexCountX;
		}
	}

	return new Mesh( name, vertexBuffer, indexBuffer );
}

RefPtr<Mesh> Mesh::CreateCube( String name )
{
	vector<Vertex> vertexBuffer( 24 );
	vector<uint32> indexBuffer( 36 );

	/* Front */
	vertexBuffer[0].Pos = { -1.0f, +1.0f, -1.0f };
	vertexBuffer[1].Pos = { +1.0f, +1.0f, -1.0f };
	vertexBuffer[2].Pos = { +1.0f, -1.0f, -1.0f };
	vertexBuffer[3].Pos = { -1.0f, -1.0f, -1.0f };

	vertexBuffer[0].Normal = { +0.0f, +0.0f, -1.0f };
	vertexBuffer[1].Normal = { +0.0f, +0.0f, -1.0f };
	vertexBuffer[2].Normal = { +0.0f, +0.0f, -1.0f };
	vertexBuffer[3].Normal = { +0.0f, +0.0f, -1.0f };

	vertexBuffer[0].Tangent = { +1.0f, +0.0f, +0.0f };
	vertexBuffer[1].Tangent = { +1.0f, +0.0f, +0.0f };
	vertexBuffer[2].Tangent = { +1.0f, +0.0f, +0.0f };
	vertexBuffer[3].Tangent = { +1.0f, +0.0f, +0.0f };

	/* Back */
	vertexBuffer[4].Pos = { +1.0f, +1.0f, +1.0f };
	vertexBuffer[5].Pos = { -1.0f, +1.0f, +1.0f };
	vertexBuffer[6].Pos = { -1.0f, -1.0f, +1.0f };
	vertexBuffer[7].Pos = { +1.0f, -1.0f, +1.0f };

	vertexBuffer[4].Normal = { +0.0f, +0.0f, +1.0f };
	vertexBuffer[5].Normal = { +0.0f, +0.0f, +1.0f };
	vertexBuffer[6].Normal = { +0.0f, +0.0f, +1.0f };
	vertexBuffer[7].Normal = { +0.0f, +0.0f, +1.0f };

	vertexBuffer[4].Tangent = { -1.0f, +0.0f, +0.0f };
	vertexBuffer[5].Tangent = { -1.0f, +0.0f, +0.0f };
	vertexBuffer[6].Tangent = { -1.0f, +0.0f, +0.0f };
	vertexBuffer[7].Tangent = { -1.0f, +0.0f, +0.0f };

	/* Right */
	vertexBuffer[ 8].Pos = { +1.0f, +1.0f, -1.0f };
	vertexBuffer[ 9].Pos = { +1.0f, +1.0f, +1.0f };
	vertexBuffer[10].Pos = { +1.0f, -1.0f, +1.0f };
	vertexBuffer[11].Pos = { +1.0f, -1.0f, -1.0f };

	vertexBuffer[ 8].Normal = { +1.0f, +0.0f, +0.0f };
	vertexBuffer[ 9].Normal = { +1.0f, +0.0f, +0.0f };
	vertexBuffer[10].Normal = { +1.0f, +0.0f, +0.0f };
	vertexBuffer[11].Normal = { +1.0f, +0.0f, +0.0f };

	vertexBuffer[ 8].Tangent = { -0.0f, +0.0f, +1.0f };
	vertexBuffer[ 9].Tangent = { -0.0f, +0.0f, +1.0f };
	vertexBuffer[10].Tangent = { -0.0f, +0.0f, +1.0f };
	vertexBuffer[11].Tangent = { -0.0f, +0.0f, +1.0f };

	/* Left */
	vertexBuffer[12].Pos = { -1.0f, +1.0f, +1.0f };
	vertexBuffer[13].Pos = { -1.0f, +1.0f, -1.0f };
	vertexBuffer[14].Pos = { -1.0f, -1.0f, -1.0f };
	vertexBuffer[15].Pos = { -1.0f, -1.0f, +1.0f };

	vertexBuffer[12].Normal = { -1.0f, +0.0f, +0.0f };
	vertexBuffer[13].Normal = { -1.0f, +0.0f, +0.0f };
	vertexBuffer[14].Normal = { -1.0f, +0.0f, +0.0f };
	vertexBuffer[15].Normal = { -1.0f, +0.0f, +0.0f };

	vertexBuffer[12].Tangent = { -0.0f, +0.0f, -1.0f };
	vertexBuffer[13].Tangent = { -0.0f, +0.0f, -1.0f };
	vertexBuffer[14].Tangent = { -0.0f, +0.0f, -1.0f };
	vertexBuffer[15].Tangent = { -0.0f, +0.0f, -1.0f };

	/* Top */
	vertexBuffer[16].Pos = { -1.0f, +1.0f, +1.0f };
	vertexBuffer[17].Pos = { +1.0f, +1.0f, +1.0f };
	vertexBuffer[18].Pos = { +1.0f, +1.0f, -1.0f };
	vertexBuffer[19].Pos = { -1.0f, +1.0f, -1.0f };

	vertexBuffer[16].Normal = { -0.0f, +1.0f, +0.0f };
	vertexBuffer[17].Normal = { -0.0f, +1.0f, +0.0f };
	vertexBuffer[18].Normal = { -0.0f, +1.0f, +0.0f };
	vertexBuffer[19].Normal = { -0.0f, +1.0f, +0.0f };

	vertexBuffer[16].Tangent = { +1.0f, +0.0f, -1.0f };
	vertexBuffer[17].Tangent = { +1.0f, +0.0f, -1.0f };
	vertexBuffer[18].Tangent = { +1.0f, +0.0f, -1.0f };
	vertexBuffer[19].Tangent = { +1.0f, +0.0f, -1.0f };

	/* Bottom */
	vertexBuffer[20].Pos = { +1.0f, -1.0f, +1.0f };
	vertexBuffer[21].Pos = { -1.0f, -1.0f, +1.0f };
	vertexBuffer[22].Pos = { -1.0f, -1.0f, -1.0f };
	vertexBuffer[23].Pos = { +1.0f, -1.0f, -1.0f };

	vertexBuffer[20].Normal = { -0.0f, -1.0f, +0.0f };
	vertexBuffer[21].Normal = { -0.0f, -1.0f, +0.0f };
	vertexBuffer[22].Normal = { -0.0f, -1.0f, +0.0f };
	vertexBuffer[23].Normal = { -0.0f, -1.0f, +0.0f };

	vertexBuffer[20].Tangent = { -1.0f, +0.0f, -1.0f };
	vertexBuffer[21].Tangent = { -1.0f, +0.0f, -1.0f };
	vertexBuffer[22].Tangent = { -1.0f, +0.0f, -1.0f };
	vertexBuffer[23].Tangent = { -1.0f, +0.0f, -1.0f };

	for ( int i = 0; i < 24; ++i )
	{
		vertexBuffer[i].Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	constexpr static const float stride = 1.0f / 6.0f;
	for ( int i = 0; i < 6; ++i )
	{
		Vertex( &array )[4] = ( Vertex( & )[4] )vertexBuffer[i * 4];

		float mul1 = ( float )i;
		float mul2 = ( float )( i + 1 );
		array[0].Tex = { stride * mul1, 0.0f };
		array[1].Tex = { stride * mul2, 0.0f };
		array[2].Tex = { stride * mul2, 1.0f };
		array[3].Tex = { stride * mul1, 1.0f };
	}

	indexBuffer =
	{
		0, 1, 3,
		1, 2, 3,
		
		4, 5, 7,
		5, 6, 7,

		8, 9, 11,
		9, 10, 11,

		12, 13, 15,
		13, 14, 15,

		16, 17, 19,
		17, 18, 19,

		20, 21, 23,
		21, 22, 23
	};

	return new Mesh( name, vertexBuffer, indexBuffer );
}