#pragma once

// base header
#include "base/base.h"

// duilib
#include "duilib/duilib.h"

/** �ļ���Ϣ
*/
struct FileInfo
{
    //�ļ���, ����·��
    std::wstring m_fileName;
    //�ļ�����·����
    std::wstring m_filePath;
    //�Ƿ�Ϊ�ļ���
    bool m_isFolder = false;
    //������ͼ��(��Դ���������ڸ����ڲ�����)
    HICON m_hIcon = nullptr;
};

class FileInfoList: public ui::VirtualListBoxElement
{
public:
    FileInfoList();
    virtual ~FileInfoList();

    /** ����һ��������
    * @return ���ش������������ָ��
    */
    virtual ui::Control* CreateElement() override;

    /** ���ָ��������
    * @param [in] pControl ������ؼ�ָ��
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    */
    virtual bool FillElement(ui::Control* pControl, size_t nElementIndex) override;

    /** ��ȡ����������
    * @return ��������������
    */
    virtual size_t GetElementCount() override;

    /** ����ѡ��״̬
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    * @param [in] bSelected true��ʾѡ��״̬��false��ʾ��ѡ��״̬
    */
    virtual void SetElementSelected(size_t nElementIndex, bool bSelected) override;

    /** ��ȡѡ��״̬
    * @param [in] nElementIndex ����Ԫ�ص�����ID����Χ��[0, GetElementCount())
    * @return true��ʾѡ��״̬��false��ʾ��ѡ��״̬
    */
    virtual bool IsElementSelected(size_t nElementIndex) override;

public:
    
    /** ����ļ��б�ԭ�����б���
    */
    void AddFileList(const std::vector<FileInfo>& pathList);

    /** ���µ��ļ��б��滻ԭ�����б�
    */
    void SetFileList(const std::vector<FileInfo>& pathList);

    /** ����ļ��б�
    */
    void ClearFileList();

private:
    /** ���˫��ʱ����
     * @param[in] args ��Ϣ��
     * @return ʼ�շ��� true
     */
    bool OnDoubleClickItem(const ui::EventArgs& args);

private:
    //�ļ��б�
    std::vector<FileInfo> m_pathList;
};

