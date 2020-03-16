#ifndef IBASE_H
#define IBASE_H
#include "Pch.h"

/********************************************************************************
*	类型：Image
*	作用：提供基本的图片处理操作
********************************************************************************/
using ImageFactory = ComPtr<IWICImagingFactory>;
using Bitmap = ComPtr<IWICBitmap>;
using BitmapDecoder = ComPtr<IWICBitmapDecoder>;
using BitmapEncoder = ComPtr<IWICBitmapEncoder>;
using BitmapFrameDecode = ComPtr<IWICBitmapFrameDecode>;
using BitmapFrameEncode = ComPtr<IWICBitmapFrameEncode>;
using FormatConverter = ComPtr<IWICFormatConverter>;

using Stream = ComPtr<IWICStream>;

enum class BitmapFormat
{
	Bmp, Jpeg, Png, Tiff, Ico, Gif
};

class Image
{
	static ImageFactory factory;
	Bitmap bmp;
public:
	Image()
	{
		if (factory == nullptr)
		{
			CoInitialize(nullptr);
			CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, __uuidof(factory), &factory);
		}
	}

	//加载图片
	Bitmap& Load(String filename)
	{
		BitmapDecoder decoder;
		factory->CreateDecoderFromFilename(filename.c_str(), nullptr, GENERIC_READ, WICDecodeOptions::WICDecodeMetadataCacheOnDemand, &decoder);

		BitmapFrameDecode frame;
		decoder->GetFrame(0, &frame);

		FormatConverter convert;
		factory->CreateFormatConverter(&convert);
		convert->Initialize(frame.Get(), GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, nullptr, 1.0, WICBitmapPaletteTypeCustom);

		factory->CreateBitmapFromSource(convert.Get(), WICBitmapCreateCacheOption::WICBitmapCacheOnDemand, bmp.ReleaseAndGetAddressOf());
		return bmp;
	}

	Bitmap& Load(HBITMAP hBitmap)
	{
		factory->CreateBitmapFromHBITMAP(hBitmap, nullptr, WICBitmapAlphaChannelOption::WICBitmapUsePremultipliedAlpha, bmp.ReleaseAndGetAddressOf());
		return bmp;
	}

	Bitmap& Load(HICON hIcon)
	{
		factory->CreateBitmapFromHICON(hIcon, bmp.ReleaseAndGetAddressOf());
		return bmp;
	}

	//保存图片
	HRESULT Save(LPCTSTR filename, BitmapFormat fmt = BitmapFormat::Bmp)
	{
		HRESULT hr = S_OK;
		UINT width, height;
		hr = bmp->GetSize(&width, &height);

		Stream stream;
		hr = factory->CreateStream(&stream);
		hr = stream->InitializeFromFilename(filename, GENERIC_WRITE);

		BitmapEncoder encoder;
		hr = factory->CreateEncoder(FormatMapping(fmt), nullptr, &encoder);
		hr = encoder->Initialize(stream.Get(), WICBitmapEncoderNoCache);

		BitmapFrameEncode frame;
		hr = encoder->CreateNewFrame(&frame, nullptr);
		hr = frame->Initialize(nullptr);
		hr = frame->SetPixelFormat(&FormatMapping(fmt));

		hr = frame->SetSize(width, height);
		hr = frame->WriteSource(bmp.Get(), nullptr);
		hr = frame->Commit();
		hr = encoder->Commit();
		return hr;
	}
	//图片旋转操作


	GUID FormatMapping(BitmapFormat fmt)
	{
		switch (fmt)
		{
		case BitmapFormat::Gif:return GUID_ContainerFormatGif;
		case BitmapFormat::Ico:return GUID_ContainerFormatIco;
		case BitmapFormat::Png:return GUID_ContainerFormatPng;
		case BitmapFormat::Jpeg:return GUID_ContainerFormatJpeg;
		case BitmapFormat::Tiff:return GUID_ContainerFormatTiff;
		default:return GUID_ContainerFormatBmp;
		}
	}

};

/********************************************************************************
*	名称：Text
*	作用：基本的文本处理操作
********************************************************************************/
using DWFactory = ComPtr<IDWriteFactory>;
using TextLayout = ComPtr<IDWriteTextLayout>;
using TextFormat = ComPtr<IDWriteTextFormat>;

struct Text
{
private:
	static DWFactory factory;
public:
	Text()
	{
		if (factory == nullptr)
		{
			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &factory);
		}
	}

	TextLayout LayoutString(String str, float fontSize = 32, float width = 200, float height = 100, String fontName = L"微软雅黑")
	{
		TextLayout layout;

		TextFormat format;
		factory->CreateTextFormat(fontName.data(), nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, __TEXT("zh-CN"), &format);
		factory->CreateTextLayout(str.c_str(), str.length(), format.Get(), width, height, &layout);
		layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		
		return layout;
	}
};

#endif // !IBASE_H
