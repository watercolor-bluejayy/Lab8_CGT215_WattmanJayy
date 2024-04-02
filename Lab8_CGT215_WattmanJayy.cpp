// Lab8_CGT215_WattmanJayy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include <vector>

using namespace std;
using namespace sf;
using namespace sfp;

//keyboard speed constant, affects how much power each key press imparts on the cbow
const float KB_Speed = 0.2;

//function for loading textures that can be referenced later
void LoadTex(Texture& tex, string filename)
{
    if (!tex.loadFromFile(filename)) 
    {
        cout << "Could not load " << filename << endl;
    }
}

//function for crossbow motion inputs
void MoveCbow(PhysicsSprite& crossbow, int elapsedMS) 
{
    if (Keyboard::isKeyPressed(Keyboard::Right)) 
    {
        Vector2f newPos(crossbow.getCenter());
        newPos.x = newPos.x + (KB_Speed * elapsedMS);
        crossbow.setCenter(newPos);
    }
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        Vector2f newPos(crossbow.getCenter());
        newPos.x = newPos.x - (KB_Speed * elapsedMS);
        crossbow.setCenter(newPos);
    }
}

int main()
{
    RenderWindow window(VideoMode(800, 600), "Duck Hunter");
    World world(Vector2f(0, 0)); //Gravity set to zero because we don't want it pulling on anything
    int score(0);
    int arrows(1);

    //let's make our crossbow
    PhysicsSprite crossbow;
    Texture cbowTex;
    LoadTex(cbowTex, "images/crossbow.png");
    crossbow.setTexture(cbowTex);
    Vector2f sz = crossbow.getSize();
    crossbow.setCenter(Vector2f(400, 600 - (sz.y / 2)));

    //time to make the arrow
    PhysicsSprite arrow;
    Texture arrowTex; 
    LoadTex(arrowTex, "images/arrow.png"); 
    arrow.setTexture(arrowTex); 
    bool drawingArrow(false);



}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
