// TP_Deplacements_WASD.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <string>


#include <SFML/Graphics.hpp>

int main()
{
    
    unsigned int largeur = 500;
    unsigned int hauteur = 500;
    std::string titre = "TP_Deplacements_WASD";

    sf::RenderWindow window(sf::VideoMode(largeur, hauteur), titre, sf::Style::Titlebar | sf::Style::Close);


    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Blue);

    
    sf::Vector2f move;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                
                if (event.key.code == sf::Keyboard::W) {
                    // 
                    move.y -= 1;
                }

                else if (event.key.code == sf::Keyboard::S)
                {
                    move.y += 1;
                }

                else if (event.key.code == sf::Keyboard::A)
                {
                    move.x -= 1;
                }

                else if (event.key.code == sf::Keyboard::D)
                {
                    move.x += 1;
                }
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::W) {
                    // 
                    move.y = 0;
                }

                else if (event.key.code == sf::Keyboard::S)
                {
                    move.y = 0;
                }

                else if (event.key.code == sf::Keyboard::A)
                {
                    move.x = 0;
                }

                else if (event.key.code == sf::Keyboard::D)
                {
                    move.x = 0;
                }
            }

        }
        sf::Vector2f newPosition = shape.getPosition();
        newPosition = newPosition + move;

        shape.setPosition(newPosition);

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
