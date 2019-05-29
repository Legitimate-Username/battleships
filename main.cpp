#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include "board.h"

//Unfortunately some global variables were necessary in order to find out if the puzzle was solved and to keep track of the found solutions
bool solved=false;
std::vector<board> solutions;
//Defining this function ahead of time so that the add function can call it
void recursion(board battleships, std::vector<ship> ships, unsigned int count, bool findall, bool constraining, std::vector<std::string> constraints);

//Function for finding out if a given solution has already been found
bool unused(board battleships){
	bool unique=true;
	//Loop through the solutions that have already been found
	for(unsigned int i=0;i<solutions.size();++i){
		//If both boards have identical character vectors, then the solution has already been found before
		if(battleships.getCharacters()==solutions[i].getCharacters()){
			unique=false;
		}
	}
	return unique;
}

//Function for finding out whether or not a solution satisfies the given conditions
bool verify(board battleships, bool constraining, std::vector<std::string> constraints){
	//This part will not run if there are no constraints in order to save time
	if(constraining){
		//Loop through all of the board's characters
		for(int i=0;i<battleships.getHeight();++i){
			for(int j=0;j<battleships.getWidth();++j){
				//If any of the characters don't match the constraints, then the solution is not valid
				if(constraints[i][j]!=' '&&constraints[i][j]!=battleships.getCharacters()[i][j]){
					return false;
				}
			}
		}
	}
	//Verify that the given row values match up with the solution
	for(int i=0;i<battleships.getHeight();++i){
		//Make sure that the row value wasn't originally set to an unknown (represented here as a value greater than the board width)
		if(battleships.getRow(i)<=battleships.getWidth()){
			int count=0;
			for(int j=0;j<battleships.getWidth();++j){
				if(battleships.getCharacters()[i][j]!=' '){
					++count;
				}
			}
			if(count!=battleships.getRow(i)){
				return false;
			}
		}
	}
	//Verify that the given column values match up with the solution
	for(int i=0;i<battleships.getWidth();++i){
		//Make sure that the column value wasn't originally set to an unknown (represented here as a value greater than the board height)
		if(battleships.getColumn(i)<=battleships.getHeight()){
			int count=0;
			for(int j=0;j<battleships.getHeight();++j){
				if(battleships.getCharacters()[j][i]!=' '){
					++count;
				}
			}
			if(count!=battleships.getColumn(i)){
				return false;
			}
		}
	}
	return true;
}

//Function for finding out if a ship can fit in a specific spot
bool canAdd(board battleships, ship added, int row, int column, std::string orientation){
	//If we're placing the ship horizontally
	if(orientation=="horizontal"){
		//If the ship extends past the right edge of the board, then it won't fit
		if(column+added.getLength()>battleships.getWidth()){
			return false;
		}
		//If there are any characters already on the board within 1 character of the ship, then it won't fit
		for(int i=std::max(0, row-1);i<std::min(battleships.getHeight(), row+2);++i){
			for(int j=std::max(0, column-1);j<std::min(battleships.getWidth(), column+added.getLength()+1);++j){
				if(battleships.getCharacters()[i][j]!=' '){
					return false;
				}
			}
		}
		//If adding this ship would cause the given row to exceed the allowed number of spaces, then it won't fit
		if(battleships.getRowCount(row)+added.getLength()>battleships.getRow(row)){
			return false;
		}
		//If adding this ship would cause the given columns to exceed the allowed number of spaces, then it won't fit
		for(int i=column;i<column+added.getLength();++i){
			if(battleships.getColumnCount(i)+1>battleships.getColumn(i)){
				return false;
			}
		}
	}
	//If we're placing the ship vertically
	if(orientation=="vertical"){
		//If the ship extends past the bottom edge of the board then it won't fit
		if(row+added.getLength()>battleships.getHeight()){
			return false;
		}
		//If there are any characters already on the board within 1 character of the ship, then it won't fit
		for(int i=std::max(0, row-1);i<std::min(battleships.getHeight(), row+added.getLength()+1);++i){
			for(int j=std::max(0, column-1);j<std::min(battleships.getWidth(), column+2);++j){
				if(battleships.getCharacters()[i][j]!=' '){
					return false;
				}
			}
		}
		//If adding this ship would cause the given column to exceed the allowed number of spaces, then it won't fit
		if(battleships.getColumnCount(column)+added.getLength()>battleships.getColumn(column)){
			return false;
		}
		//If adding this ship would cause the given rows to exceed the allowed number of spaces, then it won't fit
		for(int i=row;i<row+added.getLength();++i){
			if(battleships.getRowCount(i)+1>battleships.getRow(i)){
				return false;
			}
		}
	}
	//If none of the above checks were triggered, then the ship can fit here
	return true;
}

