#include "View.h"

using namespace Page;

void View::create(Operations &opts)
{
    // 获取View回调函数集
    _opts = opts;

    // 总画布的创建
    contCreate(lv_scr_act());

    // 图片列表画布的创建
    listCreate(ui.cont);

    // 按钮画布的创建
    btnContCreate(ui.cont);

    // 为当前屏幕添加事件回调函数
    AttachEvent(lv_scr_act());

    // 动画的创建
    ui.anim_timeline = lv_anim_timeline_create();
    ui.anim_timelineClick = lv_anim_timeline_create();

#define ANIM_DEF(start_time, obj, attr, start, end) \
    {start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}

#define ANIM_OPA_DEF(start_time, obj) \
    ANIM_DEF(start_time, obj, opa_scale, LV_OPA_COVER, LV_OPA_TRANSP)

    lv_anim_timeline_wrapper_t wrapper[] =
        {
            ANIM_DEF(0, ui.btnCont.cont, height, 20, lv_obj_get_height(ui.btnCont.cont)),
            ANIM_DEF(0, ui.btnCont.cont, width, 20, lv_obj_get_width(ui.btnCont.cont)),

            LV_ANIM_TIMELINE_WRAPPER_END // 这个标志着结构体成员结束，不能省略，在下面函数lv_anim_timeline_add_wrapper的轮询中做判断条件
        };
    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);

    lv_coord_t xOriginal = lv_obj_get_x_aligned(lv_obj_get_child(ui.btnCont.cont, 1));
    lv_coord_t yOriginal = lv_obj_get_y_aligned(lv_obj_get_child(ui.btnCont.cont, 1));

    lv_anim_timeline_wrapper_t wrapperForClick[] =
        {
            ANIM_DEF(0, lv_obj_get_child(ui.btnCont.cont, 1), x, xOriginal, xOriginal - 30),
            ANIM_OPA_DEF(300, lv_obj_get_child(ui.btnCont.cont, 1)),
            {300, lv_obj_get_child(ui.btnCont.cont, 1), (lv_anim_exec_xcb_t)lv_obj_set_shadow_opa_scale, LV_OPA_COVER, LV_OPA_TRANSP, 500, lv_anim_path_ease_out, true},

            ANIM_DEF(0, lv_obj_get_child(ui.btnCont.cont, 2), x, xOriginal, xOriginal - 21),
            ANIM_DEF(0, lv_obj_get_child(ui.btnCont.cont, 2), y, yOriginal, yOriginal - 21),
            ANIM_OPA_DEF(300, lv_obj_get_child(ui.btnCont.cont, 2)),
            {300, lv_obj_get_child(ui.btnCont.cont, 2), (lv_anim_exec_xcb_t)lv_obj_set_shadow_opa_scale, LV_OPA_COVER, LV_OPA_TRANSP, 500, lv_anim_path_ease_out, true},

            ANIM_DEF(0, lv_obj_get_child(ui.btnCont.cont, 3), y, yOriginal, yOriginal - 30),
            ANIM_OPA_DEF(300, lv_obj_get_child(ui.btnCont.cont, 3)),
            {300, lv_obj_get_child(ui.btnCont.cont, 3), (lv_anim_exec_xcb_t)lv_obj_set_shadow_opa_scale, LV_OPA_COVER, LV_OPA_TRANSP, 500, lv_anim_path_ease_out, true},

            ANIM_DEF(0, lv_obj_get_child(ui.btnCont.cont, 4), x, xOriginal, xOriginal + 21),
            ANIM_DEF(0, lv_obj_get_child(ui.btnCont.cont, 4), y, yOriginal, yOriginal - 21),
            ANIM_OPA_DEF(300, lv_obj_get_child(ui.btnCont.cont, 4)),
            {300, lv_obj_get_child(ui.btnCont.cont, 4), (lv_anim_exec_xcb_t)lv_obj_set_shadow_opa_scale, LV_OPA_COVER, LV_OPA_TRANSP, 500, lv_anim_path_ease_out, true},

            ANIM_DEF(0, lv_obj_get_child(ui.btnCont.cont, 5), x, xOriginal, xOriginal + 30),
            ANIM_OPA_DEF(300, lv_obj_get_child(ui.btnCont.cont, 5)),
            {300, lv_obj_get_child(ui.btnCont.cont, 5), (lv_anim_exec_xcb_t)lv_obj_set_shadow_opa_scale, LV_OPA_COVER, LV_OPA_TRANSP, 500, lv_anim_path_ease_out, true},

            ANIM_DEF(0, lv_obj_get_child(ui.btnCont.cont, 6), x, xOriginal, xOriginal + 21),
            ANIM_DEF(0, lv_obj_get_child(ui.btnCont.cont, 6), y, yOriginal, yOriginal + 21),
            ANIM_OPA_DEF(300, lv_obj_get_child(ui.btnCont.cont, 6)),
            {300, lv_obj_get_child(ui.btnCont.cont, 6), (lv_anim_exec_xcb_t)lv_obj_set_shadow_opa_scale, LV_OPA_COVER, LV_OPA_TRANSP, 500, lv_anim_path_ease_out, true},

            ANIM_DEF(0, lv_obj_get_child(ui.btnCont.cont, 7), y, yOriginal, yOriginal + 30),
            ANIM_OPA_DEF(300, lv_obj_get_child(ui.btnCont.cont, 7)),
            {300, lv_obj_get_child(ui.btnCont.cont, 7), (lv_anim_exec_xcb_t)lv_obj_set_shadow_opa_scale, LV_OPA_COVER, LV_OPA_TRANSP, 500, lv_anim_path_ease_out, true},

            ANIM_DEF(0, lv_obj_get_child(ui.btnCont.cont, 8), x, xOriginal, xOriginal - 21),
            ANIM_DEF(0, lv_obj_get_child(ui.btnCont.cont, 8), y, yOriginal, yOriginal + 21),
            ANIM_OPA_DEF(300, lv_obj_get_child(ui.btnCont.cont, 8)),
            {300, lv_obj_get_child(ui.btnCont.cont, 8), (lv_anim_exec_xcb_t)lv_obj_set_shadow_opa_scale, LV_OPA_COVER, LV_OPA_TRANSP, 500, lv_anim_path_ease_out, true},

            LV_ANIM_TIMELINE_WRAPPER_END // 这个标志着结构体成员结束，不能省略，在下面函数lv_anim_timeline_add_wrapper的轮询中做判断条件
        };
    lv_anim_timeline_add_wrapper(ui.anim_timelineClick, wrapperForClick);

    // 开始动画
    appearAnimStart();
}

