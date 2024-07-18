## ���ڵ�����
| ��������          | ���Է���| Ĭ��ֵ  |��������| ����[Window.h](../duilib/Core/Window.h)�еĺ���| ��; |
| :---              | :---    | :---    | :---   |:---                     |:--- |
| caption           | ������  | 0,0,0,0 | rect   | SetCaptionRect          |���ڿ��϶��ı�������С�ı߾�, ���һ��������ָ���ϱ߿�ľ���,��(0,0,0,36) |
| use_system_caption| ������  | false   | bool   | SetUseSystemCaption     |�����Ƿ�ʹ��ϵͳ������ |
| snap_layout_menu  | ������  | true    | bool   | SetEnableSnapLayoutMenu |�Ƿ�֧����ʾ�������ֲ˵���Windows 11�¹��ܣ�ͨ���������ͣ�ڴ��ڵ���󻯰�ť�ϻ� Win + Z���������ɷ��ʶ��벼�֡��� |
| sys_menu          | ������  | true    | bool   | SetEnableSysMenu        |���Ҽ����������ʱ���Ƿ���ʾϵͳ�Ĵ��ڲ˵����ɽ��е�������״̬���رմ��ڵȲ�����|
| sys_menu_rect     | ������  | 0,0,0,0 | rect   | SetSysMenuRect          | ���ڲ˵������ڴ��ڱ����������Ͻǣ�˫����������˳����ڣ������ʾϵͳ�Ĵ��ڲ˵������ù�����XML�������ã�Ĭ���ǹرյ� |
| icon              | ������  |         | string | SetWindowIcon           |���ô��ڵ�ͼ���ļ�·����֧��ico��ʽ |
| text              | ������  |         | string | SetText                 |��������ַ���|
| textid            | ������  |         | string | SetTextId               |��������ַ�����ID, ID�ڶ������ļ���ָ��, ��(STRID_MIANWINDOW_TITLE) |
| shadow_attached   | ������Ӱ| true    | bool   | SetShadowAttached       |�����Ƿ񸽼���ӰЧ��,��(true) |
| shadow_image      | ������Ӱ|         | string | SetShadowImage          |ʹ���Զ������Ӱ�ز�ȥ����Ĭ�ϵ���ӰЧ�������õ�·��Ҫע�����·���Լ��Ź������ԣ���(file='../public/bk/bk_shadow.png' corner='30,30,30,30') |
| shadow_corner     | ������Ӱ| 0,0,0,0 | rect   | SetShadowCorner         |������shadowimage���Ժ����ô�������ָ����Ӱ�زĵľŹ������� |
| size              | ���ڴ�С| 0,0     | size   | SetInitSize             |���ڵĳ�ʼ����С, ֧�ֵĸ�ʽ��size="1200,800", ����size="50%,50%", ����size="1200,50%", size="50%,800"���ٷֱ���ָ��Ļ��Ȼ��߸߶ȵİٷֱ� |
| mininfo           | ���ڴ�С| 0,0     | size   | SetMinInfo              |������С��С, ��(320,240) |
| maxinfo           | ���ڴ�С| 0,0     | size   | SetMaxInfo              |��������С, ��(1600,1200) |
| sizebox           | ���ڴ�С| 0,0,0,0 | rect   | SetSizeBox              |���ڿ��϶��ı䴰�ڴ�С�ı߾�, ��(4,4,6,6) |
| round_corner      | ������״| 0,0     | size   | SetRoundCorner          |����Բ�Ǵ�С, ��(4,4) |
| alpha_fix_corner  | ������״|14,14,14,14| rect | SetAlphaFixCorner       |����Բ�ǵ�͸��ͨ���޲���Χ |
| layered_window    | ���ڻ���| false   | bool   | SetLayeredWindow        |�����Ƿ�Ϊ�㴰�� |
| alpha             | ���ڻ���| 255     | int    | SetLayeredWindowAlpha   |����͸������ֵ[0, 255]���� alpha Ϊ 0 ʱ����������ȫ͸���ġ� �� alpha Ϊ 255 ʱ�������ǲ�͸���ġ�<br>����layered_window="true"ʱ��Ч��<br>�ò�����UpdateLayeredWindow��������Ϊ����ʹ��(BLENDFUNCTION.SourceConstantAlpha)|
| opacity           | ���ڻ���| 255     | int    | SetLayeredWindowOpacity |����͸��������ֵ[0, 255]���� opacity Ϊ 0 ʱ����������ȫ͸���ġ� �� opacity Ϊ 255 ʱ�������ǲ�͸���ġ�<br> ����IsLayeredWindow()Ϊtrue��ʱ����Ч�����������ǰ���Ƿֲ㴰�ڣ��ڲ����Զ�����Ϊ�ֲ㴰�� <br>�ò�����SetLayeredWindowAttributes��������Ϊ����ʹ��(bAlpha)|
| render_backend_type|���ڻ���| "CPU"   | string |SetRenderBackendType     | "CPU": CPU���� <br> "GL": ʹ��OpenGL���� <br> ע������: <br> ��1��һ���߳��ڣ�ֻ������һ������ʹ��OpenGL���ƣ��������ֵ��³������������ <br> ��2��OpenGL���ƵĴ��ڣ������Ƿֲ㴰�ڣ�������WS_EX_LAYERED���ԵĴ��ڣ�<br> ��3��ʹ��OpenGL�Ĵ��ڣ�ÿ�λ��ƶ��ǻ����������ڣ���֧�־ֲ����ƣ����Բ�һ����ʹ��CPU���Ƶ���������ܸ���|

��ע���������ԵĽ��������μ���[WindowBuilder::ParseWindowAttributes����](../duilib/Core/WindowBuilder.cpp)    
��ע��������XML�еı�ǩ�����ǣ�"Window"     
ʹ��ʾ����    
```xml
<Window size="75%,90%" mininfo="80,50" use_system_caption="false" caption="0,0,0,36"
        shadow_attached="true" layered_window="true" alpha="255" sizebox="4,4,4,4">
</Window>
```