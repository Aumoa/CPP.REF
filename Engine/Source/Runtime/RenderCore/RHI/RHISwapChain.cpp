// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.RenderCore.Internal;

int main1()
{
	ID3D12Debug* interface;
	D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&interface);
	return 0;
}