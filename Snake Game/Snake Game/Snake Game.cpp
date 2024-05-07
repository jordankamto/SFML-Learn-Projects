#include<SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<iostream>
#include<Windows.h>
#include<string>

#pragma region MesVariablesGlobales
int wTileNum = 56, hTileNum = 40, tileSize = 15;
int windowWidth = 840 , windowHeight = 600;
double gameTime =0, gameTimeDelay = 0.1, foodX, foodY;
int snakeDirection, snakeSize = 2;
sf::RenderWindow mainMenuWindow, gameWindow, gameOverWindow;
sf::Event mainMenuEvent, gameEvent, gameOverEvent;
sf::Texture playBoutonTexture, exitBoutonTexture, creditsBoutonTexture, mainMenubgTexture, gameTerrainTexture, retryTexture;
sf::Sprite playBoutonSprite, exitBoutonSprite, creditsBoutonSprite, mainMenubgSprite, gameTerrainSprite, retrySprite;
sf::SoundBuffer mainMenuSoundBuffer, mainMenuBoutonSoundBuffer, gamebgSoundBuffer, snakeEatBuffer, snakeDeathBuffer;
sf::Sound mainMenuSound, mainMenuBoutonSound, gamebgSound, snakeEatSound, snakeDeatSound;
sf::Font gameFont;
sf::Text gameScoreDisplay, hiScoreDisplay;
std::string gameScorestring, hiScorestring;
int gameScore = 0, hiScore = 0;
sf::Texture snakeBodyTexture;
sf::Sprite snakeHead, snakeBody, foodSprite;
sf::Texture foodTexture;
sf::Clock gameClock;
sf::Vector2i snakePosition;
bool pause = false;
#pragma endregion MesVariablesGlobales

#pragma region MesFontions
void majScore();
void afficherCredits();
void dessinerSerpent();
void dessinerNourriture();
void mouvementSerpent();
void majDifficulte();
void gestionCollision();
void lancerJeu();
void initialiserJeu();
#pragma endregion MesFonctions

//structure pour definir le serpent
struct Serpent
{
	int x, y;
} 
snake[100];

void afficherCredits()
{
	//code pour affichage des credits du jeu
	std::cout << "JEU FAIT PAR KAMGAING K JORDAN STEVE" << std::endl;
	std::cout << "SOUS LA SUPERVISION DE ING TEUGUIA RODOLPHE" << std::endl;
	std::cout << "ECOLE NATIONALE SUPERIEUR POLYTECHNIQUE DE YAOUNDE" << std::endl;
	std::cout << "MADE WITH ASSETS FROM https://fabioarnold.itch.io/snake" << std::endl;
	std::cout << " " << std::endl;
}

void dessinerSerpent()
{

	if (!snakeBodyTexture.loadFromFile("ingame/snake_body.png"))
	{
		std::cout << "Erreur lors du chargement de snake_body.png" << std::endl;
	}

	snakeBodyTexture.setSmooth(true);

	//assignation de la texture de depart a la tete du serpent
	
	snakeBody.setTexture(snakeBodyTexture);

}

void dessinerNourriture()
{
	//importation des textures de la pomme
	if (!foodTexture.loadFromFile("ingame/food.png"))
	{
		std::cout << "Erreur lors du chargement de food.png" << std::endl;
	}
	foodTexture.setSmooth(true);
	foodSprite.setTexture(foodTexture);

	//selection d'une position aleatoire pour l'affichage de nourriture
	foodX = rand() % (wTileNum - 1);
	foodY = rand() % (hTileNum - 1);
	if (foodX  >= 0 && foodX <= 3) foodX = 4;
	if (foodX >= 0 && foodX <= 4) foodY = 4;
	foodSprite.setPosition(sf::Vector2f(foodX * tileSize, foodY * tileSize));
}

