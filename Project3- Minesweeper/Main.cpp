#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include "Random.h"
#include "TextureManager.h"
#include "Board.h"
#include "Tile.h"
using namespace std;


void drawBoard(sf::RenderWindow& window, Board& board)
{
    for (int ii = 0; ii < board.getWidthInTiles(); ii++)
    {
        for (int jj = 0; jj < board.getHeightInTiles(); jj++)
        {
            if (board.tiles[ii][jj]->isRevealed)
            {
                //draw just the revealed tile
                board.tiles[ii][jj]->tileSprite.setTexture(TextureManager::getTexture("tile_revealed"));
                window.draw(board.tiles[ii][jj]->tileSprite);

                //Now get the layer that will go on top of it
                sf::Vector2i mousePosition;
                mousePosition.x = ii;
                mousePosition.y = jj;
                board.uncoverTile(mousePosition);
            }
            else if (board.tiles[ii][jj]->isFlagged)
            {
                //draw just the plain tile first
                board.tiles[ii][jj]->tileSprite.setTexture(TextureManager::getTexture("tile_hidden"));
                window.draw(board.tiles[ii][jj]->tileSprite);
                board.tiles[ii][jj]->isFlagged = false; //Set to false to prep for replaccing flag
                board.tiles[ii][jj]->placeFlag();
            }
            window.draw(board.tiles[ii][jj]->tileSprite);

            //If in debug mode, draw mine on top of everything
            if (board.getDebugMode() && board.tiles[ii][jj]->isMine)
            {
                board.tiles[ii][jj]->tileSprite.setTexture(TextureManager::getTexture("mine"));
                window.draw(board.tiles[ii][jj]->tileSprite);
                board.tiles[ii][jj]->tileSprite.setTexture(TextureManager::getTexture("tile_hidden"));
            }
               
        }
    }
}
void initializeMineDisplay(vector<sf::Texture>& mineNum)
{
    for (int ii = 0; ii < mineNum.size(); ii++)
    {
        mineNum[ii].loadFromFile("images/digits.png", sf::IntRect(ii*21, 0, 21, 32));
    }
}
void updateMineDisplay(vector<sf::Texture>& mineNum, sf::Sprite& mineDisplay1, sf::Sprite& mineDisplay10,sf::Sprite& mineDisplay100, sf::Sprite& mineDisplay1000, int mineCount)
{
    mineDisplay1.setTexture(mineNum[abs(mineCount % 10)]);
    mineDisplay10.setTexture(mineNum[abs(mineCount/10 % 10)]);
    mineDisplay100.setTexture(mineNum[abs(mineCount/100 % 10)]);
    if (mineCount < 0)
        mineDisplay1000.setTexture(mineNum[10]);
    else
        mineDisplay1000.setTexture(mineNum[mineCount/1000 % 10]);
}

