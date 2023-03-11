#include <stdio.h>
#include <config.h>
#include <string.h>
static ItemCfg g_tItemCfgs[ITEMCFG_MAX_NUM];
static int g_iItemCfgCount = 0;

int ParseConfigFile(void)
{
    FILE *fp;
    char buf[100];
    char *p = buf;

    fp = fopen(CFG_FILE, "r");
    if (!fp)
    {
        printf("cannot open\n");
        return -1;
    }

    while (fgets(buf, 100, fp))
    {
        buf[99] = '\0';
        p = buf;
        while (*p == ' ' || *p == '\t');
            p++;
        
        if (*p == '#')
            continue;

        g_tItemCfgs[g_iItemCfgCount].command[0] = '\0';
        g_tItemCfgs[g_iItemCfgCount].index = g_iItemCfgCount;
        sscanf(p, "%s %d %s", g_tItemCfgs[g_iItemCfgCount].name, &g_tItemCfgs[g_iItemCfgCount].bCanBeTouched, g_tItemCfgs[g_iItemCfgCount].command);
        g_iItemCfgCount++;
    }
    fclose(fp);
    return 0;
}

int GetItemCfgCount(void)
{
    return g_iItemCfgCount;
}

PItemCfg GetItemCfgByIndex(int index)
{
    if (index < g_iItemCfgCount)
        return &g_tItemCfgs[index];
    else
        return NULL;
}

PItemCfg GetItemCfgByName(char *name)
{
    int i;
    for (i = 0; i < g_iItemCfgCount; i++)
    {
        if (strcmp(name, g_tItemCfgs[i].name) == 0)
            return &g_tItemCfgs[i];
    }
    return NULL;
}