void mouvementSerpent()
{
	//mise a jour de la position du serpent en decalant chaque segment  l'avant
	for (int i = snakeSize; i > 0; --i)
	{
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}

	//mise a jour de la position de la tete du serpent en fonction de la direction actuelle
	if (snakeDirection == 0)
	{
		// deplacement vers le bas
		snake[0].y += 1;
		snakePosition.y += 15;
	}
	if (snakeDirection == 1)
	{
		// deplacement vers la gauche
		snake[0].x -= 1;
		snakePosition.x -= 15;
	}
	if (snakeDirection == 2)
	{
		// deplacement vers la droite
		snake[0].x += 1;
		snakePosition.x += 15;
	}
	if (snakeDirection == 3)
	{
		// deplacement vers le haut
		snake[0].y -= 1;
		snakePosition.y -= 15;
	}

}

void gameOver()
{
	gamebgSound.stop();
	pause = true;
	snakeDeatSound.play();
	Sleep(1500);
	gameWindow.close();

	gameOverWindow.create(sf::VideoMode(windowWidth, windowHeight), "Game over - Snake Game 🐍");
	gameOverWindow.setPosition(sf::Vector2i(0, 0));

	if (!retryTexture.loadFromFile("mainmenu/retry.png"))
	{
		std::cout << "Erreur lors du chargement de retry.png" << std::endl;
	}

	if (!exitBoutonTexture.loadFromFile("mainmenu/ExitBouton.png"))
	{
		std::cout << "Erreur lors du chargement de ExitBouton.png" << std::endl;
	}
	if (!mainMenubgTexture.loadFromFile("mainmenu/bgMainmenu.png"))
	{
		std::cout << "Erreur lors du chargement de bgMainmenu.png" << std::endl;
	}

	mainMenuBoutonSound.setBuffer(mainMenuBoutonSoundBuffer);
	mainMenubgTexture.setSmooth(true);
	mainMenubgSprite.setTexture(mainMenubgTexture);

	exitBoutonTexture.setSmooth(true);
	retryTexture.setSmooth(true);

	exitBoutonSprite.setTexture(exitBoutonTexture);
	retrySprite.setTexture(retryTexture);

	exitBoutonSprite.setOrigin(sf::Vector2f(72, 27));
	retrySprite.setOrigin(sf::Vector2f(92, 27));

	retrySprite.setPosition(sf::Vector2f(416, 375));
	exitBoutonSprite.setPosition(sf::Vector2f(416, 435));

	while (gameOverWindow.isOpen())
	{
		while (gameOverWindow.pollEvent(gameOverEvent))
		{
			if (gameOverEvent.type == sf::Event::Closed)
			{
				gameOverWindow.close();
			}
			if (gameOverEvent.type == sf::Event::MouseButtonPressed)
			{
				/*std::cout << sf::Mouse::getPosition(gameOverWindow).x << std::endl;
				std::cout << sf::Mouse::getPosition(gameOverWindow).y << std::endl;*/

				if (gameOverEvent.mouseButton.x >= 326 && gameOverEvent.mouseButton.x <= 502)
				{
					if (gameOverEvent.mouseButton.y >= 350 && gameOverEvent.mouseButton.y <= 400)
					{
						//effet sonore sur clic de bouton
						mainMenuBoutonSound.play();
						Sleep(500);
						gameOverWindow.close();
						
						//effet de clic sur le bouton retry
						lancerJeu();
					}
				}
				if (gameOverEvent.mouseButton.x >= 347 && gameOverEvent.mouseButton.x <= 483)
				{
					if (gameOverEvent.mouseButton.y >= 413 && gameOverEvent.mouseButton.y <= 460)
					{
						//effet sonore sur clic de bouton
						mainMenuBoutonSound.play();
						Sleep(500);
						gameOverWindow.close();
						
					}
				}
			}
		}

		gameOverWindow.draw(mainMenubgSprite);
		gameOverWindow.draw(retrySprite);
		gameOverWindow.draw(exitBoutonSprite);
		gameOverWindow.display();
		gameOverWindow.clear();
	}
}

void majScore()
{
	if (gameScore > hiScore) hiScore = gameScore;
}

void majDifficulte()
{
	if (gameScore % 5 == 0 && gameScore != 0) gameTimeDelay -= 0.01;
}

