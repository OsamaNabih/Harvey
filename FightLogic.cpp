#include "FightLogic.h"
#include "Spawn-Sort.h"

void TowerAttack (ThreeCs consts, int CurrT,  castle* castle1)
{
	enemy* ptr;
	for(int i = 0;i < 4; i++)
	{
		if(castle1->towers[i].High.front == NULL && castle1->towers[i].Low.front == NULL)
			continue;
		if(castle1->towers[i].High.count <= castle1->towers[i].TTargets)
		{
			ptr = castle1->towers[i].High.front;
			while( ptr != NULL)
			{
				if(ptr->FS == 0)
					ptr->FS = CurrT;
				Damage(ptr,castle1->towers[i].TPower,castle1->towers[i].TotalKilled,castle1->towers[i].LastKilled);
				ptr = ptr->next;
			}

			if( castle1->towers[i].High.count < castle1->towers[i].TTargets)
			{
				
				ptr = castle1->towers[i].Low.front;
				for(int j = 0; j < (castle1->towers[i].TTargets-castle1->towers[i].High.count); j++)
				{
					if(ptr == NULL) 
						break;
					if(ptr->FS == 0)
						ptr->FS = CurrT;
					Damage(ptr,castle1->towers[i].TPower,castle1->towers[i].TotalKilled,castle1->towers[i].LastKilled);
					ptr = ptr->next;	
				}
			}
		}
		else 
		{

			enemy** A = new enemy*[castle1->towers[i].High.count];
			ptr = castle1->towers[i].High.front;
			for (int j = 0; j < castle1->towers[i].High.count; j++)
			{
				SetPriority(ptr,consts,CurrT);
				A[j] = ptr;
				ptr = ptr ->next;
			}
			sort(A,castle1->towers[i].High.count);
			for(int k = 0; k<castle1->towers[i].TTargets; k++)
			{
				if(A[k]->FS == 0)
				{
					A[k]->FS = CurrT;
				}
				Damage(A[k],castle1->towers[i].TPower,castle1->towers[i].TotalKilled,castle1->towers[i].LastKilled);
			}
			delete []A;
		}

	}
}




void Damage (enemy* &Focused, int TowerPwr, int &Tvictims,int &victims)
{
	float Damage = float(TowerPwr) / Focused->Distance;
	if (Focused->Type == 2)
		Damage /= 2;
	Focused->Health -= Damage;
	if (Focused->Health <= 0)
	{
		Focused->Health = 0;
		Tvictims++;
		victims++;
	}
	return;
}


void SetPriority (enemy* &Shielded,ThreeCs consts,int CurrTime)
{
	float a = (consts.C1 * (Shielded->Power / Shielded->Distance));
	float b = consts.C2 / ((CurrTime - Shielded->ArrivalTime) % (Shielded->Reload + 1) + 1);
	float c = Shielded->Health * consts.C3;
	Shielded->Priority = a + b + c;
	return;
}


void MoveEnemies (castle* castle1)
{
	//if a paver is reloading he should walk into the unpaved region 1 distance at each time step
	for (int i = 0; i < 4; i++)
	{
		enemy* ptr = castle1->towers[i].High.front;
		while (ptr != NULL)
		{
			if (ptr->Distance == 2)
				break;
			else if ((ptr->Distance - ptr->speed) > castle1->towers[i].Unpaved)
				ptr->Distance -= ptr->speed;
			else
				ptr->Distance = castle1->towers[i].Unpaved;

			ptr = ptr->next;
		}
		ptr = castle1->towers[i].Low.front;
		while (ptr != NULL)
		{
			if (ptr->Distance == 2)
				break;
			if (ptr->Type == 0)
			{
				if (ptr->Distance - ptr->Power > castle1->towers[i].Unpaved)
					ptr->Distance -= ptr->Power;
				else
					ptr->Distance = castle1->towers[i].Unpaved;
			}
			else
			{
				if ( (ptr->Distance - ptr->speed) > castle1->towers[i].Unpaved)
					ptr->Distance -= ptr->speed;
				else
					ptr->Distance = castle1->towers[i].Unpaved;
			}
			ptr = ptr->next;
		}
	}
}



