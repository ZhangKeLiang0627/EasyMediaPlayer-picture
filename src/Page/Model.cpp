#include "Model.h"
#include <dirent.h>

#include "JpegImage.h"
#include "PngImage.h"
#include "BitImage.h"

#define IMAGE_DIR "/mnt/UDISK/pictures/"

using namespace Page;

/**
 * @brief Model构造函数
 *
 * @param exitCb
 * @param mutex
 */
Model::Model(std::function<void(void)> exitCb, pthread_mutex_t &mutex)
{
    _threadExitFlag = false;
    _mutex = &mutex;

    // 设置UI回调函数
    Operations uiOpts = {0};
    uiOpts.getImageCb = std::bind(&Model::getImage, this, std::placeholders::_1, std::placeholders::_2);
    uiOpts.getNextTagCb = std::bind(&Model::getNextTag, this, std::placeholders::_1);
    uiOpts.getPrevTagCb = std::bind(&Model::getPrevTag, this, std::placeholders::_1);
    uiOpts.changeListPageCb = std::bind(&Model::changeImageListPage, this, std::placeholders::_1);
    _view.create(uiOpts);

    // 这里设置一个1000ms的定时器，软定时器，用于在onTimerUpdate里update
    _timer = lv_timer_create(onTimerUpdate, 1000, this);
    // 创建执行线程，传递this指针
    pthread_create(&_pthread, NULL, threadProcHandler, this);
}

Model::~Model()
{
    _threadExitFlag = true;

    lv_timer_del(_timer);

    _view.release();
}

/**
 * @brief 定时器更新函数
 *
 */
void Model::onTimerUpdate(lv_timer_t *timer)
{
    Model *instance = (Model *)timer->user_data;

    instance->update();
}

/**
 * @brief 更新UI等事务
 *
 */
void Model::update(void)
{
}

/**
 * @brief 线程处理函数
 *
 * @return void*
 */
void *Model::threadProcHandler(void *arg)
{
    Model *model = static_cast<Model *>(arg); // 将arg转换为Model指针

    usleep(5000);

    std::string imagePath = IMAGE_DIR;

    int imgTotal = model->searchImage(imagePath, 300);
    printf("[Model] search image complete! total:%d\n", imgTotal);

    int w, h, bpp;
    unsigned char *image = nullptr;

    for (int i = 0; i < imgTotal; i++)
    {
        // std::string filePath = "/mnt/UDISK/pictures/" + std::to_string(i + 1) + ".jpg";
        std::string filePath = model->_imageTable[i];

        const char *pfile = strrchr(filePath.c_str(), '.');
        if (strcasecmp(pfile, ".bmp") == 0)
        {
            printf("bmp file\n");
            image = bmpImageDecode(filePath, w, h, bpp);
        }
        else if (strcasecmp(pfile, ".jpg") == 0 || strcasecmp(pfile, ".jpeg") == 0)
        {
            printf("jpg/jpeg file\n");
            image = jpegImageDecode(filePath, w, h, bpp);
        }
        else if (strcasecmp(pfile, ".png") == 0)
        {
            printf("png file\n");
            image = pngImageDecode(filePath, w, h, bpp);
        }

        if (image != NULL)
        {
            unsigned char *zoomimge = model->bitImageZoom(w, h, image, 210, 158, bpp);
            delete[] image;
            pthread_mutex_lock(model->_mutex);
            model->_view.addImageList((ImgInfo){210, 158, zoomimge, bpp}, i);
            pthread_mutex_unlock(model->_mutex);
        }
        // usleep(30000);
    }

    while (!model->_threadExitFlag)
    {

        usleep(50000);
    }
}

int Model::searchImage(std::string &path, int listMax)
{
    int count = 0;
    bool legalImg = false;
    std::string filePath;

    struct dirent *ent;
    DIR *dir = opendir(path.c_str());

    for (int i = 0; i < listMax; i++)
    {
        ent = readdir(dir);
        if (ent == NULL)
            break;

        if (ent->d_type == DT_REG)
        {
            const char *pfile = strrchr(ent->d_name, '.');
            if (pfile != NULL)
            {
                filePath = path + std::string(ent->d_name);

                if (strcasecmp(pfile, ".bmp") == 0)
                {
                    printf("bmp file\n");
                    legalImg = true;
                }
                else if (strcasecmp(pfile, ".jpg") == 0 || strcasecmp(pfile, ".jpeg") == 0)
                {
                    printf("jpg/jpeg file\n");
                    legalImg = true;
                }
                else if (strcasecmp(pfile, ".png") == 0)
                {
                    printf("png file\n");
                    legalImg = true;
                }
            }
        }
        if (legalImg == true)
        {
            legalImg = false;
            _imageTable.insert({count, filePath}); // 将图像索引和文件名插入map
            count++;
        }

        // usleep(50000);
    }

    closedir(dir);

    return count;
}

