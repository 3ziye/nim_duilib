#ifndef UI_RENDER_IRENDER_H_
#define UI_RENDER_IRENDER_H_

#pragma once

#include "duilib/duilib_defs.h"
#include "duilib/Core/Define.h"
#include "duilib/Render/UiColor.h"
#include "base/callback/callback.h"

namespace ui 
{

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

	/** ��λͼ�Ƿ���͸������(��͸��ͨ���У����в���255������)
	*/
	virtual bool IsAlphaBitmap() const = 0;

	/** ��¡�����µĵ�λͼ
	*@return ���������ɵ�λͼ�ӿڣ��ɵ��÷��ͷ���Դ
	*/
	virtual IBitmap* Clone() = 0;
};

class UILIB_API IPen : public virtual nbase::SupportWeakCallback
{
public:

	virtual IPen* Clone() const = 0;

	virtual void SetWidth(int width) = 0;
	virtual int GetWidth() const = 0;
	virtual void SetColor(UiColor color) = 0;
	virtual UiColor GetColor() const = 0;

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
	virtual LineCap GetStartCap() const = 0;
	virtual LineCap GetEndCap() const = 0;
	virtual LineCap GetDashCap() const = 0;

	enum LineJoin
	{
		LineJoinMiter = 0,
		LineJoinBevel = 1,
		LineJoinRound = 2,
		LineJoinMiterClipped = 3
	};
	virtual void SetLineJoin(LineJoin join) = 0;
	virtual LineJoin GetLineJoin() const = 0;

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
	virtual DashStyle GetDashStyle() const = 0;
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

/** ��դ��������
*/
enum class UILIB_API RopMode
{
	kSrcCopy,	//��Ӧ�� SRCCOPY
	kDstInvert, //��Ӧ�� DSTINVERT
	kSrcInvert, //��Ӧ�� SRCINVERT
	kSrcAnd     //��Ӧ�� SRCAND
};

/** �����ı�ʱ�ĸ�ʽ
*/
enum UILIB_API DrawStringFormat
{
	TEXT_SINGLELINE		= DT_SINGLELINE,
	TEXT_LEFT			= DT_LEFT,
	TEXT_CENTER			= DT_CENTER,
	TEXT_RIGHT			= DT_RIGHT,
	TEXT_TOP			= DT_TOP,
	TEXT_VCENTER		= DT_VCENTER,
	TEXT_BOTTOM			= DT_BOTTOM,
	TEXT_END_ELLIPSIS	= DT_END_ELLIPSIS,
	TEXT_PATH_ELLIPSIS  = DT_PATH_ELLIPSIS,
	TEXT_NOCLIP			= DT_NOCLIP,
};

class UILIB_API IRender : public virtual nbase::SupportWeakCallback
{
public:
	/** ��ȡ�������
	*/
	virtual int	GetWidth() = 0;

	/** ��ȡ�����߶�
	*/
	virtual int GetHeight() = 0;

	/** ����������С
	*/
	virtual bool Resize(int width, int height) = 0;

	/** �жϻ����Ƿ�֧��͸��
	*/
	virtual bool IsRenderTransparent() const = 0;

	/** ���û����Ƿ�֧��͸��
	*/
	virtual bool SetRenderTransparent(bool bTransparent) = 0;

	/** ���ô�������ԭ������ƫ�ƣ�
	 *  ��ԭ����ԭ��xֵ����ptOffset.x����Ϊ�µ�����ԭ��x;
	 *  ��ԭ����ԭ��yֵ����ptOffset.y����Ϊ�µ�����ԭ��y;
	 * @param [in] ptOffset ����ԭ������ƫ����
	 *@return ����ԭ��������ԭ������(x,y)
	 */
	virtual UiPoint OffsetWindowOrg(UiPoint ptOffset) = 0;

	/** ����(pt.x, pt.y)ӳ�䵽����ԭ�� (0, 0)
	 *@return ����ԭ��������ԭ������(x,y)
	 */
	virtual UiPoint SetWindowOrg(UiPoint pt) = 0;

	/** ��ȡ����ԭ������(x,y)
	 * @return ���ص�ǰ������ԭ������(x,y)
	 */
	virtual UiPoint GetWindowOrg() const = 0;
	
	/** ����ָ���豸�����ĵĵ�ǰ״̬
	* @param [out] ���ر�����豸�����ı�־����RestoreClip��ʱ����Ϊ��������
	*/
	virtual void SaveClip(int& nState) = 0;

	/** ���豸�����Ļ�ԭ�����һ�α����״̬
	* @param [in] ������豸�����ı�־����SaveClip���أ�
	*/
	virtual void RestoreClip(int nState) = 0;

	/** ���þ��μ������򣬲����浱ǰ�豸�����ĵ�״̬
	* @param [in] rc��������
	*/
	virtual void SetClip(const UiRect& rc) = 0;

	/** ����Բ�Ǿ��μ������򣬲����浱ǰ�豸�����ĵ�״̬
	* @param [in] rcItem ��������
	* @param [in] width Բ�ǵĿ��
	* @param [in] height Բ�ǵĵĶ�
	*/
	virtual void SetRoundClip(const UiRect& rcItem, int width, int height) = 0;

