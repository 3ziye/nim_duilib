#ifndef UI_BOX_SCROLLBOX_H_
#define UI_BOX_SCROLLBOX_H_

#pragma once

#include "duilib/Box/HBox.h"
#include "duilib/Box/VBox.h"
#include "duilib/Box/TileBox.h"
#include "duilib/Core/ScrollBar.h"
#include "duilib/Animation/AnimationPlayer.h"

namespace ui 
{

 /** ���д�ֱ��ˮƽ��������������ʹ�����������ɸ�������
 *   ͨ���޸Ĳ��֣��γ� HScrollBox �� VScrollBox �� TileScrollBox��������
 */
class UILIB_API ScrollBox : public Box
{
public:
	explicit ScrollBox(Layout* pLayout = new Layout);
	ScrollBox(const ScrollBox& r) = delete;
	ScrollBox& operator=(const ScrollBox& r) = delete;

	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& pstrName, const std::wstring& pstrValue) override;
	virtual void SetPos(UiRect rc) override;
	virtual void HandleEvent(const EventArgs& event) override;
	virtual bool MouseEnter(const EventArgs& msg) override;
	virtual bool MouseLeave(const EventArgs& msg) override;
	virtual void PaintChild(IRender* pRender, const UiRect& rcPaint) override;
	virtual void SetMouseEnabled(bool bEnable = true) override;
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit) override;
	virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, UiPoint scrollPos = UiPoint()) override;
	virtual void ClearImageCache() override;

	/** ��ȡ����������ʵƫ����, ��32λ����ֵ��־ƫ�� (����ʹ�����������λ��)
	*   ���������ScrollVirtualOffset����ô��������Ὣ��������λ�ü�ȥ�������ƫ�ƣ�
	*   ���û������ScrollVirtualOffset����ô�������������UiSize64 GetScrollPos()��ͬ�Ľ�����������Ƿ�Խ�磻
	*   ����������ڵ�������֧�ִ������������VirtualScrollBox��������UiRectԽ�硣
	*/
	UiSize GetScrollOffset() const;

	/** ��ȡ������λ��(cx��������������λ�ã�cy���������������λ��)
	 */
	virtual UiSize64 GetScrollPos() const;

	/** ��ȡ�������ķ�Χ(cx�������������ķ�Χ��cy��������������ķ�Χ)
	 */
	virtual UiSize64 GetScrollRange() const;

	/** ���ù�����λ��
	 * @param[in] szPos Ҫ���õ�λ������(cx��������������λ�ã�cy���������������λ��)
	 */
	virtual void SetScrollPos(UiSize64 szPos);

	/** ���ù����� Y ������
	 * @param[in] y Ҫ���õ� Y ��������ֵ
	 */
	virtual void SetScrollPosY(int64_t y);
	
	/** ���ù����� X ������
	 * @param[in] x Ҫ���õ� X ��������ֵ
	 */
    virtual void SetScrollPosX(int64_t x);
	
	/** ���Ϲ���������
	 * @param[in] deltaValue �������룬Ĭ��Ϊ DUI_NOSET_VALUE
 	 * @param[in] withAnimation �Ƿ񸽴�����Ч����Ĭ��Ϊ true
	 */
	virtual void LineUp(int detaValue = DUI_NOSET_VALUE, bool withAnimation = true);
	
	/** ���¹���������
	 * @param[in] deltaValue �������룬Ĭ��Ϊ DUI_NOSET_VALUE
 	 * @param[in] withAnimation �Ƿ񸽴�����Ч����Ĭ��Ϊ true
	 */
	virtual void LineDown(int detaValue = DUI_NOSET_VALUE, bool withAnimation = true);
	
	/** �������������
	 * @param[in] deltaValue �������룬Ĭ��Ϊ DUI_NOSET_VALUE
	 */
    virtual void LineLeft(int detaValue = DUI_NOSET_VALUE);
	
	/** ���ҹ���������
	 * @param[in] deltaValue �������룬Ĭ��Ϊ DUI_NOSET_VALUE
	 */
    virtual void LineRight(int detaValue = DUI_NOSET_VALUE);
	
	/** ���Ϲ���һ��ҳ���С�ľ���
	 */
	virtual void PageUp();

	/** ���¹���һ��ҳ���С�ľ���
	 */
	virtual void PageDown();

	/** �ص����������Ϸ�
	 */
	virtual void HomeUp();

	/** ���������·�λ��
	 * @param[in] arrange �Ƿ����ù�����λ�ã�Ĭ��Ϊ true
	 * @param[in] withAnimation �Ƿ����������Ч��Ĭ��Ϊ true
	 */
	virtual void EndDown(bool arrange = true, bool withAnimation = true);

	/** �������һ��ҳ���С�ľ���
	 */
	virtual void PageLeft();

	/** ���ҹ���һ��ҳ���С�ľ���
	 */
	virtual void PageRight();

	/** �����������
	 */
	virtual void HomeLeft();

	/** ���������Ҳ�
	 */
	virtual void EndRight();

	/** �������Ϲ�������Ӧ WM_TOUCH ��Ϣ��
	 * @param[in] deltaValue ��������
	 */
	virtual void TouchUp(int deltaValue);

	/** �������¹�������Ӧ WM_TOUCH ��Ϣ��
	 * @param[in] deltaValue ��������
	 */
	virtual void TouchDown(int deltaValue);

	/** ���ù�����
	 * @param[in] bEnableVertical �Ƿ����ô�ֱ��������Ĭ��Ϊ true
	 * @param[in] bEnableHorizontal �Ƿ�����ˮƽ��������Ĭ��Ϊ true
	 */
	virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);

	/** ��ȡ��ֱ����������ָ��
	 */
	virtual ScrollBar* GetVScrollBar() const;

	/** ��ȡˮƽ����������ָ��
	 */
	virtual ScrollBar* GetHScrollBar() const;

	/** ���������������λ��
	 */
	virtual void ProcessVScrollBar(UiRect rc, int64_t cyRequired);

	/** ���ú����������λ��
	 */
	virtual void ProcessHScrollBar(UiRect rc, int64_t cxRequired);

	/** �жϴ�ֱ�������Ƿ���Ч
	 */
	bool IsVScrollBarValid() const;

	/** �ж�ˮƽ�������Ƿ���Ч
	 */
	bool IsHScrollBarValid() const;

	/** ���Ŷ���
	 */
	void PlayRenderOffsetYAnimation(int64_t nRenderY);

	/** �Ƿ��Ѿ��ڵײ�
	 */
	bool IsAtEnd() const;

	/** �Ƿ��������ײ�
	 */
	bool IsHoldEnd() const;

	/** ���ù������Ƿ�ʼ���������ײ�λ��
	 * @param[in] bHoldEnd ���� true ��ʾ������false Ϊ������
	 */
	void SetHoldEnd(bool bHoldEnd);
	
	/** ��ȡ��ֱ��������������
	 */
	int GetVerScrollUnitPixels() const;

	/** ���ô�ֱ��������������
	 * @param[in] nUnitPixels Ҫ���õĲ���
	 */
	void SetVerScrollUnitPixels(int nUnitPixels);

	/** ��ȡ�����������������
	*/
	int GetHorScrollUnitPixels() const;

	/** ���ú����������������
	* @param[in] nUnitPixels Ҫ���õĲ���
	*/
	void SetHorScrollUnitPixels(int nUnitPixels);

	/** ��ȡ�����Ĺ������Ƿ��������ӿؼ�����
	 * @return ���� true ��ʾ�����ڹ������ϣ�����Ϊ false
	 */
	bool GetScrollBarFloat() const;

	/** ���������Ĺ������Ƿ��������ӿؼ�����
	 * @param[in] bScrollBarFloat true ��ʾ�����ڹ������ϣ�false ��ʾ�������ڿؼ���
	 */
	void SetScrollBarFloat(bool bScrollBarFloat);

	/** ��ȡ�����Ĺ������Ƿ��������ʾ
	 * @return ���� true ��ʾ����࣬false Ϊ�Ҳ�
	 */
	bool GetVScrollBarLeftPos() const;

	/** ���������Ĺ������Ƿ��������ʾ
	* @param[in] bLeftPos true ��ʾ����࣬false Ϊ�Ҳ�
	*/
	void SetVScrollBarLeftPos(bool bLeftPos);

	/** ��ȡ����������߾�
	 */
	const UiPadding& GetScrollBarPadding() const;

	/** ���ù���������߾࣬�����ù�������ռ������
	 * @param[in] rcScrollBarPadding Ҫ���õı߾�
	 */
	void SetScrollBarPadding(UiPadding rcScrollBarPadding);

	/** ֹͣ����������
	*/
	void StopScrollAnimation();

	/** ����������λ�ñ仯�¼�
	 * @param[in] callback �б仯��֪ͨ�Ļص�����
	 */
	void AttachScrollChange(const EventCallback& callback) { AttachEvent(kEventScrollChange, callback); }

