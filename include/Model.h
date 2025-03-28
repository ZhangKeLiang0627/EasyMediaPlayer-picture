#pragma once

#include <string>
#include <functional>
#include <map>
#include "common_inc.h"
#include "View.h"
#include "../libs/lvgl/lvgl.h"

namespace Page
{
    class Model
    {
    public:
    private:
        pthread_t _pthread;                     // 数据处理线程
        bool _threadExitFlag;                   // 线程退出标志位
        pthread_mutex_t *_mutex;                // 互斥量
        View _view;                             // View的实例
        lv_timer_t *_timer;                     // LVGL软定时器
        std::map<int, std::string> _imageTable; // 图像映射表

    private:
        static void *threadProcHandler(void *);
        void update(void);
        static void onTimerUpdate(lv_timer_t *timer);

        // funtion for View
    private:
        static unsigned char *bmpImageDecode(std::string &file, int &wight, int &height, int &bitPerPixel);
        static unsigned char *jpegImageDecode(std::string &file, int &wight, int &height, int &bitPerPixel);
        static unsigned char *pngImageDecode(std::string &file, int &wight, int &height, int &bitPerPixel);
        static unsigned char *bitImageZoom(int w, int h, unsigned char *bmpin, int zoomw, int zoomh, int bpp);
        int searchImage(std::string &path, int listMax);

        void getImage(int tag, ImgInfo *info);
        int getNextTag(int curTag);
        int getPrevTag(int curTag);
        void changeImageListPage(int page);

    public:
        Model(std::function<void(void)> exitCb, pthread_mutex_t &mutex);
        ~Model();
    };
}