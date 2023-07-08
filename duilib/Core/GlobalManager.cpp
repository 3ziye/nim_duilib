#include "GlobalManager.h"
#include "duilib/Utils/DpiManager.h"
#include "duilib/Utils/StringUtil.h"
#include "duilib/Utils/MultiLangSupport.h"
#include "duilib/Utils/VersionHelpers.h"

#include "duilib/Core/Window.h"
#include "duilib/Core/Control.h"
#include "duilib/Core/Box.h"

//��Ⱦ�����ѡ��(Ŀǰ��֧���ڱ����ڼ�ѡ��)
#include "duilib/Render/RenderConfig.h"
#if (duilib_kRenderType == duilib_kRenderType_Skia)
//Skia����
#include "duilib/RenderSkia/RenderFactory_Skia.h"
	#pragma comment (lib, "opengl32.lib")
#ifdef _DEBUG
    //��ע��
	//1�����ܷ��棺LLVM����İ汾���������Ժ���Visual Studio 2022����İ汾��
	//��1��LLVM�����Debug�汾�������������У��о�������������
	//��2��Visual Studio 2022����Debug�汾���������Կ��٣��ٶȺ�������GIF����ʱ���о��ܲ�����
	//2�������Է��棺
	//��1��Visual Studio 2022����İ汾��Debug��Release��û�����⣻
	//��2��DebugLLVM �汾�����⣬���ڲ�������ָ��ĸ�ֵ�ӿڣ�����������ReleaseLLVM��ʱû�������⡣
    //     ����void SkPaint::setShader(sk_sp<SkShader> shader);
    //     ��Ҫ��Ϊ��void SkPaint::setShader(const sk_sp<SkShader>& shader); ���ܱ������
    //     �������͵ĽӿڱȽ϶࣬�����Ҫʹ�ã�����Ҫ�޸�Դ�룬Ȼ�����±���Skia��
	#pragma comment (lib, "../../../../develop/skia/skia/out/DebugLLVM/skia.lib")
#else
	#pragma comment (lib, "../../../../develop/skia/skia/out/ReleaseLLVM/skia.lib")
#endif

#else if(duilib_kRenderType == duilib_kRenderType_GdiPlus)
//Gdiplus����
#include "duilib/RenderGdiPlus/GdiPlusDefs.h"
#include "duilib/RenderGdiPlus/RenderFactory_Gdiplus.h"

#endif

//ToolTip/����ʱ��ȱ�׼�ؼ�����Ҫ��ʼ��commctrl
#include <commctrl.h>

namespace ui 
{

#if (duilib_kRenderType == duilib_kRenderType_GdiPlus)
//Gdiplus����
static ULONG_PTR g_gdiplusToken = 0;
static Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
#endif


const std::wstring kLanguageFileName = L"gdstrings.ini";

GlobalManager::GlobalManager():
	m_dwUiThreadId(0),
	m_pfnCreateControlCallback(nullptr)
{
}

GlobalManager::~GlobalManager()
{
	Shutdown();
}

GlobalManager& GlobalManager::Instance()
{
	static GlobalManager self;
	return self;
}

void GlobalManager::Startup(const std::wstring& strResourcePath, const CreateControlCallback& callback, bool bAdaptDpi, const std::wstring& theme, const std::wstring& language)
{
	ASSERT(m_renderFactory == nullptr);
	m_dwUiThreadId = GetCurrentThreadId();

#if (duilib_kRenderType == duilib_kRenderType_Skia)
	//Skia��Ⱦ����ʵ��
	m_renderFactory = std::make_unique<RenderFactory_Skia>();	
#else if (duilib_kRenderType == duilib_kRenderType_GdiPlus)
	//Gdiplus��Ⱦ����ʵ��
	Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);
	m_renderFactory = std::make_unique<RenderFactory_GdiPlus>();
#endif

	ASSERT(m_renderFactory != nullptr);

	GlobalManager::SetResourcePath(StringHelper::JoinFilePath(strResourcePath, theme));
	m_pfnCreateControlCallback = callback;

    // ����DPI
	DpiManager::GetInstance()->SetAdaptDPI(bAdaptDpi);
	if (bAdaptDpi) {
		DpiManager::GetInstance()->SetScale(DpiManager::GetMainMonitorDPI());
	}

    // ����ȫ����Դ��Ϣ
	LoadGlobalResource();

	SetLanguagePath(StringHelper::JoinFilePath(strResourcePath, language));

