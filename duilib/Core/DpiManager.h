#ifndef UI_CORE_DPI_MANAGER_H_
#define UI_CORE_DPI_MANAGER_H_

#pragma once

#include "duilib/Core/UiTypes.h"
#include "duilib/Core/DpiAwareness.h"

namespace ui
{
/** @class DpiManager
* @brief DPI���������
* @copyright (c) 2016, NetEase Inc. All rights reserved
* @author Redrain
* @date 2016/10/10
*/
class Window;

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
	* @param [in] dpiInitParam ��ʼ���������������˵��
	*/
	void InitDpiAwareness(const DpiInitParam& dpiInitParam);

	/** DPIֵ�Ƿ�Ϊ�û��Զ���(����InitDpiAwareness����ʱ��dpiInitParam.m_dpiFlag == DpiInitParam::DpiFlag::kFromUserDefine)
	*/
	bool IsUserDefineDpi() const;

	/** ��ȡ���̵�DPI��֪ģʽ
	*/
	DpiAwarenessMode GetDpiAwareness() const;

	/** �Ƿ�DPIΪÿ��ʾ��ģʽ
	*/
	bool IsPerMonitorDpiAware() const;

	/** ��DPI�����仯ʱ�������ü�����DPIֵΪ��ֵ
	* @param [out] nOldDPI ����и��£�����ԭ����DPIֵ
	* @return ������¹�������true�����򷵻�false
	*/
	bool UpdateDPI(uint32_t& nOldDPI);

	/** ���ݴ��ڵ�DPIֵ����ʼ��DPI��������DPIֵ
	    (Win10 �Ժ�ÿ����ʾ�������в�ͬ��DPIֵ�����Բ�ͬ�Ĵ��ڿ����в�ͬ��DPIֵ)
	 @param [in] pWindow ���ڵĽӿڣ����Ϊnullptr�����ȡϵͳ���õ�DPIֵ
	*/
	void SetDpiByWindow(Window* pWindow);

	/** ����DPIֵ�������½������ű�
	*   DPIֵ��Ӱ��������űȣ����õ�ֵ��ϵ���£�
	*   DPI ֵ    ���Űٷֱ�(Scaleֵ)
	* ---------------------------------------------
	*   96        100%
	*   120       125%
	*   144       150%
	*   168       175%
	*   192       200%
	*   216       225%
	*   240       250%
	*   288       300%
	* ---------------------------------------------
	* DPI�����ŰٷֱȵĹ�ϵ��DPI = ���Űٷֱ� * 96
	*                     ���������������������
	* ʾ����
	*     �����ű���Ϊ150%ʱ��DPI = 150% * 96 = 144
	* ---------------------------------------------
	* @param [in] uDPI DPIֵ���������Ϊ96����������������
	*/
	void SetDPI(uint32_t uDPI);

public:
	/** ��ȡDPIֵ
	*/
	uint32_t GetDPI() const;

	/** ��ȡ��ǰ�����������Űٷֱȣ�������100�������Űٷֱ�Ϊ100%�������ţ�
	* @return ���űȣ����磺����������ű�Ϊ125������������Űٷֱ�Ϊ125%�����Ӧ��DPIֵ��120
	*/
	uint32_t GetScale() const;

	/** ��ǰ�����Ƿ���DPI����
	* @return �����ǰ�������Űٷֱ�Ϊ100����false�����򷵻�true
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
	UiSize GetScaleSize(UiSize size) const;

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

	/** DPIֵ�Ƿ�Ϊ�û��Զ���
	*/
	bool m_bUserDefineDpi;

	/** ��ǰ��DPIֵ
	*/
	uint32_t m_uDpi;

	/** DPI�������ӣ�100��ʾ������
	*/
	uint32_t m_nScaleFactor;
};
}
#endif //UI_CORE_DPI_MANAGER_H_