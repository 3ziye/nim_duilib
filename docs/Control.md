## Control�������ؼ���������

`Control` �ؼ�Ϊ���п��ÿؼ��Ļ��࣬�����˿ؼ����õ����ԣ����ȡ��߶ȡ��߾�����ԡ�һ������²���Ϊ������ÿؼ�ʹ�ã���������һЩռλ������ο�ʾ����
`Control` �����пؼ��Ļ��࣬���������пؼ��Ŀ���ͨ�����ԡ�

```xml
<!-- һ�����ݸ���������Զ������ Control �ؼ���һ��������Ϊռλ��ʹ�� -->
<Control width="stretch"/>
```

| �������� | Ĭ��ֵ | �������� |����[Control.h](../duilib/Core/Control.h)�еĺ���| ��; |
| :--- | :--- | :--- | :---| :--- |
| class |  | string | SetClass|�ؼ���ʽ,����ʽ���������ֵ�����ñ��ؼ�������,��(btn_default),����ͬʱָ�������ʽ,������ʽ֮���ÿո�ָ�,��Ҫд�ڵ�һ������λ�� |
| name |  | string | SetName|�ؼ�����,ͬһ�����ڱ���Ψһ,��(testbtn) |
| width | stretch | int / string | SetFixedWidth|��������int��string���͵�ֵ.��ֵΪint�������ÿؼ��Ŀ��ֵ,��(100);��ֵΪstringʱ,stretch�����ɸ���������ؼ����,auto���������������Զ�������,���Ϊ�ٷֱ�ֵ"50%"������ÿؼ��Ŀ������ֵΪ���ؼ���ȵ�50% |
| height | stretch | int / string | SetFixedHeight|��������int��string���͵�ֵ.��ֵΪint�������ÿؼ��ĸ߶�ֵ,��(100);��ֵΪstringʱ,stretch�����ɸ���������ؼ��߶�,auto���������������Զ�����߶�,���Ϊ�ٷֱ�ֵ"30%"������ÿؼ��Ŀ������ֵΪ���ؼ��߶ȵ�30%  |
| min_width | -1 | int | SetMinWidth|�ؼ�����С���,��(30) |
| min_height | -1 | int | SetMinHeight|�ؼ�����С�߶�,��(30) |
| max_width | INT32_MAX | int / string | SetMaxWidth|�ؼ��������,����ͬwidth |
| max_height | INT32_MAX | int / string | SetMaxHeight|�ؼ������߶�,����ͬheight |
| margin | 0,0,0,0 | rect | SetMargin|��߾�,��(2,2,2,2) |
| padding | 0,0,0,0 | rect | SetPadding|�ڱ߾�,��(2,2,2,2) |
| control_padding | true | bool | SetEnableControlPadding|�Ƿ�����ؼ����������ڱ߾� |
| halign | left | string | SetHorAlignType|�ؼ��ĺ���λ��,��(center),֧��left��center��right����λ�� |
| valign | top | string | SetVerAlignType|�ؼ�������λ��,��(center),֧��top��center��bottom����λ�� |
| float | false | bool | SetFloat|�Ƿ�ʹ�þ��Զ�λ,��(true) |
| bkcolor |  | string | SetBkColor|������ɫ�ַ�������,��(white) |
| normal_color |  | string | SetStateColor|��ͨ״̬��ɫ,��(white) |
| hot_color |  | string | SetStateColor|����״̬��ɫ,��(white) |
| pushed_color |  | string | SetStateColor|����״̬��ɫ,��(white) |
| disabled_color |  | string | SetStateColor|����״̬��ɫ,��(white) |
| border_color |  | string | SetBorderColor|��������״̬�µı߿���ɫ,��(blue) |
| normal_border_color |  | string | SetBorderColor|����״̬�ı߿���ɫ,��(blue) |
| hot_border_color |  | string | SetBorderColor|����״̬�ı߿���ɫ,��(blue) |
| pushed_border_color |  | string | SetBorderColor|����״̬�ı߿���ɫ,��(blue) |
| disabled_border_color |  | string | SetBorderColor|��ֹ״̬�ı߿���ɫ,��(blue) |
| focus_border_color |  | string | SetFocusBorderColor|����״̬�ı߿���ɫ,��(blue) |
| border_size | 0 | int / rect | SetBorderSize|��������int��rect���͵�ֵ����ֵΪintʱ�����ϡ��ҡ��¶��ø�ֵ��Ϊ��ֵΪrect����ʱ��ֱ��������ϡ��ҡ��µı߿� |
| left_border_size | 0 | int | SetLeftBorderSize|��߱߿��С,��(1),���ø�ֵ����0,�򽫺���border_size���Ե����� |
| top_border_size | 0 | int | SetTopBorderSize|�����߿��С,��(1),���ø�ֵ����0,�򽫺���border_size���Ե����� |
| right_border_size | 0 | int | SetRightBorderSize|�ұ߱߿��С,��(1),���ø�ֵ����0,�򽫺���border_size���Ե����� |
| bottom_border_size | 0 | int | SetBottomBorderSize|�ײ��߿��С,��(1),���ø�ֵ����0,�򽫺���border_size���Ե����� |
| border_round | 0,0 | size | SetBorderRound|�߿�Բ�ǰ뾶,��(2,2) |
| bkimage |  | string | SetBkImage|����ͼƬ,��(bk.bmp��file='aaa.jpg' res='' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' fade='255' xtiled='false' ytiled='false') |
| normal_image |  | string | SetStateImage|��ͨ״̬ͼƬ |
| hot_image |  | string | SetStateImage|���������״̬ͼƬ |
| pushed_image |  | string | SetStateImage|��갴�µ�״̬ͼƬ |
| disabled_image |  | string | SetStateImage|���õ�״̬ͼƬ |
| fore_normal_image |  | string | SetForeStateImage|��ͨ״̬ǰ��ͼƬ |
| fore_hot_image |  | string | SetForeStateImage |���������״̬ǰ��ͼƬ |
| fore_pushed_image |  | string | SetForeStateImage|��갴�µ�״̬ǰ��ͼƬ |
| fore_disabled_image |  | string | SetForeStateImage|���õ�״̬ǰ��ͼƬ |
| tooltip_text |  | string | SetToolTipText|���������ʾ,��(�������������������) |
| tooltip_textid |  | string | SetToolTipTextId|���������ʾ,ָ��������ģ���ID,��tooltiptextΪ��ʱ����ʾ������,��(TOOL_TIP_ID) |
| dataid |  | string | SetDataID|�Զ����ַ�������,�������������û�ʹ�� |
| user_dataid |  | size_t | SetUserDataID|�Զ�����������, ���û�ʹ�� |
| enabled | true | bool | SetEnabled|�Ƿ������Ӧ�û�����,��(true) |
| mouse_enabled | true | bool | SetMouseEnabled|���ؼ��Ƿ������Ӧ������,��(true) |
| keyboard_enabled | true | bool | SetKeyboardEnabled|��CButtonUI����Ը�ֵ,Ϊfalseʱ��֧��TAB_STOP,�Ҹö��󲻴��������Ϣ |
| receivepointer | true | bool | |�Ƿ���Ӧ������Ϣ,��(true) |
| visible | true | bool | SetVisible|�Ƿ�ɼ�,��(true) |
| fade_visible | true | bool | SetFadeVisible|�Ƿ�ɼ�,��(true),�����Իᴥ���ؼ�����Ч�� |
| menu | false | bool | |�Ƿ���Ҫ�Ҽ��˵�,��(true) |
| nofocus | false | bool | SetNoFocus|�Ƿ���Ի�ȡ����,��(true) |
| tab_stop | true | bool | SetTabStop| �Ƿ�����ͨ����TAB���л����˿ؼ� |
| show_focus_rect | false| bool | SetLoadingBkColor| �Ƿ���ʾ����״̬(һ�����߹��ɵľ���) |
| focus_rect_color | | string | SetFocusRectColor| ����״̬���ε���ɫ |
| alpha | 255 | int | SetAlpha|�ؼ�������͸����,��(128)����ЧֵΪ 0-255 |
| state | normal | string | SetState|�ؼ��ĵ�ǰ״̬: ֧��normal��hot��pushed��disabled״̬ |
| cursortype | arrow | string | SetCursorType|����ƶ����ؼ���ʱ�������: arrow(��ͷ)��hand(����)��ibeam(��I����״)��sizewe(�����϶�)��sizens(�����϶�)|
| render_offset | 0,0 | size | SetRenderOffset|�ؼ�����ʱ��ƫ����,��(10,10),һ�����ڻ��ƶ��� |
| fade_alpha | false | bool | GetAnimationManager(). SetFadeAlpha|�Ƿ����ÿؼ�͸�����䶯��,��(true) |
| fade_hot | false | bool |GetAnimationManager(). SetFadeHot |�Ƿ����ÿؼ�����״̬�� ��͸�����䶯��,��(true) |
| fade_width | false | bool | GetAnimationManager(). SetFadeWidth|�Ƿ����ÿؼ���Ƚ��䶯��,��(true) |
| fade_height | false | bool | GetAnimationManager(). SetFadeHeight|�Ƿ����ÿؼ��߶Ƚ��䶯��,��(true) |
| fade_in_out_x_from_left | false | bool | GetAnimationManager(). SetFadeInOutX|�Ƿ����ÿؼ������ҵĶ���,��(true) |
| fade_in_out_x_from_right | false | bool | GetAnimationManager(). SetFadeInOutX|�Ƿ����ÿؼ����ҵ���Ķ���,��(true) | 
| fade_in_out_y_from_top | false | bool | GetAnimationManager().  SetFadeInOutY|�Ƿ����ÿؼ����ϵ��µĶ���,��(true) | 
| fade_in_out_y_from_bottom | false | bool | GetAnimationManager().  SetFadeInOutY|�Ƿ����ÿؼ����µ��ϵĶ���,��(true) |
| loading_image | | string | SetLoadingImage| ������״̬��ͼƬ |
| loading_bkcolor | | string | SetLoadingBkColor| ������״̬�ı���ɫ |
| paint_order | | string | SetPaintOrder| ���û���˳��0 ��ʾ������ƣ���0��ʾָ������˳��ֵԽ���ʾԽ����� |
| start_gif_play | | int | StartGifPlay| ���Ŷ�����������ʾ����ѭ������ |
| stop_gif_play | | int | StopGifPlay| ֹͣ������������ʾֹͣ����һ֡ |
| box_shadow | | string | SetBoxShadow|���ÿؼ�����Ӱ���ԣ�������boxshadow="color='red' offset='0,0' blurradius='8' spreadradius='8' |
| cache | false | bool |SetUseCache |�Ƿ����ÿؼ����ƻ���,��(true) |