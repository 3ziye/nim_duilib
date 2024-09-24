#!/bin/sh

SRC_ROOT_DIR=$(realpath $(dirname "$0"))

# ����zlib
cmake -S "$SRC_ROOT_DIR/duilib/third_party/zlib/" -B "$SRC_ROOT_DIR/build_temp/zlib" -DZLIB_BUILD_EXAMPLES=OFF
cd "$SRC_ROOT_DIR/build_temp/zlib"
make 
cd "$SRC_ROOT_DIR/"

rm -f "$SRC_ROOT_DIR/libs/libz.a"
cp "$SRC_ROOT_DIR/build_temp/zlib/libz.a" "$SRC_ROOT_DIR/libs/libz.a"

#�ָ�zconf.h�ļ�
mv "$SRC_ROOT_DIR/duilib/third_party/zlib/zconf.h.included" "$SRC_ROOT_DIR/duilib/third_party/zlib/zconf.h"

#����libpng
cmake -S "$SRC_ROOT_DIR/duilib/third_party/libpng/" -B "$SRC_ROOT_DIR/build_temp/libpng" -DPNG_TESTS=OFF -DPNG_SHARED=OFF -DPNG_TOOLS=OFF -DZLIB_ROOT="$SRC_ROOT_DIR/duilib/third_party/zlib/"
cd "$SRC_ROOT_DIR/build_temp/libpng"
make 
cd "$SRC_ROOT_DIR/"

rm -f "$SRC_ROOT_DIR/libs/libpng.a"
cp "$SRC_ROOT_DIR/build_temp/libpng/libpng16.a" "$SRC_ROOT_DIR/libs/libpng.a"

#����cximage
cmake -S "$SRC_ROOT_DIR/duilib/third_party/cximage/" -B "$SRC_ROOT_DIR/build_temp/cximage"
cd "$SRC_ROOT_DIR/build_temp/cximage"
make 
cd "$SRC_ROOT_DIR/"

rm -f "$SRC_ROOT_DIR/libs/libcximage.a"
cp "$SRC_ROOT_DIR/build_temp/cximage/libcximage.a" "$SRC_ROOT_DIR/libs/libcximage.a"

#����libwebp
cmake -S "$SRC_ROOT_DIR/duilib/third_party/libwebp/" -B "$SRC_ROOT_DIR/build_temp/libwebp"
cd "$SRC_ROOT_DIR/build_temp/libwebp"
make 
cd "$SRC_ROOT_DIR/"

rm -f "$SRC_ROOT_DIR/libs/libwebp.a"
cp "$SRC_ROOT_DIR/build_temp/libwebp/libwebp.a" "$SRC_ROOT_DIR/libs/libwebp.a"

#����duilib
cmake -S "$SRC_ROOT_DIR/duilib/" -B "$SRC_ROOT_DIR/build_temp/duilib"
cd "$SRC_ROOT_DIR/build_temp/duilib"
make 
cd "$SRC_ROOT_DIR/"

rm -f "$SRC_ROOT_DIR/libs/libduilib.a"
cp "$SRC_ROOT_DIR/build_temp/duilib/libduilib.a" "$SRC_ROOT_DIR/libs/libduilib.a"

#������ʱĿ¼
rm -rf "$SRC_ROOT_DIR/build_temp/"

