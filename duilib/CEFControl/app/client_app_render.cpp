#include "client_app.h" 
#include "duilib/CEFControl/util/util.h"

#include "duilib/CEFControl/app/js_handler.h"
#include "duilib/CEFControl/app/ipc_string_define.h"
#include "duilib/CEFControl/app/cef_js_bridge.h"

#pragma warning (push)
#pragma warning (disable:4100)
#include "include/cef_cookie.h"
#include "include/cef_process_message.h"
#include "include/cef_task.h"
#include "include/cef_v8.h"
#pragma warning (pop)

#include <string>

namespace ui
{
//////////////////////////////////////////////////////////////////////////////////////////

void ClientApp::OnWebKitInitialized() 
{
    /**
     * JavaScript 扩展代码，这里定义一个 NimCefWebFunction 对象提供 call 方法来让 Web 端触发 native 的 CefV8Handler 处理代码
     * param[in] functionName    要调用的 C++ 方法名称
     * param[in] params            调用该方法传递的参数，在前端指定的是一个 Object，但转到 Native 的时候转为了字符串
     * param[in] callback        执行该方法后的回调函数
     * 前端调用示例
     * NimCefWebHelper.call('showMessage', { message: 'Hello C++' }, (arguments) => {
     *    console.log(arguments)
     * })
     */
    std::string extensionCode = R"(
        var NimCefWebInstance = {};
        (() => {
            NimCefWebInstance.call = (functionName, arg1, arg2) => {
                if (typeof arg1 === 'function') {
                    native function call(functionName, arg1);
                    return call(functionName, arg1);
                } else {
                    const jsonString = JSON.stringify(arg1);
                    native function call(functionName, jsonString, arg2);
                    return call(functionName, jsonString, arg2);
                }
            };
            NimCefWebInstance.register = (functionName, callback) => {
                native function register(functionName, callback);
                return register(functionName, callback);
            };
        })();
    )";
    CefRefPtr<CefJSHandler> handler = new CefJSHandler();

    if (!render_js_bridge_.get())
        render_js_bridge_.reset(new CefJSBridge);
    handler->AttachJSBridge(render_js_bridge_);
     CefRegisterExtension("v8/extern", extensionCode, handler);
}

void ClientApp::OnBrowserCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDictionaryValue> extra_info)
{
    if (!render_js_bridge_.get())
        render_js_bridge_.reset(new CefJSBridge);
}

void ClientApp::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) 
{
}

CefRefPtr<CefLoadHandler> ClientApp::GetLoadHandler()
{
    return nullptr;
}

void ClientApp::OnContextCreated(CefRefPtr<CefBrowser> /*browser*/, CefRefPtr<CefFrame> /*frame*/, CefRefPtr<CefV8Context> /*context*/)
{

}

void ClientApp::OnContextReleased(CefRefPtr<CefBrowser> /*browser*/, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> /*context*/)
{
    render_js_bridge_->RemoveCallbackFuncWithFrame(frame);
    render_js_bridge_->UnRegisterJSFuncWithFrame(frame);
}

void ClientApp::OnUncaughtException(
    CefRefPtr<CefBrowser> /*browser*/,
    CefRefPtr<CefFrame> /*frame*/,
    CefRefPtr<CefV8Context> /*context*/,
    CefRefPtr<CefV8Exception> /*exception*/,
    CefRefPtr<CefV8StackTrace> /*stackTrace*/)
{
}

void ClientApp::OnFocusedNodeChanged(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefDOMNode> node) 
{
    bool is_editable = (node.get() && node->IsEditable());
    if (is_editable != last_node_is_editable_)
    {
        // Notify the browser of the change in focused element type.
        last_node_is_editable_ = is_editable;
        CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(kFocusedNodeChangedMessage);

        message->GetArgumentList()->SetBool(0, is_editable);
        if (frame != nullptr) {
            frame->SendProcessMessage(PID_BROWSER, message);
        }
    }
}

bool ClientApp::OnProcessMessageReceived( CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message)
{
    (void)source_process;
    ASSERT(source_process == PID_BROWSER);
    // 收到 browser 的消息回复
    const CefString& message_name = message->GetName();
    if (message_name == kExecuteJsCallbackMessage)
    {
        int callback_id = message->GetArgumentList()->GetInt(0);
        bool has_error = message->GetArgumentList()->GetBool(1);
        CefString json_string = message->GetArgumentList()->GetString(2);

        // 将收到的参数通过管理器传递给调用时传递的回调函数
        render_js_bridge_->ExecuteJSCallbackFunc(callback_id, has_error, json_string);
    }
    else if (message_name == kCallJsFunctionMessage)
    {
        CefString function_name = message->GetArgumentList()->GetString(0);
        CefString json_string = message->GetArgumentList()->GetString(1);
        int cpp_callback_id = message->GetArgumentList()->GetInt(2);
        CefString frame_id_string = message->GetArgumentList()->GetString(3);

        // 通过 C++ 执行一个已经注册过的 JS 方法
        // frame_id 小于 0 则可能是 browser 进程的 browser 是无效的，所以这里为了避免出现错误就获取一个顶层 frame 执行代码
        render_js_bridge_->ExecuteJSFunc(function_name, json_string, frame_id_string.empty() ? browser->GetMainFrame() : browser->GetFrameByIdentifier(frame_id_string), cpp_callback_id);
    }

    return false;
}
}