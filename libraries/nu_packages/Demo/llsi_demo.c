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

#define THREAD_PRIORITY   10
#define THREAD_STACK_SIZE 1024
#define THREAD_TIMESLICE  5
#define LLSI_PIXEL_COUNT  8

void BuildPatterns(uint32_t *pu32LEDBuf, uint32_t u32BufLen, E_COLOR c, int reversed)
{
    rt_memset((void *)pu32LEDBuf, 0, u32BufLen);

    /* Generate patterns where only one LED is on (red), others are off */
    for (int p = 0; p < LLSI_PIXEL_COUNT; p++)
    {
        volatile uint8_t *pu8Buf = (volatile uint8_t *)&pu32LEDBuf[(LLSI_PIXEL_COUNT * p)];
        if (reversed)
            pu8Buf[evCNT * (LLSI_PIXEL_COUNT - p - 1) + c] = 0xFF;
        else
            pu8Buf[evCNT * p + c] = 0xFF;
    }
}

static void llsi_marquee_worker(void *parameter)
{
    rt_err_t err;
    rt_device_t dev = rt_device_find((const char *)parameter);
    uint32_t *pu32LEDBuf;

    pu32LEDBuf = rt_malloc_align(LLSI_PIXEL_COUNT * LLSI_PIXEL_COUNT * sizeof(uint32_t), 4);
    if (pu32LEDBuf == RT_NULL)
    {
        rt_kprintf("No memory\n");
        goto exit_llsi_marquee_worker;
    }

    S_LLSI_CONFIG_T config = NU_LLSI_CONFIG_DEFAULT;
    config.u32PCNT = LLSI_PIXEL_COUNT;

    err = rt_device_control(dev, 0, &config);
    if (err != RT_EOK)
    {
        rt_kprintf("Failed to set llsi controlling.\n");
        goto exit_llsi_marquee_worker;
    }

    err = rt_device_open(dev, 0);
    if (err != RT_EOK)
    {
        rt_kprintf("Failed to open\n");
        goto exit_llsi_marquee_worker;
    }

    uint32_t u32PatternToggle = 0;
    E_COLOR s_color = evR;
    while (1)
    {
        if (u32PatternToggle == 0)
        {
            /* Build marquee LED patterns */
            BuildPatterns(pu32LEDBuf,
                          LLSI_PIXEL_COUNT * LLSI_PIXEL_COUNT * sizeof(uint32_t),
                          s_color % evCNT,
                          s_color % 2);
            s_color++;
        }

        rt_device_write(dev,
                        0,
                        (const void *)&pu32LEDBuf[u32PatternToggle * LLSI_PIXEL_COUNT],
                        LLSI_PIXEL_COUNT * sizeof(S_LLSI_RGB));
        rt_thread_mdelay(50);

        /* Switch to next pattern */
        u32PatternToggle = (u32PatternToggle + 1) % LLSI_PIXEL_COUNT;
    }

    rt_device_close(dev);

exit_llsi_marquee_worker:

    if (pu32LEDBuf)
        rt_free_align(pu32LEDBuf);
}

int llsi_marquee(int argc, char **argv)
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
                               llsi_marquee_worker,
                               dev->parent.name,
                               THREAD_STACK_SIZE,
                               THREAD_PRIORITY,
                               THREAD_TIMESLICE);

        if (tid != RT_NULL)
            rt_thread_startup(tid);
    }

    return 0;
}
MSH_CMD_EXPORT(llsi_marquee, llsi marquee demo);

#endif /* #if defined(BSP_USING_SLCD) */
