#include <cstdlib>
#include <vector>
#include "ship.h"

class board{
	public:
	board(int height, int width, std::vector<int> rows, std::vector<int> columns);
	int getWidth()const;
	int getHeight()const;
	int getRow(int row)const;
	int getColumn(int column)const;
	std::vector<std::string> getCharacters()const;
	int getShipCount()const;
	int getRowCount(int row)const;
	int getColumnCount(int column)const;
	void addShip(ship added, int row, int column);
	void addShip(ship added, int row, int column, std::string orientation);
	void removeShip();
	void setCharacter(int x, int y, char character);
	void print();
	private:
	int height;
	int width;
	std::vector<int> rows;
	std::vector<int> columns;
	std::vector<std::string> characters;
	std::vector<ship> ships;
};