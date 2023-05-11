#ifndef UI_CORE_BOX_H_
#define UI_CORE_BOX_H_

#pragma once

#include "duilib/duilib_defs.h"
#include "duilib/Core/Control.h"
#include "duilib/Control/ScrollBar.h"
#include <vector>
#include <memory>

namespace ui 
{
class Box;
class AnimationPlayer;

/** �ؼ�����
*/
class UILIB_API Layout
{
public:
	Layout();
	Layout(const Layout& r) = delete;
	Layout& operator=(const Layout& r) = delete;
	virtual ~Layout() = default;

	/**
	 * @brief ����������
	 * @return ��
	 */
	void SetOwner(Box* pOwner);

	/**
	 * @brief ���ø���״̬�µ�������Ϣ
	 * @param[in] pControl �ؼ����
	 * @param[in] rcContainer Ҫ���õ�λ����Ϣ
	 * @return ���ؿؼ����յ�λ����Ϣ
	 */
	static UiSize SetFloatPos(Control* pControl, UiRect rcContainer);

	/**
	 * @brief ���ò�������
	 * @param[in] strName Ҫ���õ�������
	 * @param[in] strValue Ҫ���õ�����ֵ
	 * @return true ���óɹ���false ���Բ�����
	 */
	virtual bool SetAttribute(const std::wstring& strName, const std::wstring& strValue);

	/**
	 * @brief �����ڲ����пؼ���λ����Ϣ
	 * @param[in] items �ؼ��б�
	 * @param[in] rc ��ǰ����λ����Ϣ
	 * @return �������к����պ��ӵĿ�Ⱥ͸߶���Ϣ
	 */
	virtual UiSize ArrangeChild(const std::vector<Control*>& items, UiRect rc);

	/**
	 * @brief �����ڲ��ӿؼ���С�������������С
	 * @param[in] items �ؼ��б�
	 * @param[in] szAvailable �ӿؼ�����������
	 * @return �������к����պ��ӵĿ�Ⱥ͸߶���Ϣ
	 */
	virtual UiSize AjustSizeByChild(const std::vector<Control*>& items, UiSize szAvailable);

	/**
	 * @brief ��ȡ�ڱ߾�
	 * @return �����ڱ߾��ıߵĴ�С
	 */
	const UiRect& GetPadding() const { return m_rcPadding; }

	/**
	 * @brief �����ڱ߾࣬�൱�����ÿͻ���
	 * @param[in] rcPadding �ڱ߾�����
	 * @param[in] bNeedDpiScale �Ƿ���� DPI ����Ӧ��Ĭ��Ϊ true
	 */
	void SetPadding(UiRect rcPadding, bool bNeedDpiScale);

	/**
	 * @brief ��ȡ�ӿؼ�֮��Ķ���߾�
	 * @return ���ض��������ֵ
	 */
	int GetChildMargin() const { return m_iChildMargin; }

	/**
	 * @brief �����ӿؼ�֮��Ķ���߾�
	 * @param[in] iMargin Ҫ���õı߾���ֵ
	 * @return ��
	 */
	void SetChildMargin(int iMargin);

	/**
	 * @brief ��ȡ�����ڱ߾���Ŀ��÷�Χ
	 * @return ���ؿ��÷�Χλ����Ϣ
	 */
	UiRect GetInternalPos() const;

protected:

	//�ڱ߾��ıߵĴ�С
	UiRect m_rcPadding;

	//�ӿؼ�֮��Ķ���߾�
	int m_iChildMargin;

	//����Box����
	Box *m_pOwner;
};


/////////////////////////////////////////////////////////////////////////////////////
//
class UILIB_API Box : public Control
{
public:
	explicit Box(Layout* pLayout = new Layout());
	Box(const Box& r) = delete;
	Box& operator=(const Box& r) = delete;
    virtual ~Box();

public:
	/// ��д����ӿڣ��ṩ���Ի����ܡ���������˵����鿴 Control �ؼ�             */
	virtual std::wstring GetType() const override;
#if defined(ENABLE_UIAUTOMATION)
	virtual UIAControlProvider* GetUIAProvider() override;
#endif
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void SetPos(UiRect rc) override;
	virtual void PaintChild(IRenderContext* pRender, const UiRect& rcPaint) override;
	virtual void SetEnabled(bool bEnabled) override;
	virtual void SetVisible(bool bVisible) override;
	virtual UiSize EstimateSize(UiSize szAvailable) override;
	virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, UiPoint scrollPos = UiPoint()) override;
	virtual void InvokeLoadImageCache() override;
	virtual void UnLoadImageCache() override;
	virtual void ClearImageCache() override;
	virtual UINT GetControlFlags() const override;

public:
	/** @name �����ӿؼ�(item)��صķ���
	* @{
	*/
	/**@brief ��ȡ�ӿؼ�����
	 */
	virtual int GetItemCount() const;

