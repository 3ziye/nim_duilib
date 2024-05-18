#ifndef UI_CONTROL_SLIDER_H_
#define UI_CONTROL_SLIDER_H_

#pragma once

#include "duilib/Control/Progress.h"
#include <string>

namespace ui
{

/** ����ؼ�
*/
class UILIB_API Slider : public Progress
{
public:
	explicit Slider(Window* pWindow);

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual UiRect GetProgressPos() override;
	virtual void HandleEvent(const EventArgs& msg) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintStateImages(IRender* pRender) override;
	virtual void PaintBkColor(IRender* pRender) override;
	virtual void ClearImageCache() override;

	/** DPI�����仯�����¿ؼ���С�Ͳ���
	* @param [in] nOldDpiScale �ɵ�DPI���Űٷֱ�
	* @param [in] nNewDpiScale �µ�DPI���Űٷֱȣ���Dpi().GetScale()��ֵһ��
	*/
	virtual void ChangeDpiScale(uint32_t nOldDpiScale, uint32_t nNewDpiScale) override;

	/** ��ȡ��������
	 * @return ���ز�������
	 */
	int GetChangeStep();

	/** ���ò�������
	 * @param[in] step Ҫ���õĲ���
	 */
	void SetChangeStep(int step);

	/** ���û���Ĵ�С
	 * @param [in] szXY Ҫ���õĴ�С
	 * @param [in] bNeedDpiScale �Ƿ� DPI ����
	 */
	void SetThumbSize(UiSize szXY, bool bNeedDpiScale);

	/** ��ȡ����Ĵ�С
	*/
	UiSize GetThumbSize() const;

	/** ��ȡ����ľ�����Ϣ
	 * @return ���ػ���ľ�����Ϣ
	 */
	UiRect GetThumbRect() const;

	/** ��ȡָ��״̬�»����ͼƬ
	 * @param[in] stateType Ҫ��ȡ��״̬��ʶ���ο� ControlStateType ö��
	 * @return ����ͼƬ·��
	 */
	std::wstring GetThumbStateImage(ControlStateType stateType) const;

	/** ����ָ��״̬�»����ͼƬ
	 * @param[in] stateType Ҫ���õ�״̬��ʶ���ο� ControlStateType ö��
	 * @param[in] pStrImage Ҫ���õ�ͼƬλ��
	 */
	void SetThumbStateImage(ControlStateType stateType, const std::wstring& pStrImage);

	/** ��ȡ�������ڱ߾�
	 */
	const UiPadding& GetProgressBarPadding() const;

	/** ���ý������ڱ߾�
	 * @param [in] padding Ҫ���õ��ڱ߾���Ϣ
	 * @param [in] bNeedDpiScale �Ƿ� DPI ����
	 */
	void SetProgressBarPadding(UiPadding padding, bool bNeedDpiScale);

	/** �������������ȸı��¼�
	 * @param[in] callback ���������ȸı����õĻص�����
	 */
	void AttachValueChange(const EventCallback& callback) { AttachEvent(kEventValueChange, callback);	}

protected:
	int m_nStep;
	UiSize m_szThumb;
	StateImage m_thumbStateImage;
	UiPadding m_rcProgressBarPadding;
	UiString m_sImageModify;
};

}

#endif // UI_CONTROL_SLIDER_H_