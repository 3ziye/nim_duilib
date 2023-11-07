#ifndef UI_CONTROL_LIST_CTRL_DATA_VIEW_H_
#define UI_CONTROL_LIST_CTRL_DATA_VIEW_H_

#pragma once

#include "duilib/Box/VirtualListBox.h"

namespace ui
{
//�����ࣺListCtrlDataView / ListCtrlDataLayout

/** �б�������ʾ�Ͳ��ֹ���
*/
class ListCtrl;
class ListCtrlDataView : public VirtualListBox
{
    friend class ListCtrlDataLayout;
public:
    ListCtrlDataView();
    virtual ~ListCtrlDataView();

    /** ����ListCtrl�ؼ��ӿ�
    */
    void SetListCtrl(ListCtrl* pListCtrl);

    /** ˢ���б�
    */
    virtual void Refresh() override;

    /** ��ȡ�б�ؼ��Ŀ�ȣ�Header�ĸ������ܿ��֮�ͣ�
    */
    int32_t GetListCtrlWidth() const;

    /** ���ö���Ԫ�ص�������
    */
    void SetTopElementIndex(size_t nTopElementIndex);

    /** ��ȡ����Ԫ�ص�������
    */
    size_t GetTopElementIndex() const;

    /** ���õ�ǰ��ʾ���������б�˳���Ǵ��ϵ���
    * @param [in] itemIndexList ��ǰ��ʾ���������������б�
    */
    void SetDisplayDataItems(const std::vector<size_t>& itemIndexList);

    /** ��ȡ��ǰ��ʾ���������б�˳���Ǵ��ϵ���
    * @param [in] itemIndexList ��ǰ��ʾ���������������б�
    */
    void GetDisplayDataItems(std::vector<size_t>& itemIndexList) const;

    /** �ж�һ���������Ƿ�ɼ�
    * @param [in] itemIndex �������������
    */
    bool IsDataItemDisplay(size_t itemIndex) const;

    /** ȷ������������ɼ�
    * @param [in] itemIndex �������������
    * @param [in] bToTop �Ƿ�ȷ�������Ϸ�
    */
    bool EnsureDataItemVisible(size_t itemIndex, bool bToTop);

public:
    /** �õ��ɼ���Χ�ڵ�һ��Ԫ�ص�����
    * @param [in] nScrollPosY ��ǰY��������λ��
    * @return ����Ԫ�ص�����
    */
    size_t GetTopDataItemIndex(int64_t nScrollPosY) const;

    /** ��ȡָ��Ԫ�صĸ߶ȣ��иߣ�
    * @param [in] itemIndex �������������
    */
    int32_t GetDataItemHeight(size_t itemIndex) const;

    struct ShowItemInfo
    {
        size_t nItemIndex;      //Ԫ������
        int32_t nItemHeight;    //Ԫ�صĸ߶�
    };

    /** ��ȡ��Ҫչʾ�����ݣ��ö������ȣ����Ұ�˳��
    @param [in] nScrollPosY ��ǰY��������λ��
    @param [in] maxCount ���ȡ��������¼(�����ö��ͷ��ö���)
    @param [out] itemIndexList ������Ҫչʾ��Ԫ�����(���ö���)
    @param [out] atTopItemIndexList ������Ҫչʾ��Ԫ�����(�ö���)
    @param [out] nPrevItemHeights ��һ���ɼ�Ԫ��֮ǰ����Ԫ�ص��ܸ߶ȣ������ö�Ԫ�أ�
    */
    void GetDataItemsToShow(int64_t nScrollPosY, size_t maxCount, 
                            std::vector<ShowItemInfo>& itemIndexList,
                            std::vector<ShowItemInfo>& atTopItemIndexList,
                            int64_t& nPrevItemHeights) const;

    /** ��ȡָ���߶ȵ�����������չʾ����������
    @param [in] nScrollPosY ��ǰY��������λ��
    @param [in] nRectHeight ����߶�
    @param [out] pItemIndexList ���ؿ�����ʾ��Ԫ�����
    @param [out] pAtTopItemIndexList �ö����Ԫ�����
    @return ���ؿ���չʾ����������
    */
    int32_t GetMaxDataItemsToShow(int64_t nScrollPosY, int32_t nRectHeight, 
                                  std::vector<size_t>* pItemIndexList = nullptr,
                                  std::vector<size_t>* pAtTopItemIndexList = nullptr) const;

