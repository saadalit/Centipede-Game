#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {0};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

// functions declaration

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void moveBullet(float bullet[], sf::Clock& bulletClock);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
void playerMovement(float player[]);
void shootBullet(float player[], float bullet[], sf::Clock& bulletClock);
void drawMushrooms(sf::RenderWindow& window, sf::Sprite& mushroomSprite);
void placeMushrooms();
void placeCentipede(float centipedemov[][2]);
void drawCentipede(sf::RenderWindow& window, float centipedemov[][2], sf::Sprite& centipedeSprite, sf::Sprite& centipedebodySprite, bool direction[]);
void moveCentipede(float centipedemov[][2], sf::Clock& centipedeClock, bool direction[]);
void checkMushroomcollision(float bullet[], int& score) ;
void moveCentipedeinplayer(float centipedemov[][2]) ;
void centipedeCollisionplayer(float centipedemov[][2], float player[], sf::RenderWindow& window);
void showGameOver(sf::RenderWindow& window);

int main()
{
	srand(time(0));

	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	// window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/M.O.O.N._Hydrogen.ogg");
	bgMusic.play();
	bgMusic.setVolume(50);

	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20)); 

	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;
	player[y] = (gameColumns * 3 / 4) * boxPixelsY;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y] - boxPixelsY;
	bullet[exists] = false;
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Mushroom and Mushroom Sprites.
	bool mushroom = false ;
	float mushroomloc[20][2] = {} ;
	sf::Texture mushroomTexture;
	sf::Sprite mushroomSprite;
	mushroomTexture.loadFromFile("Textures/mushroom.png");
	mushroomSprite.setTexture(mushroomTexture);
	mushroomSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Centipede and Centipede Sprites.
	bool centipede = false ;
	bool direction[12] = {false} ;
	float centipedemov[12][2] = {};
	sf::Texture centipedeTexture;
	sf::Sprite centipedeSprite;
	sf::Texture centipedebodyTexture;
	sf::Sprite centipedebodySprite;
	sf::Clock centipedeClock;
	centipedeTexture.loadFromFile("Textures/c_head_left_walk.png");
	centipedeSprite.setTexture(centipedeTexture);
	centipedeSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	centipedebodyTexture.loadFromFile("Textures/c_body_left_walk.png");
	centipedebodySprite.setTexture(centipedebodyTexture);
	centipedebodySprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	//Initializing score and its textures
	int score = 0;
	sf::Font font;
	font.loadFromFile("Textures/SfDigitalReadoutHeavy-AwVg.ttf"); 
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(60);
	scoreText.setFillColor(sf::Color::White);

	while(window.isOpen()) //loop that iterates till game window is closed
	{
		
		window.draw(backgroundSprite);		//setting background
		drawPlayer(window, player, playerSprite);	//drawing the player
		playerMovement(player);		//controlling player movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))	//checks if spacebar is pressed to shoot bullet
		{
            shootBullet(player, bullet, bulletClock);	//shoots bullet
        }

        if (bullet[exists]==true)		//moves and draws bullet in each iteration
		{
            moveBullet(bullet, bulletClock);
            drawBullet(window, bullet, bulletSprite);
        }
		if(mushroom == false)	//places mushroom in first iteration
		{
			placeMushrooms();
			mushroom = true ;
		}
		drawMushrooms(window, mushroomSprite);	//draws mushrooms in every iterarion
		checkMushroomcollision(bullet, score);	//checks for bullet collision with mushroom
		if(centipede == false)
		{
			placeCentipede(centipedemov);	//places centipede in first iteration
			centipede = true ;
		}
		drawCentipede(window, centipedemov, centipedeSprite, centipedebodySprite, direction);	//draws and moves centipede in every iteration
		moveCentipede(centipedemov, centipedeClock, direction);
		centipedeCollisionplayer(centipedemov, player, window);
		
		sf::Event e;
		while (window.pollEvent(e))			//handles closing of game console
		{
			if (e.type == sf::Event::Closed)
			{
				return 0;
			}
			
		}
		scoreText.setString("Score: " + to_string(score));		//displays the score of the player on the game console
		window.draw(scoreText);		
		window.display();
		window.clear();
	}
}

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite)	//function that draws player on the game console
{
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}
void moveBullet(float bullet[], sf::Clock& bulletClock)		//function that controls movement of bullet
{
	if (bulletClock.getElapsedTime().asMilliseconds() < 50)
		return;

	bulletClock.restart();
	bullet[y] -= 32;		//bullet moves one box up in every iteration
	if (bullet[y] < -32)
		bullet[exists] = false;
}
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite)		//function that draws bullet on the game console
{
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}
void playerMovement(float player[])		//function that controls players movement on key presses
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player[x] > 0)	//left movement
	{
        player[x] -= 2;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player[x] < 928)	//right movement
	{
        player[x] += 2; 
    }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player[y] < 928)	//down movement
	{
        player[y] += 2;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player[y] > 800)	//up movement
	{
        player[y] -= 2; 
    }
}
void shootBullet(float player[], float bullet[], sf::Clock& bulletClock)		//function that initializes movement of bullet when it is shot
{

    if (bullet[exists]==false)		//initializes bullet position when it is shot
	{
        bullet[x] = player[x];
        bullet[y] = player[y] - 32;		
        bullet[exists] = true;
        bulletClock.restart();
    }
}
void placeMushrooms()							//assigns value to game grid where mushrooms are to be placed
{
	for(int i=0; i<gameRows-5; i++)
	{
		for(int j=0; j<gameColumns; j++)
		{
			if(rand()%50 == 0)
			{
				gameGrid[i][j] = 1;				//assign value 1 to game grid where mushrooms are to be placed
			}
		}
	}
}
void drawMushrooms(sf::RenderWindow& window, sf::Sprite& mushroomSprite)	//function to draw mushrooms on the game grid
{
	for(int i=0; i<gameRows-5; i++)
	{
		for(int j=0; j<gameColumns; j++)
		{
			if(gameGrid[i][j]==1)											//draws mushrooms on the game grid with value 1
			{
				mushroomSprite.setPosition(j*gameColumns,i*gameRows);
	  			window.draw(mushroomSprite);
			}
		}
	}
}
void placeCentipede(float centipedemov[][2])		//function that places the centipede randomly in the top 10 rows
{
	const float initialxpos = (rand()%30)*32 ;	//placing the head
	const float initialypos = (rand()%10)*32 ;
	centipedemov[0][x] = initialxpos ;
	centipedemov[0][y] = initialypos ;
	for(int i=1; i<12; i++)
	{
		centipedemov[i][x] = centipedemov[i-1][x] + boxPixelsX;	//placing the next 11 segments
		centipedemov[i][y] = initialypos;
	}
	
}
void drawCentipede(sf::RenderWindow& window, float centipedemov[][2], sf::Sprite& centipedeSprite, sf::Sprite& centipedebodySprite, bool direction[])	//function that draws the centipede according to direction
{
	for(int i=0; i<12; i++)		//iterates to draw 12 segments
	{
		if(i==0)				//for head segment
		{
			centipedeSprite.setScale(1.0f, 1.0f);	//left drawing
			centipedeSprite.setPosition(centipedemov[i][x], centipedemov[i][y]);
			if(direction[i] == true)				//right drawing
			{
				centipedeSprite.setScale(-1.0f, 1.0f);		//this reverses the picture of centipede
				centipedeSprite.setPosition(centipedemov[i][x], centipedemov[i][y]);
			}
			window.draw(centipedeSprite);
		}
		else 					//for the rest 11 segments
		{
			centipedebodySprite.setScale(1.0f, 1.0f);	//left drawing
			centipedebodySprite.setPosition(centipedemov[i][x], centipedemov[i][y]);
			if(direction[i] == true)					//right drawing
			{
				centipedebodySprite.setScale(-1.0f, 1.0f);	//this reverses the picture of centipede
				centipedebodySprite.setPosition(centipedemov[i][x], centipedemov[i][y]);
			}
			window.draw(centipedebodySprite);
		}
	}
}

