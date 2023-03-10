#include <ui.h>

static int DefaultOnDraw(struct Button *ptButton, PDispBuff ptDispBuff)
{
    DrawRegion(&ptButton->tRegion, BUTTON_DEFAULT_COLOR);

    DrawTextRegionCentral(ptButton->name, &ptButton->tRegion, BUTTON_TEXT_COLOR);

    FlushDisplayRegion(&ptButton->tRegion,ptDispBuff);
    return 0;
}

static int DefaultOnPressed(struct Button *ptButton, PDispBuff ptDispBuff, PInputEvent ptInputEvent)
{
    unsigned int dwColor = BUTTON_DEFAULT_COLOR;

    ptButton->status = !ptButton->status;
    if (ptButton->status)
    {
        dwColor = BUTTON_PRESSED_COLOR;
    }

    DrawRegion(&ptButton->tRegion, dwColor);

    DrawTextRegionCentral(ptButton->name, &ptButton->tRegion, BUTTON_TEXT_COLOR);

    FlushDisplayRegion(&ptButton->tRegion,ptDispBuff);
    return 0;
}

void InitButton(PButton ptButton, char *name ,PRegion ptRegion, ONDRAW_FUNC OnDraw, ONPRESSED_FUNC OnPressed)
{
    ptButton->status = 0;
    ptButton->name = name;
    ptButton->tRegion = *ptRegion;
    ptButton->OnDraw = OnDraw ? OnDraw : DefaultOnDraw;
    ptButton->OnPressed = OnPressed ? OnPressed : DefaultOnPressed;
}