    // ���ض������ļ������ʹ������Դѹ��������ڴ��м��������ļ�
	if (m_zipManager.IsUseZip()) {
		std::vector<unsigned char> file_data;
		std::wstring filePath = StringHelper::JoinFilePath(GetLanguagePath(), kLanguageFileName);
		if (m_zipManager.GetZipData(filePath, file_data)) {
			MultiLangSupport::GetInstance()->LoadStringTable(file_data);
		}
	}
	else {
		std::wstring filePath = StringHelper::JoinFilePath(GetLanguagePath(), kLanguageFileName);
		MultiLangSupport::GetInstance()->LoadStringTable(filePath);
	}
	// Boot Windows Common Controls (for the ToolTip control)
	::InitCommonControls();
}

void GlobalManager::Shutdown()
{
	m_zipManager.CloseResZip();
	m_renderFactory.reset();
	m_fontManager.RemoveAllFonts();
	m_fontManager.RemoveAllFontFiles();
	m_renderFactory = nullptr;
	m_pfnCreateControlCallback = nullptr;
	m_resourcePath.clear();
	m_languagePath.clear();
	m_builderMap.clear();
	m_globalClass.clear();
	m_windowList.clear();
	m_dwUiThreadId = 0;

#if (duilib_kRenderType == duilib_kRenderType_GdiPlus)
	if (g_gdiplusToken != 0) {
		Gdiplus::GdiplusShutdown(g_gdiplusToken);
		g_gdiplusToken = 0;
	}	
#endif
}

const std::wstring& GlobalManager::GetResourcePath()
{
	return m_resourcePath;
}

const std::wstring& GlobalManager::GetLanguagePath()
{
	return m_languagePath;
}

void GlobalManager::SetResourcePath(const std::wstring& strPath)
{
	m_resourcePath = StringHelper::NormalizeDirPath(strPath);
}

void GlobalManager::SetLanguagePath(const std::wstring& strPath)
{
	m_languagePath = StringHelper::NormalizeDirPath(strPath);
}

std::wstring GlobalManager::GetResFullPath(const std::wstring& windowResPath, const std::wstring& resPath)
{
	if (resPath.empty() || !StringHelper::IsRelativePath(resPath)) {
		return resPath;
	}

	std::wstring imageFullPath = StringHelper::JoinFilePath(GlobalManager::GetResourcePath(), windowResPath);
	imageFullPath = StringHelper::JoinFilePath(imageFullPath, resPath);
	imageFullPath = StringHelper::NormalizeFilePath(imageFullPath);
	if (!m_zipManager.IsZipResExist(imageFullPath) && !StringHelper::IsExistsPath(imageFullPath)) {
		imageFullPath = StringHelper::JoinFilePath(GlobalManager::GetResourcePath(), resPath);
		imageFullPath = StringHelper::NormalizeFilePath(imageFullPath);
	}
	return imageFullPath;
}

void GlobalManager::LoadGlobalResource()
{
	WindowBuilder dialog_builder;
	Window paint_manager;
	dialog_builder.Create(L"global.xml", CreateControlCallback(), &paint_manager);
}

void GlobalManager::AddWindow(Window* pWindow)
{
	AssertUIThread();
	ASSERT(pWindow != nullptr);
	if (pWindow != nullptr) {
		m_windowList.push_back(pWindow);
	}	
}

void GlobalManager::RemoveWindow(Window* pWindow)
{
	AssertUIThread();
	auto it = std::find(m_windowList.begin(), m_windowList.end(), pWindow);
	if (it != m_windowList.end()) {
		m_windowList.erase(it);
	}
}

void GlobalManager::RemoveAllImages()
{
	AssertUIThread();
	for (Window* pWindow : m_windowList) {
		Box* pBox = nullptr;
		if (pWindow != nullptr) {
			pBox = pWindow->GetRoot();
		}
		if (pBox != nullptr) {
			pBox->ClearImageCache();
		}
	}
	m_imageManager.RemoveAllImages();
}

void GlobalManager::ReloadSkin(const std::wstring& resourcePath)
{
	AssertUIThread();
	if (GetResourcePath() == StringHelper::NormalizeDirPath(resourcePath)) {
		return;
	}

	m_fontManager.RemoveAllFonts();
	m_fontManager.RemoveAllFontFiles();
	m_colorManager.RemoveAllColors();
	RemoveAllImages();
	RemoveAllClasss();

	SetResourcePath(resourcePath);
	LoadGlobalResource();
	for (Window* pWindow : m_windowList) {
		Box* pBox = nullptr;
		if (pWindow != nullptr) {
			pBox = pWindow->GetRoot();
		}
		if (pBox != nullptr) {
			pBox->Invalidate();
		}
	}
}

