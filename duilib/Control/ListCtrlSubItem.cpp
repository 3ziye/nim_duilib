#include "ListCtrlSubItem.h"
#include "duilib/Control/ListCtrl.h"

namespace ui
{
ListCtrlSubItem::ListCtrlSubItem(Window* pWindow):
    ListCtrlLabel(pWindow),
    m_pItem(nullptr),
    m_imageId(-1),
    m_nIconSpacing(-1)
{
}

std::wstring ListCtrlSubItem::GetType() const { return L"ListCtrlSubItem"; }

void ListCtrlSubItem::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
    if (strName == L"icon_spacing") {
        SetIconSpacing(_wtoi(strValue.c_str()), true);
    }
    else {
        __super::SetAttribute(strName, strValue);
    }
}

void ListCtrlSubItem::ChangeDpiScale(uint32_t nOldDpiScale, uint32_t nNewDpiScale)
{
    ASSERT(nNewDpiScale == Dpi().GetScale());
    if (nNewDpiScale != Dpi().GetScale()) {
        return;
    }
    int32_t iValue = GetIconSpacing();
    iValue = Dpi().GetScaleInt(iValue, nOldDpiScale);
    SetIconSpacing(iValue, false);
    __super::ChangeDpiScale(nOldDpiScale, nNewDpiScale);
}

void ListCtrlSubItem::SetListCtrlItem(ListCtrlItem* pItem)
{
    m_pItem = pItem;
}

ListCtrlItem* ListCtrlSubItem::GetListCtrlItem() const
{
    ASSERT(m_pItem != nullptr);
    return m_pItem;
}

bool ListCtrlSubItem::SetCheckBoxVisible(bool bVisible)
{
    bool bRet = false;
    if (bVisible) {
        ListCtrlItem* pItem = GetListCtrlItem();
        if (pItem == nullptr) {
            return false;
        }
        ListCtrlCheckBox* pCheckBox = nullptr;
        if (GetItemCount() > 0) {
            pCheckBox = dynamic_cast<ListCtrlCheckBox*>(GetItemAt(0));
        }
        if (pCheckBox == nullptr) {
            pCheckBox = new ListCtrlCheckBox(GetWindow());
            AddItem(pCheckBox);
            ListCtrl* pListCtrl = pItem->GetListCtrl();            
            std::wstring checkBoxClass;
            if (pListCtrl != nullptr) {
                checkBoxClass = pListCtrl->GetCheckBoxClass();
            }
            ASSERT(!checkBoxClass.empty());
            pCheckBox->SetClass(checkBoxClass);
        }
        //�����ڱ߾࣬�����������ص�
        UiPadding textPadding = GetTextPadding();
        int32_t nCheckBoxWidth = pCheckBox->GetCheckBoxWidth();
        if ((nCheckBoxWidth > 0) && (textPadding.left < nCheckBoxWidth)) {
            textPadding.left = nCheckBoxWidth;
            SetTextPadding(textPadding, false);
        }
        pCheckBox->SetVisible(true);
    }
    else if (GetItemCount() > 0) {
        ListCtrlCheckBox* pCheckBox = dynamic_cast<ListCtrlCheckBox*>(GetItemAt(0));
        if (pCheckBox != nullptr) {
            UiPadding textPadding = GetTextPadding();
            int32_t nCheckBoxWidth = pCheckBox->GetCheckBoxWidth();
            if ((nCheckBoxWidth > 0) && (textPadding.left >= nCheckBoxWidth)) {
                textPadding.left -= nCheckBoxWidth;
                SetTextPadding(textPadding, false);
            }
            RemoveItemAt(0);
            bRet = true;
        }
    }
    return bRet;
}

bool ListCtrlSubItem::IsCheckBoxVisible() const
{
    if (GetItemCount() > 0) {
        CheckBox* pCheckBox = dynamic_cast<CheckBox*>(GetItemAt(0));
        if (pCheckBox != nullptr) {
            return pCheckBox->IsVisible();
        }
    }
    return false;
}

