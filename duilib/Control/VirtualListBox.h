// Copyright 2017, NetEase (Hangzhou) Network Co., Ltd. All rights reserved.
//
// zqw
// 2015/7/22
//
// virtual ListBox class
#pragma once

namespace ui
{

/**
 * @brief �����б�ӿ���
 * �ṩ�����߶��������ݹ�����Ի�����
 */
class VirtualListInterface
{
public:
	/**
	 * @brief ����һ������
	 * @return ���ش����������ָ��
	 */
	virtual Control* CreateElement() = 0;

	/**
	 * @brief ���ָ������
	 * @param[in] control ����ؼ�ָ��
	 * @param[in] index ����
	 * @return ���ش����������ָ��
	 */
	virtual void FillElement(Control *control, int index) = 0;

	/**
	 * @brief ��ȡ��������
	 * @return ������������
	 */
	virtual int GetElementCount() = 0;
};

/**
 * @brief �����б�
 */
class UILIB_API VirtualVLayout : public VLayout
{
protected:
	virtual ui::CSize ArrangeChild(const std::vector<ui::Control*>& items, ui::UiRect rc);
};

class UILIB_API VirtualHLayout : public HLayout
{
protected:
	virtual ui::CSize ArrangeChild(const std::vector<ui::Control*>& items, ui::UiRect rc);
};

class UILIB_API VirtualListBox : public ListBox
{
	friend class VirtualVLayout;
	friend class VirtualHLayout;
public:
	VirtualListBox(ui::Layout* pLayout = new VirtualVLayout);

	virtual std::wstring GetType() const override;

	/**
	 * @brief �������ݴ������
	 * @param[in] pProvider ��������Ҫ��д VirtualListInterface �Ľӿ�����Ϊ���ݴ������
	 * @return ��
	 */
	virtual void SetDataProvider(VirtualListInterface *pProvider);

	/**
	 * @brief ��������߶�
	 * @param[in] nHeight �߶�ֵ
	 * @return ��
	 */
	virtual void SetElementHeight(int nHeight);

	/**
	 * @brief ��ʼ������
	 * @param[in] nMaxItemCount ��ʼ��������Ĭ�� 50
	 * @return ��
	 */
	virtual void InitElement(int nMaxItemCount = 50);

	/**
	 * @brief �����б���ݺ᷽��
	 * @param[in] direction ����
	 * @return ��
	 */
	enum ListDirection
	{
		kListHorizontal = 0,
		kListVertical = 1
	};
	void SetDirection(ListDirection direction);

	/**
	 * @brief ˢ���б�
	 * @return ��
	 */
	virtual void Refresh();

	/**
	 * @brief ɾ����������
	 * @return ��
	 */
	virtual void RemoveAll() override;

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
    void GetDisplayCollection(std::vector<int>& collection);

	/**
	 * @brief �ÿؼ��ڿɼ���Χ��
	 * @param[in] iIndex �ؼ�����
	 * @param[in] bToTop �Ƿ������Ϸ�
	 * @return ��
	 */
    void EnsureVisible(int iIndex, bool bToTop = false);

protected:
	/// ��д����ӿڣ��ṩ���Ի�����
	virtual void SetScrollPos(ui::CSize szPos) override;
	virtual void HandleMessage(ui::EventArgs& event) override;
	virtual void SetPos(UiRect rc) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/**
	 * @brief ���²�������
	 * @param[in] bForce �Ƿ�ǿ�����²���
	 * @return ��
	 */
	void ReArrangeChild(bool bForce);

	/**
	 * @brief ���һ������
	 * @param[in] iIndex Ҫ�����λ��
	 * @return ��
	 */
	void AddElement(int iIndex);

	/**
	 * @brief �Ƴ�һ������
	 * @param[in] iIndex Ҫ�Ƴ�����������
	 * @return ��
	 */
	void RemoveElement(int iIndex);

	/**
	 * @brief ���̰���֪ͨ
	 * @param[in] ch ����
	 * @return ��
	 */
	virtual void OnKeyDown(TCHAR ch) {}

	/**
	 * @brief ���̵���֪ͨ
	 * @param[in] ch ����
	 * @return ��
	 */
	virtual void OnKeyUp(TCHAR ch) {}

private:
	enum ScrollDirection
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
	void FillElement(Control *pControl, int iIndex);

	/**
	 * @brief ��ȡԪ������
	 * @return ����Ԫ����ָ��
	 */
	int GetElementCount();

	/**
	 * @brief ʹ��Ĭ�ϲ���
	 * @return �ɹ����� true�����򷵻� false
	 */
	bool UseDefaultLayout();

	/**
	 * @brief �õ�n��Ԫ�ض�Ӧ�ĸ߶Ⱥͣ�
	 * @param[in] nCount Ҫ�õ�����Ԫ�صĸ߶ȣ�-1��ʾȫ��Ԫ��
	 * @return ����ָ������Ԫ�صĸ߶Ⱥ�
	 */
    int CalcElementsHeight(int nCount);

	/**
	 * @brief �õ��ɼ���Χ�ڵ�һ��Ԫ�ص�ǰһ��Ԫ������
	 * @param[out] bottom ������һ��Ԫ�ص� bottom ֵ
	 * @return ������һ��Ԫ�ص�����
	 */
	int GetTopElementIndex(int &bottom);

	/**
	 * @brief �ж�ĳ��Ԫ���Ƿ��ڿɼ���Χ��
	 * @param[in] iIndex Ԫ������
	 * @return ���� true ��ʾ�ɼ�������Ϊ���ɼ�
	 */
    bool IsElementDisplay(int iIndex);

    /**
     * @brief �ж��Ƿ�Ҫ���²���
	 * @param[out] direction ���Ϲ����������¹���
	 * @return true Ϊ��Ҫ���²��֣�����Ϊ false
     */
	bool NeedReArrange(ScrollDirection &direction);

private:
	VirtualListInterface *m_pDataProvider;
	int m_nElementHeight;	// ÿ��������б���ĳ���
	int m_nMaxItemCount;	// �б���ʵ�ؼ���������  
	int m_nOldYScrollPos;
	bool m_bArrangedOnce;
	bool m_bForceArrange;	// ǿ�Ʋ��ֱ��

	ListDirection m_eDirection = kListVertical;
};

}