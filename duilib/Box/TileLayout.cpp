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
	m_bAutoCalcColumns(false),
	m_bScaleDown(true)
{
}

UiSize TileLayout::CalcEstimateSize(Control* pControl, const UiSize& szItem, UiRect rc)
{
	if ((pControl == nullptr) || !pControl->IsVisible()) {
		return UiSize();
	}
	
	//����Ŀ�������ʼ��ʹ��������
	UiMargin rcMargin = pControl->GetMargin();
	int32_t width = rc.Width() - rcMargin.left - rcMargin.right;
	int32_t height = rc.Height() - rcMargin.top - rcMargin.bottom;
	UiSize szAvailable(width, height);
	szAvailable.Validate();
	UiEstSize estSize = pControl->EstimateSize(szAvailable);
	UiSize childSize(estSize.cx.GetInt32(), estSize.cy.GetInt32());
	if (estSize.cy.IsStretch()) {
		childSize.cy = szItem.cy;
	}
	if (childSize.cy < pControl->GetMinHeight()) {
		childSize.cy = pControl->GetMinHeight();
	}
	if (childSize.cy > pControl->GetMaxHeight()) {
		childSize.cy = pControl->GetMaxHeight();
	}

	if (estSize.cx.IsStretch()) {
		childSize.cx = szItem.cx;
	}
	if (childSize.cx < pControl->GetMinWidth()) {
		childSize.cx = pControl->GetMinWidth();
	}
	if (childSize.cx > pControl->GetMaxWidth()) {
		childSize.cx = pControl->GetMaxWidth();
	}
	return childSize;
}

void TileLayout::CalcTileColumns(const std::vector<ItemSizeInfo>& normalItems, const UiRect& rc,
								 int32_t tileWidth, int32_t childMarginX, int32_t childMarginY,
	                             int32_t& nColumns)
{
	nColumns = 0;
	if (tileWidth <= 0) {
		//��Ҫ�ȼ�����Ƭ�ؼ���ȣ�Ȼ����ݿ�ȼ�������
		int32_t maxWidth = 0;
		int64_t areaTotal = 0;
		const int64_t maxArea = (int64_t)rc.Width() * rc.Height();
		for (const ItemSizeInfo& itemSizeInfo : normalItems) {
			UiMargin rcMargin = itemSizeInfo.pControl->GetMargin();
			UiSize childSize(itemSizeInfo.cx, itemSizeInfo.cy);
			if (childSize.cx > 0) {
				maxWidth = std::max(maxWidth, childSize.cx + rcMargin.left + rcMargin.right);
			}
			int32_t childWidth = childSize.cx;
			int32_t childHeight = childSize.cy;
			if (childWidth > 0) {
				childWidth += childMarginX;
			}			
			if (childHeight > 0) {
				childHeight += childMarginY;
			}
			areaTotal += ((int64_t)childWidth * childHeight);
			if (areaTotal > maxArea) {
				//����ǰ������չʾ���������
				break;
			}
		}
		//ȡ�������ؼ�������ֵ����Ϊÿ��Item�Ŀ��
		tileWidth = maxWidth;
	}
	if (tileWidth > 0) {
		//ʹ�����õĿ����Ϊ��Ƭ�ؼ��Ŀ�ȣ���ͨ�����õĿ�ȣ���������
		int32_t totalWidth = rc.Width();
		while (totalWidth > 0) {
			totalWidth -= tileWidth;
			if (nColumns != 0) {
				totalWidth -= childMarginX;
			}
			if (totalWidth >= 0) {
				++nColumns;
			}
		}
	}
}

UiSize64 TileLayout::EstimateFloatSize(Control* pControl, UiRect rc)
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

