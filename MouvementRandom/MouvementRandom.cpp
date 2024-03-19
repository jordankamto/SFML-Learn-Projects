// MouvementRandom.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>
#include <thread>

void wait(std::chrono::milliseconds duration) {
	std::this_thread::sleep_for(duration);
}


int main()
{
	unsigned int width = 700;
	unsigned int height = 500;
	sf::RenderWindow window(sf::VideoMode(width, height), "Mouvement Random");
	

	float rayon = 20;
	sf::CircleShape shape(rayon);
	shape.setOrigin(rayon, rayon);
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(width / 2.0, height / 2.0);

	sf::Clock clock;
	while (window.isOpen())
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		int lower_bound = -5;
		int upper_bound = 5;

		std::uniform_int_distribution<> distrib(lower_bound, upper_bound);

		int random_number = distrib(gen);

		sf::Vector2f speed;
		
		speed.x = distrib(gen);
		speed.y = distrib(gen);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		if (clock.getElapsedTime().asMilliseconds() > 1000. / 60.0) {
			
			shape.move(speed);
			shape.move(speed);
			shape.move(speed);

			clock.restart();
		}

		window.clear();
		window.draw(shape);
		window.display();
		
	}
	return 0;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
