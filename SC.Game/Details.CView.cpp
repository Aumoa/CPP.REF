using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

CView::CView( RefPtr<ViewStorage>& viewStorage, int lockIndex, D3D12_CPU_DESCRIPTOR_HANDLE handle )
	: viewStorage( viewStorage )
	, lockIndex( lockIndex )
	, Handle( handle )
{

}

CView::~CView()
{
	viewStorage->Free( lockIndex );
}