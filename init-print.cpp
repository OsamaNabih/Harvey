#include "Init-print.h"

int ModeSelect ()
{
	int mode;
	cout<<"Enter Mode\n";
	cin>>mode;
	return mode;
}



void PrintAll(castle* castle1,int currT)
{
	int num = 0;
	for(int i = 0; i < 4; i++)
		 num += castle1->towers[i].High.count + castle1->towers[i].Low.count;
	enemy** enemarray = new enemy*[num];
	num = 0; 
	enemy* ptr;
	for(int i = 0; i < 4; i++)
	{
		ptr = castle1->towers[i].High.front;
		while(ptr != NULL)
		{
			enemarray[num] = ptr;
			ptr = ptr->next; 
			num++;
		}
		ptr = castle1->towers[i].Low.front;
		while(ptr != NULL)
		{
			enemarray[num] = ptr;
			ptr = ptr->next; 
			num++;
		}
	}
	DrawCastle(*castle1,currT);
	DrawEnemies(enemarray,num);
	cout<<"\nTotal current enemies\t";
	for (int i = 0; i < 4; i++)
	{
		char c = i + 65;
		cout<<c<<" = "<<castle1->towers[i].High.count + castle1->towers[i].Low.count<<"\t";
	}
	cout<<"\nLast time step kill\t";
	for (int i = 0; i < 4; i++)
	{
		char c = i + 65;
		cout<<c<<" = "<<castle1->towers[i].LastKilled<<"\t";
		castle1->towers[i].LastKilled = 0;
	}
	cout<<"\nTotal killed by\t\t";
	for (int i = 0; i < 4; i++)
	{
		char c = i + 65;
		cout<<c<<" = "<<castle1->towers[i].TotalKilled<<"\t";
	}
	cout<<"\nUnpaved\t\t\t";
	for (int i = 0; i < 4; i++)
	{
		char c = i + 65;
		cout<<c<<" = "<<castle1->towers[i].Unpaved<<"\t";
	}
}

