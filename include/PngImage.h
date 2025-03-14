#pragma once

#include <string>
#include <string.h>
#include <stdio.h>
#include <png.h>

class PngImage
{
public:
    typedef struct _ImageInfo
    {
        int width;
        int height;
        int bpp;
    } ImageInfo;

    typedef enum
    {
        IMAGE_ERR = -1,
        IMAGE_OK = 0,
    } ImageStatus;

private:
    png_structp _pngPtr;
    png_infop _infoPtr;
    FILE *_file;
    ImageInfo _imageInfo;
    unsigned long _lineBytes; // 一行的字节数
    ImageStatus _status;

public:
    PngImage(std::string path);
    ~PngImage();

    void getImage(unsigned char *buf, unsigned long line);

    const ImageInfo &getInfo() const { return _imageInfo; }
};