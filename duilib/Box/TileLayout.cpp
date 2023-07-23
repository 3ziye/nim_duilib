#include "TileLayout.h"
#include "duilib/Core/Box.h"
#include "duilib/Utils/AttributeUtil.h"
#include "duilib/Core/GlobalManager.h"
#include <numeric>

namespace ui 
{
TileLayout::TileLayout(): 
	m_nColumns(0), 
	m_szItem(0, 0),
	m_bAutoCalcColumns(false)
{
}

UiSize TileLayout::CalcEstimateSize(Control* pControl, const UiSize& szItem, UiRect rc) const
{
	if ((pControl == nullptr) || !pControl->IsVisible()) {
		return UiSize();
	}

	UiSize szAvailable;
	UiMargin rcMargin = pControl->GetMargin();
	int32_t width = szItem.cx;
	if (width == 0) {
		width = rc.Width();
	}
	width = width - rcMargin.left - rcMargin.right;
	int32_t height = szItem.cy;
	if (height == 0) {
		height = rc.Height();
	}
	height = height - rcMargin.top - rcMargin.bottom;
	szAvailable = UiSize(width, height);
	szAvailable.Validate();

	UiEstSize estSize = pControl->EstimateSize(szAvailable);
	UiSize childSize(estSize.cx.GetInt32(), estSize.cy.GetInt32());
	if (estSize.cy.IsStretch()) {
		childSize.cy = 0;
	}
	if (childSize.cy < pControl->GetMinHeight()) {
		childSize.cy = pControl->GetMinHeight();
	}
	if (childSize.cy > pControl->GetMaxHeight()) {
		childSize.cy = pControl->GetMaxHeight();
	}

	if (estSize.cx.IsStretch()) {
		childSize.cx = 0;
	}
	if (childSize.cx < pControl->GetMinWidth()) {
		childSize.cx = pControl->GetMinWidth();
	}
	if (childSize.cx > pControl->GetMaxWidth()) {
		childSize.cx = pControl->GetMaxWidth();
	}
	return childSize;
}

void TileLayout::CalcTileWidthAndColumns(const std::vector<Control*>& items, UiRect rc,
										 UiSize& szItem, int32_t& nColumns) const
{
	szItem = m_szItem;		//��Ƭ�ؼ���Ⱥ͸߶ȣ�����ֵ��
	szItem.Validate();
	nColumns = m_nColumns;  //����������ֵ��, ��������Զ�����������������ʹ�����õ�����
	if (nColumns < 0) {
		nColumns = 0;
	}
	if (!m_bAutoCalcColumns && (nColumns == 0) && (szItem.cx == 0)) {
		//���ɲ���ģʽ
		return;
	}
	if (m_bAutoCalcColumns) {
		//����Զ����������������¼�������
		nColumns = 0;
	}
	
	if ((szItem.cx == 0) && (nColumns > 0)) {
		//�Ѿ�ָ����������������������Ƭ�ؼ��Ŀ��
		szItem.cx = (rc.Width() - (nColumns - 1) * GetChildMarginX()) / nColumns;
		if (szItem.cx <= 0) {
			szItem.cx = rc.Width() / nColumns;
		}
	}
	if ((szItem.cx == 0) && (nColumns == 0)) {
		//��Ҫ�ȼ�����Ƭ�ؼ���ȣ�Ȼ����ݿ�ȼ�������
		int32_t maxWidth = 0;
		int64_t areaTotal = 0;
		const int64_t maxArea = (int64_t)rc.Width() * rc.Height();
		for (Control* pChild : items) {
			if ((pChild == nullptr) || !pChild->IsVisible() || pChild->IsFloat()) {
				continue;
			}
			UiMargin rcMargin = pChild->GetMargin();
			UiSize childSize = CalcEstimateSize(pChild, UiSize(0, 0), rc);
			if (childSize.cx > 0) {
				maxWidth = std::max(maxWidth, childSize.cx + rcMargin.left + rcMargin.right);
			}
			int32_t childWidth = childSize.cx;
			int32_t childHeight = childSize.cy;
			if (childWidth > 0) {
				childWidth += GetChildMarginX();
			}			
			if (childHeight > 0) {
				childHeight += GetChildMarginY();
			}
			areaTotal += ((int64_t)childWidth * childHeight);
			if (areaTotal > maxArea) {
				//����ǰ������չʾ���������
				break;
			}
		}
		//ȡ��һ�еĿ�����ֵ����Ϊÿ��Item�Ŀ��
		szItem.cx = maxWidth;		
	}
	if ((nColumns == 0) && (szItem.cx > 0)) {
		//ʹ�����õĿ����Ϊ��Ƭ�ؼ��Ŀ�ȣ���ͨ�����õĿ�ȣ���������
		int32_t totalWidth = rc.Width();
		while (totalWidth > 0) {
			totalWidth -= szItem.cx;
			if (nColumns != 0) {
				totalWidth -= GetChildMarginX();
			}
			if (totalWidth >= 0) {
				++nColumns;
			}
		}		
	}
	if (szItem.cx == 0) {
		//���ӿؼ�ȫ��������������ʱ����Ȼ�ò�����Чֵ����ʱ��Ҫ���̶�ֵ
		szItem.cx = rc.Width();
		nColumns = 1;
	}
	if (nColumns < 1) {
		nColumns = 1;
	}
}

void TileLayout::CalcStretchTileSize(const std::vector<Control*>& items, UiRect rc,
								     int32_t nColumns, UiSize& szMaxItem) const 
{
	ASSERT(nColumns > 0);
	int32_t maxWidth = 0;  //�����
	int32_t maxHeight = 0; //���߶�
	const int64_t maxArea = (int64_t)rc.Width() * rc.Height();
	int64_t calcArea = 0;
	for (Control* pChild : items) {
		if ((pChild == nullptr) || !pChild->IsVisible() || pChild->IsFloat()) {
			continue;
		}
		UiMargin rcMargin = pChild->GetMargin();
		UiSize childSize = CalcEstimateSize(pChild, UiSize(0, 0), rc);
		if (childSize.cx > 0) {
			maxWidth = std::max(maxWidth, childSize.cx + rcMargin.left + rcMargin.right);
		}
		if (childSize.cy > 0) {
			maxHeight = std::max(maxHeight, childSize.cy + rcMargin.top + rcMargin.bottom);
		}

		int32_t childWidth = childSize.cx;
		if (childWidth > 0) {
			childWidth += GetChildMarginX();
		}
		
		int32_t childHeight = childSize.cy;
		if (childHeight > 0) {
			childHeight += GetChildMarginY();
		}
		calcArea += ((int64_t)childWidth * childHeight);
		if (calcArea > maxArea) {
			//��ǰ��ʾ�����������ٲ���
			break;
		}
	}
	//ȡ��һ�еĿ�����ֵ����Ϊÿ��Item�Ŀ��
	szMaxItem.cx = maxWidth;
	szMaxItem.cy = maxHeight;
	if ((nColumns > 0) && (szMaxItem.cx == 0)) {
		szMaxItem.cx = (rc.Width() - (nColumns - 1) * GetChildMarginX()) / nColumns;
		if (szMaxItem.cx <= 0) {
			szMaxItem.cx = rc.Width() / nColumns;
		}
	}
	if ((szMaxItem.cx > 0) && (szMaxItem.cy == 0)) {
		szMaxItem.cy = szMaxItem.cx;
	}
}

UiSize64 TileLayout::EstimateFloatSize(Control* pControl, UiRect rc) const
{
	ASSERT(pControl != nullptr);
	if ((pControl == nullptr) || !pControl->IsVisible()) {
		return UiSize64();
	}
	UiMargin margin = pControl->GetMargin();
	UiSize childSize = CalcEstimateSize(pControl, UiSize(0, 0), rc);
	if (childSize.cx > 0) {
		childSize.cx += (margin.left + margin.right);
	}
	if (childSize.cy > 0) {
		childSize.cy += (margin.top + margin.bottom);
	}	
	return UiSize64(childSize.cx, childSize.cy);
}

int32_t TileLayout::CalcTileLineHeight(const std::vector<Control*>& items,
									   const std::vector<Control*>::const_iterator iterBegin,
									   int32_t nColumns,
									   const UiSize& szItem,
									   const UiRect& rc) const
{
	ASSERT((szItem.cx > 0) && (nColumns > 0));
	int32_t cyHeight = szItem.cy;
	if (cyHeight > 0) {
		//��������˸߶ȣ�������ʹ�����õĸ߶�ֵ
		return cyHeight;
	}
	if (nColumns == 0) {
		return 0;
	}

	int32_t iIndex = 0;
	for (auto it = iterBegin; it != items.end(); ++it) {
		auto pChild = *it;
		if ((pChild == nullptr) || !pChild->IsVisible() || pChild->IsFloat()) {
			continue;
		}

		UiMargin rcMargin = pChild->GetMargin();
		UiSize szTile = CalcEstimateSize(pChild, szItem, rc);

		//�����߶����ֵ
		if (szTile.cy > 0) {
			cyHeight = std::max(cyHeight, szTile.cy + rcMargin.top + rcMargin.bottom);
		}

		++iIndex;
		if ((iIndex % nColumns) == 0) {
			//���У��˳�
			break;
		}
	}
	return cyHeight;
}

UiSize TileLayout::CalcTilePosition(Control* pControl, 
	                                int32_t itemWidth, int32_t itemHeight,
							        const UiSize& szStretchItem, const UiRect& rcContainer,
	                                const UiPoint& ptTile, UiRect& szTilePos) const
{
	szTilePos.Clear();
	ASSERT(pControl != nullptr);
	if ((pControl == nullptr) || !pControl->IsVisible() || pControl->IsFloat()) {
		return UiSize();
	}
	//Ŀ�������С����͸ߣ�
	UiSize szItem(itemWidth, itemHeight);
	szItem.Validate();

	//��Ƭ�ؼ���С(��͸�)
	UiSize childSize = CalcEstimateSize(pControl, szItem, rcContainer);
	if (childSize.cx == 0) {
		childSize.cx = szStretchItem.cx;
	}
	if (childSize.cy == 0) {
		childSize.cy = szStretchItem.cy;
	}
	childSize.Validate();

	if ((szItem.cx == 0) && (childSize.cx > 0)) {
		szItem.cx = childSize.cx;
	}
	if ((childSize.cx == 0) && (szItem.cx > 0)) {
		childSize.cx = szItem.cx;
	}

	if ((szItem.cy == 0) && (childSize.cy > 0)) {
		szItem.cy = childSize.cy;
	}
	if ((childSize.cy == 0) && (szItem.cy > 0)) {
		childSize.cy = szItem.cy;
	}
	
	int32_t cxWidth = szItem.cx;	//ÿ���ؼ�����Ƭ���Ŀ�ȣ���̬����ֵ��
	int32_t cyHeight = szItem.cy;	//ÿ���ؼ�����Ƭ���ĸ߶ȣ���̬����ֵ��

	//Ŀ������أ����Ͻ����꣬��͸ߣ�
	UiRect rcTile(ptTile.x, ptTile.y, ptTile.x + cxWidth, ptTile.y + cyHeight);
	szTilePos = GetFloatPos(pControl, rcTile, childSize);
	return UiSize(cxWidth, cyHeight);
}

UiSize64 TileLayout::ArrangeChild(const std::vector<Control*>& items, UiRect rc)
{
	//���岼�ֲ��ԣ�
	// (1) �������������߽磨�������׵ĵ�һ��Ԫ�ش�С��rc����������º���ᳬ���߽磩��
	//     ������ܻ���ֳ����߽硣
	// (2) ��Ƭ�Ŀ�߱ȣ�Ĭ�ϱ��֡�
	//     �����Ȼ��߸߶ȳ��������ţ���Ҫ���ֿ�߱ȣ�������ֱ��Σ�
	//     �����ṩһ��ѡ������ֿ�߱ȣ���������£����б�������
	// (3) ���ڲ�������ģʽ����������m_bAutoCalcColumnsΪtrue����m_nColumns������
	//���ֵļ�������:
	// (1) !m_bAutoCalcColumns && (m_nColumns == 0) && (m_szItem.cx == 0)
	//     ���ֲ��ԣ�	1�����������ɲ��֣������У�ÿ����Ҫ������߽磬�ͻ��У�ÿ�е��������ܶ���ͬ����
	//				2����Ƭ�ؼ��Ŀ�ȣ�����ʵ�ʿ��չʾ��
	//              3����Ƭ�ؼ��ĸ߶ȣ�
	//                ��1�����m_szItem.cy > 0������Ϊ�̶�m_szItem.cy
	//                ��2�����m_szItem.cy == 0: ����ʵ�ʿ��չʾ��
	// (2) (m_nColumns == 0) && (m_szItem.cx > 0)
	//     ���ֲ��ԣ�	1������������ rc.Width() �� m_szItem.cx ������Ӧ�÷ּ��У������̶���
	//				2����Ƭ�ؼ��Ŀ�ȣ��̶�Ϊ m_szItem.cx��
	//              3����Ƭ�ؼ��ĸ߶ȣ�
	//                ��1�����m_szItem.cy > 0������Ϊ�̶�m_szItem.cy
	//                ��2�����m_szItem.cy == 0: ����ʵ�ʿ��չʾ��
	// (3) (m_nColumns > 0) && (m_szItem.cx == 0)
	//     ���ֲ��ԣ�	1�������������̶�Ϊm_nColumns��
	//				2����Ƭ�ؼ��Ŀ�ȣ�����ʵ�ʿ��չʾ��
	//              3����Ƭ�ؼ��ĸ߶ȣ�
	//                ��1�����m_szItem.cy > 0������Ϊ�̶�m_szItem.cy
	//                ��2�����m_szItem.cy == 0: ����ʵ�ʿ��չʾ��
	// (4) (m_nColumns > 0) && (m_szItem.cx > 0)
	//     ���ֲ��ԣ�	1�������������̶�Ϊm_nColumns��
	//				2����Ƭ�ؼ��Ŀ�ȣ��̶�Ϊ m_szItem.cx��
	//              3����Ƭ�ؼ��ĸ߶ȣ�
	//                ��1�����m_szItem.cy > 0������Ϊ�̶�m_szItem.cy
	//                ��2�����m_szItem.cy == 0: ����ʵ�ʿ��չʾ��

	std::vector<int32_t> inColumnWidths;
	std::vector<int32_t> outColumnWidths;
	ArrangeChildInternal(items, rc, true, inColumnWidths, outColumnWidths);
	inColumnWidths.swap(outColumnWidths);
	return ArrangeChildInternal(items, rc, false, inColumnWidths, outColumnWidths);
}

UiSize64 TileLayout::ArrangeChildInternal(const std::vector<Control*>& items,
										  UiRect rect,
										  bool isCalcOnly,
										  const std::vector<int32_t>& inColumnWidths,
										  std::vector<int32_t>& outColumnWidths) const
{
	UiRect orgRect = rect;
	DeflatePadding(rect);           //��ȥ�ڱ߾࣬ʣ�µ��ǿ�������
	const UiRect& rc = rect;

	UiSize szItem;					//��Ƭ�ؼ���Ⱥ͸߶ȣ�����ֵ��
	int32_t nColumns = 0;			//����������ֵ��
	CalcTileWidthAndColumns(items, rc, szItem, nColumns);
	if ((nColumns == 0) && (szItem.cx == 0)) {
		//ʹ�����ɲ������пؼ�(�޹̶������������������չʾ�ռ䣬��ʾ�����ܶ������)
		return ArrangeChildFreeLayout(items, orgRect, isCalcOnly);
	}
	ASSERT(nColumns > 0);
	if (nColumns <= 0) {
		return UiSize64();
	}
	
	//������ʾ�������Ŀ��, ������Ϊ�������͵Ŀؼ���Ⱥ͸߶�
	UiSize szStretchItem;
	CalcStretchTileSize(items, rc, nColumns, szStretchItem);

#ifdef _DEBUG
	{
		int32_t cx = szItem.cx;
		int32_t cy = szItem.cy;
		int32_t col = nColumns;

		int32_t cx_s = szStretchItem.cx;
		int32_t cy_s = szStretchItem.cy;

		int32_t cx_config = m_szItem.cx;
		int32_t cy_config = m_szItem.cy;
		int32_t col_config = m_nColumns;
		bool autoCalc = m_bAutoCalcColumns;

		int32_t ii = 0;
	}
#endif

	int64_t cxNeededFloat = 0;	//�����ؼ���Ҫ���ܿ��	
	int64_t cyNeededFloat = 0;	//�����ؼ���Ҫ���ܸ߶�	
	int32_t cyLineHeight = 0;   //ÿ�пؼ�����Ƭ���ĸ߶ȣ���̬����ֵ��

	std::vector<int32_t> rowHeights;   //ÿ�еĿ��ֵ
	std::vector<int32_t> columnWidths; //ÿ�еĸ߶�ֵ
	columnWidths.resize(nColumns);
	rowHeights.resize(1);

	int32_t nRowTileCount = 0;  //�������ɵ���Ƭ�ؼ�����
	int32_t nRowIndex = 0;      //��ǰ���к�

	UiPoint ptTile(rc.left, rc.top);	//ÿ���ؼ�����Ƭ���Ķ�������	
	for( auto it = items.begin(); it != items.end(); ++it ) {
		auto pControl = *it;
		if ((pControl == nullptr) || !pControl->IsVisible()) {
			continue;
		}
		if( pControl->IsFloat() ) {
			//�����ؼ�
			UiSize64 floatSize;
			if (!isCalcOnly) {
				//���ø����ؼ���λ��
				floatSize = SetFloatPos(pControl, rc);
			}
			else {
				//����Float�ؼ��Ĵ�С
				floatSize = EstimateFloatSize(pControl, rc);				
			}
			if (cxNeededFloat < floatSize.cx) {
				cxNeededFloat = floatSize.cx;
			}
			if (cyNeededFloat < floatSize.cy) {
				cyNeededFloat = floatSize.cy;
			}
			continue;
		}
		
		if (nRowTileCount == 0) {
			//һ�еĿ�ʼ��������һ�еĸ߶�
			nRowTileCount = nColumns;
			cyLineHeight = CalcTileLineHeight(items, it, nColumns, szItem, rc);
			if (cyLineHeight == 0) {
				//�������ȫ�������������͵��ӿؼ�����ֵ
				cyLineHeight = szStretchItem.cy;
			}
			ASSERT(cyLineHeight > 0);
		}
		
		const int32_t colIndex = nColumns - nRowTileCount;//��ǰ���±�[0, nColumns)
		int32_t columnWidth = 0; //��ǰ������п��
		if (((int32_t)inColumnWidths.size() == nColumns) &&
			(colIndex < inColumnWidths.size())) {
			columnWidth = inColumnWidths[colIndex];
		}

		//���㵱ǰ��Ƭ�ؼ���λ�����ꡢ���(cxWidth)�͸߶�(cyHeight)
		int32_t itemWidth = szItem.cx;
		if (columnWidth > 0) {
			itemWidth = columnWidth;
		}
		UiRect rcTilePos;
		UiSize szTileSize = CalcTilePosition(pControl, itemWidth, cyLineHeight, szStretchItem, rc, ptTile, rcTilePos);
		UiMargin rcMargin = pControl->GetMargin();
		int32_t cxWidth = szTileSize.cx + rcMargin.left + rcMargin.right;
		if (!isCalcOnly) {
			pControl->SetPos(rcTilePos);
		}

		//���㱾�и߶����ֵ���߶�ֵ��Ҫ������߾�
		int32_t tileHeight = rcTilePos.Height() + rcMargin.top + rcMargin.bottom;
		rowHeights[nRowIndex] = std::max(tileHeight, rowHeights[nRowIndex]);

		--nRowTileCount;
		if(nRowTileCount == 0 ) {
			//����
			rowHeights.push_back(0);
			nRowIndex = (int32_t)rowHeights.size() - 1;			
			
			//��������X�����Y�����λ��
			ptTile.x = rc.left;
			//Y�������л�����һ�У����и��л�
			ptTile.y += cyLineHeight + GetChildMarginY();
		}
		else {
			//ͬһ�У������л����꣬����ǰ��Ƭ�ؼ��Ŀ���л�
			if (columnWidth > 0) {
				ptTile.x += columnWidth + GetChildMarginX();
			}
			else {
				ptTile.x += cxWidth + GetChildMarginX();
			}
		}		
		//��¼ÿ�еĿ�ȣ�ȡ��һ���У��ؼ���ȵ����ֵ�������˿ؼ�����߾ࣩ
		if (colIndex < columnWidths.size()) {
			int32_t tileWidth = rcTilePos.Width() + rcMargin.left + rcMargin.right;
			tileWidth = std::min(tileWidth, szItem.cx);
			columnWidths[colIndex] = std::max(tileWidth, columnWidths[colIndex]);
		}
	}

	//�����ڱ߾��Ѿ������������Ⱥ͸߶ȵ�ʱ����Ҫ�����ڱ߾�
	const UiPadding& padding = GetPadding();

	//����������
	int64_t cxNeeded = std::accumulate(columnWidths.begin(), columnWidths.end(), 0);
	if (columnWidths.size() > 1) {
		cxNeeded += (columnWidths.size() - 1) * GetChildMarginX();
	}
	cxNeeded = std::max(cxNeeded, cxNeededFloat);
	cxNeeded += (padding.left + padding.right);

	//��������߶�
	int64_t cyNeeded = std::accumulate(rowHeights.begin(), rowHeights.end(), 0);
	if (rowHeights.size() > 1) {
		cyNeeded += (rowHeights.size() - 1) * GetChildMarginY();
	}
	cyNeeded = std::max(cyNeeded, cyNeededFloat);
	cyNeeded += (padding.top + padding.bottom);	

	outColumnWidths.swap(columnWidths);
	UiSize64 size(cxNeeded, cyNeeded);
	return size;
}

UiSize64 TileLayout::ArrangeChildFreeLayout(const std::vector<Control*>& items, UiRect rc, bool isCalcOnly) const
{
	DeflatePadding(rc);				//��ȥ�ڱ߾࣬ʣ�µ��ǿ�������
	return UiSize64();
}

int32_t TileLayout::CalcLineHeightAndTileCount(const std::vector<Control*>& items,
	const std::vector<Control*>::const_iterator iterBegin,
	int32_t nColumns,
	const UiSize& szItem,
	const UiSize& szStretchItem,
	const UiRect& rc,
	int32_t& nLineTileCount) const
{
	return 0;
	//nLineTileCount = 0;
	////��� (nColumns == 0) && (szItem.cx == 0)����ʹ�����ɲ������пؼ�
	//bool isValid = ((nColumns == 0) && (szItem.cx == 0)) || ((nColumns > 0) && (szItem.cx > 0));
	//ASSERT(isValid);
	//if (!isValid) {
	//	return 0;
	//}

	//int32_t cyHeight = szItem.cy;
	//if ((cyHeight > 0) && (nColumns > 0)) {
	//	//��������˸߶Ⱥ�������������ʹ�����õ�ֵ
	//	nLineTileCount = nColumns;
	//	return cyHeight;
	//}

	//int32_t stretchWidth = szItem.cx; //�������͵Ŀؼ��Ŀ��
	//if (stretchWidth == 0) {
	//	stretchWidth = szStretchItem.cx;
	//}

	//int32_t lineLeftWidth = rc.Width(); //����ʣ����ÿ��
	//int32_t iIndex = 0; //�ؼ����
	//bool bLineFirstTile = true; //��ǰ�ǲ������׵Ŀؼ�
	//for (auto it = iterBegin; it != items.end(); ++it) {
	//	auto pChild = *it;
	//	if ((pChild == nullptr) || !pChild->IsVisible() || pChild->IsFloat()) {
	//		continue;
	//	}
	//	UiMargin rcMargin = pChild->GetMargin();
	//	UiSize szAvailable(lineLeftWidth - rcMargin.left - rcMargin.right,
	//		rc.Height() - rcMargin.top - rcMargin.bottom);

	//	if (iIndex == 0) {
	//		bLineFirstTile = true;
	//	}
	//	else if ((nColumns > 0) && ((iIndex + 1) % nColumns == 0)) {
	//		bLineFirstTile = true;
	//	}
	//	else if (nColumns == 0) {
	//		//
	//	}
	//	else {
	//		bLineFirstTile = false;
	//	}
	//	if (!bLineFirstTile) {
	//		//���м���ӿؼ�
	//		szAvailable.cx -= GetChildMarginX();
	//	}

	//	UiEstSize estSize = pChild->EstimateSize(szAvailable);
	//	UiSize szTile(estSize.cx.GetInt32(), estSize.cy.GetInt32());
	//	if (estSize.cy.IsStretch()) {
	//		//�ÿؼ����������͵Ŀؼ�
	//		szTile.cy = 0;
	//	}
	//	if (szTile.cy < pChild->GetMinHeight()) {
	//		szTile.cy = pChild->GetMinHeight();
	//	}
	//	if (szTile.cy > pChild->GetMaxHeight()) {
	//		szTile.cy = pChild->GetMaxHeight();
	//	}
	//	//�����߶����ֵ
	//	cyHeight = std::max(cyHeight, szTile.cy + rcMargin.top + rcMargin.bottom);

	//	++iIndex;
	//	if ((nColumns > 0) && (iIndex % nColumns) == 0) {
	//		//���У��˳�
	//		break;
	//	}
	//	else {
	//		//
	//	}
	//}
	//return cyHeight;
}

UiSize TileLayout::EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable)
{
	szAvailable.Validate();
	UiRect rc(0, 0, szAvailable.Width(), szAvailable.Height());
	std::vector<int32_t> inColumnWidths;
	std::vector<int32_t> outColumnWidths;
	UiSize64 requiredSize = ArrangeChildInternal(items, rc, true, inColumnWidths, outColumnWidths);
	UiSize size(TruncateToInt32(requiredSize.cx), TruncateToInt32(requiredSize.cy));
	return size;
}

