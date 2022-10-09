#pragma once
#include "Tile.h"
#include "Random.h"
#include <vector>
using namespace std;
class Board
{
	bool win = false;
	bool lose = false;
	bool debugMode = false;
	int tilesW;
	int tilesH;
	int width;
	int height;
	int mineCount;
	int tileCount;
	int mineDisplay;
	

public:
	vector<vector<Tile*>> tiles;

	//Constructors
	Board();
	Board(int width, int height, int mineCount);

	//Getters
	bool getWin();
	bool getLose();
	bool getDebugMode();
	int getWidthInTiles();
	int getHeightInTiles();
	int getWidth();
	int getHeight();
	int getMineCount();
	int getMineDisplay();
	int getTileCount();

	//Setters
	void setDebugMode();

	//Utilities
	void uncoverTile(sf::Vector2i &mousePosition);
	void placeFlag(sf::Vector2i& mousePosition);
	void calcTileNeighbors();
	void placeMines(int mineNum);
	void reset();

	//Destructors
	~Board();

	
};

