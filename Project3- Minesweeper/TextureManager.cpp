#include "TextureManager.h"

//"redeclare" static variable
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::loadTexture(string filename) //LoadTexture("space")
{
	string path = "images/";
	path += filename + ".png";
	textures[filename].loadFromFile(path);
}

sf::Texture& TextureManager::getTexture(string textureName)
{
	//if texture doesn't exist...
	//load it first? then return it?

	if (textures.find(textureName) == textures.end())  //Didn't find it
		loadTexture(textureName);

	return textures[textureName];
}

void TextureManager::clear() //Call one in main()
{
	textures.clear(); //get rid of all textures
}