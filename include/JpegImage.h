#pragma once

#include <string>
#include <string.h>
#include <stdio.h>
#include <jpeglib.h>

class JpegImage
{
public:
    typedef enum
    {
        IMAGE_ERR = -1,
        IMAGE_OK = 0,
    } ImageStatus;

private:
    struct jpeg_decompress_struct _info;
    struct jpeg_error_mgr _err;
    FILE *_file;
    unsigned long _lineBytes; // 一行的字节数
    ImageStatus _status;

public:
    JpegImage(std::string path);
    ~JpegImage();

    void getImage(unsigned char *buf, unsigned long line);

    const jpeg_decompress_struct &getInfo() const { return _info; }
};
