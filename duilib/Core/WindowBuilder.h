#ifndef UI_CORE_WINDOWBUILDER_H_
#define UI_CORE_WINDOWBUILDER_H_

#pragma once

#include "duilib/duilib_defs.h"
#include <functional>
#include <string>
#include <memory>

namespace pugi
{
    //XML ��������ض���
    class xml_document;
    class xml_node;
}

namespace ui 
{

class Box;
class Window;
class Control;
class RichTextSlice;

/** �����ؼ��Ļص�����
*/
typedef std::function<Control* (const std::wstring&)> CreateControlCallback;

/** ����XML�ļ��������������ؼ��Ͳ���
*/
class UILIB_API WindowBuilder
{
public:
    WindowBuilder();
    ~WindowBuilder();

    WindowBuilder(const WindowBuilder&) = delete;
    WindowBuilder& operator = (const WindowBuilder&) = delete;

public:
    /** ʹ��XML�ļ��������ڲ��ֵ�
    @param [in] xml �������ļ��ı����ݣ������XML�ļ����ݣ��ַ�����Ҫ���ַ� '<'��ͷ;
                Ҳ������XML�ļ���·��
    @param [in] pWindow �����Ĵ���
    @param [in] pParent ������
    */
    Box* Create(const std::wstring& xml, 
                CreateControlCallback pCallback = CreateControlCallback(),
                Window* pWindow = nullptr, 
                Box* pParent = nullptr, 
                Box* pUserDefinedBox = nullptr);

    //ʹ�û����е�XML�ļ�(�������Create���������XML�ļ�)�������ڲ��ֵ�
    Box* Create(CreateControlCallback pCallback = CreateControlCallback(), 
                Window* pWindow = nullptr,
                Box* pParent = nullptr, 
                Box* pUserDefinedBox = nullptr);

public:
    /** ��������ʽ���ı����ݣ������õ�RichText Control����
    */
    static bool ParseRichTextXmlText(const std::wstring& xmlText, Control* pControl);
    
    /** ��������ʽ���ı����ݣ������õ�RichText Control����
    */
    static bool ParseRichTextXmlNode(const pugi::xml_node& xmlNode, Control* pControl, RichTextSlice* pTextSlice = nullptr);

private:
    Control* ParseXmlNode(const pugi::xml_node& xmlNode, Control* pParent = nullptr, Window* pWindow = nullptr);
	Control* CreateControlByClass(const std::wstring& strControlClass);
	void AttachXmlEvent(bool bBubbled, const pugi::xml_node& node, Control* pParent);

private:
    std::unique_ptr<pugi::xml_document> m_xml;
	CreateControlCallback m_createControlCallback;
};

} // namespace ui

#endif // UI_CORE_WINDOWBUILDER_H_
