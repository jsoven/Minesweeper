#include "Tile.h"
Tile::Tile()
{
	tileSprite.setTexture(TextureManager::getTexture("tile_hidden"));
}

Tile::Tile(bool isMine)
{
	this->isMine = isMine;
	tileSprite.setTexture(TextureManager::getTexture("tile_hidden"));


}

void Tile::calcMines()
{
	//Loop through each neighboring tile and count how many are mines
	for (auto iter = adjTiles.begin(); iter != adjTiles.end(); iter++)
	{
		//If not an edge see if it has a mine
		if (iter->second != nullptr)
		{
			if (iter->second->isMine)
			{
				adjMines++;
			}
		}
	}
}

void Tile::placeFlag()
{
	if (isFlagged == false)
	{
		tileSprite.setTexture(TextureManager::getTexture("flag"));
	}
	else if (isFlagged == true)
	{
		tileSprite.setTexture(TextureManager::getTexture("tile_hidden"));
	}
	isFlagged = !isFlagged;
}

void Tile::revealTile()
{
	isRevealed = true;
	if (isMine)
	{
		tileSprite.setTexture(TextureManager::getTexture("mine"));
	}
	else if (adjMines == 0)
	{
		tileSprite.setTexture(TextureManager::getTexture("tile_revealed"));
		//Begin recursively attacking other tiles to be revealed
		for (auto iter = adjTiles.begin(); iter != adjTiles.end(); iter++)
		{
			if (iter->second != nullptr && !iter->second->isRevealed)
			{
				iter->second->revealTile();
			}
		}
	}

	else if (adjMines == 1)
	{
		tileSprite.setTexture(TextureManager::getTexture("number_1"));
	}

	else if (adjMines == 2)
	{
		tileSprite.setTexture(TextureManager::getTexture("number_2"));
	}

	else if (adjMines == 3)
	{
		tileSprite.setTexture(TextureManager::getTexture("number_3"));
	}

	else if (adjMines == 4)
	{
		tileSprite.setTexture(TextureManager::getTexture("number_4"));
	}

	else if (adjMines == 5)
	{
		tileSprite.setTexture(TextureManager::getTexture("number_5"));
	}

	else if (adjMines == 6)
	{
		tileSprite.setTexture(TextureManager::getTexture("number_6"));
	}

	else if (adjMines == 7)
	{
		tileSprite.setTexture(TextureManager::getTexture("number_7"));
	}

	else if (adjMines == 8)
	{
		tileSprite.setTexture(TextureManager::getTexture("number_8"));
	}
	else
	{
		throw(exception(), "Couldn't reveal texture");
	}
}