bool TileLayout::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	bool hasAttribute = true;
	if((strName == L"item_size") || (strName == L"itemsize")){
		UiSize szItem;
		AttributeUtil::ParseSizeValue(strValue.c_str(), szItem);
		SetItemSize(szItem);
	}
	else if( strName == L"columns") {		
		if (strValue == L"auto") {
			//�Զ���������
			m_bAutoCalcColumns = true;
		}
		else {
			m_bAutoCalcColumns = false;
			SetColumns(_wtoi(strValue.c_str()));
		}
	}
	else {
		hasAttribute = Layout::SetAttribute(strName, strValue);
	}
	return hasAttribute;
}

const UiSize& TileLayout::GetItemSize() const
{
	return m_szItem;
}

void TileLayout::SetItemSize(UiSize szItem, bool bNeedDpiScale)
{
	szItem.cx = std::max(szItem.cx, 0);
	szItem.cy = std::max(szItem.cy, 0);
	if (bNeedDpiScale) {
		GlobalManager::Instance().Dpi().ScaleSize(szItem);
	}

	if( (m_szItem.cx != szItem.cx) || (m_szItem.cy != szItem.cy) ) {
		m_szItem = szItem;
		m_pOwner->Arrange();
	}
}

int32_t TileLayout::GetColumns() const
{
	return m_nColumns;
}

void TileLayout::SetColumns(int32_t nCols)
{
	nCols = std::max(nCols, 0);
	if (m_nColumns != nCols) {
		m_nColumns = nCols;
		m_pOwner->Arrange();
	}	
}

} // namespace ui
