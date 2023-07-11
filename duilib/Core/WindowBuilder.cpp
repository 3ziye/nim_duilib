#include "WindowBuilder.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Core/Window.h"
#include "duilib/Core/Box.h"
#include "duilib/Core/ScrollBox.h"
#include "duilib/Core/Control.h"
#include "duilib/Core/ScrollBar.h"
#include "duilib/Control/TreeView.h"
#include "duilib/Control/Combo.h"
#include "duilib/Control/Slider.h"
#include "duilib/Control/Progress.h"
#include "duilib/Control/CircleProgress.h"
#include "duilib/Control/RichEdit.h"
#include "duilib/Control/RichText.h"
#include "duilib/Control/VirtualListBox.h"
#include "duilib/Control/VirtualTileBox.h"
#include "duilib/Control/DateTime.h"

#include "duilib/Box/HBox.h"
#include "duilib/Box/VBox.h"
#include "duilib/Box/ChildBox.h"
#include "duilib/Box/TabBox.h"
#include "duilib/Box/TileBox.h"

#include "duilib/Utils/StringUtil.h"

#include "duilib/third_party/xml/pugixml.hpp"

#include <tchar.h>

namespace ui 
{

WindowBuilder::WindowBuilder()
{
	m_xml = std::make_unique<pugi::xml_document>();
}

WindowBuilder::~WindowBuilder()
{
}

Box* WindowBuilder::Create(const std::wstring& xml, 
	                       CreateControlCallback pCallback,
						   Window* pWindow, 
	                       Box* pParent, 
	                       Box* pUserDefinedBox)
{
	ASSERT(!xml.empty() && L"xml ����Ϊ�գ�");
	if (xml.empty()) {
		return nullptr;
	}
	//�ַ�����<��ͷ��Ϊ��XML�ַ�����������Ϊ��XML�ļ�
	//���ʹ���� zip ѹ����������ڴ��ж�ȡ
	if (xml.front() == L'<') {
		pugi::xml_parse_result result = m_xml->load_buffer(xml.c_str(), 
														   xml.size() * sizeof(std::wstring::value_type), 
														   pugi::parse_default, 
			                                               pugi::xml_encoding::encoding_utf16);
		if (result.status != pugi::status_ok) {
			ASSERT(!L"WindowBuilder::Create load xml from string data failed!");
			return nullptr;
		}
	}
	else if (GlobalManager::Instance().Zip().IsUseZip()) {
		std::wstring sFile = StringHelper::JoinFilePath(GlobalManager::Instance().GetResourcePath(), xml);
		std::vector<unsigned char> file_data;
		if (GlobalManager::Instance().Zip().GetZipData(sFile, file_data)) {
			pugi::xml_parse_result result = m_xml->load_buffer(file_data.data(), file_data.size());
			if (result.status != pugi::status_ok) {
				ASSERT(!L"WindowBuilder::Create load xml from zip data failed!");
				return nullptr;
			}
		}
		else {
			std::wstring xmlFilePath = GlobalManager::Instance().GetResourcePath();
			if (StringHelper::IsRelativePath(xml)) {
				xmlFilePath = StringHelper::JoinFilePath(xmlFilePath, xml);
			}
			else {
				xmlFilePath = xml;
			}
			pugi::xml_parse_result result = m_xml->load_file(xmlFilePath.c_str());
			if (result.status != pugi::status_ok) {
				ASSERT(!L"WindowBuilder::Create load xml file failed!");
				return nullptr;
			}
		}
	}
	else {
		std::wstring xmlFilePath = GlobalManager::Instance().GetResourcePath();
		if (StringHelper::IsRelativePath(xml)) {
			xmlFilePath = StringHelper::JoinFilePath(xmlFilePath, xml);
		}
		else {
			xmlFilePath = xml;
		}
		pugi::xml_parse_result result = m_xml->load_file(xmlFilePath.c_str());
		if (result.status != pugi::status_ok) {
			ASSERT(!L"WindowBuilder::Create load xml file failed!");
			return nullptr;
		}
	}
	return Create(pCallback, pWindow, pParent, pUserDefinedBox);
}

Box* WindowBuilder::Create(CreateControlCallback pCallback, Window* pWindow, Box* pParent, Box* pUserDefinedBox)
{
	m_createControlCallback = pCallback;
	pugi::xml_node root = m_xml->root().first_child();
	ASSERT(!root.empty());
	if (root.empty()) {
		return nullptr;
	}

	if( pWindow != nullptr) {
		std::wstring strClass;
		std::wstring strName;
		std::wstring strValue;
		strClass = root.name();

		if( strClass == _T("Window") ) {
			if( pWindow->GetHWND() ) {
				for (pugi::xml_attribute attr : root.attributes()) {
					strName = attr.name();
					strValue = attr.value();
					if( strName == _T("size") ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(strValue.c_str(), &pstr, 10);	ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);	ASSERT(pstr); 
						pWindow->SetInitSize(cx, cy);
					} 
					else if( strName == _T("heightpercent") ) {
						double lfHeightPercent = _ttof(strValue.c_str());
	
						MONITORINFO oMonitor = {}; 
						oMonitor.cbSize = sizeof(oMonitor);
						::GetMonitorInfo(::MonitorFromWindow(pWindow->GetHWND(), MONITOR_DEFAULTTOPRIMARY), &oMonitor);
						int nWindowHeight = int((oMonitor.rcWork.bottom - oMonitor.rcWork.top) * lfHeightPercent);
						int nMinHeight = pWindow->GetMinInfo().cy;
						int nMaxHeight = pWindow->GetMaxInfo().cy;
						if (nMinHeight != 0 && nWindowHeight < nMinHeight) {
							nWindowHeight = nMinHeight;
						}
						if (nMaxHeight != 0 && nWindowHeight > nMaxHeight) {
							nWindowHeight = nMaxHeight;
						}

						UiSize xy = pWindow->GetInitSize();
						pWindow->SetInitSize(xy.cx, nWindowHeight, false, false);
					}
					else if( strName == _T("sizebox") ) {
						UiRect rcSizeBox;
						LPTSTR pstr = NULL;
						rcSizeBox.left = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);    
						rcSizeBox.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
						rcSizeBox.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
						rcSizeBox.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
						pWindow->SetSizeBox(rcSizeBox);
					}
					else if( strName == _T("caption") ) {
						UiRect rcCaption;
						LPTSTR pstr = NULL;
						rcCaption.left = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);    
						rcCaption.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
						rcCaption.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
						rcCaption.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
						pWindow->SetCaptionRect(rcCaption);
					}
					else if( strName == _T("text") ) {
						pWindow->SetText(strValue);
					}
					else if (strName == _T("textid")) {
						pWindow->SetTextId(strValue);
					}
					else if( strName == _T("roundcorner") ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pWindow->SetRoundCorner(cx, cy);
					} 
					else if( strName == _T("mininfo") ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pWindow->SetMinInfo(cx, cy);
					}
					else if( strName == _T("maxinfo") ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pWindow->SetMaxInfo(cx, cy);
					}					
					else if (strName == _T("alphafixcorner") || strName == _T("custom_shadow")) {
						UiRect rc;
						LPTSTR pstr = NULL;
						rc.left = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);
						rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
						rc.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
						rc.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
						pWindow->SetAlphaFixCorner(rc);
					}
					else if (strName == _T("renderalpha")) {
						pWindow->SetRenderTransparent(strValue == _T("true"));
					}
					else if ((strName == _T("shadow_attached")) || (strName == _T("shadowattached"))) {
						//�����Ƿ�֧�ִ�����Ӱ����Ӱʵ�������֣��㴰�ں���ͨ���ڣ�
						pWindow->SetShadowAttached(strValue == _T("true"));
					}
					else if ((strName == _T("shado_wimage")) || (strName == _T("shadowimage"))) {
						//������ӰͼƬ
						pWindow->SetShadowImage(strValue);
					}
					else if ((strName == _T("shadow_corner")) || (strName == _T("shadowcorner"))) {
						//���ô�����Ӱ�ľŹ�������
						UiRect rc;
						LPTSTR pstr = NULL;
						rc.left = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);
						rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
						rc.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
						rc.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
						pWindow->SetShadowCorner(rc);
					}
					else if (strName == _T("layered_window")) {
						//�����Ƿ����ò㴰�����ԣ��㴰�ڻ�����ͨ���ڣ�
						pWindow->SetLayeredWindow(strValue == _T("true"));
					}
					else if (strName == _T("alpha")) {
						//���ô��ڵ�͸���ȣ�0 - 255��������ʹ�ò㴰��ʱ��Ч
						LPTSTR pstr = NULL;
						int nAlpha = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);
						ASSERT(nAlpha >= 0 && nAlpha <= 255);
						if ((nAlpha >= 0) && (nAlpha <= 255)) {
							pWindow->SetWindowAlpha(nAlpha);
						}
					}
				}
			}
		}

		if( strClass == _T("Global") ) {
			for(pugi::xml_node node : root.children()) {
				strClass = node.name();
				if( strClass == _T("Image") ) {
					ASSERT(FALSE);	//����
				}
				else if (strClass == L"FontResource") {
					std::wstring strFontFile;
					std::wstring strFontName;
					for (pugi::xml_attribute attr : node.attributes()) {
						strName = attr.name();
						strValue = attr.value();
						if (strName == L"file") {
							strFontFile = strValue;
						}
						else if (strName == L"name") {
							strFontName = strValue;
						}
					}
					if (!strFontFile.empty() && !strFontName.empty()) {
						GlobalManager::Instance().Font().AddFontFile(strFontFile, strFontName);
					}
				}
				else if( strClass == L"Font") {					
					std::wstring strFontId;
					std::wstring strFontName;
					int size = 12;
					bool bold = false;
					bool underline = false;
					bool strikeout = false;
					bool italic = false;
					bool isDefault = false;
					for (pugi::xml_attribute attr : node.attributes()) {
						strName = attr.name();
						strValue = attr.value();
						if (strName == L"id")
						{
							strFontId = strValue;
						}
						else if( strName == L"name") {
							strFontName = strValue;
						}
						else if( strName == L"size") {
							size = wcstol(strValue.c_str(), nullptr, 10);
						}
						else if( strName == L"bold") {
							bold = (strValue == L"true");
						}
						else if( strName == L"underline") {
							underline = (strValue == _T("true"));
						}
						else if (strName == L"strikeout") {
							strikeout = (strValue == _T("true"));
						}
						else if( strName == L"italic") {
							italic = (strValue == _T("true"));
						}
						else if( strName == L"default") {
							isDefault = (strValue == L"true");
						}
					}
					if (!strFontName.empty() && !strFontId.empty()) {
						UiFont fontInfo;
						fontInfo.m_fontName = strFontName;
						fontInfo.m_fontSize = GlobalManager::Instance().Dpi().GetScaleInt(size);
						fontInfo.m_bBold = bold;
						fontInfo.m_bItalic = italic;
						fontInfo.m_bUnderline = underline;
						fontInfo.m_bStrikeOut = strikeout;
						GlobalManager::Instance().Font().AddFont(strFontId, fontInfo, isDefault);
					}
				}
				else if( strClass == _T("Class") ) {
					std::wstring strClassName;
					std::wstring strAttribute;
					for (pugi::xml_attribute attr : node.attributes()) {
						strName = attr.name();
						strValue = attr.value();
						if( strName == _T("name") ) {
							strClassName = strValue;
						}
						else if( strName == _T("value") ) {
							strAttribute.append(strValue);
						}
						else if (strName == _T("_value")) {
							strAttribute.append(StringHelper::Printf(L" value=\"%s\"",strValue.c_str()));
						}
						else {
							strAttribute.append(StringHelper::Printf(L" %s=\"%s\"",
								strName.c_str(), strValue.c_str()));
						}
					}
					if( !strClassName.empty() ) {
						StringHelper::TrimLeft(strAttribute);
						GlobalManager::Instance().AddClass(strClassName, strAttribute);
					}
				}
				else if( strClass == _T("TextColor") ) {
					std::wstring colorName = node.attribute(L"name").as_string();
					std::wstring colorValue = node.attribute(L"value").as_string();
					if(!colorName.empty() && !colorValue.empty()) {
						ColorManager& colorManager = GlobalManager::Instance().Color();
						colorManager.AddColor(colorName, colorValue);
						if (colorName == _T("default_font_color")) {
							colorManager.SetDefaultTextColor(colorName);
						}
						else if (colorName == _T("disabled_font_color")) {
							colorManager.SetDefaultDisabledTextColor(colorName);
						}
						else if (colorName == _T("default_link_font_color")) {
							colorManager.SetDefaultLinkFontColor(colorName);
						}
						else if (colorName == _T("hover_link_font_color")) {
							colorManager.SetDefaultLinkHoverFontColor(colorName);
						}
					}
				}
			}
		}
		else if ( strClass == _T("Window") )
		{
			for (pugi::xml_node node : root.children()) {
				strClass = node.name();
				if( strClass == _T("Class") ) {					
					std::wstring strClassName;
					std::wstring strAttribute;
					for (pugi::xml_attribute attr : node.attributes()) {
						strName = attr.name();
						strValue = attr.value();
						if( strName == _T("name") ) {
							strClassName = strValue;
						}
						else if( strName == _T("value") ) {
							strAttribute.append(strValue);
						}
						else if (strName == _T("_value")) {
							strAttribute.append(StringHelper::Printf(L" value=\"%s\"", strValue.c_str()));
						}
						else {
							strAttribute.append(StringHelper::Printf(L" %s=\"%s\"",
								strName.c_str(), strValue.c_str()));
						}
					}
					if( !strClassName.empty() ) {
						ASSERT(GlobalManager::Instance().GetClassAttributes(strClassName).empty() );	//�����е�Class������ȫ�ֵ�����
						StringHelper::TrimLeft(strAttribute);
						pWindow->AddClass(strClassName, strAttribute);
					}
				}
				else if (strClass == _T("TextColor")) {
					std::wstring strColorName;
					std::wstring strColor;
					for (pugi::xml_attribute attr : node.attributes()) {
						strName = attr.name();
						strValue = attr.value();
						if (strName == _T("name")) {
							strColorName = strValue;
						}
						else if (strName == _T("value")) {
							strColor = strValue;
						}
					}
					if (!strColorName.empty()) {
						pWindow->AddTextColor(strColorName, strColor);
					}
				}
			}
		}
	}

	for (pugi::xml_node node : root.children()) {
		std::wstring strClass = node.name();
		if (strClass == _T("Image") || strClass == _T("FontResource") || strClass == _T("Font")
			|| strClass == _T("Class") || strClass == _T("TextColor") ) {

		}
		else {
			if (!pUserDefinedBox) {
				return (Box*)ParseXmlNode(root, pParent, pWindow);
			}
			else {
				ParseXmlNode(node, pUserDefinedBox, pWindow);
				int i = 0;
				for (pugi::xml_attribute attr : node.attributes()) {					
					if (_tcscmp(attr.name(), _T("class")) == 0) {
						//class�����ǵ�һ������
						ASSERT(i == 0);
					}
					++i;
					pUserDefinedBox->SetAttribute(attr.name(), attr.value());
				}
				return pUserDefinedBox;
			}
		}
	}

	return nullptr;
}

