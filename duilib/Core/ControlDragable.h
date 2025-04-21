#ifndef UI_CORE_CONTROL_DRAGABLE_H_
#define UI_CORE_CONTROL_DRAGABLE_H_

#include "duilib/Core/Control.h"
#include "duilib/Core/Box.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Core/DragWindow.h"
#include "duilib/Box/VBox.h"
#include "duilib/Box/HBox.h"
#include "duilib/Box/ScrollBox.h"
#include "duilib/Utils/StringUtil.h"
#include "duilib/Core/WindowCreateParam.h"

#ifdef DUILIB_BUILD_FOR_SDL
    #include "duilib/Core/DragWindowFilter_SDL.h"
#endif

//拖动操作，最短的像素距离
#define DRAG_OPERATION_MIN_PT 5

namespace ui
{
/** 主要功能：支持相同Box内的子控件通过拖动来调整顺序
*/
template<typename T = Control>
class UILIB_API ControlDragableT: public T
{
    typedef T BaseClass;
public:
    explicit ControlDragableT(Window* pWindow);
    virtual ~ControlDragableT() override;

    /// 重写父类方法，提供个性化功能，请参考父类声明
    virtual DString GetType() const override;    
    virtual void SetAttribute(const DString& strName, const DString& strValue) override;

    /** 设置是否支持拖动改变控件的顺序
    */
    void SetEnableDragOrder(bool bEnable);

    /** 判断是否支持拖动改变控件的顺序
    */
    virtual bool IsEnableDragOrder() const;

    /** 设置拖动顺序时，控件的透明度
    */
    void SetDragAlpha(uint8_t nAlpha);

    /** 获取拖动顺序时，控件的透明度
    */
    uint8_t GetDragAlpha() const;

    /** 设置是否支持拖出操作
    */
    void SetEnableDragOut(bool bEnable);

    /** 判断是否支持拖出操作
    */
    virtual bool IsEnableDragOut() const;

    /** 当前是否处于拖拽调序操作中
    */
    bool IsInDraggingOrder() const;

    /** 当前是否处于拖出操作中
    */
    bool IsInDraggingOut() const;

    /** 取消拖动操作
    */
    void CancelDragOperation();

protected:
    /** @name 拖动相关的成员函数
    * @{ */

    virtual bool ButtonDown(const EventArgs& msg) override;
    virtual bool ButtonUp(const EventArgs& msg) override;
    virtual bool MouseMove(const EventArgs& msg) override;
    virtual bool OnWindowKillFocus(const EventArgs& msg) override;//控件所属的窗口失去焦点
    virtual void HandleEvent(const EventArgs& msg) override;

protected:
    /** 子控件的状态
    */
    struct ItemStatus
    {
        Control* m_pItem = nullptr;
        UiRect m_rcPos;
        uint8_t m_nPaintOrder = 0;
        size_t m_index = Box::InvalidIndex;
    };

    /** 根据鼠标位置调整各个控件的位置(拖动操作的一部分)
    * @param [in] pt 当前鼠标的位置
    * @param [in] ptMouseDown 鼠标按下时的位置
    * @param [in] rcItemList 子控件的列表
    */
    virtual void AdjustItemPos(const UiPoint& pt, const UiPoint& ptMouseDown,
                               const std::vector<ItemStatus>& rcItemList) const;

    /** 交换两个控件的位置，完成顺序调整
    * @param [in] pt 当前鼠标的位置
    * @param [in] rcItemList 子控件的列表
    * @param [in] nOldItemIndex 原来的子项索引号
    * @param [in] nNewItemIndex 最新的子项索引号
    * @return 如果有顺序调整，返回true；否则返回false
    */
    virtual bool AdjustItemOrders(const UiPoint& pt,
                                  const std::vector<ItemStatus>& rcItemList,
                                  size_t& nOldItemIndex,
                                  size_t& nNewItemIndex);

    /** 控件位置拖动完成事件（在同一个容器内）
    * @param [in] nOldItemIndex 原来的子项索引号
    * @param [in] nNewItemIndex 最新的子项索引号
    */
    virtual void OnItemOrdersChanged(size_t nOldItemIndex, size_t nNewItemIndex);

    /** 清除拖动状态，恢复原状态
    */
    virtual void ClearDragStatus();

    /** 获取子项所在区域的有效的范围
    */
    virtual bool GetItemsValidRect(UiRect& itemsValidRect) const;

    /** @} */

protected:
    /** @name 在相同窗口内不同的Box内拖动相关的成员变量
    * @{ */

    /** 拖拽拖放事件
    * @param [in] ptMouse 当前鼠标所在位置
    * @return 返回true表示匹配到拖入的控件，返回false表示未找到拖入的控件位置
    */
    virtual bool OnDragOutMouseOver(Box* pTargetBox, Box* pOldTargetBox, const UiPoint& ptMouse);

    /** 创建拖出操作时，显示的跟随窗口
    */
    virtual DragWindow* CreateDragWindow();

    /** 目标位置指示控件
    * @param [in] pTargetBox 即将拖入的目标Box控件接口
    */
    virtual Control* CreateDestControl(Box* pTargetBox);

