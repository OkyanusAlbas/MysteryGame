#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm> // For std::swap
#include <chrono> // For timing functionality
#include <cctype> // For tolower()

// Define variables for maze dimensions (no const)
int WIDTH = 20;
int HEIGHT = 20;
int timerLimit = 60; // Timer limit for escape in seconds

// 2D array representing the maze
char maze[20][20];

// Player's position
int playerX = 1, playerY = 1;

// Enemy structure
struct Enemy {
    int x, y;
    void moveRandomly() {
        int move = rand() % 4;
        if (move == 0 && maze[y-1][x] != '#') y--; // Move up
        else if (move == 1 && maze[y+1][x] != '#') y++; // Move down
        else if (move == 2 && maze[y][x-1] != '#') x--; // Move left
        else if (move == 3 && maze[y][x+1] != '#') x++; // Move right
    }
};

// Inventory structure
struct Item {
    std::string name;
    int quantity;
};

// Store items collected by the player
std::vector<Item> inventory;

// Directions for recursive backtracking (up, down, left, right)
int directions[4][2] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};

// Function to initialize the maze with walls
void initializeMaze() {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            maze[i][j] = '#'; // Fill the maze with walls initially
        }
    }
}

// Function to display the maze
void displayMaze() {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            std::cout << maze[i][j];
        }
        std::cout << std::endl;
    }
}

// Function to check if a cell is within the maze bounds
bool inBounds(int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}

// Manual shuffling for directions in the maze generation
void carveMaze(int x, int y) {
    maze[y][x] = ' '; // Mark the current cell as a path

    // Randomize the directions manually using simple swaps
    std::vector<int> dirs = {0, 1, 2, 3};  // Directions: 0 = Up, 1 = Down, 2 = Left, 3 = Right

    // Manually shuffle directions using a simple swapping technique
    for (int i = 0; i < 4; ++i) {
        int j = rand() % 4;  // Random index between 0 and 3
        std::swap(dirs[i], dirs[j]);  // Swap elements i and j in the directions array
    }

    // Try all directions in the randomized order
    for (int i = 0; i < 4; ++i) {
        int dir = dirs[i];
        int newX = x + directions[dir][0];
        int newY = y + directions[dir][1];

        if (inBounds(newX, newY) && maze[newY][newX] == '#') {
            // Carve a path to the neighboring cell
            maze[newY][newX] = ' ';
            // Carve the wall between the current cell and the new cell
            maze[y + directions[dir][1] / 2][x + directions[dir][0] / 2] = ' ';
            // Recursively carve the maze from the new cell
            carveMaze(newX, newY);
        }
    }
}

// Function to add an item to the inventory
void addItem(const std::string& itemName, int quantity) {
    bool found = false;
    for (auto& item : inventory) {
        if (item.name == itemName) {
            item.quantity += quantity;
            found = true;
            break;
        }
    }
    if (!found) {
        inventory.push_back({itemName, quantity});
    }
}

// Function to display the inventory
void displayInventory() {
    std::cout << "Inventory:\n";
    for (const auto& item : inventory) {
        std::cout << item.name << " x" << item.quantity << "\n";
    }
}

// Function to ask the player a puzzle question
void puzzle() {
    int answer;
    std::cout << "Solve this puzzle to proceed: 5 + 3 = ";
    std::cin >> answer;
    if (answer == 8) {
        std::cout << "Correct! You may proceed.\n";
    } else {
        std::cout << "Incorrect, try again.\n";
    }
}

// Function to show the main menu
void showMainMenu() {
    std::cout << "\nMain Menu:\n";
    std::cout << "1. Start Game\n";
    std::cout << "2. Instructions\n";
    std::cout << "3. Settings\n";
    std::cout << "4. Quit\n";
}

// Function to show the settings menu
void showSettingsMenu() {
    std::cout << "\nSettings Menu:\n";
    std::cout << "1. Change Difficulty\n";
    std::cout << "2. Change Controls\n";
    std::cout << "3. Back to Main Menu\n";
}

