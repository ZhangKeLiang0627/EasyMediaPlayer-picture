#include "BitImage.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

BitImage::BitImage(std::string path)
{
    file_name = path;
    status = IMAGE_NOT_INIT;
    inverted = true;

    do
    {
        /* 打开文件 */
        if (0 > (fd = open(file_name.c_str(), O_RDONLY)))
        {
            perror("open error");
            break;
        }
        /* 读取 BMP 文件头 */
        if (sizeof(BmpFileHeader) != read(fd, &file_h, sizeof(BmpFileHeader)))
        {
            perror("read error");
            close(fd);
            fd = 0;
            break;
        }
        if (0 != memcmp(file_h.type, "BM", 2))
        {
            fprintf(stderr, "it's not a BMP file\n");
            close(fd);
            fd = 0;
            break;
        }
        /* 读取位图信息头 */
        if (sizeof(BmpInfoHeader) != read(fd, &info_h, sizeof(BmpInfoHeader)))
        {
            perror("read error");
            close(fd);
            fd = 0;
            break;
        }
        status = IMAGE_OK;
        image_seek = 0;
        line_bytes = info_h.width * info_h.bpp / 8;

        if (info_h.height < 0) // 图片高度小于0为正向位图
        {
            inverted = false;
            info_h.height = -info_h.height;
        }

    } while (0);
}

BitImage::~BitImage()
{
    if (fd != 0)
        close(fd);
}

void BitImage::GetImage(unsigned char *buf, unsigned long line)
{
    if (buf == NULL || line <= 0 || status == IMAGE_NOT_INIT)
        return;

    if (image_seek == 0)
    {
        if (-1 == lseek(fd, file_h.offset, SEEK_SET))
        {
            perror("lseek error");
            return;
        }
    }

    if (inverted == true)
    {
        for (int i = line; i > 0; i--)
        {
            unsigned char *p = buf + line_bytes * (i - 1);

            read(fd, p, line_bytes);
        }
    }
    else
        read(fd, buf, line * line_bytes);
}

unsigned long BitImage::GetImage()
{
    return file_h.offset;
}
