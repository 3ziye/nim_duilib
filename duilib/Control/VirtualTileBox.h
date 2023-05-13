#pragma once

#include "duilib/Box/TileBox.h"
#include "duilib/Control/ListBox.h"
#include "base/callback/callback.h"
#include <functional>

namespace ui {

typedef std::function<void(size_t nStartIndex, size_t nEndIndex)> DataChangedNotify;
typedef std::function<void()> CountChangedNotify;

class UILIB_API VirtualTileBoxElement : public virtual nbase::SupportWeakCallback
{
public:
    VirtualTileBoxElement();
    /**
    * @brief ����һ������
    * @return ���ش����������ָ��
    */
    virtual ui::Control* CreateElement() = 0;

    /**
    * @brief ���ָ������
    * @param[in] control ����ؼ�ָ��
    * @param[in] index ����
    */
    virtual void FillElement(ui::Control* control, size_t index) = 0;

    /**
    * @brief ��ȡ��������
    * @return ������������
    */
    virtual size_t GetElementtCount() = 0;

    void RegNotifys(const DataChangedNotify& dcNotify, const CountChangedNotify& ccNotify);

protected:
    void EmitDataChanged(size_t nStartIndex, size_t nEndIndex);
    void EmitCountChanged();

private:
    DataChangedNotify m_DataChangedNotify;
    CountChangedNotify m_CountChangedNotify;
};

class UILIB_API VirtualTileLayout : public ui::TileLayout
{
public:
    VirtualTileLayout();
    virtual ui::UiSize ArrangeChild(const std::vector<ui::Control*>& items, ui::UiRect rc) override;
    virtual ui::UiSize AjustSizeByChild(const std::vector<ui::Control*>& items, ui::UiSize szAvailable) override;
    virtual bool SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
    virtual size_t GetElementsHeight(size_t nCount);
    virtual void LazyArrangeChild();
    virtual size_t AjustMaxItem();

private:
    bool m_bAutoCalcColumn;
};

class UILIB_API VirtualTileBox : public ui::ListBox
{
    friend class VirtualTileLayout;
public:
    explicit VirtualTileBox(ui::Layout* pLayout = new VirtualTileLayout);

    /**
    * @brief �������ݴ������
    * @param[in] pProvider ��������Ҫ��д VirtualListBoxElement �Ľӿ�����Ϊ���ݴ������
    * @return ��
    */
    virtual void SetDataProvider(VirtualTileBoxElement* pProvider);

    virtual VirtualTileBoxElement* GetDataProvider();

    /**
    * @brief ˢ���б�
    * @return ��
    */
    virtual void Refresh();

    /**
    * @brief ɾ����������
    * @return ��
    */
    virtual void RemoveAllItems() override;

    /**
    * @brief �����Ƿ�ǿ�����²���
    * @param[in] bForce ����Ϊ true Ϊǿ�ƣ�����Ϊ��ǿ��
    * @return ��
    */
    void SetForceArrange(bool bForce);

    /**
    * @brief ��ȡ��ǰ���пɼ��ؼ�������
    * @param[out] collection �����б�
    * @return ��
    */
    void GetDisplayCollection(std::vector<size_t>& collection);

    /**
    * @brief �ÿؼ��ڿɼ���Χ��
    * @param[in] iIndex �ؼ�����
    * @param[in] bToTop �Ƿ������Ϸ�
    * @return ��
    */
    void EnsureVisible(size_t iIndex, bool bToTop = false);

    /**
    * @brief ��ȡ�б��������ʵ�ؼ�����
    * @return �����ʵ�ؼ�����
    */
    size_t GetMaxItemCount() { return m_nMaxItemCount; }

protected:
    /// ��д����ӿڣ��ṩ���Ի�����
    virtual void SetScrollPos(ui::UiSize szPos) override;
    virtual void HandleEvent(const ui::EventArgs& event) override;
    virtual void SetPos(ui::UiRect rc) override;

    /**
    * @brief ���²�������
    * @param[in] bForce �Ƿ�ǿ�����²���
    * @return ��
    */
    void ReArrangeChild(bool bForce);

    /**
    * @brief ���̰���֪ͨ
    * @param[in] ch ����
    * @return ��
    */
    virtual void OnKeyDown(TCHAR ch) { (void)ch; }

    /**
    * @brief ���̵���֪ͨ
    * @param[in] ch ����
    * @return ��
    */
    virtual void OnKeyUp(TCHAR ch) { (void)ch; }

private:
    enum class ScrollDirection
    {
        kScrollUp = -1,
        kScrollDown = 1
    };

    /**
    * @brief ����һ������
    * @return ���ش����������ָ��
    */
    Control* CreateElement();

    /**
    * @brief ���ָ������
    * @param[in] control ����ؼ�ָ��
    * @param[in] index ����
    * @return ���ش����������ָ��
    */
    void FillElement(Control* pControl, size_t iIndex);

    /**
    * @brief ��ȡԪ������
    * @return ����Ԫ����ָ��
    */
    size_t GetElementCount();

    /**
    * @brief �õ�n��Ԫ�ض�Ӧ�ĸ߶Ⱥͣ�
    * @param[in] nCount Ҫ�õ�����Ԫ�صĸ߶ȣ�-1��ʾȫ��Ԫ��
    * @return ����ָ������Ԫ�صĸ߶Ⱥ�
    */
    size_t CalcElementsHeight(size_t nCount);

    /**
    * @brief �õ��ɼ���Χ�ڵ�һ��Ԫ�ص�ǰһ��Ԫ������
    * @param[out] bottom ������һ��Ԫ�ص� bottom ֵ
    * @return ������һ��Ԫ�ص�����
    */
    size_t GetTopElementIndex(size_t& bottom);

    /**
    * @brief �ж�ĳ��Ԫ���Ƿ��ڿɼ���Χ��
    * @param[in] iIndex Ԫ������
    * @return ���� true ��ʾ�ɼ�������Ϊ���ɼ�
    */
    bool IsElementDisplay(size_t iIndex);

    /**
    * @brief �ж��Ƿ�Ҫ���²���
    * @param[out] direction ���Ϲ����������¹���
    * @return true Ϊ��Ҫ���²��֣�����Ϊ false
    */
    bool NeedReArrange(ScrollDirection& direction);

    VirtualTileLayout* GetTileLayout();

    size_t GetRealElementHeight();

    size_t GetColumns();

    void LazyArrangeChild();

    void OnModelDataChanged(size_t nStartIndex, size_t nEndIndex);

    void OnModelCountChanged();

    size_t ElementIndexToItemIndex(size_t nElementIndex);

    size_t ItemIndexToElementIndex(size_t nItemIndex);

private:
    VirtualTileBoxElement* m_pDataProvider;
    size_t m_nMaxItemCount;	// �б���ʵ�ؼ���������  
    size_t m_nOldYScrollPos;
    bool m_bArrangedOnce;
    bool m_bForceArrange;	// ǿ�Ʋ��ֱ��
};

}