// Function to change the difficulty
void changeDifficulty() {
    int difficulty;
    std::cout << "\nSelect Difficulty:\n";
    std::cout << "1. Easy\n";
    std::cout << "2. Medium\n";
    std::cout << "3. Hard\n";
    std::cin >> difficulty;

    switch (difficulty) {
        case 1:
            WIDTH = 10;
            HEIGHT = 10;
            timerLimit = 120;  // 2 minutes
            std::cout << "Easy difficulty selected. Timer: 2 minutes\n";
            break;
        case 2:
            WIDTH = 15;
            HEIGHT = 15;
            timerLimit = 90;  // 1.5 minutes
            std::cout << "Medium difficulty selected. Timer: 1.5 minutes\n";
            break;
        case 3:
            WIDTH = 20;
            HEIGHT = 20;
            timerLimit = 60;  // 1 minute
            std::cout << "Hard difficulty selected. Timer: 1 minute\n";
            break;
        default:
            std::cout << "Invalid option.\n";
            break;
    }
}

// Main game function
int main() {
    srand(time(0)); // Seed the random number generator
    int choice;
    bool inGame = false;

    // Main menu loop
    while (true) {
        if (!inGame) {
            showMainMenu();
            std::cin >> choice;
        }

        switch (choice) {
            case 1: { // Start Game
                // Declare and initialize enemy inside the game case
                Enemy enemy = {5, 5}; // Initial position of enemy
                initializeMaze();
                carveMaze(1, 1); // Start carving from position (1, 1)

                // Add a resource (example: Key)
                addItem("Key", 1);
                displayInventory();

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

                    // Clear the old positions (before updating the maze)
                    maze[enemy.y][enemy.x] = ' '; // Remove the enemy from the previous position
                    maze[playerY][playerX] = ' '; // Remove the player from the previous position

                    // Move the enemy randomly
                    enemy.moveRandomly();

                    // Place the player and enemy in the maze again
                    maze[enemy.y][enemy.x] = 'E'; // Update enemy position
                    maze[playerY][playerX] = 'P'; // Update player position

                    // Display the maze with player and enemy
                    displayMaze();

                    // Ask player to move or exit
                    char direction;
                    std::cout << "Move (WASD) or press E to Exit: ";
                    std::cin >> direction;

                    // Convert to lowercase for case insensitivity
                    direction = std::tolower(direction);

                    if (direction == 'e') {
                        std::cout << "Exiting to Main Menu...\n";
                        break; // Exit the game and return to main menu
                    }

                    // Move player based on direction input
                    if (direction == 'w' && maze[playerY - 1][playerX] != '#') playerY--;
                    else if (direction == 's' && maze[playerY + 1][playerX] != '#') playerY++;
                    else if (direction == 'a' && maze[playerY][playerX - 1] != '#') playerX--;
                    else if (direction == 'd' && maze[playerY][playerX + 1] != '#') playerX++;

                    // Check for interaction (e.g., puzzle)
                    if (playerX == 2 && playerY == 2) {
                        std::cout << "You've reached a puzzle area!\n";
                        puzzle();
                    }

                    // Display inventory after player move
                    displayInventory();

                    // Escape condition
                    if (playerX == WIDTH - 2 && playerY == HEIGHT - 2) {
                        std::cout << "Congratulations! You've escaped the maze!\n";
                        escaped = true;
                        break;
                    }
                }

                if (!escaped) {
                    std::cout << "Game Over!\n";
                }

                break;
            }

            case 2: // Instructions
                std::cout << "Use WASD (lowercase) to move. Avoid enemies and solve puzzles!\n";
                break;

            case 3: // Settings
                while (true) {
                    showSettingsMenu();
                    std::cin >> choice;
                    if (choice == 3) break;
                    if (choice == 1) changeDifficulty();
                    // Additional settings logic could be added here
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