	/** ������μ������򣬲��ָ��豸�����ĵ����һ�α����״̬
	*/
	virtual void ClearClip() = 0;

	/** ����ִ�����ָ��Դ�豸�����ĵ�Ŀ���豸�������е����ؾ��ζ�Ӧ����ɫ���ݵ�λ�鴫��
	* @param [in] x Ŀ��������Ͻǵ� x ����
	* @param [in] y Ŀ��������Ͻǵ� y ����
	* @param [in] cx Դ���κ�Ŀ����εĿ��
	* @param [in] cy Դ��Ŀ����εĸ߶�
	* @param [in] pSrcBitmap ԴͼƬ�ӿ�
	* @param [in] pSrcRender ԴRender����
	* @param [in] xSrc Դ�������Ͻǵ� x ����
	* @param [in] ySrc Դ�������Ͻǵ� y ����
	* @param [in] rop ��դ��������
	*/
	virtual bool BitBlt(int x, int y, int cx, int cy, 
		                IBitmap* pSrcBitmap, int xSrc, int ySrc,
		                RopMode rop) = 0;
	virtual bool BitBlt(int x, int y, int cx, int cy, 
		                IRender* pSrcRender, int xSrc, int ySrc,
		                RopMode rop) = 0;

	/** ������һ��λͼ��Դ���θ��Ƶ�Ŀ������У��������ѹ��λͼ����ӦĿ����εĳߴ磨���б�Ҫ���� 
	    ϵͳ���ݵ�ǰ��Ŀ���豸�����������õ�����ģʽ�����ѹ��λͼ��
	* @param [in] xDest Ŀ��������Ͻǵ� x ����
	* @param [in] yDest Ŀ��������Ͻǵ� y ����
	* @param [in] widthDest Ŀ����εĿ��
	* @param [in] heightDest Ŀ����εĸ߶�
	* @param [in] pSrcRender ԴRender����
	* @param [in] xSrc Դ�������Ͻǵ� x ����
	* @param [in] ySrc Դ�������Ͻǵ� y ����
	* @param [in] widthSrc Դ���εĿ��
	* @param [in] heightSrc Դ���εĸ߶�
	* @param [in] rop ��դ��������
	*/
	virtual bool StretchBlt(int xDest, int yDest, int widthDest, int heightDest,
		                    IRender* pSrcRender, int xSrc, int ySrc, int widthSrc, int heightSrc,
		                    RopMode rop) = 0;


	/** ��ʾ����͸�����͸�����ص�λͼ�����Դ���κ�Ŀ����εĴ�С����ͬ���������Դλͼ��ƥ��Ŀ����Ρ�
	* @param [in] xDest Ŀ��������Ͻǵ� x ����
	* @param [in] yDest Ŀ��������Ͻǵ� y ����
	* @param [in] widthDest Ŀ����εĿ��
	* @param [in] heightDest Ŀ����εĸ߶�
	* @param [in] pSrcRender ԴRender����
	* @param [in] xSrc Դ�������Ͻǵ� x ����
	* @param [in] ySrc Դ�������Ͻǵ� y ����
	* @param [in] widthSrc Դ���εĿ��
	* @param [in] heightSrc Դ���εĸ߶�
	* @param [in] alpha ͸���� alpha ֵ��0 - 255��
	*/
	virtual bool AlphaBlend(int xDest, int yDest, int widthDest, int heightDest,
		                    IRender* pSrcRender, int xSrc, int ySrc, int widthSrc, int heightSrc,
		                    uint8_t alpha = 255) = 0;

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
						   int nTiledMargin = 0) = 0;

	/** ����ֱ��
	* @param [in] pt1 ��ʼ������
	* @param [in] pt2 ��ֹ������
	* @param [in] penColor ���ʵ���ɫֵ
	* @param [in] nWidth ���ʵĿ��
	*/
	virtual void DrawLine(const UiPoint& pt1, const UiPoint& pt2, UiColor penColor, int nWidth) = 0;

	/** ���ƾ���
	* @param [in] rc ��������
	* @param [in] penColor ���ʵ���ɫֵ
	* @param [in] nWidth ���ʵĿ��
	*/
	virtual void DrawRect(const UiRect& rc, UiColor penColor, int nWidth) = 0;

	/** ����ɫ������
	* @param [in] rc Ŀ���������
	* @param [in] dwColor ��ɫֵ
	* @param [in] uFade ͸���ȣ�0 - 255��
	*/
	virtual void FillRect(const UiRect& rc, UiColor dwColor, uint8_t uFade = 255) = 0;

	/** ����Բ�Ǿ���
	* @param [in] rc ��������
	* @param [in] roundSize Բ�ǵĿ�͸�
	* @param [in] penColor ���ʵ���ɫֵ
	* @param [in] nWidth ���ʵĿ��
	*/
	virtual void DrawRoundRect(const UiRect& rc, const UiSize& roundSize, UiColor penColor, int nWidth) = 0;

