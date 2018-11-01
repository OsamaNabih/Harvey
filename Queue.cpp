#include "Queue.h"

// Initializing a Queue to null
void InitializeQ (Queue &x)
{
	x.rear = NULL;
	x.front = NULL;
}


//Initializing AllRegions to null
void InitializeAll (castle* castle1)
{
	
	castle1->Xstrt = CastleXStrt;
	castle1->Ystrt = CastleYStrt;
	castle1->W = CastleWidth;
	castle1->L = CastleLength;
	for(int i = 0; i < 4; i++)
	{
		castle1->towers[i].High.front=NULL;
		castle1->towers[i].High.rear=NULL;
		castle1->towers[i].High.count=0;
		castle1->towers[i].Low.front=NULL;
		castle1->towers[i].Low.rear=NULL;
		castle1->towers[i].Low.count=0;
		castle1->towers[i].Unpaved = 30;
		castle1->towers[i].TotalKilled = 0;
		castle1->towers[i].LastKilled = 0;
	}
	castle1->KilledNum=0;
	castle1->TotalInactive=0;
}
void Enque (Queue &x,enemy val)
{
	enemy* temp = new enemy;
	*temp = val;
	temp->next = NULL;
	if (x.rear == NULL && x.front == NULL)
		x.front = x.rear = temp;
	else
	{
		x.rear->next = temp;
		x.rear = temp;
	}
}

void Deque (Queue &x,TwoPtrLL &A)
{
	if (A.rear == NULL)
	{
		A.front = A.rear = x.front;
		x.front = x.front->next;
	}
	else
	{
		A.rear->next = x.front;
		x.front = x.front->next;
		A.rear = A.rear->next;
	}
	A.rear->next = NULL;
	return;
}

