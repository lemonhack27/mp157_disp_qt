#include <stdio.h>
#include <page_manager.h>

void MainPageRun(void *pParams)
{
    printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
}

static PageAction g_tMainPage = {
    .name = "main",
    .Run = MainPageRun,
};

void MainPageRegister()
{
    PageRegister(&g_tMainPage);
}