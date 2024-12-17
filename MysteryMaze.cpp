#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

// Global variables for the maze dimensions and player position
const int WIDTH = 20;
const int HEIGHT = 10;
int playerX = 1, playerY = 1;  // Initial player position
int exitX = WIDTH - 2, exitY = HEIGHT - 2;  // Exit position
int timerLimit = 60;  // Timer limit for the game
char maze[HEIGHT][WIDTH];  // Maze array

// Enemy structure
struct Enemy {
    int x, y;
    void moveRandomly() {
        int direction = rand() % 4;
        if (direction == 0 && y > 1) y--; // Move up
        else if (direction == 1 && y < HEIGHT - 2) y++; // Move down
        else if (direction == 2 && x > 1) x--; // Move left
        else if (direction == 3 && x < WIDTH - 2) x++; // Move right
    }
};

// Function to generate the maze with 2-block wide corridors
void generateMaze(int startX, int startY) {
    // Fill maze with walls ('#')
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            maze[i][j] = '#';
        }
    }

    // Ensure the exit position is always valid and does not get overwritten
    maze[exitY][exitX] = 'X';  // Exit is placed here

    // Create corridors (2-block wide)
    for (int i = 1; i < WIDTH - 1; i += 3) {  // Step by 3 to ensure at least 2-block wide corridors
        for (int j = 1; j < HEIGHT - 1; ++j) {
            maze[j][i] = ' ';
            maze[j][i + 1] = ' ';
        }
    }

    for (int i = 1; i < HEIGHT - 1; i += 3) {  // Step by 3 to ensure at least 2-block wide corridors
        for (int j = 1; j < WIDTH - 1; ++j) {
            maze[i][j] = ' ';
            maze[i + 1][j] = ' ';
        }
    }

    // Ensure that the player's starting position is valid
    maze[startY][startX] = ' ';

    // Optional: Randomly clear out some more paths, ensuring 2-block wide corridors
    for (int i = 3; i < WIDTH - 3; ++i) {
        for (int j = 3; j < HEIGHT - 3; ++j) {
            if (rand() % 4 == 0) {
                maze[j][i] = ' ';
                maze[j + 1][i] = ' ';
            }
        }
    }
}

// Function to display the maze
void displayMaze() {
    #ifdef _WIN32
        system("cls"); // Use "cls" for Windows
    #else
        system("clear"); // Use "clear" for Unix-based systems
    #endif

    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            std::cout << maze[i][j];
        }
        std::cout << std::endl;
    }
}

// Function to move the player
void movePlayer(char direction) {
    maze[playerY][playerX] = ' '; // Clear the old position
    if (direction == 'w' || direction == 'W') {
        if (playerY > 1 && maze[playerY - 1][playerX] != '#') {
            playerY--;
        }
    } else if (direction == 's' || direction == 'S') {
        if (playerY < HEIGHT - 2 && maze[playerY + 1][playerX] != '#') {
            playerY++;
        }
    } else if (direction == 'a' || direction == 'A') {
        if (playerX > 1 && maze[playerY][playerX - 1] != '#') {
            playerX--;
        }
    } else if (direction == 'd' || direction == 'D') {
        if (playerX < WIDTH - 2 && maze[playerY][playerX + 1] != '#') {
            playerX++;
        }
    }
    maze[playerY][playerX] = 'P'; // Place player in new position
}

// Main game loop
int main() {
    srand(time(0)); // Seed the random number generator
    int choice;
    bool inGame = false;
    int difficulty = 1; // Default difficulty: 1 (Easy)

    // Main menu loop
    while (true) {
        if (!inGame) {
            std::cout << "\nMain Menu:\n";
            std::cout << "1. Start Game\n";
            std::cout << "2. Instructions\n";
            std::cout << "3. Settings\n";
            std::cout << "4. Quit\n";

            while (true) {
                std::cout << "Enter your choice (1-4): ";
                std::cin >> choice;

                if (choice >= 1 && choice <= 4) {
                    break; // Valid input, break the loop
                } else {
                    std::cout << "Invalid input. Please enter a number between 1 and 4.\n";
                }
            }
        }

        switch (choice) {
            case 1: { // Start Game
                // Adjust difficulty (if needed)
                if (difficulty == 2) {
                    timerLimit = 45; // Less time on harder levels
                } else if (difficulty == 3) {
                    timerLimit = 30; // Even less time
                }

                Enemy enemy = {5, 5}; // Initial position of enemy
                generateMaze(1, 1); // Generate maze starting at (1, 1)
                maze[playerY][playerX] = 'P'; // Place player in the maze
                maze[enemy.y][enemy.x] = 'E'; // Represent enemy with 'E'

                // Game loop with timer
                auto start = std::chrono::steady_clock::now();
                bool escaped = false;

                while (true) {
                    // Timer check
                    auto now = std::chrono::steady_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

                    if (elapsed > timerLimit) {
                        std::cout << "Time's up! You failed to escape the maze.\n";
                        break;
                    }

                    // Move the enemy randomly
                    maze[enemy.y][enemy.x] = ' '; // Clear the old enemy position
                    enemy.moveRandomly();
                    maze[enemy.y][enemy.x] = 'E'; // Update enemy position

                    // Display the maze with player and enemy
                    displayMaze();

                    // Ask player to move or exit
                    char direction;
                    std::cout << "Move (WASD) or press E to Exit: ";
                    std::cin >> direction;

                    // Handle player movement
                    if (direction == 'e' || direction == 'E') {
                        std::cout << "Exiting to Main Menu...\n";
                        break; // Exit the game and return to main menu
                    }

                    // Move the player based on the input (case insensitive)
                    movePlayer(direction);

                    // Check if player reaches the exit
                    if (playerX == exitX && playerY == exitY) {
                        std::cout << "Congratulations! You've escaped the maze!\n";
                        escaped = true;
                        break;
                    }

                    // Check if player touches the enemy
                    if (playerX == enemy.x && playerY == enemy.y) {
                        std::cout << "You were caught by the enemy! Game Over.\n";
                        break;
                    }
                }

                if (!escaped) {
                    std::cout << "Game Over!\n";
                }

                break;
            }

            case 2: // Instructions
                std::cout << "Use WASD (lowercase or uppercase) to move. Avoid enemies and solve puzzles!\n";
                break;

            case 3: // Settings (difficulty selection)
                std::cout << "Select Difficulty:\n";
                std::cout << "1. Easy\n";
                std::cout << "2. Medium\n";
                std::cout << "3. Hard\n";

                while (true) {
                    std::cin >> difficulty;
                    if (difficulty >= 1 && difficulty <= 3) {
                        break; // Valid input, break the loop
                    } else {
                        std::cout << "Invalid input. Please select a difficulty between 1 and 3.\n";
                    }
                }
                break;

            case 4: // Quit
                std::cout << "Thanks for playing!\n";
                return 0;

            default:
                std::cout << "Invalid option.\n";
                break;
        }
    }
}
