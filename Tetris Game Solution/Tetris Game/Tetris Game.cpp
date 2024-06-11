
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cstdlib>
#include<iostream>
#include<Windows.h>

#define PI 3.14159265359
sf::Sprite mainmenu_sprite;
sf::Texture mainmenu_texture;
sf::SoundBuffer mainmenu_soundbuffer;
sf::Sound mainmenu_sound;

typedef uint8_t byte;

byte grid[20][12];
byte colliders[20][12];

struct Vec2 {
    byte y, x;
    Vec2() {}
    Vec2(byte dy, byte dx) : x(dx), y(dy) {}
};

struct Piece {
    Vec2 a, b, c, d;
    Piece() {}
    Piece(byte ax, byte ay, byte bx, byte by, byte cx, byte cy, byte dx, byte dy)
        : a(Vec2(ax, ay)), b(Vec2(bx, by)), c(Vec2(cx, cy)), d(Vec2(dx, dy)) {}
    Piece(Vec2 da, Vec2 db, Vec2 dc, Vec2 dd)
        : a(da), b(db), c(dc), d(dd) {}
};

enum PIECE_TYPE {
    S, Z, L, J, SQR, I, T
};

Piece CreatePiece(PIECE_TYPE type) {
    switch (type) {
    case S: return Piece(Vec2(1, 5), Vec2(1, 4), Vec2(0, 4), Vec2(2, 5));
    case Z: return Piece(Vec2(1, 4), Vec2(1, 5), Vec2(0, 5), Vec2(2, 4));
    case L: return Piece(Vec2(1, 5), Vec2(1, 4), Vec2(1, 6), Vec2(0, 6));
    case J: return Piece(Vec2(1, 5), Vec2(1, 4), Vec2(1, 6), Vec2(0, 4));
    case SQR: return Piece(Vec2(1, 5), Vec2(1, 4), Vec2(0, 5), Vec2(0, 4));
    case I: return Piece(Vec2(1, 5), Vec2(1, 4), Vec2(1, 6), Vec2(1, 7));
    case T: return Piece(Vec2(1, 5), Vec2(1, 4), Vec2(1, 6), Vec2(0, 5));
    default: return Piece(Vec2(0, 0), Vec2(0, 0), Vec2(0, 0), Vec2(0, 0)); // Cas par défaut pour éviter les avertissements
    }
}

void rotate(Piece& piece) {
    // Rotation de la pièce de 90 degrés
    float angle = 90 * (PI / 180);

    float b_offset_x = piece.b.x - piece.a.x;
    float c_offset_x = piece.c.x - piece.a.x;
    float d_offset_x = piece.d.x - piece.a.x;

    float b_offset_y = piece.b.y - piece.a.y;
    float c_offset_y = piece.c.y - piece.a.y;
    float d_offset_y = piece.d.y - piece.a.y;

    float pbx = piece.a.x + (b_offset_x * cosf(angle) - b_offset_y * sinf(angle));
    float pby = piece.a.y + (b_offset_x * sinf(angle) + b_offset_y * cosf(angle));

    float pcx = piece.a.x + (c_offset_x * cosf(angle) - c_offset_y * sinf(angle));
    float pcy = piece.a.y + (c_offset_x * sinf(angle) + c_offset_y * cosf(angle));

    float pdx = piece.a.x + (d_offset_x * cosf(angle) - d_offset_y * sinf(angle));
    float pdy = piece.a.y + (d_offset_x * sinf(angle) + d_offset_y * cosf(angle));

    // Empêche la rotation si cela entraînerait une collision avec des objets ou des murs
    if (colliders[(byte)pby][(byte)pbx] != 2 &&
        colliders[(byte)pcy][(byte)pcx] != 2 &&
        colliders[(byte)pdy][(byte)pdx] != 2 &&
        piece.a.x != 1 && piece.a.y != 1) {
        piece.b.x = pbx;
        piece.b.y = pby;
        piece.c.x = pcx;
        piece.c.y = pcy;
        piece.d.x = pdx;
        piece.d.y = pdy;
    }
}

