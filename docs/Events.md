## �ؼ����¼���EventArgs��˵���ĵ�

| �¼�(eventType)           | ����(wParam)  | ����(lParam) | ����(ptMouse) | ����(vkCode) | ����(modifierKey) |����(eventData) | ��ע     |
| :---                      | :---          | :---         |:---           |:---          |:---               |:---            |:---      |
|kEventKeyDown              |     |     |     |   �����İ���  |  ������־  |     |     |
|kEventKeyUp                |     |     |     |   �����İ���  |  ������־  |     |     |
|kEventChar                 |     |     |     |   �����İ���  |  ������־  |     |     |
|kEventMouseEnter           |     |     |�������λ��<br>�Ŀͻ�������   |     | ������־   |     |     |
|kEventMouseLeave           |     |     |�������λ��<br>�Ŀͻ�������   |     | ������־   |     |     |
|kEventMouseMove            |     |     |�������λ��<br>�Ŀͻ�������   |     | ������־   |     |     |
|kEventMouseHover           |     |     |�������λ��<br>�Ŀͻ�������   |     | ������־   |     |     |
|kEventMouseWheel           |     |     |�������λ��<br>�Ŀͻ�������   |     | ������־   |  wheelDelta���� |     |
|kEventMouseButtonDown      |     |     |�������λ��<br>�Ŀͻ�������   |     | ������־   |     |     |
|kEventMouseButtonUp        |     |     |�������λ��<br>�Ŀͻ�������   |     | ������־   |     |     |
|kEventMouseDoubleClick     |     |     |�������λ��<br>�Ŀͻ�������   |     | ������־   |     |     |
|kEventMouseRButtonDown     |     |     |�������λ��<br>�Ŀͻ�������   |     | ������־   |     |     |
|kEventMouseRButtonUp       |     |     |�������λ��<br>�Ŀͻ�������   |     | ������־   |     |     |
|kEventMouseRDoubleClick    |     |     |�������λ��<br>�Ŀͻ�������   |     | ������־   |     |     |
|kEventContextMenu          |     |Control*<br> �������λ�õĿؼ�| �������λ��<br>�Ŀͻ�������    |     |       |     | ����û����� SHIFT+F10��<br> ��ptMouseΪ(-1,-1)��lParamΪ0 |
|kEventClick                |     |     |�������λ��<br>�Ŀͻ�������   | ����(vkCode)   | ������־   |     | ���������Խ϶࣬��Ҫ���  |
|kEventRClick               |     |     |�������λ��<br>�Ŀͻ�������   |     | ������־   |     |     |
|kEventMouseClickChanged    |     |     |     |     |       |     | �޲��� |
|kEventMouseClickEsc        |     |     |     |     |       |     | �޲��� |

| �¼�(eventType)           | ����(wParam)  | ����(lParam) | ����(ptMouse) | ����(vkCode) | ����(modifierKey) |����(eventData) | ��ע     |
| :---                      | :---          | :---         |:---           |:---          |:---               |:---            |:---      |
|kEventSetFocus             |     |     |     |     |       |     | �޲��� |
|kEventKillFocus            |     |Control*<br> �µĽ���ؼ�<br>����Ϊnullptr |     |     |       |     |     |
|kEventSetCursor            |     |     | ����<br>�ͻ�������   |     |       |     |   |
|kEventImeStartComposition  |     |     |     |     |       |     |�޲���|
|kEventImeEndComposition    |     |     |     |     |       |     |�޲���|
|kEventWindowKillFocus      |     |     |     |     |       |     |�޲��� |
|kEventWindowSize           |     |     |     |     |       |���ڴ�С�仯���ͣ�<br>WindowSizeType|     |
|kEventWindowMove           |     |     | ���ڵ�<br>���Ͻ�����    |     |       |     |     |
|kEventWindowClose          |0: �����ر� <br> 1: ȡ���ر�|     |     |     |       |     |     |
|kEventSelect               |ListBox/Combo: <br>��ѡ��������� | ListBox/Combo: <br>��ѡ����������|     |     |       |     | �������޲��� |
|kEventUnSelect             |ListBox: <br>��ѡ��������� | ListBox��<br>��ѡ����������|     |     |       |     |  �������޲���   |
|kEventChecked              |     |     |     |     |       |     | �޲��� |
|kEventUnCheck              |     |     |     |     |       |     | �޲��� |
|kEventTabSelect            |��ѡ��������� | ��ѡ����������|     |     |    |     |    |
|kEventExpand               |     |     |     |     |       |     | �޲��� |
|kEventCollapse             |     |     |     |     |       |     | �޲��� |


| �¼�(eventType)           | ����(wParam)  | ����(lParam) | ����(ptMouse) | ����(vkCode) | ����(modifierKey) |����(eventData) | ��ע     |
| :---                      | :---          | :---         |:---           |:---          |:---               |:---            |:---      |
|kEventZoom                 | �����ű�������[0,64] | �����ű�����ĸ(0,64] |     |     |       |     | RichEdit: Ctrl + ���֣����Ź���|
|kEventTextChange           |     |     |     |     |       |     | �޲��� |
|kEventSelChange            |     |     |     |     |       |     | �޲��� |
|kEventReturn               |     |     |     |     |       |     | �޲���    |
|kEventTab                  |     |     |     |     |       |     | �޲���   |
|kEventLinkClick            | DString.c_str()<br> URL�ַ���    |     |     |     |       |     |     |
|kEventScrollChange         | 0: cy�ޱ仯<br> 1: cy�б仯   | 0: cx�ޱ仯<br> 1: cx�б仯    |     |     |       |     |     |
|kEventValueChange          |     |     |     |     |       |     | �޲��� |
|kEventResize               |     |     |     |     |       |     | �޲��� |
|kEventVisibleChange        |     |     |     |     |       |     | �޲��� |
|kEventStateChange          | ��״̬ | ��״̬   |     |     |       |     | ControlStateType |
|kEventSelectColor          | ��ѡ��ɫ |     |     |     |       |     | newColor.GetARGB() |
|kEventSplitDraged          | Control*: <br>��1���ؼ��ӿ�| Control*:<br>��2���ؼ��ӿ�|     |     |       |     |  ������nullptr  |
|kEventEnterEdit            | ListCtrlEditParam*:<br>����༭״̬������  |     |     |     |       |     |     |
|kEventLeaveEdit            | ListCtrlEditParam*:<br>�뿪�༭״̬������    |     |     |     |       |     |     |
|kEventLast                 |     |     |     |     |       |     |  �޲���   |