void View::release()
{
    if (ui.anim_timeline)
    {
        lv_anim_timeline_del(ui.anim_timeline);
        ui.anim_timeline = nullptr;
    }
    if (ui.anim_timelineClick)
    {
        lv_anim_timeline_del(ui.anim_timelineClick);
        ui.anim_timelineClick = nullptr;
    }
    // 移除屏幕手势回调函数
    lv_obj_remove_event_cb(lv_scr_act(), onEvent);
}

void View::appearAnimStart(bool reverse) // 开始开场动画
{
    lv_anim_timeline_set_reverse(ui.anim_timeline, reverse);
    lv_anim_timeline_start(ui.anim_timeline);
}

void View::appearAnimClick(bool reverse) // 按钮动画
{
    lv_anim_timeline_set_reverse(ui.anim_timelineClick, reverse);
    lv_anim_timeline_start(ui.anim_timelineClick);
}

void View::AttachEvent(lv_obj_t *obj)
{
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

// 总画布的创建
void View::contCreate(lv_obj_t *obj)
{
    lv_obj_t *cont = lv_obj_create(obj);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0xcccccc), 0);
    lv_obj_set_style_bg_img_src(cont, "S:./res/icon/main1.bin", 0);
    lv_obj_set_style_bg_img_opa(cont, LV_OPA_COVER, 0);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    ui.cont = cont;
}

// 图片列表画布的创建
void View::listCreate(lv_obj_t *obj)
{
    lv_obj_t *cont = lv_obj_create(obj);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_HOR_RES - 30, lv_pct(80));
    lv_obj_add_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 10, 0);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x888888), 0);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, -33);

    // 设置网格布局
    static lv_coord_t col_dsc[] = {210, 21, 210, LV_GRID_TEMPLATE_LAST};                            // 每列宽度为210，间隔为21
    static lv_coord_t row_dsc[] = {158, 21, 158, 21, 158, 21, 158, 21, 158, LV_GRID_TEMPLATE_LAST}; // 每行高度为158，间隔为21
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);

    // 设置内边距
    lv_obj_set_style_pad_all(cont, 4, 0); // 设置所有方向的内边距为4像素

    ui.listCont.cont = cont;
}

