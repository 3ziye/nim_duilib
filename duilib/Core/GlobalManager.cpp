#include "GlobalManager.h"
#include "duilib/Utils/StringUtil.h"

#include "duilib/Core/Window.h"
#include "duilib/Core/Control.h"
#include "duilib/Core/Box.h"

//��Ⱦ�����ѡ��(Ŀǰ��֧���ڱ����ڼ�ѡ��)
#include "duilib/Render/RenderConfig.h"

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

#if (duilib_kRenderType == duilib_kRenderType_Skia)
//Skia����
#include "duilib/RenderSkia/RenderFactory_Skia.h"
	#pragma comment (lib, "opengl32.lib")

#ifdef DUILIB_RENDER_SKIA_BY_LLVM
    //ʹ��LLVM����Skia
	#ifdef _DEBUG
		//Debug�汾
		#ifdef  _WIN64
			#pragma comment (lib, "../../../skia/out/LLVM.x64.Debug/skia.lib")
		#else
			#pragma comment (lib, "../../../skia/out/LLVM.x86.Debug/skia.lib")
		#endif //_WIN64	
	#else
		//Release�汾
		#ifdef  _WIN64
			#pragma comment (lib, "../../../skia/out/LLVM.x64.Release/skia.lib")
		#else
			#pragma comment (lib, "../../../skia/out/LLVM.x86.Release/skia.lib")
		#endif //_WIN64	
	#endif //_DEBUG
#else
    //ʹ��Visual Studio 2022����Skia
    #ifdef _DEBUG
		//Debug�汾
		#ifdef  _WIN64
			#pragma comment (lib, "../../../skia/out/vs2022.x64.Debug/skia.lib")
		#else
			#pragma comment (lib, "../../../skia/out/vs2022.x86.Debug/skia.lib")
		#endif //_WIN64	
	#else
		//Release�汾
		#ifdef  _WIN64
			#pragma comment (lib, "../../../skia/out/vs2022.x64.Release/skia.lib")
		#else
			#pragma comment (lib, "../../../skia/out/vs2022.x86.Release/skia.lib")
		#endif //_WIN64	
	#endif //_DEBUG
#endif //DUILIB_RENDER_SKIA_BY_LLVM

#else if(duilib_kRenderType == duilib_kRenderType_GdiPlus)
//Gdiplus����
#include "duilib/RenderGdiPlus/GdiPlusDefs.h"
#include "duilib/RenderGdiPlus/RenderFactory_Gdiplus.h"

#endif

//ToolTip/����ʱ��ȱ�׼�ؼ�����Ҫ��ʼ��commctrl
#include <commctrl.h>

#include <filesystem>

namespace ui 
{

#if (duilib_kRenderType == duilib_kRenderType_GdiPlus)
//Gdiplus����
static ULONG_PTR g_gdiplusToken = 0;
static Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
#endif

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

bool GlobalManager::Startup(const ResourceParam& resParam,
	                        DpiInitParam dpiInitParam,
							const CreateControlCallback& callback)
{
	ASSERT(m_renderFactory == nullptr);
	if (m_renderFactory != nullptr) {
		return false;
	}
	//��¼��ǰ�߳�ID
	m_dwUiThreadId = GetCurrentThreadId();

	//����ص�����
	m_pfnCreateControlCallback = callback;

	//��ʼ��DPI��֪ģʽ��//��ʼ��DPIֵ
	DpiManager& dpiManager = Dpi();
	dpiManager.InitDpiAwareness(dpiInitParam);

#if (duilib_kRenderType == duilib_kRenderType_Skia)
	//Skia��Ⱦ����ʵ��
	m_renderFactory = std::make_unique<RenderFactory_Skia>();	
#else if (duilib_kRenderType == duilib_kRenderType_GdiPlus)
	//Gdiplus��Ⱦ����ʵ��
	Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);
	m_renderFactory = std::make_unique<RenderFactory_GdiPlus>();
#endif

#ifdef UILIB_IMPL_WINSDK
	//Init Windows Common Controls (for the ToolTip control)
	::InitCommonControls();
#endif