bool ListCtrlSubItem::SetCheckBoxSelect(bool bSelected, bool bPartSelect)
{
    if (GetItemCount() > 0) {
        CheckBox* pCheckBox = dynamic_cast<CheckBox*>(GetItemAt(0));
        if (pCheckBox != nullptr) {
            bool bChanged = pCheckBox->IsSelected() != bSelected;
            pCheckBox->SetSelected(bSelected);
            if (bSelected) {
                if (pCheckBox->IsPartSelected() != bPartSelect) {
                    pCheckBox->SetPartSelected(bPartSelect);
                    bChanged = true;
                }
            }
            if (bChanged) {
                pCheckBox->Invalidate();
            }
            return true;
        }
    }
    return false;
}

bool ListCtrlSubItem::GetCheckBoxSelect(bool& bSelected, bool& bPartSelect) const
{
    bSelected = false;
    bPartSelect = false;
    if (GetItemCount() > 0) {
        CheckBox* pCheckBox = dynamic_cast<CheckBox*>(GetItemAt(0));
        if (pCheckBox != nullptr) {
            bSelected = pCheckBox->IsSelected();
            if (bSelected) {
                bPartSelect = pCheckBox->IsPartSelected();
            }
            return true;
        }
    }
    return false;
}

ListCtrlCheckBox* ListCtrlSubItem::GetCheckBox() const
{
    ListCtrlCheckBox* pCheckBox = nullptr;
    if (GetItemCount() > 0) {
        pCheckBox = dynamic_cast<ListCtrlCheckBox*>(GetItemAt(0));
    }
    return pCheckBox;
}

void ListCtrlSubItem::SetImageId(int32_t imageId)
{
    if (m_imageId != imageId) {
        m_imageId = imageId;
        Invalidate();
    }
}

int32_t ListCtrlSubItem::GetImageId() const
{
    return m_imageId;
}

void ListCtrlSubItem::SetIconSpacing(int32_t nIconSpacing, bool bNeedDpiScale)
{
    if (bNeedDpiScale) {
        Dpi().ScaleInt(nIconSpacing);
    }
    if (m_nIconSpacing != nIconSpacing) {
        m_nIconSpacing = nIconSpacing;
        if (m_nIconSpacing < 0) {
            m_nIconSpacing = 0;
        }
        Invalidate();
    }
}

int32_t ListCtrlSubItem::GetIconSpacing() const
{
    int32_t nIconSpacing = m_nIconSpacing;
    if (nIconSpacing < 0) {
        nIconSpacing = Dpi().GetScaleInt(2);
    }
    return nIconSpacing;
}

ImagePtr ListCtrlSubItem::LoadItemImage() const
{
    ImagePtr pItemImage;
    if ((m_imageId >= 0) && (m_pItem != nullptr)) {
        ListCtrl* pListCtrl = m_pItem->GetListCtrl();
        if (pListCtrl != nullptr) {
            ImageList* pImageList = pListCtrl->GetImageList(ListCtrlType::Report);
            if (pImageList != nullptr) {
                pItemImage = pImageList->GetImageData(m_imageId);
                ASSERT(pItemImage != nullptr);
            }
        }
    }
    if (pItemImage != nullptr) {
        LoadImageData(*pItemImage);
        std::shared_ptr<ImageInfo> pItemImageCache = pItemImage->GetImageCache();
        if (pItemImageCache == nullptr) {
            pItemImage = nullptr;
        }
        else {
            if ((pItemImageCache->GetWidth() <= 0) ||
                (pItemImageCache->GetHeight() <= 0)) {
                pItemImage = nullptr;
            }
        }
    }
    return pItemImage;
}

