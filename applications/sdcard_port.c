/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-14     balanceTWK   add sdcard port file
 */

#include <rtthread.h>

#ifdef BSP_USING_TF_CARD

#include <dfs_elm.h>
#include <dfs_fs.h>
#include <dfs_posix.h>
#include "drv_spi.h"
#include "spi_msd.h"

#define DBG_TAG "app.card"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

static int rt_hw_spi2_tfcard(void)
{
//    __HAL_RCC_GPIOB_CLK_ENABLE();
    rt_hw_spi_device_attach("spi2", "spi20", GPIOB, GPIO_PIN_12);
    return msd_init("sd0", "spi20");
}
INIT_COMPONENT_EXPORT(rt_hw_spi2_tfcard);

void sd_mount(void *parameter)
{
    while (1)
    {
        rt_thread_mdelay(1000);
        if(rt_device_find("sd0") != RT_NULL)
        {
            if (dfs_mount("sd0", "/", "elm", 0, 0) == RT_EOK)
            {
                LOG_I("sd card mount to '/'");
                break;
            }
            else
            {
                LOG_W("sd card mount to '/' failed!");
                dfs_mkfs("elm","sd0");
                LOG_I("sd card mkfs!");
            }
        }
    }
}

int stm32_sdcard_mount(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("sd_mount", sd_mount, RT_NULL,
                           1024, RT_THREAD_PRIORITY_MAX - 2, 20);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        LOG_E("create sd_mount thread err!");
    }
    return RT_EOK;
}
INIT_APP_EXPORT(stm32_sdcard_mount);
//MSH_CMD_EXPORT(stm32_sdcard_mount, sd device sample);
#endif /* BSP_USING_SDCARD */

