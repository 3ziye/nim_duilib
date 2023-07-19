#include "VLayout.h"

namespace ui 
{

VLayout::VLayout()
{
}

UiSize64 VLayout::ArrangeChild(const std::vector<Control*>& items, UiRect rc)
{
	UiSize szAvailable(rc.Width(), rc.Height());
	szAvailable.Validate();
	if (rc.Width() < 0) {
		rc.right = rc.left;
	}
	if (rc.Height() < 0) {
		rc.bottom = rc.top;
	}

	//�߶�Ϊstretch�Ŀؼ���
	int32_t stretchCount = 0;
	//�̶��߶ȵĿؼ����ܵĸ߶�
	int32_t cyFixedTotal = 0;
	//��Ҫ���в��ִ�������пؼ�(KEY�ǿؼ���VALUE�ǿ�Ⱥ͸߶�)
	std::map<Control*, UiSize> itemsMap;

	//����ÿ���ؼ��Ŀ�Ⱥ͸߶ȣ�����¼��Map��
	for(auto pControl : items) {
		if ((pControl == nullptr) || !pControl->IsVisible() || pControl->IsFloat()) {
			continue;
		}

		UiSize sz = pControl->EstimateSize(szAvailable);
		ASSERT((sz.cx >= DUI_LENGTH_STRETCH) && (sz.cy >= DUI_LENGTH_STRETCH));

		UiRect rcMargin = pControl->GetMargin();
		//����߶�
		if( sz.cy == DUI_LENGTH_STRETCH ) {
			stretchCount++;
			cyFixedTotal += (rcMargin.top + rcMargin.bottom);
		}
		else {
			if (sz.cy < pControl->GetMinHeight()) {
				sz.cy = pControl->GetMinHeight();
			}
			if (sz.cy > pControl->GetMaxHeight()) {
				sz.cy = pControl->GetMaxHeight();
			}
			if (sz.cy < 0) {
				sz.cy = 0;
			}
			cyFixedTotal += (sz.cy + rcMargin.top + rcMargin.bottom);
		}

		//������
		if (sz.cx == DUI_LENGTH_STRETCH) {
			sz.cx = szAvailable.cx - rcMargin.left - rcMargin.right;
		}
		if (sz.cx < pControl->GetMinWidth()) {
			sz.cx = pControl->GetMinWidth();
		}
		if (sz.cx > pControl->GetMaxWidth()) {
			sz.cx = pControl->GetMaxWidth();
		}
		if (sz.cx < 0) {
			sz.cx = 0;
		}
		itemsMap[pControl] = sz;
	}
	if (!itemsMap.empty()) {
		cyFixedTotal += ((int32_t)itemsMap.size() - 1) * GetChildMarginY();
	}
	
	//ÿ���߶�Ϊstretch�Ŀؼ�����������ʵ�ʸ߶ȣ�ȡƽ��ֵ��
	int32_t cyStretch = 0;
	if (stretchCount > 0) {
		cyStretch = std::max(0, (szAvailable.cy - cyFixedTotal) / stretchCount);
	}

	//��һ��Ԥ����ȥ����Ҫʹ��minheight/maxheight�Ŀؼ��������¼���ƽ���߶�
	if ((cyStretch > 0) && !itemsMap.empty()) {
		for (auto iter = itemsMap.begin(); iter != itemsMap.end(); ++iter) {
			Control* pControl = iter->first;
			UiSize sz = iter->second;
			if (sz.cy == DUI_LENGTH_STRETCH) {
				sz.cy = cyStretch;
				if (sz.cy < pControl->GetMinHeight()) {
					sz.cy = pControl->GetMinHeight();
				}
				if (sz.cy > pControl->GetMaxHeight()) {
					sz.cy = pControl->GetMaxHeight();
				}
				if (sz.cy != cyStretch) {
					//����ؼ���Ҫʹ��min����max�߶ȣ���ƽ��ֵ���Ƴ�������Fixed�ؼ���
					iter->second = sz;
					--stretchCount;
					cyFixedTotal += sz.cy; //Margin�Ѿ��ۼӹ�������Ҫ�����ۼ�
				}
			}
		}
	}

	//���¼���Stretch�ؼ��ĸ߶ȣ���������μ����Ϊ׼��
	//��������ܿռ䲻�㣬��ԭ��������ƽ���߶ȣ����ȱ�֤ǰ��Ŀؼ�����������ʾ
	if ((stretchCount > 0) && (szAvailable.cy > cyFixedTotal)){
		cyStretch = std::max(0, (szAvailable.cy - cyFixedTotal) / stretchCount);
	}

	// Position the elements
	int32_t deviation = szAvailable.cy - cyFixedTotal - cyStretch * stretchCount;//ʣ����ø߶ȣ����ھ���ƫ��
	if (deviation < 0) {
		deviation = 0;
	}

	int32_t iPosLeft = rc.left;
	int32_t iPosRight = rc.right;
	int32_t iPosY = rc.top;

	// Place elements
	int64_t cyNeeded = 0;//��Ҫ���ܸ߶�
	int64_t cxNeeded = 0;//��Ҫ���ܿ�ȣ�ȡ�����ӿؼ��Ŀ�����ֵ��

	for(auto pControl : items) {
		if ((pControl == nullptr) || !pControl->IsVisible()) {
			continue;
		}
		if( pControl->IsFloat() ) {
			SetFloatPos(pControl, rc);
			continue;
		}

		UiRect rcMargin = pControl->GetMargin();
		ASSERT(itemsMap.find(pControl) != itemsMap.end());
		UiSize sz = itemsMap[pControl];

		//����߶�
		if( sz.cy == DUI_LENGTH_STRETCH ) {
			sz.cy = cyStretch;
			if (sz.cy < pControl->GetMinHeight()) {
				sz.cy = pControl->GetMinHeight();
			}
			if (sz.cy > pControl->GetMaxHeight()) {
				sz.cy = pControl->GetMaxHeight();
			}
			if ((sz.cy <= cyStretch) && (deviation > 0)) {
				sz.cy += 1;
				deviation--;
			}
		}
				
		//����������뷽ʽ��ȷ��X������
		int32_t childLeft = 0;
		int32_t childRight = 0;
		HorAlignType horAlignType = pControl->GetHorAlignType();
		if (horAlignType == kHorAlignLeft) {
			childLeft = iPosLeft + rcMargin.left;
			childRight = childLeft + sz.cx;
		}
		else if (horAlignType == kHorAlignRight) {
			childRight = iPosRight - rcMargin.right;
			childLeft = childRight - sz.cx;
		}
		else if (horAlignType == kHorAlignCenter) {
			childLeft = iPosLeft + (iPosRight - iPosLeft + rcMargin.left - rcMargin.right - sz.cx) / 2;
			childRight = childLeft + sz.cx;
		}

		//���ÿؼ���λ��
		UiRect controlRect(childLeft, iPosY + rcMargin.top, childRight, iPosY + rcMargin.top + sz.cy);		
		pControl->SetPos(controlRect);
		cxNeeded = std::max(cxNeeded, (int64_t)controlRect.Width());

		//������ǰY������ֵ
		iPosY += (sz.cy + rcMargin.top + rcMargin.bottom + GetChildMarginY());
		cyNeeded += (sz.cy + rcMargin.top + rcMargin.bottom);
	}
	if (!itemsMap.empty()) {
		cyNeeded += ((int64_t)itemsMap.size() - 1) * GetChildMarginY();
	}

	UiSize64 size(cxNeeded, cyNeeded);
	return size;
}

UiSize VLayout::EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable)
{
	UiSize totalSize;
	UiSize itemSize;
	int32_t estimateCount = 0;
	for(Control* pControl : items)	{
		if ((pControl == nullptr) || !pControl->IsVisible() || pControl->IsFloat()) {
			continue;
		}

		estimateCount++;
		UiRect rcMargin = pControl->GetMargin();
		itemSize = pControl->EstimateSize(szAvailable);
		if (itemSize.cx < pControl->GetMinWidth()) {
			itemSize.cx = pControl->GetMinWidth();
		}
		if (itemSize.cx > pControl->GetMaxWidth()) {
			itemSize.cx = pControl->GetMaxWidth();
		}
		if (itemSize.cy < pControl->GetMinHeight()) {
			itemSize.cy = pControl->GetMinHeight();
		}
		if (itemSize.cy > pControl->GetMaxHeight()) {
			itemSize.cy = pControl->GetMaxHeight();
		}
		totalSize.cx = std::max(itemSize.cx + rcMargin.left + rcMargin.right, totalSize.cx);
		totalSize.cy += (itemSize.cy + rcMargin.top + rcMargin.bottom);
	}

	if ((estimateCount - 1) > 0) {
		totalSize.cy += (estimateCount - 1) * GetChildMarginY();
	}
	UiRect rcPadding = GetPadding();
	totalSize.cx += (rcPadding.left + rcPadding.right);
	totalSize.cy += (rcPadding.top + rcPadding.bottom);
	return totalSize;
}

} // namespace ui
