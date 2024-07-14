#include "multi_browser_form.h"
#include "browser_box.h"
#include "multi_browser_manager.h"

#include "duilib/Utils/BitmapHelper.h"

#include "OleIdl.h"
#include "ShObjIdl.h"
#include <shlobj.h>

using namespace ui;

namespace
{
    const int kDragImageWidth = 300;
    const int kDragImageHeight = 300;
}

bool MultiBrowserForm::InitDragDrop()
{
    if (NULL != drop_helper_)
        return false;

    if (FAILED(CoCreateInstance(CLSID_DragDropHelper, NULL,
        CLSCTX_INPROC_SERVER,
        IID_IDropTargetHelper,
        (void**)&drop_helper_)))
    {
        return false;
    }

    if (FAILED(::RegisterDragDrop(this->NativeWnd()->GetHWND(), this)))
    {
        return false;
    }

    return true;
}

void MultiBrowserForm::UnInitDragDrop()
{
    if (NULL != drop_helper_)
        drop_helper_->Release();

    RevokeDragDrop(this->NativeWnd()->GetHWND());
}

HRESULT MultiBrowserForm::QueryInterface(REFIID iid, void ** ppvObject)
{
    if (NULL == drop_helper_)
        return E_NOINTERFACE;

    return drop_helper_->QueryInterface(iid, ppvObject);
}

ULONG MultiBrowserForm::AddRef(void)
{
    if (NULL == drop_helper_)
        return 0;

    return drop_helper_->AddRef();
}

ULONG MultiBrowserForm::Release(void)
{
    if (NULL == drop_helper_)
        return 0;

    return drop_helper_->Release();
}

HRESULT MultiBrowserForm::DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
    if (NULL == drop_helper_)
        return S_OK;

    // 如果不是拖拽浏览器盒子
    if (!MultiBrowserManager::GetInstance()->IsDragingBorwserBox())
    {
        if (NULL != active_browser_box_)
        {
            //active_browser_box_->DragEnter(pDataObject, grfKeyState, pt, pdwEffect);
            if (IsWindowMinimized()) {
                ShowWindow(kSW_RESTORE);
            }
            else {
                ShowWindow(kSW_SHOW);
            }
        }
    }
    else
    {
        *pdwEffect = DROPEFFECT_MOVE;
    }

    drop_helper_->DragEnter(this->NativeWnd()->GetHWND(), pDataObject, (LPPOINT)&pt, *pdwEffect);
    return S_OK;
}

HRESULT MultiBrowserForm::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    if (NULL == drop_helper_)
        return S_OK;

    // 如果不是拖拽浏览器盒子
    if (!MultiBrowserManager::GetInstance()->IsDragingBorwserBox())
    {
        if (NULL != active_browser_box_)
        {
            //active_browser_box_->DragOver(grfKeyState, pt, pdwEffect);
        }
    }
    else
    {
        *pdwEffect = DROPEFFECT_MOVE;
    }

    drop_helper_->DragOver((LPPOINT)&pt, *pdwEffect);
    return S_OK;
}

HRESULT MultiBrowserForm::DragLeave(void)
{
    if (NULL == drop_helper_)
        return S_OK;

    // 如果不是拖拽浏览器盒子
    if (!MultiBrowserManager::GetInstance()->IsDragingBorwserBox())
    {
        if (NULL != active_browser_box_)
        {
            //active_browser_box_->DragLeave();
        }
    }

    drop_helper_->DragLeave();
    return S_OK;
}

HRESULT MultiBrowserForm::Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect)
{
    // 如果不是拖拽浏览器盒子
    if (!MultiBrowserManager::GetInstance()->IsDragingBorwserBox())
    {
#if 0
         if (NULL != active_browser_box_ && active_browser_box_->CheckDropEnable(pt))
         {
             active_browser_box_->Drop(pDataObj, grfKeyState, pt, pdwEffect);
         }
#endif
    }
    else
    {
        *pdwEffect = DROPEFFECT_MOVE;
        MultiBrowserManager::GetInstance()->SetDropForm(this);
    }

    drop_helper_->Drop(pDataObj, (LPPOINT)&pt, *pdwEffect);
    return S_OK;
}

