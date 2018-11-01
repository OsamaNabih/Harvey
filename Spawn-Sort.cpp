#include "Spawn-Sort.h"

//Changed this functions to sort it to the different lists, changed the passed parrameters to adjust. Even numbers are high priority lists, odds are low.
void Spawn (castle* castle,Queue &head, int CurrT)	
{											//Sorts them into a linked list according to their respective region and type
	while (true)							
	{
		if (head.front == NULL)					//In case the queue became empty
			return;
		if (head.front->ArrivalTime == CurrT) //Check if enemy is spawning this turn
		{
			if (head.front->Type == 2)
			{
				for(int i = 0; i < 4; i++)
				{
						if(i == head.front->Region)
						{
							if(castle->towers[i].Health > 0 )
							{
								castle->towers[i].High.count++; //adds to the counter of the high list.
								castle->TotalInactive--;
								Deque(head,castle->towers[i].High);
								break;
							}
							else
							{
								int j = i + 1;
								while(castle->towers[j % 4].Health < 0)
								{
									if(j - 4 == i) 
										return;
									j++; 
								}
								castle->towers[j % 4].High.count++; //adds to the counter of the high list.
								castle->TotalInactive--;
								Deque(head,castle->towers[j % 4].High);
								break;
							}
						}
				}
			}
			else
			{
				for(int i = 0; i < 4; i++)
				{
						if(i == head.front->Region)
						{
						
							if(castle->towers[i].Health > 0 )
							{
								castle->towers[i].Low.count++; //adds to the counter of the high list.
								castle->TotalInactive--;
								Deque(head,castle->towers[i].Low);
								break;
							}
							else
							{
								int j = i + 1;
								while(castle->towers[j % 4].Health < 0)
								{
									if(j - 4 == i) 
										return;
								j++; 
								}
								castle->towers[j % 4].Low.count++; //adds to the counter of the high list.
								castle->TotalInactive--;
								Deque(head,castle->towers[j % 4].Low);
								break;
							}
						}
					}
			}
		}
		else
			return;
	}
}


void sort (enemy** A,int n )
{
	if (n == 1)
		return;
	enemy* temp= NULL;
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n - 1; j++)
		{
		
			if(A[j]->Priority < A[j+1]->Priority)
			{
			    temp = A[j];
				A[j] = A[j+1];
				A[j+1] = temp;
			}
		}
	}
	return;
}


void MergeSort(TwoPtrLL* low)
{
	// if the list is null or 1 return
	if(low->front == NULL || low->front->next == NULL)
		return;
	// making the two new lists for the split
	TwoPtrLL* half= new TwoPtrLL;
	half->front = half->rear = NULL;
	half->count=0;
	TwoPtrLL* otherHalf= new TwoPtrLL;
	otherHalf->front=otherHalf->rear=NULL;
	otherHalf->count=0;
	//SPLITING, will output half the list in the half variable and the other in the other variable
	Split(low, half, otherHalf);
	// recursively calls itself with the first half, returns if the list is 1
	MergeSort(half);
	// recursively calls itself with the second half, returns if the list is 1
		MergeSort(otherHalf);
	// MERGES the two halfs that was passed, puts the rear NULL (we have a function that assigns the rear later in this file).
	low->front = Merge(half->front,otherHalf->front);
	low->rear=NULL;
	// deletes the dynamically allocated lists.
	delete half;
	delete otherHalf;
}


void Split(TwoPtrLL* MainList, TwoPtrLL* List, TwoPtrLL* OtherList )
{
	// if the list is 1 it will return, nothing to split.
	if(MainList->count == 1)
		return;
	int count= MainList->count/2; 
	// CORNER CASE, if the list is only two variables
	if(MainList->count == 2)
	{
		// the list is front and rear, so the first half's front and rear will be the main list's front and the count =1;
		List->front=List->rear= MainList->front;
		List->count=1;
		// the list is front and rear, so the SECOND half's front and rear will be the main list's REAR and the count =1;
		OtherList->front=OtherList->rear= MainList->front->next;
		OtherList->count=1;
		// both lists next will be NULL
		List->front->next=NULL;
		OtherList->front->next = NULL;
		return;
	}
	else
	{
		// makes 2 temp enemy variables, lasten points to the first enemy and new points to the second one
		enemy* lasten=MainList->front;
		enemy* newen=lasten->next;
		int i =1; // a counter to detect when to stop, STARTS AT 1 NOT 0.
		while(i<count)
		{
			// for each count next the variables.
			lasten=lasten->next;
			newen=newen->next;
			i++;
		}
		// EXAMPLE CASE: the list is 4 nodes, count is 2, first loop lasten will point to second enemy, newen will point to 3rd enemy.
		// since the count is 2 and after the first loop the i will be 2 then it will only enter once
		enemy* temp = MainList->front; //temp variable to the head of the MAIN LIST.
		lasten->next=NULL; //Last enemy is now at second enemy, to split the list in half we point the second's next to NULL
		List->front= temp; // the first list's front is the main's front
		List->count=(MainList->count/2); // count is count/2, if it is 4 it will be 2, if 5 also 2.
		List->rear= lasten; // rear is equal to lasten.
		// SINCE, newen points to the third enemy, which is the second half of the list it will point at it.
		OtherList->front=newen; 
		OtherList->count= MainList->count - MainList->count/2; // count is count-count/2, if it was 4 then it will be 2, if it was 5 then it will
															   // be 3 as the first half will be 2
		OtherList->rear=MainList->rear; // the list's rear is the main list's rear.
		return;
	}
}


enemy* Merge(enemy* half,enemy* otherHalf)
{
	// this function will recieve 2 lists, it's purpose is to link them sorted.
	enemy* result;
	if(half == NULL) // if list 1 is null return list 2
		return otherHalf;
	if(otherHalf == NULL) // same
		return half;
	if(half->ArrivalTime <= otherHalf->ArrivalTime) // if list 1 is less than or equal to list 2 
	{
		result= half; // result is the new list, so it will point at the lowest.
		result->next= Merge(half->next,otherHalf); // calls the function recursively to loop on both itself and the other list until one
												   // or both are null
	}
	else
	{	// same what happens above
		result= otherHalf;
		result->next= Merge(otherHalf->next,half);
	}

	return(result); // returns the sorted list.
}


void ApplyRear(TwoPtrLL* low)
{
	enemy* temp = low->front;
	if (temp == NULL)
		return;
	while(temp->next != NULL)
		temp = temp->next;
	low->rear= temp;
}



