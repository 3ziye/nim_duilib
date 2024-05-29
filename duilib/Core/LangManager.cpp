#include "LangManager.h"
#include "duilib/Utils/StringUtil.h"
#include "duilib/Utils/FileUtil.h"

namespace ui 
{
LangManager::LangManager()
{
};

LangManager::~LangManager()
{
    m_stringTable.clear();
};

bool LangManager::LoadStringTable(const DString& strFilePath)
{
    m_stringTable.clear();
    std::vector<uint8_t> fileData;
    FileUtil::ReadFileData(strFilePath, fileData);
    ASSERT(!fileData.empty());
    if (fileData.empty()) {
        return false;
    }
    return LoadStringTable(fileData);
}

bool LangManager::LoadStringTable(const std::vector<uint8_t>& fileData)
{
    std::vector<DString> string_list;
    if (fileData.empty()) {
        return false;
    }
    size_t bomSize = 0;
    if ((fileData.size() >= 3)   &&
        (fileData.at(0) == 0xEF) &&
        (fileData.at(1) == 0xBB) &&
        (fileData.at(2) == 0xBF) ) {
        //跳过UTF8的BOM头
        bomSize = 3;
    }
    std::string fragment((const char*)fileData.data() + bomSize, fileData.size() - bomSize);
    StringUtil::ReplaceAll("\r\n", "\n", fragment);
    StringUtil::ReplaceAll("\r", "\n", fragment);
    fragment.append("\n");
    std::string src;
    DString string_resourse;
    for (const auto& it : fragment)    {
        if (it == '\0' || it == '\n') {
            if (!src.empty()) {
                string_resourse.clear();
                string_resourse = StringUtil::UTF8ToT(src);
                string_resourse = StringUtil::Trim(string_resourse);
                if (!string_resourse.empty()) {
                    string_list.push_back(string_resourse);
                }
                src.clear();
            }
            continue;
        }
        src.push_back(it);
    }
    AnalyzeStringTable(string_list);
    return true;
}

void LangManager::ClearStringTable()
{
    m_stringTable.clear();
}

bool LangManager::AnalyzeStringTable(const std::vector<DString>& list)
{
    int    nCount = (int)list.size();
    if (nCount <= 0) {
        return false;
    }
    DString id;
    DString strResource;
    for (int i = 0; i < nCount; ++i) {
        const DString& strSrc = list[i];
        if (strSrc.empty() || strSrc.at(0) == _T(';')) {
            //注释以";"开头
            continue;
        }
        size_t pos = strSrc.find(_T('='));
        if (pos == DString::npos) {
            //无分隔符，忽略
            continue;
        }

        id = strSrc.substr(0, pos);
        if (!id.empty()) {
            id = StringUtil::Trim(id);
        }
        if ((pos + 1) < strSrc.size()) {
            strResource = strSrc.substr(pos + 1);
            strResource = StringUtil::Trim(strResource);
            //将\n和\r替换为真实的换行符、回车符
            StringUtil::ReplaceAll(_T("\\r"), _T("\r"), strResource);
            StringUtil::ReplaceAll(_T("\\n"), _T("\n"), strResource);
        }
        else {
            strResource.clear();
        }
        if (!id.empty()) {
            m_stringTable[id] = strResource;
        }
    }
    return true;
}

DString LangManager::GetStringViaID(const DString& id)
{
    DString text;
    if (id.empty()) {
        return text;
    }
    auto it = m_stringTable.find(id);
    if (it == m_stringTable.end()) {
        ASSERT(!"MultiLang::GetStringViaID failed!");
        return text;
    }
    else {
        text = it->second;
    }
    return text;
}

}//namespace ui 
