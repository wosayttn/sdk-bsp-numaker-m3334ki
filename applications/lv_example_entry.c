/**************************************************************************//**
 * @file     lv_examle_app.c
 * @brief    LVGL run an example.
 *
 * @note
 * Copyright (C) 2025 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "rtthread.h"

#if defined(BOARD_USING_NUFUN)

#include "rtdevice.h"
#include "drv_gpio.h"

#include "lvgl.h"

#define SW2      NU_GET_PININDEX(NU_PH, 4)
#define SW3      NU_GET_PININDEX(NU_PH, 5)
#define SW4      NU_GET_PININDEX(NU_PH, 6)
#define SW5      NU_GET_PININDEX(NU_PH, 7)

#define UP       NU_GET_PININDEX(NU_PG, 9)
#define DOWN     NU_GET_PININDEX(NU_PG, 11)
#define CENTER   NU_GET_PININDEX(NU_PA, 12)
#define LEFT     NU_GET_PININDEX(NU_PG, 15)
#define RIGHT    NU_GET_PININDEX(NU_PG, 10)

#define STATUS_LED1     NU_GET_PININDEX(NU_PG, 4)
#define STATUS_LED2     NU_GET_PININDEX(NU_PF, 11)

static uint32_t s_LastKeypadValue = 0;
static lv_obj_t *needle_line;
static lv_indev_t *s_indev = NULL;
static lv_obj_t *led1, *led2, *led3, *led4;

static rt_device_t psDevSensor_Baro;
static rt_device_t psDevSensor_Temperture;
static rt_device_t psDevSensor_3Axis;
static rt_device_t psDevSensor_Encoder;

static lv_subject_t SubjectValue_Baro;
static lv_subject_t SubjectValue_Temperture;
static lv_subject_t SubjectValue_3Axis_X;
static lv_subject_t SubjectValue_3Axis_Y;
static lv_subject_t SubjectValue_3Axis_Z;
static lv_subject_t SubjectValue_Encoder;

static struct rt_device_pwm *psDevBPWM0, *psDevEPWM1;

static void set_needle_line_value(void *obj, int32_t v)
{
    lv_scale_set_line_needle_value((lv_obj_t *)obj, needle_line, 60, v);
}

static void anim_x_cb(void *var, int32_t v)
{
    lv_obj_set_x((lv_obj_t *) var, v);
}

static void start_llsi_marquee(void)
{
    int argc = 2;
    char *argv[2];

    argv[0] = "llsi_marquee";
    argv[1] = "llsi0";

    int llsi_marquee(int argc, char **argv);
    llsi_marquee(argc, argv);
}

static void start_mp3_player(void)
{
    int argc = 3;
    char *argv[3];

    argv[0] = "mp3play";
    argv[1] = "-s";
    argv[2] = "/test.mp3";

    int mp3_player(int argc, char **argv);
    mp3_player(argc, argv);
}

static void stop_mp3_player(void)
{
    int argc = 2;
    char *argv[2];

    argv[0] = "mp3play";
    argv[1] = "-t";

    int mp3_player(int argc, char **argv);
    mp3_player(argc, argv);
}


static void sw_event_cb(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target_obj(e);
    lv_obj_t *label = (lv_obj_t *) lv_event_get_user_data(e);

    if (lv_obj_has_state(sw, LV_STATE_CHECKED))
    {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, label);
        lv_anim_set_values(&a, lv_obj_get_x(label), 100);
        lv_anim_set_duration(&a, 500);
        lv_anim_set_exec_cb(&a, anim_x_cb);
        lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
        lv_anim_start(&a);

        start_llsi_marquee();
        start_mp3_player();

        rt_pin_write(STATUS_LED1, PIN_HIGH);
        rt_pin_write(STATUS_LED2, PIN_LOW);

        if (psDevBPWM0 != RT_NULL)
        {
            rt_pwm_set(psDevBPWM0, 0, 1000, 0);
            rt_pwm_set(psDevBPWM0, 1, 1000, 0);
            rt_pwm_set(psDevBPWM0, 2, 1000, 0);
        }
        if (psDevEPWM1 != RT_NULL)
        {
            rt_pwm_set(psDevEPWM1, 0, 1000, 1000);
        }
    }
    else
    {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, label);
        lv_anim_set_values(&a, lv_obj_get_x(label), -lv_obj_get_width(label));
        lv_anim_set_duration(&a, 500);
        lv_anim_set_exec_cb(&a, anim_x_cb);
        lv_anim_set_path_cb(&a, lv_anim_path_ease_in);
        lv_anim_start(&a);

        stop_mp3_player();
        rt_pin_write(STATUS_LED1, PIN_LOW);
        rt_pin_write(STATUS_LED2, PIN_HIGH);

        if (psDevBPWM0 != RT_NULL)
        {
            rt_pwm_set(psDevBPWM0, 0, 1000, 1000);
            rt_pwm_set(psDevBPWM0, 1, 1000, 1000);
            rt_pwm_set(psDevBPWM0, 2, 1000, 1000);
        }
        if (psDevEPWM1 != RT_NULL)
        {
            rt_pwm_set(psDevEPWM1, 0, 1000, 0);
        }
    }
}

/**
 * Start animation on an event
 */
