#ifndef UI_BOX_TILELAYOUT_H_
#define UI_BOX_TILELAYOUT_H_

#pragma once

#include "duilib/Box/Layout.h"

namespace ui 
{

/** ��Ƭ����
*/
class UILIB_API TileLayout : public Layout
{
public:
	TileLayout();

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual UiSize64 ArrangeChild(const std::vector<Control*>& items, UiRect rc) override;
	virtual UiSize EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable) override;
	virtual bool SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/** ��ȡ�����С
	 */
	const UiSize& GetItemSize() const;

	/** ���������С
	 * @param[in] szItem �����С����
	 */
	void SetItemSize(UiSize szItem, bool bNeedDpiScale = true);

	/** ��ȡ������
	 */
	int32_t GetColumns() const;

	/** ������ʾ��������
	 * @param[in] nCols Ҫ������ʾ���е���ֵ
	 */
	void SetColumns(int32_t nCols);

private:
	/** ��ȡ������������Ƭ��Ⱥ������������������ڲ�������ǰ��ȷ�����ǹ̶�ֵ
	* @param [in] items �ӿؼ��б�
	* @param [in] szAvailable ��������Ŀ�Ⱥ͸߶�
	* @param [out] itemWidth ����ÿ����Ƭ�ؼ��Ŀ�ȣ���������߾�Margin.left + Margin.rightֵ
	* @param [out] columns ����������
	*/
	void CalcTileWidthAndColumns(const std::vector<Control*>& items, UiSize szAvailable,
		                        int32_t& itemWidth, int32_t& columns) const;

	/** ��ȡ������������Ƭ�߶ȣ��������й����У���ÿ�п�ʼʱ�����㱾�еĸ߶�
	* @param [in] items �ӿؼ��б�
	* @param [in] iterBegin �ӿؼ���ʼ�ĵ�����
	* @param [in] nColumns ������
	* @param [in] tileWidth ��Ƭ�ؼ����
	* @param [in] tileBoxHeight ��Ƭ�ؼ������������ܸ߶�
	* @return ���ظ߶�ֵ����������߾�Margin.top + Margin.bottomֵ
	*/
	int32_t CalcTileHeight(const std::vector<Control*>& items,
		                   const std::vector<Control*>::const_iterator iterBegin, 
		                   int32_t nColumns,
		                   int32_t tileWidth,
		                   int32_t tileBoxHeight);

private:

	//��ʾ��������
	int32_t m_nColumns;

	//�����С
	UiSize m_szItem;
};

} // namespace ui

#endif // UI_BOX_TILELAYOUT_H_