//NEW FUNCTION: it passes on every enemy in every list and damages the tower if the enemy is ready to fire, if it is
//not then it ignores this enemy, if it is the first shot of this enemy, it updates the FS variable.
void EnemyToTower(castle* castle1,int currT)
{
	enemy* HighPtr;
	enemy* LowPtr;
	enemy* traversePtr;
	for (int i= 0; i < 4; i++)
	{
		LowPtr = castle1->towers[i].Low.front;
		while(LowPtr != NULL)
		{
			if((currT - LowPtr->ArrivalTime) % LowPtr->Reload == 0) 
			{
				if (LowPtr->Type == 3)			//If enemy is buffer type
				{
					traversePtr = castle1->towers[i].Low.front;		//Set a ptr to traverse with on the entire low priority list
					while (traversePtr != NULL)						//while list not empty
					{
						if ((traversePtr->Distance - LowPtr->Distance <= 4 && traversePtr->Distance - LowPtr->Distance >= 0) 
							|| (traversePtr->Distance - LowPtr->Distance < 0 && traversePtr->Distance - LowPtr->Distance >= -4))
							//If enemy is within 4 units of distance of the buffer, set his state to buffed
							traversePtr->buffed = true;
						else
							traversePtr->buffed = false;
						traversePtr = traversePtr->next;
					}
					traversePtr = castle1->towers[i].High.front;	//Now traverse the high priority to determine their state
					while (traversePtr != NULL)
					{
						if ((traversePtr->Distance - LowPtr->Distance <= 4 && traversePtr->Distance - LowPtr->Distance >= 0) 
							|| (traversePtr->Distance - LowPtr->Distance < 0 && traversePtr->Distance - LowPtr->Distance >= -4))
							traversePtr->buffed = true;
						else
							traversePtr->buffed = false;
						traversePtr = traversePtr->next;
					}
				}
				if (LowPtr->Type == 4)			//If enemy is HARVEY type
				{
					srand(currT);
					int x = rand() % 2;			//Flip a coin! (Harvey makes his own luck)
					if (x == 1)					//If result is one, Harvey will damage the tower
						castle1->towers[i].Health -= (1/(float)LowPtr->Distance) * LowPtr->Power;
					else						//If 1st coin flip was zero, Harvey flips another coin to determine his fate!
					{
						srand(currT + 20);		//Harvey has lost the first flip, and must decide between shooting himself, or a friend of his
						x = rand() % 2;
						if (x == 1)				//If second flip is one, Harvey hits the highest priority enemy
						{
							if (castle1->towers[i].High.front != NULL)		//If there are shielded enemies
							{
								traversePtr = castle1->towers[i].High.front;
								while (traversePtr->next != NULL && traversePtr->Health == 0)
									traversePtr = traversePtr->next;		//Reach the first enemy who isn't already dead
								traversePtr->Health -= LowPtr->Power / 5;	//Damage him for one fifth of Harvey's power
								if (traversePtr->Health < 0)
									traversePtr->Health = 0;
							}
							else if (castle1->towers[i].Low.front != NULL)	//If no shielded exist, get the first low priorty enemy
							{
								traversePtr = castle1->towers[i].Low.front;
								while (traversePtr->next != NULL && traversePtr->Health == 0)
									traversePtr = traversePtr->next;		//Reach the first low priority enemy that isn't dead
								traversePtr->Health -= LowPtr->Power / 5;	//Damage him for one fifth of Harvey's power
								if (traversePtr->Health < 0)
									traversePtr->Health = 0;
							}
						}
						else				//If second flip is zero
						{
							LowPtr->Health -= LowPtr->Power / 5;	//Harvey has failed and must damage himself as punishment!
							if (LowPtr->Health < 0)
								LowPtr->Health = 0;
						}
					}
				}
				if(LowPtr->Type == 0 && LowPtr->Distance == castle1->towers[i].Unpaved)
				{
					castle1->towers[i].Unpaved -= LowPtr->Power;
					if(castle1->towers[i].Unpaved < 2) 
						castle1->towers[i].Unpaved = 2;
					LowPtr->Distance = castle1->towers[i].Unpaved;
				}
				
				else
				{
					if (LowPtr->buffed)
						castle1->towers[i].Health -= (1/(float)LowPtr->Distance) * (2*LowPtr->Power);
					else
						castle1->towers[i].Health -= (1/(float)LowPtr->Distance) * LowPtr->Power;
				}
			}
			LowPtr= LowPtr->next;
		}
		HighPtr = castle1->towers[i].High.front;
		while(HighPtr != NULL)
		{
			if( (currT -HighPtr->ArrivalTime) % HighPtr->Reload == 0) 
			{
				if (HighPtr->buffed)
					castle1->towers[i].Health -= (2/(float)HighPtr->Distance) * (2*HighPtr->Power);
				else
					castle1->towers[i].Health -= (1/(float)HighPtr->Distance) * HighPtr->Power;
			}
			HighPtr = HighPtr->next;
		} // end of while loop
		// end of for loop
	}
	for(int i = 0; i < 4; i++)
	{
		if(castle1->towers[i].Health < 0)
			{
				castle1->towers[i].Health=0;
				TransferTower(castle1, i);
			}
	}
}

