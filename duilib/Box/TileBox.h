#ifndef UI_CORE_TILEBOX_H_
#define UI_CORE_TILEBOX_H_

#pragma once
#include "duilib/Core/Box.h"

namespace ui
{

class UILIB_API TileLayout : public Layout
{
public:
	TileLayout();

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual UiSize ArrangeChild(const std::vector<Control*>& items, UiRect rc) override;
	virtual UiSize AjustSizeByChild(const std::vector<Control*>& items, UiSize szAvailable) override;
	virtual bool SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/**
	 * @brief ��ȡ�����С
	 * @return ���������С
	 */
	const UiSize& GetItemSize() const;

	/**
	 * @brief ���������С
	 * @param[in] szItem �����С����
	 * @return ��
	 */
	void SetItemSize(UiSize szItem, bool bNeedDpiScale = true);

	/**
	 * @brief ��ȡ������
	 * @return ����������
	 */
	int GetColumns() const;

	/**
	 * @brief ������ʾ��������
	 * @param[in] nCols Ҫ������ʾ���е���ֵ
	 * @return ��
	 */
	void SetColumns(int nCols);

protected:

	//��ʾ��������
	int m_nColumns;

	//�����С
	UiSize m_szItem;
};

class UILIB_API TileBox : public Box
{
public:
	TileBox();

	virtual std::wstring GetType() const override;
};

}
#endif // UI_CORE_TILEBOX_H_
