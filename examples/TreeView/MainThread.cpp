#include "MainThread.h"
#include "MainForm.h"

void WorkerThread::Init()
{
	::CoInitialize(NULL);
	nbase::ThreadManager::RegisterThread(m_threadID);
}

void WorkerThread::Cleanup()
{
	nbase::ThreadManager::UnregisterThread();
	::CoUninitialize();
}

void MainThread::Init()
{
	::CoInitialize(NULL);
	nbase::ThreadManager::RegisterThread(kThreadUI);

	//���������߳�
	m_workerThread.reset(new WorkerThread(kThreadWorker, "WorkerThread"));
	m_workerThread->Start();

	//��ʼ��ȫ����Դ, ʹ�ñ����ļ�����Ϊ��Դ
	std::wstring resourcePath = nbase::win32::GetCurrentModuleDirectory();
	resourcePath += L"resources\\";
	ui::GlobalManager::Instance().Startup(ui::LocalFilesResParam(resourcePath));

	// ����һ��Ĭ�ϴ�����Ӱ�ľ��д���
	MainForm* window = new MainForm();
	window->CreateWnd(NULL, MainForm::kClassName.c_str(), UI_WNDSTYLE_FRAME, WS_EX_LAYERED);
	window->CenterWindow();
	window->ShowWindow();
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Instance().Shutdown();
	if (m_workerThread != nullptr) {
		m_workerThread->Stop();
		m_workerThread.reset(nullptr);
	}
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
	::CoUninitialize();
}
