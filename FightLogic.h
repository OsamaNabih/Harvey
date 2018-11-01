#include "Improved-utility.h"
#include <stdlib.h>

void TowerAttack (ThreeCs consts, int CurrT,  castle* castle1);
void sort (enemy** A,int n );
void Damage (enemy* &Focused,int TowerPwr,int &Tvictims,int &victims);
void SetPriority (enemy* &Shielded,ThreeCs consts,int CurrTime);
void EnemyToTower(castle* castle1,int currT);
void TransferTower(castle* castle1, int num);
void MoveEnemies (castle* castle1);
void DeleteEnemy (TwoPtrLL* head,int pos);
