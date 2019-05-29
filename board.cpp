#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "board.h"

board::board(int h, int w, std::vector<int> r, std::vector<int> c){
	height=h;
	width=w;
	rows=r;
	columns=c;
	std::string add;
	for(int i=0;i<width;++i){
		add+=' ';
	}
	for(int i=0;i<height;++i){
		characters.push_back(add);
	}
}
int board::getHeight()const{
	return height;
}
int board::getWidth()const{
	return width;
}
int board::getRow(int row)const{
	return rows[row];
}
int board::getColumn(int column)const{
	return columns[column];
}
std::vector<std::string> board::getCharacters()const{
	return characters;
}
int board::getShipCount()const{
	return ships.size();
}
int board::getRowCount(int row)const{
	int count=0;
	for(int i=0;i<width;++i){
		if(characters[row][i]!=' '){
			count++;
		}
	}
	return count;
}
int board::getColumnCount(int column)const{
	int count=0;
	for(int i=0;i<height;++i){
		if(characters[i][column]!=' '){
			count++;
		}
	}
	return count;
}
void board::addShip(ship added, int row, int column){
	added.setRow(row);
	added.setColumn(column);
	ships.push_back(added);
}
void board::addShip(ship added, int row, int column, std::string orientation){
	added.setRow(row);
	added.setColumn(column);
	added.setOrientation(orientation);
	ships.push_back(added);
}
void board::removeShip(){
	ships.pop_back();
}
void board::setCharacter(int row, int column, char character){
	characters[row][column]=character;
}
void board::print(){
	//Function for printing out the added ships and the board used when a solution is found
	//I decided to sort the ships by position rather than by size right before printing, because I think it looks better that way
	std::sort(ships.begin(), ships.end(), topleft);
	std::cout<<"Solution:"<<std::endl;
	//Call the print function in the ship class to print out each ship
	for(unsigned int i=0;i<ships.size();++i){
		ships[i].print();
	}
	std::cout<<"+-";
	for(int i=0;i<width;++i){
		std::cout<<"--";
	}
	std::cout<<'+'<<std::endl;
	for(int i=0;i<height;++i){
		std::cout<<"| ";
		for(int j=0;j<width;++j){
			std::cout<<characters[i][j]<<" ";
		}
		if(rows[i]<=width){
			std::cout<<'|'<<rows[i]<<std::endl;
		}
		else{
			std::cout<<"|?"<<std::endl;
		}
	}
	std::cout<<"+-";
	for(int i=0;i<width;++i){
		std::cout<<"--";
	}
	std::cout<<'+'<<std::endl<<"  ";
	for(int i=0;i<width;++i){
		if(columns[i]<=height){
			std::cout<<columns[i]<<' ';
		}
		else{
			std::cout<<"? ";
		}
	}
	std::cout<<std::endl<<std::endl;
}