#ifndef UI_CORE_RENDER_GDIPLUS_H_
#define UI_CORE_RENDER_GDIPLUS_H_

#pragma once

#include "duilib/Render/IRender.h"
#include "duilib/Render/Clip.h"
#include "duilib/Render/RenderBitmap_GDI.h"

namespace ui 
{

class UILIB_API Render_GdiPlus : public IRender
{
public:
	Render_GdiPlus();
	virtual ~Render_GdiPlus();

	virtual HDC GetDC() override;
	virtual bool Resize(int width, int height) override;
	virtual void Clear() override;
	virtual std::unique_ptr<IRender> Clone() override;

	/** ����λͼ
	*@return ����λͼ�ӿڣ����غ��ɵ��÷�������Դ�������ͷ���Դ�ȣ�
	*/
	virtual IBitmap* DetachBitmap();

	virtual int	GetWidth() override;
	virtual int GetHeight() override;
	virtual void ClearAlpha(const UiRect& rcDirty, int alpha = 0) override;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding, int alpha) override;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding = UiRect()) override;

	virtual bool IsRenderTransparent() const override;
	virtual bool SetRenderTransparent(bool bTransparent) override;

	virtual void Save() override;
	virtual void Restore() override;

	virtual UiPoint OffsetWindowOrg(UiPoint ptOffset) override;
	virtual UiPoint SetWindowOrg(UiPoint ptOffset) override;
	virtual UiPoint GetWindowOrg() const override;

	virtual void SetClip(const UiRect& rc) override;
	virtual void SetRoundClip(const UiRect& rc, int width, int height) override;
	virtual void ClearClip() override;

	virtual bool BitBlt(int x, int y, int cx, int cy, HDC hdcSrc, int xSrc = 0, int yScr = 0, DWORD rop = SRCCOPY) override;
	virtual bool StretchBlt(int xDest, int yDest, int widthDest, int heightDest, HDC hdcSrc, int xSrc, int yScr, int widthSrc, int heightSrc, DWORD rop = SRCCOPY) override;
	virtual bool AlphaBlend(int xDest, int yDest, int widthDest, int heightDest, HDC hdcSrc, int xSrc, int yScr, int widthSrc, int heightSrc, uint8_t alpha = 255) override;

	/** ����ͼƬ�����þŹ���ʽ����ͼƬ��
	* @param [in] rcPaint ��ǰȫ���ɻ����������ڱ���ǿɻ�������Ļ��ƣ�����߻������ܣ�
	* @param [in] pBitmap ���ڻ��Ƶ�λͼ�ӿ�
	* @param [in] rcImageDest ���Ƶ�Ŀ������
	* @param [in] rcImageSource ���Ƶ�ԴͼƬ����
	* @param [in] rcImageCorners ����ԴͼƬ�ı߽���Ϣ�����ھŹ������
	* @param [in] bBitmapDpiScaled λͼ�ߴ��Ƿ��Ѿ�����DPI��Ӧ
	* @param [in] uFade ͸���ȣ�0 - 255��
	* @param [in] xtiled ����ƽ��
	* @param [in] ytiled ����ƽ��
	* @param [in] fullxtiled ���Ϊtrue������ƽ�̻���ʱ��ȷ������������ͼƬ���ò�������xtiledΪtrueʱ��Ч
	* @param [in] fullytiled ���Ϊtrue������ƽ�̻���ʱ��ȷ������������ͼƬ���ò�������ytiledΪtrueʱ��Ч
	* @param [in] nTiledMargin ƽ�̻���ʱ��ͼƬ�ĺ������������ò�������xtiledΪtrue����ytiledΪtrueʱ��Ч
	*/
	virtual void DrawImage(const UiRect& rcPaint, 
						   IBitmap* pBitmap, 
						   const UiRect& rcImageDest, 
						   UiRect rcImageSource, 
						   UiRect rcImageCorners,
						   bool bBitmapDpiScaled = false,
						   uint8_t uFade = 255,
						   bool xtiled = false, 
						   bool ytiled = false, 
						   bool fullxtiled = true, 
						   bool fullytiled = true, 
						   int nTiledMargin = 0) override;

	virtual void DrawColor(const UiRect& rc, UiColor dwColor, BYTE uFade = 255) override;
	virtual void DrawLine(const UiPoint& pt1, const UiPoint& pt2, UiColor penColor, int nWidth) override;
	virtual void DrawRect(const UiRect& rc, UiColor penColor, int nWidth) override;
	virtual void DrawRoundRect(const UiRect& rc, const UiSize& roundSize, UiColor penColor, int nWidth) override;
	virtual void DrawPath(const IPath* path, const IPen* pen) override;
	virtual void FillPath(const IPath* path, const IBrush* brush) override;

	virtual void DrawText(const UiRect& rc, const std::wstring& strText, UiColor dwTextColor, const std::wstring& strFontId, UINT uStyle, BYTE uFade = 255) override;


	virtual UiRect MeasureText(const std::wstring& strText, const std::wstring& strFontId, UINT uStyle, int width = DUI_NOSET_VALUE) override;

	void DrawBoxShadow(const UiRect& rc, const UiSize& roundSize, const UiPoint& cpOffset, int nBlurRadius, int nSpreadRadius, UiColor dwColor, bool bExclude) override;

private:
	HDC			m_hDC;
	int			m_saveDC;
	HBITMAP		m_hOldBitmap;

	bool		m_bTransparent;
	GdiClip		m_clip;
	RenderBitmap_GDI m_bitmap;
};

} // namespace ui

#endif // UI_CORE_RENDER_GDIPLUS_H_