    /** ��ȡָ��Ԫ�ص���ʾλ���ܸ߶�ֵ
    * @param [in] itemIndex �������������
    * @parma [in] bIncludeAtTops �Ƿ�����ö�Ԫ�صĸ߶�ֵ
    * @return ��ʾλ���ܸ߶�ֵ����������Ԫ������
    */
    int64_t GetDataItemTotalHeights(size_t itemIndex, bool bIncludeAtTops) const;

public:
    /** �Ƿ�Ϊ��׼ģʽ���и߶�ΪĬ���иߣ��������У����ö��У�
    */
    bool IsNormalMode() const;

    /** �����ö���UI�ؼ�������
    */
    void SetAtTopControlIndex(const std::vector<size_t>& atTopControlList);

    /** ����UI�ؼ���������ȷ���㹻��ʾ��Ӧ��ʾ������
    */
    void AjustItemCount();

protected:
    /** �����ӿؼ�
    */
    virtual void PaintChild(IRender* pRender, const UiRect& rcPaint) override;

    /** ִ����ˢ�²���, �����UI�ؼ��������ܻᷢ���仯
    */
    virtual void OnRefresh() override;

    /** ִ�������Ų����������UI�ؼ�����������������䣨ͨ��FillElement������
    */
    virtual void OnArrangeChild() override;

    /** �����ӿؼ�
    */
    virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, UiPoint scrollPos = UiPoint()) override;

    /** ���㱾ҳ������ʾ��������
    * @param [in] bIsHorizontal ��ǰ�����Ƿ�Ϊˮƽ����
    * @param [out] nColumns ��������
    * @param [out] nRows ��������
    * @return ���ؿ���������ʾ�ļ�¼��
    */
    virtual size_t GetDisplayItemCount(bool bIsHorizontal, size_t& nColumns, size_t& nRows) const override;

    /** �ж�һ������Ԫ���Ƿ�Ϊ��ѡ����
    * @param [in] nElementIndex Ԫ�������ţ���Ч��Χ��[0, GetElementCount())
    */
    virtual bool IsSelectableElement(size_t nElementIndex) const override;

    /** ��ȡ��һ����ѡ�������Ԫ��
    * @param [in] nElementIndex Ԫ�������ţ���Ч��Χ��[0, GetElementCount())
    * @param [in] bForward true��ʾ��ǰ���ң�false��ʾ������
    */
    virtual size_t FindSelectableElement(size_t nElementIndex, bool bForward) const override;

private:
    /** ��header���ö���������
    * @param [in,out] items ��Ҫ�ƶ��Ŀؼ��б�
    * @param [out] atTopItems �����ö��Ŀؼ��б�
    */
    void MoveTopItemsToLast(std::vector<Control*>& items, std::vector<Control*>& atTopItems) const;

private:
    /** ListCtrl �ؼ��ӿ�
    */
    ListCtrl* m_pListCtrl;

    /** ����Ԫ�ص�������(���ڻ�������)
    */
    size_t m_nTopElementIndex;

    /** ��ǰ�ɼ���Ԫ���б�
    */
    std::vector<size_t> m_diplayItemIndexList;

    /** �ö���UI�ؼ�������
    */
    std::vector<size_t> m_atTopControlList;
};

/** �б�������ʾ�ؼ��Ĳ��ֹ���ӿ�
*/
class ListCtrlDataLayout : public Layout, public VirtualLayout
{
public:
    ListCtrlDataLayout();

    /** ��������
    */
    virtual LayoutType GetLayoutType() const { return LayoutType::ListCtrlReportLayout; }

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
    virtual UiSize EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable) override;