Control* WindowBuilder::ParseXmlNode(const pugi::xml_node& xmlNode, Control* pParent, Window* pWindow)
{
	if (xmlNode.empty()) {
		return nullptr;
	}
    Control* pReturn = nullptr;
	for (pugi::xml_node node : xmlNode.children()) {
		std::wstring strClass = node.name();
		if( (strClass == L"Image") || 
			(strClass == L"Font")  ||
			(strClass == L"Class") || 
			(strClass == L"TextColor") ) {
				continue;
		}

        Control* pControl = nullptr;
        if (strClass == L"Include") {
			if (node.attributes().empty()) {
				continue;
			}
			pugi::xml_attribute countAttr = node.attribute(L"count");
			int nCount = countAttr.as_int();

			pugi::xml_attribute sourceAttr = node.attribute(L"source");
			std::wstring sourceValue = sourceAttr.as_string();
			if (sourceValue.empty()) {
				continue;
			}
            for ( int i = 0; i < nCount; i++ ) {
                WindowBuilder builder;
                pControl = builder.Create(sourceValue, m_createControlCallback, pWindow, (Box*)pParent);
            }
            continue;
        }
        else {
			pControl = CreateControlByClass(strClass);
			if (pControl == nullptr) {
				if ((strClass == L"Event") || 
					(strClass == L"BubbledEvent")) {
					bool bBubbled = (strClass == L"BubbledEvent");
					AttachXmlEvent(bBubbled, node, pParent);
					continue;
				}
			}

            // User-supplied control factory
            if( pControl == nullptr) {
				pControl = GlobalManager::Instance().CreateControl(strClass);
            }

            if( pControl == nullptr && m_createControlCallback ) {
                pControl = m_createControlCallback(strClass);
            }
        }

		if(pControl == nullptr) {
			std::wstring nodeName = strClass;
			ASSERT(!"Found unknown node name, can't create control!");
			continue;
		}

		// TreeView��ر�������Ӻ����
		if (strClass == DUI_CTR_TREENODE) {
			TreeNode* pNode = dynamic_cast<TreeNode*>(pControl);
			ASSERT(pNode != nullptr);
			TreeView* pTreeView = dynamic_cast<TreeView*>(pParent);
			ASSERT(pTreeView != nullptr);
			if (pTreeView) {
				pTreeView->GetRootNode()->AddChildNode(pNode);
			}
			else {
				TreeNode* pTreeNode = dynamic_cast<TreeNode*>(pParent);
				ASSERT(pTreeNode != nullptr);
				if (pTreeNode) {
					pTreeNode->AddChildNode(pNode);
				}
			}
		}

		pControl->SetWindow(pWindow);
		
		// Process attributes
		if(!node.attributes().empty()) {
			//��ȡ�ڵ�����ԣ����ÿؼ�������
			int i = 0;
			for (pugi::xml_attribute attr : node.attributes()) {
				ASSERT(i == 0 || _tcscmp(attr.name(), L"class") != 0);	//class�����ǵ�һ������
				++i;
				pControl->SetAttribute(attr.name(), attr.value());
			}
		}

		if (strClass == DUI_CTR_RICHTEXT) {
			ParseRichTextXmlNode(node, pControl);
#ifdef _DEBUG
			//����Ч�����������ɴ���ʽ���ı������ڲ�����֤��������ȷ��
			RichText* pRichText = dynamic_cast<RichText*>(pControl);
			std::wstring richText;
			if (pRichText) {
				richText = pRichText->ToString();
			}
			richText.clear();
#endif
		}
		else {
			// Add children
			if (!node.children().empty()) {
				//�ݹ�ýڵ�������ӽڵ㣬�������
				ParseXmlNode(node, (Box*)pControl, pWindow);
			}
		}

		// Attach to parent
        // ��ΪĳЩ���Ժ͸�������أ�����selected��������Add��������
		if (pParent != nullptr && strClass != DUI_CTR_TREENODE) {
			Box* pContainer = dynamic_cast<Box*>(pParent);
			ASSERT(pContainer != nullptr);
			if (pContainer == nullptr) {
				return nullptr;
			}
			if( !pContainer->AddItem(pControl) ) {
				ASSERT(FALSE);
				delete pControl;
				continue;
			}
		}
        
        // Return first item
		if (pReturn == nullptr) {
			pReturn = pControl;
		}
    }
    return pReturn;
}