bool MultiBrowserForm::OnProcessTabItemDrag(const ui::EventArgs& param)
{
    if (!MultiBrowserManager::GetInstance()->IsEnableMerge())
        return true;

    switch (param.eventType)
    {
    case kEventMouseMove:
    {
        if (::GetKeyState(VK_LBUTTON) >= 0)
            break;

        LONG cx = abs(param.ptMouse.x - old_drag_point_.x);
        LONG cy = abs(param.ptMouse.y - old_drag_point_.y);

        if (!is_drag_state_ && (cx > 5 || cy > 5))
        {
            if (NULL == active_browser_box_)
                break;

            is_drag_state_ = true;

            // 把被拖拽的浏览器盒子生成一个宽度300的位图
            IBitmap* pBitmap = nullptr;
            if (nim_comp::CefManager::GetInstance()->IsEnableOffsetRender()) {
                pBitmap = GenerateBoxOffsetRenderBitmap(borwser_box_tab_->GetPos());
            }
            else {
                pBitmap = GenerateBoxWindowBitmap();
            }
            HBITMAP hBitmap = ui::BitmapHelper::CreateGDIBitmap(pBitmap);
            if (pBitmap != nullptr) {
                delete pBitmap;
                pBitmap = nullptr;
            }
            // pt应该指定相对bitmap位图的左上角(0,0)的坐标,这里设置为bitmap的中上点
            POINT pt = { kDragImageWidth / 2, 0 };

            StdClosure cb = [=]{
                MultiBrowserManager::GetInstance()->DoDragBorwserBox(active_browser_box_, hBitmap, pt);
                ::DeleteObject(hBitmap);
            };
            ui::GlobalManager::Instance().Thread().PostTask(kThreadUI, cb);
        }
    }
    break;
    case kEventMouseButtonDown:
    {
        old_drag_point_ = { param.ptMouse.x, param.ptMouse.y };
        is_drag_state_ = false;
    }
    break;
    }
    return true;
}

ui::IBitmap* MultiBrowserForm::GenerateBoxOffsetRenderBitmap(const UiRect& src_rect)
{
    ASSERT(!src_rect.IsEmpty());
    int src_width = src_rect.right - src_rect.left;
    int src_height = src_rect.bottom - src_rect.top;

    std::unique_ptr<IRender> render;
    IRenderFactory* pRenderFactory = GlobalManager::Instance().GetRenderFactory();
    ASSERT(pRenderFactory != nullptr);
    if (pRenderFactory != nullptr) {
        render.reset(pRenderFactory->CreateRender(GetRenderDpi()));
    }
    ASSERT(render != nullptr);
    if (render->Resize(kDragImageWidth, kDragImageHeight)) {
        int dest_width = 0;
        int dest_height = 0;
        float scale = (float)src_width / (float)src_height;
        if (scale >= 1.0)
        {
            dest_width = kDragImageWidth;
            dest_height = (int)(kDragImageWidth * (float)src_height / (float)src_width);
        }
        else
        {
            dest_height = kDragImageHeight;
            dest_width = (int)(kDragImageHeight * (float)src_width / (float)src_height);
        }

        render->AlphaBlend((kDragImageWidth - dest_width) / 2, 0, dest_width, dest_height, 
                           this->GetRender(),
                           src_rect.left, src_rect.top, src_rect.right - src_rect.left, src_rect.bottom - src_rect.top);
    }

    return render->MakeImageSnapshot();

}

ui::IBitmap* MultiBrowserForm::GenerateBoxWindowBitmap()
{
    if (!active_browser_box_)
        return NULL;

    HWND cef_window = active_browser_box_->GetCefControl()->GetCefHandle();
    RECT src_rect = {0, };
    ::GetClientRect(cef_window, &src_rect);
    
    int src_width = src_rect.right - src_rect.left;
    int src_height = src_rect.bottom - src_rect.top;

    //创建一个内存DC
    HDC cef_window_dc = ::GetDC(cef_window);
    ui::IBitmap* pBitmap = ui::BitmapHelper::CreateBitmapObject(kDragImageWidth, kDragImageHeight, cef_window_dc, src_width, src_height);
    ::ReleaseDC(cef_window, cef_window_dc);
    return pBitmap;
}
