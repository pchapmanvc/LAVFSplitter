#!/bin/bash 

if [ ! -d bin_Win32/lib ]
then
mkdir -p bin_Win32/lib
fi

if [ ! -d bin_Win32d/lib ]
then
mkdir -p bin_Win32d/lib
fi

cd ffmpeg

make distclean

OPTIONS="
--enable-shared \
--enable-gpl \
--enable-version2 \
--enable-w32threads \
--enable-runtime-cpudetect \
--enable-asm \
--disable-postproc \
--enable-zlib \
--enable-swscale \
--disable-swresample \
--disable-static \
--disable-altivec \
--disable-muxers \
--disable-encoders \
--disable-decoders \
--disable-demuxers \
--enable-decoder=rawvideo \
--enable-decoder=dvvideo \
--enable-decoder=mpeg2video \
--enable-decoder=aac \
--enable-decoder=aac_latm \
--enable-decoder=pcm_alaw \
--enable-decoder=pcm_dvd \
--enable-decoder=pcm_f32be \
--enable-decoder=pcm_f32le \
--enable-decoder=pcm_f64be \
--enable-decoder=pcm_f64le \
--enable-decoder=pcm_lxf \
--enable-decoder=pcm_mulaw \
--enable-decoder=pcm_s16be \
--enable-decoder=pcm_s16le \
--enable-decoder=pcm_s16le_planar \
--enable-decoder=pcm_s24be \
--enable-decoder=pcm_s24daud \
--enable-decoder=pcm_s24le \
--enable-decoder=pcm_s32be \
--enable-decoder=pcm_s32le \
--enable-decoder=pcm_u16be \
--enable-decoder=pcm_u16le \
--enable-decoder=pcm_u24be \
--enable-decoder=pcm_u24le \
--enable-decoder=pcm_u32be \
--enable-decoder=pcm_u32le \
--enable-decoder=h264 \
--enable-demuxer=gxf \
--enable-demuxer=lxf \
--enable-demuxer=matroska \
--disable-debug \
--disable-ffplay \
--disable-ffserver \
--disable-ffmpeg \
--disable-avconv \
--disable-ffprobe \
--disable-devices \
--disable-filters \
--disable-avfilter \
--disable-avdevice \
--disable-hwaccels \
--disable-bsfs \
--disable-network \
--disable-protocols \
--enable-protocol=file \
--enable-muxer=spdif \
--arch=x86 --cpu=i686 --target-os=mingw32 \
--build-suffix=-vclav"

./configure --extra-libs="-lwsock32" --extra-cflags="-mmmx -msse -DPTW32_STATIC_LIB" ${OPTIONS} &&
 
make -j8 &&
cp lib*/*-vclav-*.dll ../bin_Win32 &&
cp lib*/*.lib ../bin_Win32/lib &&
cp lib*/*-vclav-*.dll ../bin_Win32d &&
cp lib*/*.lib ../bin_Win32d/lib &&

cd ..
