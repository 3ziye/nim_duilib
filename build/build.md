# ��Ŀ��������ĵ��ͽű�
��Ŀ¼����Ҫ��������Ŀ���������ĵ��������Ľű��ļ���������Ŀ�ĸ�Ŀ¼Ϊ����${DUILIB_ROOT}��
## ��Ŀ¼�е��ļ��б�
| FileName/�ļ���               | OS/����ϵͳ   | Compiler/������        |  Comment/��ע   |
| :---                          | :---          |:---                    |:---             |
| `build_duilib_all_in_one.bat` | Windows       |Visual Studio 2022      |һ������ű����ű��������Դ�����أ���skia��������ͱ���<br>Ĭ�ϱ������ݣ�<br>skia: llvm.x64.debug/llvm.x64.release/llvm.x86.release/llvm.x86.debug|
| `duilib.sln`                  | Windows       |Visual Studio 2022      |duilib��Visual Studio�Ľ�������ļ���ֻ����lib��������ʾ������|
| `examples.sln`                | Windows       |Visual Studio 2022      |ʾ�������Visual Studio�Ľ�������ļ�������lib������ʾ������|
| `msvc_build.bat`              | Windows       |cmake/MSVC              |�����б���ű���ʹ��cmake���룬������ΪMSVC|
| `gcc-mingw-w64_build.bat`     | Windows       |MinGW-w64 gcc/g++       |MinGW-w64�ı���ű���������Ϊgcc/g++|
| `llvm-mingw-w64_build.bat`    | Windows       |MinGW-w64 clang/clang++ |MinGW-w64�ı���ű���������Ϊclang/clang++|
| `build_duilib_all_in_one.sh`  | Windows       |MSYS2 gcc/g++ or clang/clang++|һ������ű����ű��������Դ�����أ���skia��������ͱ��룬������Ϊgcc/g++ ���� clang/clang++|
| `msys2_build.sh`              | Windows       |MSYS2 gcc/g++ or clang/clang++|MSYS2ϵͳ�µı���ű���������Ϊgcc/g++ ���� clang/clang++|
| `build_duilib_all_in_one.sh`  | Linux         |gcc/g++ or clang/clang++|һ������ű����ű��������Դ�����أ���skia��������ͱ��룬������Ϊgcc/g++ ���� clang/clang++|
| `linux_build.sh`              | Linux         |gcc/g++ or clang/clang++|Linuxϵͳ�µı���ű���������Ϊgcc/g++ ���� clang/clang++|
| `build_duilib_all_in_one.sh`  | MacOS         |clang/clang++           |һ������ű����ű��������Դ�����أ���skia��������ͱ��룬������Ϊgcc/g++ ���� clang/clang++|
| `macos_build.sh`              | MacOS         |clang/clang++           |MacOSϵͳ�µı���ű���������Ϊclang/clang++|
| `build_duilib_all_in_one.sh`  | FreeBSD       |gcc/g++ or clang/clang++|һ������ű����ű��������Դ�����أ���skia��������ͱ��룬������Ϊgcc/g++ ���� clang/clang++|
| `freebsd_build.sh`            | FreeBSD       |gcc/g++ or clang/clang++|FreeBSDϵͳ�µı���ű���������Ϊgcc/g++ ���� clang/clang++|

## �����Ŀ���ļ�
1. lib�ļ���`${DUILIB_ROOT}/lib`
2. bin�ļ���`${DUILIB_ROOT}/bin`
3. ��ʱ�ļ���������    
��1��`${DUILIB_ROOT}/build/build_temp`: �������ʱĿ¼��������    
��2��`${DUILIB_ROOT}/build/.vs`: ���ص�Ŀ¼��Visual Studio ���ɵĻ���Ŀ¼��ռ�пռ�ܴ���Խ��Խ�󣬿ɶ�������    
��3��`${DUILIB_ROOT}/cef_temp`: libCEFģ������ʱ�����绺��Ŀ¼����ɾ����
    
## ���򷢲�ʱ���������ļ�
1. `${DUILIB_ROOT}/bin/resources`Ŀ¼���������Դ�ļ���XML�ļ���ͼƬ��Դ�ȣ�    
��1��`${DUILIB_ROOT}/bin/resources/fonts`������Ŀ¼�����δʹ���Զ������壬��Ŀ¼��ɾ����    
��2��`${DUILIB_ROOT}/bin/resources/lang`�������԰�������ļ������δʹ�ö���������ݣ���Ŀ¼��ɾ����    
��3��`${DUILIB_ROOT}/bin/resources/themes/default`�������������ԴĿ¼��ֻ����`public`Ŀ¼��`global.xml`�ļ�������Ŀ¼����ɾ����    
2. Windowsƽ̨�������Ҫ����Դ�����zip������exe����Դ�У�    
   ���Խ���1������õ�`bin/resources`Ŀ¼�����Ϊ`resources.zip`�ļ����滻����`bin/resources.zip`�ļ��������±���exe���ɡ�    
   ����ʹ��`resources.zip`�ķ��������Բο�`examples/basic`ʾ������Ĺ��̴��롣    
3. ���ʹ����CEFģ�飺��Ҫ��CEF��ReleaseĿ¼��ResourcesĿ¼������ļ��ŵ�`bin\libcef_win`����`bin\libcef_win_109`Ŀ¼��    
   ��ϸ���ݿɲο�CEF��ʹ��˵���ĵ���[docs/CEF.md](../docs/CEF.md)��
