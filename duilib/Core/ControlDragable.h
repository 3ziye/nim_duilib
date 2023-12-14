#ifndef UI_CORE_CONTROL_DRAGABLE_H_
#define UI_CORE_CONTROL_DRAGABLE_H_

#pragma once

#include "duilib/Core/Control.h"
#include "duilib/Core/Box.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Box/VBox.h"
#include "duilib/Box/HBox.h"

namespace ui
{
/** ��Ҫ���ܣ�֧����ͬBox�ڵ��ӿؼ�ͨ���϶�������˳��
*/
template<typename T = Control>
class UILIB_API ControlDragableT: public T
{
public:
    ControlDragableT();
    virtual ~ControlDragableT();

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;	
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/** �����Ƿ�֧���϶��ı�ؼ���˳��
	*/
	void SetEnableDragOrder(bool bEnable);

	/** �ж��Ƿ�֧���϶��ı�ؼ���˳��
	*/
	virtual bool IsEnableDragOrder() const;

	/** �����϶�˳��ʱ���ؼ���͸����
	*/
	void SetDragAlpha(uint8_t nAlpha);

	/** ��ȡ�϶�˳��ʱ���ؼ���͸����
	*/
	uint8_t GetDragAlpha() const;

	/** ��ǰ�Ƿ�����ק������
	*/
	bool IsInDragging() const;

protected:
	/** @name �϶���صĳ�Ա����
	* @{ */

	virtual bool ButtonDown(const EventArgs& msg) override;
	virtual bool ButtonUp(const EventArgs& msg) override;
	virtual bool MouseMove(const EventArgs& msg) override;
	virtual bool OnWindowKillFocus(const EventArgs& msg) override;//�ؼ������Ĵ���ʧȥ����

protected:
	/** �ӿؼ���״̬
	*/
	struct ItemStatus
	{
		Control* m_pItem = nullptr;
		UiRect m_rcPos;
		uint8_t m_nPaintOrder = 0;
		size_t m_index = Box::InvalidIndex;
	};

	/** �������λ�õ��������ؼ���λ��(�϶�������һ����)
	* @param [in] pt ��ǰ����λ��
	* @param [in] ptMouseDown ��갴��ʱ��λ��
	* @param [in] rcItemList �ӿؼ����б�
	*/
	virtual void AdjustItemPos(const UiPoint& pt, const UiPoint& ptMouseDown,
							   const std::vector<ItemStatus>& rcItemList) const;

	/** ���������ؼ���λ�ã����˳�����
	* @param [in] pt ��ǰ����λ��
	* @param [in] rcItemList �ӿؼ����б�
	* @param [in] nOldItemIndex ԭ��������������
	* @param [in] nNewItemIndex ���µ�����������
	* @return �����˳�����������true�����򷵻�false
	*/
	virtual bool AdjustItemOrders(const UiPoint& pt,
								  const std::vector<ItemStatus>& rcItemList,
								  size_t& nOldItemIndex,
								  size_t& nNewItemIndex);

	/** �ؼ�λ���϶�����¼�
	* @param [in] nOldItemIndex ԭ��������������
	* @param [in] nNewItemIndex ���µ�����������
	*/
	virtual void OnItemOrdersChanged(size_t nOldItemIndex, size_t nNewItemIndex);

	/** ����϶�״̬���ָ�ԭ״̬
	*/
	void ClearDragStatus();

    /** ����ǰ���λ�ã�ת������������������Ч�ķ�Χ��
    */
    void GetValidPointInItemRects(UiPoint& pt) const;

	/** @} */

private:
	/** @name �϶���صĳ�Ա����
	* @{ */

	/** �Ƿ�֧���϶��ı��е�˳��
	*/
	bool m_bEnableDragOrder;

	/** �Ƿ�����������
	*/
	bool m_bMouseDown;

	/** �Ƿ�����ק������
	*/
	bool m_bInDragging;

	/** �϶�˳��ʱ���ؼ���͸����
	*/
	uint8_t m_nDragAlpha;

