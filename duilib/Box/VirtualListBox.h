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
    virtual ui::Control* CreateElement() = 0;

    /** ���ָ��������
    * @param [in] pControl ������ؼ�ָ��
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    */
    virtual bool FillElement(ui::Control* pControl, size_t nElementIndex) = 0;

    /** ��ȡ����������
    * @return ��������������
    */
    virtual size_t GetElementCount() = 0;

    /** ����ѡ��״̬
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    * @param [in] bSelected true��ʾѡ��״̬��false��ʾ��ѡ��״̬
    */
    virtual void SetElementSelected(size_t nElementIndex, bool bSelected) = 0;

    /** ��ȡѡ��״̬
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    * @return true��ʾѡ��״̬��false��ʾ��ѡ��״̬
    */
    virtual bool IsElementSelected(size_t nElementIndex) = 0;

public:
    /** ע���¼�֪ͨ�ص�
    * @param [in] dcNotify �������ݱ仯֪ͨ�ӿ�
    * @param [in] ccNotify ����������仯֪ͨ�ӿ�
    */
    void RegNotifys(const DataChangedNotify& dcNotify, const CountChangedNotify& ccNotify);

protected:

    /** ����֪ͨ���������ݷ����仯
    * @param [in] nStartElementIndex ���ݵĿ�ʼ�±�
    * @param [in] nEndElementIndex ���ݵĽ����±�
    */
    void EmitDataChanged(size_t nStartElementIndex, size_t nEndElementIndex);

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

    /** �Ƿ������ݴ������ӿ�
    */
    bool HasDataProvider() const;

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

    virtual void HandleEvent(const EventArgs& msg) override;

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

    /** ִ����ˢ�²���, �����UI�ؼ��������ܻᷢ���仯
    */
    virtual void OnRefresh() {};

    /** ִ�������Ų����������UI�ؼ�����������������䣨ͨ��FillElement������
    */
    virtual void OnArrangeChild() {};

protected:
    /** ����һ������
    * @return ���ش����������ָ��
    */
    Control* CreateElement();

    /** ���ָ��������
    * @param[in] pControl ������ؼ�ָ��
    * @param[in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    */
    void FillElement(Control* pControl, size_t nElementIndex);

    /** ��ȡԪ������
    * @return ����Ԫ����ָ��
    */
    size_t GetElementCount();

    /** ���²�������
    * @param[in] bForce �Ƿ�ǿ�����²���
    */
    void ReArrangeChild(bool bForce);

    /** �������ݷ����仯�����¼�����Ҫ���¼���չʾ����
    */
    void OnModelDataChanged(size_t nStartElementIndex, size_t nEndElementIndex);

    /** ���ݸ��������仯�����¼�����Ҫ���¼���չʾ����
    */
    void OnModelCountChanged();

    /** ѡ��״̬�仯��ѡ����ĳ������
    */
    bool OnSelectedItem(const ui::EventArgs&);

    /** ѡ��״̬�仯��ȡ��ѡ����ĳ������
    */
    bool OnUnSelectedItem(const ui::EventArgs&);

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
