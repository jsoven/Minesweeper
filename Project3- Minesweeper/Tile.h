#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
using namespace std;
class Tile
{
public:
	bool isMine = false;
	bool isFlagged = false;
	bool isRevealed = false;
	int adjMines = 0;
	map<string,Tile*> adjTiles; // Store each tile near, stored as "N,NE,E,..."
	sf::Sprite tileSprite;

	Tile();
	Tile(bool isMine);
	void calcMines();
	void placeFlag();
	void revealTile();
};

