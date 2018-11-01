#pragma once

#include "Improved-utility.h"
#include<iostream>
using namespace std;
// Initializing a Queue to null
void InitializeQ (Queue &x);
//Initializing castle
void InitializeAll (castle* castle);
void Enque (Queue &x,enemy val);
void Deque (Queue &x,TwoPtrLL &A);
