#include "FontManager.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Core/DpiManager.h"
#include "duilib/Render/IRender.h"
#include "duilib/Utils/StringUtil.h"
#include <VersionHelpers.h>

namespace ui 
{

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
	RemoveAllFonts();
	RemoveAllFontFiles();
}

bool FontManager::AddFont(const std::wstring& fontId, const UiFont& fontInfo, bool bDefault)
{
	ASSERT(!fontId.empty());
	if (fontId.empty()) {
		return false;
	}

	ASSERT(fontInfo.m_fontSize > 0);
	if (fontInfo.m_fontSize <= 0) {
		return false;
	}

	auto iter = m_fontIdMap.find(fontId);
	ASSERT(iter == m_fontIdMap.end());
	if (iter != m_fontIdMap.end()) {
		//������ͬ������ID�ظ����
		return false;
	}

	//����������Ϣ������������������
	m_fontIdMap[fontId] = fontInfo;
	if (bDefault) {
		//Ĭ������ID
		m_defaultFontId = fontId;
	}
	return true;
}

std::wstring FontManager::GetDpiFontId(const std::wstring& fontId, const DpiManager& dpi) const
{
	std::wstring dpiFontId;
	if (!fontId.empty()) {
		dpiFontId = fontId + L"@" + StringHelper::UInt32ToString(dpi.GetScale());
	}
	return dpiFontId;
}

IFont* FontManager::GetIFont(const std::wstring& fontId, const DpiManager& dpi)
{
	//���ڻ����в���
	IFont* pFont = nullptr;
	if (!fontId.empty()) {		
		std::wstring dpiFontId = GetDpiFontId(fontId, dpi);
		auto iter = m_fontMap.find(dpiFontId);
		if (iter != m_fontMap.end()) {
			pFont = iter->second;
		}
	}
	if (pFont == nullptr) {
		auto iter = m_fontIdMap.find(fontId);
		if ((iter == m_fontIdMap.end()) && !m_defaultFontId.empty()) {
			//û���������ID��ʹ��Ĭ�ϵ�����ID
			std::wstring dpiFontId = GetDpiFontId(m_defaultFontId, dpi);
			auto pos = m_fontMap.find(dpiFontId);
			if (pos != m_fontMap.end()) {
				pFont = pos->second;
			}
		}
	}
	if (pFont != nullptr) {
		//ʹ�û������Ѿ������õ���������
		return pFont;
	}

	//�����в����ڣ���Ҫ��������
	UiFont fontInfo;
	std::wstring realFontId = fontId;
	auto iter = m_fontIdMap.find(realFontId);
	if (iter == m_fontIdMap.end()) {
		realFontId = m_defaultFontId;
		iter = m_fontIdMap.find(realFontId);
		if (iter != m_fontIdMap.end()) {
			fontInfo = iter->second;
		}
		else {
			realFontId.clear();
		}
	}
	else {
		fontInfo = iter->second;
	}
	ASSERT(!realFontId.empty());
	if (realFontId.empty()) {
		//�޴�����ID
		return nullptr;
	}
	std::wstring dpiFontId = GetDpiFontId(realFontId, dpi);	
	if (fontInfo.m_fontName.empty() || 
		StringHelper::IsEqualNoCase(fontInfo.m_fontName, L"system")) {
		//����ʹ��Ӣ�����ƣ����ּ�����
		static bool bOsOverXp = IsWindowsVistaOrGreater();
		fontInfo.m_fontName = bOsOverXp ? L"Microsoft YaHei" : L"SimSun";
	}

	//�������С����DPI����
	ASSERT(fontInfo.m_fontSize > 0);
	dpi.ScaleInt(fontInfo.m_fontSize);
	ASSERT(fontInfo.m_fontSize > 0);

	IRenderFactory* pRenderFactory = GlobalManager::Instance().GetRenderFactory();
	if (pRenderFactory != nullptr) {
		pFont = pRenderFactory->CreateIFont();
	}
	ASSERT(pFont != nullptr);
	if (pFont == nullptr) {
		return nullptr;
	}
	bool isInitOk = pFont->InitFont(fontInfo);
	ASSERT(isInitOk);
	if (!isInitOk) {
		delete pFont;
		pFont = nullptr;
		return nullptr;
	}
	m_fontMap.insert(std::make_pair(dpiFontId, pFont));
	return pFont;
}

