#include <cstdlib>
#include <iostream>
#include "ship.h"

ship::ship(std::string n){
	name=n;
	if(name=="submarine"){
		length=1;
	}
	else if(name=="destroyer"){
		length=2;
	}
	else if(name=="cruiser"){
		length=3;
	}
	else if(name=="battleship"){
		length=4;
	}
	else if(name=="carrier"){
		length=5;
	}
	else if(name=="cargo"){
		length=6;
	}
	else if(name=="tanker"){
		length=7;
	}
	else if(name=="unknown"){
		//A length of 0 is used to represent unknown values, the main code will check for this
		length=0;
	}
}
int ship::getLength()const{
	return length;
}
int ship::getRow()const{
	return row;
}
int ship::getColumn()const{
	return column;
}
void ship::setRow(int r){
	row=r;
}
void ship::setColumn(int c){
	column=c;
}
void ship::setOrientation(std::string o){
	orientation=o;
}
void ship::print()const{
	//Print out the ship's name, position, and orientation
	std::cout<<name;
	for(unsigned int i=0;i<11-name.size();++i){
		std::cout<<' ';
	}
	std::cout<<row<<' '<<column<<' '<<orientation<<std::endl;
}
bool longer(const ship& ship1, const ship& ship2){
	//Sort the ships from longest to shortest
	return ship1.getLength()>ship2.getLength();
}
bool topleft(const ship& ship1, const ship& ship2){
	//Sort the ships from first to last row, followed by first to last column
	if(ship1.getRow()==ship2.getRow()){
		return ship1.getColumn()<ship2.getColumn();
	}
	else{
		return ship1.getRow()<ship2.getRow();
	}
}