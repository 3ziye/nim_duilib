// Copyright 2017, NetEase (Hangzhou) Network Co., Ltd. All rights reserved.
//
// zqw
// 2015/7/22
//
// virtual ListBox class
#pragma once

#include "duilib/Box/VBox.h"
#include "duilib/Box/HBox.h"
#include "duilib/Box/ListBox.h"

namespace ui
{

/**
 * @brief �����б�ӿ���
 * �ṩ�����߶��������ݹ�����Ի�����
 */
class VirtualListBoxElement
{
public:
	/**@brief ����һ������
	 * @return ���ش����������ָ��
	 */
	virtual Control* CreateElement() = 0;

	/**@brief ���ָ������
	 * @param[in] control ����ؼ�ָ��
	 * @param[in] index ����
	 * @return ���ش����������ָ��
	 */
	virtual void FillElement(Control* control, size_t index) = 0;

	/**@brief ��ȡ��������
	 * @return ������������
	 */
	virtual size_t GetElementCount() = 0;
};

/**
 * @brief �����б�
 */
class UILIB_API VirtualVLayout : public VLayout
{
protected:
	virtual ui::UiSize64 ArrangeChild(const std::vector<ui::Control*>& items, ui::UiRect rc);
};

class UILIB_API VirtualHLayout : public HLayout
{
protected:
	virtual ui::UiSize64 ArrangeChild(const std::vector<ui::Control*>& items, ui::UiRect rc);
};

/** ���ʵ�ֵ�ListBox����ֱ����
*/
class UILIB_API VirtualListBox : public ListBox
{
	friend class VirtualVLayout;
	friend class VirtualHLayout;
public:
	explicit VirtualListBox(ui::Layout* pLayout = new VirtualVLayout);

	virtual std::wstring GetType() const override;

	/**@brief �������ݴ������
	 * @param[in] pProvider ��������Ҫ��д VirtualListBoxElement �Ľӿ�����Ϊ���ݴ������
	 */
	virtual void SetDataProvider(VirtualListBoxElement* pProvider);

	/**@brief ��������߶�
	 * @param[in] nHeight �߶�ֵ
	 */
	virtual void SetElementHeight(size_t nHeight);

	/**@brief ��ʼ������
	 * @param[in] nMaxItemCount ��ʼ��������Ĭ�� 50
	 */
	virtual void InitElement(size_t nMaxItemCount = 50);

	/**@brief �����б���ݺ᷽��
	 * @param[in] direction ����
	 */
	enum class ListDirection
	{
		kListHorizontal = 0, //����
		kListVertical   = 1  //����
	};
	void SetDirection(ListDirection direction);

	/**@brief ˢ���б�
	 */
	virtual void Refresh();

	/**@brief ɾ����������
	 */
	virtual void RemoveAllItems() override;

	/**@brief �����Ƿ�ǿ�����²���
	 * @param[in] bForce ����Ϊ true Ϊǿ�ƣ�����Ϊ��ǿ��
	 */
	void SetForceArrange(bool bForce);

	/**@brief ��ȡ��ǰ���пɼ��ؼ�������
	 * @param[out] collection �����б�
	 */
    void GetDisplayCollection(std::vector<size_t>& collection);

	/**@brief �ÿؼ��ڿɼ���Χ��
	 * @param[in] iIndex �ؼ�����
	 * @param[in] bToTop �Ƿ������Ϸ�
	 */
    void EnsureVisible(size_t iIndex, bool bToTop = false);

protected:
	/// ��д����ӿڣ��ṩ���Ի�����
	virtual void SetScrollPos(ui::UiSize64 szPos) override;
	virtual void HandleEvent(const ui::EventArgs& event) override;
	virtual void SetPos(UiRect rc) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/**@brief ���²�������
	 * @param[in] bForce �Ƿ�ǿ�����²���
	 */
	void ReArrangeChild(bool bForce);

	/**@brief ���һ������
	 * @param[in] iIndex Ҫ�����λ��
	 */
	void AddElement(size_t iIndex);

	/**@brief �Ƴ�һ������
	 * @param[in] iIndex Ҫ�Ƴ�����������
	 */
	void RemoveElement(size_t iIndex);

	/**@brief ���̰���֪ͨ
	 * @param[in] ch ����
	 */
	virtual void OnKeyDown(TCHAR ch) { (void)ch; }

	/**
	 * @brief ���̵���֪ͨ
	 * @param[in] ch ����
	 * @return ��
	 */
	virtual void OnKeyUp(TCHAR ch) { (void)ch; }

private:
	enum ScrollDirection
	{
		kScrollUp = -1,
		kScrollDown = 1
	};

	/**@brief ����һ������
	 */
	Control* CreateElement();

	/**@brief ���ָ������
	 * @param[in] control ����ؼ�ָ��
	 * @param[in] index ����
	 */
	void FillElement(Control *pControl, size_t iIndex);

	/**@brief ��ȡԪ������
	 */
	size_t GetElementCount();

	/**@brief ʹ��Ĭ�ϲ���
	 */
	bool UseDefaultLayout();

	/**@brief �õ�n��Ԫ�ض�Ӧ�ĸ߶Ⱥͣ�
	 * @param[in] nCount Ҫ�õ�����Ԫ�صĸ߶ȣ�-1��ʾȫ��Ԫ��
	 * @return ����ָ������Ԫ�صĸ߶Ⱥ�
	 */
    int64_t CalcElementsHeight(size_t nCount);

	/**@brief �õ��ɼ���Χ�ڵ�һ��Ԫ�ص�ǰһ��Ԫ������
	 * @param[out] bottom ������һ��Ԫ�ص� bottom ֵ
	 * @return ������һ��Ԫ�ص�����
	 */
	size_t GetTopElementIndex(int64_t* bottom);

	/**@brief �ж�ĳ��Ԫ���Ƿ��ڿɼ���Χ��
	 * @param[in] iIndex Ԫ������
	 * @return ���� true ��ʾ�ɼ�������Ϊ���ɼ�
	 */
    bool IsElementDisplay(size_t iIndex);

    /**@brief �ж��Ƿ�Ҫ���²���
	 * @param[out] direction ���Ϲ����������¹���
	 * @return true Ϊ��Ҫ���²��֣�����Ϊ false
     */
	bool NeedReArrange(ScrollDirection& direction);

private:

	//�����ṩ�߽ӿ�
	VirtualListBoxElement* m_pDataProvider;

	// ÿ��������б���ĳ���(����Ϊ���ȣ�����Ϊ�߶�)
	size_t m_nElementHeight;

	// �б���ʵ�ؼ���������  
	size_t m_nMaxItemCount;

	//������λ��
	int64_t m_nOldScrollPos;

	//�������в��ֱ�־
	bool m_bArrangedOnce;

	// ǿ�Ʋ��ֱ��
	bool m_bForceArrange;

	//�б���ݺ᷽��
	ListDirection m_eDirection = ListDirection::kListVertical;
};

}