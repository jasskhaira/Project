//============================================================================
// Name        : BB-pixy.cpp
// Author      : Jaspreet Singh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include"Pixy2BBB.h"
using namespace std;

Pixy2BBB pixy;
int main() {

	int i;
	pixy.init();
	pixy.setLamp(1,0);

while(1){
	pixy.ccc.getBlocks();

	if(pixy.ccc.numBlocks)
	{
		printf("Detected");
		printf("%d",pixy.ccc.numBlocks);
		for(i=0;i<pixy.ccc.numBlocks;i++);{
			cout<<"blocks"<<i<<":";
			pixy.ccc.blocks[i].print();
		}


	}}

	return 0;
}