void finDePartie()
{
	//mettre le jeu en pause en arriere plan et afficher une fenetre relancer une partie (fermer la fenetre de jeu et la fenetre de pause et relancerla fonction jeu)
	//verification de collision  avec les murs de la fenetre de jeu
	if (snake[0].x > wTileNum - 1 || snake[0].x < 0 || snake[0].y > hTileNum - 1 || snake[0].y < 0)
	{
		std::cout << "Game Over" << std::endl;
		gameOver();
	}

	//vertifier si la tete du serpent entre en collision avec son propre coprs
	for (int i=1; i < snakeSize; i++)
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
		{
			std::cout << "Game Over" << std::endl;
			gameOver();
		}
	}
}

void gestionCollision()
{
	//gestion des collisions avec les murs (mort)
	//snake_death_sound.mp3

	//gestion des collisons avec la pomme (ajout de point)
	//snake_eat_sound.mp3
	if ((snake[0].x == foodX) && (snake[0].y == foodY))
	{
		//// deboggage zone de collision pomme
		//std::cout << snakePosition.x << std::endl;
		//std::cout << snakePosition.y << std::endl;
		//std::cout << " " << std::endl;
		//std::cout << foodSprite.getPosition().x << std::endl;
		//std::cout << foodSprite.getPosition().y << std::endl;
		//std::cout << " " << std::endl;

		//augmenter la taille du serpent et du score
		snakeEatSound.play();
		snakeSize++;
		gameScore++;
		majScore();
		majDifficulte();
		dessinerNourriture();
	}

	finDePartie();

}

