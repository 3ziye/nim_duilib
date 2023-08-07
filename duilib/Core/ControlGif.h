#ifndef UI_CORE_CONTROL_GIF_H_
#define UI_CORE_CONTROL_GIF_H_

#pragma once

#include "duilib/Core/UiTypes.h"
#include "duilib/Utils/Delegate.h"
#include "base/callback/callback.h"
#include <memory>
#include <map>

namespace ui 
{
class Image;
class Control;
class IRender;
typedef std::map<int32_t, CEventSource> GifEventMap;

/** �ؼ�������̬���Ŷ������߼���װ��֧��GIF/WebP/APNG������
*/
class ControlGif: public nbase::SupportWeakCallback
{
public:
	explicit ControlGif(Control* pControl);
	~ControlGif();
	ControlGif(const ControlGif&) = delete;
	ControlGif& operator = (const ControlGif&) = delete;

public:
	/** ���ù����ı���ͼƬ
	*/
	void SetBkImage(const std::shared_ptr<Image>& pBkImage);

	/** GIF�������ű�־
	*/
	bool CanGifPlay() const;

	/** ��ʼ���Ŷ���
	*@return �ɹ�����true�����򷵻�false
	*/
	bool StartGifPlay(bool bRedraw);

	/** ֹͣ����GIF����
	* @return �ɹ�����true�����򷵻�false
	*/
	bool StopGifPlay(GifStopType frame = kGifStopCurrent);

	/** ֹͣ���� GIF
	 * @param [in] transfer �Ƿ�ֹͣ�¼�֪ͨ�������ߣ��ο� AttachGifPlayStop ����
	 * @param [frame] frame ���Ž���ֹͣ����һ֡�������õ�һ֡����ǰ֡�����һ֡����ο� GifStopType ö��
	 */
	bool StopGifPlayForUI(bool transfer = false, GifStopType frame = kGifStopCurrent);

	/** ���� GIF
	 * @param[in] �������ֹͣ����һ֡�������õ�һ֡����ǰ֡�����һ֡����ο� GifStopType ö��
	 */
	bool StartGifPlayForUI(GifStopType frame = kGifStopFirst, int32_t playcount = -1);

	/** ���� GIF �������֪ͨ
	 * @param[in] callback Ҫ���� GIF ֹͣ���ŵĻص�����
	 */
	void AttachGifPlayStop(const EventCallback& callback) { m_OnGifEvent[m_nVirtualEventGifStop] += callback; };

private:
	/** ������߷��Ͳ���ֹͣ�¼�
	*/
	void BroadcastGifEvent(int32_t nVirtualEvent) const;

	/** ��ȡGIF֡��
	*/
	uint32_t GetGifFrameIndex(GifStopType frame) const;

	/** ��ʱ������GIF�Ļص�����
	*/
	bool GifPlay();

private:
	//�Ƿ�Ϊ����GIF��״̬��������ͼƬm_pBkImage��GIF�ļ�ʱ���������߼���
	bool m_bGifPlay;

	//GIF����ͼƬ���ŵ�ȡ������
	nbase::WeakCallbackFlag m_gifWeakFlag;

	//GIF�����¼��Ļص�ע���������(Ŀǰֻ�в������һ���¼�)
	GifEventMap m_OnGifEvent;

	//GIF����ͼƬ��������¼���ID
	static const int32_t m_nVirtualEventGifStop = 1;

	//������Control����
	Control* m_pControl;

	//�ؼ��ı���ͼƬ
	std::shared_ptr<Image> m_pBkImage;
};

} // namespace ui

#endif // UI_CORE_CONTROL_GIF_H_
