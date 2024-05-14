#ifndef UI_CORE_DPI_AWARENESS_H_
#define UI_CORE_DPI_AWARENESS_H_

#pragma once

#include "duilib/Core/UiTypes.h"

namespace ui
{
/** DPI��֪ģʽ
*/
enum class UILIB_API DpiAwarenessMode
{
	/** �޷���֪
	 *    Ӧ�ó���� DPI ��ͼ��������ʾ����Ϊ 96 DPI
	 *	  DPI ����ʱ����Ϊ��λͼ���죨ģ����
	 */
	kDpiUnaware = 0,

	/** ϵͳ
	 *  �����Windows�汾��Vista
	 *  Ӧ�ó���� DPI ��ͼ��������ʾ����Ϊ 96 DPI
	 *	DPI ����ʱ����Ϊ��λͼ���죨ģ������
	 */
	kSystemDpiAware = 1,

	/** ÿ��ʾ��
	 *  �����Windows�汾��8.1
	 *  Ӧ�ó���� DPI ��ͼ��Ӧ�ó��򴰿���Ҫλ�ڵ���ʾ���� DPI
	 *	DPI ����ʱ����Ϊ��֪ͨ���� HWND DPI ���ģ�û���κ� UI Ԫ�ص� DPI ���š�
	 */
	kPerMonitorDpiAware = 2,

	/** ÿ��ʾ�� V2
	 *  �����Windows�汾��Windows 10 �����߸��� (1703)
	 *  Ӧ�ó���� DPI ��ͼ��Ӧ�ó��򴰿���Ҫλ�ڵ���ʾ���� DPI
	 *	DPI ����ʱ����Ϊ��֪ͨ���� HWND DPI ���ģ��Զ� DPI ���ţ��ǹ����������ÿؼ��е��������λͼ (comctl32 V6)���Ի��� (CreateDialog)��
	 */
	kPerMonitorDpiAware_V2 = 3
};

/** DPI��֪���ܳ�ʼ������
*/
class UILIB_API DpiInitParam
{
public:
	DpiInitParam();

	enum class DpiAwarenessFlag
	{
		kFromManifest,  //�ӿ�ִ�г����manifest�����ж�ȡ������Ҫ�ٴ�����
		kFromUserDefine //ͨ���������ã�����DpiInitParam::m_dpiAwarenessMode ��Ա������
	};
	enum class DpiFlag
	{
		kFromSystem,    //�Ӳ���ϵͳ��DPI�����ж�ȡ
		kFromUserDefine //ͨ���������ã�����DpiInitParam::m_uDPI ��Ա������
	};

	/** DPI��֪�������ͣ�Ĭ����ͨ�� m_dpiAwarenessMode ֵ���õģ�
	*/
	DpiAwarenessFlag m_dpiAwarenessFlag;

	/** �������õ�DPI��֪����ģʽ(���� m_dpiAwarenessFlag Ϊ kFromUserDefineʱ��Ч)
	*/
	DpiAwarenessMode m_dpiAwarenessMode;

	/** DPIֵ�Ĳ������ͣ�Ĭ�϶�ȡϵͳ���õ�DPIֵ������ȡm_uDPIֵ��
	*/
	DpiFlag m_dpiFlag;

	/** �������õ�DPIֵ(���� m_dpiFlag Ϊ kFromUserDefineʱ��Ч)
	*/
	uint32_t m_uDPI;
};

/** DPI��֪���ܵĽӿ�
*/
class UILIB_API DpiAwareness
{
public:
	DpiAwareness();
	~DpiAwareness();
	DpiAwareness(const DpiAwareness&) = delete;
	DpiAwareness& operator = (const DpiAwareness&) = delete;

public:
	/** ��ʼ��DPI��֪ģʽ��DPIֵ���ú���ֻ�ɵ���һ�Σ��������ñ����ԣ�
	* @param [in] initParam ��ʼ���������������˵��
	*/
	bool InitDpiAwareness(const DpiInitParam& initParam);

	/** ���ý��̵�DPI��֪ģʽ
	   ��DPI��֪ģʽֻ��������һ�Σ����óɹ��������޸ģ�����ڿ�ִ�г����manifest�����������˸�֪ģʽ��Ҳ�޷����ģ�
	* @param [in] dpiAwarenessMode ������DPI��֪ģʽ������������𲽽�������
	*             ����Ϊ��kPerMonitorDpiAware_V2 -> kPerMonitorDpiAware -> kSystemDpiAware
	* @return ����ʵ�ʵĽ���DPI��֪ģʽ
	*/
	DpiAwarenessMode SetDpiAwareness(DpiAwarenessMode dpiAwarenessMode) const;

	/** ��ȡ��ǰ���̵�DPI��֪ģʽ
	*/
	DpiAwarenessMode GetDpiAwareness() const;
};
}
#endif //UI_CORE_DPI_AWARENESS_H_