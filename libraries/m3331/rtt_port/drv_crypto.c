/**************************************************************************//**
*
* @copyright (C) 2024 Nuvoton Technology Corp. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date            Author       Notes
* 2025-9-2        Wayne        First version
*
******************************************************************************/

#include <rtconfig.h>

#if (defined(BSP_USING_CRC) && defined(RT_USING_HWCRYPTO))

#include <rtdevice.h>
#include "NuMicro.h"
#include "nu_bitutil.h"
#include "drv_crc.h"

/* Private typedef --------------------------------------------------------------*/
#define LOG_TAG         "drv.crypto"
#define DBG_ENABLE
#define DBG_SECTION_NAME LOG_TAG
#define DBG_LEVEL         DBG_INFO
#define DBG_COLOR
#include <rtdbg.h>

/* Private functions ------------------------------------------------------------*/
static rt_err_t nu_hwcrypto_create(struct rt_hwcrypto_ctx *ctx);
static void nu_hwcrypto_destroy(struct rt_hwcrypto_ctx *ctx);
static rt_err_t nu_hwcrypto_clone(struct rt_hwcrypto_ctx *des, const struct rt_hwcrypto_ctx *src);
static void nu_hwcrypto_reset(struct rt_hwcrypto_ctx *ctx);

/* Private variables ------------------------------------------------------------*/
static const struct rt_hwcrypto_ops nu_hwcrypto_ops =
{
    .create = nu_hwcrypto_create,
    .destroy = nu_hwcrypto_destroy,
    .copy = nu_hwcrypto_clone,
    .reset = nu_hwcrypto_reset,
};

/* CRC operation ------------------------------------------------------------*/
#if defined(BSP_USING_CRC)
static const struct hwcrypto_crc_ops nu_crc_ops =
{
    .update = nu_crc_update,
};
#endif


/*
 * @brief  Create and initialize a hardware crypto context.
 * @param  ctx: Pointer to hardware crypto context
 * @return RT_EOK on success, negative error code otherwise
 */
static rt_err_t nu_hwcrypto_create(struct rt_hwcrypto_ctx *ctx)
{
    RT_ASSERT(ctx != RT_NULL);

    switch (ctx->type & HWCRYPTO_MAIN_TYPE_MASK)
    {
#if defined(BSP_USING_CRC) && defined(RT_HWCRYPTO_USING_CRC)
    case HWCRYPTO_TYPE_CRC:
        ctx->contex = RT_NULL;
        // Set CRC operation handlers
        ((struct hwcrypto_crc *)ctx)->ops = &nu_crc_ops;
        break;
#endif
    default:
        LOG_E("Unsupported crypto type: 0x%08X", ctx->type);
        return -RT_ERROR;
    }

    nu_hwcrypto_reset(ctx);

    return RT_EOK;
}


/*
 * @brief  Destroy a hardware crypto context and free resources.
 * @param  ctx: Pointer to hardware crypto context
 */
static void nu_hwcrypto_destroy(struct rt_hwcrypto_ctx *ctx)
{
    RT_ASSERT(ctx != RT_NULL);
    if (ctx->contex)
    {
        rt_free(ctx->contex);
        ctx->contex = RT_NULL;
    }
}


/*
 * @brief  Clone (copy) a hardware crypto context.
 * @param  des: Destination context
 * @param  src: Source context
 * @return RT_EOK on success, negative error code otherwise
 */
static rt_err_t nu_hwcrypto_clone(struct rt_hwcrypto_ctx *des, const struct rt_hwcrypto_ctx *src)
{
    RT_ASSERT(des != RT_NULL);
    RT_ASSERT(src != RT_NULL);

    if (des->contex && src->contex)
    {
        rt_memcpy(des->contex, src->contex, sizeof(struct rt_hwcrypto_ctx));
        return RT_EOK;
    }

    return -RT_EINVAL;
}


/*
 * @brief  Reset a hardware crypto context (currently a stub).
 * @param  ctx: Pointer to hardware crypto context
 */
static void nu_hwcrypto_reset(struct rt_hwcrypto_ctx *ctx)
{
    RT_ASSERT(ctx != RT_NULL);
    // Add per-type reset logic if needed in the future
    switch (ctx->type & HWCRYPTO_MAIN_TYPE_MASK)
    {
    default:
        break;
    }
}

/* Init and register nu_hwcrypto_dev */

/*
 * @brief  Initialize and register the Nuvoton hardware crypto device.
 * @return 0 on success
 */
int nu_hwcrypto_device_init(void)
{
    rt_err_t result;
    static struct rt_hwcrypto_device nu_hwcrypto_dev;

    nu_hwcrypto_dev.ops = &nu_hwcrypto_ops;
    nu_hwcrypto_dev.id = 0;
    nu_hwcrypto_dev.user_data = &nu_hwcrypto_dev;

#if defined(BSP_USING_CRC)
    nu_crc_init();
#endif

    // Register hardware crypto device
    result = rt_hwcrypto_register(&nu_hwcrypto_dev, RT_HWCRYPTO_DEFAULT_NAME);
    RT_ASSERT(result == RT_EOK);

    return 0;
}
INIT_DEVICE_EXPORT(nu_hwcrypto_device_init);

#endif //#if (defined(BSP_USING_CRC) && defined(RT_USING_HWCRYPTO))