	/** ԭ����͸����
	*/
	uint8_t m_nOldAlpha;

	/** ��갴��ʱ�����λ��
	*/
	UiPoint m_ptMouseDown;

	/** ��갴��ʱ�Ŀؼ���������
	*/
	UiRect m_rcMouseDown;

	/** ��갴��ʱ���������У�ÿ���ؼ���λ��
	*/
	std::vector<ItemStatus> m_rcItemList;

	/** @} */
};

template<typename T>
ControlDragableT<T>::ControlDragableT():
    m_bEnableDragOrder(true),
    m_bMouseDown(false),
    m_bInDragging(false),
    m_nDragAlpha(216),
    m_nOldAlpha(255)
{
    
}

template<typename T>
ControlDragableT<T>::~ControlDragableT()
{
}

template<typename T>
inline std::wstring ControlDragableT<T>::GetType() const { return DUI_CTR_CONTROL_DRAGABLE; }

template<>
inline std::wstring ControlDragableT<Box>::GetType() const { return DUI_CTR_BOX_DRAGABLE; }

template<>
inline std::wstring ControlDragableT<HBox>::GetType() const { return DUI_CTR_HBOX_DRAGABLE; }

template<>
inline std::wstring ControlDragableT<VBox>::GetType() const { return DUI_CTR_VBOX_DRAGABLE; }

template<typename T>
void ControlDragableT<T>::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == L"drag_order") {
		SetEnableDragOrder(strValue == L"true");
	}
	else if (strName == L"drag_order") {
		SetDragAlpha((uint8_t)_wtoi(strValue.c_str()));
	}
	else {
		__super::SetAttribute(strName, strValue);
	}
}

template<typename T>
void ControlDragableT<T>::SetEnableDragOrder(bool bEnable)
{
    m_bEnableDragOrder = bEnable;
}

template<typename T>
bool ControlDragableT<T>::IsEnableDragOrder() const
{
    return m_bEnableDragOrder;
}

template<typename T>
void ControlDragableT<T>::SetDragAlpha(uint8_t nAlpha)
{
    m_nDragAlpha = nAlpha;
}

template<typename T>
uint8_t ControlDragableT<T>::GetDragAlpha() const
{
    return m_nDragAlpha;
}

template<typename T>
bool ControlDragableT<T>::IsInDragging() const
{
    return m_bInDragging;
}

template<typename T>
bool ControlDragableT<T>::ButtonDown(const EventArgs& msg)
{
    m_bMouseDown = false;
    bool bRet = __super::ButtonDown(msg);
    if (!IsEnableDragOrder()) {
        //��ǰ��Ϊ�̶��У����������˳��
        return bRet;
    }
    Box* pParent = this->GetParent();
    if (pParent == nullptr) {
        return bRet;
    }
    Layout* pLayout = pParent->GetLayout();
    if ((pLayout == nullptr) || (!pLayout->IsHLayout() && !pLayout->IsVLayout())) {
        return bRet;
    }
    UiPoint pt(msg.ptMouse);
    pt.Offset(this->GetScrollOffsetInScrollBox());

    m_bMouseDown = true;
    m_ptMouseDown = pt;
    m_rcMouseDown = this->GetRect();

    m_rcItemList.clear();
    size_t nItemCount = pParent->GetItemCount();
    for (size_t index = 0; index < nItemCount; ++index) {
        ItemStatus itemStatus;
        itemStatus.m_index = index;
        itemStatus.m_pItem = pParent->GetItemAt(index);
        if (itemStatus.m_pItem != nullptr) {
            itemStatus.m_rcPos = itemStatus.m_pItem->GetRect();
            itemStatus.m_nPaintOrder = itemStatus.m_pItem->GetPaintOrder();
            m_rcItemList.push_back(itemStatus);
        }
        if (itemStatus.m_pItem == this) {
            //��ǰ�ؼ�������˳����Ҫ����󣬱��ⱻ�����ؼ��ڵ�
            itemStatus.m_pItem->SetPaintOrder(255);
            ASSERT(itemStatus.m_rcPos.ContainsPt(m_ptMouseDown));
        }
    }
    return bRet;
}