// 按钮画布的创建
void View::btnContCreate(lv_obj_t *obj)
{
    lv_obj_t *btnCont = lv_obj_create(obj);
    lv_obj_remove_style_all(btnCont);
    lv_obj_set_size(btnCont, lv_pct(95), LV_VER_RES / 4);
    lv_obj_clear_flag(btnCont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(btnCont, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(btnCont, lv_color_hex(0x6a8d6d), 0);
    lv_obj_align(btnCont, LV_ALIGN_BOTTOM_MID, 0, 42);
    lv_obj_set_style_radius(btnCont, 16, LV_PART_MAIN);
    ui.btnCont.cont = btnCont;

    lv_obj_t *btn = btnCreate(btnCont, LV_SYMBOL_PLAY, -20);
    lv_obj_add_event_cb(btn, buttonEventHandler, LV_EVENT_ALL, this);
    ui.btnCont.btn = btn;

    lv_obj_t *catGif1 = lv_gif_create(btnCont);
    lv_gif_set_src(catGif1, "S:./cat1.gif");
    lv_obj_align_to(catGif1, btn, LV_ALIGN_OUT_LEFT_MID, -20, -10);

    lv_obj_t *catGif2 = lv_gif_create(btnCont);
    lv_gif_set_src(catGif2, "S:./cat2.gif");
    lv_obj_align_to(catGif2, btn, LV_ALIGN_OUT_RIGHT_MID, 20, -5);

    /* Render octagon explode */
    lv_obj_t *roundRect_1 = roundRectCreate(btnCont, 0, -20);
    lv_obj_t *roundRect_2 = roundRectCreate(btnCont, 0, -20);
    lv_obj_t *roundRect_3 = roundRectCreate(btnCont, 0, -20);
    lv_obj_t *roundRect_4 = roundRectCreate(btnCont, 0, -20);
    lv_obj_t *roundRect_5 = roundRectCreate(btnCont, 0, -20);
    lv_obj_t *roundRect_6 = roundRectCreate(btnCont, 0, -20);
    lv_obj_t *roundRect_7 = roundRectCreate(btnCont, 0, -20);
    lv_obj_t *roundRect_8 = roundRectCreate(btnCont, 0, -20);
}

lv_obj_t *View::btnCreate(lv_obj_t *par, const void *img_src, lv_coord_t y_ofs)
{
    lv_obj_t *obj = lv_obj_create(par);
    lv_obj_remove_style_all(obj);
    lv_obj_set_size(obj, 180, 50);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_align(obj, LV_ALIGN_CENTER, 0, y_ofs);
    lv_obj_set_style_bg_img_src(obj, img_src, 0);

    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_width(obj, 150, LV_STATE_PRESSED);                        // 设置button按下时的宽
    lv_obj_set_style_height(obj, 40, LV_STATE_PRESSED);                        // 设置button按下时的长
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x356b8c), 0);                 // 设置按钮默认的颜色
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x242947), LV_STATE_PRESSED);  // 设置按钮在被按下时的颜色
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xf2daaa), LV_STATE_FOCUSED);  // 设置按钮在被聚焦时的颜色
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xa99991), LV_STATE_DISABLED); // 设置按钮失能时的颜色
    lv_obj_set_style_radius(obj, 9, 0);                                        // 按钮画圆角

    static lv_style_transition_dsc_t tran;
    static const lv_style_prop_t prop[] = {LV_STYLE_WIDTH, LV_STYLE_HEIGHT, LV_STYLE_PROP_INV};
    lv_style_transition_dsc_init(
        &tran,
        prop,
        lv_anim_path_ease_out,
        150,
        0,
        NULL);
    lv_obj_set_style_transition(obj, &tran, LV_STATE_PRESSED);
    lv_obj_set_style_transition(obj, &tran, LV_STATE_FOCUSED);

    lv_obj_update_layout(obj);

    return obj;
}

lv_obj_t *View::roundRectCreate(lv_obj_t *par, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    /* Render octagon explode */
    lv_obj_t *roundRect = lv_obj_create(par);
    lv_obj_remove_style_all(roundRect);
    lv_obj_set_size(roundRect, 10, 10);
    lv_obj_set_style_radius(roundRect, 2, 0);

    lv_obj_set_style_shadow_width(roundRect, 10, 0);
    lv_obj_set_style_shadow_ofs_x(roundRect, 1, 0);
    lv_obj_set_style_shadow_ofs_y(roundRect, 1, 0);
    lv_obj_set_style_shadow_color(roundRect, lv_color_hex(0x5d8c3d), 0);
    lv_obj_set_style_shadow_spread(roundRect, 1, 0);
    lv_obj_set_style_shadow_opa(roundRect, LV_OPA_TRANSP, 0);

    lv_obj_set_style_bg_color(roundRect, lv_color_hex(0x88d35e), 0);
    lv_obj_set_style_bg_opa(roundRect, LV_OPA_TRANSP, 0);
    lv_obj_align(roundRect, LV_ALIGN_CENTER, x_ofs, y_ofs);

    return roundRect;
}

/**
 * @brief 在图片列表添加一个img对象显示图片缩略图
 * @param info 图片信息
 * @param tag 图片标签
 * @param get_image 获取图像原始数据回调
 */
