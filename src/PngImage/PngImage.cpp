#include "PngImage.h"

/**
 * @brief Construct a new Png Image:: Png Image object
 * 
 * @param path 
 */
PngImage::PngImage(std::string path)
{
    _status = IMAGE_ERR;
    _pngPtr = NULL;
    _infoPtr = NULL;

    do
    {
        _file = fopen(path.c_str(), "r");
        if (_file == NULL)
        {
            perror("[PngImage] open error!");
            break;
        }

        /* 创建png_ptr, info_ptr */
        _pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (_pngPtr == NULL)
        {
            printf("[PngImage] png decoder create failed\n");
            fclose(_file);
            _file = NULL;
            break;
        }

        _infoPtr = png_create_info_struct(_pngPtr);
        if (_infoPtr == NULL)
        {
            printf("[PngImage] png decoder create failed\n");
            png_destroy_read_struct(&_pngPtr, NULL, NULL);
            fclose(_file);
            _file = NULL;
            break;
        }

        /* 设置png错误跳转点 */
        if (setjmp(png_jmpbuf(_pngPtr)))
        {
            printf("[PngImage] png error\n");
            png_destroy_read_struct(&_pngPtr, &_infoPtr, NULL);
            fclose(_file);
            _file = NULL;
            break;
        }
        // 指定png数据源
        png_init_io(_pngPtr, _file);
        // 获取png图像数据
        png_read_info(_pngPtr, _infoPtr);
        _imageInfo.width = png_get_image_width(_pngPtr, _infoPtr);
        _imageInfo.height = png_get_image_height(_pngPtr, _infoPtr);
        _imageInfo.bpp = png_get_bit_depth(_pngPtr, _infoPtr) * 4;
        _lineBytes = png_get_rowbytes(_pngPtr, _infoPtr);

        unsigned char color_type = png_get_color_type(_pngPtr, _infoPtr);

        if (color_type == PNG_COLOR_TYPE_RGB)
            png_set_add_alpha(_pngPtr, 0xff, PNG_FILLER_AFTER); // 添加alpha通道转argb8888

        png_set_bgr(_pngPtr);

        png_read_update_info(_pngPtr, _infoPtr);

        _status = IMAGE_OK;

    } while (0);
}

/**
 * @brief Destroy the Png Image:: Png Image object
 * 
 */
PngImage::~PngImage()
{
    png_destroy_read_struct(&_pngPtr, &_infoPtr, NULL);

    if (_file != NULL)
        fclose(_file);
}

/**
 * @brief getImage 获取Png图片
 * 
 * @param buf 
 * @param line 
 */
void PngImage::getImage(unsigned char *buf, unsigned long line)
{
    if (buf == NULL || line <= 0 || _status == IMAGE_ERR)
        return;

    /* 设置png错误跳转点 */
    if (setjmp(png_jmpbuf(_pngPtr)))
    {
        printf("png error\n");
        png_destroy_read_struct(&_pngPtr, &_infoPtr, NULL);
        fclose(_file);
        _file = NULL;
        return;
    }

    unsigned char *lineBuf = new unsigned char[_lineBytes];

    for (int i = 0; i < line; i++)
    {
        png_read_rows(_pngPtr, &lineBuf, NULL, 1); // 每次读取一行

        unsigned char *p = buf + _lineBytes * i;

        memcpy(p, lineBuf, _lineBytes);
    }

    png_read_end(_pngPtr, _infoPtr);

    delete[] lineBuf;
}