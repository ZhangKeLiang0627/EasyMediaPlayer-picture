#include "JpegImage.h"

/**
 * @brief Construct a new Jpeg Image:: Jpeg Image object
 *
 * @param path
 */
JpegImage::JpegImage(std::string path)
{
    _status = IMAGE_ERR;

    do
    {
        _file = fopen(path.c_str(), "r");
        if (_file == NULL)
        {
            perror("[JpegImage] open error!");
            break;
        }
        // 绑定默认错误处理函数
        _info.err = jpeg_std_error(&_err);
        // 创建JPEG解码器
        jpeg_create_decompress(&_info);
        // 指定解码图片文件
        jpeg_stdio_src(&_info, _file);
        // 读取图像信息
        int ret = jpeg_read_header(&_info, TRUE);
        if (ret != JPEG_HEADER_OK)
        {
            printf("[JpegImage] %s: read jpeg header failed\n", path.c_str());
            fclose(_file);
            _file = NULL;
            break;
        }
        // 输出颜色格式为RGB888
        _info.out_color_space = JCS_EXT_BGRA;
        // 开始解码
        jpeg_start_decompress(&_info);

        _status = IMAGE_OK;
        _lineBytes = _info.output_components * _info.output_width;

    } while (0);
}

/**
 * @brief Destroy the Jpeg Image:: Jpeg Image object
 *
 */
JpegImage::~JpegImage()
{
    jpeg_finish_decompress(&_info);  // 完成解码
    jpeg_destroy_decompress(&_info); // 销毁解码器

    if (_file != NULL)
        fclose(_file);
}

/**
 * @brief getImage 获取Jpeg图片
 *
 * @param buf
 * @param line
 */
void JpegImage::getImage(unsigned char *buf, unsigned long line)
{
    if (buf == NULL || line <= 0 || _status == IMAGE_ERR)
        return;

    unsigned char *lineBuf = new unsigned char[_lineBytes];

    for (int i = 0; i < line; i++)
    {
        // 每次读取一行
        jpeg_read_scanlines(&_info, &lineBuf, 1);

        unsigned char *p = buf + _lineBytes * i;

        memcpy(p, lineBuf, _lineBytes);
    }

    delete[] lineBuf;
}
