#include<SFML/Graphics.hpp>
#include<iostream>
#include<fstream>

const int SQUARE_SIZE = 120;
const int BOARD_SIZE = 8;
const int WINDOW_SIZE = SQUARE_SIZE * BOARD_SIZE;


enum CELLSTATE { WHITE, BLACK, NONE };
CELLSTATE board[BOARD_SIZE][BOARD_SIZE] = { NONE };

void drawMenu(sf::RenderWindow& window);
bool exitButton(sf::RenderWindow& window, sf::Event& event);
bool newGameButton(sf::RenderWindow& window, sf::Event& event);
bool loadGameButton(sf::RenderWindow& window, sf::Event& event);
char checkWinner();
bool checkGameOver();
void handleClick(int x, int y, CELLSTATE currentPlayer);
void flipPieces(int row, int col, CELLSTATE currentPlayer);
void drawBoard(sf::RenderWindow& window);

int main() {

	using namespace sf;

	RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "REVERSI");

	CELLSTATE currentPlayer = WHITE; 
	

	while (window.isOpen()) { // MENU FOR ASKING OPTION

		Event event;

		while (window.pollEvent(event)) {

			if (event.type == Event::Closed) {  // CHECKS FOR ANY EVENT TO CLOSE WINDOW
				
				window.close();
				return 0;
			}
		}

		window.clear(sf::Color::Magenta);
		drawMenu(window);

		if (exitButton(window, event)) {
			window.close();
			return 0;
		}

		if (newGameButton(window, event)) {
			
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					board[i][j] = NONE;
				}
			}
			board[3][3] = WHITE;
			board[3][4] = BLACK;
			board[4][4] = WHITE;
			board[4][3] = BLACK;
			
			window.close();
		}

		if (loadGameButton(window, event)) {
			

			using namespace std;

			ifstream fin("game.txt", ios::in);

			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					
					
					char ch;
					fin >> ch;
					
					if (ch == 'B')
						board[i][j] = BLACK;
					else if (ch == 'W')
						board[i][j] = WHITE;
					else
						board[i][j] = NONE;

				}
			}
			window.close();

		}

		window.display();


	}
	
	RenderWindow window2(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "REVERSI");

	while (window2.isOpen()) {
		        sf::Event event;
		        while (window2.pollEvent(event)) {
		            if (event.type == sf::Event::Closed) {
		                window2.close();
						return 0;
		            }
		
		            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		                handleClick(event.mouseButton.x, event.mouseButton.y, currentPlayer);
		
		               
		                currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
		            }
					if (event.type == sf::Event::KeyPressed) {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
							sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
							
							std::ofstream fout("game.txt");
							
							for (int i = 0; i < 8; i++) {
								for (int j = 0; j < 8; j++) {

									if (board[i][j] == NONE)
										fout << "-";
									else if (board[i][j] == WHITE)
										fout << "W";
									else if(board[i][j] == BLACK)
										fout << "B";
								}
								fout << "\n";
							}
						}
					}
		        }
		
		        window2.clear();
		        drawBoard(window2);
		        window2.display();
		
				
			


		        if (checkGameOver()) {
		
		            using namespace std;
		
		            if (checkWinner() == 'W')
		                cout << "WHITE IS THE WINNER" << endl;
		            else if (checkWinner() == 'B')
		                cout << "BLACK IS THE WINNER" << endl;
		            else
		                cout << "IT'S A DRAW" << endl;
		
		            window2.close();
					return 0;
		        }

		    }
		
		   



}

void drawMenu(sf::RenderWindow& window) {

	using namespace sf;


	Font font;
	
	if (!font.loadFromFile("font.ttf")) {
		std::cerr << "Error";
	}

	Text heading;


	
	heading.setFont(font);
	heading.setCharacterSize(100);
	heading.setString("REVERSI");
	heading.setPosition(WINDOW_SIZE / 4 + 30  , WINDOW_SIZE / 4 - 220);
	heading.setFillColor(Color::White);
	
	window.draw(heading);

	

	

	

}

bool exitButton(sf::RenderWindow& window , sf::Event& event) {

	using namespace sf;
	
	// Create button
	RectangleShape button(Vector2f(100, 50));

	button.setFillColor(Color::Transparent);
	button.setPosition(WINDOW_SIZE / 4 + 190, WINDOW_SIZE / 2 + 100);
	window.draw(button);

	// Create text

	Font font;
	font.loadFromFile("font.ttf");
	Text buttonText;
	buttonText.setFont(font);                         
	buttonText.setString("EXIT");                 
	buttonText.setCharacterSize(30);                  
	buttonText.setFillColor(sf::Color::White);        

	// Center the text in the rectangle

	FloatRect textBounds = buttonText.getLocalBounds();
	FloatRect buttonBounds = button.getGlobalBounds();

	buttonText.setPosition(
		buttonBounds.left + (buttonBounds.width - textBounds.width) / 2 - textBounds.left,
		buttonBounds.top + (buttonBounds.height - textBounds.height) / 2 - textBounds.top
	);

	window.draw(buttonText);


	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			// Get mouse position
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);

			// Check if the mouse is within the button's bounds
			if (button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {

				return true;
				
				
				
			}
			return false;
		}
		return false;
	}
	return false;
}