void moveCentipede(float centipedemov[][2], sf::Clock& centipedeClock, bool direction[])		//function for the movement of the centipede
{
    const int centipedeLength = 12;		//variables for use in the function
    if (centipedeClock.getElapsedTime().asMilliseconds() < 75)		//controls the speed of the centipede
    {
        return;
    }
	for (int i = 0; i < centipedeLength; i++)
	{
        if (direction[i] == false)
		{
            centipedemov[i][x] -= boxPixelsX; // to move centipede left 
			if(centipedemov[i][x]<0)
			{
        		centipedemov[i][y] += boxPixelsY;		//to move centipede one row down
				direction[i] = !direction[i]; 			//flag to indicate change in direction each time row is changed
			}
        } 
		else
		{
            centipedemov[i][x] += boxPixelsX; // to move centipede right
			if(centipedemov[i][x]>960)
			{
        		centipedemov[i][y] += boxPixelsY;		//to move centipede one row down
				direction[i] = !direction[i]; 			//flag to indicate change in direction each time row is changed
			}
        }
		if(centipedemov[i][y]>=800)				//after centipede crosses player area the player area movement function of the centipede is called in every iteration
		{
			moveCentipedeinplayer(centipedemov);
		}
    }
	
		
    centipedeClock.restart();
}