UiSize64 TileLayout::ArrangeFloatChild(const std::vector<Control*>& items,
									   UiRect rc,
									   const UiSize& szItem,
									   bool isCalcOnly,
									   std::vector<ItemSizeInfo>& normalItems)
{
	int64_t cxNeededFloat = 0;	//�����ؼ���Ҫ���ܿ��
	int64_t cyNeededFloat = 0;	//�����ؼ���Ҫ���ܸ߶�
	for (Control* pControl : items) {
		if ((pControl == nullptr) || !pControl->IsVisible()) {
			continue;
		}
		if (pControl->IsFloat()) {
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
		}
		else {
			//��ͨ�ؼ�
			UiSize childSize = CalcEstimateSize(pControl, szItem, rc);
			if ((childSize.cx <= 0) || (childSize.cy <= 0)) {
				//��СΪ0�ģ�������ʾ�ؼ�(����������ؼ�)
				if (!isCalcOnly) {
					UiRect rcPos(rc);
					rcPos.right = rcPos.left;
					rcPos.bottom = rcPos.top;
					pControl->SetPos(rcPos);
				}
			}
			else {
				ItemSizeInfo info;
				info.pControl = pControl;
				info.cx = childSize.cx;
				info.cy = childSize.cy;
				normalItems.push_back(info);
			}
		}
	}
	return UiSize64(cxNeededFloat, cyNeededFloat);
}