template<typename T>
bool ControlDragableT<T>::MouseMove(const EventArgs& msg)
{
    bool bRet = __super::MouseMove(msg);
    if (!m_bMouseDown) {
        return bRet;
    }
    Box* pParent = this->GetParent();
    if (pParent == nullptr) {
        return bRet;
    }
    Layout* pLayout = pParent->GetLayout();
    if ((pLayout == nullptr) || (!pLayout->IsHLayout() && !pLayout->IsVLayout())) {
        return bRet;
    }

    UiPoint pt(msg.ptMouse);
    pt.Offset(this->GetScrollOffsetInScrollBox());
    GetValidPointInItemRects(pt);
    if (pLayout->IsHLayout()) {
        int32_t xOffset = pt.x - m_ptMouseDown.x;
        if (std::abs(xOffset) < GlobalManager::Instance().Dpi().GetScaleInt(3)) {
            return bRet;
        }
        //���������ؼ���λ��
        AdjustItemPos(pt, m_ptMouseDown, m_rcItemList);

        UiRect rect = m_rcMouseDown;
        rect.left += xOffset;
        rect.right += xOffset;
        this->SetPos(rect);
    }
    else {
        int32_t yOffset = pt.y - m_ptMouseDown.y;
        if (std::abs(yOffset) < GlobalManager::Instance().Dpi().GetScaleInt(3)) {
            return bRet;
        }
        //���������ؼ���λ��
        AdjustItemPos(pt, m_ptMouseDown, m_rcItemList);

        UiRect rect = m_rcMouseDown;
        rect.top += yOffset;
        rect.bottom += yOffset;
        this->SetPos(rect);
    }

    if (!m_bInDragging) {
        m_bInDragging = true;
        m_nOldAlpha = (uint8_t)this->GetAlpha();
        //����Ϊ��͸����Ч��
        this->SetAlpha(m_nDragAlpha);
    }
    return bRet;
}

template<typename T>
bool ControlDragableT<T>::ButtonUp(const EventArgs& msg)
{
    bool bRet = __super::ButtonUp(msg);
    if (!m_bInDragging) {
        //û�д����϶��ı���˳���״̬
        ClearDragStatus();
        return bRet;
    }
    UiPoint pt(msg.ptMouse);
    pt.Offset(this->GetScrollOffsetInScrollBox());
    GetValidPointInItemRects(pt);

    size_t nOldItemIndex = Box::InvalidIndex;
    size_t nNewItemIndex = Box::InvalidIndex;
    bool bOrderChanged = AdjustItemOrders(pt, m_rcItemList, nOldItemIndex, nNewItemIndex);
    ClearDragStatus();
    if (bOrderChanged) {
        //�����н����¼�
        OnItemOrdersChanged(nOldItemIndex, nNewItemIndex);
    }
    return bRet;
}

template<typename T>
void ControlDragableT<T>::GetValidPointInItemRects(UiPoint& pt) const
{
    Box* pParent = this->GetParent();
    if (pParent == nullptr) {
        return;
    }

    UiRect boxRect = pParent->GetRect();
    UiRect rcItemRects = boxRect;
    for (size_t nIndex = 0; nIndex < m_rcItemList.size(); ++nIndex) {
        const ItemStatus& itemStatus = m_rcItemList[nIndex];
        if ((itemStatus.m_pItem != nullptr) && !itemStatus.m_pItem->IsFloat()) {
            if (itemStatus.m_rcPos.left > rcItemRects.left) {
                rcItemRects.left = itemStatus.m_rcPos.left;
            }
            if (itemStatus.m_rcPos.top > rcItemRects.top) {
                rcItemRects.top = itemStatus.m_rcPos.top;
            }
            break;
        }
    }
    for (int32_t nIndex = (int32_t)m_rcItemList.size() - 1; nIndex >= 0; --nIndex) {
        const ItemStatus& itemStatus = m_rcItemList[nIndex];
        if ((itemStatus.m_pItem != nullptr) && !itemStatus.m_pItem->IsFloat()) {
            if (itemStatus.m_rcPos.right < rcItemRects.right) {
                rcItemRects.right = itemStatus.m_rcPos.right;
            }
            if (itemStatus.m_rcPos.bottom < rcItemRects.bottom) {
                rcItemRects.bottom = itemStatus.m_rcPos.bottom;
            }
            break;
        }
    }
    if (pt.x < rcItemRects.left) {
        pt.x = rcItemRects.left;
    }
    if (pt.x >= rcItemRects.right) {
        pt.x = rcItemRects.right - 1;
    }
    if (pt.y < rcItemRects.top) {
        pt.y = rcItemRects.top;
    }
    if (pt.y >= rcItemRects.bottom) {
        pt.y = rcItemRects.bottom - 1;
    }
}