unsigned char *Model::bmpImageDecode(std::string &file, int &wight, int &height, int &bitPerPixel)
{
    BitImage *bmp = new BitImage(file);
    int w = bmp->GetInfo().width;
    int h = bmp->GetInfo().height;
    int bpp = bmp->GetInfo().bpp;

    printf("[Model] bmp image width:%d\n", w);
    printf("[Model] bmp image height:%d\n", h);
    printf("[Model] bmp image bpp:%d\n", bpp);

    unsigned char *buf = new unsigned char[w * h * bpp / 8];

    bmp->GetImage(buf, h);

    wight = w;
    height = h;
    bitPerPixel = bpp;

    delete bmp;

    return buf;
}

unsigned char *Model::jpegImageDecode(std::string &file, int &wight, int &height, int &bitPerPixel)
{
    JpegImage *jpeg = new JpegImage(file);
    int w = jpeg->getInfo().output_width;
    int h = jpeg->getInfo().output_height;
    int bpp = jpeg->getInfo().output_components * 8;

    printf("[Model] jpeg image width:%d\n", w);
    printf("[Model] jpeg image height:%d\n", h);
    printf("[Model] jpeg image bpp:%d\n", bpp);

    unsigned char *buf = new unsigned char[w * h * bpp / 8];

    jpeg->getImage(buf, h);

    wight = w;
    height = h;
    bitPerPixel = bpp;

    delete jpeg;

    return buf;
}

unsigned char *Model::pngImageDecode(std::string &file, int &wight, int &height, int &bitPerPixel)
{
    PngImage *png = new PngImage(file);
    int w = png->getInfo().width;
    int h = png->getInfo().height;
    int bpp = png->getInfo().bpp;

    printf("[Model] png image width:%d\n", w);
    printf("[Model] png image height:%d\n", h);
    printf("[Model] png image bpp:%d\n", bpp);

    unsigned char *buf = new unsigned char[w * h * bpp / 8];

    png->getImage(buf, h);

    wight = w;
    height = h;
    bitPerPixel = bpp;

    delete png;

    return buf;
}

unsigned char *Model::bitImageZoom(int w, int h, unsigned char *bmpin, int zoomw, int zoomh, int bpp)
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

void Model::getImage(int tag, ImgInfo *info)
{
    std::string path = _imageTable[tag];
    unsigned char *image = NULL;

    const char *pfile = strrchr(path.c_str(), '.');
    if (pfile != NULL)
    {
        int w, h, bpp;

        if (strcasecmp(pfile, ".bmp") == 0)
        {
            printf("bmp file\n");
            image = bmpImageDecode(path, w, h, bpp);
        }
        else if (strcasecmp(pfile, ".jpg") == 0 || strcasecmp(pfile, ".jpeg") == 0)
        {
            printf("jpg/jpeg file\n");
            image = jpegImageDecode(path, w, h, bpp);
        }
        else if (strcasecmp(pfile, ".png") == 0)
        {
            printf("png file\n");
            image = pngImageDecode(path, w, h, bpp);
        }

        info->w = w;
        info->h = h;
        info->bpp = bpp;
        info->imgMap = image;
    }
}

int Model::getNextTag(int curTag)
{
    int tag = curTag + 1;

    int imgTotal = _imageTable.size();

    if (tag == imgTotal)
        tag = 0;

    return tag;
}

int Model::getPrevTag(int curTag)
{
    int tag = curTag - 1;

    int imgTotal = _imageTable.size();

    if (tag < 0)
        tag = imgTotal - 1;

    return tag;
}

void Model::changeImageListPage(int page)
{
    // destpage = page;

    // updateMutex.unlock(); // 释放互斥量
}