//Function for adding a ship onto the board
void add(board battleships, ship boat, std::string orientation, std::vector<ship> ships, unsigned int count, bool findall, bool constraining, std::vector<std::string> constraints){
	//Loop through every possible square and try to add a ship there
	for(int i=0;i<battleships.getHeight();++i){
		for(int j=0;j<battleships.getWidth();++j){
			//Check if the ship actually fits in the spot
			if(canAdd(battleships, boat, i, j, orientation)){
				//Case for if the ship is of length 1
				if(boat.getLength()==1){
					//Add the ship to the board's list of added ships
					battleships.addShip(boat, i, j);
					//Set the board's characters to have the ship shown on it
					battleships.setCharacter(i, j, 'o');
				}
				//Case for placing the ship horizontally
				else if(orientation=="horizontal"){
					//Add the ship to the board's list of added ships
					battleships.addShip(boat, i, j, orientation);
					//Set the board's characters to have the ship shown on it
					battleships.setCharacter(i, j, '<');
					for(int k=j+1;k<j+boat.getLength()-1;++k){
						battleships.setCharacter(i, k, 'X');
					}
					battleships.setCharacter(i, j+boat.getLength()-1, '>');
				}
				//Case for placing the ship vertically
				else if(orientation=="vertical"){
					//Add the ship to the board's list of added ships
					battleships.addShip(boat, i, j, orientation);
					//Set the board's characters to have the ship shown on it
					battleships.setCharacter(i, j, '^');
					for(int k=i+1;k<i+boat.getLength()-1;++k){
						battleships.setCharacter(k, j, 'X');
					}
					battleships.setCharacter(i+boat.getLength()-1, j, 'v');
				}
				//If we've already added all of the given ships
				if(count+1==ships.size()){
					//Make sure that the found solution is valid
					if(verify(battleships, constraining, constraints)){
						solved=true;
						//If we only need to find one solution
						if(!findall){
							battleships.print();
							exit(0);
						}
						//If we need to find all possible solutions
						else{
							if(unused(battleships)){
								solutions.push_back(battleships);
							}
						}
					}
				}
				//If we still have more ships to add
				else{
					//Return to the recursion function in order to add the next ship
					recursion(battleships, ships, count+1, findall, constraining, constraints);
					//Remove the ship that we just added from the board
					battleships.removeShip();
					if(orientation=="horizontal"){
						for(int k=j;k<j+boat.getLength();++k){
							battleships.setCharacter(i, k, ' ');
						}
					}
					else if(orientation=="vertical"){
						for(int k=i;k<i+boat.getLength();++k){
							battleships.setCharacter(k, j, ' ');
						}
					}
				}
			}
		}
	}
}

