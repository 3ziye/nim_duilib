#include "CefWindowUtils.h"
#include "duilib/Core/Window.h"

#ifdef DUILIB_BUILD_FOR_LINUX
//Linux OS
#include <X11/Xlib.h>

namespace ui
{
//�жϴ����Ƿ���Ч
static bool IsX11WindowValid(Display* display, ::Window window)
{
    // ���Ի�ȡ��������
    XWindowAttributes attrs;        
    Status status = XGetWindowAttributes(display, window, &attrs);
    return (status != 0);  // ����1��Ч��0��Ч
}

//����X���ڵĸ�����
class SetX11WindowParentWindowTask : public CefTask
{
    IMPLEMENT_REFCOUNTING(SetX11WindowParentWindowTask);
public:
    explicit SetX11WindowParentWindowTask(CefControl* pCefControl):
        m_pCefControl(pCefControl)
    {
        m_pCefControlFlag = pCefControl->GetWeakFlag();
    }
public:
    virtual void Execute() override
    {
        if (m_pCefControlFlag.expired()) {
            return;
        }
        CefWindowHandle hParentHandle = 0;
        Window* pWindow = m_pCefControl->GetWindow();
        if (pWindow != nullptr) {
            hParentHandle = pWindow->NativeWnd()->GetX11WindowNumber();
        }
        CefWindowHandle handle = m_pCefControl->GetCefWindowHandle();
        if ((handle != 0) && (hParentHandle != 0)) {
            Display* display = XOpenDisplay(nullptr);
            if ((display != nullptr) && IsX11WindowValid(display, handle) && IsX11WindowValid(display, hParentHandle)) {
                UiRect rc = m_pCefControl->GetPos();
                XReparentWindow(display, handle, hParentHandle, rc.left, rc.top);
                XFlush(display);
                XCloseDisplay(display);
            }
        }
    }
private:
    CefControl* m_pCefControl;
    std::weak_ptr<WeakFlag> m_pCefControlFlag;
};

//����X11���ڵ���ʾ��������
class SetX11WindowVisibleTask : public CefTask
{
    IMPLEMENT_REFCOUNTING(SetX11WindowVisibleTask);
public:
    explicit SetX11WindowVisibleTask(CefControl* pCefControl):
        m_pCefControl(pCefControl)
    {
        m_pCefControlFlag = pCefControl->GetWeakFlag();
    }
public:
    virtual void Execute() override
    {
        if (m_pCefControlFlag.expired()) {
            return;
        }
        CefWindowHandle handle = m_pCefControl->GetCefWindowHandle();
        if (handle != 0) {
            Display* display = XOpenDisplay(nullptr);
            if ((display != nullptr) && IsX11WindowValid(display, handle)){
                if (m_pCefControl->IsVisible()) {
                    XMapWindow(display, handle);
                }
                else {
                    XUnmapWindow(display, handle);
                }
                XFlush(display);
                XCloseDisplay(display);
            }
        }
    }
private:
    CefControl* m_pCefControl;
    std::weak_ptr<WeakFlag> m_pCefControlFlag;
};

//����X11���ڵ�λ�úʹ�С
class SetX11WindowPosTask : public CefTask
{
    IMPLEMENT_REFCOUNTING(SetX11WindowPosTask);
public:
    explicit SetX11WindowPosTask(CefControl* pCefControl):
        m_pCefControl(pCefControl)
    {
        m_pCefControlFlag = pCefControl->GetWeakFlag();
    }
public:
    virtual void Execute() override
    {
        if (m_pCefControlFlag.expired() || !m_pCefControl->IsVisible()) {
            //�������ص�ʱ�򣬲���Ҫ���ã�������õĻ����ᵼ�³������
            return;
        }
        CefWindowHandle handle = m_pCefControl->GetCefWindowHandle();
        ui::UiRect rc = m_pCefControl->GetPos();
        if (handle != 0) {
            Display* display = XOpenDisplay(nullptr);
            if ((display != nullptr) && IsX11WindowValid(display, handle)){
                XMoveResizeWindow(display, handle, rc.left, rc.top, rc.Width(), rc.Height());
                XFlush(display);
                XCloseDisplay(display);
            }
        }
    }
private:
    CefControl* m_pCefControl;
    std::weak_ptr<WeakFlag> m_pCefControlFlag;
};

void SetCefWindowPos(CefWindowHandle cefWindow, CefControl* pCefControl)
{
    if ((cefWindow == 0) || (pCefControl == nullptr)) {
        return;
    }
    Window* pWindow = pCefControl->GetWindow();
    if (pWindow == nullptr) {
        return;
    }
    CefPostTask(TID_UI, new SetX11WindowPosTask(pCefControl));
}

void SetCefWindowVisible(CefWindowHandle cefWindow, CefControl* pCefControl)
{
    if ((cefWindow == 0) || (pCefControl == nullptr)) {
        return;
    }
    Window* pWindow = pCefControl->GetWindow();
    if (pWindow == nullptr) {
        return;
    }
    //��Ҫ��CEF��UI�̵߳���
    CefPostTask(TID_UI, new SetX11WindowVisibleTask(pCefControl));
}

void SetCefWindowParent(CefWindowHandle cefWindow, CefControl* pCefControl)
{
    if ((cefWindow == 0) || (pCefControl == nullptr)) {
        return;
    }
    Window* pWindow = pCefControl->GetWindow();
    if (pWindow == nullptr) {
        return;
    }
    //��Ҫ��CEF��UI�̵߳���
    CefPostTask(TID_UI, new SetX11WindowParentWindowTask(pCefControl));
}

} //namespace ui

#endif //DUILIB_BUILD_FOR_LINUX