void lancerJeu() {
    // Charge les textures et configure les tuiles
    sf::Texture tile_tex;
    tile_tex.loadFromFile("images/tetris_tile.png");
    sf::Sprite tile(tile_tex);
    tile.setScale(2.83, 2.83);
    float tile_size = tile_tex.getSize().x * tile.getScale().x;
    size_t width = tile_size * 12, height = tile_size * 20;

    // Charge la musique et configure la lecture
    sf::Music track;
    track.openFromFile("sounds/tetris-theme.wav");
    track.play();
    track.setLoop(true);

    // Charge les effets sonores
    sf::SoundBuffer buffer;
    buffer.loadFromFile("sounds/fall.wav");
    sf::Sound fall;
    fall.setBuffer(buffer);

    sf::SoundBuffer bufferTwo;
    bufferTwo.loadFromFile("sounds/line.mp3");
    sf::Sound line;
    line.setBuffer(bufferTwo);

    sf::SoundBuffer bufferThree;
    bufferThree.loadFromFile("sounds/gameover_tetris.mp3");
    sf::Sound tetris;
    tetris.setBuffer(bufferThree);

    // Configure l'affichage du score
    sf::Text score;
    sf::Font font;
    font.loadFromFile("Roboto-Bold.ttf");
    score.setFont(font);
    score.setCharacterSize(15);
    score.setFillColor(sf::Color::Blue);
    score.setPosition(10, 25);
    sf::Vector2<float> score_scale(1.5f, 1.5f);
    score.setScale(score_scale);
    score.setString("Lines: 0");

    // Configure la fenêtre et crée la première pièce
    sf::RenderWindow window(sf::VideoMode(width, height), "Tetris", sf::Style::Titlebar | sf::Style::Close);
    window.setKeyRepeatEnabled(true);
    window.setFramerateLimit(60);

    Piece piece = CreatePiece(static_cast<PIECE_TYPE>((rand() % 7)));
    unsigned int timer = 0, gamespeed = 10, scoreCounter = 0;

    while (window.isOpen()) {
        srand(time(NULL));

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                // Gestion des mouvements
                if (event.key.code == sf::Keyboard::W) {
                    rotate(piece);
                }
                else if (event.key.code == sf::Keyboard::A &&
                    piece.a.x != 0 && piece.b.x != 0 && piece.c.x != 0 && piece.d.x != 0 &&
                    (colliders[piece.a.y][piece.a.x - 1]) != 2 && (colliders[piece.b.y][piece.b.x - 1]) != 2 &&
                    (colliders[piece.c.y][piece.c.x - 1]) != 2 && (colliders[piece.d.y][piece.d.x - 1]) != 2) {
                    piece.a.x--;
                    piece.b.x--;
                    piece.c.x--;
                    piece.d.x--;
                }
                else if (event.key.code == sf::Keyboard::D &&
                    piece.a.x != 11 && piece.b.x != 11 && piece.c.x != 11 && piece.d.x != 11 &&
                    (colliders[piece.a.y][piece.a.x + 1]) != 2 && (colliders[piece.b.y][piece.b.x + 1]) != 2 &&
                    (colliders[piece.c.y][piece.c.x + 1]) != 2 && (colliders[piece.d.y][piece.d.x + 1]) != 2) {
                    piece.a.x++;
                    piece.b.x++;
                    piece.c.x++;
                    piece.d.x++;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                gamespeed = 10;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            gamespeed = 1;
        }

        window.clear();

        // Dessine la pièce actuelle
        sf::Sprite piece_tile = tile;
        piece_tile.setPosition(tile_size * piece.a.x, tile_size * piece.a.y);
        window.draw(piece_tile);
        piece_tile.setPosition(tile_size * piece.b.x, tile_size * piece.b.y);
        window.draw(piece_tile);
        piece_tile.setPosition(tile_size * piece.c.x, tile_size * piece.c.y);
        window.draw(piece_tile);
        piece_tile.setPosition(tile_size * piece.d.x, tile_size * piece.d.y);
        window.draw(piece_tile);

        // Rafraîchit le tableau de la grille
        for (size_t i = 0; i < 20; i++) {
            for (size_t j = 0; j < 12; j++) {
                if (colliders[i][j] == 2) {
                    grid[i][j] = 2;
                }
                else {
                    grid[i][j] = 0;
                }
            }
        }

        // Horloge
        if (timer > gamespeed) {
            // Vérifications de collision
            if (grid[piece.a.y + 1][piece.a.x] == 2 ||
                grid[piece.b.y + 1][piece.b.x] == 2 ||
                grid[piece.c.y + 1][piece.c.x] == 2 ||
                grid[piece.d.y + 1][piece.d.x] == 2 ||
                piece.a.y == 19 || piece.b.y == 19 || piece.c.y == 19 || piece.d.y == 19) {
                fall.play();
                grid[piece.a.y][piece.a.x] = 2;
                grid[piece.b.y][piece.b.x] = 2;
                grid[piece.c.y][piece.c.x] = 2;
                grid[piece.d.y][piece.d.x] = 2;
                colliders[piece.a.y][piece.a.x] = 2;
                colliders[piece.b.y][piece.b.x] = 2;
                colliders[piece.c.y][piece.c.x] = 2;
                colliders[piece.d.y][piece.d.x] = 2;
                piece = CreatePiece(static_cast<PIECE_TYPE>((rand() % 7)));
            }
            else {
                grid[piece.a.y + 1][piece.a.x] = 1;
                grid[piece.b.y + 1][piece.b.x] = 1;
                grid[piece.c.y + 1][piece.c.x] = 1;
                grid[piece.d.y + 1][piece.d.x] = 1;
                piece.a.y++;
                piece.b.y++;
                piece.c.y++;
                piece.d.y++;
            }

            // Vérifie si le joueur a une ligne ou un "tetris"
            byte tetris_row = 0;
            for (size_t i = 0; i < 20; i++) {
                byte blocks_in_a_row = 0;
                for (size_t j = 0; j < 12; j++) {
                    if (colliders[i][j] == 2) {
                        blocks_in_a_row++;
                    }
                }
                if (blocks_in_a_row == 12) {
                    tetris_row++;
                    if (tetris_row >= 4) {
                        
                    }
                    else {
                        line.play();
                    }
                    for (size_t k = i; k > 0; k--) {
                        for (size_t l = 0; l < 12; l++) {
                            colliders[k][l] = colliders[k - 1][l];
                        }
                    }
                    scoreCounter++;
                    char temp[256];
                    sprintf_s(temp, "Lines: %i", scoreCounter);
                    score.setString(temp);
                }
            }

            // Si le jeu est terminé, ferme l'application
            for (size_t i = 0; i < 12; i++) {
                if (colliders[0][i] == 2) {
                    track.stop();
                    tetris.play();
                    Sleep(2000);
                    window.close();
                }
            }

            timer = 0;
        }
        else {
            timer++;
        }

        // Dessine toutes les tuiles
        for (size_t i = 0; i < 20; i++) {
            for (size_t j = 0; j < 12; j++) {
                if (grid[i][j] == 1) {
                    sf::Sprite t = tile;
                    t.setPosition(tile_size * j, tile_size * i);
                    window.draw(t);
                }
                if (colliders[i][j] == 2) {
                    sf::Sprite t = tile;
                    t.setPosition(tile_size * j, tile_size * i);
                    window.draw(t);
                }
            }
        }

        // Dessine le score et met à jour la fenêtre
        window.draw(score);
        window.display();
    }
}

