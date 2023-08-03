#ifndef UI_BOX_VIRTUAL_LISTBOX_H_
#define UI_BOX_VIRTUAL_LISTBOX_H_

#pragma once

#include "duilib/Box/ListBox.h"
#include "duilib/Box/VirtualHLayout.h"
#include "duilib/Box/VirtualVLayout.h"
#include "duilib/Box/VirtualHTileLayout.h"
#include "duilib/Box/VirtualVTileLayout.h"
#include "base/callback/callback.h"
#include <functional>

namespace ui {

typedef std::function<void(size_t nStartIndex, size_t nEndIndex)> DataChangedNotify;
typedef std::function<void()> CountChangedNotify;

class UILIB_API VirtualListBoxElement : public virtual nbase::SupportWeakCallback
{
public:
    VirtualListBoxElement();

    /** ����һ��������
    * @return ���ش������������ָ��
    */
    virtual Control* CreateElement() = 0;

    /** ���ָ��������
    * @param[in] control ������ؼ�ָ��
    * @param[in] index ����
    */
    virtual void FillElement(Control* control, size_t index) = 0;

    /** ��ȡ����������
    * @return ��������������
    */
    virtual size_t GetElementCount() = 0;

    /** ע���¼�֪ͨ�ص�
    * @param [in] dcNotify �������ݱ仯֪ͨ�ӿ�
    * @param [in] ccNotify ����������仯֪ͨ�ӿ�
    */
    void RegNotifys(const DataChangedNotify& dcNotify, const CountChangedNotify& ccNotify);

protected:

    /** ����֪ͨ���������ݷ����仯
    * @param [in] nStartIndex ���ݵĿ�ʼ�±�
    * @param [in] nEndIndex ���ݵĽ����±�
    */
    void EmitDataChanged(size_t nStartIndex, size_t nEndIndex);

    /** ����֪ͨ����������������仯
    */
    void EmitCountChanged();

private:

    /** �������ݷ����仯����Ӧ����
    */
    DataChangedNotify m_pfnDataChangedNotify;

    /** ���ݸ��������仯����Ӧ����
    */
    CountChangedNotify m_pfnCountChangedNotify;
};

/** ���ʵ�ֵ�ListBox��֧�ִ���������ֻ֧�����������
*/
class UILIB_API VirtualListBox : public ListBox
{
    friend class VirtualHLayout;
    friend class VirtualVLayout;    
    friend class VirtualHTileLayout;
    friend class VirtualVTileLayout;
public:
    explicit VirtualListBox(Layout* pLayout);

    /**
    * @brief �������ݴ������
    * @param[in] pProvider ��������Ҫ��д VirtualListBoxElement �Ľӿ�����Ϊ���ݴ������
    */
    void SetDataProvider(VirtualListBoxElement* pProvider);

    /** ��ȡ���ݴ������
    */
    VirtualListBoxElement* GetDataProvider();

    /** ˢ���б�
    */
    void Refresh();

    /** ��ȡ��ǰ���пɼ��ؼ�������Ԫ������
    * @param[out] collection �����б���Χ�ǣ�[0, GetElementCount())
    */
    void GetDisplayElements(std::vector<size_t>& collection);

    /** �ÿؼ��ڿɼ���Χ��
    * @param[in] iIndex Ԫ�������ţ���Χ�ǣ�[0, GetElementCount())
    * @param[in] bToTop �Ƿ������Ϸ�
    */
    void EnsureVisible(size_t iIndex, bool bToTop);

protected:
    /// ��д����ӿڣ��ṩ���Ի�����
    virtual void SetScrollPos(UiSize64 szPos) override;
    virtual void SetPos(UiRect rc) override;
    virtual void PaintChild(IRender* pRender, const UiRect& rcPaint) override;

    //����ɾ����������ֹ�ⲿ����
    virtual bool RemoveItem(Control* pControl) override;
    virtual bool RemoveItemAt(size_t iIndex) override;
    virtual void RemoveAllItems() override;

    virtual void HandleEvent(const EventArgs& event) override;

protected:
    /** ���̰���֪ͨ
    * @param[in] ch ����
    */
    virtual void OnKeyDown(TCHAR ch);

