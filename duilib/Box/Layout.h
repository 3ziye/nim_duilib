#ifndef UI_BOX_LAYOUT_H_
#define UI_BOX_LAYOUT_H_

#pragma once

#include "duilib/Core/UiTypes.h"
#include <string>
#include <vector>

namespace ui 
{
class Box;
class Control;

/** �ؼ����֣������ӿؼ���������(left,top)����ͬ�������ؼ��ѵ����У��൱�ڶ�������Float�ؼ���
*            �ӿؼ�����ָ��������뷽ʽ��������뷽ʽ�����е�ʱ��ᰴ���ӿؼ�ָ���Ķ��뷽ʽ����
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
	 * @param[in] rcContainer Ҫ���õľ������򣬰����ڱ߾࣬������߾�
	 * @return ���ؿؼ����յĴ�С��Ϣ����Ⱥ͸߶ȣ�
	 */
	static UiSize64 SetFloatPos(Control* pControl, const UiRect& rcContainer);

	/** ���ò�������
	 * @param[in] strName Ҫ���õ�������
	 * @param[in] strValue Ҫ���õ�����ֵ
	 * @return true ���óɹ���false ���Բ�����
	 */
	virtual bool SetAttribute(const std::wstring& strName, const std::wstring& strValue);

	/** �����ڲ����пؼ���λ����Ϣ
	 * @param[in] items �ؼ��б�
	 * @param[in] rc ��ǰ����λ����Ϣ, �����ڱ߾࣬����������߾�
	 * @return �������к����պ��ӵĿ�Ⱥ͸߶���Ϣ������Owner Box���ڱ߾࣬��������߾�
	 */
	virtual UiSize64 ArrangeChild(const std::vector<Control*>& items, UiRect rc);

	/** �����ڲ��ӿؼ���С�������������С���������͵��ӿؼ������ԣ��������С����
	 * @param[in] items �ӿؼ��б�
	 * @param [in] szAvailable ���ô�С������������ÿؼ����ڱ߾࣬��������������ؼ�����߾�
	 * @return �������к����ղ��ֵĴ�С��Ϣ����Ⱥ͸߶ȣ���
	           ����items���ӿؼ�����߾࣬����items���ӿؼ����ڱ߾ࣻ
			   ����Box�ؼ�������ڱ߾ࣻ
			   ������Box�ؼ��������߾ࣻ
	           ����ֵ�в������������͵��ӿؼ���С��
	 */
	virtual UiSize EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable);

	/** �Ƿ���Tile���֣�Tile�������޿�ȵģ���Щ�߼���ͬ��
	*/
	virtual bool IsTileLayout() const;

public:
	/** ��ȡ�ڱ߾�
	 * @return �����ڱ߾��ıߵĴ�С��Rect���ĸ������ֱ�����ĸ��ߵ��ڱ߾ࣩ
	 */
	const UiPadding& GetPadding() const { return m_rcPadding; }

	/**
	 * @brief �����ڱ߾�
	 * @param[in] rcPadding �ڱ߾�����
	 * @param[in] bNeedDpiScale �Ƿ���� DPI ����Ӧ��Ĭ��Ϊ true
	 */
	void SetPadding(UiPadding rcPadding, bool bNeedDpiScale);

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

	/** ������ȥ���ڱ߾�, ��ȷ��rc������Ч
	*/
	void DeflatePadding(UiRect& rc) const;

protected:
	/** ������õĿ�͸��Ƿ���ȷ, ������ִ��󣬸������
	*/
	void CheckConfig(const std::vector<Control*>& items);

	/** ���տؼ�ָ���Ķ��뷽ʽ������ؼ��Ĳ���λ��
	* @param [in] pControl �ؼ��Ľӿ�
	* @param [in] rcContainer Ŀ�������ľ��Σ������ؼ�����߾���ڱ߾�
	* @param [in] childSize �ؼ�pControl�Ĵ�С����͸ߣ�, �����ڱ߾࣬�ڲ������ټ���ؼ��Ĵ�С
	* @return ���ؿؼ���λ�úʹ�С����������߾࣬�����ڱ߾�
	          �������ֵ������pControl->SetPos(rect)�������ؼ�λ��;
	*/
	static UiRect GetFloatPos(Control* pControl, UiRect rcContainer, UiSize childSize);

protected:

	//����Box����
	Box* m_pOwner;

private:
	//�ڱ߾��ıߵĴ�С���ϣ��£����ұ߾ࣩ���ڱ߾��ǿؼ��������ڵĿռ䣬�ǰ����ڿؼ��������ڵ�
	UiPadding m_rcPadding;

	//�ӿؼ�֮��Ķ���߾�: X �᷽��
	int32_t m_iChildMarginX;

	//�ӿؼ�֮��Ķ���߾�: Y �᷽��
	int32_t m_iChildMarginY;
};

} // namespace ui

#endif // UI_BOX_LAYOUT_H_
