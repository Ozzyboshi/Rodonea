
#include <ace/generic/main.h>
#include <ace/managers/key.h>
#include <ace/managers/state.h>


// Without it compiler will yell about undeclared gameGsCreate etc
#include "main.h"
#include "demointro.h"
#include "demointro2.h"
#include "rodonea.h"

tStateManager *g_pGameStateManager = 0;
tState *g_pGameStates[GAME_STATE_COUNT] = {0};

void genericCreate(void)
{
  keyCreate(); // We'll use keyboard
               // Initialize gamestate
  g_pGameStateManager = stateManagerCreate();
  g_pGameStates[0] = stateCreate(introGsCreate, introGsLoop, introGsDestroy, 0, 0, 0);
  g_pGameStates[1] = stateCreate(introGsCreate2, introGsLoop2, introGsDestroy2, 0, 0, 0);
  g_pGameStates[2] = stateCreate(gameGsCreate, gameGsLoop, gameGsDestroy, 0, 0, 0);
 // g_pGameState = stateCreate(gameGsCreate, gameGsLoop, gameGsDestroy, 0, 0, 0);

//g_pGameState = stateCreate(introGsCreate, introGsLoop, introGsDestroy, 0, 0, 0);

  statePush(g_pGameStateManager, g_pGameStates[0]);
}

void genericProcess(void)
{
  // Here goes code done each game frame
  keyProcess();
  stateProcess(g_pGameStateManager);
}

void genericDestroy(void)
{
  stateManagerDestroy(g_pGameStateManager);
  stateDestroy(g_pGameStates[0]);
  stateDestroy(g_pGameStates[1]);

  // Here goes your cleanup code
  keyDestroy(); // We don't need it anymore
}