void lancerJeu()
{
	//reinitialisation du jeu a chaque lancement
	bool pause = false;
	gameScore = 0;
	snakeDirection = 0;
	snakeSize = 2;
	snake[0].x = 0;
	snake[0].y = 0;
	gameTime = 0, gameTimeDelay = 0.1;


	//code pour lancement d'une nouvelle partie de jeu
	gameWindow.create(sf::VideoMode(windowWidth, windowHeight), "Playing - Snake Game 🐍");
	gameWindow.setPosition(sf::Vector2i(0, 0));
	if (!gamebgSoundBuffer.loadFromFile("audio/bgSnakegameSound.mp3"))
	{
		std::cout << "Erreur lors du chragement de audio/bgSnakegameSound.mp3" << std::endl;
	}
	gamebgSound.setBuffer(gamebgSoundBuffer);

	if (!snakeEatBuffer.loadFromFile("audio/snake_eat_sound.mp3"))
	{
		std::cout << "Erreur lors du chragement de audio/snake_eat_sound.mp3" << std::endl;
	}
	snakeEatSound.setBuffer(snakeEatBuffer);
	if (!snakeDeathBuffer.loadFromFile("audio/snake_death_sound.mp3"))
	{
		std::cout << "Erreur lors du chragement de audio/snake_death_sound.mp3" << std::endl;
	}
	snakeDeatSound.setBuffer(snakeDeathBuffer);
	gamebgSound.setLoop(true);
	gamebgSound.play();

	// importation des textures de terrain
	if (!gameTerrainTexture.loadFromFile("ingame/terrain_snake_game.png"))
	{
		std::cout << "Erreur lors du chargement de terrain_snake_game.png" << std::endl;
	}
	gameTerrainTexture.setSmooth(true);
	gameTerrainSprite.setTexture(gameTerrainTexture);
	
	
	//chargement des polices d'affichage pour les Scores du jeu
	if (!gameFont.loadFromFile("fonts/Roboto-Regular.ttf"))
	{
		std::cout << "Erreur lors du chargement  de fonts/Roboto-Regular.ttf" << std::endl;
	}
	gameScoreDisplay.setFont(gameFont);
	hiScoreDisplay.setFont(gameFont);
	gameScoreDisplay.setCharacterSize(22);
	hiScoreDisplay.setCharacterSize(22);
	gameScoreDisplay.setFillColor(sf::Color::White);
	hiScoreDisplay.setFillColor(sf::Color::White);
	hiScoreDisplay.setStyle(sf::Text::Bold);
	gameScoreDisplay.setPosition(sf::Vector2f(20, 20));
	hiScoreDisplay.setPosition(sf::Vector2f(620, 20));

	dessinerNourriture();
	dessinerSerpent();

	while (gameWindow.isOpen())
	{
		gameTime += gameClock.getElapsedTime().asSeconds();
		gameClock.restart();
		
		while (gameWindow.pollEvent(gameEvent))
		{
			if (gameEvent.type == sf::Event::Closed)
			{
				gameWindow.close();
			}

			//Debuggage au clic de souris
			//if (gameEvent.type == sf::Event::MouseButtonPressed)
			//{
			//	std::cout << sf::Mouse::getPosition().x << std::endl;
			//	std::cout << sf::Mouse::getPosition().y << std::endl;

			//	////augmenter la taille du serpent et du score
			//	//snakeSize++;
			//	//gameScore++;
			//	//majScore();
			//	//majDifficulte();
			//	//dessinerNourriture();
			//}
		}
		
		//gestion des entrees du joueur pour changer la direction du serpent
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && snakeDirection != 2)
		{
			snakeDirection = 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && snakeDirection != 1)
		{
			snakeDirection = 2;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && snakeDirection != 0)
		{
			snakeDirection = 3;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && snakeDirection != 3)
		{
			snakeDirection = 0;
		}

		if (!pause)
		{
			if (gameTime > gameTimeDelay)
			{
				gameTime = 0;
				mouvementSerpent();
				gestionCollision();

			}
		}

		
		//assignation des character d'affichages au texte a afficher
		gameScorestring = "Votre Score : " + std::to_string(gameScore);
		hiScorestring = "Meilleur Score : " + std::to_string(hiScore);

		gameScoreDisplay.setString(gameScorestring);
		hiScoreDisplay.setString(hiScorestring);

		gameWindow.clear();

		// affichage du terrain
		for (int i = 0; i < wTileNum; i++)
		{
			for (int j = 0; j < hTileNum; j++)
			{
				gameTerrainSprite.setPosition(i * tileSize, j * tileSize);
				gameWindow.draw(gameTerrainSprite);

			}
		}

		//affichage du serpent en deplacement
		for (int i = 0; i < snakeSize; i++)
		{
			snakeBody.setPosition(snake[i].x * tileSize, snake[i].y * tileSize);
			gameWindow.draw(snakeBody);
		}

		//affichage des scores
		gameWindow.draw(gameScoreDisplay);
		gameWindow.draw(hiScoreDisplay);

		gameWindow.draw(foodSprite);
		gameWindow.display();
	}
}