void checkMushroomcollision(float bullet[], int& score)		//checks for collision of bullet with mushroom
{
    int bulletGridX = static_cast<int>((bullet[x]+32) / boxPixelsX);	//forms grid for the bullet and makes it easier for collision checking
    int bulletGridY = static_cast<int>(bullet[y] / boxPixelsY);

    if (bulletGridX >= 0 && bulletGridX < gameColumns && bulletGridY >= 0 && bulletGridY < gameRows - 5) 
    {
        if (gameGrid[bulletGridY][bulletGridX] == 1) //condition to check if bullet has collided with mushroom
        {
            gameGrid[bulletGridY][bulletGridX] = 0; // removes mushroom
            score += 1;								//increases score
            bullet[exists] = false; 				//removes bullet after collision
        }
    } 
	
}

void moveCentipedeinplayer(float centipedemov[][2])		//function that allows centipede to move within player area
{
    for (int i = 0; i < 12; i++)
	{
        if (centipedemov[i][y] >= gameRows * boxPixelsY)	//when centipede reaches the last row
		{
            centipedemov[i][y] = (gameRows - 5) * boxPixelsY;	//it is sent back to the top row of player area 
        }
    }
}

void centipedeCollisionplayer(float centipedemov[][2], float player[], sf::RenderWindow& window)	//this function checks for collision between player and centipede
{
	for(int i=0; i<12; i++)
	{
		if(centipedemov[i][x] >= player[x] && centipedemov[i][x] < player[x] + boxPixelsX && centipedemov[i][y] >= player[y] && centipedemov[i][y] < player[y] + boxPixelsY)	//checks for the collision
		{
			player[x] = 992 ;		//sends player sprite out of screen so it can not be seen (shows elimination)
			player[y] = 992 ;
			showGameOver(window) ;	//calling game over function to display game over screen
		}
	}
}

void showGameOver(sf::RenderWindow& window)		//function that displays the game over screen when centipede and player collide
{
	//setting styles and font for game over text
    sf::Font font;
    font.loadFromFile("Textures/SfDigitalReadoutHeavy-AwVg.ttf");
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over!");
    gameOverText.setCharacterSize(100);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setStyle(sf::Text::Bold);

    gameOverText.setPosition(300, 400);		//sets position of game over on screen

    window.clear();
    window.draw(gameOverText);		//Draws the gane over screen
    window.display();
	sf::sleep(sf::seconds(20));
}

