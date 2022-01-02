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

	const int32 Layer = 0;
	const Geometry CachedGeometry;

	SPROPERTY(TintBrush)
	IRHISolidColorBrush* TintBrush = nullptr;
	SPROPERTY(Layout)
	IRHITextLayout* Layout = nullptr;

public:
	SRenderElement(STextBlock* Source, const PaintArgs& InPaintArgs, int32 InLayer, const Geometry& AllottedGeometry)
		: Super()
		, Args(InPaintArgs)
		, Layer(InLayer)
		, CachedGeometry(AllottedGeometry)

		, TintBrush(Source->TintBrush)
		, Layout(Source->Layout)
	{
	}

	virtual int32 GetLayer() override
	{
		return Layer;
	}

	virtual Geometry GetCachedGeometry() override
	{
		return CachedGeometry;
	}

	virtual void RenderElement(IRHIDeviceContext2D* CommandBuffer, const LocalRenderLayout& LocalLayout) override
	{
		CommandBuffer->DrawTextLayout(LocalLayout.LocalPosition, Layout, TintBrush);
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
		bNeedToReallocateLayout = true;
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
		bNeedToReallocateLayout = true;
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

void STextBlock::SetTextAlignment(ERHITextAlignment Alignment)
{
	if (TextAlignment != Alignment)
	{
		TextAlignment = Alignment;

		if (Layout)
		{
			RenderThread::EnqueueRenderThreadWork<"STextBlock.SetTextAlignment">([&](auto)
			{
				Layout->SetTextAlignment(Alignment);
			});
		}
	}
}

ERHITextAlignment STextBlock::GetTextAlignment()
{
	return TextAlignment;
}

void STextBlock::SetParagraphAlignment(ERHIParagraphAlignment Alignment)
{
	if (ParagraphAlignment != Alignment)
	{
		ParagraphAlignment = Alignment;

		if (Layout)
		{
			RenderThread::EnqueueRenderThreadWork<"STextBlock.SetParagraphAlignment">([&](auto)
			{
				Layout->SetParagraphAlignment(Alignment);
			});
		}
	}
}

ERHIParagraphAlignment STextBlock::GetParagraphAlignment()
{
	return ParagraphAlignment;
}

void STextBlock::Tick(const Geometry& AllottedGeometry, float InDeltaTime)
{
	if (bNeedToReallocateLayout)
	{
		ReallocLayout();
	}

	Super::Tick(AllottedGeometry, InDeltaTime);
}

Vector2 STextBlock::GetDesiredSize()
{
	return CachedDesiredSize;
}

int32 STextBlock::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled)
{
	if (bNeedToReallocateLayout)
	{
		ReallocLayout();
	}

	if (TintBrush)
	{
		TintBrush->SetColor(TintColor);
	}
	else
	{
		TintBrush = Args.Device->CreateSolidColorBrush(TintColor);
	}

	if (Layout)
	{
		Vector2 LocalSize = AllottedGeometry.GetLocalSize();
		if (!CachedLocalMaxSize.NearlyEquals(LocalSize))
		{
			Layout->SetMaxSize(LocalSize);
			CachedLocalMaxSize = LocalSize;
		}
	}

	DrawCollector->AddRenderElement(gcnew SRenderElement(this,
		Args.WithNewParent(this),
		InLayer,
		AllottedGeometry
	));
	return InLayer;
}

void STextBlock::ReallocLayout()
{
	Layout = nullptr;
	Format = nullptr;

	if (!Text.empty() && !Font.FamilyName.empty() && Font.Size > 0.1f)
	{
		IRHIFactory* Factory = IApplicationInterface::Get().GetFactory();
		Format = Factory->CreateTextFormat(Font.FamilyName, Font.Collection, ERHIFontWeight::Normal, ERHIFontStyle::Normal, ERHIFontStretch::Normal, Font.Size, L"ko-KR");

		Vector2 LocalSize = Vector2(1048576.0f, 1048576.0f);
		Layout = Factory->CreateTextLayout(Format, Text, LocalSize);
		CachedLocalMaxSize = LocalSize;
		CachedDesiredSize = Layout->GetDesiredSize();

		bNeedToReallocateLayout = false;

		Layout->SetTextAlignment(TextAlignment);
		Layout->SetParagraphAlignment(ParagraphAlignment);
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