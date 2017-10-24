#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace sf;
using namespace std;

// VARIABLES
sf::Text startText;
sf::Text restartText;
sf::Text exitText;
sf::Text winText;
sf::Text levelText;
sf::Text levelSelectText;

sf::Text score;
int playerScore;
int aiScore;

int gameSpeed;
int scoreToWin;
int pongBallY;
int pongBallX;
float r;

bool movingRight;
bool movingLeft;
bool pongMoving;

CircleShape pongBall;

// SET SCORE STRING TO CURRENT SCORE
void setScore()
{
    char temp[256];
    sprintf(temp, "%i - %i", playerScore, aiScore);
    score.setString(temp);
}

// SET BASE PARAMETERS OF PONG GAME
void setPong()
{
    gameSpeed = 7;
    scoreToWin = 11;
    
    playerScore = 0;
    aiScore = 0;
    
    pongBallY = -gameSpeed;
    if (rand() % 2 == 1)
        pongBallX = gameSpeed;
    else
        pongBallX = -gameSpeed;
    
    r = ((double)rand()) / (RAND_MAX) / 2.0 + 0.75;
    
    movingRight = false;
    movingLeft = false;
    pongMoving = false;
    
    pongBall.setRadius(10);
}

// CLEAR STRINGS FOR GAME START
void clearStrings()
{
    startText.setString("");
    restartText.setString("");
    exitText.setString("");
    levelText.setString("");
    levelSelectText.setString("");
}

// RESET TEXT AND SCORES OF PONG
void resetGame()
{
    startText.setString("Press SPACE to launch");
    levelText.setString("Select level:1-3");
    restartText.setString("Press r to restart");
    exitText.setString("Press ESC to exit");
    winText.setString("");
    playerScore = 0;
    aiScore = 0;
}

