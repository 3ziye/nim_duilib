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
    DataChangedNotify m_pfnDataChangedNotify;

    /** ���ݸ��������仯����Ӧ����
    */
    CountChangedNotify m_pfnCountChangedNotify;
};

class UILIB_API VirtualTileLayout : public TileLayout
{
public:
    VirtualTileLayout();

    /** �����ڲ����пؼ���λ����Ϣ
     * @param [in] items �ؼ��б�
     * @param[in] rc ��ǰ����λ����Ϣ, �����ڱ߾࣬����������߾�
     * @return �������к����պ��ӵĿ�Ⱥ͸߶���Ϣ
     */
    virtual UiSize64 ArrangeChild(const std::vector<Control*>& items, UiRect rc) override;

    /** �����ڲ��ӿؼ���С�������������С���������͵��ӿؼ������ԣ��������С����
     * @param[in] items �ӿؼ��б�
     * @param [in] szAvailable ���ô�С������������ÿؼ����ڱ߾࣬��������������ؼ�����߾�
     * @return �������к����ղ��ֵĴ�С��Ϣ����Ⱥ͸߶ȣ���
               ����items���ӿؼ�����߾࣬����items���ӿؼ����ڱ߾ࣻ
               ����Box�ؼ�������ڱ߾ࣻ
               ������Box�ؼ��������߾ࣻ
               ����ֵ�в������������͵��ӿؼ���С��
     */
    virtual UiSize EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable);
    
public:
    /** ��ȡ������ĸ߶�
    * @param [in] nCount ��������������ΪBox::InvalidIndex�����ȡ����������ĸ߶��ܺ�
    * @param [in] rc ��ǰ������С��Ϣ, �ⲿ����ʱ����Ҫ�ȼ�ȥ�ڱ߾�
    * @return ���� nCount ��������ĸ߶��ܺ�
    */
    int64_t GetElementsHeight(UiRect rc, size_t nCount);

    /** �ӳټ���չʾ����
    * @param [in] rc ��ǰ������С��Ϣ, �ⲿ����ʱ����Ҫ�ȼ�ȥ�ڱ߾�
    */
    void LazyArrangeChild(UiRect rc);

    /** ��ȡ��Ҫչʾ����ʵ������������������Control�����Ӧ����ʵ�����
    * @param [in] rc ��ǰ������С��Ϣ, �ⲿ����ʱ����Ҫ�ȼ�ȥ�ڱ߾�
    */
    size_t AjustMaxItem(UiRect rc);

    /** ��������
    *@param [in] rcWidth ����������
    *@return ����õ�������, ���ڻ����1
    */
    int32_t CalcTileColumns(int32_t rcWidth) const;
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
    void SetDataProvider(VirtualTileBoxElement* pProvider);

    /** ��ȡ���ݴ������
    */
    VirtualTileBoxElement* GetDataProvider();

    /** ˢ���б�
    */
    void Refresh();

    /** ��ȡ��ǰ���пɼ��ؼ�������Ԫ������
    * @param[out] collection �����б���Χ�ǣ�[0, GetElementCount())
    */
    void GetDisplayCollection(std::vector<size_t>& collection);

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

private:
    /** �ж��Ƿ�Ҫ���²���
    */
    bool NeedReArrange();

    /** ���²�������
    * @param[in] bForce �Ƿ�ǿ�����²���
    */
    void ReArrangeChild(bool bForce);

    /** �������������ݣ�ֻ���ؽ�����ʾ������ǲ������ݣ�
    */
    void LazyArrangeChild();

    /** �õ�n��Ԫ�ض�Ӧ�ĸ߶Ⱥͣ�
    * @param[in] nCount Ҫ�õ�����Ԫ�صĸ߶ȣ�-1��ʾȫ��Ԫ��
    * @return ����ָ������Ԫ�صĸ߶Ⱥ�
    */
    int64_t CalcElementsHeight(size_t nCount);

    /** ��ȡ���ֽӿ�
    */
    VirtualTileLayout* GetTileLayout();

    /** ��ȡ������Ŀ�ĸ߶�
    */
    int64_t GetRealElementHeight();

    /** ��ȡ����
    */
    size_t GetColumns();

    /** �ж�ĳ��Ԫ���Ƿ��ڿɼ���Χ��
    * @param[in] iIndex Ԫ������
    * @return ���� true ��ʾ�ɼ�������Ϊ���ɼ�
    */
    bool IsElementDisplay(size_t iIndex);

    /** �õ��ɼ���Χ�ڵ�һ��Ԫ�ص�ǰһ��Ԫ������
    * @return ������һ��Ԫ�ص�����
    */
    size_t GetTopElementIndex();

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
};

}