const uint8_t *View::addImageList(ImgInfo info, int tag)
{
    lv_img_dsc_t *src = createImgDsc(info);

    ImgData *data = new ImgData;

    data->src = src;
    data->tag = tag;

    if (src != nullptr)
    {
        lv_obj_t *img = lv_img_create(ui.listCont.cont);
        lv_img_set_src(img, src);

        lv_obj_set_style_bg_color(img, lv_color_black(), LV_PART_MAIN);

        lv_obj_set_style_radius(img, 12, LV_PART_MAIN);
        lv_obj_set_style_clip_corner(img, true, LV_PART_MAIN);

        lv_obj_set_style_img_opa(img, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_opa(img, LV_OPA_80, LV_PART_MAIN | LV_STATE_PRESSED);

        lv_obj_set_style_outline_width(img, 2, LV_STATE_DEFAULT);
        lv_obj_set_style_outline_color(img, lv_color_hex(0x333333), LV_STATE_DEFAULT);
        lv_obj_set_style_outline_pad(img, 1, LV_STATE_PRESSED);
        lv_obj_set_style_outline_pad(img, 3, LV_STATE_DEFAULT);

        lv_obj_add_flag(img, LV_OBJ_FLAG_EVENT_BUBBLE |
                                 LV_OBJ_FLAG_CLICK_FOCUSABLE |
                                 LV_OBJ_FLAG_CLICKABLE); // 设置事件同时传播到父对象cell

        // 将图像对象添加到网格布局中
        lv_obj_set_grid_cell(img, LV_GRID_ALIGN_STRETCH, (tag % 2) * 2, 1, LV_GRID_ALIGN_STRETCH, (tag / 2) * 2, 1);

        lv_obj_set_user_data(img, data);
        // lv_obj_add_event_cb(img, img_list_click_event_handler, LV_EVENT_SHORT_CLICKED, nullptr);

        // if (++columnIndex == COLUMN_NUM)
        // {
        //     columnIndex = 0;
        //     rowIndex++;
        // }
    }

    return src->data;
}

/**
 * @brief 根据图像数据构建一个img_dsc
 */
lv_img_dsc_t *View::createImgDsc(ImgInfo &info)
{
    lv_img_dsc_t *src = nullptr;
    const uint8_t *map = info.imgMap;

    src = new lv_img_dsc_t;
    memset(src, 0, sizeof(lv_img_dsc_t));

    if (src != nullptr)
    {
        if (info.bpp == 24)
        {
            map = rgb888Toargb888(info.imgMap, info.w * info.h);
            delete[] info.imgMap;
        }
        src->header.always_zero = 0;
        src->header.w = info.w;
        src->header.h = info.h;
        src->data_size = info.w * info.h * LV_COLOR_SIZE / 8;
        src->header.cf = LV_IMG_CF_TRUE_COLOR;
        src->data = map;
    }

    return src;
}

/**
 * @brief rgb888Toargb888
 *
 * @param src
 * @param len
 * @return uint8_t*
 */
uint8_t *View::rgb888Toargb888(const uint8_t *src, int len)
{
    uint8_t *argb888 = new uint8_t[len * 4];

    for (int i = 0; i < len; i++)
    {
        argb888[4 * i] = src[3 * i];
        argb888[4 * i + 1] = src[3 * i + 1];
        argb888[4 * i + 2] = src[3 * i + 2];
        argb888[4 * i + 3] = 0xff;
    }

    return argb888;
}

void View::buttonEventHandler(lv_event_t *event)
{
    View *instance = (View *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t *obj = lv_event_get_current_target(event);

    /* Transparent background style */
    static lv_style_t style_scr_act;
    if (style_scr_act.prop_cnt == 0)
    {
        lv_style_init(&style_scr_act);
        lv_style_set_bg_opa(&style_scr_act, LV_OPA_COVER);
        lv_obj_add_style(lv_scr_act(), &style_scr_act, 0);
    }

    if (code == LV_EVENT_SHORT_CLICKED)
    {
        instance->appearAnimClick();
        instance->_opts.exitCb();
    }
}

void View::onEvent(lv_event_t *event)
{
    View *instance = (View *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_GESTURE)
    {
        switch (lv_indev_get_gesture_dir(lv_indev_get_act()))
        {
        case LV_DIR_LEFT:
            printf("[View] LV_DIR_LEFT!\n");

            break;
        case LV_DIR_RIGHT:
            printf("[View] LV_DIR_RIGHT!\n");

            break;
        case LV_DIR_TOP:
            printf("[View] LV_DIR_TOP!\n");

            break;
        case LV_DIR_BOTTOM:
            printf("[View] LV_DIR_BOTTOM!\n");
            // instance->_opts.exitCb();
            break;

        default:
            break;
        }
    }
}