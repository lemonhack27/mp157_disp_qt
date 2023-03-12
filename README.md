# mp157_disp_qt
使用stm32mp157开发板实现QT显示功能
LCD存在固定参数和可变参数，二者不一样，使用的时候选择错误会出现输出字符错位情况
需要交叉编译tslib库才能成功包含头文件tslib.h

# 各模块函数
## 初始化显示系统、
### 显示系统的结构体
typedef struct DispOpr {\
    char *name; _显示系统的名称，LCD的framebuffer还是WEB浏览器_\
    int (*DeviceInit)(void);
    _LCD屏幕的硬件数据获取以及映射framebuffer地址_\
    int (*DeviceExit)(void);_关闭lcd设备以及解除映射_\
    int (*GetBuffer)(PDispBuff ptDispBuff);_将硬件数据赋值给全局存储硬件数据结构体DispBuff_\
    int (*FlushRegion)(PRegion ptRegion, PDispBuff ptDispBuff);\
    struct DispOpr *ptNext;\
}DispOpr, *PDispOpr;\

typedef struct DispBuff\
{\
    int iXres;_X方向分辨率_\
    int iYres;_Y方向分辨率_\
    int iBpp;_每个像素的位数_\
    int line_length;_一行字节大小_\
    char *buff;_framebuffer起始地址_\
}DispBuff, *PDispBuff;\

### 显示系统注册
    将需要使用的显示系统注册到显示系统管理链表里面。将LCD屏幕作为显示系统注册到显示系统管理链表里面，如果需要扩展使用WEB浏览器，则将WEB浏览器作为显示系统注册到显示系统管理链表里面。\
    DisplayInit()-->FramebufferInit()-->RegisterDisplay(PDispOpr ptDispOpr)

### 选择要使用的显示系统
    是选择使用LCD的framebuffer还是WEB浏览器，根据名称选择依次比较显示系统管理链表内部各个节点的名称来匹配，匹配成功则将全局的显示操作结构体赋值为显示系统管理链表内节点的结构体。\
    SelectDefaultDisplay(char *name)

### 初始化显示设备
    在选择要使用的显示系统基础上面，调用全局的显示操作结构体内部的设备初始化函数，调用底层framebuffer的驱动。使用open来打开设备；使用ioctl来获取lcd屏幕的硬件数据，如分辨率；使用mmap来映射lcd屏幕的起始地址和大小。将获取的硬件数据放在专门的结构体里面。\
    InitDefaultDisplay()-->FbDeviceInit(void);\
    InitDefaultDisplay()-->FbGetBuffer(PDispBuff ptDispBuff);\


