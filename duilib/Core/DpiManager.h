#ifndef UI_UTILS_DPIMANAGER_H_
#define UI_UTILS_DPIMANAGER_H_

#pragma once

#include "duilib/Core/UiTypes.h"

namespace ui
{
/** @class DpiManager
* @brief DPI���������
* @copyright (c) 2016, NetEase Inc. All rights reserved
* @author Redrain
* @date 2016/10/10
*/

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
	DpiAwarenessFlag m_dpiAwarenessFlag = DpiInitParam::DpiAwarenessFlag::kFromUserDefine;

	/** �������õ�DPI��֪����ģʽ(���� m_dpiAwarenessFlag Ϊ kFromUserDefineʱ��Ч)
	*/
	DpiAwarenessMode m_dpiAwarenessMode = DpiAwarenessMode::kPerMonitorDpiAware_V2;

	/** DPIֵ�Ĳ������ͣ�Ĭ�϶�ȡϵͳ���õ�DPIֵ������ȡm_uDPIֵ��
	*/
	DpiFlag m_dpiFlag = DpiInitParam::DpiFlag::kFromSystem;

	/** �������õ�DPIֵ(���� m_dpiFlag Ϊ kFromUserDefineʱ��Ч)
	*/
	uint32_t m_uDPI = 96;
};

/** DPI��֪���ܵĽӿ�
*/
class UILIB_API DpiManager
{
public:
	DpiManager();
	~DpiManager();
	DpiManager(const DpiManager&) = delete;
	DpiManager& operator = (const DpiManager&) = delete;

public:
	/** ��ʼ��DPI��֪ģʽ��DPIֵ���ú���ֻ�ɵ���һ�Σ��������ñ����ԣ�
	* @param [in] initParam ��ʼ���������������˵��
	*/
	bool InitDpiAwareness(const DpiInitParam& initParam);
	
	/** ��ȡ���̵�DPI��֪ģʽ
	*/
	DpiAwarenessMode GetDpiAwareness() const;

public:
	/** ����DPIֵ�������½������ű�
	*   DPIֵ��Ӱ��������űȣ����õ�ֵ��ϵ���£�
	*   DPI ֵ    ���Űٷֱ�(Scaleֵ)
	* -------------------------------
	*   96        100%
	*   120       125%
	*   144       150%
	*   168       175%
	*   192       200%
	* @param [in] uDPI DPIֵ���������Ϊ96����������������
	*/
	void SetDPI(uint32_t uDPI);

	/** ��ȡDPIֵ
	*/
	uint32_t GetDPI() const;

	/** ��ȡ��ǰ�������űȣ�100���������ţ�
	* @return ���űȣ����磺����������ű�Ϊ125�����ӦDPIֵ��120
	*/
	uint32_t GetScale() const;

	/** ��ǰ�����Ƿ���DPI����
	* @return �����ǰ�������ű�Ϊ100����false�����򷵻�true
	*/
	bool IsScaled() const;

public:
	/** ���ݽ������ű�����������
	* @param[in] iValue ����
	* @return int ���ź��ֵ
	*/
	int32_t ScaleInt(int32_t& iValue) const;
	int32_t GetScaleInt(int32_t iValue) const;
	uint32_t GetScaleInt(uint32_t iValue) const;

	/** ���ݽ������ű�������SIZE
	* @param[in] size ��Ҫ���ŵ�Size����
	*/
	void ScaleSize(SIZE& size) const;
	void ScaleSize(UiSize& size) const;

	/** ���ݽ������ű�������POINT
	* @param[in] point ��Ҫ���ŵ�point����
	*/
	void ScalePoint(POINT& point) const;
	void ScalePoint(UiPoint& point) const;

	/** ���ݽ������ű�������RECT
	* @param[in] rect ��Ҫ���ŵ�rect����
	* @return void	�޷���ֵ
	*/
	void ScaleRect(RECT& rect) const;
	void ScaleRect(UiRect& rect) const;
	void ScalePadding(UiPadding& padding) const;
	void ScaleMargin(UiMargin& margin) const;

private:

	/** ���ý��̵�DPI��֪ģʽ
	   ��DPI��֪ģʽֻ��������һ�Σ����óɹ��������޸ģ�����ڿ�ִ�г����manifest�����������˸�֪ģʽ��Ҳ�޷����ģ�
	* @param [in] dpiAwarenessMode ������DPI��֪ģʽ������������𲽽�������
	*             ����Ϊ��kPerMonitorDpiAware_V2 -> kPerMonitorDpiAware -> kSystemDpiAware
	* @return ����ʵ�ʵĽ���DPI��֪ģʽ
	*/
	DpiAwarenessMode SetDpiAwareness(DpiAwarenessMode dpiAwarenessMode) const;

	/** ��ȡ��ǰ���̵�DPI��֪ģʽ
	*/
	DpiAwarenessMode GetDpiAwarenessMode() const;

	/** ��ȡ����ʾ��DPI������DPI��֪����Ч
	* @return ���� DPIֵ
	*/
	uint32_t GetMainMonitorDPI();

	/** MulDiv ������װ
	*/
	int32_t MulDiv(int32_t nNumber, int32_t nNumerator, int32_t nDenominator) const;

private:
	/** �Ƿ��Ѿ���ʼ����
	*/
	bool m_bDpiInited;

	/** ��ǰ���̵�DPI��֪ģʽ
	*/
	DpiAwarenessMode m_dpiAwarenessMode;

	/** ��ǰ��DPIֵ
	*/
	uint32_t m_uDpi;

	/** DPI�������ӣ�100��ʾ������
	*/
	uint32_t m_nScaleFactor;
};
}
#endif //UI_UTILS_DPIMANAGER_H_