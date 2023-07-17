#pragma once

#include "duilib/Box/TileBox.h"
#include "duilib/Box/ListBox.h"
#include "base/callback/callback.h"
#include <functional>

namespace ui {

typedef std::function<void(size_t nStartIndex, size_t nEndIndex)> DataChangedNotify;
typedef std::function<void()> CountChangedNotify;

class UILIB_API VirtualTileBoxElement : public virtual nbase::SupportWeakCallback
{
public:
    VirtualTileBoxElement();

    /** ����һ��������
    * @return ���ش������������ָ��
    */
    virtual ui::Control* CreateElement() = 0;

    /** ���ָ��������
    * @param[in] control ������ؼ�ָ��
    * @param[in] index ����
    */
    virtual void FillElement(ui::Control* control, size_t index) = 0;

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
    DataChangedNotify m_DataChangedNotify;

    /** ���ݸ��������仯����Ӧ����
    */
    CountChangedNotify m_CountChangedNotify;
};

class UILIB_API VirtualTileLayout : public ui::TileLayout
{
public:
    VirtualTileLayout();
    virtual UiSize64 ArrangeChild(const std::vector<ui::Control*>& items, ui::UiRect rc) override;
    virtual UiSize EstimateSizeByChild(const std::vector<ui::Control*>& items, ui::UiSize szAvailable) override;
    virtual bool SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
    
    /** ��ȡ������ĸ߶�
    *@param [in] nCount ��������������ΪBox::InvalidIndex�����ȡ����������ĸ߶��ܺ�
    *@return ���� nCount ��������ĸ߶��ܺ�
    */
    virtual int64_t GetElementsHeight(size_t nCount);

    /** �ӳټ���չʾ����
    */
    virtual void LazyArrangeChild();

    /** ��ȡ��Ҫչʾ����ʵ������������������Control�����Ӧ����ʵ�����
    */
    virtual size_t AjustMaxItem();

private:
    /** �Զ�����������־�����Ϊtrue���Զ�����������������ʱ��ʹ��m_nColumns������
    */
    bool m_bAutoCalcColumn;
};

/** ���ʵ�ֵ�TileBox��֧�ִ���������ֻ֧�����������
*/
class UILIB_API VirtualTileBox : public ListBox
{
    friend class VirtualTileLayout;
public:
    explicit VirtualTileBox(Layout* pLayout = new VirtualTileLayout);

    /**
    * @brief �������ݴ������
    * @param[in] pProvider ��������Ҫ��д VirtualListBoxElement �Ľӿ�����Ϊ���ݴ������
    */
    virtual void SetDataProvider(VirtualTileBoxElement* pProvider);

    /** ��ȡ���ݴ������
    */
    virtual VirtualTileBoxElement* GetDataProvider();

    /** ˢ���б�
    */
    virtual void Refresh();

    /** ɾ����������
    */
    virtual void RemoveAllItems() override;

    /** �����Ƿ�ǿ�����²���, ���Ϊtrue������ˢ�º͹���λ�÷����仯ʱ������ǿ�����²���
    * @param[in] bForce ����Ϊ true Ϊǿ�ƣ�����Ϊ��ǿ��
    */
    void SetForceArrange(bool bForce);

    /** ��ȡ��ǰ���пɼ��ؼ�������Ԫ������
    * @param[out] collection �����б���Χ�ǣ�[0, GetElementCount())
    */
    void GetDisplayCollection(std::vector<size_t>& collection);

    /** �ÿؼ��ڿɼ���Χ��
    * @param[in] iIndex Ԫ�������ţ���Χ�ǣ�[0, GetElementCount())
    * @param[in] bToTop �Ƿ������Ϸ�
    */
    void EnsureVisible(size_t iIndex, bool bToTop = false);

protected:
    /// ��д����ӿڣ��ṩ���Ի�����
    virtual void SetScrollPos(UiSize64 szPos) override;
    virtual void HandleEvent(const EventArgs& event) override;
    virtual void SetPos(UiRect rc) override;
    virtual void PaintChild(IRender* pRender, const UiRect& rcPaint) override;

