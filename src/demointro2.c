#include "demointro2.h"

#include <ace/managers/game.h>                  // For using gameClose
#include <ace/managers/system.h>                // For systemUnuse and systemUse
#include <ace/managers/viewport/simplebuffer.h> // Simple buffer
#include <ace/utils/palette.h>
#include <ace/managers/blit.h>

// Assets
#include "../_res/rodoflash.h"

static tVPort *s_pVpMain; // Viewport for playfield
static tSimpleBufferManager *s_pMainBuffer;

static tView *s_pView; // View containing all the viewports

static tVPort *s_pVpMain; // Viewport for playfield
static tSimpleBufferManager *s_pMainBuffer;
void copyToMainBpl2(const unsigned char *, const UBYTE, const UBYTE);

void introGsCreate2(void)
{
    // Create a view - first arg is always zero, then it's option-value
    s_pView = viewCreate(0,
                         TAG_VIEW_GLOBAL_CLUT, 1, // Same Color LookUp Table for all viewports
                         TAG_END);                // Must always end with TAG_END or synonym: TAG_DONE

    // Now let's do the same for main playfield
    s_pVpMain = vPortCreate(0,
                            TAG_VPORT_VIEW, s_pView,
                            TAG_VPORT_BPP, 3, // 3 bits per pixel, 8 colors
                                              // We won't specify height here - viewport will take remaining space.
                            TAG_END);
    s_pMainBuffer = simpleBufferCreate(0,
                                       TAG_SIMPLEBUFFER_VPORT, s_pVpMain, // Required: parent viewport
                                       TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
                                       TAG_END);

    s_pVpMain->pPalette[0] = 0x0000;
    s_pVpMain->pPalette[1] = 0x0000;
    s_pVpMain->pPalette[2] = 0x0000;
    s_pVpMain->pPalette[3] = 0x0000;
    s_pVpMain->pPalette[4] = 0x0000;
    s_pVpMain->pPalette[5] = 0x0000;
    s_pVpMain->pPalette[6] = 0x0000;
    s_pVpMain->pPalette[7] = 0x0000;

    copyToMainBpl2(rodoflash_data, 0, 3);

    // We don't need anything from OS anymore
    //systemUnuse();
    //Execute("RUN casentino2020");

    // Load the view
    viewLoad(s_pView);
}

void introGsLoop2(void)
{
    static int iFrameNo = 0;
    static BYTE bDimCounter = 0;
    static BYTE bDimCounter2 = 15;
    if (bDimCounter <= 15)
    {
        s_pVpMain->pPalette[0] = paletteColorDim(0x0321, bDimCounter);
        s_pVpMain->pPalette[1] = paletteColorDim(0x0986, bDimCounter);
        s_pVpMain->pPalette[2] = paletteColorDim(0x0CB9, bDimCounter);
        s_pVpMain->pPalette[3] = paletteColorDim(0x0653, bDimCounter);
        s_pVpMain->pPalette[4] = paletteColorDim(0x0BA7, bDimCounter);
        s_pVpMain->pPalette[5] = paletteColorDim(0x0875, bDimCounter);
        s_pVpMain->pPalette[6] = paletteColorDim(0x0442, bDimCounter);
        s_pVpMain->pPalette[7] = paletteColorDim(0x0EDA, bDimCounter);

        if ((iFrameNo % 10) == 0)
            bDimCounter++;
    }

    if (iFrameNo > 400)
    {
        if (bDimCounter2 >= 0)
        {
            s_pVpMain->pPalette[0] = paletteColorDim(0x0321, bDimCounter2);
            s_pVpMain->pPalette[1] = paletteColorDim(0x0986, bDimCounter2);
            s_pVpMain->pPalette[2] = paletteColorDim(0x0CB9, bDimCounter2);
            s_pVpMain->pPalette[3] = paletteColorDim(0x0653, bDimCounter2);
            s_pVpMain->pPalette[4] = paletteColorDim(0x0BA7, bDimCounter2);
            s_pVpMain->pPalette[5] = paletteColorDim(0x0875, bDimCounter2);
            s_pVpMain->pPalette[6] = paletteColorDim(0x0442, bDimCounter2);
            s_pVpMain->pPalette[7] = paletteColorDim(0x0EDA, bDimCounter2);

            if ((iFrameNo % 10) == 0)
                bDimCounter2--;
        }

        //gameExit();return ;
    }
    if (bDimCounter2 < 0)
    {
        statePush(g_pGameStateManager, g_pGameStates[2]);
    }
    iFrameNo++;
    viewUpdateCLUT(s_pView);
    vPortWaitForEnd(s_pVpMain);
}

void introGsDestroy2(void)
{
    // Cleanup when leaving this gamestate
    systemUse();

    // This will also destroy all associated viewports and viewport managers
    viewDestroy(s_pView);
}

// Function to copy data to a main bitplane
// Pass ubMaxBitplanes = 0 to use all available bitplanes in the bitmap
void copyToMainBpl2(const unsigned char *pData, const UBYTE ubSlot, const UBYTE ubMaxBitplanes)
{
    UBYTE ubBitplaneCounter;
    for (ubBitplaneCounter = 0; ubBitplaneCounter < s_pMainBuffer->pBack->Depth; ubBitplaneCounter++)
    {
        blitWait();
        g_pCustom->bltcon0 = 0x09F0;
        g_pCustom->bltcon1 = 0x0000;
        g_pCustom->bltafwm = 0xFFFF;
        g_pCustom->bltalwm = 0xFFFF;
        g_pCustom->bltamod = 0x0000;
        g_pCustom->bltbmod = 0x0000;
        g_pCustom->bltcmod = 0x0000;
        g_pCustom->bltdmod = 0x0000;
        g_pCustom->bltapt = (UBYTE *)((ULONG)&pData[40 * 256 * ubBitplaneCounter]);
        g_pCustom->bltdpt = (UBYTE *)((ULONG)s_pMainBuffer->pBack->Planes[ubBitplaneCounter] + (40 * ubSlot));
        g_pCustom->bltsize = 0x4014;
        if (ubMaxBitplanes > 0 && ubBitplaneCounter + 1 >= ubMaxBitplanes)
            return;
    }
    return;
}