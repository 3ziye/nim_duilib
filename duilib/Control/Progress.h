#ifndef UI_CONTROL_PROGRESS_H_
#define UI_CONTROL_PROGRESS_H_

#pragma once

#include "duilib/Control/Label.h"
#include "duilib/Image/Image.h"

namespace ui
{

/** �������ؼ�
*/
class UILIB_API Progress : public LabelTemplate<Control>
{
public:
	explicit Progress(Window* pWindow);
	virtual ~Progress();

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintStateImages(IRender* pRender) override;
	virtual void ClearImageCache() override;

	/** DPI�����仯�����¿ؼ���С�Ͳ���
	* @param [in] nOldDpiScale �ɵ�DPI���Űٷֱ�
	* @param [in] nNewDpiScale �µ�DPI���Űٷֱȣ���Dpi().GetScale()��ֵһ��
	*/
	virtual void ChangeDpiScale(uint32_t nOldDpiScale, uint32_t nNewDpiScale) override;

	/** �ж��Ƿ���ˮƽ������
	 * @return ���� true ��ˮƽ��������false Ϊ��ֱ������
	 */
	bool IsHorizontal() const;

	/** ����ˮƽ��ֱ������
	 * @param [in] bHorizontal Ϊ true ʱ����Ϊˮƽ��������false ʱ����Ϊ��ֱ��������Ĭ��Ϊ true
	 */
	void SetHorizontal(bool bHorizontal = true);

	/** ��ȡ��������Сֵ
	 * @return ���ؽ�������Сֵ
	 */
	int32_t GetMinValue() const;

	/** ���ý�������Сֵ
	 * @param [in] nMin ��Сֵ��ֵ
	 */
	void SetMinValue(int32_t nMin);

	/** ��ȡ���������ֵ
	 * @return ���ؽ��������ֵ
	 */
	int32_t GetMaxValue() const;

	/** ���ý��������ֵ
	 * @param [in] nMax Ҫ���õ����ֵ
	 */
	void SetMaxValue(int32_t nMax);

	/** ��ȡ��ǰ���Ȱٷֱ�
	 * @return ���ص�ǰ���Ȱٷֱ�
	 */
	double GetValue() const;

	/** ���õ�ǰ���Ȱٷֱ�
	 * @param[in] nValue Ҫ���õİٷֱ���ֵ
	 */
	void SetValue(double nValue);

	/** ������ǰ��ͼƬ�Ƿ�������ʾ
	 * @return ���� true Ϊ������ʾ��false Ϊ��������ʾ
	 */
	bool IsStretchForeImage();

	/** ���ý�����ǰ��ͼƬ�Ƿ�������ʾ
	 * @param [in] bStretchForeImage true Ϊ������ʾ��false Ϊ��������ʾ
	 */
	void SetStretchForeImage(bool bStretchForeImage = true);

	/** ��ȡ����������ͼƬ
	 * @return ���ر���ͼƬλ��
	 */
	std::wstring GetProgressImage() const;

	/** ���ý���������ͼƬ
	 * @param [in] strImage ͼƬ��ַ
	 */
	void SetProgressImage(const std::wstring& strImage);

	/** ��ȡ������������ɫ
	 * @return ���ر�����ɫ���ַ���ֵ����Ӧ global.xml �е�ָ��ɫֵ
	 */
	std::wstring GetProgressColor() const;

	/** ���ý�����������ɫ
	 * @param [in] Ҫ���õı�����ɫ�ַ��������ַ��������� global.xml �д���
	 */
	void SetProgressColor(const std::wstring& strProgressColor);

	/** ��ȡ������λ��
	 */
	virtual UiRect GetProgressPos();

	/** ����Marquee
	 */
	virtual void Play();

	/** ����Marquee
	 */
	void PaintMarquee(IRender* pRender);

	/** �Ƿ���Marquee
	 */
	bool IsMarquee() const;

	/** ����Marquee
	 */
	void SetMarquee(bool bMarquee);

	/** ��ȡMarquee���
	 */
	int32_t GetMarqueeWidth() const;

	/** ����Marquee���
	 */
	void SetMarqueeWidth(int32_t nMarqueeWidth, bool bNeedDpiScale);

	/** ��ȡMarquee����
	 */
	int32_t GetMarqueeStep() const;

	/** ����Marquee����
	 */
	void SetMarqueeStep(int32_t nMarqueeStep, bool bNeedDpiScale);

	/** ��ȡMarquee����ʱ��
	 */
	int32_t GetMarqueeElapsed() const;

	/** ����Marquee����ʱ��
	 */
	void SetMarqueeElapsed(int32_t nMarqueeElapsed);

	/** �����Ƿ�����������
	*/
	void SetReverse(bool bReverse);

protected:
	//��������ˮƽ��ֱ: trueΪˮƽ��falseΪ��ֱ
	bool m_bHorizontal;

	//ָ��������ǰ��ͼƬ�Ƿ�������ʾ
	bool m_bStretchForeImage;

	//�������ֵ��Ĭ��Ϊ100��
	int32_t m_nMax;

	//������Сֵ��Ĭ��Ϊ0��
	int32_t m_nMin;

	//��ǰ����ֵ
	double m_nValue;

	//������ǰ��ͼƬ
	Image* m_pProgressImage;

	//������ǰ��ͼƬ����
	UiString m_sProgressImageModify;

	//������ǰ����ɫ,��ָ����ʹ��Ĭ����ɫ
	UiString m_sProgressColor;

	//����Ч��
	bool m_bMarquee;
	int32_t m_nMarqueeWidth;
	int32_t m_nMarqueeStep;
	int32_t m_nMarqueeElapsed;
	int32_t m_nMarqueePos;

	//�Ƿ��������ȴ�100 �� 0��
	bool m_bReverse;

	//��ʱ��ȡ������
	nbase::WeakCallbackFlag m_timer;
};

} // namespace ui

#endif // UI_CONTROL_PROGRESS_H_