    /** ���²�������
    * @param[in] bForce �Ƿ�ǿ�����²���
    */
    void ReArrangeChild(bool bForce);

    /** ���̰���֪ͨ
    * @param[in] ch ����
    */
    virtual void OnKeyDown(TCHAR ch);

    /** ���̵���֪ͨ
    * @param[in] ch ����
    */
    virtual void OnKeyUp(TCHAR ch);

private:
    enum class ScrollDirection
    {
        kScrollUp = -1,
        kScrollDown = 1
    };

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

    /** �õ�n��Ԫ�ض�Ӧ�ĸ߶Ⱥͣ�
    * @param[in] nCount Ҫ�õ�����Ԫ�صĸ߶ȣ�-1��ʾȫ��Ԫ��
    * @return ����ָ������Ԫ�صĸ߶Ⱥ�
    */
    int64_t CalcElementsHeight(size_t nCount);

    /** �õ��ɼ���Χ�ڵ�һ��Ԫ�ص�ǰһ��Ԫ������
    * @param[out] bottom ������һ��Ԫ�ص� bottom ֵ
    * @return ������һ��Ԫ�ص�����
    */
    size_t GetTopElementIndex(int64_t* bottom);

    /** �ж�ĳ��Ԫ���Ƿ��ڿɼ���Χ��
    * @param[in] iIndex Ԫ������
    * @return ���� true ��ʾ�ɼ�������Ϊ���ɼ�
    */
    bool IsElementDisplay(size_t iIndex);

    /** �ж��Ƿ�Ҫ���²���
    * @param[out] direction ���Ϲ����������¹���
    * @return true Ϊ��Ҫ���²��֣�����Ϊ false
    */
    bool NeedReArrange(ScrollDirection& direction);

    /** ��ȡ���ֽӿ�
    */
    VirtualTileLayout* GetTileLayout();

    /** ��ȡ������Ŀ�ĸ߶�
    */
    int64_t GetRealElementHeight();

    /** ��ȡ����
    */
    size_t GetColumns();

    /** �������������ݣ�ֻ���ؽ�����ʾ������ǲ������ݣ�
    */
    void LazyArrangeChild();

    /** �������ݷ����仯�����¼�����Ҫ���¼���չʾ����
    */
    void OnModelDataChanged(size_t nStartIndex, size_t nEndIndex);

    /** ���ݸ��������仯�����¼�����Ҫ���¼���չʾ����
    */
    void OnModelCountChanged();

    /** ������Ԫ�ص�������ת��Ϊչʾ���ݵ�������
    * @param [in] nElementIndex ����Ԫ�ص�������
    * @return ���nElementIndex����ʾ��Χ�ڵ����ݣ����ض�Ӧ��Item������
    *         ���nElementIndex������ʾ��Χ�ڵ����ݣ�����Box::InvalidIndex����ʾ�޶�Ӧ��Item������
    */
    size_t ElementIndexToItemIndex(size_t nElementIndex);

    /** ��չʾ���ݵ�Item�����ţ�ת��Ϊ����Ԫ�ص�������
    */
    size_t ItemIndexToElementIndex(size_t nItemIndex);

private:
    /** ���ݴ������ӿڣ��ṩչʾ����
    */
    VirtualTileBoxElement* m_pDataProvider;

    /** �б���ʵ�ؼ���������(��̬����)
    */
    size_t m_nMaxItemCount;

    /** �ɵĹ�����λ��
    */
    int64_t m_nOldYScrollPos;

    /** ������ݸı���Ƿ��Ѿ������һ��Arrange����
    */
    bool m_bArrangedOnce;

    /** ǿ�Ʋ��ֱ��
    */
    bool m_bForceArrange;
};

}