    /** 生成控件拖出时的位图
    */
    virtual std::shared_ptr<IBitmap> CreateDragoutImage();

    /** 判断鼠标位置是否在控件的矩形范围内
    */
    virtual bool IsPtInControlRect(Control* pControl, const UiPoint& pt) const;

    /** 控件所属容器拖动完成事件（在同一个容器/或者不同容器内）
    * @param [in] pOldBox 控件原来所属的容器接口
    * @param [in] nOldItemIndex 原来的子项索引号
    * @param [in] pNewBox 控件最新所属的容器接口
    * @param [in] nNewItemIndex 最新的子项索引号
    */
    virtual void OnItemBoxChanged(Box* pOldBox, size_t nOldItemIndex, 
                                  Box* pNewBox, size_t nNewItemIndex);

    /** @} */

private:
    /** 鼠标移动时执行的拖出操作
    * @return 返回true表示处于拖出操作中，返回false表示未执行拖出操作
    */
    bool DragOutMouseMove(const EventArgs& msg);

    /** 鼠标弹起时执行的拖出操作
    * @return 返回true表示执行了拖出操作，返回false表示未执行拖出操作
    */
    bool DragOutMouseUp(const EventArgs& msg);

    /** 鼠标按下时执行的拖动调整顺序操作
    */
    void DragOrderMouseDown(const EventArgs& msg);

    /** 鼠标移动时执行的拖动调整顺序操作
    * @return 返回true表示处于拖动调序操作中，返回false表示未执行拖动调序操作
    */
    bool DragOrderMouseMove(const EventArgs& msg);

    /** 鼠标弹起时执行的拖出操作
    */
    void DragOrderMouseUp(const EventArgs& msg);

private:
    /** @name 在相同窗口内同一个Box内拖动相关的成员变量
    * @{ */

    /** 是否支持拖动改变列的顺序(功能开关)
    */
    bool m_bEnableDragOrder;

    /** 是否鼠标左键按下
    */
    bool m_bMouseDown;

    /** 是否处于拖拽调序操作中
    */
    bool m_bInDraggingOrder;

    /** 拖动顺序时，控件的透明度
    */
    uint8_t m_nDragAlpha;

    /** 原来的透明度
    */
    uint8_t m_nOldAlpha;

    /** 鼠标按下时的鼠标位置
    */
    UiPoint m_ptMouseDown;

    /** 鼠标按下时的控件矩形区域
    */
    UiRect m_rcMouseDown;

    /** 鼠标按下时，父容器中，每个控件的位置
    */
    std::vector<ItemStatus> m_rcItemList;

    /** @} */

    /** @name 在相同窗口内不同的Box内拖动相关的成员变量
    * @{ */

    /** 是否支持拖出操作(功能开关)
    */
    bool m_bEnableDragOut;

    /** 目标Box
    */
    Box* m_pTargetBox;

    /** 目标位置指示控件
    */
    Control* m_pDestControl;

    /** 拖拽的状态窗口
    */
    DragWindow* m_pDragWindow;

#ifdef DUILIB_BUILD_FOR_SDL
    /** 拖拽窗口的消息过滤器
    */
    std::unique_ptr<IUIMessageFilter> m_pDragWindowFilter;
#endif

    /** 该拖出控件的位图
    */
    std::shared_ptr<IBitmap> m_pDragImage;

    /** 当前是否正在执行拖出操作
    */
    bool m_bDraggingOut;

