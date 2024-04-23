## ���ֵ�����
| ������ | �������� |����ͷ�ļ�| ��; |
| :--- | :--- |:--- | :--- |
| Layout | |[duilib/Box/Layout.h](../duilib/Box/Layout.h) | ���ɲ��֣��������֣�|
| HLayout |Layout |[duilib/Box/HLayout.h](../duilib/Box/HLayout.h) | ���򲼾�|
| VLayout |Layout |[duilib/Box/VLayout.h](../duilib/Box/VLayout.h) | ���򲼾�|
| HTileLayout |Layout| [duilib/Box/HTileLayout.h](../duilib/Box/HTileLayout.h) | ������Ƭ����|
| VTileLayout |Layout| [duilib/Box/VTileLayout.h](../duilib/Box/VTileLayout.h) | ������Ƭ����|
| VirtualHLayout |HLayout| [duilib/Box/VirtualHLayout.h](../duilib/Box/VirtualHLayout.h) | �����򲼾�|
| VirtualVLayout |VLayout| [duilib/Box/VirtualVLayout.h](../duilib/Box/VirtualVLayout.h) | ������򲼾�|
| VirtualHTileLayout |HTileLayout| [duilib/Box/VirtualHTileLayout.h](../duilib/Box/VirtualHTileLayout.h) | ��������Ƭ����|
| VirtualVTileLayout |VTileLayout| [duilib/Box/VirtualVTileLayout.h](../duilib/Box/VirtualVTileLayout.h) | ���������Ƭ����|

1. ���ɲ��֣�Layout���������֣�
| �������� | Ĭ��ֵ | �������� | ��; |
| :--- | :--- | :--- | :--- |
| child_margin | 0 | int | �ӿؼ�֮��Ķ���߾�: X �᷽�� �� Y �᷽��ͬʱ����Ϊ��ֵͬ|
| child_margin_x | 0 | int | �ӿؼ�֮��Ķ���߾�: X �᷽�� |
| child_margin_y | 0 | int | �ӿؼ�֮��Ķ���߾�: Y �᷽��|
2. ˮƽ���֣�HLayout�����������Լ̳����ɲ��ֵ�����
3. ��ֱ���֣�VLayout�����������Լ̳����ɲ��ֵ�����
4. ˮƽ��Ƭ���֣�HTileLayout�����������Լ̳����ɲ��ֵ�����
| �������� | Ĭ��ֵ | �������� | ��; |
| :--- | :--- | :--- | :--- |
| item_size | 0,0 | size | �����С, �ÿ�Ⱥ͸߶ȣ��ǰ����˿ؼ�����߾���ڱ߾�ģ�����"100,40"|
| rows | 0 | int | �������Ϊ"auto"����ʾ�Զ��������������Ϊ��ֵ����ʾ����Ϊ�̶�����|
| scale_down | true | bool | ���ؼ����ݳ����߽�ʱ����������С����ʹ�ؼ�������ȫ��ʾ����Ƭ������|
5. ��ֱ��Ƭ���֣�VTileLayout�����������Լ̳����ɲ��ֵ�����
| �������� | Ĭ��ֵ | �������� | ��; |
| :--- | :--- | :--- | :--- |
| item_size | 0,0 | size | �����С, �ÿ�Ⱥ͸߶ȣ��ǰ����˿ؼ�����߾���ڱ߾�ģ�����"100,40"|
| columns | 0 | int | �������Ϊ"auto"����ʾ�Զ��������������Ϊ��ֵ����ʾ����Ϊ�̶�����|
| scale_down | true | bool | ���ؼ����ݳ����߽�ʱ����������С����ʹ�ؼ�������ȫ��ʾ����Ƭ������|
6. ���ˮƽ���֣�VirtualHLayout�����������Լ̳�ˮƽ���ֵ�����
| �������� | Ĭ��ֵ | �������� | ��; |
| :--- | :--- | :--- | :--- |
| item_size | 0,0 | size | �����С, �ÿ�Ⱥ͸߶ȣ��ǰ����˿ؼ�����߾���ڱ߾�ģ�����"100,40"|
7. ���ֱ���֣�VirtualVLayout�����������Լ̳д�ֱ���ֵ�����
| �������� | Ĭ��ֵ | �������� | ��; |
| :--- | :--- | :--- | :--- |
| item_size | 0,0 | size | �����С, �ÿ�Ⱥ͸߶ȣ��ǰ����˿ؼ�����߾���ڱ߾�ģ�����"100,40"|
8. ���ˮƽ��Ƭ���֣�VirtualHLayout�����������Լ̳�ˮƽ��Ƭ���ֵ�����
9. ���ֱ��Ƭ���֣�VirtualVLayout�����������Լ̳�ˮƽ��Ƭ���ֵ�����

