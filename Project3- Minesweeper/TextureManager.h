#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using namespace std;

class TextureManager
{
	//static == one and only one of these in memory
	static unordered_map<string, sf::Texture> textures;

public:
	static void loadTexture(string textureName); //LoadTexture("space")
	static sf::Texture& getTexture(string textureName);
	static void clear(); //Call one in main()

};