template<typename T>
bool ControlDragableT<T>::OnWindowKillFocus(const EventArgs& msg)
{
    bool bRet = __super::OnWindowKillFocus(msg);
    ClearDragStatus();
    return bRet;
}

template<typename T>
void ControlDragableT<T>::AdjustItemPos(const UiPoint& pt, const UiPoint& ptMouseDown,
                                        const std::vector<ItemStatus>& rcItemList) const
{
    Control* pMouseItem = nullptr;
    size_t nMouseItemIndex = Box::InvalidIndex;
    size_t nMouseDownItemIndex = Box::InvalidIndex;
    for (const ItemStatus& itemStatus : rcItemList) {
        if (itemStatus.m_rcPos.ContainsPt(pt)) {
            pMouseItem = itemStatus.m_pItem;
            nMouseItemIndex = itemStatus.m_index;
        }
        if (itemStatus.m_pItem == this) {
            nMouseDownItemIndex = itemStatus.m_index;
        }
    }
    if ((pMouseItem == nullptr) ||
        (nMouseItemIndex == Box::InvalidIndex) ||
        (nMouseDownItemIndex == Box::InvalidIndex)) {
        return;
    }
    ControlDragableT<T>* pItem = dynamic_cast<ControlDragableT<T>*>(pMouseItem);
    if (pItem == nullptr) {
        //��겻�ڿ��϶��ؼ���
        return;
    }
    if (!pItem->IsEnableDragOrder()) {
        //��ǰ��Ϊ�̶������������˳��
        return;
    }

    const size_t itemCount = rcItemList.size();
    int32_t xOffset = pt.x - ptMouseDown.x;
    if (pMouseItem == this) {
        //��ǰ���λ�ã��������λ�ã��ָ������ؼ���ʵ��λ��
        for (const ItemStatus& item : rcItemList) {
            if (item.m_pItem == this) {
                continue;
            }
            if (item.m_pItem->GetRect() != item.m_rcPos) {
                item.m_pItem->SetPos(item.m_rcPos);
            }
        }
    }
    else if (xOffset < 0) {
        //��ǰ���λ�ã��ڰ��µ����࣬���Ҳ��ƶ��ؼ�
        for (size_t index = 0; index < itemCount; ++index) {
            const ItemStatus& item = rcItemList[index];
            if ((item.m_index >= nMouseItemIndex) && (item.m_index < nMouseDownItemIndex)) {
                //���Ҳ��ƶ�
                if ((index + 1) < itemCount) {
                    const ItemStatus& nextItem = rcItemList[index + 1];
                    item.m_pItem->SetPos(nextItem.m_rcPos);
                }
                else {
                    if (item.m_pItem->GetRect() != item.m_rcPos) {
                        item.m_pItem->SetPos(item.m_rcPos);
                    }
                }
            }
            else {
                //�ָ�ԭλ��
                if (item.m_pItem->GetRect() != item.m_rcPos) {
                    item.m_pItem->SetPos(item.m_rcPos);
                }
            }
        }
    }
    else {
        //��ǰ���λ�ã��ڰ��µ���Ҳ࣬������ƶ��ؼ�
        for (size_t index = 0; index < itemCount; ++index) {
            const ItemStatus& item = rcItemList[index];
            if ((item.m_index > nMouseDownItemIndex) && (item.m_index <= nMouseItemIndex)) {
                //������ƶ�
                if ((index - 1) < itemCount) {
                    const ItemStatus& nextItem = rcItemList[index - 1];
                    item.m_pItem->SetPos(nextItem.m_rcPos);
                }
                else {
                    if (item.m_pItem->GetRect() != item.m_rcPos) {
                        item.m_pItem->SetPos(item.m_rcPos);
                    }
                }
            }
            else {
                //�ָ�ԭλ��
                if (item.m_pItem->GetRect() != item.m_rcPos) {
                    item.m_pItem->SetPos(item.m_rcPos);
                }
            }
        }
    }
}

