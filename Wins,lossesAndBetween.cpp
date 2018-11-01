#include "Wins-Losses.h"


// Fills the dead enemy array with the things needed in the output folder.
void TakeStats(castle* castle1, enemy* en,int currT)
{
	castle1->deadarr[castle1->KilledNum].FD = en->FS - en->ArrivalTime;
	castle1->deadarr[castle1->KilledNum].KTS = currT;
	castle1->deadarr[castle1->KilledNum].KD= currT - en->FS;
	castle1->deadarr[castle1->KilledNum].ID = en->ID;
	castle1->deadarr[castle1->KilledNum].FT= castle1->deadarr[castle1->KilledNum].FD + castle1->deadarr[castle1->KilledNum].KD;
}


// checks if the game is won (all enemies are deleted).
bool CheckWin(castle* castle1, Queue &x)
{
	if(x.front == NULL && castle1->towers[0].High.count == 0 && castle1->towers[0].Low.count == 0 && 
	castle1->towers[1].High.count == 0 && castle1->towers[1].Low.count == 0 &&
	castle1->towers[2].High.count == 0 && castle1->towers[2].Low.count == 0 &&
	castle1->towers[3].High.count == 0 && castle1->towers[3].Low.count == 0)
		return true;
	return false;
}


// checks if the game is lost (all towers are destroyed).
bool CheckLoss(castle* castle1)
{
	if(castle1->towers[0].Health == 0 && castle1->towers[1].Health == 0 && castle1->towers[2].Health == 0 &&
		castle1->towers[3].Health == 0)
		return true;
	return false;
}



//Kills enemies that have 0 health.
void KillThemAll(castle* castle1,int currT)
{
	enemy* ptr;
	for(int i = 0; i < 4; i++)
	{
		if(castle1->towers[i].High.front == NULL && castle1->towers[i].Low.front == NULL) // if null, go to next tower.
			continue;
		ptr = castle1->towers[i].High.front;
		int j = 0;
		while( ptr != NULL)
		{
			if(ptr->Health == 0)
			{
				TakeStats(castle1,ptr,currT); // takes stats first
				ptr=ptr->next;
				DeleteEnemy(&castle1->towers[i].High,j); // kills enemy based on position
				castle1->towers[i].High.count--;
				castle1->KilledNum++; // increases killed num
			}
			else
			{
				ptr = ptr->next;
				j++;	
			}
		}
		ptr = castle1->towers[i].Low.front;
		j = 0;
		while(ptr != NULL)
		{
			if(ptr->Health == 0)
			{
				TakeStats(castle1,ptr,currT);
				ptr=ptr->next;
				DeleteEnemy(&castle1->towers[i].Low,j);
				castle1->towers[i].Low.count--;
				castle1->KilledNum++;
			}
			else
			{
				ptr = ptr->next;
				j++;	
			}
		}
	}
}



