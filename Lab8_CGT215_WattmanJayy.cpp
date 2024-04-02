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
    int score(0); //start at zero because you haven't earned any points yet
    int arrows(5); //start at 5 so it can decrement down
    int fontSz(48);

    Font font;
    if (!font.loadFromFile("arial.ttf")) 
    {
        cout << "Could not load font" << endl;
        exit(0);
    }

    //let's make our crossbow
    PhysicsSprite crossbow;
    Texture cbowTex;
    LoadTex(cbowTex, "images/crossbow.png");
    crossbow.setTexture(cbowTex);
    Vector2f szBow = crossbow.getSize();
    crossbow.setCenter(Vector2f(400, 600 - (szBow.y / 2)));

    //time to make the arrow
    PhysicsSprite arrow;
    Texture arrowTex; 
    LoadTex(arrowTex, "images/arrow.png"); 
    arrow.setTexture(arrowTex);
    bool drawingArrow(false);

    //Let's make a ceiling so the arrows don't just go flying on forever
    PhysicsRectangle top;
    top.setSize(Vector2f(800, 10));
    top.setCenter(Vector2f(400, 5)); 
    top.setStatic(true); 
    world.AddPhysicsBody(top); 
    top.onCollision = [&arrow, &arrows, &world, &drawingArrow](PhysicsBodyCollisionResult result) 
        {
            arrows--;
            drawingArrow = false;
            world.RemovePhysicsBody(arrow);
        };
  

    //ducks quack quack
    Texture duckTex;
    LoadTex(duckTex, "images/duck.png");
    PhysicsShapeList<PhysicsSprite> ducks;

    //need a wall for the ducks to collide with so it can be removed
    PhysicsRectangle rWall;
    rWall.setSize(Vector2f(10, 600));
    rWall.setCenter(Vector2f(950, 400)); //needs to be just past the view port so it looks like the ducks are moving on
    rWall.setStatic(true);
    world.AddPhysicsBody(rWall);

    //text
    Text arrowText;
    arrowText.setFont(font); 
    arrowText.setCharacterSize(fontSz); 
    
    Text scoreText; 
    scoreText.setFont(font); 
    scoreText.setCharacterSize(fontSz); 

    //let's start some time set up
    Clock clock;
    Time lastTime(clock.getElapsedTime());
    Time lastDUCKTime(clock.getElapsedTime()); //ducks need their own time loop so it can look like an endless scroll of ducks


    //Game loop time
    while(arrows > 0) 
    {
        Time currentTime(clock.getElapsedTime());
        int deltaTime((currentTime - lastTime).asMilliseconds());
    }
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