void initialiserJeu()
{
	//afficher la fenetre du menu principal
	mainMenuWindow.create(sf::VideoMode(windowWidth, windowHeight), "Main menu - Snake Game 🐍");
	mainMenuWindow.setPosition(sf::Vector2i(0, 0));

	//initialisation des boutons de l'interface
	if (!playBoutonTexture.loadFromFile("mainmenu/PlayBouton.png"))
	{
		std::cout << "Erreur lors du chargement de PlayBouton.png" << std::endl;
	}
	if (!exitBoutonTexture.loadFromFile("mainmenu/ExitBouton.png"))
	{
		std::cout << "Erreur lors du chargement de ExitBouton.png" << std::endl;
	}
	if (!creditsBoutonTexture.loadFromFile("mainmenu/CreditsBouton.png"))
	{
		std::cout << "Erreur lors du chargement de CreditsBouton.png" << std::endl;
	}
	if (!mainMenubgTexture.loadFromFile("mainmenu/bgMainmenu.png"))
	{
		std::cout << "Erreur lors du chargement de bgMainmenu.png" << std::endl;
	}
	

	playBoutonTexture.setSmooth(true);
	exitBoutonTexture.setSmooth(true);
	creditsBoutonTexture.setSmooth(true);
	mainMenubgTexture.setSmooth(true);
	retryTexture.setSmooth(true);

	playBoutonSprite.setTexture(playBoutonTexture);
	exitBoutonSprite.setTexture(exitBoutonTexture);
	creditsBoutonSprite.setTexture(creditsBoutonTexture);
	mainMenubgSprite.setTexture(mainMenubgTexture);
	

	playBoutonSprite.setPosition(sf::Vector2f(433, 390));
	creditsBoutonSprite.setPosition(sf::Vector2f(433, 450));
	exitBoutonSprite.setPosition(sf::Vector2f(433, 510));
	

	playBoutonSprite.setOrigin(sf::Vector2f(96, 27));
	creditsBoutonSprite.setOrigin(sf::Vector2f(112, 27));
	exitBoutonSprite.setOrigin(sf::Vector2f(72, 27));
	
	

	//initialisation du son d'arriere plan
	if (!mainMenuSoundBuffer.loadFromFile("audio/snake_game_audio.mp3"))
	{
		std::cout << "Erreur lors du chargement de snake_game_audio.mp3" << std::endl;
	}
	if (!mainMenuBoutonSoundBuffer.loadFromFile("audio/snakeGameBoutonSound.wav"))
	{
		std::cout << "Erreur lors du chargement de snakeGameBoutonSound.wav" << std::endl;
	}

	mainMenuSound.setBuffer(mainMenuSoundBuffer);
	mainMenuBoutonSound.setBuffer(mainMenuBoutonSoundBuffer);
	mainMenuSound.setLoop(true);
	mainMenuSound.play();

	//boucle de lancement de la fenetre principal
	while (mainMenuWindow.isOpen())
	{
		while (mainMenuWindow.pollEvent(mainMenuEvent))
		{
			if (mainMenuEvent.type == sf::Event::Closed)
			{
				mainMenuWindow.close();
			}

			//gestions des entres via la souris sur le menu principal
			if (mainMenuEvent.type == sf::Event::MouseButtonPressed)
			{
				if (mainMenuEvent.mouseButton.x >= 336 && mainMenuEvent.mouseButton.x <= 517)
				{
					if (mainMenuEvent.mouseButton.y >= 368 && mainMenuEvent.mouseButton.y <= 416)
					{
						//effet sonore sur clic de bouton
						mainMenuBoutonSound.play();
						Sleep(500);
						mainMenuWindow.close();
						mainMenuSound.stop();
						//effet de clic sur le bouton play
						lancerJeu();
					}
				}
				if (mainMenuEvent.mouseButton.x >= 325 && mainMenuEvent.mouseButton.x <= 551)
				{
					if (mainMenuEvent.mouseButton.y >= 425 && mainMenuEvent.mouseButton.y <= 473)
					{
						//effet sonore sur clic de bouton
						mainMenuBoutonSound.play();

						//effet de clic sur le bouton credits
						afficherCredits();
					}
				}
				if (mainMenuEvent.mouseButton.x >= 364 && mainMenuEvent.mouseButton.x <= 497)
				{
					if (mainMenuEvent.mouseButton.y >= 487 && mainMenuEvent.mouseButton.y <= 534)
					{
						//effet sonore sur clic de bouton
						mainMenuBoutonSound.play();
						Sleep(1000);
						//effet de clic sur le bouton exit
						mainMenuWindow.close();
					}
				}
				
				//std::cout << mainMenuEvent.mouseButton.x << std::endl;
				//std::cout << mainMenuEvent.mouseButton.y << std::endl;
			}

		}

		mainMenuWindow.draw(mainMenubgSprite);
		mainMenuWindow.draw(playBoutonSprite);
		mainMenuWindow.draw(creditsBoutonSprite);
		mainMenuWindow.draw(exitBoutonSprite);
		mainMenuWindow.display();
		mainMenuWindow.clear();
	}

	//dessiner les formes et textes du menu principal

	//gerer les inputs sur les boutons du menu principal

	//gerer le lancement du jeu en cas de nouvelle partie

	//afficher les formes et textes du menu principale
}

int main()
{
	initialiserJeu();
	//lancerJeu();
	//gameOver();
	return 0;
}