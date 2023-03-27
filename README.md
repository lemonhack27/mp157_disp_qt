# mp157_disp_qt
使用stm32mp157开发板实现QT显示功能\
LCD存在固定参数和可变参数，二者不一样，使用的时候选择错误会出现输出字符错位情况\
需要交叉编译tslib库才能成功包含头文件tslib.h

# 各模块函数
## 初始化显示系统
### 显示系统的结构体
```
typedef struct DispOpr {
    char *name; __显示系统的名称，LCD的framebuffer还是WEB浏览器__
    int (*DeviceInit)(void);
    __LCD屏幕的硬件数据获取以及映射framebuffer地址__
    int (*DeviceExit)(void);__关闭lcd设备以及解除映射__
    int (*GetBuffer)(PDispBuff ptDispBuff);__将硬件数据赋值给全局存储硬件数据结构体DispBuff__
    int (*FlushRegion)(PRegion ptRegion, PDispBuff ptDispBuff);__将framebuffer内容刷新__
    struct DispOpr *ptNext;__链表结构__
}DispOpr, *PDispOpr;
```
```
typedef struct DispBuff
{
    int iXres;__X方向分辨率__
    int iYres;__Y方向分辨率__
    int iBpp;__每个像素的位数__
    int line_length;__一行字节大小__
    char *buff;__framebuffer起始地址__
}DispBuff, *PDispBuff;
```
### 显示系统注册
*   将需要使用的显示系统注册到显示系统管理链表里面。将LCD屏幕作为显示系统注册到显示系统管理链表里面，如果需要扩展使用WEB浏览器，则将WEB浏览器作为显示系统注册到显示系统管理链表里面。\
    DisplayInit()-->FramebufferInit()-->RegisterDisplay(PDispOpr ptDispOpr)

### 选择要使用的显示系统
*   是选择使用LCD的framebuffer还是WEB浏览器，根据名称选择依次比较显示系统管理链表内部各个节点的名称来匹配，匹配成功则将全局的显示操作结构体赋值为显示系统管理链表内节点的结构体。\
    SelectDefaultDisplay(char *name)

### 初始化显示设备
*   在选择要使用的显示系统基础上面，调用全局的显示操作结构体内部的设备初始化函数，调用底层framebuffer的驱动。使用open来打开设备；使用ioctl来获取lcd屏幕的硬件数据，如分辨率；使用mmap来映射lcd屏幕的起始地址和大小。将获取的硬件数据放在专门的结构体里面。\
    InitDefaultDisplay()-->FbDeviceInit(void);\
    InitDefaultDisplay()-->FbGetBuffer(PDispBuff ptDispBuff);

## 初始化输入系统
### 输入系统结构体
```
typedef struct InputEvent
{
    struct timeval tTime;__网络事件产生的时间__
    int iType;__事件的类型__
    int iX;__触摸事件的Y点__
    int iY;__触摸事件的Y点__
    int iPressure;__触摸事件是否被按下或者松开__
    char str[1024];__网络事件接收的数据__
}InputEvent, *PInputEvent;
```
```
typedef struct InputDevice
{
    char *name;__输入事件的名称（触摸or网络）__
    int (*GetInputEvent)(PInputEvent ptInputEvent);__获取输入事件信息__
    int (*DeviceInit)(void);__触摸屏硬件初始化或者网络配置UDP通信过程__
    int (*DeviceExit)(void);__关闭事件__
    struct InputDevice *ptNext;
}InputDevice, *PInputDevice;
```
### 输入系统注册
*   注册触摸屏输入和网络输入，即将触摸屏输入定义的输入设备结构体与网络输入定义的输入结构体全部挂载全局的输入设备结构体下面，便于查找与使用。
InputInit(void)-->TouchscreenRegister(void)-->RegisterInputDevice(PInputDevice ptInputDev)\
InputInit(void)-->NetInputRegister(void)-->RegisterInputDevice(PInputDevice ptInputDev)

### 输入设备初始化
*   从挂载了全部输入设备的结构体中将各个设备取出来，对其调用结构体内部定义的关于自己设备初始化的函数，并且对各个输入设备产生单独线程，以便于接收信息，同时在线程里面循环的接收信息，将接收到的信息存到输入缓冲区里面。

#### 触摸屏输入事件初始化
*   直接调用触摸屏的内部函数初始化，同时在线程里面循环读取触摸屏上面的数据，将读取到的数据存入到缓冲区中。\
InputDeviceInit(void)-->TouchscreenDeviceInit(void)-->pthread_create(&tid, NULL, input_recv_thread_func, ptTmp)-->input_recv_thread_func(void *data)-->TouchscreenGetInputEvent(PInputEvent ptInputEvent)-->PutInputEventToBuffer(PInputEvent ptInputEvent)

#### 网络输入事件初始化
*   网络事件初始化首先需要在服务器端建立socket，配置接收端服务器的类型、网络IP、网络端口等，将socket与服务器进行绑定。然后创建线程，等待接收客户端发来的网络数据，将读取到的数据存入到缓冲区中。\
InputDeviceInit(void)-->NetinputDeviceInit(void)-->pthread_create(&tid, NULL, input_recv_thread_func, ptTmp)-->NetinputGetInputEvent(PInputEvent ptInputEvent)-->PutInputEventToBuffer(PInputEvent ptInputEvent)

## 初始化文字系统
### 文字系统结构体
```
typedef struct FontOpr 
{
    char *name;__字体引擎名称__
    int (*FontInit)(char *aFineName);__初始化FreeType库__
    int (*SetFontSize)(int iFontSize);__设置字体的大小__
    int (*GetFontBitMap)(unsigned int dwCode, PFontBitMap ptFontBitMap);____
    struct FontOpr *ptNext;
}FontOpr, *PFontOpr;
```
### 文字系统注册
*   注册字体引擎，方便以后使用各种字体引擎来访问各种字体格式文件，这里只注册freetype字体引擎。\
FontsRegister(void)-->FreetypeRegister(void)-->RegisterFont(PFontOpr ptFontOpr)

### 选择字体引擎以及选择字体格式
*   选择字体引擎为freetype，同时匹配输入的字体文件，使用freetype来驱动字体文件。
SelectAndInitFont(char *aFontOprName, char *aFontFileName)-->FreeTypeFontInit(char *aFineName)

## 初始化页面系统
### 页面系统结构体
```
typedef struct PageAction 
{
    char *name;__页面系统名称__
    void (*Run)(void *pParams);__运行页面系统__
    struct PageAction *ptNext;
}PageAction, *PPageAction;
```
### 页面系统注册
*   多个页面挂载到页面系统结构里面。\
PagesRegister(void)-->MainPageRegister()-->PageRegister(PPageAction ptPageAction)

### 生成页面系统
*   运行主页面系统
Page(char *name)-->MainPageRun(void *pParams)

*   获取需要绘制的主页系统的大小，读取配置文件，根据配置文件生成按钮、界面，读取输入事件。\
MainPageRun(void *pParams)-->GetDisplayBuffer(void)-->ParseConfigFile(void)-->GenerateButtons(void)-->GetInputEvent(PInputEvent ptInputEvent)