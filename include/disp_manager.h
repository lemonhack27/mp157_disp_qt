#ifndef _DISP_MANAGER_H
#define _DISP_MANAGER_H

#include <common.h>
#include <font_manager.h>
#ifndef NULL
#define NULL (void *)0
#endif // !NULL

typedef struct DispBuff {
    int iXres;
    int iYres;
    int iBpp;
    int line_length;
    char *buff;
}DispBuff, *PDispBuff;


typedef struct DispOpr {
    char *name;
    int (*DeviceInit)(void);
    int (*DeviceExit)(void);
    int (*GetBuffer)(PDispBuff ptDispBuff);
    int (*FlushRegion)(PRegion ptRegion, PDispBuff ptDispBuff);
    struct DispOpr *ptNext;
}DispOpr, *PDispOpr;

void RegisterDisplay(PDispOpr ptDispOpr);

void DisplayInit(void);
int SelectDefaultDisplay(char *name);
int InitDefaultDisplay(void);
PDispBuff GetDisplayBuffer(void);
int PutPixel(int x, int y, unsigned int dwColor);
int FlushDisplayRegion(PRegion ptRegion, PDispBuff ptDispBuff);
void DrawFontBitMap(PFontBitMap ptFontBitMap, unsigned int dwColor);
#endif // !_DISP_MANAGER_H
