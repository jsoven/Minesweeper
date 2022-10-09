#include "Board.h"
#include <iostream>
Board::Board()
{
	width = 0;
	height = 0;
	tilesW = 0;
	tilesH = 0;
	mineCount = 0;
	tileCount = 0;
}

Board::Board(int width, int height, int mineCount)
{
	this->tilesW = width;
	this->tilesH = height;
	this->width = width*32;
	this->height = (height*32)+88;
	this->mineCount = mineCount;
	tileCount = width * height;
	mineDisplay = mineCount;
	tiles.resize(width);

	//Build the board with empty tiles
	for (int ii = 0; ii < width; ii++)
	{
		tiles[ii].resize(height);
		for (int jj = 0; jj < height; jj++)
		{
			tiles[ii][jj] = new Tile();
			tiles[ii][jj]->tileSprite.setPosition(ii * 32, jj * 32);	
		}
	}

	//Place Mines on the board
	placeMines(mineCount);

	//Do all calculation of blocks
	calcTileNeighbors();
}

//Getters
bool Board::getWin()
{
	return win;
}
bool Board::getLose()
{
	return lose;
}
bool Board::getDebugMode()
{
	return debugMode;
}
int Board::getWidthInTiles()
{
	return tilesW;
}
int Board::getHeightInTiles()
{
	return tilesH;
}
int Board::getWidth()
{
	return width;
}
int Board::getHeight()
{
	return height;
}
int Board::getMineCount()
{
	return mineCount;
}
int Board::getTileCount()
{
	return tileCount;
}
int Board::getMineDisplay()
{
	return mineDisplay;
}

//Setters
void Board::setDebugMode()
{
	debugMode = !debugMode;
}

//Utilities
void Board::uncoverTile(sf::Vector2i &mousePosition)
{
	if (mousePosition.x >= 0 && mousePosition.x < tilesW && mousePosition.y >= 0 && mousePosition.y < tilesH)
	{
		tiles[mousePosition.x][mousePosition.y]->revealTile();
		
		//If it's a mine reveal it but also lose game
		if (tiles[mousePosition.x][mousePosition.y]->isMine)
		{
			lose = true;
		}
	}

	//Check to see if this has won the game for us
	win = true;
	for (int ii = 0; ii < tilesW; ii++)
	{
		for (int jj = 0; jj < tilesH; jj++)
		{
			if (!tiles[ii][jj]->isRevealed && !tiles[ii][jj]->isMine)
				win = false;
		}
	}


}
void Board::placeFlag(sf::Vector2i& mousePosition)
{
	mousePosition.x = mousePosition.x / 32;
	mousePosition.y = (mousePosition.y) / 32;

	if (mousePosition.x >= 0 && mousePosition.x < tilesW && mousePosition.y >= 0 && mousePosition.y < tilesH)
	{
		tiles[mousePosition.x][mousePosition.y]->placeFlag();
		if (tiles[mousePosition.x][mousePosition.y]->isFlagged)
			mineDisplay--;
		else
			mineDisplay++;
	}
}
void Board::calcTileNeighbors()  //Assign tile neighbors and calc mines
{
	//Loop through and assign pointers to all tile neighbors
	for (int ii = 0; ii < tilesW; ii++)
	{
		for (int jj = 0; jj < tilesH; jj++)
		{
			//Start off by just trying to assign all pointers to everything
			try { tiles[ii][jj]->adjTiles["NW"] = tiles.at(ii - 1).at(jj - 1); }
			catch (...) {}

			try { tiles[ii][jj]->adjTiles["N"] = tiles.at(ii).at(jj - 1); }
			catch (...) {}

			try { tiles[ii][jj]->adjTiles["NE"] = tiles.at(ii + 1).at(jj - 1); }
			catch (...) {}

			try { tiles[ii][jj]->adjTiles["E"] = tiles.at(ii + 1).at(jj); }
			catch (...) {}

			try { tiles[ii][jj]->adjTiles["SE"] = tiles.at(ii + 1).at(jj + 1); }
			catch (...) {}

			try { tiles[ii][jj]->adjTiles["S"] = tiles.at(ii).at(jj + 1); }
			catch (...) {}

			try { tiles[ii][jj]->adjTiles["SW"] = tiles.at(ii - 1).at(jj + 1); }
			catch (...) {}

			try { tiles[ii][jj]->adjTiles["W"] = tiles.at(ii - 1).at(jj); }
			catch (...) {}


			//All the edges shouldn't have worked so now assign them nullptrs

			//If tile is on top edge
			if (jj == 0)
			{
				tiles[ii][jj]->adjTiles["NW"] = nullptr;
				tiles[ii][jj]->adjTiles["N"] = nullptr;
				tiles[ii][jj]->adjTiles["NE"] = nullptr;
			}
			//If tile is on bottom edge
			if (jj == tilesH - 1)
			{
				tiles[ii][jj]->adjTiles["SW"] = nullptr;
				tiles[ii][jj]->adjTiles["S"] = nullptr;
				tiles[ii][jj]->adjTiles["SE"] = nullptr;

			}
			//If tile is on left edge
			if (ii == 0)
			{
				tiles[ii][jj]->adjTiles["SW"] = nullptr;
				tiles[ii][jj]->adjTiles["W"] = nullptr;
				tiles[ii][jj]->adjTiles["NW"] = nullptr;
			}
			//If tile is on right edge
			if (ii == tilesW - 1)
			{
				tiles[ii][jj]->adjTiles["NE"] = nullptr;
				tiles[ii][jj]->adjTiles["E"] = nullptr;
				tiles[ii][jj]->adjTiles["SE"] = nullptr;
			}
		}
	}

	//Loop through again and tell each tile to count it's neighbors
	for (int ii = 0; ii < tilesW; ii++)
	{
		for (int jj = 0; jj < tilesH; jj++)
		{
			tiles[ii][jj]->calcMines();
			
		}
	}
}
void Board::placeMines(int mineNum)
{
	//Get random coordinates to place mines at
	int randomX;
	int randomY;
	int ii = 0;
	
	//Loop through placing mines. If a mine is already at that location
	//Don't do anything and just loop through again 
	while (ii < mineNum)
	{
		randomX = Random::Int(0,tilesW-1);
		randomY = Random::Int(0, tilesH-1);
		
		if (!tiles[randomX][randomY]->isMine)
		{
			tiles[randomX][randomY]->isMine = true;
			ii++;
		}
	}
}
void Board::reset()
{
	lose = false;
	win = false;
	mineDisplay = mineCount;
	//Delete old board and create new blank board
	for (int ii = 0; ii < tilesW; ii++)
	{
		for (int jj = 0; jj < tilesH; jj++)
		{
			delete tiles[ii][jj];
			tiles[ii][jj] = new Tile();
			tiles[ii][jj]->tileSprite.setPosition(ii * 32, jj * 32);
		}
	}

	//Place Mines again
	placeMines(mineCount);

	//Calculate all values for tiles
	calcTileNeighbors();
}

//Destructor
Board::~Board()
{
	for (int ii = 0; ii < tilesW; ii++)
	{
		for (int jj = 0; jj < tilesH; jj++)
		{
			delete tiles[ii][jj];
		}
	}
}


