#include "FightLogic.h"
#include "Spawn-Sort.h"

void TakeStats(castle* castle1, enemy* en,int currT);
bool CheckWin(castle* castle1, Queue &x);
bool CheckLoss(castle* castle1);
void KillThemAll(castle* castle1,int currT);
