#pragma once

#include <string>
#include <string.h>

class BitImage
{
public:
    typedef struct
    {
        unsigned char type[2];    // 文件类型
        unsigned int size;        // 文件大小
        unsigned short reserved1; // 保留字段 1
        unsigned short reserved2; // 保留字段 2
        unsigned int offset;      // 到位图数据的偏移量
    } __attribute__((packed)) BmpFileHeader;

    /**** 位图信息头数据结构 ****/
    typedef struct
    {
        unsigned int size;        // 位图信息头大小
        int width;                // 图像宽度
        int height;               // 图像高度
        unsigned short planes;    // 位面数
        unsigned short bpp;       // 像素深度
        unsigned int compression; // 压缩方式
        unsigned int image_size;  // 图像大小
        int x_pels_per_meter;     // 像素/米
        int y_pels_per_meter;     // 像素/米
        unsigned int clr_used;
        unsigned int clr_omportant;
    } __attribute__((packed)) BmpInfoHeader;

    typedef enum
    {
        IMAGE_NOT_INIT = -1,
        IMAGE_OK = 0,
    } ImageStatus;

private:
    int fd;
    bool inverted;            // 是否是倒置位图,图片高度>0为倒置位图
    std::string file_name;    // 文件名
    BmpFileHeader file_h;     // 文件头
    BmpInfoHeader info_h;     // 图片信息头
    unsigned long line_bytes; // 一行的字节数
    unsigned long image_seek; // 图像数据偏移，分开读取一个图像时记录每次的读取的偏移量
    ImageStatus status;

public:
    BitImage(std::string path);
    ~BitImage();

    void GetImage(unsigned char *buf, unsigned long line);
    unsigned long GetImage();

    const void SetImageSeek(unsigned long seek) { image_seek = seek; }

    const BmpInfoHeader &GetInfo() const { return info_h; }
    const unsigned long GetLineBytes() const { return line_bytes; }
    const ImageStatus GetStatus() const { return status; }
    const int GetFd() const { return fd; }

    static unsigned char *BitImageZoom(int w, int h, unsigned char *bmpin, int zoomw, int zoomh, int bpp)
    {
        int bmp_h = zoomh;
        int bmp_w = zoomw;
        int bytes = bpp / 8;

        float zoom = (float)zoomh / bmp_h;

        unsigned char *bmpout = (unsigned char *)malloc(bmp_h * bmp_w * bytes);
        unsigned char *pucDest;
        unsigned char *pucSrc;
        unsigned long dwsrcX;
        unsigned long dwsrcY;

        for (int i = 0; i < bmp_h; i++)
        {
            dwsrcY = i * h / bmp_h;
            pucDest = bmpout + i * bmp_w * bytes;
            pucSrc = bmpin + dwsrcY * w * bytes;
            for (int j = 0; j < bmp_w; j++)
            {
                dwsrcX = j * w / bmp_w;
                memcpy(pucDest + j * bytes, pucSrc + dwsrcX * bytes, bytes);
            }
        }

        return bmpout;
    }
};