	/**
	 * @brief ������������ָ���ؼ�
	 * @param[in] iIndex �ؼ�����
	 * @return ���ؿؼ�ָ��
	 */
	virtual Control* GetItemAt(size_t iIndex) const;

	/**
	 * @brief ���ݿؼ�ָ���ȡ����
	 * @param[in] pControl �ؼ�ָ��
	 * @return ���� pControl ��ָ��Ŀؼ�����
	 */
	virtual int GetItemIndex(Control* pControl) const;

	/**
	 * @brief ���ÿؼ��������ڲ����������пؼ�λ�ã�
	 * @param[in] pControl �ؼ�ָ��
	 * @param[in] iIndex Ҫ���õ�����ֵ
	 * @return ���� true ���óɹ���false ����ʧ��
	 */
	virtual bool SetItemIndex(Control* pControl, size_t iIndex);

	/**
	 * @brief ���һ���ؼ���������
	 * @param[in] pControl �ؼ�ָ��
	 * @return ���� true ��ӳɹ���false Ϊ���ʧ��
	 */
	virtual bool AddItem(Control* pControl);

	/**
	 * @brief ��ָ��λ�����һ���ؼ�
	 * @param[in] pControl �ؼ�ָ��
	 * @param[in] iIndex �ڸ�����֮�����ؼ�
	 * @return ���� true Ϊ��ӳɹ���false Ϊ���ʧ��
	 */
	virtual bool AddItemAt(Control* pControl, size_t iIndex);

	/**
	 * @brief ���ݿؼ�ָ����������Ƴ�һ���ؼ�
	 * @param[in] pControl �ؼ���ָ��
	 * @return ���� true Ϊ�Ƴ��ɹ���false Ϊ�Ƴ�ʧ�ܣ��ؼ����ܲ����ڣ�
	 */
	virtual bool RemoveItem(Control* pControl);

	/**
	 * @brief ���ݿؼ��������������Ƴ�һ���ؼ�
	 * @param[in] iIndex Ҫ�Ƴ��Ŀؼ�����
	 * @return ���� true Ϊ�Ƴ��ɹ���false Ϊ�Ƴ�ʧ�ܣ�����ֵ̫С�򳬳����ӿؼ�������
	 */
	virtual bool RemoveItemAt(size_t iIndex);

	/**
	 * @brief �Ƴ������ӿؼ�
	 * @return ��
	 */
	virtual void RemoveAllItems();

	/** @} */

public:
	/**
	 * @brief ����ָ���ӿؼ�
	 * @param[in] pstrSubControlName �ӿؼ�����
	 * @return �����ӿؼ�ָ��
	 */
	Control* FindSubControl(const std::wstring& pstrSubControlName);

	/**
	 * @brief ������һ����ѡ�ؼ������������� list��combo��
	 * @param[in] iIndex ָ��Ҫ��ʼ���ҵ�����
	 * @param[in] bForward true Ϊ�������ң� false Ϊ�ݼ�����
	 * @return ��һ����ѡ�ؼ������������� -1 Ϊû�п�ѡ�ؼ�
	 */
	int FindSelectable(int iIndex, bool bForward = true) const;

	/**
	 * @brief �ж��Ƿ��Զ�����
	 * @return true Ϊ�Զ����٣�false Ϊ���Զ�����
	 */
    bool IsAutoDestroyChild() const { return m_bAutoDestroyChild; }

	/**
	 * @brief ���ÿؼ��Ƿ��Զ�����
	 * @param[in] bAuto true Ϊ�Զ����٣�false Ϊ���Զ�����
	 * @return ��
	 */
    void SetAutoDestroyChild(bool bAuto) { m_bAutoDestroyChild = bAuto; }

	/**
	 * @brief �жϴ��ڹرպ��Ƿ��Զ�����
	 * @return true Ϊ�Զ����٣�false Ϊ���Զ�����
	 */
    bool IsDelayedDestroy() const { return m_bDelayedDestroy; }

	/**
	 * @brief ���ô��ڹرպ��Ƿ��Զ�����
	 * @param[in] bDelayedDestroy true Ϊ�Զ����٣�false Ϊ���Զ�����
	 * @return ��
	 */
	void SetDelayedDestroy(bool bDelayedDestroy) { m_bDelayedDestroy = bDelayedDestroy; }

