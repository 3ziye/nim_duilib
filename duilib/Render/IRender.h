#ifndef UI_RENDER_IRENDER_H_
#define UI_RENDER_IRENDER_H_

#pragma once

#include "duilib/duilib_defs.h"
#include "duilib/Core/Define.h"
#include "duilib/Render/UiColor.h"
#include "base/callback/callback.h"

namespace ui 
{

class UILIB_API IClip : public virtual nbase::SupportWeakCallback
{
public:
	virtual void CreateClip(HDC hDC, UiRect rc) = 0;
	virtual void CreateRoundClip(HDC hDC, UiRect rc, int width, int height) = 0;
	virtual void ClearClip(HDC hDC) = 0;
};

class IRender;
class UILIB_API AutoClip
{
public:
	AutoClip(IRender* pRender, const UiRect& rc, bool bClip = true);
	AutoClip(IRender* pRender, const UiRect& rcRound, int width, int height, bool bClip = true);
	~AutoClip();

private:
	IRender *m_pRender;
	bool			m_bClip;
};

class UILIB_API IBitmap : public virtual nbase::SupportWeakCallback
{
public:
	/** �����ݳ�ʼ����ARGB��ʽ��
	@param [in] nWidth ���
	@param [in] nHeight �߶�
	@param [in] flipHeight �Ƿ�תλͼ�����Ϊtrue������λͼ��ʱ�������Ͻ�ΪԲ�㣬ͼ�����Ǵ��ϵ��µģ�
	                       ���Ϊfalse���������½�ΪԲ�㣬ͼ�����Ǵ��µ��ϡ�
	@param [in] pPixelBits λͼ����, ���Ϊnullptr��ʾ���ڿ�λͼ�������Ϊnullptr�������ݳ���Ϊ��nWidth*4*nHeight
	*/
	virtual bool Init(uint32_t nWidth, uint32_t nHeight, bool flipHeight, const void* pPixelBits) = 0;

	/** ��ȡͼƬ���
	*/
	virtual uint32_t GetWidth() const = 0;

	/** ��ȡͼƬ�߶�
	*/
	virtual uint32_t GetHeight() const = 0;

	/** ��ȡͼƬ��С
	@return ͼƬ��С
	*/
	virtual UiSize GetSize() const = 0;

	/** ����λͼ���ݣ����ݳ��� = GetWidth() * GetHeight() * 4
	*/
	virtual void* LockPixelBits() = 0;

	/** �ͷ�λͼ����
	*/
	virtual void UnLockPixelBits() = 0;

	/** ��¡�����µĵ�λͼ
	*@return ���������ɵ�λͼ�ӿڣ��ɵ��÷��ͷ���Դ
	*/
	virtual IBitmap* Clone() = 0;
};

class UILIB_API IPen : public virtual nbase::SupportWeakCallback
{
public:
	IPen(UiColor color, int width = 1) : color_(color) { (void)width; };
	IPen(const IPen& r) : color_(r.color_) {};
	IPen& operator=(const IPen& r) = delete;

	virtual IPen* Clone() = 0;

	virtual void SetWidth(int width) = 0;
	virtual int GetWidth() = 0;
	virtual void SetColor(UiColor color) = 0;
	virtual UiColor GetColor() { return color_; };

	enum LineCap
	{
		LineCapFlat = 0,
		LineCapSquare = 1,
		LineCapRound = 2,
		LineCapTriangle = 3
	};
	virtual void SetStartCap(LineCap cap) = 0;
	virtual void SetEndCap(LineCap cap) = 0;
	virtual void SetDashCap(LineCap cap) = 0;
	virtual LineCap GetStartCap() = 0;
	virtual LineCap GetEndCap() = 0;
	virtual LineCap GetDashCap() = 0;

	enum LineJoin
	{
		LineJoinMiter = 0,
		LineJoinBevel = 1,
		LineJoinRound = 2,
		LineJoinMiterClipped = 3
	};
	virtual void SetLineJoin(LineJoin join) = 0;
	virtual LineJoin GetLineJoin() = 0;

