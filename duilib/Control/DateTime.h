#ifndef _UI_CONTROL_DATETIME_H_
#define _UI_CONTROL_DATETIME_H_
#pragma once
#include "duilib/Control/Label.h"

namespace ui
{
/** ����ʱ��ؼ�
*/
class DateTimeWnd;
class UILIB_API DateTime : public Label
{
    friend class DateTimeWnd;
public:
    DateTime();
    DateTime(const DateTime& r) = delete;
    DateTime& operator=(const DateTime& r) = delete;
    virtual ~DateTime();

public:
    /** ��ȡ���ڸ�ʽ
    */
    const std::wstring& GetFormat() const;
    
    /** �������ڸ�ʽ
    */
    void SetFormat(const std::wstring& val);

    /** �Ա���ʱ���ʼ��
    */
    void InitLocalTime();

    /** �������ʱ��ֵ
    */
    void ClearTime();

    /** ��ȡ��ǰ����ʱ���ַ���
    */
    std::wstring GetDateTime() const;

    /** ��ȡʱ��ֵ
    */
    const SYSTEMTIME& GetSystemTime() const;

    /** ����ʱ��ֵ
    */
    void SetTime(const SYSTEMTIME& systemTime);

    /** ��ǰʱ��ֵ�Ƿ�Ϊ��Ч������ʱ��
    */
    bool IsValidTime() const;

    /** ����ֻ��ģʽ
    */
    void SetReadOnly(bool bReadOnly);

    /** �Ƿ�Ϊֻ��ģʽ
    */
    bool IsReadOnly() const;

    /** �������ʱ��ֵ�仯�����¼�
    */
    void AttachTimeChange(const EventCallback& callback) { AttachEvent(kEventValueChange, callback); }

public:
    //������麯��
    virtual std::wstring GetType() const override;
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
    virtual void HandleEvent(const EventArgs& event) override;

private:
    //�Ƚ�����ʱ���Ƿ���ͬ
    bool IsEqual(const SYSTEMTIME& st) const;

private:
    //ϵͳʱ��ֵ
    SYSTEMTIME m_sysTime;

    //ֻ��ģʽ
    bool m_bReadOnly;

    /* ����ʱ���ʽ
    
    %a ���ڼ��ļ�д
    %A ���ڼ���ȫ��
    %b �·ֵļ�д
    %B �·ݵ�ȫ��
    %c ��׼�����ڵ�ʱ�䴮
    %C ��ݵĺ���λ����
    %d ʮ���Ʊ�ʾ��ÿ�µĵڼ���
    %D ��/��/��
    %e �����ַ����У�ʮ���Ʊ�ʾ��ÿ�µĵڼ���
    %F ��-��-��
    %g ��ݵĺ���λ���֣�ʹ�û����ܵ���
    %G ��֣�ʹ�û����ܵ���
    %h ��д���·���
    %H 24Сʱ�Ƶ�Сʱ
    %I 12Сʱ�Ƶ�Сʱ
    %j ʮ���Ʊ�ʾ��ÿ��ĵڼ���
    %m ʮ���Ʊ�ʾ���·�
    %M ʮʱ�Ʊ�ʾ�ķ�����
    %n ���з�
    %p ���ص�AM��PM�ĵȼ���ʾ
    %r 12Сʱ��ʱ��
    %R ��ʾСʱ�ͷ��ӣ�hh:mm
    %S ʮ���Ƶ�����
    %t ˮƽ�Ʊ��
    %T ��ʾʱ���룺hh:mm:ss
    %u ÿ�ܵĵڼ��죬����һΪ��һ�� ��ֵ��0��6������һΪ0��
    %U ����ĵڼ��ܣ�����������Ϊ��һ�죨ֵ��0��53��
    %V ÿ��ĵڼ��ܣ�ʹ�û����ܵ���
    %w ʮ���Ʊ�ʾ�����ڼ���ֵ��0��6��������Ϊ0��
    %W ÿ��ĵڼ��ܣ�������һ��Ϊ��һ�죨ֵ��0��53��
    %x ��׼�����ڴ�
    %X ��׼��ʱ�䴮
    %y �������͵�ʮ������ݣ�ֵ��0��99��
    %Y �����Ͳ��ֵ�ʮ�������
    %z��%Z ʱ�����ƣ�������ܵõ�ʱ�������򷵻ؿ��ַ���
    %% �ٷֺ�
    */
    std::wstring m_sFormat;

    //�������ڿؼ����ڽӿ�
    DateTimeWnd* m_pDateWindow;
};

}//namespace ui

#endif // _UI_CONTROL_DATETIME_H_
