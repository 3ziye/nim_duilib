#ifndef UI_UTILS_DPIMANAGER_H_
#define UI_UTILS_DPIMANAGER_H_

#pragma once

#include "duilib/duilib_defs.h"

namespace ui
{
/** @class DpiManager
  * @brief DPI���������
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2016/10/10
  */
class UILIB_API DpiManager
{
public:
	/**
	* @brief ��ȡĳ����ʾ����DPI������DPI��֪����Ч
	* @param[in] HMONITOR���
	* @return ���� DPIֵ
	*/
	static uint32_t GetMonitorDPI(HMONITOR hMonitor);

	/**
	* @brief ��ȡ����ʾ��DPI������DPI��֪����Ч
	* @return ���� DPIֵ
    */
	static uint32_t GetMainMonitorDPI();

public:
	/** ��ȡ��������
	*/
	static DpiManager* GetInstance();

	/**
	* @brief �Ƿ�����Լ�����DPI����Ч��
	* @return bool true �ǣ�false ��
	*/
	bool IsAdaptDPI() const;

	/**
	* @brief �����Ƿ�֧��DPI����Ӧ
	* @param [in] bAdaptDPI true��ʾ֧��DPI����Ӧ��false��ʾ��֧��DPI����Ӧ
	* @return bool true ���óɹ���false ����ʧ��
	*/
	bool SetAdaptDPI(bool bAdaptDPI);

	/**
	* @brief ��ȡ��ǰ�������űȣ�100���������ţ�
	* @return ���ű�
	*/
	uint32_t GetScale() const;

	/**
	* @brief ����DPIֵ���ý������űȣ�ֻ�г����Լ�����DPI����ʱ����Ч
	* @param[in] uDPI DPIֵ
	* @return void	�޷���ֵ
	*/
	void SetScale(uint32_t uDPI);

	/**
	* @brief ���ݽ������ű�������������ֻ�г����Լ�����DPI����ʱ����Ч
	* @param[in] iValue ����
	* @return int	���ź��ֵ
	*/
	int ScaleInt(int &iValue);

	/**
	* @brief ���ݽ������ű�������SIZE��ֻ�г����Լ�����DPI����ʱ����Ч
	* @param[in] pSize SIZEָ��
	* @return void	�޷���ֵ
	*/
	void ScaleSize(SIZE &size);
	void ScaleSize(UiSize &size);

	/**
	* @brief ���ݽ������ű�������POINT��ֻ�г����Լ�����DPI����ʱ����Ч
	* @param[in] pSize SIZEָ��
	* @return void	�޷���ֵ
	*/
	void ScalePoint(POINT &point);
	void ScalePoint(UiPoint &point);

	/**
	* @brief ���ݽ������ű�������RECT��ֻ�г����Լ�����DPI����ʱ����Ч
	* @param[in] pSize SIZEָ��
	* @return void	�޷���ֵ
	*/
	void ScaleRect(RECT &rect);
	void ScaleRect(UiRect &rect);

private:
	DpiManager();
	~DpiManager() {};
	DpiManager(const DpiManager&) = delete;
	DpiManager& operator = (const DpiManager&) = delete;

private:
	//DPI�������ӣ�100��ʾ������
	uint32_t m_nScaleFactor;

	//�Ƿ���DPI����Ӧ
	bool m_bAdaptDPI;
};
}
#endif //UI_UTILS_DPIMANAGER_H_