int32_t TileLayout::CalcTileLineHeight(const std::vector<ItemSizeInfo>& normalItems,
									   const std::vector<ItemSizeInfo>::const_iterator iterBegin,
									   int32_t nColumns,
									   const UiSize& szItem)
{
	ASSERT(nColumns > 0);
	int32_t cyHeight = szItem.cy;
	if (cyHeight > 0) {
		//��������˸߶ȣ�������ʹ�����õĸ߶�ֵ
		return cyHeight;
	}
	if (nColumns == 0) {
		return 0;
	}

	int32_t iIndex = 0;
	for (auto it = iterBegin; it != normalItems.end(); ++it) {
		const ItemSizeInfo& itemSizeInfo = *it;
		UiMargin rcMargin = itemSizeInfo.pControl->GetMargin();
		UiSize szTile(itemSizeInfo.cx, itemSizeInfo.cy);

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

UiSize TileLayout::CalcTilePosition(const ItemSizeInfo& itemSizeInfo,
	                                int32_t tileWidth, int32_t tileHeight,
	                                const UiPoint& ptTile, bool bScaleDown, UiRect& szTilePos)
{
	szTilePos.Clear();
	//Ŀ�������С����͸ߣ�
	UiSize szItem(tileWidth, tileHeight);
	szItem.Validate();

	//��Ƭ�ؼ���С(��͸�)
	UiSize childSize(itemSizeInfo.cx, itemSizeInfo.cy);
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

	//�Կؼ����еȱ�������
	if (bScaleDown && 
		(rcTile.Width() > 0) && (rcTile.Height() > 0) &&
		(childSize.cx > 0) && (childSize.cy > 0) ) {
		if ((childSize.cx > rcTile.Width()) || (childSize.cy > rcTile.Height())) {
			UiSize oldSize = childSize;
			double cx = childSize.cx;
			double cy = childSize.cy;
			double cxRatio = cx / rcTile.Width();
			double cyRatio = cy / rcTile.Height();
			if (cxRatio > cyRatio) {
				ASSERT(childSize.cx > rcTile.Width());
				double ratio = cx / childSize.cy;
				childSize.cx = rcTile.Width();
				childSize.cy = static_cast<int32_t>(childSize.cx / ratio + 0.5);
			}
			else {
				ASSERT(childSize.cy > rcTile.Height());
				double ratio = cy / childSize.cx;
				childSize.cy = rcTile.Height();
				childSize.cx = static_cast<int32_t>(childSize.cy / ratio + 0.5);
			}
			//double r0 = 1.0 * oldSize.cy / childSize.cy;
			//double r1 = 1.0 * oldSize.cx / childSize.cx;
			//ASSERT(childSize.cx * oldSize.cy == childSize.cy * oldSize.cx);
		}
	}

	szTilePos = GetFloatPos(itemSizeInfo.pControl, rcTile, childSize);
	if (szTilePos.left < ptTile.x) {
		//����ؼ��ϴ󣬳����߽磬�������
		int32_t width = szTilePos.Width();
		szTilePos.left = ptTile.x;
		szTilePos.right = ptTile.x + width;
	}
	if (szTilePos.top < ptTile.y) {
		//����ؼ��ϴ󣬳����߽磬���϶���
		int32_t height = szTilePos.Height();
		szTilePos.top = ptTile.y;
		szTilePos.bottom = ptTile.y + height;
	}
	return UiSize(cxWidth, cyHeight);
}

UiSize64 TileLayout::ArrangeChild(const std::vector<Control*>& items, UiRect rc)
{
	//���岼�ֲ��ԣ�
	// (1) �������������߽磨�������׵ĵ�һ��Ԫ�ش�С��rc����������º���ᳬ���߽磩��
	//     ������ܻ���ֳ����߽硣
	// (2) ��Ƭ�Ŀ�߱ȣ�Ĭ�ϱ��ֿ�߱Ƚ�����С������ӦĿ����ʾ��(����m_bScaleDown���ƴ���Ϊ)��
	//     �����Ȼ��߸߶ȳ��������ţ���Ҫ���ֿ�߱ȣ�������ֱ��Σ�
	//     �����ṩһ��ѡ��SetScaleDown()�������ֿ�߱ȣ���������£����г����߽������
	// (3) ���ڲ�������ģʽ����������m_bAutoCalcColumnsΪtrue����m_nColumns������
	//���ֵļ�������:
	// (1) !m_bAutoCalcColumns && (m_nColumns == 0) && (m_szItem.cx == 0)
	//     ���ֲ��ԣ�	1�����������ɲ��֣������У�ÿ����Ҫ������߽磬�ͻ��У�ÿ�е��������ܶ���ͬ����
	//				2����Ƭ�ؼ��Ŀ�ȣ�����ʵ�ʿ��չʾ��
	//              3����Ƭ�ؼ��ĸ߶ȣ�
	//                ��1�����m_szItem.cy > 0������Ϊ�̶�m_szItem.cy
	//                ��2�����m_szItem.cy == 0: ����ʵ�ʸ߶�չʾ��
	// (2) (m_nColumns == 0) && (m_szItem.cx > 0)
	//     ���ֲ��ԣ�	1������������ rc.Width() �� m_szItem.cx ������Ӧ�÷ּ��У������̶���
	//				2����Ƭ�ؼ��Ŀ�ȣ��̶�Ϊ m_szItem.cx��
	//              3����Ƭ�ؼ��ĸ߶ȣ�
	//                ��1�����m_szItem.cy > 0������Ϊ�̶�m_szItem.cy
	//                ��2�����m_szItem.cy == 0: ����ʵ�ʸ߶�չʾ��
	// (3) (m_nColumns > 0) && (m_szItem.cx == 0)
	//     ���ֲ��ԣ�	1�������������̶�Ϊm_nColumns��
	//				2����Ƭ�ؼ��Ŀ�ȣ�����ʵ�ʿ��չʾ��
	//              3����Ƭ�ؼ��ĸ߶ȣ�
	//                ��1�����m_szItem.cy > 0������Ϊ�̶�m_szItem.cy
	//                ��2�����m_szItem.cy == 0: ����ʵ�ʸ߶�չʾ��
	// (4) (m_nColumns > 0) && (m_szItem.cx > 0)
	//     ���ֲ��ԣ�	1�������������̶�Ϊm_nColumns��
	//				2����Ƭ�ؼ��Ŀ�ȣ��̶�Ϊ m_szItem.cx��
	//              3����Ƭ�ؼ��ĸ߶ȣ�
	//                ��1�����m_szItem.cy > 0������Ϊ�̶�m_szItem.cy
	//                ��2�����m_szItem.cy == 0: ����ʵ�ʸ߶�չʾ��

	if (IsFreeLayout()) {
		//ʹ�����ɲ������пؼ�(�޹̶������������������չʾ�ռ䣬��ʾ�����ܶ������)
		return ArrangeChildFreeLayout(items, rc, false);
	}
	else {
		std::vector<int32_t> inColumnWidths;
		std::vector<int32_t> outColumnWidths;
		ArrangeChildNormal(items, rc, true, inColumnWidths, outColumnWidths);
		inColumnWidths.swap(outColumnWidths);
		return ArrangeChildNormal(items, rc, false, inColumnWidths, outColumnWidths);
	}
}

bool TileLayout::IsFreeLayout() const
{
	return (!m_bAutoCalcColumns && (m_nColumns == 0) && (m_szItem.cx == 0));
}

UiSize64 TileLayout::ArrangeChildNormal(const std::vector<Control*>& items,
										UiRect rect,
										bool isCalcOnly,
										const std::vector<int32_t>& inColumnWidths,
										std::vector<int32_t>& outColumnWidths) const
{
	ASSERT(!IsFreeLayout());
	DeflatePadding(rect);           //��ȥ�ڱ߾࣬ʣ�µ��ǿ�������
	const UiRect& rc = rect;

	//���������ؼ����������ؿؼ���������ʾ�ؼ���
	//�������͵��ӿؼ������(m_szItem.cx > 0) && (m_szItem.cy > 0) Ϊtrue���������ʾ������ᱻ���˵�
	std::vector<ItemSizeInfo> normalItems;
	UiSize64 floatNeeded = ArrangeFloatChild(items, rc, m_szItem, isCalcOnly, normalItems); //�����ؼ���Ҫ���ܿ�Ⱥ͸߶�

	int32_t nColumns = m_nColumns;  //����������ֵ��
	if (m_bAutoCalcColumns) {
		//����Զ����������������¼�������
		nColumns = 0;
	}
	if (nColumns <= 0) {
		CalcTileColumns(normalItems, rc, m_szItem.cx, GetChildMarginX(), GetChildMarginY(), nColumns);
	}
	if (nColumns < 1) {
		//�޷���ȷ����ʱ��Ĭ��ֵ����Ϊ1
		nColumns = 1;
	}

	//�п�����, �̶�ֵ
	std::vector<int32_t> fixedColumnWidths = inColumnWidths;
	if (m_szItem.cx > 0) {
		fixedColumnWidths.clear();
		fixedColumnWidths.resize(nColumns, m_szItem.cx);
	}

	int32_t cyLineHeight = 0;   //ÿ�пؼ�����Ƭ���ĸ߶ȣ���̬����ֵ��

	std::vector<int32_t> rowHeights;   //ÿ�еĸ߶�ֵ������ֵ
	std::vector<int32_t> columnWidths; //ÿ�еĿ��ֵ������ֵ
	columnWidths.resize(nColumns);
	rowHeights.resize(1);

	int32_t nRowTileCount = 0;  //�������ɵ���Ƭ�ؼ�����
	int32_t nRowIndex = 0;      //��ǰ���к�

	int32_t xPosLeft = rc.left; //�ؼ���ʾ���ݵ��������ֵ(����������ж���)
	if (!isCalcOnly && !fixedColumnWidths.empty()) {
		int32_t cxTotal = std::accumulate(fixedColumnWidths.begin(), fixedColumnWidths.end(), 0);
		if (fixedColumnWidths.size() > 1) {
			cxTotal += ((int32_t)fixedColumnWidths.size() - 1) * GetChildMarginX();
		}
		if (cxTotal < rc.Width()) {
			xPosLeft = rc.CenterX() - cxTotal / 2;
		}
	}

	UiPoint ptTile(xPosLeft, rc.top);	//ÿ���ؼ�����Ƭ���Ķ�������	
	for( auto it = normalItems.begin(); it != normalItems.end(); ++it ) {
		const ItemSizeInfo& itemSizeInfo = *it;
		Control* pControl = itemSizeInfo.pControl;
		if (nRowTileCount == 0) {
			//һ�еĿ�ʼ��������һ�еĸ߶�
			nRowTileCount = nColumns;
			cyLineHeight = CalcTileLineHeight(normalItems, it, nColumns, m_szItem);
			ASSERT(cyLineHeight > 0);//����������
		}
		
		const int32_t colIndex = nColumns - nRowTileCount;//��ǰ���±�[0, nColumns)
		int32_t fixedColumnWidth = 0; //��ǰ������п��, �̶�ֵ
		if (((int32_t)fixedColumnWidths.size() == nColumns) &&
			(colIndex < fixedColumnWidths.size())) {
			fixedColumnWidth = fixedColumnWidths[colIndex];
		}

		//���㵱ǰ��Ƭ�ؼ���λ�����ꡢ���(cxWidth)�͸߶�(cyHeight)
		UiRect rcTilePos;
		UiSize szTileSize = CalcTilePosition(itemSizeInfo, fixedColumnWidth, cyLineHeight,
			                                 ptTile, m_bScaleDown, rcTilePos);
		
		if (!isCalcOnly) {
			pControl->SetPos(rcTilePos);
		}

		UiMargin rcMargin = pControl->GetMargin();
		int32_t cxWidth = szTileSize.cx + rcMargin.left + rcMargin.right;
		if (fixedColumnWidth > 0) {
			cxWidth = fixedColumnWidth;
		}

		//���㱾�и߶����ֵ���߶�ֵ��Ҫ������߾�
		int32_t tileHeight = rcTilePos.Height() + rcMargin.top + rcMargin.bottom;
		rowHeights[nRowIndex] = std::max(tileHeight, rowHeights[nRowIndex]);

		--nRowTileCount;
		if(nRowTileCount == 0 ) {
			//����
			rowHeights.push_back(0);
			nRowIndex = (int32_t)rowHeights.size() - 1;			
			
			//��������X�����Y�����λ��(����)
			ptTile.x = xPosLeft;
			//Y�������л�����һ�У����и��л�
			ptTile.y += cyLineHeight + GetChildMarginY();
		}
		else {
			//ͬһ�У������л����꣬����ǰ��Ƭ�ؼ��Ŀ���л�
			ptTile.x += cxWidth + GetChildMarginX();
		}		
		//��¼ÿ�еĿ�ȣ�ȡ��һ���У��ؼ���ȵ����ֵ�������˿ؼ�����߾ࣩ
		if (colIndex < columnWidths.size()) {
			int32_t tileWidth = rcTilePos.Width();
			if (m_szItem.cx > 0) {
				tileWidth = m_szItem.cx;
			}
			tileWidth += rcMargin.left + rcMargin.right;
			//�Զ������ʱ���п����ƣ�������ƽ��ֵ
			if (m_szItem.cx == 0) {
				tileWidth = std::min(tileWidth, rc.Width() / nColumns);
			}
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
	cxNeeded = std::max(cxNeeded, floatNeeded.cx);
	cxNeeded += (padding.left + padding.right);

	//��������߶�
	int64_t cyNeeded = std::accumulate(rowHeights.begin(), rowHeights.end(), 0);
	if (rowHeights.size() > 1) {
		cyNeeded += (rowHeights.size() - 1) * GetChildMarginY();
	}
	cyNeeded = std::max(cyNeeded, floatNeeded.cy);
	cyNeeded += (padding.top + padding.bottom);	

	outColumnWidths.swap(columnWidths);
	UiSize64 size(cxNeeded, cyNeeded);
	return size;
}

UiSize64 TileLayout::ArrangeChildFreeLayout(const std::vector<Control*>& items, 
											UiRect rect, bool isCalcOnly) const
{
	DeflatePadding(rect); //��ȥ�ڱ߾࣬ʣ�µ��ǿ�������
	const UiRect& rc = rect;

	//���������ؼ����������ؿؼ���������ʾ�ؼ���
	//�������͵��ӿؼ������(m_szItem.cx > 0) && (m_szItem.cy > 0) Ϊtrue���������ʾ������ᱻ���˵�
	std::vector<ItemSizeInfo> normalItems;
	UiSize64 floatNeeded = ArrangeFloatChild(items, rc, m_szItem, isCalcOnly, normalItems); //�����ؼ���Ҫ���ܿ�Ⱥ͸߶�

	int64_t cxNeeded = 0;		//�Ǹ����ؼ���Ҫ���ܿ��	
	int64_t cyNeeded = 0;		//�Ǹ����ؼ���Ҫ���ܸ߶�

	int32_t cyLineHeight = 0;   //ÿ�пؼ�����Ƭ���ĸ߶ȣ���̬����ֵ��

	int32_t xPosLeft = rc.left;         //�ؼ���ʾ���ݵ��������ֵ��ʼ�ղ�ȡ�����
	UiPoint ptTile(xPosLeft, rc.top);	//ÿ���ؼ�����Ƭ���Ķ�������
	const size_t itemCount = normalItems.size();
	for (size_t index = 0; index < itemCount; ++index) {
		const ItemSizeInfo& itemSizeInfo = normalItems[index];
		Control* pControl = itemSizeInfo.pControl;

		//�ؼ�����߾�
		UiMargin rcMargin = pControl->GetMargin();

		//���㵱ǰ��Ƭ�ؼ���λ�����ꡢ��Ⱥ͸߶�
		UiRect rcTilePos;
		UiSize szTileSize = CalcTilePosition(itemSizeInfo, 0, 0,
											 ptTile, m_bScaleDown, rcTilePos);
		if (rcTilePos.right >= rc.right) {
			//�Ҳ��Ѿ������߽�, ������ǿ�������࣬���Ȼ��У�����ʾ
			if (rcTilePos.left > rc.left) {
				//�Ȼ���, Ȼ������ʾ
				ptTile.x = xPosLeft;
				//Y�������л�����һ�У����и��л�
				ptTile.y += cyLineHeight + GetChildMarginY();
				cyLineHeight = 0;

				szTileSize = CalcTilePosition(itemSizeInfo, 0, 0,
											  ptTile, m_bScaleDown, rcTilePos);				
			}
		}
		if (!isCalcOnly) {
			pControl->SetPos(rcTilePos);
		}
		cxNeeded = std::max((int64_t)rcTilePos.right + rcMargin.left + rcMargin.right, cxNeeded);
		cyNeeded = std::max((int64_t)rcTilePos.bottom + rcMargin.top + rcMargin.bottom, cyNeeded);

		//���¿ؼ����ֵ���и�ֵ
		int32_t cxWidth = rcTilePos.Width() + rcMargin.left + rcMargin.right;
		cyLineHeight = std::max(rcTilePos.Height() + rcMargin.top + rcMargin.bottom, cyLineHeight);

		if (rcTilePos.right >= rc.right) {
			//��ǰ�ؼ��Ѿ������߽磬��Ҫ����
			ptTile.x = xPosLeft;
			//Y�������л�����һ�У����и��л�
			ptTile.y += cyLineHeight + GetChildMarginY();
			cyLineHeight = 0;
		}
		else {
			//�����У�����л�������
			ptTile.x += cxWidth + GetChildMarginX();
		}
	}

	//�����ڱ߾��Ѿ������������Ⱥ͸߶ȵ�ʱ����Ҫ�����ڱ߾�
	const UiPadding& padding = GetPadding();
	cxNeeded = std::max(cxNeeded, floatNeeded.cx);
	cyNeeded = std::max(cyNeeded, floatNeeded.cy);

	if (isCalcOnly) {
		//���صĿ�ȣ���󲻳�����������Ŀռ䣬��Ϊ�˷���ֵ���Ϊ�������յĿ��ֵ
		if (cxNeeded > (rect.Width())) {
			cxNeeded = rect.Width();
		}
	}

	cxNeeded += (padding.left + padding.right);
	cyNeeded += (padding.top + padding.bottom);

	UiSize64 size(cxNeeded, cyNeeded);
	return size;
}

UiSize TileLayout::EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable)
{
	szAvailable.Validate();
	UiRect rc(0, 0, szAvailable.Width(), szAvailable.Height());
	UiSize64 requiredSize;
	if (IsFreeLayout()) {
		requiredSize = ArrangeChildFreeLayout(items, rc, true);
	}
	else {
		std::vector<int32_t> inColumnWidths;
		std::vector<int32_t> outColumnWidths;
		requiredSize = ArrangeChildNormal(items, rc, true, inColumnWidths, outColumnWidths);
	}
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
			SetAutoCalcColumns(true);
		}
		else {
			SetAutoCalcColumns(false);
			SetColumns(_wtoi(strValue.c_str()));
		}
	}
	else if ((strName == L"scale_down") || (strName == L"scaledown")) {
		SetScaleDown(strValue == L"true");
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
		if (m_pOwner != nullptr) {
			m_pOwner->Arrange();
		}
	}	
}

void TileLayout::SetAutoCalcColumns(bool bAutoCalcColumns)
{
	if (m_bAutoCalcColumns != bAutoCalcColumns) {
		m_bAutoCalcColumns = bAutoCalcColumns;
		if (m_pOwner != nullptr) {
			m_pOwner->Arrange();
		}
	}
}

bool TileLayout::IsAutoCalcColumns() const
{
	return m_bAutoCalcColumns;
}

void TileLayout::SetScaleDown(bool bScaleDown)
{
	if (m_bScaleDown != bScaleDown) {
		m_bScaleDown = bScaleDown;
		if (m_pOwner != nullptr) {
			m_pOwner->Arrange();
		}
	}
}

} // namespace ui
