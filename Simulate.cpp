#include "Simulate.h"

void Simulate()
{
	int mode = ModeSelect();
	castle* Fort= new castle;
	ThreeCs constants;
	Queue Q;
	int time = 1;
	InitializeQ (Q); 
	InitializeAll (Fort);
	LoadInActive (Q,Fort,constants);
	Fort->deadarr = new DeadEnemy[Fort->TotalInactive];
	bool win,loss;
	while (true)
	{
		if(mode == 1 || mode == 2)
		{
			cout<<"Timestep is  "<<time<<endl;
			PrintAll(Fort,time);
			if(mode == 1)
			{
				if (time == 1)
					cin.ignore();
				cin.ignore();
			}

			else
				Sleep(1000);
		}
		Spawn (Fort,Q,time); //check the Spawn-Kill
		MoveEnemies(Fort);
		TowerAttack (constants,time,Fort);
		EnemyToTower(Fort,time);
		KillThemAll(Fort,time);
		win = CheckWin(Fort,Q);
		loss = CheckLoss(Fort);
		if (win)
		{
			OutputFile(Fort,true);
			if (mode != 3)
				PrintAll(Fort,time);
			delete []Fort->deadarr;
			delete Fort;
			break;
		}
		else if(loss)
		{
			OutputFile(Fort,false);
			if (mode != 3)
				PrintAll(Fort,time);
			delete []Fort->deadarr;
			delete Fort;
			break;
		}
		time++;	
	}
}