bool WindowBuilder::ParseRichTextXmlText(const std::wstring& xmlText, Control* pControl)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_buffer(xmlText.c_str(),
													xmlText.size() * sizeof(std::wstring::value_type),
													pugi::parse_default,
													pugi::xml_encoding::encoding_utf16);
	if (result.status != pugi::status_ok) {
		ASSERT(!L"WindowBuilder::ParseRichTextXmlText load xml text failed!");
		return false;
	}
	pugi::xml_node root = doc.root();
	std::wstring rootName = root.name();
	std::wstring rootValue = root.value();
	if (rootName.empty() && rootValue.empty()) {
		root = doc.root().first_child();
	}
	rootName = root.name();
	ASSERT(rootName == DUI_CTR_RICHTEXT);
	if (rootName != DUI_CTR_RICHTEXT) {
		return false;
	}
	return ParseRichTextXmlNode(root, pControl, nullptr);
}

bool WindowBuilder::ParseRichTextXmlNode(const pugi::xml_node& xmlNode, Control* pControl, RichTextSlice* pTextSlice)
{
	RichText* pRichText = dynamic_cast<RichText*>(pControl);
	ASSERT(pRichText != nullptr);
	if (pRichText == nullptr) {
		return false;
	}

	for (pugi::xml_node node : xmlNode.children()) {
		RichTextSlice textSlice;
		textSlice.m_nodeName = node.name();
		const std::wstring& nodeName = textSlice.m_nodeName;

		bool bParseChildren = true;
		if (nodeName.empty()) {
			std::wstring nodeValue = node.value();
			if (!nodeValue.empty()) {
				textSlice.m_text = nodeValue;
				StringHelper::Trim(textSlice.m_text);
			}
			//�޽ڵ����ƣ�ֻ��ȡ�ı�����, ����Ҫ�ݹ�����ӽڵ�
			bParseChildren = false;
		}		
		else if (nodeName == L"a") {			
			textSlice.m_text = node.first_child().value();
			textSlice.m_linkUrl = node.attribute(L"href").as_string();
			StringHelper::Trim(textSlice.m_text);
			StringHelper::Trim(textSlice.m_linkUrl);
			//�������ӽڵ�, ����Ҫ�ݹ�����ӽڵ�
			bParseChildren = false;
		}
		else if (nodeName == L"b") {
			//������
			textSlice.m_fontInfo.m_bBold = true;
		}
		else if (nodeName == L"i") {
			//б����
			textSlice.m_fontInfo.m_bItalic = true;
		}
		else if ((nodeName == L"del") || (nodeName == L"s") || (nodeName == L"strike")) {
			//ɾ����
			textSlice.m_fontInfo.m_bStrikeOut = true;
		}
		else if ( (nodeName == L"ins") || (nodeName == L"u") ){
			//�»���
			textSlice.m_fontInfo.m_bUnderline = true;
		}
		else if (nodeName == L"bgcolor") {
			//������ɫ
			textSlice.m_bgColor = node.attribute(L"color").as_string();
			StringHelper::Trim(textSlice.m_bgColor);
		}
		else if (nodeName == L"font") {
			//�������ã��ı���ɫ
			textSlice.m_textColor = node.attribute(L"color").as_string();
			textSlice.m_fontInfo.m_fontName = node.attribute(L"face").as_string();
			textSlice.m_fontInfo.m_fontSize = node.attribute(L"size").as_int();
		}
		else {
			//��������ʶ�Ľڵ㣬����
			ASSERT(!"Found unknown xml node name!");
			continue;
		}
		if (bParseChildren) {
			//�ݹ��ӽڵ�
			ParseRichTextXmlNode(node, pRichText, &textSlice);
		}
		//���ӽڵ���ӵ�Control���߸��ڵ�(ע�⣺std::move�Ժ�textSlice����ʧЧ)
		if (pTextSlice != nullptr) {
			pTextSlice->m_childs.emplace_back(std::move(textSlice));
		}
		else {
			pRichText->AppendTextSlice(std::move(textSlice));
		}
	}
	return true;
}