void lv_switch_anim(void)
{
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "NuMaker-M3334KI@RTT");
    lv_obj_set_pos(label, 10, 10);

    lv_obj_t *sw = lv_switch_create(lv_screen_active());
    lv_obj_align(sw, LV_ALIGN_BOTTOM_RIGHT, -20, -10);
    lv_obj_add_state(sw, LV_STATE_CHECKED);
    lv_obj_set_state(sw, LV_STATE_CHECKED, false);
    lv_obj_add_event_cb(sw, sw_event_cb, LV_EVENT_VALUE_CHANGED, label);
}

/**
 * A simple round scale
 */
static void lv_meter(void)
{
    lv_obj_t *scale_line = lv_scale_create(lv_screen_active());

    lv_obj_set_size(scale_line, 150, 150);
    lv_scale_set_mode(scale_line, LV_SCALE_MODE_ROUND_INNER);
    lv_obj_set_style_bg_opa(scale_line, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(scale_line, lv_palette_lighten(LV_PALETTE_RED, 5), 0);
    lv_obj_set_style_radius(scale_line, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_clip_corner(scale_line, true, 0);
    lv_obj_align(scale_line, LV_ALIGN_LEFT_MID, LV_PCT(2), 0);

    lv_scale_set_label_show(scale_line, true);

    lv_scale_set_total_tick_count(scale_line, 31);
    lv_scale_set_major_tick_every(scale_line, 5);

    lv_obj_set_style_length(scale_line, 5, LV_PART_ITEMS);
    lv_obj_set_style_length(scale_line, 10, LV_PART_INDICATOR);
    lv_scale_set_range(scale_line, 10, 40);

    lv_scale_set_angle_range(scale_line, 270);
    lv_scale_set_rotation(scale_line, 135);

    needle_line = lv_line_create(scale_line);

    lv_obj_set_style_line_width(needle_line, 6, LV_PART_MAIN);
    lv_obj_set_style_line_rounded(needle_line, true, LV_PART_MAIN);

    lv_anim_t anim_scale_line;
    lv_anim_init(&anim_scale_line);
    lv_anim_set_var(&anim_scale_line, scale_line);
    lv_anim_set_exec_cb(&anim_scale_line, set_needle_line_value);
    lv_anim_set_duration(&anim_scale_line, 1000);
    lv_anim_set_repeat_count(&anim_scale_line, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_reverse_duration(&anim_scale_line, 1000);
    lv_anim_set_values(&anim_scale_line, 10, 40);
    lv_anim_start(&anim_scale_line);
}

static void keypad_event_cb(lv_event_t *e)
{
    uint32_t key = lv_event_get_key(e);
    lv_event_code_t code = lv_event_get_code(e);

    if (code != LV_EVENT_KEY)
        return;

    switch (key)
    {
    case LV_KEY_UP:
        lv_led_toggle(led4);
        break;

    case LV_KEY_DOWN:
        lv_led_toggle(led3);
        break;

    case LV_KEY_LEFT:
        lv_led_toggle(led2);
        break;

    case LV_KEY_RIGHT:
        lv_led_toggle(led1);
        break;

    default:
        break;
    }
}

static void lv_leds(void)
{
    if (s_indev)
    {
        /* LED objects */
        led1 = lv_led_create(lv_screen_active());
        lv_obj_align(led1, LV_ALIGN_CENTER, -120, 100);
        lv_led_off(led1);

        led2 = lv_led_create(lv_screen_active());
        lv_obj_align(led2, LV_ALIGN_CENTER, -80, 100);
        lv_led_off(led2);

        led3 = lv_led_create(lv_screen_active());
        lv_obj_align(led3, LV_ALIGN_CENTER, -40, 100);
        lv_led_off(led3);

        led4 = lv_led_create(lv_screen_active());
        lv_obj_align(led4, LV_ALIGN_CENTER, 0, 100);
        lv_led_off(led4);

        lv_group_t *g = lv_group_create();
        lv_group_add_obj(g, led1);
        lv_indev_set_group(s_indev, g);
        lv_obj_add_event_cb(led1, keypad_event_cb, LV_EVENT_KEY, NULL);
        lv_group_focus_obj(led1);
    }
}

static void sensor_timer_cb(lv_timer_t *timer)
{

    if (psDevSensor_Temperture)
    {
        static struct rt_sensor_data SensorData_Temperture = {0};

        rt_device_read(psDevSensor_Temperture, 0, &SensorData_Temperture, 1);

        lv_subject_set_int(&SubjectValue_Temperture, (int)SensorData_Temperture.data.temp);
    }

    if (psDevSensor_3Axis)
    {
        static struct rt_sensor_data SensorData_3Axis = {0};

        rt_device_read(psDevSensor_3Axis, 0, &SensorData_3Axis, 1);

        lv_subject_set_int(&SubjectValue_3Axis_X, SensorData_3Axis.data.acce.x);
        lv_subject_set_int(&SubjectValue_3Axis_Y, SensorData_3Axis.data.acce.y);
        lv_subject_set_int(&SubjectValue_3Axis_Z, SensorData_3Axis.data.acce.z);
    }

    if (psDevSensor_Baro)
    {
        static struct rt_sensor_data SensorData_baro = {0};

        rt_device_read(psDevSensor_Baro, 0, &SensorData_baro, 1);

        lv_subject_set_int(&SubjectValue_Baro, (int)SensorData_baro.data.baro);
    }

    if (psDevSensor_Encoder)
    {
        static rt_int32_t curr_encoder_value = 50;
        rt_int32_t count;

        rt_device_read(psDevSensor_Encoder, 0, &count, 1);
        rt_device_control(psDevSensor_Encoder, PULSE_ENCODER_CMD_CLEAR_COUNT, RT_NULL);

        if (count != 0)
        {
            if (count < 100) //Turn right
            {
                curr_encoder_value += (count / 2);
                curr_encoder_value = (curr_encoder_value >= 100) ? 100 : curr_encoder_value;
            }
            else if (count > 900) //Turn left
            {
                curr_encoder_value -= ((1000 - count) / 2);
                curr_encoder_value = (curr_encoder_value <= 0) ? 0 : curr_encoder_value;
            }

            lv_subject_set_int(&SubjectValue_Encoder, curr_encoder_value);
        }

    }
}

static void lv_sensors(void)
{
    lv_subject_init_int(&SubjectValue_Baro, 1000);
    lv_subject_init_int(&SubjectValue_Temperture, 30);
    lv_subject_init_int(&SubjectValue_3Axis_X, 0);
    lv_subject_init_int(&SubjectValue_3Axis_Y, 0);
    lv_subject_init_int(&SubjectValue_3Axis_Z, 0);
    lv_subject_init_int(&SubjectValue_Encoder, 50);

    lv_obj_t *label_baro = lv_label_create(lv_screen_active());
    lv_obj_align(label_baro, LV_ALIGN_CENTER, 60, -60);
    lv_label_bind_text(label_baro, &SubjectValue_Baro, "Pressure: %d pa");

    lv_obj_t *label_temperture = lv_label_create(lv_screen_active());
    lv_obj_align(label_temperture, LV_ALIGN_CENTER, 60, -40);
    lv_label_bind_text(label_temperture, &SubjectValue_Temperture, "Temperature: %d °C");

    lv_obj_t *label_3axis_x = lv_label_create(lv_screen_active());
    lv_obj_align(label_3axis_x, LV_ALIGN_CENTER, 60, -20);
    lv_label_bind_text(label_3axis_x, &SubjectValue_3Axis_X, "X-Axis: %d mG");

    lv_obj_t *label_3axis_y = lv_label_create(lv_screen_active());
    lv_obj_align(label_3axis_y, LV_ALIGN_CENTER, 60, 0);
    lv_label_bind_text(label_3axis_y, &SubjectValue_3Axis_Y, "Y-Axis: %d mG");

    lv_obj_t *label_3axis_z = lv_label_create(lv_screen_active());
    lv_obj_align(label_3axis_z, LV_ALIGN_CENTER, 60, 20);
    lv_label_bind_text(label_3axis_z, &SubjectValue_3Axis_Z, "Z-Axis: %d mG");

    lv_obj_t *label_encoder = lv_label_create(lv_screen_active());
    lv_obj_align(label_encoder, LV_ALIGN_CENTER, 60, 40);
    lv_label_bind_text(label_encoder, &SubjectValue_Encoder, "Encoder: %d %%");

    lv_timer_create(sensor_timer_cb, 100, NULL);
}

void lv_user_gui_init(void)
{
    rt_pin_mode(STATUS_LED1, PIN_MODE_OUTPUT);
    rt_pin_mode(STATUS_LED2, PIN_MODE_OUTPUT);

    rt_pin_write(STATUS_LED1, PIN_HIGH);
    rt_pin_write(STATUS_LED2, PIN_HIGH);

    psDevBPWM0 = (struct rt_device_pwm *)rt_device_find("bpwm0");
    if (psDevBPWM0 != RT_NULL)
    {
        rt_pwm_set(psDevBPWM0, 0, 1000, 1000);
        rt_pwm_set(psDevBPWM0, 1, 1000, 1000);
        rt_pwm_set(psDevBPWM0, 2, 1000, 1000);

        rt_pwm_enable(psDevBPWM0, 0);
        rt_pwm_enable(psDevBPWM0, 1);
        rt_pwm_enable(psDevBPWM0, 2);
    }

    psDevEPWM1 = (struct rt_device_pwm *)rt_device_find("epwm1");
    if (psDevEPWM1 != RT_NULL)
    {
        rt_pwm_set(psDevEPWM1, 0, 1000, 0);
        rt_pwm_enable(psDevEPWM1, 0);
    }

    psDevSensor_Temperture = rt_device_find("tm-tmp111");
    if (psDevSensor_Temperture)
    {
        rt_device_open(psDevSensor_Temperture, RT_DEVICE_FLAG_RDWR);
    }

    psDevSensor_3Axis = rt_device_find("ac-lis3dh");
    if (psDevSensor_3Axis)
    {
        rt_device_open(psDevSensor_3Axis, RT_DEVICE_FLAG_RDWR);
    }

    psDevSensor_Baro = rt_device_find("br-lps22hh");
    if (psDevSensor_Baro)
    {
        rt_kprintf("Open barometer sensor device\n");
        rt_device_open(psDevSensor_Baro, RT_DEVICE_FLAG_RDWR);
    }

    psDevSensor_Encoder = rt_device_find("eqei0");
    if (psDevSensor_Encoder)
    {
        rt_device_open(psDevSensor_Encoder, RT_DEVICE_OFLAG_RDONLY);
    }

    lv_meter();
    lv_switch_anim();
    lv_leds();
    lv_sensors();
}

static int key_pressed(void)
{
    if (rt_pin_read(SW2) == PIN_LOW)
        s_LastKeypadValue = LV_KEY_RIGHT;
    else if (rt_pin_read(SW3) == PIN_LOW)
        s_LastKeypadValue = LV_KEY_LEFT;
    else if (rt_pin_read(SW4) == PIN_LOW)
        s_LastKeypadValue = LV_KEY_DOWN;
    else if (rt_pin_read(SW5) == PIN_LOW)
        s_LastKeypadValue = LV_KEY_UP;
    else if (rt_pin_read(UP) == PIN_LOW)
        s_LastKeypadValue = LV_KEY_UP;
    else if (rt_pin_read(DOWN) == PIN_LOW)
        s_LastKeypadValue = LV_KEY_DOWN;
    else if (rt_pin_read(CENTER) == PIN_LOW)
        s_LastKeypadValue = LV_KEY_ENTER;
    else if (rt_pin_read(LEFT) == PIN_LOW)
        s_LastKeypadValue = LV_KEY_LEFT;
    else if (rt_pin_read(RIGHT) == PIN_LOW)
        s_LastKeypadValue = LV_KEY_RIGHT;
    else
        return RT_FALSE;

    return RT_TRUE;
}

static uint32_t keypad_last_key(void)
{
    return s_LastKeypadValue;
}

static void keypad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    if (key_pressed())
    {
        data->key = keypad_last_key();
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void lv_port_other_indev_init(void)
{
    // 4-buttons
    rt_pin_mode(SW2, PIN_MODE_INPUT);
    rt_pin_mode(SW3, PIN_MODE_INPUT);
    rt_pin_mode(SW4, PIN_MODE_INPUT);
    rt_pin_mode(SW5, PIN_MODE_INPUT);

    // JoyStick
    rt_pin_mode(UP, PIN_MODE_INPUT);
    rt_pin_mode(DOWN, PIN_MODE_INPUT);
    rt_pin_mode(CENTER, PIN_MODE_INPUT);
    rt_pin_mode(LEFT, PIN_MODE_INPUT);
    rt_pin_mode(RIGHT, PIN_MODE_INPUT);

    s_indev = lv_indev_create();

    lv_indev_set_type(s_indev, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(s_indev, keypad_read);
}

#endif