	/** ����ɫ���Բ�Ǿ���
	* @param [in] rc ��������
	* @param [in] roundSize Բ�ǵĿ�͸�
	* @param [in] dwColor ��ɫֵ
	* @param [in] uFade ͸���ȣ�0 - 255��
	*/
	virtual void FillRoundRect(const UiRect& rc, const UiSize& roundSize, UiColor dwColor, uint8_t uFade = 255) = 0;

	virtual void DrawPath(const IPath* path, const IPen* pen) = 0;
	virtual void FillPath(const IPath* path, const IBrush* brush) = 0;

	/** ����ָ���ı��ַ����Ŀ�Ⱥ͸߶�
	* @param [in] strText ��������
	* @param [in] strFontId ���ֵ�����ID������������ȫ������������
	* @param [in] uFormat ���ֵĸ�ʽ���μ� enum DrawStringFormat ���Ͷ���
	* @param [in] width ��ǰ��������ƿ��
	* @return �����ı��ַ����Ŀ�Ⱥ͸߶ȣ��Ծ��α�ʾ���
	*/
	virtual UiRect MeasureString(const std::wstring& strText, 
		                         const std::wstring& strFontId, 
		                         uint32_t uFormat,
		                         int width = DUI_NOSET_VALUE) = 0;
	/** ��������
	* @param [in] ��������
	* @param [in] strText ��������
	* @param [in] dwTextColor ������ɫֵ
	* @param [in] strFontId ���ֵ�����ID������������ȫ������������
	* @param [in] uFormat ���ֵĸ�ʽ���μ� enum DrawStringFormat ���Ͷ���
	* @param [in] uFade ͸���ȣ�0 - 255��
	*/
	virtual void DrawString(const UiRect& rc,
		                    const std::wstring& strText,
		                    UiColor dwTextColor,
		                    const std::wstring& strFontId, 
		                    uint32_t uFormat,
		                    uint8_t uFade = 255) = 0;

	/** ��ָ��������Χ������Ӱ����˹ģ����
	* @param [in] rc ��������
	* @param [in] roundSize ��Ӱ��Բ�ǿ�Ⱥ͸߶�
	* @param [in] cpOffset ������Ӱƫ������offset-x �� offset-y��
	*                      <offset-x> ����ˮƽƫ����������Ǹ�ֵ����Ӱλ�ھ�����ߡ� 
	*                      <offset-y> ���ô�ֱƫ����������Ǹ�ֵ����Ӱλ�ھ������档
	* @param [in] nBlurRadius ģ���뾶���뾶������� 0 �� 255 �ķ�Χ�ڡ�
	*                         ֵԽ��ģ�����Խ����Ӱ��Խ��Խ���� ����Ϊ��ֵ��Ĭ��Ϊ0����ʱ��Ӱ��Ե������
	* @param [in] nSpreadRadius ��չ�뾶����ģ���������rc���α�Ե���ٸ����ء�
	*                         ȡ��ֵʱ����Ӱ����ȡ��ֵʱ����Ӱ������Ĭ��Ϊ0����ʱ��Ӱ��Ԫ��ͬ����
	* @param [in] dwColor ��Ӱ����ɫֵ
	* @param [in] bExclude ��Ϊtrue��ʱ���ʾ��Ӱ�ھ��α߿����棬Ϊfalse��ʱ���ʾ��Ӱ�ڱ߿�����ڲ�
	*/
	virtual void DrawBoxShadow(const UiRect& rc, 
		                       const UiSize& roundSize, 
		                       const UiPoint& cpOffset, 
		                       int nBlurRadius, 
		                       int nSpreadRadius,
		                       UiColor dwColor, 
		                       bool bExclude) = 0;


	/** ����λͼ
	*@return ����λͼ�ӿڣ����غ��ɵ��÷�������Դ�������ͷ���Դ�ȣ�
	*/
	virtual IBitmap* DetachBitmap() = 0;

	virtual void ClearAlpha(const UiRect& rcDirty, int alpha = 0) = 0;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding, int alpha) = 0;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding = UiRect()) = 0;

#ifdef UILIB_IMPL_WINSDK
	/** ��ȡDC���������ʹ�ú���Ҫ����ReleaseDC�ӿ��ͷ���Դ
	*/
	virtual HDC GetDC() = 0;

	/** �ͷ�DC��Դ
	* @param [in] hdc ��Ҫ�ͷŵ�DC���
	*/
	virtual void ReleaseDC(HDC hdc) = 0;
#endif

public:

	/** ���λͼ���ݣ�ʹ����ȫ͸��λͼ��λͼ����ȫ�����㣩
	*/
	virtual void Clear() = 0;

	/** ��¡һ���µĶ���
	*/
	virtual std::unique_ptr<IRender> Clone() = 0;
};

class UILIB_API IRenderFactory
{
public:
	virtual ~IRenderFactory() = default;
	virtual IPen* CreatePen(UiColor color, int width = 1) = 0;
	virtual IBrush* CreateBrush(UiColor corlor) = 0;
	virtual IPath* CreatePath() = 0;
	virtual IBitmap* CreateBitmap() = 0;
	virtual IRender* CreateRender() = 0;
};

} // namespace ui

#endif // UI_RENDER_IRENDER_H_
