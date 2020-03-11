using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

CShaderResourceView::CShaderResourceView( RefPtr<ViewStorage>& viewStorage, int lockIndex, D3D12_CPU_DESCRIPTOR_HANDLE handle ) : CView( viewStorage, lockIndex, handle )
{

}