void FontManager::RemoveAllFonts()
{
	for (auto fontInfo : m_fontMap) {
		IFont* pFont = fontInfo.second;
		if (pFont != nullptr) {
			delete pFont;
		}
	}
	m_fontMap.clear();
	m_defaultFontId.clear();
	m_fontIdMap.clear();
}

void FontManager::AddFontFile(const std::wstring& strFontFile, const std::wstring& strFontName)
{
	FontFileInfo fontFileInfo;
	std::wstring path = GlobalManager::Instance().GetResFullPath(L"", L"font\\" + strFontFile);
	bool res = false;
	if (GlobalManager::Instance().Zip().IsUseZip()) {
		std::vector<unsigned char> file_data;
		if (GlobalManager::Instance().Zip().GetZipData(path, file_data)) {
			DWORD dwFonts = 0;
			fontFileInfo.m_hFontFile = ::AddFontMemResourceEx(file_data.data(), static_cast<DWORD>(file_data.size()), NULL, &dwFonts);
			res = fontFileInfo.m_hFontFile != nullptr;
		}
	}
	else {
		res = ::AddFontResourceEx(path.c_str(), FR_PRIVATE, 0) != 0;
	}
	ASSERT(res);
	if (res) {
		fontFileInfo.m_fontFilePath = path;
		fontFileInfo.m_fontName = strFontName;
		m_fontFileInfo.push_back(fontFileInfo);
	}
}

void FontManager::RemoveAllFontFiles()
{
	for (const auto& it : m_fontFileInfo) {
		if (it.m_hFontFile != nullptr) {
			::RemoveFontMemResourceEx(it.m_hFontFile);
		}
		else if(!it.m_fontFilePath.empty()){
			::RemoveFontResourceEx(it.m_fontFilePath.c_str(), FR_PRIVATE, 0);
		}
	}
	m_fontFileInfo.clear();
}

//�������Ӣ������ӳ���
static void GetFontNameList(std::vector<std::pair<std::wstring, std::wstring>>& fontNameList)
{
	//ϵͳ����
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"����", L"SimSun"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"����", L"SimHei"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"΢���ź�", L"Microsoft YaHei"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"΢��������", L"Microsoft JhengHei"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"����", L"KaiTi"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"������", L"NSimSun"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"����", L"FangSong"));
	
	//Office����
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"��Բ", L"YouYuan"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"����", L"LiSu"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"����ϸ��", L"STXiHei"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"���Ŀ���", L"STKaiTi"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"��������", L"STSong"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"���ķ���", L"STFangSong"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"��������", L"STZhongSong"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"���Ĳ���", L"STCaiYun"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"��������", L"STHuPo"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"������κ", L"STXinWei"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"��������", L"STLiTi"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"�����п�", L"STXingKai"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"��������", L"FZShuTi"));
	fontNameList.push_back(std::make_pair<std::wstring, std::wstring>(L"����Ҧ��", L"FZYaoTi"));
}

std::wstring FontManager::GetFontEnglishName(const std::wstring& fontName)
{
	std::vector<std::pair<std::wstring, std::wstring>> fontNameList;
	GetFontNameList(fontNameList);
	for (const auto& pair : fontNameList) {
		if (fontName == pair.first) {
			return pair.second;
		}
	}
	return fontName;
}

std::wstring FontManager::GetFontSystemName(const std::wstring& fontName)
{
	std::vector<std::pair<std::wstring, std::wstring>> fontNameList;
	GetFontNameList(fontNameList);
	for (const auto& pair : fontNameList) {
		if (ui::StringHelper::IsEqualNoCase(fontName, pair.second)) {
			return pair.first;
		}
	}
	return fontName;
}

}