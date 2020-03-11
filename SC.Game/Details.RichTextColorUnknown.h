#pragma once

namespace SC::Game::Details
{
	class RichTextColorUnknown : virtual public Object, virtual public IUnknown
	{
		ULONG ref = 0;

	public:
		RichTextColorUnknown( Drawing::Color value )
		{
			solidColorBrush = new UI::SolidColorBrush( value );
		}

		~RichTextColorUnknown()
		{
			solidColorBrush = nullptr;
		}

		ULONG STDMETHODCALLTYPE AddRef() override
		{
			return ++ref;
		}

		ULONG STDMETHODCALLTYPE Release() override
		{
			if ( --ref == 0 )
			{
				delete this;
				return 0;
			}
			else
			{
				return ref;
			}
		}

		HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, void** ppvObject ) override
		{
			if ( riid == __uuidof( IUnknown ) )
			{
				*ppvObject = dynamic_cast< IUnknown* >( this );
				return S_OK;
			}
			else
			{
				return E_NOINTERFACE;
			}
		}

		RefPtr<UI::SolidColorBrush> solidColorBrush;
	};
}