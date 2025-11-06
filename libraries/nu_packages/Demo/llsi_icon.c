/**************************************************************************//**
*
* @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date            Author       Notes
* 2020-11-11      Wayne        First version
*
******************************************************************************/

#include "rtthread.h"

#if defined(BSP_USING_LLSI) || defined(BSP_USING_ELLSI)

#include "drv_llsi.h"
#include "drv_common.h"

#define THREAD_PRIORITY   10
#define THREAD_STACK_SIZE 1024
#define THREAD_TIMESLICE  5
#define LLSI_PIXEL_COUNT  256

#define PATH_BMP_INCBIN    ".//Cancel.bmp"
INCBIN(cancel_bmp, PATH_BMP_INCBIN);

typedef struct
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
} S_BMP_COLOR;

void color_transform(void *src, uint32_t pixel_count)
{
    S_BMP_COLOR *pSrc = (S_BMP_COLOR *)src;

    for (uint32_t i = 0; i < pixel_count; i++)
    {
        uint8_t tmp = pSrc[i].r;  // save red
        pSrc[i].r = pSrc[i].b;    // put blue into red
        pSrc[i].b = tmp;          // put saved red into blue
    }
}

static void llsi_icon_worker(void *parameter)
{
    rt_err_t err;
    rt_device_t dev = rt_device_find((const char *)parameter);
    uint32_t *pu32LEDBuf;
    uint8_t  *file_ptr = (uint8_t *)&incbin_cancel_bmp_start;
    uint32_t  bs_len = (uint32_t)((char *)&incbin_cancel_bmp_end - (char *)&incbin_cancel_bmp_start);

    pu32LEDBuf = rt_malloc_align(RT_ALIGN(LLSI_PIXEL_COUNT * 3, 4), 4);
    if (pu32LEDBuf == RT_NULL)
    {
        rt_kprintf("No memory\n");
        goto exit_llsi_icon_worker;
    }

    S_LLSI_CONFIG_T config = NU_LLSI_CONFIG_DEFAULT;
    config.u32PCNT = LLSI_PIXEL_COUNT;

    err = rt_device_control(dev, 0, &config);
    if (err != RT_EOK)
    {
        rt_kprintf("Failed to set llsi controlling.\n");
        goto exit_llsi_icon_worker;
    }

    err = rt_device_open(dev, 0);
    if (err != RT_EOK)
    {
        rt_kprintf("Failed to open\n");
        goto exit_llsi_icon_worker;
    }

    while (1)
    {
        rt_memcpy((void *)pu32LEDBuf, (const void *)file_ptr + 54, bs_len - 54);
        color_transform((void *)pu32LEDBuf, LLSI_PIXEL_COUNT);

        rt_device_write(dev,
                        0,
                        (const void *)&pu32LEDBuf[0],
                        LLSI_PIXEL_COUNT * sizeof(S_LLSI_RGB));

        rt_thread_mdelay(1000);
    }

    rt_device_close(dev);

exit_llsi_icon_worker:

    if (pu32LEDBuf)
        rt_free_align(pu32LEDBuf);
}

int llsi_icon(int argc, char **argv)
{
    if (argc < 2)
    {
        rt_kprintf("input llsi device name.\n");
        return -RT_ERROR;
    }

    rt_device_t dev = rt_device_find(argv[1]);
    if (dev == RT_NULL)
        return -RT_ERROR;

    rt_thread_t tid = rt_thread_find(dev->parent.name);
    if (tid == RT_NULL)
    {
        rt_kprintf("LLSI device name: %s\n", argv[1]);

        tid = rt_thread_create(dev->parent.name,
                               llsi_icon_worker,
                               dev->parent.name,
                               THREAD_STACK_SIZE,
                               THREAD_PRIORITY,
                               THREAD_TIMESLICE);

        if (tid != RT_NULL)
            rt_thread_startup(tid);
    }

    return 0;
}
MSH_CMD_EXPORT(llsi_icon, llsi show icon demo);

#endif /* #if defined(BSP_USING_SLCD) */