    /** ���̵���֪ͨ
    * @param[in] ch ����
    */
    virtual void OnKeyUp(TCHAR ch);

    /** ��������ֽӿ�
    */
    void SetVirtualLayout(VirtualLayout* pVirtualLayout);

private:
    /** �Ƿ������ݴ������ӿ�
    */
    bool HasDataProvider() const;

    /** ����һ������
    * @return ���ش����������ָ��
    */
    Control* CreateElement();

    /** ���ָ������
    * @param[in] control ����ؼ�ָ��
    * @param[in] index ����
    * @return ���ش����������ָ��
    */
    void FillElement(Control* pControl, size_t iIndex);

    /** ��ȡԪ������
    * @return ����Ԫ����ָ��
    */
    size_t GetElementCount();

    /** �������ݷ����仯�����¼�����Ҫ���¼���չʾ����
    */
    void OnModelDataChanged(size_t nStartIndex, size_t nEndIndex);

    /** ���ݸ��������仯�����¼�����Ҫ���¼���չʾ����
    */
    void OnModelCountChanged();

    /** ���²�������
    * @param[in] bForce �Ƿ�ǿ�����²���
    */
    void ReArrangeChild(bool bForce);

    /** ������Ԫ�ص�������ת��Ϊչʾ���ݵ�������
    * @param [in] nElementIndex ����Ԫ�ص�������
    * @return ���nElementIndex����ʾ��Χ�ڵ����ݣ����ض�Ӧ��Item������
    *         ���nElementIndex������ʾ��Χ�ڵ����ݣ�����Box::InvalidIndex����ʾ�޶�Ӧ��Item������
    */
    size_t ElementIndexToItemIndex(size_t nElementIndex) const;

private:
    /** ���ݴ������ӿڣ��ṩչʾ����
    */
    VirtualListBoxElement* m_pDataProvider;

    /** ����ֽӿ�
    */
    VirtualLayout* m_pVirtualLayout;
};

/** ���򲼾ֵ����ListBox
*/
class UILIB_API VirtualHListBox : public VirtualListBox
{
public:
    VirtualHListBox() :
        VirtualListBox(new VirtualHLayout)
    {
        VirtualLayout* pVirtualLayout = dynamic_cast<VirtualHLayout*>(GetLayout());
        SetVirtualLayout(pVirtualLayout);
    }

    virtual std::wstring GetType() const override { return DUI_CTR_VIRTUAL_HLISTBOX; }
};

/** ���򲼾ֵ����ListBox
*/
class UILIB_API VirtualVListBox : public VirtualListBox
{
public:
    VirtualVListBox() :
        VirtualListBox(new VirtualVLayout)
    {
        VirtualLayout* pVirtualLayout = dynamic_cast<VirtualVLayout*>(GetLayout());
        SetVirtualLayout(pVirtualLayout);
    }

    virtual std::wstring GetType() const override { return DUI_CTR_VIRTUAL_VLISTBOX; }
};

/** ��Ƭ���ֵ����ListBox(���򲼾�)
*/
class UILIB_API VirtualHTileListBox : public VirtualListBox
{
public:
    VirtualHTileListBox() :
        VirtualListBox(new VirtualHTileLayout)
    {
        VirtualLayout* pVirtualLayout = dynamic_cast<VirtualHTileLayout*>(GetLayout());
        SetVirtualLayout(pVirtualLayout);
    }

    virtual std::wstring GetType() const override { return DUI_CTR_VIRTUAL_HTILE_LISTBOX; }
};

/** ��Ƭ���ֵ����ListBox(���򲼾�)
*/
class UILIB_API VirtualVTileListBox : public VirtualListBox
{
public:
    VirtualVTileListBox() :
        VirtualListBox(new VirtualVTileLayout)
    {
        VirtualLayout* pVirtualLayout = dynamic_cast<VirtualVTileLayout*>(GetLayout());
        SetVirtualLayout(pVirtualLayout);
    }

    virtual std::wstring GetType() const override { return DUI_CTR_VIRTUAL_VTILE_LISTBOX; }
};

}

#endif //UI_BOX_VIRTUAL_LISTBOX_H_