	ASSERT(m_renderFactory != nullptr);
	if (m_renderFactory == nullptr) {
		return false;
	}

	//������Դ
	if (!ReloadResource(resParam, false)) {
		m_pfnCreateControlCallback = nullptr;
		m_renderFactory.reset();
		return false;
	}
	return true;
}

void GlobalManager::Shutdown()
{
	m_timerManager.Clear();
	m_colorManager.Clear();	
	m_fontManager.RemoveAllFonts();
	m_fontManager.RemoveAllFontFiles();
	m_imageManager.RemoveAllImages();
	m_zipManager.CloseResZip();	
	m_langManager.ClearStringTable();
	
	m_renderFactory.reset();
	m_renderFactory = nullptr;
	m_pfnCreateControlCallback = nullptr;
	m_globalClass.clear();
	m_windowList.clear();
	m_dwUiThreadId = 0;
	m_resourcePath.clear();
	m_languagePath.clear();
	m_builderMap.clear();	

#if (duilib_kRenderType == duilib_kRenderType_GdiPlus)
	if (g_gdiplusToken != 0) {
		Gdiplus::GdiplusShutdown(g_gdiplusToken);
		g_gdiplusToken = 0;
	}	
#endif
}

const std::wstring& GlobalManager::GetResourcePath() const
{
	return m_resourcePath;
}

void GlobalManager::SetResourcePath(const std::wstring& strPath)
{
	m_resourcePath = StringHelper::NormalizeDirPath(strPath);
}

void GlobalManager::SetLanguagePath(const std::wstring& strPath)
{
	m_languagePath = StringHelper::NormalizeDirPath(strPath);
}

const std::wstring& GlobalManager::GetLanguagePath() const
{
	return m_languagePath;
}

const std::wstring& GlobalManager::GetLanguageFileName() const
{
	return m_languageFileName;
}

bool GlobalManager::ReloadResource(const ResourceParam& resParam, bool bInvalidate)
{
	AssertUIThread();
	//У���������
	std::wstring strResourcePath = resParam.resourcePath;
	if (resParam.GetResType() == ResourceType::kLocalFiles) {
		//�����ļ�����ʽ��������Դ���ѱ����ļ�����ʽ����
		//const LocalFilesResParam& param = static_cast<const LocalFilesResParam&>(resParam);
		ASSERT(!strResourcePath.empty());
		if (strResourcePath.empty()) {
			return false;
		}
	}
	else if (resParam.GetResType() == ResourceType::kZipFile) {
		//��Դ�ļ����Ϊzipѹ������Ȼ���Ա����ļ�����ʽ����
		const ZipFileResParam& param = static_cast<const ZipFileResParam&>(resParam);
		bool bZipOpenOk = Zip().OpenZipFile(param.zipFilePath.c_str(), param.zipPassword);
		if (!bZipOpenOk) {
			ASSERT(!"OpenZipFile failed!");
			return false;
		}
	}
	else if (resParam.GetResType() == ResourceType::kResZipFile) {
		//��Դ�ļ����Ϊzipѹ������Ȼ�����exe/dll����Դ�ļ���
		const ResZipFileResParam& param = static_cast<const ResZipFileResParam&>(resParam);
		bool bZipOpenOk = Zip().OpenResZip(param.hResModule, param.resourceName, param.resourceType, param.zipPassword);
		if (!bZipOpenOk) {
			ASSERT(!"OpenResZip failed!");
			return false;
		}
	}
	else {
		ASSERT(false);
		return false;
	}

	//���ԭ����Դ���ݣ����塢��ɫ��Class���塢ͼƬ��Դ�ȣ�
	m_fontManager.RemoveAllFonts();
	m_fontManager.RemoveAllFontFiles();
	m_colorManager.RemoveAllColors();
	RemoveAllImages();
	RemoveAllClasss();

	//������Դ·��
	SetResourcePath(StringHelper::JoinFilePath(strResourcePath, resParam.themePath));

	//����ȫ����Դ��Ϣ(Ĭ����"global.xml"�ļ�)
	ASSERT(!resParam.globalXmlFileName.empty());
	if (!resParam.globalXmlFileName.empty()) {
		WindowBuilder dialog_builder;
		Window paint_manager;
		dialog_builder.Create(resParam.globalXmlFileName, CreateControlCallback(), &paint_manager);
	}

	//���ض������ļ�(��ѡ)
	if (!resParam.languagePath.empty() && !resParam.languageFileName.empty()) {
		std::wstring languagePath = StringHelper::JoinFilePath(strResourcePath, resParam.languagePath);
		ReloadLanguage(languagePath, resParam.languageFileName, false);
	}
	else if (!resParam.languagePath.empty()) {
		SetLanguagePath(StringHelper::JoinFilePath(strResourcePath, resParam.languagePath));
	}

	//���´����е������ӿؼ�״̬
	if (bInvalidate) {
		std::vector<WindowWeakFlag> windowList = m_windowList;
		for (const WindowWeakFlag& windowFlag : windowList) {
			Box* pBox = nullptr;
			if ((windowFlag.m_pWindow != nullptr) && !windowFlag.m_weakFlag.expired()) {
				pBox = windowFlag.m_pWindow->GetRoot();
			}
			if ((pBox != nullptr) && !windowFlag.m_weakFlag.expired()) {
				pBox->Invalidate();
			}
		}
	}
	return true;
}