Control* WindowBuilder::CreateControlByClass(const std::wstring& strControlClass)
{
	Control* pControl = nullptr;
	size_t cchLen = strControlClass.size();
	switch( cchLen ) {
	case 3:
		if( strControlClass == DUI_CTR_BOX )					pControl = new Box;
		break;
	case 4:
		if( strControlClass == DUI_CTR_HBOX )					pControl = new HBox;
		else if( strControlClass == DUI_CTR_VBOX )				pControl = new VBox;
		break;
	case 5:
		if( strControlClass == DUI_CTR_COMBO )                  pControl = new Combo;
		else if( strControlClass == DUI_CTR_LABEL )             pControl = new Label;
		break;
	case 6:
		if( strControlClass == DUI_CTR_BUTTON )                 pControl = new Button;
		else if( strControlClass == DUI_CTR_OPTION )            pControl = new Option;
		else if( strControlClass == DUI_CTR_SLIDER )            pControl = new Slider;
		else if( strControlClass == DUI_CTR_TABBOX )			pControl = new TabBox;
		break;
	case 7:
		if( strControlClass == DUI_CTR_CONTROL )                pControl = new Control;
		else if( strControlClass == DUI_CTR_TILEBOX )		  	pControl = new TileBox;
		else if (strControlClass == DUI_CTR_LISTBOX)			pControl = new ListBox(new Layout);
		break;
	case 8:
		if( strControlClass == DUI_CTR_PROGRESS )               pControl = new Progress;
		else if( strControlClass == DUI_CTR_RICHEDIT )          pControl = new RichEdit;
		else if (strControlClass == DUI_CTR_RICHTEXT)           pControl = new RichText;
		else if( strControlClass == DUI_CTR_CHECKBOX )			pControl = new CheckBox;
		else if( strControlClass == DUI_CTR_TREEVIEW )			pControl = new TreeView;
		else if( strControlClass == DUI_CTR_TREENODE )			pControl = new TreeNode;
		else if( strControlClass == DUI_CTR_HLISTBOX )			pControl = new ListBox(new HLayout);
		else if( strControlClass == DUI_CTR_VLISTBOX )          pControl = new ListBox(new VLayout);
		else if( strControlClass == DUI_CTR_CHILDBOX )			pControl = new ChildBox;
		else if( strControlClass == DUI_CTR_LABELBOX )          pControl = new LabelBox;
		else if( strControlClass == DUI_CTR_DATETIME)			pControl = new DateTime;
		break;
	case 9:
		if( strControlClass == DUI_CTR_SCROLLBAR )				pControl = new ScrollBar; 
		else if( strControlClass == DUI_CTR_BUTTONBOX )         pControl = new ButtonBox;
		else if( strControlClass == DUI_CTR_OPTIONBOX )         pControl = new OptionBox;
		else if (strControlClass == DUI_CTR_SCROLLBOX)          pControl = new ScrollBox(new Layout);
		break;
	case 11:
		if( strControlClass == DUI_CTR_TILELISTBOX )			pControl = new ListBox(new TileLayout);
		else if( strControlClass == DUI_CTR_CHECKBOXBOX )		pControl = new CheckBoxBox;
		break;
	case 14:
    if (strControlClass == DUI_CTR_VIRTUALLISTBOX)			pControl = new VirtualListBox;
    else if (strControlClass == DUI_CTR_CIRCLEPROGRESS)     pControl = new CircleProgress;
    else if (strControlClass == DUI_CTR_VIRTUALTILEBOX)     pControl = new VirtualTileBox;
		break;
	case 15:
		break;
	case 16:
		break;
	case 20:
		if( strControlClass == DUI_CTR_LISTBOX_ELEMENT)   pControl = new ListBoxElement;
		break;
	default:
		break;
	}

	return pControl;
}

