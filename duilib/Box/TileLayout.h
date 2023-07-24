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

	/** �����ڲ����пؼ���λ����Ϣ
	 * @param[in] items �ؼ��б�
	 * @param[in] rc ��ǰ����λ����Ϣ, �ⲿ����ʱ������Ҫ�۳��ڱ߾�
	 * @return �������к����պ��ӵĿ�Ⱥ͸߶���Ϣ
	 */
	virtual UiSize64 ArrangeChild(const std::vector<Control*>& items, UiRect rc) override;

	/** �����ڲ��ӿؼ���С�������������С���������͵��ӿؼ������ԣ��������С����
	 * @param[in] items �ӿؼ��б�
	 * @param[in] szAvailable �ӿؼ����������Ⱥ͸߶�
	 * @return �������к����ղ��ֵĴ�С��Ϣ����Ⱥ͸߶ȣ����������������͵��ӿؼ���С
	 */
	virtual UiSize EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable) override;

	/** ���ò�������
	 * @param[in] strName Ҫ���õ�������
	 * @param[in] strValue Ҫ���õ�����ֵ
	 * @return true ���óɹ���false ���Բ�����
	 */
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

	/** �����Ƿ��Զ���������
	 */
	void SetAutoCalcColumns(bool bAutoCalcColumns);

	/** ���ؼ����ݳ����߽�ʱ����������С����ʹ�ؼ�������ȫ��ʾ����Ƭ������
	*/
	void SetScaleDown(bool bScaleDown);

