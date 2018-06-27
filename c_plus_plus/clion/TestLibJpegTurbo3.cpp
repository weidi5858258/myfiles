//
// Created by root on 18-5-18.
//

#include "./include/MyHead.h"

int tjpeg2yuv(unsigned char *jpeg_buffer, int jpeg_size, unsigned char **yuv_buffer, int *yuv_size, int *yuv_type) {
    tjhandle handle = NULL;
    int width, height, subsample, colorspace;
    int flags = 0;
    int padding = 1; // 1或4均可，但不能是0
    int ret = 0;

    handle = tjInitDecompress();
    tjDecompressHeader3(handle, jpeg_buffer, jpeg_size, &width, &height, &subsample, &colorspace);

    printf("w: %d h: %d subsample: %d color: %d\n", width, height, subsample, colorspace);

    flags |= 0;

    *yuv_type = subsample;
    // 注：经测试，指定的yuv采样格式只对YUV缓冲区大小有影响，实际上还是按JPEG本身的YUV格式来转换的
    *yuv_size = tjBufSizeYUV2(width, padding, height, subsample);
    *yuv_buffer = (unsigned char *) malloc(*yuv_size);
    if (*yuv_buffer == NULL) {
        printf("malloc buffer for rgb failed.\n");
        return -1;
    }

    ret = tjDecompressToYUV2(handle, jpeg_buffer, jpeg_size, *yuv_buffer, width,
                             padding, height, flags);
    if (ret < 0) {
        printf("compress to jpeg failed: %s\n", tjGetErrorStr());
    }
    tjDestroy(handle);

    return ret;
}

int
tyuv2jpeg(unsigned char *yuv_buffer, int yuv_size, int width, int height, int subsample, unsigned char **jpeg_buffer,
          unsigned long *jpeg_size, int quality) {
    tjhandle handle = NULL;
    int flags = 0;
    int padding = 1; // 1或4均可，但不能是0
    int need_size = 0;
    int ret = 0;

    handle = tjInitCompress();

    flags |= 0;

    need_size = tjBufSizeYUV2(width, padding, height, subsample);
    if (need_size != yuv_size) {
        printf("we detect yuv size: %d, but you give: %d, check again.\n", need_size, yuv_size);
        return 0;
    }

    ret = tjCompressFromYUV(handle, yuv_buffer, width, padding, height, subsample, jpeg_buffer, jpeg_size, quality,
                            flags);
    if (ret < 0) {
        printf("compress to jpeg failed: %s\n", tjGetErrorStr());
    }

    tjDestroy(handle);

    return ret;
}

int
trgb2yuv(unsigned char *rgb_buffer, int width, int height, unsigned char **yuv_buffer, int *yuv_size, int subsample) {
    tjhandle handle = NULL;
    int flags = 0;
    int padding = 1; // 1或4均可，但不能是0
    int pixelfmt = TJPF_RGB;
    int ret = 0;

    handle = tjInitCompress();

    flags |= 0;

    *yuv_size = tjBufSizeYUV2(width, padding, height, subsample);

    *yuv_buffer = (unsigned char *) malloc(*yuv_size);
    if (*yuv_buffer == NULL) {
        printf("malloc buffer for rgb failed.\n");
        return -1;
    }
    ret = tjEncodeYUV3(handle, rgb_buffer, width, 0, height, pixelfmt, *yuv_buffer, padding, subsample, flags);
    if (ret < 0) {
        printf("encode to yuv failed: %s\n", tjGetErrorStr());
    }

    tjDestroy(handle);

    return ret;
}

int tyuv2rgb(unsigned char *yuv_buffer, int yuv_size, int width, int height, int subsample, unsigned char **rgb_buffer,
             int *rgb_size) {
    tjhandle handle = NULL;
    int flags = 0;
    int padding = 1; // 1或4均可，但不能是0
    int pixelfmt = TJPF_RGB;
    int need_size = 0;
    int ret = 0;

    handle = tjInitDecompress();

    flags |= 0;

    need_size = tjBufSizeYUV2(width, padding, height, subsample);
    if (need_size != yuv_size) {
        printf("we detect yuv size: %d, but you give: %d, check again.\n", need_size, yuv_size);
        return -1;
    }

    *rgb_size = width * height * tjPixelSize[pixelfmt];

    *rgb_buffer = (unsigned char *) malloc(*rgb_size);
    if (*rgb_buffer == NULL) {
        printf("malloc buffer for rgb failed.\n");
        return -1;
    }
    ret = tjDecodeYUV(handle, yuv_buffer, padding, subsample, *rgb_buffer, width, 0, height, pixelfmt, flags);
    if (ret < 0) {
        printf("decode to rgb failed: %s\n", tjGetErrorStr());
    }

    tjDestroy(handle);

    return ret;
}