public:
	/** ��ȡ����������ƫ����(cx������������������ƫ������cy�������������������ƫ����)
	 */
	UiSize64 GetScrollVirtualOffset() const;

	/** ���ù���������ƫ����
	 * @param[in] szPos Ҫ���õ�λ������(cx��������������λ�ã�cy���������������λ��)
	 */
	void SetScrollVirtualOffset(UiSize64 szOffset);

	/** ���ù����� Y ������ƫ����
	 */
	void SetScrollVirtualOffsetY(int64_t yOffset);

	/** ���ù����� X ������ƫ����
	 */
	void SetScrollVirtualOffsetX(int64_t xOffset);

protected:

	/** ��������ĳߴ�
	 * @param[in] rc ��ǰλ����Ϣ, �ⲿ����ʱ������Ҫ��ȥ�ڱ߾�
	 * @return ��������ߴ��С
	 */
	virtual UiSize64 CalcRequiredSize(const UiRect& rc);

	/** ����ͼƬ���棬���� ScrollBox �ڲ�ʹ��
	 * @param[in] bFromTopLeft ��������
	 */
	virtual void LoadImageCache(bool bFromTopLeft);

private:
	/**@brief ����λ�ô�С
	 */
	void SetPosInternally(UiRect rc);

protected:
	//��ֱ�������ӿ�
	std::unique_ptr<ScrollBar> m_pVScrollBar;

	//ˮƽ�������ӿ�
	std::unique_ptr<ScrollBar> m_pHScrollBar;

	//������������ƫ����
	UiSize64 m_scrollVirtualOffset;