	enum DashStyle
	{
		DashStyleSolid,          // 0
		DashStyleDash,           // 1
		DashStyleDot,            // 2
		DashStyleDashDot,        // 3
		DashStyleDashDotDot,     // 4
		DashStyleCustom          // 5
	};
	virtual void SetDashStyle(DashStyle style) = 0;
	virtual DashStyle GetDashStyle() = 0;

protected:
	UiColor color_;
};

class UILIB_API IBrush : public virtual nbase::SupportWeakCallback
{
public:
	IBrush(UiColor color) : color_(color){};
	IBrush(const IBrush& r)	: color_(r.color_)	{};
	IBrush& operator=(const IBrush& r) = delete;

	virtual IBrush* Clone() = 0;
	virtual UiColor GetColor() { return color_; };
protected:
	UiColor color_;
};

class UILIB_API IPath : public virtual nbase::SupportWeakCallback
{
public:
	IPath(){};
	IPath(const IPath& r) { (void)r; };
	IPath& operator=(const IPath& r) = delete;

	virtual IPath* Clone() = 0;
	virtual void Reset() = 0;

	enum FillMode
	{
		FillModeAlternate,        // 0
		FillModeWinding           // 1
	};
	virtual void SetFillMode(FillMode mode) = 0;
	virtual FillMode GetFillMode() = 0;

	virtual void StartFigure() = 0;
	virtual void CloseFigure() = 0;

	virtual void AddLine(int x1, int y1, int x2, int y2) = 0;
	virtual void AddLines(const UiPoint* points, int count) = 0;
	virtual void AddBezier(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) = 0;
	virtual void AddCurve(const UiPoint* points, int count) = 0;
	virtual void AddRect(int left, int top, int right, int bottom) = 0;
	virtual void AddRect(const UiRect& rect) = 0;
	virtual void AddEllipse(int left, int top, int right, int bottom) = 0;
	virtual void AddEllipse(const UiRect& rect) = 0;
	virtual void AddArc(int x, int y, int width, int height, float startAngle, float sweepAngle) = 0;
	virtual void AddPie(int x, int y, int width, int height, float startAngle, float sweepAngle) = 0;
	virtual void AddPolygon(const UiPoint* points, int count) = 0;

	virtual UiRect GetBound(const IPen* pen) = 0;
	virtual bool IsContainsPoint(int x, int y) = 0;
	virtual bool IsStrokeContainsPoint(int x, int y, const IPen* pen) = 0;
};

class UILIB_API IRender : public virtual nbase::SupportWeakCallback
{
public:
	virtual HDC GetDC() = 0;
	virtual bool Resize(int width, int height) = 0;
	virtual void Clear() = 0;
	virtual std::unique_ptr<IRender> Clone() = 0;

	/** ����λͼ
	*@return ����λͼ�ӿڣ����غ��ɵ��÷�������Դ�������ͷ���Դ�ȣ�
	*/
	virtual IBitmap* DetachBitmap() = 0;

