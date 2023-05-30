// Copyright (c) 2010-2011, duilib develop team(www.duilib.com).
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or 
// without modification, are permitted provided that the 
// following conditions are met.
//
// Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above 
// copyright notice, this list of conditions and the following
// disclaimer in the documentation and/or other materials 
// provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#if defined _M_IX86
    #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
    #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
    #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
    #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include "duilib_config.h"
#include <commctrl.h>
#include <stddef.h>
#include <richedit.h>
#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>
#include <malloc.h>
#include <algorithm>
#include <string>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <unordered_set>
#include <queue>
#include <functional>

#if defined(ENABLE_UIAUTOMATION)
    #include <UIAutomation.h>
    #include <UIAutomationCoreApi.h>
    
    #pragma comment(lib, "uiautomationcore.lib")

    #include "Automation/UIAControlProvider.h"
    #include "Automation/UIAWindowProvider.h"
    #include "Automation/UIAScrollBarProvider.h"
    #include "Automation/UIABoxProvider.h"
    #include "Automation/UIAScrollableBoxProvider.h"
    #include "Automation/UIALabelProvider.h"
    #include "Automation/UIAButtonProvider.h"
    #include "Automation/UIACheckBoxProvider.h"
    #include "Automation/UIAProgressProvider.h"
    #include "Automation/UIARichEditProvider.h"
    #include "Automation/UIAComboBoxProvider.h"
    #include "Automation/UIAListBoxProvider.h"
    #include "Automation/UIAListBoxItemProvider.h"
    #include "Automation/UIATreeNodeProvider.h"
#endif

#include "duilib_defs.h"
#include "Utils/Macros.h"
#include "Utils/VersionHelpers.h"
#include "Utils/ApiWrapper.h"
#include "Utils/DpiManager.h"
#include "Utils/Delegate.h"
#include "Utils/StringUtil.h"
#include "Utils/MultiLangSupport.h"
#include "Utils/FontManager.h"
#include "Utils/TimerManager.h"
#include "Utils/OnScreenKeyboardManager.h"
#include "Utils/Shadow.h"

#include "Image/Image.h"
#include "Animation/AnimationPlayer.h"
#include "Animation/AnimationManager.h"

#include "Render/IRender.h"
#include "Render/Clip.h"
#include "Render/Render_GdiPlus.h"
#include "Render/Bitmap_GDI.h"
#include "Render/BitmapHelper.h"
#include "Render/Pen.h"
#include "Render/Brush.h"
#include "Render/Path.h"
#include "Render/Factory.h"
#include "Render/UiColor.h"

#include "Core/Define.h"
#include "Core/WindowBuilder.h"
#include "Core/GlobalManager.h"
#include "Core/Window.h"
#include "Core/Placeholder.h"
#include "Core/Control.h"
#include "Core/BoxShadow.h"
#include "Core/Box.h"
#include "Utils/WinImplBase.h"

#include "Box/VBox.h"
#include "Box/HBox.h"
#include "Box/TileBox.h"
#include "Box/TabBox.h"
#include "Box/ChildBox.h"

#include "Control/ListBox.h"
#include "Control/Combo.h"
#include "Control/ScrollBar.h"
#include "Control/TreeView.h"

#include "Control/Label.h"
#include "Control/Button.h"
#include "Control/CheckBox.h"
#include "Control/Option.h"

#include "Control/Progress.h"
#include "Control/CircleProgress.h"
#include "Control/Slider.h"

#include "Control/RichEdit.h"
#include "Control/VirtualListBox.h"
#include "Control/VirtualTileBox.h"

#include "Control/Menu.h"
#include "duilib/Control/DateTime.h"