private:
	//��ֱ��������������
	int m_nVScrollUnitPixels;

	//��ֱ��������������(Ĭ��)
	const int m_nVScrollUnitPixelsDefault = 30;

	//ˮƽ��������������
    int m_nHScrollUnitPixels;

	//ˮƽ��������������(Ĭ��)
	int m_nHScrollUnitPixelsDefault = 30;

	// ��ֹSetPosѭ������
	bool m_bScrollProcess; 

	//�Ƿ��������ײ�
	bool m_bHoldEnd;

	//�����Ĺ������Ƿ��������ӿؼ�����
	bool m_bScrollBarFloat;

	//�����Ĺ������Ƿ��������ʾ
	bool m_bVScrollBarLeftPos;

	//����������߾�
	UiPadding m_rcScrollBarPadding;

	//����������Ч��֧��
	std::unique_ptr<AnimationPlayer> m_scrollAnimation;

	//����������Ч��֧��
	std::unique_ptr<AnimationPlayer> m_renderOffsetYAnimation;
};

/** ��Ƭ���ֵ�ScrollBox
*/
class UILIB_API TileScrollBox : public ScrollBox
{
public:
	TileScrollBox() :
		ScrollBox(new TileLayout)
	{
	}

	virtual std::wstring GetType() const override { return DUI_CTR_TILESCROLLBOX; }
};

/** ˮƽ���ֵ�ScrollBox
*/
class UILIB_API HScrollBox : public ScrollBox
{
public:
	HScrollBox() :
		ScrollBox(new HLayout)
	{
	}

	virtual std::wstring GetType() const override { return DUI_CTR_HSCROLLBOX; }
};

/** ��ֱ���ֵ�ScrollBox
*/
class UILIB_API VScrollBox : public ScrollBox
{
public:
	VScrollBox() :
		ScrollBox(new VLayout)
	{
	}

	virtual std::wstring GetType() const override { return DUI_CTR_VSCROLLBOX; }
};

} // namespace ui

#endif // UI_BOX_SCROLLBOX_H_
