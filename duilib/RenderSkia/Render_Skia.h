#ifndef UI_RENDER_SKIA_RENDER_H_
#define UI_RENDER_SKIA_RENDER_H_

#pragma once

#include "duilib/Render/IRender.h"

//Skia������ǰ������
class SkCanvas;
struct SkPoint;
class SkPaint;

namespace ui 
{

class Bitmap_Skia;

class UILIB_API Render_Skia : public IRender
{
public:
	Render_Skia();
	virtual ~Render_Skia();

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

	virtual UiPoint OffsetWindowOrg(UiPoint ptOffset) override;
	virtual UiPoint SetWindowOrg(UiPoint ptOffset) override;
	virtual UiPoint GetWindowOrg() const override;

	virtual void SaveClip(int& nState) override;
	virtual void RestoreClip(int nState) override;
	virtual void SetClip(const UiRect& rc) override;
	virtual void SetRoundClip(const UiRect& rc, int width, int height) override;
	virtual void ClearClip() override;

	virtual bool BitBlt(int x, int y, int cx, int cy, IBitmap* pSrcBitmap, int xSrc, int ySrc, RopMode rop) override;
	virtual bool BitBlt(int x, int y, int cx, int cy, IRender* pSrcRender, int xSrc, int ySrc, RopMode rop) override;
	virtual bool StretchBlt(int xDest, int yDest, int widthDest, int heightDest, IRender* pSrcRender, int xSrc, int ySrc, int widthSrc, int heightSrc, RopMode rop) override;
	virtual bool AlphaBlend(int xDest, int yDest, int widthDest, int heightDest, IRender* pSrcRender, int xSrc, int ySrc, int widthSrc, int heightSrc, uint8_t alpha = 255) override;

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

	virtual void DrawColor(const UiRect& rc, UiColor dwColor, uint8_t uFade = 255) override;
	virtual void DrawLine(const UiPoint& pt1, const UiPoint& pt2, UiColor penColor, int nWidth) override;
	virtual void DrawRect(const UiRect& rc, UiColor penColor, int nWidth) override;
	virtual void DrawRoundRect(const UiRect& rc, const UiSize& roundSize, UiColor penColor, int nWidth) override;
	
	virtual void DrawPath(const IPath* path, const IPen* pen) override;
	virtual void FillPath(const IPath* path, const IBrush* brush) override;

	virtual void DrawString(const UiRect& rc, const std::wstring& strText,
		                    UiColor dwTextColor, const std::wstring& strFontId, uint32_t uFormat, uint8_t uFade = 255) override;


	virtual UiRect MeasureString(const std::wstring& strText,
		                         const std::wstring& strFontId, uint32_t uFormat, int width = DUI_NOSET_VALUE) override;

	void DrawBoxShadow(const UiRect& rc, const UiSize& roundSize, const UiPoint& cpOffset, int nBlurRadius, int nSpreadRadius, UiColor dwColor, bool bExclude) override;

#ifdef UILIB_IMPL_WINSDK
	/** ��ȡDC���������ʹ�ú���Ҫ����ReleaseDC�ӿ��ͷ���Դ
	*/
	virtual HDC GetDC() override;

	/** �ͷ�DC��Դ
	* @param [in] hdc ��Ҫ�ͷŵ�DC���
	*/
	virtual void ReleaseDC(HDC hdc) override;
#endif

private:
	/** λͼ���Ʒ�װ
	*/
	bool BitBlt(int x, int y, int cx, int cy, Bitmap_Skia* pSrcBitmap, int xSrc, int ySrc, RopMode rop);

	/** ��ȡGDI�Ĺ�դ��������
	*/
	void SetRopMode(SkPaint& skPaint, RopMode rop) const;

private:

	/** λͼ����
	*/
	std::unique_ptr<Bitmap_Skia> m_pBitmapSkia;

	/** Canvas �ӿ�
	*/
	SkCanvas* m_pSkCanvas;

	/** Canval�����״̬
	*/
	int m_saveCount;

	/** ��������
	*/
	SkPaint* m_pSkPaint;

	/** ��ͼ��ԭ������
	*/
	SkPoint* m_pSkPointOrg;

	/** �Ƿ�͸��
	*/
	bool m_bTransparent;

	/** ������DC���
	*/
	HDC m_hDC;
};

} // namespace ui

#endif // UI_RENDER_SKIA_RENDER_H_