    /** @} */
};

template<typename T>
ControlDragableT<T>::ControlDragableT(Window* pWindow):
    T(pWindow),
    m_bEnableDragOrder(true),
    m_bEnableDragOut(true),
    m_bInDraggingOrder(false),
    m_bMouseDown(false),
    m_nDragAlpha(216),
    m_nOldAlpha(255),    
    m_pTargetBox(nullptr),
    m_pDestControl(nullptr),
    m_pDragWindow(nullptr),
    m_bDraggingOut(false)
{
}

template<typename T>
ControlDragableT<T>::~ControlDragableT()
{
}

template<typename T>
inline DString ControlDragableT<T>::GetType() const { return DUI_CTR_CONTROL_DRAGABLE; }

template<>
inline DString ControlDragableT<Box>::GetType() const { return DUI_CTR_BOX_DRAGABLE; }

template<>
inline DString ControlDragableT<HBox>::GetType() const { return DUI_CTR_HBOX_DRAGABLE; }

template<>
inline DString ControlDragableT<VBox>::GetType() const { return DUI_CTR_VBOX_DRAGABLE; }

template<typename T>
void ControlDragableT<T>::SetAttribute(const DString& strName, const DString& strValue)
{
    if (strName == _T("drag_order")) {
        //是否支持拖动调整顺序（在同一个容器内）
        SetEnableDragOrder(strValue == _T("true"));
    }
    else if (strName == _T("drag_alpha")) {
        SetDragAlpha((uint8_t)StringUtil::StringToInt32(strValue));
    }
    else if (strName == _T("drag_out")) {
        //是否支持拖出操作（在相同窗口的不同容器内）
        SetEnableDragOut(strValue == _T("true"));
    }
    else {
        BaseClass::SetAttribute(strName, strValue);
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
void ControlDragableT<T>::SetEnableDragOut(bool bEnable)
{
    m_bEnableDragOut = bEnable;
}

template<typename T>
bool ControlDragableT<T>::IsEnableDragOut() const
{
    return m_bEnableDragOut;
}

template<typename T>
bool ControlDragableT<T>::IsInDraggingOrder() const
{
    return m_bInDraggingOrder;
}

template<typename T>
bool ControlDragableT<T>::IsInDraggingOut() const
{
    return m_bDraggingOut;
}

template<typename T>
void ControlDragableT<T>::CancelDragOperation()
{
    ClearDragStatus();
}

template<typename T>
bool ControlDragableT<T>::ButtonDown(const EventArgs& msg)
{
    m_bMouseDown = false;
    bool bRet = BaseClass::ButtonDown(msg);
    if (msg.IsSenderExpired()) {
        return false;
    }
    if (!IsEnableDragOrder() && !IsEnableDragOut()) {
        return bRet;
    }
    const Box* pParent = this->GetParent();
    if (pParent == nullptr) {
        return bRet;
    }
    const Layout* pLayout = pParent->GetLayout();
    if ((pLayout == nullptr) || (!pLayout->IsHLayout() && !pLayout->IsVLayout())) {
        return bRet;
    }
    UiPoint pt(msg.ptMouse);
    pt.Offset(this->GetScrollOffsetInScrollBox());

    m_bMouseDown = true;
    m_ptMouseDown = pt;
    m_rcMouseDown = this->GetRect();

    DragOrderMouseDown(msg); 
    return bRet;
}

template<typename T>
bool ControlDragableT<T>::MouseMove(const EventArgs& msg)
{
    bool bRet = BaseClass::MouseMove(msg);
    if (m_bMouseDown) {
        m_bDraggingOut = DragOutMouseMove(msg);
        if (!m_bDraggingOut) {
            DragOrderMouseMove(msg);
        }
    }
    return bRet;
}

template<typename T>
bool ControlDragableT<T>::ButtonUp(const EventArgs& msg)
{
    bool bRet = BaseClass::ButtonUp(msg);
    if (msg.IsSenderExpired()) {
        return false;
    }
    if (!DragOutMouseUp(msg)) {
        DragOrderMouseUp(msg);
    }
    ClearDragStatus();
    return bRet;
}

template<typename T>
bool ControlDragableT<T>::OnWindowKillFocus(const EventArgs& msg)
{
    bool bRet = BaseClass::OnWindowKillFocus(msg);
    ClearDragStatus();
    return bRet;
}

template<typename T>
void ControlDragableT<T>::HandleEvent(const EventArgs& msg)
{
    BaseClass::HandleEvent(msg);
    if ((msg.eventType == kEventMouseClickChanged) ||
        (msg.eventType == kEventMouseClickEsc) ||
        (msg.eventType == kEventMouseRButtonDown)) {
        //取消拖动调序或者拖出操作
        ClearDragStatus();
    }
}

template<typename T>
bool ControlDragableT<T>::GetItemsValidRect(UiRect& itemsValidRect) const
{
    const Box* pParent = this->GetParent();
    ASSERT(pParent != nullptr);
    if (pParent == nullptr) {
        return false;
    }

    UiRect boxRect = pParent->GetRect();//父容器所在的区域
    UiPoint offsetParent = pParent->GetScrollOffsetInScrollBox();
    boxRect.Offset(-offsetParent.x, -offsetParent.y);//转换为窗口的客户区坐标
    UiRect rcItemRects = boxRect;//子项所占的总区域
    for (size_t nIndex = 0; nIndex < m_rcItemList.size(); ++nIndex) {
        const ItemStatus& itemStatus = m_rcItemList[nIndex];
        if ((itemStatus.m_pItem != nullptr) && !itemStatus.m_pItem->IsFloat()) {
            UiRect rcPos = itemStatus.m_rcPos;
            UiPoint offsetItem = itemStatus.m_pItem->GetScrollOffsetInScrollBox();
            rcPos.Offset(-offsetItem.x, -offsetItem.y);//转换为窗口的客户区坐标
            if (rcPos.left > rcItemRects.left) {
                rcItemRects.left = rcPos.left;
            }
            if (rcPos.top > rcItemRects.top) {
                rcItemRects.top = rcPos.top;
            }
            break;
        }
    }
    for (int32_t nIndex = (int32_t)m_rcItemList.size() - 1; nIndex >= 0; --nIndex) {
        const ItemStatus& itemStatus = m_rcItemList[nIndex];
        if ((itemStatus.m_pItem != nullptr) && !itemStatus.m_pItem->IsFloat()) {
            UiRect rcPos = itemStatus.m_rcPos;
            UiPoint offsetItem = itemStatus.m_pItem->GetScrollOffsetInScrollBox();
            rcPos.Offset(-offsetItem.x, -offsetItem.y);//转换为窗口的客户区坐标
            if (rcPos.right < rcItemRects.right) {
                rcItemRects.right = rcPos.right;
            }
            if (rcPos.bottom < rcItemRects.bottom) {
                rcItemRects.bottom = rcPos.bottom;
            }
            break;
        }
    }

    UiPoint offset = this->GetScrollOffsetInScrollBox();
    rcItemRects.Offset(offset);//转换为控件位置坐标
    itemsValidRect = rcItemRects;
    return true;
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
        //鼠标不在可拖动控件上
        return;
    }
    if (!pItem->IsEnableDragOrder()) {
        //当前列为固定，不允许调整顺序
        return;
    }

    const size_t itemCount = rcItemList.size();
    int32_t xOffset = pt.x - ptMouseDown.x;
    if (pMouseItem == this) {
        //当前鼠标位置：在自身的位置，恢复各个控件的实际位置
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
        //当前鼠标位置：在按下点的左侧，向右侧移动控件
        for (size_t index = 0; index < itemCount; ++index) {
            const ItemStatus& item = rcItemList[index];
            if ((item.m_index >= nMouseItemIndex) && (item.m_index < nMouseDownItemIndex)) {
                //向右侧移动
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
                //恢复原位置
                if (item.m_pItem->GetRect() != item.m_rcPos) {
                    item.m_pItem->SetPos(item.m_rcPos);
                }
            }
        }
    }
    else {
        //当前鼠标位置：在按下点的右侧，向左侧移动控件
        for (size_t index = 0; index < itemCount; ++index) {
            const ItemStatus& item = rcItemList[index];
            if ((item.m_index > nMouseDownItemIndex) && (item.m_index <= nMouseItemIndex)) {
                //向左侧移动
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
                //恢复原位置
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
            const ControlDragableT<T>* pItem = dynamic_cast<const ControlDragableT<T>*>(itemStatus.m_pItem);
            if ((pItem != nullptr) && !pItem->IsEnableDragOrder()) {
                //当前控件为固定，不允许调整顺序
                nMouseItemIndex = Box::InvalidIndex;
            }
        }
        if ((itemStatus.m_pItem == this) && !itemStatus.m_rcPos.ContainsPt(pt)) {
            nCurrentItemIndex = itemStatus.m_index;
        }
    }
    if ((nMouseItemIndex < itemCount) && (nCurrentItemIndex < itemCount)) {
        //调整控件的位置
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
    //恢复拖出操作的状态
    if ((m_pDestControl != nullptr) && (m_pTargetBox != nullptr)) {
        if (m_pTargetBox->RemoveItem(m_pDestControl)) {
            m_pDestControl = nullptr;
        }
    }
    if (m_pDragWindow != nullptr) {
#ifdef DUILIB_BUILD_FOR_SDL
        if (m_pDragWindowFilter != nullptr) {
            m_pDragWindow->RemoveMessageFilter(m_pDragWindowFilter.get());
        }
#endif
        if (!m_pDragWindow->IsClosingWnd()) {
            m_pDragWindow->SetDragImage(nullptr);
            m_pDragWindow->CloseWnd();
        }
        m_pDragWindow->Release();
        m_pDragWindow = nullptr;

#ifdef DUILIB_BUILD_FOR_SDL
        m_pDragWindowFilter.reset();
#endif
    }

    const bool bInDraggingOut = IsInDraggingOut();
    m_pDragImage.reset();
    m_pTargetBox = nullptr;
    m_bDraggingOut = false;

    if (bInDraggingOut) {
        //只有执行了拖出操作，才恢复可见，避免产生副作用
        this->SetVisible(true);
    }    

    //恢复拖动调序操作的状态
    if (m_bInDraggingOrder) {
        this->SetAlpha(m_nOldAlpha);
        m_nOldAlpha = 255;
        m_bInDraggingOrder = false;
    }
    m_bMouseDown = false;
    if (!m_rcItemList.empty()) {
        //恢复子控件的绘制顺序
        for (const ItemStatus& itemStatus : m_rcItemList) {
            if ((itemStatus.m_pItem != nullptr) &&
                (itemStatus.m_nPaintOrder != itemStatus.m_pItem->GetPaintOrder())) {
                itemStatus.m_pItem->SetPaintOrder(itemStatus.m_nPaintOrder);
            }
        }
        m_rcItemList.clear();
        //对父控件重绘
        Control* pParent = this->GetParent();
        if (pParent != nullptr) {
            pParent->Invalidate();
            pParent->SetPos(pParent->GetPos());
        }
    }
}

template<typename T>
bool ControlDragableT<T>::DragOutMouseUp(const EventArgs& msg)
{
    if (!IsInDraggingOut() || !IsEnableDragOut() || (m_pTargetBox == nullptr)) {
        return false;
    }
    //控件拖出操作
    Box* pParent = this->GetParent();
    UiPoint pt(msg.ptMouse);
    pt.Offset(m_pTargetBox->GetScrollOffsetInScrollBox());
    if ((pParent == nullptr) || !m_pTargetBox->GetRect().ContainsPt(pt)) {
        return false;
    }
    size_t nIndex = Box::InvalidIndex;
    if (m_pDestControl != nullptr) {
        nIndex = m_pTargetBox->GetItemIndex(m_pDestControl);
    }
    if (nIndex == Box::InvalidIndex) {
        const size_t nCount = m_pTargetBox->GetItemCount();
        for (size_t nItem = 0; nItem < nCount; ++nItem) {
            Control* pControl = m_pTargetBox->GetItemAt(nItem);
            if ((pControl == nullptr) || !pControl->IsVisible() || pControl->IsFloat()) {
                continue;
            }
            if (IsPtInControlRect(pControl, pt)) {
                nIndex = nItem;
                break;
            }
        }
    }
    if (nIndex < m_pTargetBox->GetItemCount()) {
        if (m_pDestControl != nullptr) {
            m_pTargetBox->RemoveItem(m_pDestControl);
            m_pDestControl = nullptr;
        }

        Box* pOldBox = pParent;
        size_t nOldItemIndex = pParent->GetItemIndex(this);
        Box* pNewBox = m_pTargetBox;
        size_t nNewItemIndex = nIndex;

        bool bAutoDestroyChild = pParent->IsAutoDestroyChild();
        pParent->SetAutoDestroyChild(false);
        pParent->RemoveItem(this);
        pParent->SetAutoDestroyChild(bAutoDestroyChild);

        this->SetVisible(true);
        m_pTargetBox->AddItem(this);
        m_pTargetBox->SetItemIndex(this, nIndex);
        m_pTargetBox = nullptr;

        //触发事件
        OnItemBoxChanged(pOldBox, nOldItemIndex, pNewBox, nNewItemIndex);
        return true;
    }
    return false;
}

template<typename T>
void ControlDragableT<T>::OnItemBoxChanged(Box* /*pOldBox*/, size_t /*nOldItemIndex*/,
                                           Box* /*pNewBox*/, size_t /*nNewItemIndex*/)
{
}

template<typename T>
void ControlDragableT<T>::DragOrderMouseUp(const EventArgs& msg)
{
    if (!IsEnableDragOrder() || !IsInDraggingOrder()) {
        return;
    }
    //控件内：拖动改变顺序
    UiPoint pt(msg.ptMouse);
    pt.Offset(this->GetScrollOffsetInScrollBox());
    UiRect itemsValidRect;
    if (GetItemsValidRect(itemsValidRect)) {
        if (pt.x < itemsValidRect.left) {
            pt.x = itemsValidRect.left;
        }
        if (pt.x >= itemsValidRect.right) {
            pt.x = itemsValidRect.right - 1;
        }
        if (pt.y < itemsValidRect.top) {
            pt.y = itemsValidRect.top;
        }
        if (pt.y >= itemsValidRect.bottom) {
            pt.y = itemsValidRect.bottom - 1;
        }
    }
    size_t nOldItemIndex = Box::InvalidIndex;
    size_t nNewItemIndex = Box::InvalidIndex;
    bool bOrderChanged = AdjustItemOrders(pt, m_rcItemList, nOldItemIndex, nNewItemIndex);
    ClearDragStatus();
    if (bOrderChanged) {
        //触发列交换事件
        OnItemOrdersChanged(nOldItemIndex, nNewItemIndex);
    }
}

template<typename T>
bool ControlDragableT<T>::OnDragOutMouseOver(Box* pTargetBox, Box* pOldTargetBox, const UiPoint& ptMouse)
{
    bool bDropped = false;
    if ((pTargetBox == nullptr) || (pTargetBox->GetLayout() == nullptr)) {
        return bDropped;
    }
    if ((pOldTargetBox != nullptr) && (pOldTargetBox != pTargetBox) && (m_pDestControl != nullptr)) {
        //从旧的中移除
        size_t nIndex = pOldTargetBox->GetItemIndex(m_pDestControl);
        if (nIndex != Box::InvalidIndex) {
            if (pOldTargetBox->RemoveItem(m_pDestControl)) {
                m_pDestControl = nullptr;
            }
        }
    }

    UiPoint pt(ptMouse);
    pt.Offset(pTargetBox->GetScrollOffsetInScrollBox());
    const size_t nCount = pTargetBox->GetItemCount();
    for (size_t nIndex = 0; nIndex < nCount; ++nIndex) {
        Control* pControl = pTargetBox->GetItemAt(nIndex);
        if ((pControl == nullptr) || !pControl->IsVisible() || pControl->IsFloat()) {
            continue;
        }
        if ((pControl == m_pDestControl) && (IsPtInControlRect(pControl, pt))) {
            //鼠标在指示控件上
            bDropped = true;
            break;
        }
        if (IsPtInControlRect(pControl, pt)) {
            bDropped = true;
            if (m_pDestControl == nullptr) {
                m_pDestControl = CreateDestControl(pTargetBox);
            }
            if (m_pDestControl == nullptr) {
                break;
            }
            size_t nOldIndex = pTargetBox->GetItemIndex(m_pDestControl);
            if (nOldIndex == Box::InvalidIndex) {
                pTargetBox->AddItem(m_pDestControl);
            }
            UiRect rc = pControl->GetRect();
            if (pTargetBox->GetLayout()->IsHLayout()) {
                size_t nNewIndex = nIndex;
                if ((pt.x - rc.left) > rc.Width() / 2) {
                    if (nNewIndex < (pTargetBox->GetItemCount() - 1)) {
                        nNewIndex += 1;
                    }
                    if (nOldIndex != nNewIndex) {
                        pTargetBox->SetItemIndex(m_pDestControl, nNewIndex);
                    }
                }
                else {
                    if (nOldIndex != (nNewIndex - 1)) {
                        pTargetBox->SetItemIndex(m_pDestControl, nNewIndex);
                    }
                }
            }
            else {
                size_t nNewIndex = nIndex;
                if ((pt.y - rc.top) > rc.Height() / 2) {
                    if (nNewIndex < (pTargetBox->GetItemCount() - 1)) {
                        nNewIndex += 1;
                    }
                    if (nOldIndex != nNewIndex) {
                        pTargetBox->SetItemIndex(m_pDestControl, nNewIndex);
                    }
                }
                else {
                    if (nOldIndex != (nNewIndex - 1)) {
                        pTargetBox->SetItemIndex(m_pDestControl, nNewIndex);
                    }
                }
            }
            break;
        }
    }
    if (!bDropped && pTargetBox->GetRect().ContainsPt(pt)) {
        bDropped = true;
        if (m_pDestControl == nullptr) {
            m_pDestControl = CreateDestControl(pTargetBox);
        }
        size_t nOldIndex = pTargetBox->GetItemIndex(m_pDestControl);
        if (nOldIndex == Box::InvalidIndex) {
            pTargetBox->AddItem(m_pDestControl);
        }
    }
    return bDropped;
}

template<typename T>
DragWindow* ControlDragableT<T>::CreateDragWindow()
{
    return new DragWindow;
}

template<typename T>
Control* ControlDragableT<T>::CreateDestControl(Box* pTargetBox)
{
    Control* pDestControl = new Control(this->GetWindow());
    pDestControl->SetAttribute(_T("bkcolor"), _T("#FF5D6B99"));
    pDestControl->SetAttribute(_T("valign"), _T("center"));
    pDestControl->SetAttribute(_T("halign"), _T("center"));

    Layout* pLayout = nullptr;
    if (pTargetBox != nullptr) {
        pLayout = pTargetBox->GetLayout();
    }
    bool bInited = false;
    if ((pLayout != nullptr) && pLayout->IsTileLayout()) {
        UiSize szItem;
        szItem.cx = this->GetFixedWidth().GetInt32();
        szItem.cy = this->GetFixedHeight().GetInt32();
        if ((szItem.cx > 0) && (szItem.cy > 0)) {
            pDestControl->SetAlpha(128);
            pDestControl->SetFixedHeight(UiFixedInt(szItem.cy), false, false);
            pDestControl->SetFixedWidth(UiFixedInt(szItem.cx), false, false);
            bInited = true;
        }
    }
    if (!bInited) {        
        if ((pLayout != nullptr) && pLayout->IsVLayout()) {
            pDestControl->SetAttribute(_T("height"), _T("4"));
            pDestControl->SetAttribute(_T("width"), _T("80%"));
        }
        else {
            pDestControl->SetAttribute(_T("width"), _T("4"));
            pDestControl->SetAttribute(_T("height"), _T("80%"));
        }
    }
    return pDestControl;
}

template<typename T>
std::shared_ptr<IBitmap> ControlDragableT<T>::CreateDragoutImage()
{
    UiRect rc = this->GetRect();
    if (rc.IsEmpty()) {
        return nullptr;
    }
    std::unique_ptr<IRender> render;
    IRenderFactory* pRenderFactory = GlobalManager::Instance().GetRenderFactory();
    ASSERT(pRenderFactory != nullptr);
    if (pRenderFactory != nullptr) {
        ASSERT(this->GetWindow() != nullptr);
        IRenderDpiPtr spRenderDpi;
        if (this->GetWindow() != nullptr) {
            spRenderDpi = this->GetWindow()->GetRenderDpi();
        }
        render.reset(pRenderFactory->CreateRender(spRenderDpi));
    }
    ASSERT(render != nullptr);
    if(render == nullptr) {
        return nullptr;
    }
    const int32_t kDragImageWidth = rc.Width();
    const int32_t kDragImageHeight = rc.Height();
    if (render->Resize(kDragImageWidth, kDragImageHeight)) {
        int32_t destWidth = 0;
        int32_t destHeight = 0;
        float scale = (float)rc.Width() / (float)rc.Height();
        if (scale >= 1.0) {
            destWidth = kDragImageWidth;
            destHeight = (int32_t)(kDragImageWidth * (float)rc.Height() / (float)rc.Width());
        }
        else {
            destHeight = kDragImageHeight;
            destWidth = (int32_t)(kDragImageHeight * (float)rc.Width() / (float)rc.Height());
        }
        render->AlphaBlend((kDragImageWidth - destWidth) / 2, 0, destWidth, destHeight,
            this->GetWindow()->GetRender(),
            rc.left, rc.top, rc.Width(), rc.Height());
    }
    std::shared_ptr<IBitmap> pDragImage(render->MakeImageSnapshot());
    return pDragImage;
}

template<typename T>
bool ControlDragableT<T>::IsPtInControlRect(Control* pControl, const UiPoint& pt) const
{
    if (pControl == nullptr) {
        return false;
    }
    UiRect rc = pControl->GetRect();
    if (rc.ContainsPt(pt)) {
        return true;
    }
    UiMargin rcMargin = pControl->GetMargin();
    rc.Inflate(rcMargin);
    if (rc.ContainsPt(pt)) {
        return true;
    }
    Layout* pLayout = nullptr;
    const Box* pBox = pControl->GetParent();
    if (pBox != nullptr) {
        pLayout = pBox->GetLayout();
    }
    if (pLayout != nullptr) {
        int32_t nChildMarginX = pLayout->GetChildMarginX() / 2;
        int32_t nChildMarginY = pLayout->GetChildMarginY() / 2;
        if ((nChildMarginX > 0) || (nChildMarginY > 0)) {
            rc = pControl->GetRect();
            rc.left -= nChildMarginX;
            rc.right += nChildMarginX;
            rc.top -= nChildMarginY;
            rc.bottom += nChildMarginY;
            if (rc.ContainsPt(pt)) {
                return true;
            }
        }
    }
    return false;
}

template<typename T>
void ControlDragableT<T>::DragOrderMouseDown(const EventArgs& /*msg*/)
{
    m_rcItemList.clear();
    if (!IsEnableDragOrder()) {
        return;
    }
    const Box* pParent = this->GetParent();
    if (pParent == nullptr) {
        return;
    }
    //控件内的顺序调整：记录每个控件的原始位置信息
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
            //当前控件，绘制顺序需要放最后，避免被其他控件遮挡
            itemStatus.m_pItem->SetPaintOrder(255);
            ASSERT(itemStatus.m_rcPos.ContainsPt(m_ptMouseDown));
        }
    }
}

template<typename T>
bool ControlDragableT<T>::DragOrderMouseMove(const EventArgs& msg)
{
    bool bRet = false;
    if (!IsEnableDragOrder()) {
        //该控件禁止拖动调序
        return bRet;
    }    
    const Box* pParent = this->GetParent();
    if (pParent == nullptr) {
        return bRet;
    }
    const Layout* pLayout = pParent->GetLayout();
    if ((pLayout == nullptr) || pLayout->IsTileLayout() || (!pLayout->IsHLayout() && !pLayout->IsVLayout())) {
        return bRet;
    }
    UiPoint pt(msg.ptMouse);
    pt.Offset(this->GetScrollOffsetInScrollBox());
    UiPoint ptNow = pt;
    UiRect itemsValidRect;
    if (GetItemsValidRect(itemsValidRect)) {
        if (pt.x < itemsValidRect.left) {
            pt.x = itemsValidRect.left;
        }
        if (pt.x >= itemsValidRect.right) {
            pt.x = itemsValidRect.right - 1;
        }
        if (pt.y < itemsValidRect.top) {
            pt.y = itemsValidRect.top;
        }
        if (pt.y >= itemsValidRect.bottom) {
            pt.y = itemsValidRect.bottom - 1;
        }
    }
    if (pLayout->IsHLayout()) {
        int32_t xOffset = pt.x - m_ptMouseDown.x;
        if (std::abs(xOffset) < this->Dpi().GetScaleInt(DRAG_OPERATION_MIN_PT)) {
            return bRet;
        }
        //调整其他控件的位置
        AdjustItemPos(pt, m_ptMouseDown, m_rcItemList);

        UiRect rect = m_rcMouseDown;
        rect.left += xOffset;
        rect.right += xOffset;

        this->SetPos(rect);

        ScrollBox* pScrollBox = dynamic_cast<ScrollBox*>(this->GetParent());
        if ((pScrollBox != nullptr) && (pScrollBox->GetScrollRange().cx > 0)) {
            if (ptNow.x <= itemsValidRect.left) {
                //向左滚动
                pScrollBox->LineLeft();
            }
            else if (ptNow.x >= itemsValidRect.right) {
                //向右滚动
                pScrollBox->LineRight();
            }
        }
    }
    else {
        int32_t yOffset = pt.y - m_ptMouseDown.y;
        if (std::abs(yOffset) < this->Dpi().GetScaleInt(DRAG_OPERATION_MIN_PT)) {
            return bRet;
        }
        //调整其他控件的位置
        AdjustItemPos(pt, m_ptMouseDown, m_rcItemList);

        UiRect rect = m_rcMouseDown;
        rect.top += yOffset;
        rect.bottom += yOffset;
        this->SetPos(rect);

        ScrollBox* pScrollBox = dynamic_cast<ScrollBox*>(this->GetParent());
        if ((pScrollBox != nullptr) && (pScrollBox->GetScrollRange().cy > 0)) {
            if (ptNow.y <= itemsValidRect.top) {
                //向上滚动
                pScrollBox->LineUp();
            }
            else if (ptNow.y >= itemsValidRect.bottom) {
                //向下滚动
                pScrollBox->LineDown();
            }
        }
    }

    if (!m_bInDraggingOrder) {
        bRet = true;
        m_bInDraggingOrder = true;
        m_nOldAlpha = (uint8_t)this->GetAlpha();
        //设置为半透明的效果
        this->SetAlpha(m_nDragAlpha);
    }
    return bRet;
}

template<typename T>
bool ControlDragableT<T>::DragOutMouseMove(const EventArgs& msg)
{
    bool bRet = false;
    if (!IsEnableDragOut()) {
        //该控件禁止拖出操作
        return bRet;
    }
    const Window* pWindow = this->GetWindow();
    const Box* pParent = this->GetParent();
    if ((pParent == nullptr) || (pWindow == nullptr)) {
        return bRet;
    }
    if (pParent->GetDragOutId() == 0) {
        //父容器不支持拖出操作
        return bRet;
    }
    const Layout* pLayout = pParent->GetLayout();
    if ((pLayout == nullptr) || (!pLayout->IsHLayout() && !pLayout->IsVLayout())) {
        return bRet;
    }
    UiPoint pt(msg.ptMouse);
    pt.Offset(this->GetScrollOffsetInScrollBox());
    if (!IsInDraggingOut()) {
        int32_t xOffset = pt.x - m_ptMouseDown.x;
        int32_t yOffset = pt.y - m_ptMouseDown.y;
        int32_t nMinOffset = this->Dpi().GetScaleInt(DRAG_OPERATION_MIN_PT);
        if ((std::abs(xOffset) < nMinOffset) && (std::abs(yOffset) < nMinOffset)) {
            return bRet;
        }
    }

    std::shared_ptr<IBitmap> pDragImage = m_pDragImage;
    if (pDragImage == nullptr) {
        pDragImage = CreateDragoutImage();
        m_pDragImage = pDragImage;
    }

    bool bDropped = false;
    Box* pOldTargetBox = m_pTargetBox;
    m_pTargetBox = pWindow->FindDroppableBox(msg.ptMouse, pParent->GetDragOutId());
    if (m_pTargetBox != nullptr) {
        bDropped = OnDragOutMouseOver(m_pTargetBox, pOldTargetBox, msg.ptMouse);
    }
    if (!bDropped) {
        if ((m_pDestControl != nullptr) && (m_pTargetBox != nullptr)) {
            if (m_pTargetBox->RemoveItem(m_pDestControl)) {
                m_pDestControl = nullptr;
            }
        }
        if ((m_pDestControl != nullptr) && (pOldTargetBox != nullptr)) {
            if (pOldTargetBox->RemoveItem(m_pDestControl)) {
                m_pDestControl = nullptr;
            }
        }
        if (m_pDestControl != nullptr) {
            delete m_pDestControl;
            m_pDestControl = nullptr;
        }
    }
    if (!bDropped) {
        m_pTargetBox = nullptr;
    }

    //拖出父容器
    if ((m_pDragWindow == nullptr) || m_pDragWindow->IsClosingWnd()) {
        if (m_pDragWindow != nullptr) {
#ifdef DUILIB_BUILD_FOR_SDL
            if (m_pDragWindowFilter != nullptr) {
                m_pDragWindow->RemoveMessageFilter(m_pDragWindowFilter.get());
            }
#endif
            m_pDragWindow->Release();
            m_pDragWindow = nullptr;

#ifdef DUILIB_BUILD_FOR_SDL
            m_pDragWindowFilter.reset();
#endif
        }
        m_pDragWindow = CreateDragWindow();
        ASSERT(m_pDragWindow != nullptr);
        if (m_pDragWindow != nullptr) {
            m_pDragWindow->AddRef();

            WindowCreateParam createWndParam;
            createWndParam.m_dwStyle = kWS_POPUP;
            createWndParam.m_dwExStyle = kWS_EX_LAYERED | kWS_EX_NOACTIVATE;
            m_pDragWindow->CreateWnd(this->GetWindow(), createWndParam);
            ASSERT(m_pDragWindow->IsWindow());
            if (m_pDragWindow->IsWindow()) {
                m_pDragWindow->AddRef();
            }
            m_pDragWindow->SetDragImage(pDragImage);
            m_pDragWindow->ShowWindow(kSW_SHOW_NA);

#ifdef DUILIB_BUILD_FOR_SDL
            m_pDragWindowFilter = std::make_unique<DragWindowFilter>(this->GetWindow(), m_pDragWindow);
            m_pDragWindow->AddMessageFilter(m_pDragWindowFilter.get());
            //减缓显示时黑屏现象
            m_pDragWindow->InvalidateAll();
#endif
        }
    }
    if (m_pDragWindow != nullptr) {
        m_pDragWindow->AdjustPos();
    }
    if (this->IsVisible()) {
        this->SetVisible(false);
    }    
    bRet = true;
    return bRet;
}

typedef ControlDragableT<Control> ControlDragable;
typedef ControlDragableT<Box> BoxDragable;
typedef ControlDragableT<HBox> HBoxDragable;
typedef ControlDragableT<VBox> VBoxDragable;

}

#endif // UI_CORE_CONTROL_DRAGABLE_H_
