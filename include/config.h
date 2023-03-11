#ifndef _CONFIG_H
#define _CONFIG_H

#define ITEMCFG_MAX_NUM 30
#define CFG_FILE "./gui.conf"

typedef struct ItemCfg {
    int index;
    char name[100];
    int bCanBeTouched;
    char command[100];
}ItemCfg, *PItemCfg;

int ParseConfigFile(void);
int GetItemCfgCount(void);
PItemCfg GetItemCfgByIndex(int index);
PItemCfg GetItemCfgByName(char *name);

#endif // !_CONFIG_H