void mainMenu() {
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Menu Principal - Tetris SFML", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    if (!mainmenu_texture.loadFromFile("images/tetris_menubg.png"))
    {
        std::cout << "Erreur lors du chargement de tetris_menubg.png" << std::endl;
    }
    mainmenu_texture.setSmooth(true);
    mainmenu_sprite.setTexture(mainmenu_texture);
    mainmenu_sprite.setPosition(0, 0);

    if (!mainmenu_soundbuffer.loadFromFile("sounds/tetris_mainmenu.mp3"))
    {
        std::cout << "Erreur lors du chargement de tetris_mainmenu.mp3" << std::endl;
    }
    mainmenu_sound.setBuffer(mainmenu_soundbuffer);
    mainmenu_sound.setBuffer(mainmenu_soundbuffer);
    mainmenu_sound.setLoop(true);
    mainmenu_sound.play();

    sf::Text title;
    sf::Font font;
    font.loadFromFile("Roboto-Bold.ttf");
    title.setFont(font);
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::Blue);
    title.setPosition(150, 200);
    title.setString("Tetris SFML - ING3-GAP");

    sf::Text instruction;
    instruction.setFont(font);
    instruction.setCharacterSize(20);
    instruction.setFillColor(sf::Color::White);
    instruction.setPosition(150, 300);
    instruction.setString("Appuyez sur W, A, S ou D pour commencer");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                window.close();
                return; // Sort de la fonction et ferme la fenetre
            }
            if (event.type == sf::Event::KeyPressed) {
                mainmenu_sound.stop();
                window.close();
                lancerJeu();
                return; // Sort de la fonction pour lancer le jeu
            }
        }

        window.clear();
        window.draw(mainmenu_sprite);
        window.draw(title);
        window.draw(instruction);
        window.display();
    }
}

int main() {
    mainMenu();
    return 0;
}