	/**
	 * @brief ��ȡ�����Ƿ���Ӧ������
	 * @return true Ϊ��Ӧ��false Ϊ����Ӧ
	 */
    bool IsMouseChildEnabled() const { return m_bMouseChildEnabled; }

	/**
	 * @brief ����������Ӧ������
	 * @param[in] bEnable ����Ϊ true Ϊ��Ӧ������������Ϊ false Ϊ����Ӧ��Ĭ��Ϊ true
	 */
	void SetMouseChildEnabled(bool bEnable) { m_bMouseChildEnabled = bEnable; }

	/**
	 * @brief ��ȡ�������ֶ���ָ��
	 * @return �������������Ĳ��ֶ���ָ��
	 */
	Layout* GetLayout() const { return m_pLayout.get(); }

	/**
	 * @brief ���¹������ֶ���
	 * @param[in] pLayout ���ֶ���ָ��
	 * @return ��
	 */
	void ReSetLayout(Layout* pLayout);

	/**
	 * @brief ��ȡ�ÿؼ�ȥ���ڱ߾���λ�ô�С��Ϣ
	 */
	UiRect GetPaddingPos() const;

private:
	/**@brief ��ָ��λ�����һ���ؼ�
	 * @param[in] pControl �ؼ�ָ��
	 * @param[in] iIndex �ڸ�����֮�����ؼ�
	 */
	bool DoAddItemAt(Control* pControl, size_t iIndex);

	/**@brief ���ݿؼ�ָ����������Ƴ�һ���ؼ�
	 * @param[in] pControl �ؼ���ָ��
	 */
	bool DoRemoveItem(Control* pControl);

protected:

	//�����е��ӿؼ��б�
	std::vector<Control*> m_items;

private:

	//�Ƿ��Զ�ɾ��item�Ķ������Ϊtrue���ڴ�m_items�Ƴ�Ԫ��ʱ����delete������������Ϊfalse����delete��
	bool m_bAutoDestroyChild;

	//�Ƿ��ӳ�ɾ��item�������Ϊtrue����Ԫ���Ƴ��󣬻�ŵ�Window�����У��ӳ�delete������󣬽���m_bAutoDestroyChildʱ��Ч
	bool m_bDelayedDestroy;
	
	//���ֹ���ӿ�
	std::unique_ptr<Layout> m_pLayout;

	//�Ƿ�������Ӧ�ӿؼ��������Ϣ
	bool m_bMouseChildEnabled;
};

 /// ���д�ֱ��ˮƽ��������������ʹ�����������ɸ�������
class UILIB_API ScrollableBox : public Box
{
public:
	explicit ScrollableBox(Layout* pLayout);
	ScrollableBox(const ScrollableBox& r) = delete;
	ScrollableBox& operator=(const ScrollableBox& r) = delete;

