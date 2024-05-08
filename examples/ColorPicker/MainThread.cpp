#include "MainThread.h"

void WorkerThread::Init()
{
	::OleInitialize(NULL);
	nbase::ThreadManager::RegisterThread(m_threadID);
}

void WorkerThread::Cleanup()
{
	nbase::ThreadManager::UnregisterThread();
	::OleUninitialize();
}

void MainThread::Init()
{
	::OleInitialize(NULL);
	nbase::ThreadManager::RegisterThread(kThreadUI);

	//���������߳�
	m_workerThread.reset(new WorkerThread(kThreadWorker, "WorkerThread"));
	m_workerThread->Start();

	//��ʼ��ȫ����Դ, ʹ�ñ����ļ�����Ϊ��Դ
	std::wstring resourcePath = nbase::win32::GetCurrentModuleDirectory();
	resourcePath += L"resources\\";
	ui::GlobalManager::Instance().Startup(ui::LocalFilesResParam(resourcePath));

	ui::ColorPicker* pColorPicker = new ui::ColorPicker;
	pColorPicker->CreateWnd(nullptr, ui::ColorPicker::kClassName.c_str(), UI_WNDSTYLE_FRAME, WS_EX_LAYERED);
	pColorPicker->CenterWindow();
	pColorPicker->ShowWindow();

	//����ѡ��ǰ����ɫ
	pColorPicker->SetSelectedColor(ui::UiColor(ui::UiColors::White));

	//���ڹر��¼�
	pColorPicker->AttachWindowClose([this](const ui::EventArgs& args) {
		//�رմ��ں��˳����߳�
		PostQuitMessage(0L);
		return true;
		});
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
	::OleUninitialize();
}