int main() {
    srand(time(0));
    
    RenderWindow window(VideoMode(800, 600, 32), "Pong");
    
    window.setFramerateLimit(60);
    
    // IMPORT FONT AND APPLY TO ALL TEXT
    sf::Font font;
    if (!font.loadFromFile("../resources/TheLedDisplaySt.ttf"))
    {
    }
    startText.setFont(font);
    startText.setString("Press space to launch");
    startText.setPosition(150, 92);
    startText.setCharacterSize(36);
    
    levelText.setFont(font);
    levelText.setString("Select level:1-3");
    levelText.setPosition(150, 128);
    levelText.setCharacterSize(36);
    
    levelSelectText.setFont(font);
    levelSelectText.setFillColor(sf::Color::Red);
    levelSelectText.setPosition(650, 128);
    levelSelectText.setCharacterSize(36);
    
    restartText.setFont(font);
    restartText.setString("Press r to restart");
    restartText.setPosition(150, 164);
    restartText.setCharacterSize(36);
    
    exitText.setFont(font);
    exitText.setString("Press ESC to exit");
    exitText.setPosition(150, 200);
    exitText.setCharacterSize(36);
    
    winText.setFont(font);
    winText.setString("");
    winText.setCharacterSize(48);
    winText.setFillColor(sf::Color::Red);
    winText.setPosition(225, 400);
    
    score.setFont(font);
    score.setCharacterSize(120);
    score.setPosition(195, 240);
    
    // IMPORT SOUND EFFECTS
    sf::SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.loadFromFile("../resources/coins.wav"))
        return EXIT_FAILURE;
    sf::Sound ballSound(ballSoundBuffer);
    
    sf::SoundBuffer gameoverSoundBuffer;
    if (!gameoverSoundBuffer.loadFromFile("../resources/gameover.wav"))
        return EXIT_FAILURE;
    sf::Sound gameOverSound(gameoverSoundBuffer);
    
    // CREATE PLAYER PADDLE
    RectangleShape playerPaddle(Vector2f(100, 20));
    playerPaddle.setPosition(350, 540);
    
    // CREATE CPU PADDLE
    RectangleShape aiPaddle(Vector2f(100, 20));
    aiPaddle.setPosition(350, 40);
    
    setPong();
    
    while (window.isOpen()) {
        Event event;
        
        window.setKeyRepeatEnabled(false);
        
        // CHECK FOR KEYBOARD EVENTS
        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    
                case Event::KeyPressed:
                    // EXIT PONG GAME
                    if (event.key.code == Keyboard::Escape)
                        window.close();
                    //MOVING PADDLE RIGHT
                    else if (event.key.code == Keyboard::Right) {
                        movingRight = true;
                        break;
                    }
                    //MOVING PADDLE LEFT
                    else if (event.key.code == Keyboard::Left) {
                        movingLeft = true;
                        break;
                    }
                    //LAUNCH BALL
                    else if (event.key.code == Keyboard::Space) {
                        clearStrings();
                        pongMoving = true;
                    }
                    //RESET GAME
                    else if (event.key.code == Keyboard::R) {
                        resetGame();
                        playerPaddle.setPosition(350, 540);
                        aiPaddle.setPosition(350, 40);
                        pongMoving = false;
                    }
                    //SELECT LEVEL
                    else if (event.key.code == Keyboard::Num1) {
                        gameSpeed = 5;
                        levelSelectText.setString("1");
                    }
                    else if (event.key.code == Keyboard::Num2) {
                        gameSpeed = 7;
                        levelSelectText.setString("2");
                    }
                    else if (event.key.code == Keyboard::Num3) {
                        gameSpeed = 9;
                        levelSelectText.setString("3");
                    }
                    
                case Event::KeyReleased:
                    if (event.key.code == Keyboard::Left)
                        movingLeft = false;
                    
                    else if (event.key.code == Keyboard::Right)
                        movingRight = false;
                    
                default:
                    break;
                    
            }
        }
        //MOVE PADDLE LEFT
        if (movingLeft == true) {
            if (playerPaddle.getPosition().x > 0)
                playerPaddle.move(-gameSpeed, 0);
        }
        //MOVE PADDLE RIGHT
        else if (movingRight == true) {
            if (playerPaddle.getPosition().x < 800 - playerPaddle.getSize().x)
                playerPaddle.move(gameSpeed, 0);
        }
        
        //LAUNCH BALL AND CHECK SCORE
        if (!pongMoving) {
            startText.setString("Press SPACE to launch");
            if(aiScore >= scoreToWin) {
                restartText.setString("Press r to restart");
                exitText.setString("Press ESC to exit");
                winText.setString("CPU Wins!");
                startText.setString("");
                
            }
            if(playerScore >= scoreToWin) {
                restartText.setString("Press r to restart");
                exitText.setString("Press ESC to exit");
                winText.setString("You win!");
                startText.setString("");
                
            }
            pongBall.setPosition(playerPaddle.getPosition().x + playerPaddle.getSize().x / 2 - 10, playerPaddle.getPosition().y - 30);
        }
        //CONTINUE TO MOVE BALL
        else if (pongMoving) {
            pongBall.move(pongBallX * r, pongBallY );
        }
        //CHECK FOR COLLISIONS WITH SIDE WALLS
        if (pongBall.getPosition().x <= 0 || pongBall.getPosition().x >= 800) {
            ballSound.play();
            pongBallX = -pongBallX;
        }
        //CHECK FOR COLLISION WITH PLAYER PADDLE
        else if (pongBall.getGlobalBounds().intersects(playerPaddle.getGlobalBounds())) {
            ballSound.play();
            pongBallY = -gameSpeed;
            r = ((double)rand()) / (RAND_MAX) / 2.0 + 0.75;
        }
        //CHECK FOR COLLISION WITH CPU PADDLE
        else if (pongBall.getGlobalBounds().intersects(aiPaddle.getGlobalBounds())) {
            ballSound.play();
            pongBallY = gameSpeed;
            r = ((double)rand()) / (RAND_MAX) / 2.0 + 0.75;
        }
        //CHECK FOR POINT TO CPU
        else if (pongBall.getPosition().y > 600) {
            ++aiScore;
            setScore();
            gameOverSound.play();
            pongMoving = false;
            pongBallY = -gameSpeed;
        }
        //CHECK FOR POINT TO PLAYER
        else if (pongBall.getPosition().y < 0) {
            ++playerScore;
            setScore();
            gameOverSound.play();
            pongMoving = false;
            pongBallY = -gameSpeed;
        }
        //MOVE CPU TO FOLLOW BALL
        if (pongMoving) {
            if (aiPaddle.getPosition().x > pongBall.getPosition().x - 40)
                aiPaddle.move(-gameSpeed, 0);
            else if (aiPaddle.getPosition().x < pongBall.getPosition().x - 40)
                aiPaddle.move(gameSpeed, 0);
        }
        setScore();
        
        //RENDER WINDOW AND DRAW ALL COMPONENTS
        window.clear();
        window.draw(playerPaddle);
        window.draw(aiPaddle);
        window.draw(pongBall);
        window.draw(startText);
        window.draw(levelText);
        window.draw(levelSelectText);
        window.draw(winText);
        window.draw(exitText);
        window.draw(restartText);
        window.draw(score);
        
        window.display();
    }
}
