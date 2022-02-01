// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Text/TextBlock.h"
#include "Draw/PaintArgs.h"
#include "Draw/IRenderSlateElement.h"
#include "Draw/SlateDrawCollector.h"
#include "RHI/IRHITextFormat.h"
#include "RHI/IRHITextLayout.h"
#include "RHI/IRHIFactory.h"
#include "RHI/IRHIDeviceContext2D.h"
#include "RHI/IRHISolidColorBrush.h"
#include "RHI/IRHIDevice.h"
#include "IApplicationInterface.h"
#include "RenderThread.h"

class STextBlock::SRenderElement : implements SObject, implements IRenderSlateElement
{
	GENERATED_BODY(SRenderElement)

public:
	const PaintArgs Args;

	int32 CachedLayer;
	Geometry CachedGeometry;
	Vector2 CachedLayoutSize;

	SPROPERTY(TintBrush)
	IRHISolidColorBrush* TintBrush = nullptr;
	SPROPERTY(Layout)
	IRHITextLayout* Layout = nullptr;

	Geometry RenderGeometry;
	int32 RenderLayer;

public:
	SRenderElement(STextBlock* Source, const PaintArgs& InPaintArgs, int32 InLayer, const Geometry& AllottedGeometry)
		: Super()
		, Args(InPaintArgs)
		, CachedLayer(InLayer)
		, CachedGeometry(AllottedGeometry)
		, CachedLayoutSize(Source->Layout->GetMaxSize())

		, TintBrush(Source->TintBrush)
		, Layout(Source->Layout)

		, RenderGeometry(AllottedGeometry)
		, RenderLayer(InLayer)
	{
	}

	virtual int32 GetLayer() override
	{
		return RenderLayer;
	}

	virtual Geometry GetCachedGeometry() override
	{
		return CachedGeometry;
	}

	virtual void RenderElement(IRHIDeviceContext2D* CommandBuffer, const LocalRenderLayout& LocalLayout) override
	{
		CommandBuffer->DrawTextLayout(LocalLayout.LocalPosition, Layout, TintBrush);
	}

	void SetGeometry_GameThread(int32 Layer, Geometry AllottedGeometry)
	{
		if (CachedGeometry != AllottedGeometry || CachedLayer != Layer)
		{
			CachedGeometry = AllottedGeometry;
			CachedLayer = Layer;

			RenderThread::Get()->EnqueueRenderThreadWork([=](auto)
			{
				RenderGeometry = AllottedGeometry;
				RenderLayer = Layer;
			});
		}
	}

	void SetMaxLayoutSize_GameThread(Vector2 LayoutSize)
	{
		if (CachedLayoutSize != LayoutSize)
		{
			CachedLayoutSize = LayoutSize;

			RenderThread::Get()->EnqueueRenderThreadWork([=](auto)
			{
				Layout->SetMaxSize(LayoutSize);
			});
		}
	}
};

GENERATE_BODY(STextBlock);
GENERATE_BODY(STextBlock::SRenderElement);

STextBlock::STextBlock() : Super()
{
}

void STextBlock::SetText(std::wstring_view Text)
{
	if (this->Text != Text)
	{
		this->Text = Text;
		InvalidateLayoutAndVolatility();
	}
}

std::wstring STextBlock::GetText()
{
	return Text;
}

void STextBlock::SetFont(const SlateFont& Font)
{
	if (this->Font != Font)
	{
		this->Font = Font;
		InvalidateLayoutAndVolatility();
	}
}

SlateFont STextBlock::GetFont()
{
	return Font;
}

void STextBlock::SetTintColor(const Color& TintColor)
{
	this->TintColor = TintColor;
}

Color STextBlock::GetTintColor()
{
	return TintColor;
}

ERHITextAlignment STextBlock::GetTextAlignment()
{
	return TextAlignment;
}

ERHIParagraphAlignment STextBlock::GetParagraphAlignment()
{
	return ParagraphAlignment;
}

bool STextBlock::PrepassLayout()
{
	bool bShouldBePrepass = false;
	if (ShouldBePrepassLayout())
	{
		ReallocLayout();
		bShouldBePrepass = true;
	}

	return Super::PrepassLayout() || bShouldBePrepass;
}

Vector2 STextBlock::ComputeDesiredSize()
{
	return CachedDesiredSize;
}

int32 STextBlock::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled)
{
	if (TintBrush)
	{
		TintBrush->SetColor(TintColor);
	}
	else
	{
		TintBrush = Args.Device->CreateSolidColorBrush(TintColor);
	}

	if (CachedRenderElement == nullptr && Layout)
	{
		CachedRenderElement = gcnew SRenderElement(this,
			Args.WithNewParent(this),
			InLayer,
			AllottedGeometry
		);
	}

	if (CachedRenderElement)
	{
		Vector2 LocalSize = AllottedGeometry.GetLocalSize();
		CachedRenderElement->SetMaxLayoutSize_GameThread(LocalSize);
		CachedRenderElement->SetGeometry_GameThread(InLayer, AllottedGeometry);
	}

	if (CachedRenderElement)
	{
		DrawCollector->AddRenderElement(CachedRenderElement);
	}

	return InLayer;
}

void STextBlock::ReallocLayout()
{
	Layout = nullptr;
	Format = nullptr;
	CachedRenderElement = nullptr;

	if (!Text.empty() && !Font.FamilyName.empty() && Font.Size > 0.1f)
	{
		IRHIFactory* Factory = IApplicationInterface::Get().GetFactory();
		Format = Factory->CreateTextFormat(Font.FamilyName, Font.Collection, ERHIFontWeight::Normal, ERHIFontStyle::Normal, ERHIFontStretch::Normal, Font.Size, L"ko-KR");

		Vector2 LocalSize = Vector2(1048576.0f, 1048576.0f);
		Layout = Factory->CreateTextLayout(Format, Text, LocalSize);
		Layout->SetTextAlignment(TextAlignment);
		Layout->SetParagraphAlignment(ParagraphAlignment);

		CachedLocalMaxSize = LocalSize;
		CachedDesiredSize = Layout->GetDesiredSize();
	}
}

void STextBlock::SetTextAlignment_GameThread(ERHITextAlignment Alignment)
{
	if (TextAlignment != Alignment)
	{
		TextAlignment = Alignment;

		if (Layout)
		{
			RenderThread::Get()->EnqueueRenderThreadWork([=](auto)
			{
				Layout->SetTextAlignment(Alignment);
			});
		}
	}
}

void STextBlock::SetParagraphAlignment_GameThread(ERHIParagraphAlignment Alignment)
{
	if (ParagraphAlignment != Alignment)
	{
		ParagraphAlignment = Alignment;

		if (Layout)
		{
			RenderThread::Get()->EnqueueRenderThreadWork([=](auto)
			{
				Layout->SetParagraphAlignment(Alignment);
			});
		}
	}
}

DEFINE_SLATE_CONSTRUCTOR(STextBlock, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
	SetText(Attr._Text);
	SetFont(Attr._Font);
	SetTintColor(Attr._TintColor);
	SetTextAlignment(Attr._TextAlignment);
	SetParagraphAlignment(Attr._ParagraphAlignment);
	ReallocLayout();
}