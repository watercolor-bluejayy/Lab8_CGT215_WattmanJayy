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
const float KB_Speed = 0.2; // magic number, might need to adjust

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
    PhysicsSprite& crossbow = *new PhysicsSprite();
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
    top.setCenter(Vector2f(400, -5)); 
    top.setStatic(true); 
    world.AddPhysicsBody(top); 
    top.onCollision = [&arrow, &arrows, &world, &drawingArrow](PhysicsBodyCollisionResult result) 
        {
            arrows--;
            drawingArrow = false;
            world.RemovePhysicsBody(arrow);
        };
  

    //need a wall for the ducks to collide with so it can be removed
    PhysicsRectangle rWall;
    rWall.setSize(Vector2f(10, 600));
    rWall.setCenter(Vector2f(950, 400)); //needs to be just past the view port so it looks like the ducks are moving on
    rWall.setStatic(true);
    world.AddPhysicsBody(rWall);

    //ducks quack quack
    Texture duckTex; 
    LoadTex(duckTex, "images/duck.png"); 
    PhysicsShapeList<PhysicsSprite> ducks; 
    /*for (int i(0); i < 8; i++) 
    {
        PhysicsSprite& duck = ducks.Create();
        duck.setTexture(duckTex);
        int x = 50 + ((850 / 5) * i);
        Vector2f sz = duck.getSize();
        duck.setCenter(Vector2f(x, 20 + (sz.y / 2)));
        duck.setVelocity(Vector2f(0.5, 0)); 
        world.AddPhysicsBody(duck);
        duck.onCollision = [&drawingArrow, &world, &arrow, &duck, &ducks, &score, &rWall](PhysicsBodyCollisionResult result) 
            {
            if (result.object2 == arrow) {
                drawingArrow = false; 
                world.RemovePhysicsBody(arrow);
                world.RemovePhysicsBody(duck);
                ducks.QueueRemove(duck);
                score += 25;
            }
            if (result.object2 == rWall) {
                world.RemovePhysicsBody(duck);
                ducks.QueueRemove(duck);
            }
            };
    }*/

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
    long duckMS(0);
    while((arrows > 0) || drawingArrow) 
    {
        Time currentTime(clock.getElapsedTime());
        Time deltaTime = currentTime - lastTime;
        long deltaMS = deltaTime.asMilliseconds();

        if (deltaMS > 9) 
        {
            lastTime = currentTime;
            world.UpdatePhysics(deltaMS);
            MoveCbow(crossbow, deltaMS);
            if (Keyboard::isKeyPressed(Keyboard::Space) && !drawingArrow) 
            {
                drawingArrow = true;
                arrows--;
                arrow.setCenter(crossbow.getCenter());
                arrow.setVelocity(Vector2f(0, -1.2)); //magic number, might need to adjust
                world.AddPhysicsBody(arrow);
            }

            window.clear();
            window.draw(crossbow);
           
            if (drawingArrow) 
            {
                window.draw(arrow);
            }
            ducks.DoRemovals();
            for (PhysicsShape& duck : ducks) 
            {
                window.draw((PhysicsSprite&)duck);
            }

            //arrows text
            arrowText.setString(to_string(arrows)); 
            FloatRect arrowSz(arrowText.getGlobalBounds()); 
            arrowText.setPosition(Vector2f(20 - (arrowSz.width / 2), 560 - (arrowSz.height / 2)));
            window.draw(arrowText); 

            //score text
            scoreText.setString(to_string(score)); 
            FloatRect scoreSz(scoreText.getGlobalBounds()); 
            scoreText.setPosition(Vector2f(770 - (arrowSz.width), 560 - (arrowSz.height))); 
            window.draw(scoreText); 


            //duck time, let's get to drawing 'em
            //Time currentDUCKTime(clock.getElapsedTime()); //ducks need their own time loop, I capitalized the "DUCK" so I'd be less likely to miss it upon editing
            if (duckMS > 2000) 
            {
                duckMS = 0;
                for (int i(0); i < 8; i++) {
                    PhysicsSprite& newDuck = ducks.Create();
                    newDuck.setTexture(duckTex);
                    newDuck.setScale(Vector2f(0.5, 0.5));
                    newDuck.setCenter(Vector2f(0, 120 - (newDuck.getGlobalBounds().height / 2)));
                    newDuck.setVelocity(Vector2f(0.5, 0));
                    world.AddPhysicsBody(newDuck);
                    newDuck.onCollision = [&world, &arrow, &arrows, &drawingArrow, &newDuck, &ducks, &score, &rWall](PhysicsBodyCollisionResult result)
                        {
                            if (result.object2 == arrow)
                            {
                                drawingArrow = false;
                                arrows--;
                                world.RemovePhysicsBody(arrow);
                                world.RemovePhysicsBody(newDuck);
                                ducks.QueueRemove(newDuck);
                                score + 10;
                            }
                            if (result.object2 == rWall)
                            {
                                world.RemovePhysicsBody(newDuck);
                                ducks.QueueRemove(newDuck);
                            }
                        };
                }
            }//end of the duck time if

           /* for (auto duck : ducks) 
            {
                window.draw(duck);
            }*/

            window.draw(rWall); //remember this is our duck removal wall
            window.display();
            ducks.DoRemovals();

        }
    }

    //let's make the game end screen, we don't want the program to just end without warning
    bool exitGame(false);
    PhysicsRectangle backboard;
    backboard.setSize(Vector2f(500, 300)); 
    backboard.setCenter(Vector2f(400, 300)); 
    backboard.setFillColor(Color(173, 216, 230)); //light blue to contrast the black background
    backboard.setStatic(true); 

    Text gameOverT;
    gameOverT.setFont(font);
    gameOverT.setString("Game Over");
    gameOverT.setCharacterSize(fontSz);
    gameOverT.setFillColor(Color(23, 94, 3)); //nice deep green for contrast with the light blue for readability
    FloatRect goSz = gameOverT.getGlobalBounds();
    gameOverT.setPosition(Vector2f(400 - (goSz.width / 2), 200 - (goSz.height)));

    Text finalScore;
    finalScore.setFont(font);
    finalScore.setString("You scored: " + to_string(score) + " points!");
    finalScore.setCharacterSize(fontSz);
    finalScore.setFillColor(Color(23, 94, 3));
    FloatRect FinScoreSz = finalScore.getGlobalBounds();
    finalScore.setPosition(Vector2f(400 - (FinScoreSz.width / 2), 300 - (FinScoreSz.height)));

    Text leaveText;
    leaveText.setFont(font);
    leaveText.setString("Press SPACE to exit game");
    leaveText.setCharacterSize(fontSz * 0.75);
    leaveText.setFillColor(Color(0, 0, 0));
    FloatRect ltSz = leaveText.getGlobalBounds();
    leaveText.setPosition(Vector2f(400 - (ltSz.width / 2), 400 - (ltSz.height)));

    while (!exitGame) 
    {
        if (Keyboard::isKeyPressed(Keyboard::Space)) 
        {
            exitGame = true;
        }
        window.clear();
        window.draw(backboard);
        window.draw(gameOverT);
        window.draw(finalScore);
        window.draw(leaveText);
        window.display();
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
