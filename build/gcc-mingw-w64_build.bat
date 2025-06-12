@echo OFF
@REM Windows 平台编译脚本，编译器：MinGW-w64
@REM 使用时确保，MinGW-w64的编译环境已经添加到PATH环境变量，示例：
@REM 假设MinGW-w64的二进制文件所在目录为： C:\mingw64\x86_64-15.1.0-release-win32-seh-ucrt-rt_v12-rev0\mingw64\bin\
@REM 使用如下命令将MinGW-w64编译环境添加到PATH变量：
@REM #SET PATH=%PATH%;C:\mingw64\x86_64-15.1.0-release-win32-seh-ucrt-rt_v12-rev0\mingw64\bin\
@REM 可使用g++ -v 查看版本信息

@REM 确保本机已经安装了camke(最低版本号: v3.21)
@REM 可使用 cmake --version 查看版本信息（本机: cmake version 4.0.2）

@for %%i in ("%~dp0..\") do set "bat_parent_dir=%%~fi"
SET DUILIB_SRC_ROOT_DIR=%bat_parent_dir%
echo DUILIB_SRC_ROOT_DIR: "%DUILIB_SRC_ROOT_DIR%"

@for %%i in ("%~dp0..\..\skia") do set "bat_parent_dir=%%~fi"
SET SKIA_SRC_ROOT_DIR=%bat_parent_dir%
echo SKIA_SRC_ROOT_DIR: "%SKIA_SRC_ROOT_DIR%"

echo Checking the necessary software
where g++.exe >nul 2>&1
if %errorlevel% equ 0 (
    echo g++ found at:  
    where g++
) else (
    echo g++ not found in PATH
    exit /b 1
)

where gcc.exe >nul 2>&1
if %errorlevel% equ 0 (
    echo gcc.exe found at:  
    where gcc.exe
) else (
    echo gcc.exe not found in PATH
    exit /b 1
)

@REM # 设置编译器
SET DUILIB_CC=gcc
SET DUILIB_CXX=g++
SET DUILIB_COMPILER_ID=gcc-mingw-w64

SET DUILIB_CMAKE=cmake -G"MinGW Makefiles" -DCMAKE_C_COMPILER=%DUILIB_CC% -DCMAKE_CXX_COMPILER=%DUILIB_CXX%
SET DUILIB_MAKE=cmake --build
SET DUILIB_BUILD_TYPE=Release
SET DUILIB_MAKE_THREADS=-j 6

SET DUILIB_SKIA_LIB_SUBPATH=gcc.x64.release
echo "DUILIB_SKIA_LIB_SUBPATH:%DUILIB_SKIA_LIB_SUBPATH%"

if not exist "%SKIA_SRC_ROOT_DIR%\out\%DUILIB_SKIA_LIB_SUBPATH%" (
    echo "Please compile the skia first or run build_duilib_all_in_one_mingw-w64.bat."
    exit /b 1
)

@REM # 编译临时目录
SET DUILIB_BUILD_DIR=%DUILIB_SRC_ROOT_DIR%\build\build_temp\%DUILIB_COMPILER_ID%
SET target_dir=%DUILIB_SRC_ROOT_DIR%\build\build_temp\
if not exist "%target_dir%" (
    @mkdir "%target_dir%"
)
if not exist "%DUILIB_BUILD_DIR%" (
    @mkdir "%DUILIB_BUILD_DIR%"
)

@REM # 编译第三方库   
SET DUILIB_THIRD_PARTY_LIBS=zlib,libpng,cximage,libwebp
for %%i in (%DUILIB_THIRD_PARTY_LIBS%) do (
    %DUILIB_CMAKE% -S "%DUILIB_SRC_ROOT_DIR%duilib\third_party\%%i" -B "%DUILIB_BUILD_DIR%\%%i" -DCMAKE_BUILD_TYPE=%DUILIB_BUILD_TYPE%
    %DUILIB_MAKE% "%DUILIB_BUILD_DIR%\%%i" %DUILIB_MAKE_THREADS%
)

@REM #编译duilib
%DUILIB_CMAKE% -S "%DUILIB_SRC_ROOT_DIR%duilib" -B "%DUILIB_BUILD_DIR%\duilib" -DCMAKE_BUILD_TYPE=%DUILIB_BUILD_TYPE%
%DUILIB_MAKE% "%DUILIB_BUILD_DIR%\duilib" %DUILIB_MAKE_THREADS%

@REM #编译examples下的各个程序
SET DUILIB_PROGRAMS=basic,controls,ColorPicker,DpiAware,layouts,ListBox,ListCtrl,MoveControl,MultiLang,render,RichEdit,VirtualListBox,threads,TreeView
for %%i in (%DUILIB_PROGRAMS%) do (
    %DUILIB_CMAKE% -S "%DUILIB_SRC_ROOT_DIR%examples\%%i" -B "%DUILIB_BUILD_DIR%\%%i" -DCMAKE_BUILD_TYPE=%DUILIB_BUILD_TYPE%
    %DUILIB_MAKE% "%DUILIB_BUILD_DIR%\%%i" %DUILIB_MAKE_THREADS%
)

@REM # 目前不支持CEF，所以不包含以下程序：cef,CefBrowser
