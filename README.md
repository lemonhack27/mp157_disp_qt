# mp157_disp_qt
使用stm32mp157开发板实现QT显示功能
LCD存在固定参数和可变参数，二者不一样，使用的时候选择错误会出现输出字符错位情况
需要交叉编译tslib库才能成功包含头文件tslib.h