	virtual std::wstring GetType() const override;
#if defined(ENABLE_UIAUTOMATION)
	virtual UIAControlProvider* GetUIAProvider() override;
#endif
	virtual void SetAttribute(const std::wstring& pstrName, const std::wstring& pstrValue) override;
	virtual void SetPos(UiRect rc) override;
	virtual void HandleEvent(const EventArgs& event) override;
	virtual bool MouseEnter(const EventArgs& msg) override;
	virtual bool MouseLeave(const EventArgs& msg) override;
	virtual void PaintChild(IRenderContext* pRender, const UiRect& rcPaint) override;
	virtual void SetMouseEnabled(bool bEnable = true) override;
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit) override;
	virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, UiPoint scrollPos = UiPoint()) override;
	virtual void ClearImageCache() override;

	/**
	 * @brief ��ȡ������λ��
	 * @return ���ع�������λ����Ϣ
	 */
	virtual UiSize GetScrollPos() const;

	/**
	 * @brief ��ȡ�������ķ�Χ
	 * @return ���ع������ķ�Χ��Ϣ
	 */
	virtual UiSize GetScrollRange() const;

	/**
	 * @brief ���ù�����λ��
	 * @param[in] szPos Ҫ���õ�λ������
	 * @return ��
	 */
	virtual void SetScrollPos(UiSize szPos);

	/**
	 * @brief ���ù����� Y ������
	 * @param[in] y Ҫ���õ� Y ��������ֵ
	 * @return ��
	 */
	virtual void SetScrollPosY(int y);
	
	/**
	 * @brief ���ù����� X ������
	 * @param[in] x Ҫ���õ� X ��������ֵ
	 * @return ��
	 */
    virtual void SetScrollPosX(int x);
	
	/**
	 * @brief ���Ϲ���������
	 * @param[in] deltaValue �������룬Ĭ��Ϊ DUI_NOSET_VALUE
 	 * @param[in] withAnimation �Ƿ񸽴�����Ч����Ĭ��Ϊ true
	 * @return ��
	 */
	virtual void LineUp(int detaValue = DUI_NOSET_VALUE, bool withAnimation = true);
	
	/**
	 * @brief ���¹���������
	 * @param[in] deltaValue �������룬Ĭ��Ϊ DUI_NOSET_VALUE
 	 * @param[in] withAnimation �Ƿ񸽴�����Ч����Ĭ��Ϊ true
	 * @return ��
	 */
	virtual void LineDown(int detaValue = DUI_NOSET_VALUE, bool withAnimation = true);
	
	/**
	 * @brief �������������
	 * @param[in] deltaValue �������룬Ĭ��Ϊ DUI_NOSET_VALUE
	 * @return ��
	 */
    virtual void LineLeft(int detaValue = DUI_NOSET_VALUE);
	
	/**
	 * @brief ���ҹ���������
	 * @param[in] deltaValue �������룬Ĭ��Ϊ DUI_NOSET_VALUE
	 * @return ��
	 */
    virtual void LineRight(int detaValue = DUI_NOSET_VALUE);
	/**
	 * @brief ���Ϲ���һ��ҳ���С�ľ���
	 * @return ��
	 */
	virtual void PageUp();

	/**
	 * @brief ���¹���һ��ҳ���С�ľ���
	 * @return ��
	 */
	virtual void PageDown();

	/**
	 * @brief �ص����������Ϸ�
	 * @return ��
	 */
	virtual void HomeUp();

	/**
	 * @brief ���������·�λ��
	 * @param[in] arrange �Ƿ����ù�����λ�ã�Ĭ��Ϊ true
	 * @param[in] withAnimation �Ƿ����������Ч��Ĭ��Ϊ true
	 * @return ��
	 */
	virtual void EndDown(bool arrange = true, bool withAnimation = true);
	/**
	 * @brief �������һ��ҳ���С�ľ���
	 * @return ��
	 */
	virtual void PageLeft();

	/**
	 * @brief ���ҹ���һ��ҳ���С�ľ���
	 * @return ��
	 */
	virtual void PageRight();

	/**
	 * @brief �����������
	 * @return ��
	 */
	virtual void HomeLeft();

	/**
	 * @brief ���������Ҳ�
	 * @return ��
	 */
	virtual void EndRight();

	/**
	 * @brief �������Ϲ�������Ӧ WM_TOUCH ��Ϣ��
	 * @param[in] deltaValue ��������
	 * @return ��
	 */
	virtual void TouchUp(int deltaValue);

	/**
	 * @brief �������¹�������Ӧ WM_TOUCH ��Ϣ��
	 * @param[in] deltaValue ��������
	 * @return ��
	 */
	virtual void TouchDown(int deltaValue);

	/**
	 * @brief ���ù�����
	 * @param[in] bEnableVertical �Ƿ����ô�ֱ��������Ĭ��Ϊ true
	 * @param[in] bEnableHorizontal �Ƿ�����ˮƽ��������Ĭ��Ϊ true
	 * @return ��
	 */
	virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);

	/**
	 * @brief ��ȡ��ֱ����������ָ��
	 * @return ���ش�ֱ����������ָ��
	 */
	virtual ScrollBar* GetVerticalScrollBar() const;

	/**
	 * @brief ��ȡˮƽ����������ָ��
	 * @return ����ˮƽ����������ָ��
	 */
	virtual ScrollBar* GetHorizontalScrollBar() const;

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	virtual void ProcessVScrollBar(UiRect rc, int cyRequired);

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	virtual void ProcessHScrollBar(UiRect rc, int cxRequired);

	/**
	 * @brief �жϴ�ֱ�������Ƿ���Ч
	 * @return ���� true ��ʾ��Ч������ false Ϊ��Ч
	 */
	bool IsVScrollBarValid() const;

	/**
	 * @brief �ж�ˮƽ�������Ƿ���Ч
	 * @return ���� true ��ʾ��Ч������ false Ϊ��Ч
	 */
	bool IsHScrollBarValid() const;

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	void ReomveLastItemAnimation();

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	void PlayRenderOffsetYAnimation(int nRenderY);

	/**
	 * @brief �Ƿ��Ѿ��ڵײ�
	 * @return ���� true ��ʾ�Ѿ��ڵײ�������Ϊ false
	 */
	bool IsAtEnd() const;

	/**
	 * @brief �Ƿ��������ײ�
	 * @return �� true ��ʾ�����ڵײ�������Ϊ false
	 */
	bool IsHoldEnd() const;

	/**
	 * @brief ���ù������Ƿ�ʼ���������ײ�λ��
	 * @param[in] bHoldEnd ���� true ��ʾ������false Ϊ������
	 * @return ��
	 */
	void SetHoldEnd(bool bHoldEnd);
	
	/**
	 * @brief ��ȡ��ֱ��������������
	 * @return ���ع�������
	 */
	int GetVerScrollUnitPixels() const;

	/**
	 * @brief ���ô�ֱ��������������
	 * @param[in] nUnitPixels Ҫ���õĲ���
	 * @return ��
	 */
	void SetVerScrollUnitPixels(int nUnitPixels);

	/**
	* @brief ��ȡ�����������������
	* @return ���ع�������
	*/
	int GetHorScrollUnitPixels() const;

	/**
	* @brief ���ú����������������
	* @param[in] nUnitPixels Ҫ���õĲ���
	* @return ��
	*/
	void SetHorScrollUnitPixels(int nUnitPixels);

	/**
	 * @brief ��ȡ�����Ĺ������Ƿ��������ӿؼ�����
	 * @return ���� true ��ʾ�����ڹ������ϣ�����Ϊ false
	 */
	bool GetScrollBarFloat() const;

	/**
	 * @brief ���������Ĺ������Ƿ��������ӿؼ�����
	 * @param[in] bScrollBarFloat true ��ʾ�����ڹ������ϣ�false ��ʾ�������ڿؼ���
	 * @return ��
	 */
	void SetScrollBarFloat(bool bScrollBarFloat);

	/**
	 * @brief ��ȡ�����Ĺ������Ƿ��������ʾ
	 * @return ���� true ��ʾ����࣬false Ϊ�Ҳ�
	 */
	bool GetVScrollBarLeftPos() const;

	/**
	* @brief ���������Ĺ������Ƿ��������ʾ
	* @param[in] bLeftPos true ��ʾ����࣬false Ϊ�Ҳ�
	* @return ��
	*/
	void SetVScrollBarLeftPos(bool bLeftPos);

	/**
	 * @brief ��ȡ����������߾�
	 * @return ���ر߾���Ϣ 
	 */
	UiRect GetScrollBarPadding() const;

	/**
	 * @brief ���ù���������߾࣬�����ù�������ռ������
	 * @param[in] rcScrollBarPadding Ҫ���õı߾�
	 * @return ��
	 */
	void SetScrollBarPadding(UiRect rcScrollBarPadding);

	/**
	 * @brief ����������λ�ñ仯�¼�
	 * @param[in] callback �б仯��֪ͨ�Ļص�����
	 * @return ��
	 */
	void AttachScrollChange(const EventCallback& callback) { AttachEvent(kEventScrollChange, callback); }

