// basic.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "main.h"
#include "basic_form.h"

enum ThreadId
{
	kThreadUI
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// �������߳�
	MainThread thread;

	// ִ�����߳�ѭ��
	thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

	return 0;
}

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(kThreadUI);

	//��ʼ��ȫ����Դ
	constexpr ui::ResourceType resType = ui::ResourceType::kLocalFiles;
	if (resType == ui::ResourceType::kLocalFiles) {
		//ʹ�ñ����ļ�����Ϊ��Դ
		std::wstring resourcePath = nbase::win32::GetCurrentModuleDirectory();
		resourcePath += L"resources\\";
		ui::GlobalManager::Instance().Startup(ui::LocalFilesResParam(resourcePath));
	}
	else if (resType == ui::ResourceType::kZipFile) {
		//ʹ�ñ���zipѹ������Ϊ��Դ��ѹ����λ��exe��ͬĿ¼��	
		ui::ZipFileResParam resParam;
		resParam.resourcePath = L"resources\\";
		resParam.zipFilePath = nbase::win32::GetCurrentModuleDirectory();
		resParam.zipFilePath += L"resources.zip";
		resParam.zipPassword = "";
		ui::GlobalManager::Instance().Startup(resParam);
	}
	else if (resType == ui::ResourceType::kResZipFile) {
		//ʹ��exe��Դ�ļ��е�zipѹ����
		ui::ResZipFileResParam resParam;
		resParam.resourcePath = L"resources\\";
		resParam.hResModule = nullptr;
		resParam.resourceName = MAKEINTRESOURCE(IDR_THEME);
		resParam.resourceType = L"THEME";
		resParam.zipPassword = "";
		ui::GlobalManager::Instance().Startup(resParam);
	}
	else {
		return;
	}

	// ����һ��Ĭ�ϴ�����Ӱ�ľ��д���
	BasicForm* window = new BasicForm();
	window->CreateWnd(NULL, BasicForm::kClassName.c_str(), UI_WNDSTYLE_FRAME, WS_EX_LAYERED);
	window->SetIcon(IDI_BASIC);
	window->CenterWindow();
	window->ShowWindow();
	//window->Maximized();
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Instance().Shutdown();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}