public:
    /** �ӳټ���չʾ����
    * @param [in] rc ��ǰ������С��Ϣ, �ⲿ����ʱ����Ҫ�ȼ�ȥ�ڱ߾�
    */
    virtual void LazyArrangeChild(UiRect rc) const override;

    /** ��ȡ��Ҫչʾ����ʵ������������������Control�����Ӧ����ʵ�����
    * @param [in] rc ��ǰ������С��Ϣ, �ⲿ����ʱ����Ҫ�ȼ�ȥ�ڱ߾�
    */
    virtual size_t AjustMaxItem(UiRect rc) const override;

    /** �õ��ɼ���Χ�ڵ�һ��Ԫ�ص�ǰһ��Ԫ������
    * @param [in] rc ��ǰ��ʾ����ľ��Σ��������ڱ߾�
    * @return ����Ԫ�ص�����
    */
    virtual size_t GetTopElementIndex(UiRect rc) const override;

    /** �ж�ĳ��Ԫ���Ƿ��ڿɼ���Χ��
    * @param[in] iIndex Ԫ����������Χ�ǣ�[0, GetElementCount())
    * @param [in] rc ��ǰ��ʾ����ľ��Σ��������ڱ߾�
    * @return ���� true ��ʾ�ɼ�������Ϊ���ɼ�
    */
    virtual bool IsElementDisplay(UiRect rc, size_t iIndex) const override;

    /** �ж��Ƿ�Ҫ���²���
    */
    virtual bool NeedReArrange() const override;

    /** ��ȡ��ǰ���пɼ��ؼ�������Ԫ������
    * @param [in] rc ��ǰ��ʾ����ľ��Σ��������ڱ߾�
    * @param[out] collection �����б���Χ�ǣ�[0, GetElementCount())
    */
    virtual void GetDisplayElements(UiRect rc, std::vector<size_t>& collection) const override;

    /** �ÿؼ��ڿɼ���Χ��
    * @param [in] rc ��ǰ��ʾ����ľ��Σ��������ڱ߾�
    * @param[in] iIndex Ԫ�������ţ���Χ�ǣ�[0, GetElementCount())
    * @param[in] bToTop �Ƿ������Ϸ�
    */
    virtual void EnsureVisible(UiRect rc, size_t iIndex, bool bToTop) const override;

public:
    /** ���ù�����Box�ӿ�
    */
    void SetDataView(ListCtrlDataView* pDataView);

private:
    /** ��ȡ������Box�ӿ�
    */
    ListCtrlDataView* GetDataView() const { return m_pDataView; }

    /** ��ȡ������ĸ߶�, �߶Ȳ�������ͷ
    * @param [in] nCount ��������������ΪBox::InvalidIndex�����ȡ����������ĸ߶��ܺ�
    * * @parma [in] bIncludeAtTops �Ƿ�����ö�Ԫ�صĸ߶�ֵ
    * @return ���� nCount ��������ĸ߶��ܺ�, ��������ͷ�ĸ߶�
    */
    int64_t GetElementsHeight(size_t nCount, bool bIncludeAtTops) const;

    /** ��ȡ������ĸ߶ȺͿ��
    * @param [in] rcWidth ��ǰ�����Ŀ��, �ⲿ����ʱ����Ҫ�ȼ�ȥ�ڱ߾�
    * @param [in] nElementIndex ����Ԫ�ص�������
    * @return ��������Ԫ�صĸ߶ȺͿ��
    */
    UiSize GetElementSize(int32_t rcWidth, size_t nElementIndex) const;

    /** ��ȡ�п�
    */
    int32_t GetItemWidth() const;

    /** ��ȡ�и�(Ŀǰ��֧�������е��и߶���ȵ����)
    */
    int32_t GetItemHeight() const;

    /** ��ȡ��ͷ�ؼ��ĸ߶�
    */
    int32_t GetHeaderHeight() const;

    /** �ӳټ���չʾ����(����ģʽ���и���ͬ������������ö���)
    * @param [in] rc ��ǰ������С��Ϣ, �ⲿ����ʱ����Ҫ�ȼ�ȥ�ڱ߾�
    */
    void LazyArrangeChildNormal(UiRect rc) const;

    /** ��ȡ��ǰ���пɼ��ؼ�������Ԫ������
    * @param [in] rc ��ǰ��ʾ����ľ��Σ��������ڱ߾�
    * @param[out] collection �����б���Χ�ǣ�[0, GetElementCount())
    */
    void GetDisplayElements(UiRect rc, std::vector<size_t>& collection, 
                            std::vector<size_t>* pAtTopItemIndexList) const;

private:
    /** ������ListBox�ӿ�
    */
    ListCtrlDataView* m_pDataView;

    /** �ײ�Ԥ���Ŀռ䣬ȷ����������ײ���ʱ�����һ���������׿�����
    */
    int32_t m_nReserveHeight;

    /** �Ƿ������˵ײ�Ԥ���ռ�
    */
    bool m_bReserveSet;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_DATA_VIEW_H_
