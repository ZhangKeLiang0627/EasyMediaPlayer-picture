#pragma once

#include "../libs/lvgl/lvgl.h"
#include "../utils/lv_ext/lv_obj_ext_func.h"
#include "../utils/lv_ext/lv_anim_timeline_wrapper.h"
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

            struct
            {
                lv_obj_t *cont;
                lv_obj_t *cancelBtn; // to cancel this app
                lv_obj_t *titleLabel;
            } topCont;

            lv_anim_timeline_t *anim_timeline;
            lv_anim_timeline_t *anim_timelineClick;
            lv_anim_timeline_t *anim_timelineTop;

            bool isTopContCollapsed = true;
        } ui;

        void create(Operations &opts);
        void release(void);
        void appearAnimStart(bool reverse = false);
        void appearAnimClick(bool reverse = false);
        void appearAnimTop(bool reverse = false);

        const uint8_t *addImageList(ImgInfo info, int tag);
        void setImageListPageNum(int total);
        void setListChangeReady(void);
        lv_obj_t *galleryImageCreate(lv_obj_t *parent, ImgInfo &info, int x_ofs, int y_ofs);
        static void imgListClickEventHandler(lv_event_t *event);

    private:
        void AttachEvent(lv_obj_t *obj);

        void listCreate(lv_obj_t *obj);
        void contCreate(lv_obj_t *obj);
        void btnContCreate(lv_obj_t *obj);
        void topContCreate(lv_obj_t *obj);

        static void onEvent(lv_event_t *event);
        static void buttonEventHandler(lv_event_t *event);
        static void imgEventHandler(lv_event_t *event);
        void image_obj_slide_right(void);
        void image_obj_slide_left(void);
        void imageDelete(lv_obj_t *img);

        lv_obj_t *roundRectCreate(lv_obj_t *par, lv_coord_t x_ofs, lv_coord_t y_ofs);
        lv_obj_t *btnCreate(lv_obj_t *par, const void *img_src, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_coord_t w, lv_coord_t h);
        lv_obj_t *imageCreate(int x, int y, ImgInfo &info);

    private:
        lv_img_dsc_t *createImgDsc(ImgInfo &info);
        uint8_t *rgb888Toargb888(const uint8_t *src, int len);
        void imageChange(int tag, bool dir);
    };

}