//Function for managing the recursive process
void recursion(board battleships, std::vector<ship> ships, unsigned int count, bool findall, bool constraining, std::vector<std::string> constraints){
	//Case for adding a ship that doesn't have an unknown value
	if(ships[count].getLength()!=0){
		add(battleships, ships[count], "horizontal", ships, count, findall, constraining, constraints);
		//This if statement keeps us from trying to add every submarine twice, which helps to save time
		if(ships[count].getLength()!=1){
			add(battleships, ships[count], "vertical", ships, count, findall, constraining, constraints);
		}
	}
	//Case for adding a ship that has an unknown value
	else{
		//Declaring some generic ships that can be added to the board
		ship submarine("submarine");
		ship destroyer("destroyer");
		ship cruiser("cruiser");
		ship battleship("battleship");
		ship carrier("carrier");
		ship cargo("cargo");
		ship tanker("tanker");
		//Try out all 13 potential ship placements, from longest to shortest and only if they fit on the board in order to save time
		if(battleships.getWidth()>=7){
			add(battleships, tanker, "horizontal", ships, count, findall, constraining, constraints);
		}
		if(battleships.getHeight()>=7){
			add(battleships, tanker, "vertical", ships, count, findall, constraining, constraints);
		}
		if(battleships.getWidth()>=6){
			add(battleships, cargo, "horizontal", ships, count, findall, constraining, constraints);
		}
		if(battleships.getHeight()>=6){
			add(battleships, cargo, "vertical", ships, count, findall, constraining, constraints);
		}
		if(battleships.getWidth()>=5){
			add(battleships, carrier, "horizontal", ships, count, findall, constraining, constraints);
		}
		if(battleships.getHeight()>=5){
			add(battleships, carrier, "vertical", ships, count, findall, constraining, constraints);
		}
		if(battleships.getWidth()>=4){
			add(battleships, battleship, "horizontal", ships, count, findall, constraining, constraints);
		}
		if(battleships.getHeight()>=4){
			add(battleships, battleship, "vertical", ships, count, findall, constraining, constraints);
		}
		if(battleships.getWidth()>=3){
			add(battleships, cruiser, "horizontal", ships, count, findall, constraining, constraints);
		}
		if(battleships.getHeight()>=3){
			add(battleships, cruiser, "vertical", ships, count, findall, constraining, constraints);
		}
		if(battleships.getWidth()>=2){
			add(battleships, destroyer, "horizontal", ships, count, findall, constraining, constraints);
		}
		if(battleships.getHeight()>=2){
			add(battleships, destroyer, "vertical", ships, count, findall, constraining, constraints);
		}
		add(battleships, submarine, "horizontal", ships, count, findall, constraining, constraints);
	}
}

int main(int argc, char* argv[]){
	std::ifstream in_str(argv[1]);
	if(!in_str.good()){
		std::cerr<<"Can't open "<<argv[1]<<" to read.\n";
		exit(1);
	}
	//Convert the file into an array for easier reading
	std::string input;
	std::vector<std::string> data;
	while(in_str>>input){
		data.push_back(input);
	}
	//Set the board height and width
	int height=atoi(data[1].c_str());
	int width=atoi(data[2].c_str());
	//Set the borad's row and column values
	std::vector<int> rows;
	std::vector<int> columns;
	for(int i=4;i<4+height;++i){
		//Unknown row values are set to a value greater than the board's width
		if(data[i]=="?"){
			rows.push_back(width+1);
		}
		else{
			rows.push_back(atoi(data[i].c_str()));
		}
	}
	for(int i=5+height;i<5+height+width;++i){
		//Unknown column values are set to a value greater than the board's width
		if(data[i]=="?"){
			columns.push_back(height+1);
		}
		else{
			columns.push_back(atoi(data[i].c_str()));
		}
	}
	//Create the board
	board battleships(height, width, rows, columns);
	//This boolean is used to save time by skipping constraint calculations if no fconstraints are present
	bool constraining=false;
	unsigned int constraint=data.size();
	//Search the input data for the word "constraint"
	for(unsigned int i=0;i<data.size();++i){
		if(data[i]=="constraint"){
			//Mark the placement of the first constraint
			constraint=i;
			constraining=true;
			break;
		}
	}
	//Create a vector of ships that will be added to the board
	std::vector<ship> ships;
	for(unsigned int i=5+height+width;i<constraint;++i){
		ship newship(data[i]);
		ships.push_back(newship);
	}
	//Sort the ships by length in order to speed up the recursive process
	std::sort(ships.begin(), ships.end(), longer);
	bool findall=false;
	//Check for a second command line argument
	if(argv[2]){
		std::string argument=argv[2];
		//Check if we should find all solutions
		if(argument=="find_all_solutions"){
			findall=true;
		}
	}
	//Create a vector of blank strings to keep track of the constraints
	std::vector<std::string> constraints;
	constraints=battleships.getCharacters();
	if(constraining){
		//Add each given constraint into the constraints board
		for(unsigned int i=constraint;i<data.size();i+=4){
			constraints[atoi(data[i+1].c_str())][atoi(data[i+2].c_str())]=data[i+3].c_str()[0];
		}
	}
	//Start the recursive process of adding ships onto the board
	recursion(battleships, ships, 0, findall, constraining, constraints);
	//If we're finding all solutions, print out the list of found solutions
	if(findall){
		for(unsigned int i=0;i<solutions.size();++i){
			solutions[i].print();
		}
		std::cout<<"Found "<<solutions.size()<<" solution(s)"<<std::endl;
	}
	//Case for if no solution was found
	if(!solved){
		std::cout<<"No solution."<<std::endl;
	}
	return 0;
}