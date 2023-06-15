#include "CircleProgress.h"
#include "duilib/RenderGdiPlus/GdiPlusDefs.h"
#include <shlwapi.h>

namespace ui
{

CircleProgress::CircleProgress() :
	m_bCircular(true),
	m_bClockwise(true),
	m_nCircleWidth(1),
	m_dwBackgroundColor(0),
	m_dwForegroundColor(0),
	m_dwGradientColor(0),
	m_pIndicator(nullptr)
{

}

std::wstring CircleProgress::GetType() const { return DUI_CTR_CIRCLEPROGRESS; }

void CircleProgress::SetAttribute(const std::wstring& srName, const std::wstring& strValue)
{
	if (srName == L"circular") SetCircular(strValue == L"true");
	else if (srName == L"circlewidth") SetCircleWidth(_wtoi(strValue.c_str()));
	else if (srName == L"indicator") SetIndicator(strValue);
	else if (srName == L"clockwise") SetClockwiseRotation(strValue == L"true");
	else if (srName == L"bgcolor") {
		LPCTSTR pValue = strValue.c_str();
		while (*pValue > '\0' && *pValue <= ' ') pValue = ::CharNext(pValue);
		SetBackgroudColor(pValue);
	}
	else if (srName == L"fgcolor") {
		LPCTSTR pValue = strValue.c_str();
		while (*pValue > L'\0' && *pValue <= L' ') pValue = ::CharNext(pValue);
		SetForegroudColor(pValue);
	}
	else if (srName == L"gradientcolor") {
		LPCTSTR pValue = strValue.c_str();
		while (*pValue > L'\0' && *pValue <= L' ') pValue = ::CharNext(pValue);
		SetCircleGradientColor(pValue);
	}
	else Progress::SetAttribute(srName, strValue);
}

void CircleProgress::PaintStatusImage(IRender* pRender)
{
	ASSERT(pRender != nullptr);
	if (pRender == nullptr) {
		return;
	}
	Progress::PaintStatusImage(pRender);
	if (m_bCircular)
	{
		//ĿǰIRender���кܶ�GDI+�ӿ�δʵ�֣���ʱֱ����gdi+��ͼ��
		//�Ժ���ܻ��������ʵ��1��DrawArc  2��Pen����brush(����)��� 3��������������Graphics����
		int direction = m_bClockwise ? 1 : -1;   //��ת����
		int bordersize = 1;          //���ȿ��Ŀǰʹ��1����

		HDC hdc = pRender->GetDC();
		Gdiplus::Graphics graphics(hdc);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::Pen bgPen(m_dwBackgroundColor.GetARGB(), static_cast<Gdiplus::REAL>(m_nCircleWidth));
		// Բ������
		UiPoint center;
		const UiRect& rect = GetRect();
		center.x = rect.left + (rect.right - rect.left) / 2;
		center.y = rect.top + (rect.bottom - rect.top) / 2;

		// �ؼ������ڵ���������εı߽�
		int side = std::min(rect.right - rect.left, rect.bottom - rect.top);
		//UiRect rcBorder;  ��Ȼ����UiRect �� RectF��ת��������ֱ����gdi��RectF��
		Gdiplus::RectF rcBorder;
		rcBorder.X = static_cast<Gdiplus::REAL>(center.x - side / 2);
		rcBorder.Y = static_cast<Gdiplus::REAL>(center.y - side / 2);
		rcBorder.Width = rcBorder.Height = static_cast<Gdiplus::REAL>(side);

		Gdiplus::RectF outer = rcBorder;
		if (m_pIndicator) {
			outer.Inflate(-1.0F *m_pIndicator->GetWidth() / 2, -1.0F * m_pIndicator->GetWidth() / 2);
		}
		else
		{
			outer.Inflate(-0.5F * m_nCircleWidth, -0.5F * m_nCircleWidth);
		}
		outer.Inflate(-1, -1);


		if (m_dwGradientColor.GetARGB() == 0)
		{
			//��ʹ�ý���ɫ��ֱ����ǰ��ɫ����
			Gdiplus::Pen fgPen(m_dwForegroundColor.GetARGB(), static_cast<Gdiplus::REAL>(m_nCircleWidth));
			graphics.DrawArc(&bgPen, outer, 270, 360);   //270�������濪ʼ��������Ϊ0�Ļ��������ұ߿�ʼ
			graphics.DrawArc(&fgPen, outer, 270, static_cast<Gdiplus::REAL>(direction * 360 * (m_nValue - m_nMin) / (m_nMax - m_nMin)));
		}
		else
		{
			Gdiplus::REAL factors[4] = { 0.0f, 0.4f, 0.6f, 1.0f };
			Gdiplus::REAL positions[4] = { 0.0f, 0.2f, 0.8f, 1.0f };

			Gdiplus::LinearGradientBrush lgbrush(rcBorder, m_dwForegroundColor.GetARGB(), m_dwGradientColor.GetARGB(), Gdiplus::LinearGradientModeVertical);
			lgbrush.SetBlend(factors, positions, 4);
			graphics.DrawArc(&bgPen, outer, 270, 360);
			Gdiplus::Pen fgPen(&lgbrush, static_cast<Gdiplus::REAL>(m_nCircleWidth));
			graphics.DrawArc(&fgPen, outer, 270, static_cast<Gdiplus::REAL>(direction * 360 * (m_nValue - m_nMin) / (m_nMax - m_nMin)));

		}

		//����תָʾ��ͼ�꣬��Ҫ�õ�����
		if (m_pIndicator)
		{
			Gdiplus::Matrix matrix;
			matrix.RotateAt(static_cast<Gdiplus::REAL>(direction * 360 * (m_nValue - m_nMin) / (m_nMax - m_nMin)), Gdiplus::PointF(static_cast<Gdiplus::REAL>(center.x), static_cast<Gdiplus::REAL>(center.y)), Gdiplus::MatrixOrderAppend);
			graphics.SetTransform(&matrix);
			Gdiplus::RectF rectf;
			rectf.X = static_cast<Gdiplus::REAL>(center.x - m_pIndicator->GetWidth() / 2);
			rectf.Y = outer.Y + bordersize / 2 - m_pIndicator->GetHeight() / 2;
			rectf.Width = static_cast<Gdiplus::REAL>(m_pIndicator->GetWidth());
			rectf.Height = static_cast<Gdiplus::REAL>(m_pIndicator->GetHeight());
			graphics.DrawImage(m_pIndicator, rectf);
		}
		pRender->ReleaseDC(hdc);
	}
}

void CircleProgress::ClearImageCache()
{
	__super::ClearImageCache();
	if (m_pIndicator)
	{
		delete m_pIndicator;
		m_pIndicator = nullptr;
	}
}

void CircleProgress::SetCircular(bool bCircular /*= true*/)
{
	m_bCircular = bCircular;
	Invalidate();
}

void CircleProgress::SetClockwiseRotation(bool bClockwise /*= true*/)
{
	if (bClockwise != m_bClockwise)
	{
		m_bClockwise = bClockwise;
		if (m_pIndicator)
		{
			//�Ѿ���ת��ͼƬ����ת���෴�ķ���
			m_pIndicator->RotateFlip(Gdiplus::Rotate180FlipNone);
		}

	}
}

void CircleProgress::SetCircleWidth(int nCircleWidth)
{
	m_nCircleWidth = nCircleWidth;
	Invalidate();
}


void CircleProgress::SetBackgroudColor(const std::wstring& strColor)
{
	m_dwBackgroundColor = GlobalManager::GetTextColor(strColor);
	ASSERT(m_dwBackgroundColor.GetARGB() != 0);
	Invalidate();
}

void CircleProgress::SetForegroudColor(const std::wstring& strColor)
{
	m_dwForegroundColor = GlobalManager::GetTextColor(strColor);
	ASSERT(m_dwForegroundColor.GetARGB() != 0);
	Invalidate();
}

void CircleProgress::SetIndicator(const std::wstring& sIndicatorImage)
{
	if (m_sIndicatorImage != sIndicatorImage)
	{
		m_sIndicatorImage = sIndicatorImage;
		if (m_pIndicator)
		{
			delete m_pIndicator;
			m_pIndicator = nullptr;
		}
		std::wstring imagepath = m_sIndicatorImage;
		if (!::PathFileExistsW(imagepath.c_str()) && (GetWindow() != nullptr)) {
			imagepath = GlobalManager::GetResourcePath() + GetWindow()->GetResourcePath() + imagepath;
		}
		if (!::PathFileExistsW(imagepath.c_str())) {
			return;
		}
		m_pIndicator = new Gdiplus::Image(imagepath.c_str());

		Gdiplus::Status state = m_pIndicator->GetLastStatus();
		if (Gdiplus::Ok == state)
		{
			// �ٶ�ͼƬָ����
			m_pIndicator->RotateFlip(m_bClockwise ? Gdiplus::Rotate90FlipNone : Gdiplus::Rotate270FlipNone);
			Invalidate();
		}
	}
}

void CircleProgress::SetCircleGradientColor(const std::wstring& strColor)
{
	m_dwGradientColor = GlobalManager::GetTextColor(strColor);
	ASSERT(m_dwGradientColor.GetARGB() != 0);
	Invalidate();
}

}
