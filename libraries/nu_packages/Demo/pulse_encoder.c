/*
 * Program Description: This is a sample program demonstrating the use of a pulse encoder device.
 * The sample exports the `pulse_encoder_sample` command to the command-line shell.
 * Command usage: pulse_encoder_sample
 * Function: Reads the pulse encoder count every 500 ms, clears the count register,
 *           and prints the retrieved count value.
 */

#include <rtthread.h>

#if defined(RT_USING_PULSE_ENCODER) && defined(BSP_USING_EQEI0)

#include <rtdevice.h>

#define PULSE_ENCODER_DEV_NAME    "eqei0"    /* Pulse encoder device name */

static int pulse_encoder_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    rt_device_t pulse_encoder_dev = RT_NULL;   /* Pulse encoder device handle */
    rt_uint32_t index;
    rt_int32_t count;

    /* Find the pulse encoder device */
    pulse_encoder_dev = rt_device_find(PULSE_ENCODER_DEV_NAME);
    if (pulse_encoder_dev == RT_NULL)
    {
        rt_kprintf("Pulse encoder sample failed! Cannot find device: %s\n", PULSE_ENCODER_DEV_NAME);
        return -RT_ERROR;
    }

    /* Open the device in read-only mode */
    ret = rt_device_open(pulse_encoder_dev, RT_DEVICE_OFLAG_RDONLY);
    if (ret != RT_EOK)
    {
        rt_kprintf("Failed to open device: %s\n", PULSE_ENCODER_DEV_NAME);
        return -ret;
    }

    while (1)
    {
        rt_thread_mdelay(1000);

        /* Read the pulse encoder count */
        rt_device_read(pulse_encoder_dev, 0, &count, 1);

        /* Clear the pulse encoder count */
        rt_device_control(pulse_encoder_dev, PULSE_ENCODER_CMD_CLEAR_COUNT, RT_NULL);

        rt_kprintf("count = %d\n", count);
    }

    rt_device_close(pulse_encoder_dev);

    return -ret;
}

/* Export to MSH command list */
MSH_CMD_EXPORT(pulse_encoder_sample, pulse encoder sample);

#endif
