#pragma once

#include "../libs/lvgl/lvgl.h"
#include "lv_obj_ext_func.h"
#include "lv_anim_timeline_wrapper.h"
#include <functional>

namespace Page
{
    struct ImgInfo
    {
        int w;
        int h;
        const uint8_t *imgMap;
        int bpp;
    };

    struct ImgData
    {
        int tag;
        lv_img_dsc_t *src;
    };

    using ExitCb = std::function<void(void)>;
    using GetImageCb = std::function<void(int, ImgInfo *)>;
    using GetImageTagCb = std::function<int(int)>;
    using ChangeListCb = std::function<void(int)>;

    struct Operations
    {
        ExitCb exitCb;                 // 退出程序回调函数
        GetImageCb getImageCb;         // 获取图片数据回调函数
        GetImageTagCb getPrevTagCb;    // 切换上一张图片的tag回调函数
        GetImageTagCb getNextTagCb;    // 切换下一张图片的tag回调函数
        ChangeListCb changeListPageCb; // 切换页面，参数为列表索引
    };

    class View
    {
    private:
        Operations _opts; // View回调函数集

    public:
        struct
        {
            lv_obj_t *cont;

            struct
            {
                lv_obj_t *cont;
                lv_obj_t *btn;
            } btnCont;

            struct
            {
                lv_obj_t *cont;
            } listCont;

            lv_anim_timeline_t *anim_timeline;
            lv_anim_timeline_t *anim_timelineClick;
        } ui;

        void create(Operations &opts);
        void release(void);
        void appearAnimStart(bool reverse = false);
        void appearAnimClick(bool reverse = false);

        const uint8_t *addImageList(ImgInfo info, int tag);
        void setImageListPageNum(int total);
        void setListChangeReady(void);

    private:
        void AttachEvent(lv_obj_t *obj);

        void listCreate(lv_obj_t *obj);
        void contCreate(lv_obj_t *obj);
        void btnContCreate(lv_obj_t *obj);

        static void onEvent(lv_event_t *event);
        static void buttonEventHandler(lv_event_t *event);

        lv_obj_t *roundRectCreate(lv_obj_t *par, lv_coord_t x_ofs, lv_coord_t y_ofs);
        lv_obj_t *btnCreate(lv_obj_t *par, const void *img_src, lv_coord_t y_ofs);

    private:
        lv_img_dsc_t *createImgDsc(ImgInfo &info);
        uint8_t *rgb888Toargb888(const uint8_t *src, int len);
    };

}