void GlobalManager::ReloadLanguage(const std::wstring& languagePath, bool invalidateAll) 
{
	AssertUIThread();
	if (GetLanguagePath() == StringHelper::NormalizeDirPath(languagePath)) {
		return;
	}
	SetLanguagePath(languagePath);
	std::wstring filePath = StringHelper::JoinFilePath(languagePath, kLanguageFileName);
	MultiLangSupport::GetInstance()->LoadStringTable(filePath);
	if (invalidateAll) {
		for (Window* pWindow : m_windowList) {
			Box* pBox = nullptr;
			if (pWindow != nullptr) {
				pBox = pWindow->GetRoot();
			}
			if (pBox != nullptr) {
				pBox->Invalidate();
			}
		}
	}
}

IRenderFactory* GlobalManager::GetRenderFactory()
{
	return m_renderFactory.get();
}

void GlobalManager::AddClass(const std::wstring& strClassName, const std::wstring& strControlAttrList)
{
	AssertUIThread();
	ASSERT(!strClassName.empty() && !strControlAttrList.empty());
	if (!strClassName.empty() && !strControlAttrList.empty()) {
		m_globalClass[strClassName] = strControlAttrList;
	}	
}

std::wstring GlobalManager::GetClassAttributes(const std::wstring& strClassName)
{
	AssertUIThread();
	auto it = m_globalClass.find(strClassName);
	if (it != m_globalClass.end()) {
		return it->second;
	}
	return std::wstring();
}

void GlobalManager::RemoveAllClasss()
{
	AssertUIThread();
	m_globalClass.clear();
}

ColorManager& GlobalManager::Color()
{
	return m_colorManager;
}

FontManager& GlobalManager::Font()
{
	return m_fontManager;
}

ImageManager& GlobalManager::Image()
{
	return m_imageManager;
}

ZipManager& GlobalManager::Zip()
{
	return m_zipManager;
}

Box* GlobalManager::CreateBox(const std::wstring& strXmlPath, CreateControlCallback callback)
{
	WindowBuilder builder;
	Box* box = builder.Create(strXmlPath, callback);
	ASSERT(box != nullptr);
	return box;
}

Box* GlobalManager::CreateBoxWithCache(const std::wstring& strXmlPath, CreateControlCallback callback)
{
	Box* box = nullptr;
	auto it = m_builderMap.find(strXmlPath);
	if (it == m_builderMap.end()) {
		WindowBuilder* builder = new WindowBuilder();
		box = builder->Create(strXmlPath, callback);
		if (box != nullptr) {
			m_builderMap[strXmlPath].reset(builder);
		}
		else {
			delete builder;
			builder = nullptr;
		}
	}
	else {
		box = it->second->Create(callback);
	}
	ASSERT(box != nullptr);
	return box;
}

void GlobalManager::FillBox(Box* pUserDefinedBox, const std::wstring& strXmlPath, CreateControlCallback callback)
{
	ASSERT(pUserDefinedBox != nullptr);
	if (pUserDefinedBox != nullptr) {
		WindowBuilder winBuilder;
		Box* box = winBuilder.Create(strXmlPath, callback, pUserDefinedBox->GetWindow(), nullptr, pUserDefinedBox);
		ASSERT_UNUSED_VARIABLE(box != nullptr);
	}	
}

void GlobalManager::FillBoxWithCache(Box* pUserDefinedBox, const std::wstring& strXmlPath, CreateControlCallback callback)
{
	ASSERT(pUserDefinedBox != nullptr);
	if (pUserDefinedBox == nullptr) {
		return;
	}
	Box* box = nullptr;
	auto it = m_builderMap.find(strXmlPath);
	if (it == m_builderMap.end()) {
		WindowBuilder* winBuilder = new WindowBuilder();
		box = winBuilder->Create(strXmlPath, callback, pUserDefinedBox->GetWindow(), nullptr, pUserDefinedBox);
		if (box != nullptr) {
			m_builderMap[strXmlPath].reset(winBuilder);
		}
		else {
			delete winBuilder;
			winBuilder = nullptr;
		}
	}
	else {
		box = it->second->Create(callback, pUserDefinedBox->GetWindow(), nullptr, pUserDefinedBox);
	}
	ASSERT_UNUSED_VARIABLE(box != nullptr);
}

Control* GlobalManager::CreateControl(const std::wstring& strControlName)
{
	if (m_pfnCreateControlCallback) {
		return m_pfnCreateControlCallback(strControlName);
	}
	return nullptr;
}

void GlobalManager::AssertUIThread()
{
	ASSERT(m_dwUiThreadId == ::GetCurrentThreadId());
}

} // namespace ui