template<typename T>
bool ControlDragableT<T>::AdjustItemOrders(const UiPoint& pt,
                                           const std::vector<ItemStatus>& rcItemList,
                                           size_t& nOldItemIndex,
                                           size_t& nNewItemIndex)
{
    nOldItemIndex = Box::InvalidIndex;
    nNewItemIndex = Box::InvalidIndex;
    Box* pParent = this->GetParent();
    if (pParent == nullptr) {
        return false;
    }

    bool bOrderChanged = false;
    const size_t itemCount = pParent->GetItemCount();
    size_t nMouseItemIndex = Box::InvalidIndex;
    size_t nCurrentItemIndex = Box::InvalidIndex;
    for (const ItemStatus& itemStatus : rcItemList) {
        if (itemStatus.m_rcPos.ContainsPt(pt)) {
            nMouseItemIndex = itemStatus.m_index;
            ControlDragableT<T>* pItem = dynamic_cast<ControlDragableT<T>*>(itemStatus.m_pItem);
            if ((pItem != nullptr) && !pItem->IsEnableDragOrder()) {
                //��ǰ�ؼ�Ϊ�̶������������˳��
                nMouseItemIndex = Box::InvalidIndex;
            }
        }
        if ((itemStatus.m_pItem == this) && !itemStatus.m_rcPos.ContainsPt(pt)) {
            nCurrentItemIndex = itemStatus.m_index;
        }
    }
    if ((nMouseItemIndex < itemCount) && (nCurrentItemIndex < itemCount)) {
        //�����ؼ���λ��
        pParent->SetItemIndex(this, nMouseItemIndex);
        nOldItemIndex = nCurrentItemIndex;
        nNewItemIndex = nMouseItemIndex;
        bOrderChanged = true;
    }
    return bOrderChanged;
}

template<typename T>
void ControlDragableT<T>::OnItemOrdersChanged(size_t /*nOldItemIndex*/, size_t /*nNewItemIndex*/)
{
}

template<typename T>
void ControlDragableT<T>::ClearDragStatus()
{
    if (m_bInDragging) {
        this->SetAlpha(m_nOldAlpha);
        m_nOldAlpha = 255;
        m_bInDragging = false;
    }
    m_bMouseDown = false;
    if (!m_rcItemList.empty()) {
        //�ָ��ӿؼ��Ļ���˳��
        for (const ItemStatus& itemStatus : m_rcItemList) {
            if ((itemStatus.m_pItem != nullptr) &&
                (itemStatus.m_nPaintOrder != itemStatus.m_pItem->GetPaintOrder())) {
                itemStatus.m_pItem->SetPaintOrder(itemStatus.m_nPaintOrder);
            }
        }
        m_rcItemList.clear();
        //�Ը��ؼ��ػ�
        Control* pParent = this->GetParent();
        if (pParent != nullptr) {
            pParent->Invalidate();
            pParent->SetPos(pParent->GetPos());
        }
    }
}

typedef ControlDragableT<Control> ControlDragable;
typedef ControlDragableT<Box> BoxDragable;
typedef ControlDragableT<HBox> HBoxDragable;
typedef ControlDragableT<VBox> VBoxDragable;

}

#endif // UI_CORE_CONTROL_DRAGABLE_H_