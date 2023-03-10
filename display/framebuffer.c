#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <disp_manager.h>

static int fd_fb;
static struct fb_var_screeninfo var;
static struct fb_fix_screeninfo var1;//可变参数，这才能获得正确的lcd宽度数据
static int screen_size;
static unsigned char *fb_base;
static unsigned int line_width;
static unsigned int pixel_width;

static int FbDeviceInit(void)
{
    fd_fb = open("/dev/fb0", O_RDWR);
    if(fd_fb < 0)
    {
        printf("can't open /dev/fd0\n");
        return -1;
    }
    if(ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
    {
        printf("cant't get var\n");
        return -1;
    }
    if(ioctl(fd_fb, FBIOGET_FSCREENINFO, &var1))
    {
        printf("cant't get var\n");
        return -1;
    }

//    line_width = var.xres * var.bits_per_pixel / 8;
    line_width = var1.line_length;
    pixel_width = var.bits_per_pixel / 8;
//    screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
    screen_size = line_width * var.yres;
    fb_base = (unsigned char *)mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
    if(fb_base == (unsigned char *)-1)
    {
        printf("can't mmap\n");
        return -1;
    }
    return 0;
}

static int FbDeviceExit(void)
{
    munmap(fb_base, screen_size);
    close(fd_fb);
    return 0;
}

/*
可以返回LCD的framebuffer，以后上层APP可以直接操作LCD，就可以不使用FbFlushRegion函数
也可以malloc返回一块无关的buffer，使用FbFlushRegion刷新数据到LCD上面
*/

static int FbGetBuffer(PDispBuff ptDispBuff)
{
    ptDispBuff->iXres = var.xres;
    ptDispBuff->iYres = var.yres;
    ptDispBuff->iBpp = var.bits_per_pixel;
    ptDispBuff->line_length = var1.line_length;
    ptDispBuff->buff = (char *)fb_base;
    return 0;
}

static int FbFlushRegion(PRegion ptRegion, PDispBuff ptDispBuff)
{
    return 0;
}


static DispOpr g_tFramebufferOpr = {
    .name = "fb",
    .DeviceInit = FbDeviceInit,
    .DeviceExit = FbDeviceExit,
    .GetBuffer = FbGetBuffer,
    .FlushRegion = FbFlushRegion,
};

void FramebufferInit(void)
{
    RegisterDisplay(&g_tFramebufferOpr);
}