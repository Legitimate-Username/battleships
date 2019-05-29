#include <cstdlib>

class ship{
	public:
	ship(std::string name);
	int getLength()const;
	int getRow()const;
	int getColumn()const;
	void setRow(int r);
	void setColumn(int c);
	void setOrientation(std::string o);
	void print()const;
	private:
	std::string name;
	int length;
	int row;
	int column;
	std::string orientation;
};
bool longer(const ship& ship1, const ship& ship2);
bool topleft(const ship& ship1, const ship& ship2);