// NEW FUNCTION: it appends the dead tower's lists to the next alive tower, then it calls merge sort to sort the 
// Low priority enemies.
// num is for the tower index that will get transfered
void TransferTower(castle* castle1, int num)
{
	int i = num + 1;
	while(castle1->towers[i % 4].Health <= 0)
	{
		if((i - num) == num + 1)
			return;
		i++;
	}
	// When transferring towers we set every enemy's region to the new region
	enemy* ptr;
	ptr=castle1->towers[num].Low.front;
	while(ptr != NULL)
	{
		ptr->Region = static_cast<REGION>(i % 4);
		ptr = ptr->next;
	}
	ptr=castle1->towers[num].High.front;
	while(ptr != NULL)
	{
		ptr->Region = static_cast<REGION>(i % 4);
		ptr = ptr->next;
	}
	if(castle1->towers[i % 4].High.rear == NULL)
	{
		castle1->towers[i % 4].High.front = castle1->towers[num].High.front;
		castle1->towers[i % 4].High.rear = castle1->towers[num].High.rear;
		castle1->towers[i % 4].High.count += castle1->towers[num].High.count;
	}
	else 
	{
		castle1->towers[i % 4].High.rear->next = castle1->towers[num].High.front;
		castle1->towers[i % 4].High.rear = castle1->towers[num].High.rear;
		castle1->towers[i % 4].High.count += castle1->towers[num].High.count;
	}
	if(castle1->towers[i % 4].Low.rear == NULL)
	{
		castle1->towers[i % 4].Low.front = castle1->towers[num].Low.front;
		castle1->towers[i % 4].Low.rear = castle1->towers[num].Low.rear;
		castle1->towers[i % 4].Low.count += castle1->towers[num].Low.count;
	}
	else
	{
		castle1->towers[i % 4].Low.rear->next = castle1->towers[num].Low.front;
		castle1->towers[i % 4].Low.rear = castle1->towers[num].Low.rear;
		castle1->towers[i % 4].Low.count += castle1->towers[num].Low.count;
	}
	
	castle1->towers[num].High.front = NULL;
	castle1->towers[num].High.rear = NULL;
	castle1->towers[num].High.count = 0;
	castle1->towers[num].Low.count = 0;
	castle1->towers[num].Low.front = NULL;
	castle1->towers[num].Low.rear = NULL;
	MergeSort(&castle1->towers[(num + 1) % 4].Low);
	ApplyRear(&castle1->towers[(num + 1) % 4].Low);
}



void DeleteEnemy (TwoPtrLL* head,int pos)
{	
	enemy* ptr = NULL;
	enemy* prevptr = NULL; 
	ptr = prevptr = head->front;
	int count = 0;
	if (pos == 0)
	{
		ptr = head->front;
		head->front = head->front->next;
		delete ptr;
		if (head->front == NULL)
			head->rear = NULL;
		return;
	}
	else
	{
		while (ptr != NULL)
		{
			if (count == pos)
			{
				prevptr->next = ptr->next;
				if (prevptr->next == NULL)
					head->rear = prevptr;
				delete ptr;
				return;
			}
			else
			{
				prevptr = ptr;
				ptr = ptr->next;
			}
			count++;
		}
		return;
	}
}