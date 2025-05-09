#include "HTileLayout.h"
#include "duilib/Core/Box.h"
#include "duilib/Utils/AttributeUtil.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Utils/StringUtil.h"
#include <numeric>

namespace ui 
{
HTileLayout::HTileLayout():
    m_nRows(0), 
    m_szItem(0, 0),
    m_bAutoCalcRows(false),
    m_bScaleDown(true),
    m_bAutoCalcItemHeight(false)
{
}

UiSize HTileLayout::CalcEstimateSize(Control* pControl, const UiSize& szItem, const UiRect& rc)
{
    if ((pControl == nullptr) || !pControl->IsVisible()) {
        return UiSize();
    }
    
    //估算的可用区域，始终使用总区域
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

void HTileLayout::CalcTileRows(const std::vector<ItemSizeInfo>& normalItems, const UiRect& rc,
                                 int32_t tileHeight, int32_t childMarginX, int32_t childMarginY,
                                 int32_t& nRows)
{
    nRows = 0;
    if (tileHeight <= 0) {
        //需要先计算瓦片控件高度，然后根据宽度计算行数
        int32_t maxHeight = 0;
        int64_t areaTotal = 0;
        const int64_t maxArea = (int64_t)rc.Width() * rc.Height();
        for (const ItemSizeInfo& itemSizeInfo : normalItems) {
            UiMargin rcMargin = itemSizeInfo.pControl->GetMargin();
            UiSize childSize(itemSizeInfo.cx, itemSizeInfo.cy);
            if (childSize.cy > 0) {
                maxHeight = std::max(maxHeight, childSize.cy + rcMargin.top + rcMargin.bottom);
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
                //按当前可视区展示的面积估算
                break;
            }
        }
        //取可视区控件高度最大值，作为每个Item的高度
        tileHeight = maxHeight;
    }
    if (tileHeight > 0) {
        //使用设置的高度作为瓦片控件的宽度，并通过设置的高度，计算行数
        int32_t totalHeight = rc.Height();
        while (totalHeight > 0) {
            totalHeight -= tileHeight;
            if (nRows != 0) {
                totalHeight -= childMarginX;
            }
            if (totalHeight >= 0) {
                ++nRows;
            }
        }
    }
}

UiSize64 HTileLayout::EstimateFloatSize(Control* pControl, const UiRect& rc)
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

UiSize64 HTileLayout::ArrangeFloatChild(const std::vector<Control*>& items,
                                       const UiRect& rc,
                                       const UiSize& szItem,
                                       bool isCalcOnly,
                                       std::vector<ItemSizeInfo>& normalItems)
{
    int64_t cxNeededFloat = 0;    //浮动控件需要的总宽度
    int64_t cyNeededFloat = 0;    //浮动控件需要的总高度
    for (Control* pControl : items) {
        if ((pControl == nullptr) || !pControl->IsVisible()) {
            continue;
        }
        if (pControl->IsFloat()) {
            //浮动控件
            UiSize64 floatSize;
            if (!isCalcOnly) {
                //设置浮动控件的位置
                floatSize = SetFloatPos(pControl, rc);
            }
            else {
                //计算Float控件的大小
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
            //普通控件
            UiSize childSize = CalcEstimateSize(pControl, szItem, rc);
            if ((childSize.cx <= 0) || (childSize.cy <= 0)) {
                //大小为0的，不可显示控件(可能是拉伸控件)
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

int32_t HTileLayout::CalcTileColumnWidth(const std::vector<ItemSizeInfo>& normalItems,
                                         const std::vector<ItemSizeInfo>::const_iterator iterBegin,
                                         int32_t nRows,
                                         const UiSize& szItem)
{
    //szItem的宽度和高度值，是包含了控件的外边距和内边距的
    ASSERT(nRows > 0);
    int32_t cyWidth = szItem.cx;
    if (cyWidth > 0) {
        //如果设置了宽度，则优先使用设置的宽度值
        return cyWidth;
    }
    if (nRows <= 0) {
        return 0;
    }

    int32_t iIndex = 0;
    for (auto it = iterBegin; it != normalItems.end(); ++it) {
        const ItemSizeInfo& itemSizeInfo = *it;
        UiMargin rcMargin = itemSizeInfo.pControl->GetMargin();
        UiSize szTile(itemSizeInfo.cx, itemSizeInfo.cy);

        //保留高度最大值
        if (szTile.cx > 0) {
            cyWidth = std::max(cyWidth, szTile.cx + rcMargin.left + rcMargin.right);
        }

        ++iIndex;
        if ((iIndex % nRows) == 0) {
            //换行，退出
            break;
        }
    }
    return cyWidth;
}

UiSize HTileLayout::CalcTilePosition(const ItemSizeInfo& itemSizeInfo,
                                    int32_t tileWidth, int32_t tileHeight,
                                    const UiPoint& ptTile, bool bScaleDown, UiRect& szTilePos)
{
    szTilePos.Clear();
    //目标区域大小（宽和高）
    UiSize szItem(tileWidth, tileHeight);
    szItem.Validate();

    //瓦片控件大小(宽和高), 包含外边距
    UiMargin rcMargin = itemSizeInfo.pControl->GetMargin();
    UiSize childSize(itemSizeInfo.cx + rcMargin.left + rcMargin.right, 
                     itemSizeInfo.cy + rcMargin.top + rcMargin.bottom);
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
    
    int32_t cxWidth = szItem.cx;    //每个控件（瓦片）的宽度（动态计算值）
    int32_t cyHeight = szItem.cy;    //每个控件（瓦片）的高度（动态计算值）

    //目标区域矩（左上角坐标，宽和高）
    UiRect rcTile(ptTile.x, ptTile.y, ptTile.x + cxWidth, ptTile.y + cyHeight);

    //对控件进行等比例缩放(缩放的时候，需要去掉外边距)
    UiRect rcRealTile = rcTile;
    rcRealTile.Deflate(rcMargin);
    UiSize realSize(childSize.cx - rcMargin.left - rcMargin.right,
                    childSize.cy - rcMargin.top - rcMargin.bottom);
    if (bScaleDown && 
        (rcRealTile.Width() > 0) && (rcRealTile.Height() > 0) &&
        (realSize.cx > 0) && (realSize.cy > 0) ) {
        if ((realSize.cx > rcRealTile.Width()) || (realSize.cy > rcRealTile.Height())) {
            //满足缩放条件，进行等比缩放
            double cx = realSize.cx;
            double cy = realSize.cy;
            double cxRatio = cx / rcRealTile.Width();
            double cyRatio = cy / rcRealTile.Height();
            if (cxRatio > cyRatio) {
                ASSERT(realSize.cx > rcRealTile.Width());
                double ratio = cx / realSize.cy;
                realSize.cx = rcRealTile.Width();
                realSize.cy = static_cast<int32_t>(realSize.cx / ratio + 0.5);
            }
            else {
                ASSERT(realSize.cy > rcRealTile.Height());
                double ratio = cy / realSize.cx;
                realSize.cy = rcRealTile.Height();
                realSize.cx = static_cast<int32_t>(realSize.cy / ratio + 0.5);
            }
        }
    }

    //rcTile包含外边距，realSize不包含外边距
    szTilePos = GetFloatPos(itemSizeInfo.pControl, rcTile, realSize);
    if (szTilePos.left < ptTile.x) {
        //如果控件较大，超过边界，则靠左对齐
        int32_t width = szTilePos.Width();
        szTilePos.left = ptTile.x;
        szTilePos.right = ptTile.x + width;
    }
    if (szTilePos.top < ptTile.y) {
        //如果控件较大，超过边界，则靠上对齐
        int32_t height = szTilePos.Height();
        szTilePos.top = ptTile.y;
        szTilePos.bottom = ptTile.y + height;
    }
    return UiSize(cxWidth, cyHeight);
}

UiSize64 HTileLayout::ArrangeChild(const std::vector<Control*>& items, UiRect rc)
{
    //总体布局策略：
    // (1) 横向尽量不超出边界（除非行首的第一个元素大小比rc宽，这种情况下横向会超出边界），
    //     纵向可能会出现超出边界。
    // (2) 瓦片的宽高比：默认保持宽高比进行缩小，以适应目标显示区(可用m_bScaleDown控制此行为)。
    //     如果宽度或者高度出现了缩放，需要保持宽高比，避免出现变形；
    //     但会提供一个选项SetScaleDown()，不保持宽高比，这种情况下，会有超出边界的现象。
    // (3) 对于不是自由模式的情况，如果m_bAutoCalcColumns为true，则m_nColumns被置零
    //布局的几种用例:
    // (1) !m_bAutoCalcRows && (m_nRows == 0) && (m_szItem.cx == 0)
    //     布局策略：    1、列数：自由布局，不分列，每行是要输出到边界，就换行（每行的列数可能都不同）；
    //                2、瓦片控件的宽度：按其实际宽度展示；
    //              3、瓦片控件的高度：
    //                （1）如果m_szItem.cy > 0：限制为固定m_szItem.cy
    //                （2）如果m_szItem.cy == 0: 按其实际高度展示；
    // (2) (m_nRows == 0) && (m_szItem.cx > 0)
    //     布局策略：    1、列数：按照 rc.Width() 与 m_szItem.cx 来计算应该分几列，列数固定；
    //                2、瓦片控件的宽度：固定为 m_szItem.cx；
    //              3、瓦片控件的高度：
    //                （1）如果m_szItem.cy > 0：限制为固定m_szItem.cy
    //                （2）如果m_szItem.cy == 0: 按其实际高度展示；
    // (3) (m_nRows > 0) && (m_szItem.cx == 0)
    //     布局策略：    1、列数：列数固定为m_nColumns；
    //                2、瓦片控件的宽度：按其实际宽度展示；
    //              3、瓦片控件的高度：
    //                （1）如果m_szItem.cy > 0：限制为固定m_szItem.cy
    //                （2）如果m_szItem.cy == 0: 按其实际高度展示；
    // (4) (m_nRows > 0) && (m_szItem.cx > 0)
    //     布局策略：    1、列数：列数固定为m_nColumns；
    //                2、瓦片控件的宽度：固定为 m_szItem.cx；
    //              3、瓦片控件的高度：
    //                （1）如果m_szItem.cy > 0：限制为固定m_szItem.cy
    //                （2）如果m_szItem.cy == 0: 按其实际高度展示；

    if (IsFreeLayout()) {
        //使用自由布局排列控件(无固定行数，尽量充分利用展示空间，显示尽可能多的内容)
        return ArrangeChildFreeLayout(items, rc, false);
    }
    else {
        if ((GetRows() > 0) && IsAutoCalcItemHeight()) {
            //设置了固定行，并且设置了自动计算子项高度
            UiRect rect = rc;
            DeflatePadding(rect); //剪去内边距，剩下的是可用区域
            int32_t nNewItemHeight = 0;
            if (AutoCalcItemHeight(GetRows(), GetChildMarginY(), rect.Height(), nNewItemHeight)) {
                UiSize szNewItemSize = GetItemSize();
                szNewItemSize.cy = nNewItemHeight;
                SetItemSize(szNewItemSize, false);
            }
        }
        std::vector<int32_t> inRowHeights;
        std::vector<int32_t> outRowHeights;
        ArrangeChildNormal(items, rc, true, inRowHeights, outRowHeights);
        inRowHeights.swap(outRowHeights);
        return ArrangeChildNormal(items, rc, false, inRowHeights, outRowHeights);
    }
}

bool HTileLayout::IsFreeLayout() const
{
    return (!m_bAutoCalcRows && (m_nRows == 0) && (m_szItem.cx == 0) && m_szItem.cy == 0);
}

UiSize64 HTileLayout::ArrangeChildNormal(const std::vector<Control*>& items,
                                        UiRect rect,
                                        bool isCalcOnly,
                                        const std::vector<int32_t>& inRowHeights,
                                        std::vector<int32_t>& outRowHeights) const
{
    ASSERT(!IsFreeLayout());
    DeflatePadding(rect); //剪去内边距，剩下的是可用区域
    const UiRect& rc = rect;

    //调整浮动控件，过滤隐藏控件、不可显示控件等
    //拉伸类型的子控件：如果(m_szItem.cx > 0) && (m_szItem.cy > 0) 为true，则可以显示，否则会被过滤掉
    std::vector<ItemSizeInfo> normalItems;
    ArrangeFloatChild(items, rc, m_szItem, isCalcOnly, normalItems); //浮动控件需要的总宽度和高度

    int32_t nRows = m_nRows;  //行数（设置值）
    if (m_bAutoCalcRows) {
        //如果自动计算列数，则重新计算行数
        nRows = 0;
    }
    if (nRows <= 0) {
        CalcTileRows(normalItems, rc, m_szItem.cy, GetChildMarginX(), GetChildMarginY(), nRows);
    }
    if (nRows < 1) {
        //无法精确计算时，默认值设置为1
        nRows = 1;
    }

    //列宽设置, 固定值
    std::vector<int32_t> fixedRowHeights = inRowHeights;
    if (m_szItem.cy > 0) {
        fixedRowHeights.clear();
        fixedRowHeights.resize(nRows, m_szItem.cy);
    }

    int32_t cxColumnWidth = 0;         //每列控件（瓦片）的宽度（动态计算值）

    std::vector<int32_t> rowHeights;   //每行的高度值，计算值
    std::vector<int32_t> columnWidths; //每列的宽度值，计算值
    rowHeights.resize(nRows);
    columnWidths.resize(1);

    int32_t nColumnTileCount = 0;      //本列容纳的瓦片控件个数
    int32_t nColumnIndex = 0;          //当前的列号

    int32_t yPosTop = rc.top;
    //控件显示内容的上侧坐标值(纵向区域居中对齐)
    if (!isCalcOnly && !fixedRowHeights.empty()) {
        int32_t cyTotal = std::accumulate(fixedRowHeights.begin(), fixedRowHeights.end(), 0);
        if (fixedRowHeights.size() > 1) {
            cyTotal += ((int32_t)fixedRowHeights.size() - 1) * GetChildMarginX();
        }
        if (cyTotal < rc.Height()) {
            yPosTop = rc.CenterY() - cyTotal / 2;
        }
    }

    UiPoint ptTile(rc.left, yPosTop);    //每个控件（瓦片）的顶点坐标    
    for( auto it = normalItems.begin(); it != normalItems.end(); ++it ) {
        const ItemSizeInfo& itemSizeInfo = *it;
        Control* pControl = itemSizeInfo.pControl;
        if (pControl == nullptr) {
            continue;
        }
        if (nColumnTileCount == 0) {
            //一列的开始，计算这一行的高度
            nColumnTileCount = nRows;
            cxColumnWidth = CalcTileColumnWidth(normalItems, it, nRows, m_szItem);
            ASSERT(cxColumnWidth > 0);//不可能是零
        }
        
        const int32_t rowIndex = nRows - nColumnTileCount;//当前列下标[0, nRows)
        int32_t fixedRowHeight = 0; //当前传入的行高宽度, 固定值
        if (((int32_t)fixedRowHeights.size() == nRows) &&
            (rowIndex < (int32_t)fixedRowHeights.size())) {
            fixedRowHeight = fixedRowHeights[rowIndex];
        }

        //计算当前瓦片控件的位置坐标、宽度(cxWidth)和高度(cyHeight)        
        UiRect rcTilePos;

        UiPoint posLeftTop = ptTile;         //该控件的左上角坐标值
        int32_t posWidth = cxColumnWidth;    //该控件的最大可用宽度
        int32_t posHeight = fixedRowHeight;  //该控件的最大可用高度
        UiSize szTileSize = CalcTilePosition(itemSizeInfo, posWidth, posHeight,
                                             posLeftTop, m_bScaleDown, rcTilePos);//返回值包含了控件的外边距
        
        if (!isCalcOnly) {
            pControl->SetPos(rcTilePos);
        }

        int32_t cyHeight = szTileSize.cy;
        if (fixedRowHeight > 0) {
            cyHeight = fixedRowHeight;
        }

        //计算本列宽度最大值，宽度值需要包含外边距
        int32_t tileWidth = (m_szItem.cx > 0) ? m_szItem.cx : szTileSize.cx;
        columnWidths[nColumnIndex] = std::max(tileWidth, columnWidths[nColumnIndex]);

        --nColumnTileCount;
        if(nColumnTileCount == 0 ) {
            //换列
            columnWidths.push_back(0);
            nColumnIndex = (int32_t)columnWidths.size() - 1;
            
            //重新设置X坐标和Y坐标的位置(行首)
            ptTile.x += cxColumnWidth + GetChildMarginX();
            //Y轴坐标切换到下一行，按行高切换
            ptTile.y = yPosTop;
        }
        else {
            //同一列，向下切换坐标，按当前瓦片控件的宽度切换
            ptTile.y += cyHeight + GetChildMarginY();
        }        
        //记录每行的高度（取这一行中，控件高度的最大值，包含此控件的外边距）
        if (rowIndex < (int32_t)rowHeights.size()) {
            int32_t tileHeight = (m_szItem.cy > 0) ? m_szItem.cy : cyHeight;    
            rowHeights[rowIndex] = std::max(tileHeight, rowHeights[rowIndex]);
        }
    }

    //由于内边距已经剪掉，计算宽度和高度的时候，需要算上内边距
    UiPadding rcPadding;
    if (GetOwner() != nullptr) {
        rcPadding = GetOwner()->GetPadding();
    }
    //计算所需宽度
    int64_t cxNeeded = std::accumulate(columnWidths.begin(), columnWidths.end(), 0);
    if (columnWidths.size() > 1) {
        cxNeeded += (columnWidths.size() - 1) * GetChildMarginX();
    }
    cxNeeded += ((int64_t)rcPadding.left + rcPadding.right);

    //计算所需高度
    int64_t cyNeeded = std::accumulate(rowHeights.begin(), rowHeights.end(), 0);
    if (rowHeights.size() > 1) {
        cyNeeded += (rowHeights.size() - 1) * GetChildMarginY();
    }
    cyNeeded += ((int64_t)rcPadding.top + rcPadding.bottom);

    outRowHeights.swap(rowHeights);
    UiSize64 size(cxNeeded, cyNeeded);
    return size;
}

UiSize64 HTileLayout::ArrangeChildFreeLayout(const std::vector<Control*>& items,
                                            UiRect rect, bool isCalcOnly) const
{
    DeflatePadding(rect); //剪去内边距，剩下的是可用区域
    const UiRect& rc = rect;

    //调整浮动控件，过滤隐藏控件、不可显示控件等
    //拉伸类型的子控件：如果(m_szItem.cx > 0) && (m_szItem.cy > 0) 为true，则可以显示，否则会被过滤掉
    std::vector<ItemSizeInfo> normalItems;
    ArrangeFloatChild(items, rc, m_szItem, isCalcOnly, normalItems); //浮动控件需要的总宽度和高度

    int64_t cxNeeded = 0;        //非浮动控件需要的总宽度    
    int64_t cyNeeded = 0;        //非浮动控件需要的总高度

    int32_t cxColumnWidth = 0;   //每列控件（瓦片）的宽度（动态计算值）

    const int32_t yPosTop = rc.top;         //控件显示内容的上侧坐标值，始终采取上对齐
    UiPoint ptTile(rc.left, yPosTop); //每个控件（瓦片）的顶点坐标
    const size_t itemCount = normalItems.size();
    for (size_t index = 0; index < itemCount; ++index) {
        const ItemSizeInfo& itemSizeInfo = normalItems[index];
        Control* pControl = itemSizeInfo.pControl;
        if (pControl == nullptr) {
            continue;
        }

        //计算当前瓦片控件的位置坐标、宽度和高度
        UiRect rcTilePos;
        UiSize szTileSize = CalcTilePosition(itemSizeInfo, 0, 0,
                                             ptTile, m_bScaleDown, rcTilePos);
        if (rcTilePos.bottom >= rc.bottom) {
            //下侧已经超过边界, 如果不是靠近最上侧，则先换列，再显示
            if (ptTile.y != yPosTop) {
                //先换列, 然后再显示
                ptTile.y = yPosTop;
                ptTile.x += cxColumnWidth + GetChildMarginX();//下一列            
                cxColumnWidth = 0;

                szTileSize = CalcTilePosition(itemSizeInfo, 0, 0,
                                              ptTile, m_bScaleDown, rcTilePos);                
            }
        }
        if (!isCalcOnly) {
            pControl->SetPos(rcTilePos);
        }

        UiMargin rcMargin = pControl->GetMargin();
        cxNeeded = std::max((int64_t)rcTilePos.right + rcMargin.right, cxNeeded);
        cyNeeded = std::max((int64_t)rcTilePos.bottom + rcMargin.bottom, cyNeeded);

        //更新控件宽度值和行高值
        int32_t cyHeight = rcTilePos.Height() + rcMargin.top + rcMargin.bottom;
        cxColumnWidth = std::max(rcTilePos.Width() + rcMargin.left + rcMargin.right, cxColumnWidth);

        if (rcTilePos.bottom >= rc.bottom) {
            //当前控件已经超出边界，需要换行
            ptTile.y = yPosTop;
            ptTile.x += cxColumnWidth + GetChildMarginX();//下一列        
            cxColumnWidth = 0;
        }
        else {
            //不换行，向后切换纵坐标
            ptTile.y += cyHeight + GetChildMarginY();
        }
    }

    //由于内边距已经剪掉，计算宽度和高度的时候，需要算上内边距
    //(只需要增加右侧和底部的内边距，因为计算的时候，是按照.rigth和.bottom计算的)
    UiPadding rcPadding;
    if (GetOwner() != nullptr) {
        rcPadding = GetOwner()->GetPadding();
    }
    cxNeeded += rcPadding.right;
    cyNeeded += rcPadding.bottom;

    if (isCalcOnly) {
        //返回的宽度，最大不超过外层容器的空间，因为此返回值会成为容器最终的宽度值
        if (cyNeeded > (rect.Height())) {
            cyNeeded = rect.Height();
        }
    }
    UiSize64 size(cxNeeded, cyNeeded);
    return size;
}

UiSize HTileLayout::EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable)
{
    szAvailable.Validate();
    UiRect rc(0, 0, szAvailable.Width(), szAvailable.Height());
    UiSize64 requiredSize;
    if (IsFreeLayout()) {
        requiredSize = ArrangeChildFreeLayout(items, rc, true);
    }
    else {
        if ((GetRows() > 0) && IsAutoCalcItemHeight()) {
            //设置了固定行，并且设置了自动计算子项高度
            UiRect rect = rc;
            DeflatePadding(rect); //剪去内边距，剩下的是可用区域
            int32_t nNewItemHeight = 0;
            if (AutoCalcItemHeight(GetRows(), GetChildMarginY(), rect.Height(), nNewItemHeight)) {
                UiSize szNewItemSize = GetItemSize();
                szNewItemSize.cy = nNewItemHeight;
                SetItemSize(szNewItemSize, false);
            }
        }
        std::vector<int32_t> inRowHeights;
        std::vector<int32_t> outRowHeights;
        requiredSize = ArrangeChildNormal(items, rc, true, inRowHeights, outRowHeights);
    }
    UiSize size(TruncateToInt32(requiredSize.cx), TruncateToInt32(requiredSize.cy));
    return size;
}

bool HTileLayout::SetAttribute(const DString& strName, const DString& strValue, const DpiManager& dpiManager)
{
    bool hasAttribute = true;
    if((strName == _T("item_size")) || (strName == _T("itemsize"))){
        UiSize szItem;
        AttributeUtil::ParseSizeValue(strValue.c_str(), szItem);
        dpiManager.ScaleSize(szItem);
        SetItemSize(szItem, true);
    }
    else if ((strName == _T("columns")) || (strName == _T("rows"))) {
        if (strValue == _T("auto")) {
            //自动计算列数
            SetAutoCalcRows(true);
        }
        else {
            SetAutoCalcRows(false);
            SetRows(StringUtil::StringToInt32(strValue));
        }
    }
    else if (strName == _T("auto_calc_item_size")) {
        SetAutoCalcItemHeight(strValue == _T("true"));
    }
    else if ((strName == _T("scale_down")) || (strName == _T("scaledown"))) {
        SetScaleDown(strValue == _T("true"));
    }
    else {
        hasAttribute = Layout::SetAttribute(strName, strValue, dpiManager);
    }
    return hasAttribute;
}

void HTileLayout::ChangeDpiScale(const DpiManager& dpiManager, uint32_t nOldDpiScale)
{
    UiSize szItem = GetItemSize();
    szItem = dpiManager.GetScaleSize(szItem, nOldDpiScale);
    SetItemSize(szItem, true);
    BaseClass::ChangeDpiScale(dpiManager, nOldDpiScale);
}

const UiSize& HTileLayout::GetItemSize() const
{
    return m_szItem;
}

void HTileLayout::SetItemSize(UiSize szItem, bool bArrange)
{
    szItem.cx = std::max(szItem.cx, 0);
    szItem.cy = std::max(szItem.cy, 0);
    if( (m_szItem.cx != szItem.cx) || (m_szItem.cy != szItem.cy) ) {
        m_szItem = szItem;
        if (bArrange && (GetOwner() != nullptr)) {
            GetOwner()->Arrange();
        }        
    }
}

int32_t HTileLayout::GetRows() const
{
    return m_nRows;
}

void HTileLayout::SetRows(int32_t nRows)
{
    nRows = std::max(nRows, 0);
    if (m_nRows != nRows) {
        m_nRows = nRows;
        if (GetOwner() != nullptr) {
            GetOwner()->Arrange();
        }
    }    
}

void HTileLayout::SetAutoCalcRows(bool bAutoCalcRows)
{
    if (m_bAutoCalcRows != bAutoCalcRows) {
        m_bAutoCalcRows = bAutoCalcRows;
        if (GetOwner() != nullptr) {
            GetOwner()->Arrange();
        }
    }
}

bool HTileLayout::IsAutoCalcRows() const
{
    return m_bAutoCalcRows;
}

void HTileLayout::SetScaleDown(bool bScaleDown)
{
    if (m_bScaleDown != bScaleDown) {
        m_bScaleDown = bScaleDown;
        if (GetOwner() != nullptr) {
            GetOwner()->Arrange();
        }
    }
}

bool HTileLayout::IsScaleDown() const
{
    return m_bScaleDown;
}

void HTileLayout::SetAutoCalcItemHeight(bool bAutoCalcItemHeight)
{
    if (m_bAutoCalcItemHeight != bAutoCalcItemHeight) {
        m_bAutoCalcItemHeight = bAutoCalcItemHeight;
        if (GetOwner() != nullptr) {
            GetOwner()->Arrange();
        }
    }
}

bool HTileLayout::IsAutoCalcItemHeight() const
{
    return m_bAutoCalcItemHeight;
}

bool HTileLayout::AutoCalcItemHeight(int32_t nRows, int32_t nMarginY, int32_t szAvailable, int32_t& nItemHeight) const
{
    if ((nRows < 1) || (szAvailable < 1)) {
        return false;
    }
    if (nMarginY < 0) {
        nMarginY = 0;
    }
    nItemHeight = (szAvailable - (nRows - 1) * nMarginY) / nRows;
    return nItemHeight > 0;
}

} // namespace ui
