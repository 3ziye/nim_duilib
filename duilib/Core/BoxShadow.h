#ifndef UI_UTILS_BOXSHADOW_H_
#define UI_UTILS_BOXSHADOW_H_

#pragma once

#include "duilib/duilib_defs.h"
#include <string>

namespace ui {

// �ؼ���Ӱ������
// ���Ż���
// 1.ʵ��������˹ģ��
//   �ο����ϣ�
//           https://www.cnblogs.com/mfbzr/p/14154156.html
//           https://stackoverflow.com/questions/42314939/diagonal-shadow-with-gdi
//           https://blog.csdn.net/aoshilang2249/article/details/45153375
//			 https://codersblock.com/blog/creating-glow-effects-with-css/
//   �㷨:
//           http://blog.ivank.net/fastest-gaussian-blur.html
//           https://software.intel.com/content/www/us/en/develop/articles/iir-gaussian-blur-filter-implementation-using-intel-advanced-vector-extensions.html
//           https://software.intel.com/content/dam/develop/public/us/en/downloads/gaussian_blur_0311.cpp
// 2.��ӰӦ��ǰ���ƺò����л��棬������ʵʱ����
class UILIB_API BoxShadow 
{
public:
	BoxShadow();
	~BoxShadow();

	/**
	 * @brief ������Ӱ����
	 * @param[in] strBoxShadow Ҫ���õ�����,�� "color='black' offset='1,1' blursize='2'"
	 * @return ��
	 */
	void SetBoxShadowString(const std::wstring& strBoxShadow);

	/**
	 * @brief �Ƿ�����Ӱ
	 * @return �ǻ��
	 */
	bool HasShadow() const;

	// ��Ӱ����
	// ��ʱ����֧�� color offset blurSize
	// https://codersblock.com/blog/creating-glow-effects-with-css/

	/** ��Ӱ����ɫֵ
	*/
	std::wstring m_strColor;

	/** ������Ӱƫ������offset-x �� offset-y��
	 *                 <offset-x> ����ˮƽƫ����������Ǹ�ֵ����Ӱλ�ھ�����ߡ� 
	 *                 <offset-y> ���ô�ֱƫ����������Ǹ�ֵ����Ӱλ�ھ������档
	 */
	UiPoint m_cpOffset;

	/** ģ���뾶���뾶������� 0 �� 255 �ķ�Χ�ڡ�
	 *  ֵԽ��ģ�����Խ����Ӱ��Խ��Խ���� ����Ϊ��ֵ��Ĭ��Ϊ0����ʱ��Ӱ��Ե������
	 */
	int m_nBlurRadius;

	/** ��չ�뾶����ģ���������rc���α�Ե���ٸ����ء�
	 *   ȡ��ֵʱ����Ӱ����ȡ��ֵʱ����Ӱ������Ĭ��Ϊ0����ʱ��Ӱ��Ԫ��ͬ����
	 */
	int m_nSpreadRadius;

	/** ��Ϊtrue��ʱ���ʾ��Ӱ�ھ��α߿����棬Ϊfalse��ʱ���ʾ��Ӱ�ڱ߿�����ڲ�
	*/
	bool m_bExclude;
};

} // namespace ui

#endif // !UI_UTILS_BOXSHADOW_H_

