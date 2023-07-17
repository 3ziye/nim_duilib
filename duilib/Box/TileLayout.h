#ifndef UI_BOX_TILELAYOUT_H_
#define UI_BOX_TILELAYOUT_H_

#pragma once

#include "duilib/Box/Layout.h"

namespace ui 
{

class UILIB_API TileLayout : public Layout
{
public:
	TileLayout();

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual UiSize64 ArrangeChild(const std::vector<Control*>& items, UiRect rc) override;
	virtual UiSize EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable) override;
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

	/* @brief ��ȡ������
	 */
	int GetColumns() const;

	/**@brief ������ʾ��������
	 * @param[in] nCols Ҫ������ʾ���е���ֵ
	 */
	void SetColumns(int nCols);

protected:

	//��ʾ��������
	int m_nColumns;

	//�����С
	UiSize m_szItem;
};

} // namespace ui

#endif // UI_BOX_TILELAYOUT_H_