protected:

	/**
	 * @brief ��������ĳߴ�
	 * @param[in] rc ��ǰλ����Ϣ
	 * @return ��������ߴ��С
	 */
	virtual UiSize CalcRequiredSize(const UiRect& rc);

	/**
	 * @brief ����ͼƬ���棬���� ScrollableBox �ڲ�ʹ��
	 * @param[in] bFromTopLeft ��������
	 * @return ��
	 */
	virtual void LoadImageCache(bool bFromTopLeft);

private:
	/**@brief ����λ�ô�С
	 */
	void SetPosInternally(UiRect rc);

protected:
	//��ֱ�������ӿ�
	std::unique_ptr<ScrollBar> m_pVerticalScrollBar;

	//ˮƽ�������ӿ�
	std::unique_ptr<ScrollBar> m_pHorizontalScrollBar;

	//��ֱ��������������
	int m_nVerScrollUnitPixels;

	//ˮƽ��������������
    int m_nHerScrollUnitPixels;

	// ��ֹSetPosѭ������
	bool m_bScrollProcess; 

	//�Ƿ��������ײ�
	bool m_bHoldEnd;

	//�����Ĺ������Ƿ��������ӿؼ�����
	bool m_bScrollBarFloat;

	//�����Ĺ������Ƿ��������ʾ
	bool m_bVScrollBarLeftPos;

	//����������߾�
	UiRect m_rcScrollBarPadding;

	//����������Ч��֧��
	std::unique_ptr<AnimationPlayer> m_scrollAnimation;

	//����������Ч��֧��
	std::unique_ptr<AnimationPlayer> m_renderOffsetYAnimation;
};

} // namespace ui

#endif // UI_CORE_BOX_H_