bool GlobalManager::ReloadLanguage(const std::wstring& languagePath,
	                               const std::wstring& languageFileName,
	                               bool bInvalidate)
{
	AssertUIThread();
	ASSERT(!languageFileName.empty());
	if (languageFileName.empty()) {
		return false;
	}

	std::wstring newLanguagePath = GetLanguagePath();
	if (!languagePath.empty()) {
		newLanguagePath = StringHelper::NormalizeDirPath(languagePath);
	}

	//���ض������ļ������ʹ������Դѹ��������ڴ��м��������ļ�
	bool bReadOk = false;
	if ( (newLanguagePath.empty() || !StringHelper::IsAbsolutePath(newLanguagePath)) && 
		 m_zipManager.IsUseZip() ) {
		std::vector<unsigned char> fileData;
		std::wstring filePath = StringHelper::JoinFilePath(newLanguagePath, languageFileName);
		if (m_zipManager.GetZipData(filePath, fileData)) {
			bReadOk = m_langManager.LoadStringTable(fileData);
		}
		else {
			ASSERT(!"GetZipData failed!");
		}
	}
	else {
		std::wstring filePath = StringHelper::JoinFilePath(newLanguagePath, languageFileName);
		bReadOk = m_langManager.LoadStringTable(filePath);
	}

	if (bReadOk) {
		//���������ļ�·��
		if (!newLanguagePath.empty() && (newLanguagePath != GetLanguagePath())) {
			SetLanguagePath(newLanguagePath);
		}
		//���������ļ���
		m_languageFileName = languageFileName;
	}

	ASSERT(bReadOk && "ReloadLanguage");
	if (bReadOk && bInvalidate) {
		//ˢ�½�����ʾ
		std::vector<WindowWeakFlag> windowList = m_windowList;
		for (const WindowWeakFlag& windowFlag : windowList) {
			Box* pBox = nullptr;
			if ((windowFlag.m_pWindow != nullptr) && !windowFlag.m_weakFlag.expired()) {
				pBox = windowFlag.m_pWindow->GetRoot();
				if (windowFlag.m_pWindow->GetText().empty() && 
					!windowFlag.m_pWindow->GetTextId().empty()) {
					//���´��ڱ������ı�
					windowFlag.m_pWindow->SetTextId(windowFlag.m_pWindow->GetTextId());
				}
			}
			if ((pBox != nullptr) && !windowFlag.m_weakFlag.expired()) {
				pBox->Invalidate();
			}
		}
	}
	return bReadOk;
}

