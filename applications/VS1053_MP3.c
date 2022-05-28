/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-17     xqyjlj       the first version
 */
#include "rtthread.h"
#include "vs1053.h"
#include <dfs_posix.h>

void mp3_play_song(void)//char *path
{
    int fp;
    VS_Restart_Play();
    VS_Set_All();
    VS_Reset_DecodeTime();
    fp = open("/abc.wav", O_RDWR | O_CREAT);
    if (fp)
    {
        VS_SPI_SpeedHigh();
        uint8_t *buf = RT_NULL;
        buf = rt_malloc(1024 * 10);
        if (buf)
        {
            int readsize = read(fp,buf,1024 * 10);
            if (readsize < 1024 * 10 && readsize > 0)
            {
                uint32_t i = 0;
                do
                {
                    if (VS_Send_MusicData(buf + i) == 0)
                    {
                        i += 32;
                    }
                } while (i <readsize);
            }
            while (readsize == 1024 * 10)
            {
                if (readsize > 0)
                {
                    uint32_t i = 0;
                    do
                    {
                        if (VS_Send_MusicData(buf + i) == 0)
                        {
                            i += 32;
                        }
                    } while (i < readsize);
                }
                readsize = read(fp,buf,1024 * 10);
            }
            close(fp);

        }
        rt_free(buf);
    }
}
MSH_CMD_EXPORT(mp3_play_song, vs1053 sample);