private:
	/** ��ȡ�����Сʱ�Ŀ��ÿ��
	* @param [in] pControl �ռ�ӿ�
	* @param [in] szItem ÿ����Ƭ�ؼ��Ŀ�Ⱥ͸߶�(����ֵ)
	* @param [in] rc ��Ƭ�ؼ����������Ŀ����������
	* @return ���ظÿռ�Ĺ����С����͸ߣ�
	*/
	static UiSize CalcEstimateSize(Control* pControl, const UiSize& szItem, UiRect rc);

	/** ��ȡ������������Ƭ������
	* @param [in] items �ӿؼ��б�
	* @param [in] rc ��Ƭ�ؼ����������ľ���
	* @param [in] itemWidth ÿ����Ƭ�ؼ��Ŀ��(����ֵ)
	* @param [in] childMarginX �ӿؼ���X����
	* @param [in] childMarginY �ӿؼ���Y����	
	* @param [out] columns ����������
	*/
	static void CalcTileColumns(const std::vector<Control*>& items, const UiRect& rc,
		                        int32_t itemWidth, int32_t childMarginX, int32_t childMarginY,
		                        int32_t& nColumns);

	/** ��ȡ������������Ƭ��Ⱥ͸߶ȵ����ֵ, ��Ϊ�������Ϳؼ��Ŀ��ֵ
	* @param [in] items �ӿؼ��б�
	* @param [in] rc ��Ƭ�ؼ����������ľ���
	* @param [in] childMarginX �ӿؼ���X����
	* @param [in] childMarginY �ӿؼ���Y����	
	* @param [in] columns ������
	* @param [out] szMaxItem ������Ƭ�ؼ��Ŀ�Ⱥ͸߶����ֵ����������߾�Margin.left + Margin.rightֵ
	*/
	static void CalcStretchTileSize(const std::vector<Control*>& items, UiRect rc,
		                            int32_t childMarginX, int32_t childMarginY,
		                            int32_t nColumns, UiSize& szMaxItem);

	/** ���㸡���ؼ��Ĵ�С
	*/
	static UiSize64 EstimateFloatSize(Control* pControl, UiRect rc);

	/** ��ȡ������������Ƭ�߶ȣ��������й����У���ÿ�п�ʼʱ�����㱾�еĸ߶�
	* @param [in] items �ӿؼ��б�
	* @param [in] iterBegin �ӿؼ���ʼ�ĵ�����
	* @param [in] nColumns ������
	* @param [in] szItem ��Ƭ�ؼ���Ⱥ͸߶ȣ�����ֵ��
	* @param [in] rc ��Ƭ�ؼ����������ľ���
	* @return ���ظ߶�ֵ����������߾�Margin.top + Margin.bottomֵ
	*/
	static int32_t CalcTileLineHeight(const std::vector<Control*>& items,
									  const std::vector<Control*>::const_iterator iterBegin,
								 	  int32_t nColumns,
								      const UiSize& szItem,
									  const UiRect& rc);

	/** ������Ƭ�ؼ�����ʾ����ʹ�С
	* @param [in] pControl ��Ƭ�ؼ��Ľӿ�
	* @param [in] itemWidth ���õ���Ƭ�ؼ����
	* @param [in] itemHeight ���õ���Ƭ�ؼ��߶ȣ�ȡ�иߣ�
	* @param [in] szStretchItem �������͵���Ƭ�ؼ���Ⱥ͸߶�
	* @param [in] rcContainer �ⲿ���þ��δ�С
	* @param [in] ptTile ��ǰ��Ƭ�ؼ����Ͻǵ�����
	* @param [in] bScaleDown ���ؼ����ݳ����߽�ʱ����������С����ʹ�ؼ�������ȫ��ʾ����Ƭ������
	* @param [out] szTilePos ��Ƭ�ؼ�����ʾ���ꡢ��Ⱥ͸߶�
	* @return ������Ƭ�ؼ�Ŀ������Ĵ�С����͸ߣ�
	*/
	static UiSize CalcTilePosition(Control* pControl,
								   int32_t itemWidth,
								   int32_t itemHeight,
								   const UiSize& szStretchItem, 
								   const UiRect& rcContainer,
								   const UiPoint& ptTile,
		                           bool bScaleDown, 
								   UiRect& szTilePos);


	/** ���ӿؼ����ֵ��ڲ�ʵ�ֺ���
	* @param [in] items �ӿؼ��б�
	* @param [in] rect �ⲿ���þ��δ�С
	* @param [in] isCalcOnly ���Ϊtrue��ʾ���������򣬶Կؼ�λ�ò������������Ϊfalse����ʾ�Կؼ�λ����������
	* @param [in] inColumnWidths ÿ�еĿ��ֵ������Ϊ��
	* @param [out] outColumnWidths ���β��֣�ʹ�õ�ÿ�п��ֵ
	* @return ��������Ŀ�Ⱥ͸߶�
	*/
	UiSize64 ArrangeChildNormal(const std::vector<Control*>& items, 
								UiRect rect,
							    bool isCalcOnly,
		                        const std::vector<int32_t>& inColumnWidths,
		                        std::vector<int32_t>& outColumnWidths) const;

	/** ʹ�����ɲ������пؼ�(�޹̶������������������չʾ�ռ䣬��ʾ�����ܶ������)
	* @param [in] items �ӿؼ��б�
	* @param [in] rect �ⲿ���þ��δ�С
	* @param [in] isCalcOnly ���Ϊtrue��ʾ���������򣬶Կؼ�λ�ò������������Ϊfalse����ʾ�Կؼ�λ����������
	* @return ��������Ŀ�Ⱥ͸߶�
	*/
	UiSize64 ArrangeChildFreeLayout(const std::vector<Control*>& items, 
									UiRect rect, 
									bool isCalcOnly) const;

	/** ��ǰ�Ƿ�Ϊ���ɲ���
	*/
	bool IsFreeLayout() const;

private:

	//��ʾ��������
	int32_t m_nColumns;

	//�����С
	UiSize m_szItem;

	//�Զ���������
	bool m_bAutoCalcColumns;

	//���ؼ����ݳ����߽�ʱ����������С����ʹ�ؼ�������ȫ��ʾ����Ƭ������
	bool m_bScaleDown;
};

} // namespace ui

#endif // UI_BOX_TILELAYOUT_H_
