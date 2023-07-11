#ifndef UI_CORE_SCROLLBOX_H_
#define UI_CORE_SCROLLBOX_H_

#pragma once

#include "duilib/Core/Box.h"
#include "duilib/Control/ScrollBar.h"
#include "duilib/Animation/AnimationPlayer.h"

namespace ui 
{

 /// ���д�ֱ��ˮƽ��������������ʹ�����������ɸ�������
class UILIB_API ScrollBox : public Box
{
public:
	explicit ScrollBox(Layout* pLayout);
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
	 * @brief ����ͼƬ���棬���� ScrollBox �ڲ�ʹ��
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

	//��ֱ��������������(Ĭ��)
	const int m_nVerScrollUnitPixelsDefault = 30;

	//ˮƽ��������������
    int m_nHerScrollUnitPixels;

	//ˮƽ��������������(Ĭ��)
	int m_nHerScrollUnitPixelsDefault = 30;

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

#endif // UI_CORE_SCROLLBOX_H_