	virtual int	GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual void ClearAlpha(const UiRect& rcDirty, int alpha = 0) = 0;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding, int alpha) = 0;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding = UiRect()) = 0;

	virtual bool IsRenderTransparent() const = 0;
	virtual bool SetRenderTransparent(bool bTransparent) = 0;

	virtual void Save() = 0;
	virtual void Restore() = 0;
	virtual UiPoint OffsetWindowOrg(UiPoint ptOffset) = 0;
	virtual UiPoint SetWindowOrg(UiPoint ptOffset) = 0;
	virtual UiPoint GetWindowOrg() const = 0;

	virtual void SetClip(const UiRect& rc) = 0;
	virtual void SetRoundClip(const UiRect& rcItem, int width, int height) = 0;
	virtual void ClearClip() = 0;

	virtual HRESULT BitBlt(int x, int y, int cx, int cy, HDC hdcSrc, int xSrc = 0, int yScr = 0, DWORD rop = SRCCOPY) = 0;
	virtual bool StretchBlt(int xDest, int yDest, int widthDest, int heightDest, HDC hdcSrc, int xSrc, int yScr, int widthSrc, int heightSrc, DWORD rop = SRCCOPY) = 0;
	virtual bool AlphaBlend(int xDest, int yDest, int widthDest, int heightDest, HDC hdcSrc, int xSrc, int yScr, int widthSrc, int heightSrc, BYTE uFade = 255) = 0;

	/** ����ͼƬ�����þŹ���ʽ����ͼƬ��
	* @param [in] rcPaint ��ǰȫ���ɻ����������ڱ���ǿɻ�������Ļ��ƣ�����߻������ܣ�
	* @param [in] pBitmap ���ڻ��Ƶ�λͼ�ӿ�
	* @param [in] bAlphaChannel ͼƬ�Ƿ���Alphaͨ��
	* @param [in] rcImageDest ���Ƶ�Ŀ������
	* @param [in] rcImageSource ���Ƶ�ԴͼƬ����
	* @param [in] rcImageCorners ����ԴͼƬ�ı߽���Ϣ�����ھŹ������
	* @param [in] bBitmapDpiScale λͼ�ߴ��Ƿ��Ѿ�����DPI��Ӧ
	* @param [in] uFade ͸���ȣ�0 - 255��
	* @param [in] xtiled ����ƽ��
	* @param [in] ytiled ����ƽ��
	* @param [in] fullxtiled ���Ϊtrue������ƽ�̻���ʱ��ȷ������������ͼƬ���ò�������xtiledΪtrueʱ��Ч
	* @param [in] fullytiled ���Ϊtrue������ƽ�̻���ʱ��ȷ������������ͼƬ���ò�������ytiledΪtrueʱ��Ч
	* @param [in] nTiledMargin ƽ�̻���ʱ��ͼƬ�ĺ������������ò�������xtiledΪtrue����ytiledΪtrueʱ��Ч
	*/
	virtual void DrawImage(const UiRect& rcPaint, 
						   IBitmap* pBitmap, 
						   bool bAlphaChannel,
						   const UiRect& rcImageDest, 
						   const UiRect& rcImageSource, 
						   UiRect rcImageCorners,
						   bool bBitmapDpiScale = false,
						   uint8_t uFade = 255,
						   bool xtiled = false, 
						   bool ytiled = false, 
						   bool fullxtiled = true, 
						   bool fullytiled = true, 
						   int nTiledMargin = 0) = 0;

	virtual void DrawColor(const UiRect& rc, UiColor dwColor, BYTE uFade = 255) = 0;
	virtual void DrawLine(const UiPoint& pt1, const UiPoint& pt2, UiColor penColor, int nWidth) = 0;
	virtual void DrawRect(const UiRect& rc, UiColor penColor, int nWidth) = 0;
	virtual void DrawRoundRect(const UiRect& rc, const UiSize& roundSize, UiColor penColor, int nWidth) = 0;
	virtual void DrawPath(const IPath* path, const IPen* pen) = 0;
	virtual void FillPath(const IPath* path, const IBrush* brush) = 0;

	virtual void DrawText(const UiRect& rc, const std::wstring& strText, UiColor dwTextColor, const std::wstring& strFontId, UINT uStyle, BYTE uFade = 255, bool bLineLimit = false, bool bFillPath = false) = 0;
	virtual UiRect MeasureText(const std::wstring& strText, const std::wstring& strFontId, UINT uStyle, int width = DUI_NOSET_VALUE) = 0;

	virtual void DrawBoxShadow(const UiRect& rc, const UiSize& roundSize, const UiPoint& cpOffset, int nBlurRadius, int nBlurSize, int nSpreadSize, UiColor dwColor, bool bExclude) = 0;
};

class UILIB_API IRenderFactory
{
public:
	virtual ~IRenderFactory() = default;
	virtual ui::IPen* CreatePen(UiColor color, int width = 1) = 0;
	virtual ui::IBrush* CreateBrush(UiColor corlor) = 0;
	virtual ui::IPath* CreatePath() = 0;
	virtual ui::IBitmap* CreateBitmap() = 0;
	virtual ui::IRender* CreateRender() = 0;
};

} // namespace ui

#endif // UI_RENDER_IRENDER_H_