int main()
{
    //Start by initializing board to know render window size needed
    int boardHeight = 20;
    int boardWidth = 20;
    int boardMines = 0;

    fstream file("boards/config.cfg", ios_base::in);
    if (file.is_open())
    {
       string line;
       getline(file, line);
       boardWidth = stoi(line);
       getline(file, line);
       boardHeight = stoi(line);
       getline(file, line);
       boardMines = stoi(line);
    }

    //Initialize Mouse Position and any other Window variables
    sf::Vector2i mousePosition;

    //Initialize board to proper size, add mines, do calculations, Create window to size
    Board board(boardWidth, boardHeight, boardMines);
    int boardW = board.getWidth();  //Render Window required me to get ints before hand
    int boardH = board.getHeight();

    if (boardW < 16 * 32)
        boardW = 16 * 32;

    sf::RenderWindow window(sf::VideoMode(boardW,boardH+64), "Minesweeper");
    boardW = board.getWidth();

    //Initialize buttons
    sf::Sprite smiley,debug,test1,test2,test3,mineDisplay1, mineDisplay10, mineDisplay100, mineDisplay1000;

    //Initialize smiley button
    smiley.setTexture(TextureManager::getTexture("face_happy"));
    smiley.setPosition(boardW / 2-64, boardH-90);
    auto smileyBounds = smiley.getGlobalBounds();

    //Initialize debug button
    debug.setTexture(TextureManager::getTexture("debug"));
    debug.setPosition(boardW / 2, boardH-90);
    auto debugBounds = debug.getGlobalBounds();

    //Initialize test buttons
    test1.setTexture(TextureManager::getTexture("Test_1"));
    test1.setPosition(boardW / 2 + (64), boardH-90);
    auto test1Bounds = test1.getGlobalBounds();
    test2.setTexture(TextureManager::getTexture("Test_2"));
    test2.setPosition(boardW / 2 + (64*2), boardH-90);
    auto test2Bounds = test2.getGlobalBounds();
    test3.setTexture(TextureManager::getTexture("Test_3"));
    test3.setPosition(boardW / 2 + (64*3), boardH-90);
    auto test3Bounds = test3.getGlobalBounds();


    //Initialize mineDisplay button
    sf::Texture zero, one, two, three, four, five, six, seven, eight, nine, minus;
    vector<sf::Texture> mineNum = { zero,one,two,three,four,five,six,seven,eight,nine,minus };
    initializeMineDisplay(mineNum);

    mineDisplay1000.setTexture(mineNum[0]);
    mineDisplay1000.setPosition(0, boardH - 90);
    mineDisplay100.setTexture(mineNum[0]);
    mineDisplay100.setPosition(21, boardH - 90);
    mineDisplay10.setTexture(mineNum[0]);
    mineDisplay10.setPosition(42, boardH - 90);
    mineDisplay1.setTexture(mineNum[0]);
    mineDisplay1.setPosition(63, boardH - 90);


  


    while (window.isOpen())
    {
        window.clear();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            //if Mouse is pressed
            if (event.type == sf::Event::MouseButtonPressed)
            {
                mousePosition = sf::Mouse::getPosition(window);
                //Which button was pressed?
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    mousePosition.x = mousePosition.x / 32;
                    mousePosition.y = (mousePosition.y) / 32;

                    //Update the tiles sprite to uncovered value as long as game is going
                    if (!board.getWin() && !board.getLose())
                    {
                        board.uncoverTile(mousePosition); //Checks internally if mouse is on a tile
                    }

                    //Check if it's on a button
                    if (smileyBounds.contains(mousePosition.x * 32, mousePosition.y * 32))
                    {
                        board.reset();
                        smiley.setTexture(TextureManager::getTexture("face_happy"));
                    }
                    if (debugBounds.contains(mousePosition.x * 32, mousePosition.y * 32))
                    {
                        board.setDebugMode();
                    }
                    if (test1Bounds.contains(mousePosition.x * 32, mousePosition.y * 32))
                    {

                    }
                    if (test2Bounds.contains(mousePosition.x * 32, mousePosition.y * 32))
                    {

                    }
                    if (test3Bounds.contains(mousePosition.x * 32, mousePosition.y * 32))
                    {

                    }




                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    board.placeFlag(mousePosition);
                }

            }
        }
        
        //Check the current state of the game before drawing
        //Loop and see if you've won
        if (board.getLose())
        {
            smiley.setTexture(TextureManager::getTexture("face_lose"));
        }
        else if (board.getWin())
        {
            smiley.setTexture(TextureManager::getTexture("face_win"));
        }
        updateMineDisplay(mineNum,mineDisplay1,mineDisplay10,mineDisplay100,mineDisplay1000,board.getMineDisplay());
        drawBoard(window, board);

        //Draw buttons
        window.draw(smiley);
        window.draw(debug);
        window.draw(test1);
        window.draw(test2);
        window.draw(test3);
        window.draw(mineDisplay1);
        window.draw(mineDisplay10);
        window.draw(mineDisplay100);
        window.draw(mineDisplay1000);

        window.display();
    }

    return 0;
}