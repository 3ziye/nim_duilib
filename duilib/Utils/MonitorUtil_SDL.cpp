#include "MonitorUtil.h"
#include "duilib/Core/WindowBase.h"

#ifdef DUILIB_BUILD_FOR_SDL

#include <SDL3/SDL.h>

namespace ui
{

uint32_t MonitorUtil::GetWindowDpi(const WindowBase* pWindowBase)
{
    //读取窗口的DPI值
    uint32_t uDPI = 96;
    SDL_Window* sdlWindow = nullptr;
    if (pWindowBase != nullptr) {
        sdlWindow = (SDL_Window*)pWindowBase->NativeWnd()->GetWindowHandle();
    }
    if (sdlWindow != nullptr) {
        float scale = SDL_GetWindowDisplayScale(sdlWindow);
        if (scale > 0) {
            uDPI = (uint32_t)(scale * 96);
        }
    }
    else {
        uDPI = GetPrimaryMonitorDPI();
    }
    return uDPI;
}

uint32_t MonitorUtil::GetPrimaryMonitorDPI()
{
    if (!SDL_WasInit(SDL_INIT_VIDEO)) {
        SDL_Init(SDL_INIT_VIDEO);
    }
    uint32_t uDPI = 96;
    SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    if (displayID != 0) {
        float scale = SDL_GetDisplayContentScale(displayID);
        if (scale > 0) {
            uDPI = (uint32_t)(scale * 96);
        }
    }
    return uDPI;
}

} // namespace ui

#endif //DUILIB_BUILD_FOR_SDL