void WindowBuilder::AttachXmlEvent(bool bBubbled, const pugi::xml_node& node, Control* pParent)
{
	ASSERT(pParent != nullptr);
	if (pParent == nullptr) {
		return;
	}
	std::wstring strType;
	std::wstring strReceiver;
	std::wstring strApplyAttribute;
	std::wstring strName;
	std::wstring strValue;
	int i = 0;
	for (pugi::xml_attribute attr : node.attributes()) {
		strName = attr.name();
		strValue = attr.value();
		ASSERT(i != 0 || strName == _T("type"));
		ASSERT(i != 1 || strName == _T("receiver"));
		ASSERT(i != 2 || strName == _T("applyattribute"));
		++i;
		if( strName == _T("type") ) {
			strType = strValue;
		}
		else if( strName == _T("receiver") ) {
			strReceiver = strValue;
		}
		else if( strName == _T("applyattribute") ) {
			strApplyAttribute = strValue;
		}
	}

	auto typeList = StringHelper::Split(strType, L" ");
	auto receiverList = StringHelper::Split(strReceiver, L" ");
	for (auto itType = typeList.begin(); itType != typeList.end(); itType++) {
		for (auto itReceiver = receiverList.begin(); itReceiver != receiverList.end(); itReceiver++) {
			EventType eventType = StringToEnum(*itType);
			auto callback = nbase::Bind(&Control::OnApplyAttributeList, pParent, *itReceiver, strApplyAttribute, std::placeholders::_1);
			if (!bBubbled) {
				pParent->AttachXmlEvent(eventType, callback);
			}
			else {
				pParent->AttachXmlBubbledEvent(eventType, callback);
			}
		}
	}
}

} // namespace ui
