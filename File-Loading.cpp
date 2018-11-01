#include "File-Loading.h"
#include <fstream>
void LoadInActive (Queue &head,castle* Fort,ThreeCs &constants)
{
	ifstream InputFile ("Input.txt", ios::in);
	int type;		//Cause Enum isn't understood directly
	char region;	//Same

	if (InputFile.is_open())
	{
		InputFile>>Fort->towers[0].Health>>Fort->towers[0].TTargets>>Fort->towers[0].TPower;
		Fort->OriginalHealth = Fort->towers[1].Health=Fort->towers[2].Health=Fort->towers[3].Health=Fort->towers[0].Health;
		Fort->towers[1].TTargets=Fort->towers[2].TTargets=Fort->towers[3].TTargets=Fort->towers[0].TTargets;
		Fort->towers[1].TPower=Fort->towers[2].TPower=Fort->towers[3].TPower=Fort->towers[0].TPower;
		InputFile>>constants.C1>>constants.C2>>constants.C3;	//Taking the three constants
		while (true)
		{
			enemy k;
			InputFile>>k.ID>>type>>k.ArrivalTime>>k.Health>>k.Power>>k.Reload>>region>>k.speed; //Taking enemy's stats
			region -= 'A';		//Deducting value so as to start from 0
			Fort->TotalInactive++;
			k.Type = static_cast<Etype>(type);
			k.Region = static_cast<REGION>(region);
			k.Distance = 60;
			k.FS = 0;
			k.buffed = false;
			if (k.ID == -1)		//End of input
			{
				Fort->TotalInactive--;
				return;
			}
			Enque(head,k);				//Enque the inactive enemies
		}
	}
	
}


// makes the output file based on the game state (WON OR LOSS).
void OutputFile(castle* castle1,bool GameState)
{
	int AverageFD = 0;
	int AverageKD = 0;
	ofstream OutputFile ("Output.txt", ios::out);
	if (OutputFile.is_open())
	{

		OutputFile<<"KTS\t"<<"S\t"<<"FD\t"<<"KD\t"<<"FT\t"<<endl;
		for(int i = 0; i <castle1->KilledNum; i++)
		{
			AverageFD += castle1->deadarr[i].FD;
			AverageKD += castle1->deadarr[i].KD;
			OutputFile<<castle1->deadarr[i].KTS<<"\t"<<castle1->deadarr[i].ID<<"\t"<<castle1->deadarr[i].FD<<"\t"
				<<castle1->deadarr[i].KD<<"\t"<<castle1->deadarr[i].FT<<"\t"<<endl;
		}
		OutputFile<<"T1_Total_Damage\t"<<"T2_Total_Damage\t"<<"T3_Total_Damage\t"<<"T4_Total_Damage\t"<<endl;
		OutputFile<<castle1->OriginalHealth-castle1->towers[0].Health<<"\t\t"<<castle1->OriginalHealth-castle1->towers[1].Health<<"\t\t"<<
			castle1->OriginalHealth-castle1->towers[2].Health<<"\t\t"<<castle1->OriginalHealth-castle1->towers[3].Health<<endl;
		OutputFile<<"R1_Distance\t"<<"R2_Distance\t"<<"R3_Distanc\t"<<"R4_Distance\t"<<endl;
		OutputFile<<castle1->towers[0].Unpaved<<"\t\t"<<castle1->towers[1].Unpaved<<"\t\t"<<castle1->towers[2].Unpaved<<"\t\t"
			<<castle1->towers[3].Unpaved<<"\t\t"<<endl;
		// if enemies are killed
		if(castle1->KilledNum > 0)
		{
		AverageFD /= castle1->KilledNum;
		AverageKD /= castle1->KilledNum;
		}
		else
		{
			AverageFD = AverageKD = 0; // 0 if no enemies are killed
		}
		if(GameState == false)
		{
			int Alive = 0;
			for(int i = 0; i < 4; i++)
			{
				Alive += castle1->towers[i].High.count+castle1->towers[i].Low.count;
			}
			Alive += castle1->TotalInactive;
			OutputFile<<"Game is LOSS"<<endl;
			OutputFile<<"Killed Enemies\t"<<"Alive Enemies\t"<<"Average Fight Delay\t"<<"Average Kill Delay\t"<<endl;
			OutputFile<<castle1->KilledNum<<"\t\t"<<Alive<<"\t\t"<<AverageFD<<"\t\t\t"<<AverageKD<<endl;
		}
		else
		{
			OutputFile<<"Game is WIN"<<endl;
			OutputFile<<"Killed Enemies\t"<<"Average Fight Delay\t"<<"Average Kill Delay\t"<<endl;
			OutputFile<<castle1->KilledNum<<"\t\t"<<AverageFD<<"\t\t\t"<<AverageKD<<endl;
		}

	}
}