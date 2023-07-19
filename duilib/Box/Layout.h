#ifndef UI_BOX_LAYOUT_H_
#define UI_BOX_LAYOUT_H_

#pragma once

#include "duilib/Core/Control.h"
#include <vector>

namespace ui 
{
class Box;

/** �ؼ����֣������ӿؼ���������(left,top)����ͬ�������ؼ��ѵ����У��൱�ڶ�������Float�ؼ���
*/
class UILIB_API Layout
{
public:
	Layout();
	Layout(const Layout& r) = delete;
	Layout& operator=(const Layout& r) = delete;
	virtual ~Layout() = default;

	/** ����������
	 */
	void SetOwner(Box* pOwner);

	/** ���ø���״̬�µ�������Ϣ
	 * @param[in] pControl �ؼ����
	 * @param[in] rcContainer Ҫ���õ�λ����Ϣ
	 * @return ���ؿؼ����յĴ�С��Ϣ����Ⱥ͸߶ȣ�
	 */
	static UiSize64 SetFloatPos(Control* pControl, UiRect rcContainer);

	/** ���ò�������
	 * @param[in] strName Ҫ���õ�������
	 * @param[in] strValue Ҫ���õ�����ֵ
	 * @return true ���óɹ���false ���Բ�����
	 */
	virtual bool SetAttribute(const std::wstring& strName, const std::wstring& strValue);

	/** �����ڲ����пؼ���λ����Ϣ
	 * @param[in] items �ؼ��б�
	 * @param[in] rc ��ǰ����λ����Ϣ
	 * @return �������к����պ��ӵĿ�Ⱥ͸߶���Ϣ
	 */
	virtual UiSize64 ArrangeChild(const std::vector<Control*>& items, UiRect rc);

	/**
	 * @brief �����ڲ��ӿؼ���С�������������С
	 * @param[in] items �ؼ��б�
	 * @param[in] szAvailable �ӿؼ�����������
	 * @return �������к����ղ��ֵĴ�С��Ϣ����Ⱥ͸߶ȣ�
	 */
	virtual UiSize EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable);

public:
	/** ��ȡ�ڱ߾�
	 * @return �����ڱ߾��ıߵĴ�С��Rect���ĸ������ֱ�����ĸ��ߵ��ڱ߾ࣩ
	 */
	const UiRect& GetPadding() const { return m_rcPadding; }

	/**
	 * @brief �����ڱ߾�
	 * @param[in] rcPadding �ڱ߾�����
	 * @param[in] bNeedDpiScale �Ƿ���� DPI ����Ӧ��Ĭ��Ϊ true
	 */
	void SetPadding(UiRect rcPadding, bool bNeedDpiScale);

	/** ��ȡ�ӿؼ�֮��Ķ���߾ࣨX�᷽��
	 * @return ���ض��������ֵ
	 */
	int32_t GetChildMarginX() const { return m_iChildMarginX; }

	/** ��ȡ�ӿؼ�֮��Ķ���߾ࣨY�᷽��
	 * @return ���ض��������ֵ
	 */
	int32_t GetChildMarginY() const { return m_iChildMarginY; }

	/** �����ӿؼ�֮��Ķ���߾ࣨX�᷽��
	 * @param[in] iMarginX Ҫ���õı߾���ֵ
	 */
	void SetChildMarginX(int32_t iMarginX);

	/** �����ӿؼ�֮��Ķ���߾ࣨY�᷽��
	 * @param[in] iMarginY Ҫ���õı߾���ֵ
	 */
	void SetChildMarginY(int32_t iMarginY);

	/** �����ӿؼ�֮��Ķ���߾ࣨX�᷽���Y�᷽�򣬾�����Ϊͬһ��ֵ��
	 * @param[in] iMargin Ҫ���õı߾���ֵ
	 */
	void SetChildMargin(int32_t iMargin);

	/** ��ȡ�����ڱ߾���Ŀ��÷�Χ
	 * @return ���ؿ��÷�Χλ����Ϣ
	 */
	UiRect GetInternalPos() const;

protected:

	//����Box����
	Box* m_pOwner;

private:
	//�ڱ߾��ıߵĴ�С
	UiRect m_rcPadding;

	//�ӿؼ�֮��Ķ���߾�: X �᷽��
	int32_t m_iChildMarginX;

	//�ӿؼ�֮��Ķ���߾�: Y �᷽��
	int32_t m_iChildMarginY;
};

} // namespace ui

#endif // UI_BOX_LAYOUT_H_