void ListCtrlSubItem::PaintText(IRender* pRender)
{
    //��Ҫ���Ƶ����ݰ�����ͼ�ꡢ����
    SetTextRect(UiRect());
    if (pRender == nullptr) {
        return;
    }
    //�ı�ǰ��ͼ��
    ImagePtr pItemImage = LoadItemImage();
    if (pItemImage == nullptr) {
        __super::PaintText(pRender);
        return;
    }

    int32_t nIconTextSpacing = GetIconSpacing();

    //CheckBox�Ŀ�ȣ���Ҫ������
    int32_t nCheckBoxWidth = 0;
    ListCtrlCheckBox* pCheckBox = GetCheckBox();
    if ((pCheckBox != nullptr) && pCheckBox->IsVisible()) {
        UiSize sz = pCheckBox->GetStateImageSize(kStateImageBk, kControlStateNormal);
        nCheckBoxWidth += sz.cx;
        nCheckBoxWidth += nIconTextSpacing;
    }

    UiSize imageSize;
    ListCtrl* pListCtrl = nullptr;
    if (m_pItem != nullptr) {
        pListCtrl = m_pItem->GetListCtrl();
    }
    if (pListCtrl != nullptr) {
        ImageList* pImageList = pListCtrl->GetImageList(ListCtrlType::Report);
        if (pImageList != nullptr) {
            imageSize = pImageList->GetImageSize();
        }
    }

    if (imageSize.cx <= 0) {
        imageSize.cx = pItemImage->GetImageCache()->GetWidth();
    }
    if (imageSize.cy <= 0) {
        imageSize.cy = pItemImage->GetImageCache()->GetHeight();
    }

    //ͼ�꿿��࣬���ְ�ԭ���ķ�ʽ����
    UiRect rc = GetRect();
    rc.Deflate(GetControlPadding());
    rc.left += nCheckBoxWidth;
    rc.Validate();
    rc.right = rc.left + imageSize.cx;

    UiRect imageRect = rc;
    VAlignRect(imageRect, GetTextStyle(), imageSize.cy);
    PaintImage(pRender, pItemImage.get(), L"", -1, nullptr, &imageRect, nullptr);
    rc.left += imageSize.cx;
    rc.left += nIconTextSpacing;

    UiRect textRect = GetRect();
    textRect.Deflate(GetControlPadding());
    textRect.Deflate(GetTextPadding());
    textRect.left = std::max(textRect.left, rc.left);

    DoPaintText(textRect, pRender);
    SetTextRect(textRect);
}

void ListCtrlSubItem::VAlignRect(UiRect& rc, uint32_t textStyle, int32_t nImageHeight)
{
    if ((nImageHeight <= 0) || (nImageHeight >= rc.Height())) {
        return;
    }
    if (textStyle & TEXT_VCENTER) {
        //���ж���
        rc.top = rc.CenterY() - nImageHeight / 2;
        rc.bottom = rc.top + nImageHeight;
    }
    else if (textStyle & TEXT_BOTTOM) {
        //�ײ�����
        rc.top = rc.bottom - nImageHeight;
    }
    else {
        //��������
        rc.bottom = rc.top + nImageHeight;
    }
}

UiSize ListCtrlSubItem::EstimateText(UiSize szAvailable)
{
    UiSize sz = __super::EstimateText(szAvailable);

    int32_t nIconTextSpacing = GetIconSpacing();

    //CheckBox�Ŀ�ȣ���Ҫ������
    int32_t nCheckBoxWidth = 0;
    ListCtrlCheckBox* pCheckBox = GetCheckBox();
    if ((pCheckBox != nullptr) && pCheckBox->IsVisible()) {
        UiSize imageSize = pCheckBox->GetStateImageSize(kStateImageBk, kControlStateNormal);
        nCheckBoxWidth += imageSize.cx;
        nCheckBoxWidth += nIconTextSpacing;
    }

    const UiPadding rcPadding = GetControlPadding();
    const UiPadding rcTextPadding = GetTextPadding();

    //ͼ�꿿��࣬���ְ�ԭ���ķ�ʽ����
    UiRect rc = GetRect();
    rc.Deflate(rcPadding);
    rc.left += nCheckBoxWidth;
    rc.Validate();

    ImagePtr pItemImage = LoadItemImage();
    if (pItemImage != nullptr) {
        rc.left += pItemImage->GetImageCache()->GetWidth();
        rc.left += nIconTextSpacing;
    }   

    UiRect textRect = GetRect();
    textRect.Deflate(rcPadding);
    textRect.Deflate(rcTextPadding);
    textRect.left = std::max(textRect.left, rc.left);

    int32_t nPaddingLeft = rcPadding.left + rcTextPadding.left;
    if (textRect.left > nPaddingLeft) {
        sz.cx -= nPaddingLeft;
        sz.cx += textRect.left;
    }    
    return sz;
}

}//namespace ui