bool newGameButton(sf::RenderWindow& window, sf::Event& event) {

	using namespace sf;

	// Create button
	RectangleShape button(Vector2f(100, 50));

	button.setFillColor(Color::Transparent);
	button.setPosition(WINDOW_SIZE / 4 + 190, WINDOW_SIZE / 2 - 100);
	window.draw(button);

	// Create text

	Font font;
	font.loadFromFile("font.ttf");
	Text buttonText;
	buttonText.setFont(font);
	buttonText.setString("NEW");
	buttonText.setCharacterSize(30);
	buttonText.setFillColor(sf::Color::White);

	// Center the text in the rectangle

	FloatRect textBounds = buttonText.getLocalBounds();
	FloatRect buttonBounds = button.getGlobalBounds();

	buttonText.setPosition(
		buttonBounds.left + (buttonBounds.width - textBounds.width) / 2 - textBounds.left,
		buttonBounds.top + (buttonBounds.height - textBounds.height) / 2 - textBounds.top
	);

	window.draw(buttonText);


	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			// Get mouse position
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);

			// Check if the mouse is within the button's bounds
			if (button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {

				
				return true;


			}
			return false;
		}
		return false;
	}
	return false;
}

bool loadGameButton(sf::RenderWindow& window, sf::Event& event) {

	using namespace sf;

	// Create button
	RectangleShape button(Vector2f(100, 50));

	button.setFillColor(Color::Transparent);
	button.setPosition(WINDOW_SIZE / 4 + 190, WINDOW_SIZE / 2);
	window.draw(button);

	// Create text

	Font font;
	font.loadFromFile("font.ttf");
	Text buttonText;
	buttonText.setFont(font);
	buttonText.setString("LOAD");
	buttonText.setCharacterSize(30);
	buttonText.setFillColor(sf::Color::White);

	// Center the text in the rectangle

	FloatRect textBounds = buttonText.getLocalBounds();
	FloatRect buttonBounds = button.getGlobalBounds();

	buttonText.setPosition(
		buttonBounds.left + (buttonBounds.width - textBounds.width) / 2 - textBounds.left,
		buttonBounds.top + (buttonBounds.height - textBounds.height) / 2 - textBounds.top
	);

	window.draw(buttonText);


	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			// Get mouse position
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);

			// Check if the mouse is within the button's bounds
			if (button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {

				
				return true;


			}
			return false;
		}
		return false;
	}
	return false;
}

char checkWinner() {

    int blackCount = 0;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == BLACK)
                blackCount++;
        }
    }

    if (blackCount > 32)
        return 'B';
    else if (blackCount < 32)
        return 'W';
    else
        return 'D';
}

bool checkGameOver() {

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == NONE) {
                return false; // Still empty spaces available
            }
        }
    }

    return true;
}

void handleClick(int x, int y, CELLSTATE currentPlayer) {
    int row = y / SQUARE_SIZE;
    int col = x / SQUARE_SIZE;

    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == NONE) {
        // Place the player's piece
        board[row][col] = currentPlayer;

        flipPieces(row, col, currentPlayer);

    }
}

void flipPieces(int row ,int col , CELLSTATE currentPlayer) { // this is the row and col where piece is placed

    int directions[8][2] = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}, // horizontal & vertical
            {-1, -1}, {1, 1}, {-1, 1}, {1, -1} // diagonals
    };

    for (int* direction : directions) {
        int dirX = direction[0];
        int dirY = direction[1];
        int x = col + dirX, y = row + dirY;

        while (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[y][x] != NONE) {
            if (board[y][x] == currentPlayer) {
                // Flip pieces in this direction
                int tempX = col + dirX, tempY = row + dirY;
                while (tempX != x || tempY != y) {
                    board[tempY][tempX] = currentPlayer;
                    tempX += dirX;
                    tempY += dirY;
                }
                break;
            }
            x += dirX;
            y += dirY;
        }
    }
}

void drawBoard(sf::RenderWindow& window) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            sf::RectangleShape square(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            square.setPosition(j * SQUARE_SIZE, i * SQUARE_SIZE);
            square.setFillColor((i + j) % 2 == 0 ? sf::Color(139, 69, 19) : sf::Color(222, 184, 135));
            window.draw(square);

            // Draw pieces
            if (board[i][j] == WHITE) {
                sf::CircleShape piece(SQUARE_SIZE / 2.5);
                piece.setFillColor(sf::Color::White);
                piece.setPosition(j * SQUARE_SIZE + SQUARE_SIZE / 11 + 2, i * SQUARE_SIZE + SQUARE_SIZE / 11 + 1);
                window.draw(piece);
            }
            else if (board[i][j] == BLACK) {
                sf::CircleShape piece(SQUARE_SIZE / 2.5);
                piece.setFillColor(sf::Color::Black);
                piece.setPosition(j * SQUARE_SIZE + SQUARE_SIZE / 11 + 2, i * SQUARE_SIZE + SQUARE_SIZE / 11 + 1);
                window.draw(piece);
            }
        }
    }
}
