#include "Model.h"
#include <dirent.h>

#include "JpegImage.h"
#include "PngImage.h"

#define IMAGE_DIR "/mnt/UDISK/images/"

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

    int w, h, bpp;
    unsigned char *image = nullptr;
    std::string filePath = "/mnt/UDISK/pic1.jpg";
    image = model->jpegImageDecode(filePath, w, h, bpp);
    if (image != NULL)
    {
        unsigned char *zoomimge = model->bitImageZoom(w, h, image, 180, 135, bpp);
        delete[] image;

        model->_view.addImageList((ImgInfo){180, 135, zoomimge, bpp}, 0);
    }

    filePath = "/mnt/UDISK/pic2.jpg";
    image = model->jpegImageDecode(filePath, w, h, bpp);
    if (image != NULL)
    {
        unsigned char *zoomimge = model->bitImageZoom(w, h, image, 180, 135, bpp);
        delete[] image;

        model->_view.addImageList((ImgInfo){180, 135, zoomimge, bpp}, 200);
    }

    while (!model->_threadExitFlag)
    {

        usleep(50000);
    }
}

unsigned char *Model::bmpImageDecode(std::string &file, int &wight, int &height, int &bitPerPixel)
{
    // BitImage *bmp = new BitImage(file);
    // int w = bmp->GetInfo().width;
    // int h = bmp->GetInfo().height;
    // int bpp = bmp->GetInfo().bpp;

    // printf("[Model] bmp image width:%d\n", w);
    // printf("[Model] bmp image height:%d\n", h);
    // printf("[Model] bmp image bpp:%d\n", bpp);

    // unsigned char *buf = new unsigned char[w * h * bpp / 8];

    // bmp->GetImage(buf, h);

    // wight = w;
    // height = h;
    // bitPerPixel = bpp;

    // delete bmp;

    // return buf;
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