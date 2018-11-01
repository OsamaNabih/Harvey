#pragma once
#include "File-loading.h"
#include<iostream>
using namespace std;
void Spawn (castle* castle,Queue &head, int CurrT);
void MergeSort(TwoPtrLL* low);
void Split(TwoPtrLL* MainList, TwoPtrLL* List, TwoPtrLL* OtherList );
enemy* Merge(enemy* half,enemy* otherHalf);
void ApplyRear(TwoPtrLL* low);