bool GlobalManager::GetLanguageList(std::vector<std::pair<std::wstring, std::wstring>>& languageList,
									const std::wstring& languageNameID) const
{
	std::wstring languagePath = GetLanguagePath();
	ASSERT(!languagePath.empty());
	if (languagePath.empty()) {
		return false;
	}

	languageList.clear();
	const std::filesystem::path path{ languagePath };
	if (path.is_absolute()) {
		//����·���������ļ��ڱ��ش�����
		for (auto const& dir_entry : std::filesystem::directory_iterator{ path }) {
			if (dir_entry.is_regular_file()) {
				languageList.push_back({ dir_entry.path().filename().c_str(), L"" });
			}
		}
		if (!languageNameID.empty()) {
			for (auto& lang : languageList) {
				const std::wstring& fileName = lang.first;
				std::wstring& displayName = lang.second;

				std::wstring filePath = StringHelper::JoinFilePath(languagePath, fileName);
				ui::LangManager langManager;
				if (langManager.LoadStringTable(filePath)) {
					displayName = langManager.GetStringViaID(languageNameID);
				}
			}
		}
	}
	else if(m_zipManager.IsUseZip()){
		//���·���������ļ�Ӧ�ö���ѹ������
		std::vector<std::wstring> fileList;
		m_zipManager.GetZipFileList(languagePath, fileList);
		for (auto const& file : fileList) {
			languageList.push_back({ file, L"" });
		}

		if (!languageNameID.empty()) {
			for (auto& lang : languageList) {
				const std::wstring& fileName = lang.first;
				std::wstring& displayName = lang.second;

				std::wstring filePath = StringHelper::JoinFilePath(languagePath, fileName);
				std::vector<unsigned char> fileData;
				if (m_zipManager.GetZipData(filePath, fileData)) {
					ui::LangManager langManager;
					if (langManager.LoadStringTable(fileData)) {
						displayName = langManager.GetStringViaID(languageNameID);
					}
				}
			}
		}
	}
	else {
		ASSERT(false);
		return false;
	}
	return true;
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

void GlobalManager::AddWindow(Window* pWindow)
{
	AssertUIThread();
	ASSERT(pWindow != nullptr);
	if (pWindow != nullptr) {
		WindowWeakFlag flag;
		flag.m_pWindow = pWindow;
		flag.m_weakFlag = pWindow->GetWeakFlag();
		m_windowList.push_back(flag);
	}	
}

void GlobalManager::RemoveWindow(Window* pWindow)
{
	AssertUIThread();
	ASSERT(pWindow != nullptr);
	if (pWindow != nullptr) {
		for (auto iter = m_windowList.begin(); iter != m_windowList.end(); ++iter) {
			if (iter->m_pWindow == pWindow) {
				m_windowList.erase(iter);
				break;
			}
		}
	}
}

void GlobalManager::RemoveAllImages()
{
	AssertUIThread();
	std::vector<WindowWeakFlag> windowList = m_windowList;
	for (const WindowWeakFlag& windowFlag : windowList) {
		Box* pBox = nullptr;
		if ((windowFlag.m_pWindow != nullptr) && !windowFlag.m_weakFlag.expired()) {
			pBox = windowFlag.m_pWindow->GetRoot();
		}
		if ((pBox != nullptr) && !windowFlag.m_weakFlag.expired()) {
			pBox->ClearImageCache();
		}
	}
	m_imageManager.RemoveAllImages();
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

std::wstring GlobalManager::GetClassAttributes(const std::wstring& strClassName) const
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

#ifdef UILIB_IMPL_WINSDK
IconManager& GlobalManager::Icon()
{
	return m_iconManager;
}
#endif

ZipManager& GlobalManager::Zip()
{
	return m_zipManager;
}

DpiManager& GlobalManager::Dpi()
{
	return m_dpiManager;
}

TimerManager& GlobalManager::Timer()
{
	return m_timerManager;
}

LangManager& GlobalManager::Lang()
{
	return m_langManager;
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
	ASSERT(pUserDefinedBox == box);
	ASSERT_UNUSED_VARIABLE(box != nullptr);
}

Control* GlobalManager::CreateControl(const std::wstring& strControlName)
{
	if (m_pfnCreateControlCallback) {
		return m_pfnCreateControlCallback(strControlName);
	}
	return nullptr;
}

void GlobalManager::AssertUIThread() const
{
	ASSERT(m_dwUiThreadId == ::GetCurrentThreadId());
}

} // namespace ui
