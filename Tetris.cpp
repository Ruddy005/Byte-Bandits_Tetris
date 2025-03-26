#include <iomanip>
#include <iostream>
#include <vector>
#include <random>
#include <conio.h>
#include <fstream>
#include <ctime> // For seeding srand()

void red() { printf("\033[1;33m"); }
void yellow() { printf("\033[1;31m"); }
void green() { printf("\033[38;2;255;165;0m"); }
void cyan() { printf("\033[1;36m"); }
void magenta() { printf("\033[1;35m"); }
void blue() { printf("\033[1;34m"); }  
void orange() { printf("\033[38;2;255;140;0m"); }  
void reset() { printf("\033[0m"); }

int score = 0;
int highscore = 0;
const std::string HIGH_SCORE_FILE = "highscore.txt";

// Loading and saving highscore functions
void loadHighScore() {
    std::ifstream file(HIGH_SCORE_FILE);
    if (file.is_open()) {
        file >> highscore;
        file.close();
    }
}

void saveHighScore() {
    std::ofstream file(HIGH_SCORE_FILE);
    if (file.is_open()) {
        file << highscore;
        file.close();
    }
}

void updateScore(int linesCleared) {
    score += linesCleared * 10;
    if (score > highscore) {
        highscore = score;
        saveHighScore();
    }
}

std::vector<std::vector<int>> stage(22, std::vector<int>(13, 0));
std::vector<std::vector<int>> block =
{
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};

std::vector<std::vector<int>> field(22, std::vector<int>(13, 0));
// coordinate
int y = 0;
int x = 4;
bool gameover = false;
size_t GAMESPEED = 20000;

std::vector<std::vector<std::vector<int>>> block_list =
{
    {
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 }
    },
   
    {
        { 0, 0, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 1, 1, 0 }
    },

    {
        { 0, 0, 0, 0 },
        { 1, 1, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 1, 1, 0 },
        { 1, 1, 0, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 1, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 1, 0 }
    }
};

int menu();
int gameOver();
void title();
void gameLoop();
void display();
bool makeBlocks();
void initGame();
void moveBlock(int, int);
void collidable();
bool isCollide(int, int);
void userInput();
bool rotateBolck();
void spwanBlock();
void checkfullrows();

int main()
{
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator
    
    switch (menu())
    {
    case 1:
        gameLoop();
        break;
    case 2:
        return 0;
    case 0:
        magenta();
        std::cerr << "Choose 1~3" << std::endl;
        return -1;
    }
    return 0;
}

void checkfullrows()
{
    int linescleared = 0;
    for (size_t i = 0; i < 20; i++)
    {
        bool full = true;
        for (size_t j = 1; j < 11; j++)
        {
            if (field[i][j] == 0)
            {
                full = false;
            }
        }
        if (full)
        {
            linescleared++;
            for (size_t j = 1; j < 11; j++)
            {
                field[i][j] = 0;
            }
            for (size_t k = i; k > 0; k--)
            {
                for (size_t j = 1; j < 11; j++)
                {
                    field[k][j] = field[k - 1][j];
                }
            }
        }
    }
    if (linescleared > 0)
    {
        updateScore(linescleared);
    }
}

int gameOver()
{
    using namespace std;

    char a;
    cyan();cout << " #####     #    #     # ####### ####### #     # ####### ######\n" ;
    red();cout << "#     #   # #   ##   ## #       #     # #     # #       #     #\n";
    yellow();cout << "#        #   #  # # # # #       #     # #     # #       #     #\n";
    orange();cout << "#  #### #     # #  #  # #####   #     # #     # #####   ######\n";
    magenta();cout << "#     # ####### #     # #       #     #  #   #  #       #   #\n";
    green();cout << "#     # #     # #     # #       #     #   # #   #       #    #\n";
    blue();cout << " #####  #     # #     # ####### #######    #    ####### #     #\n";
    cyan();
    cout << " \n\t  ~ THANK YOU FOR PLAYING! ~    \n";
    red();
    cout << " \n\t      See you again soon!        \n";
    green();
    cout << " \n\t    Press any key and enter      \n";
    if (cin >> a)
{
    int choice = menu();
    if (choice == 1)
    {
        // Reset game variables before restarting
        score = 0;
        gameover = false;
        initGame();  // Reset the field and blocks
        gameLoop();
    }
    else if (choice == 2)
    {
        return 0;
    }
    else
    {
        magenta();
        std::cerr << "Choose 1~3" << std::endl;
        return -1;
}
}
}

void gameLoop()
{
    size_t time = 0;
    initGame();

    while (!gameover)
    {
        if (kbhit())
        {
            userInput();
        }

        if (time < GAMESPEED)
        {
            time++;
        }
        else
        {
            spwanBlock();
            time = 0;
        }
    }
}

int menu()
{
    title();

    int select_num = 0;

    std::cin >> select_num;

    switch (select_num)
    {
    case 1:
    case 2:
        std::cout << "High Score: " << highscore << std::endl;
        return select_num;
    case 3:
        break;
    default:
        select_num = 0;
        break;
    }

    return select_num;
}

void title()
{
    using namespace std;

    system("cls");
    red();
    cout << "#==============================================================================#\n";
    yellow();
    cout << "####### ####### ####### ######    ###    #####\n";
    cout << "   #    #          #    #     #    #    #     #\n";
    cout << "   #    #          #    #     #    #    #\n";
    cout << "   #    #####      #    ######     #     #####\n";
    cout << "   #    #          #    #   #      #          #\n";
    cout << "   #    #          #    #    #     #    #     #\n";
    cout << "   #    #######    #    #     #   ###    #####\t\n";

    magenta();
    cout << " MADE BY Byte-Bandits\n";
    cout << "\n\n\n\n";
    green();
    cout << "\tMENU:\n";
    cyan();
    cout << "\t1: Start Game\n\t2: view High score\n\t3: Quit\n\n";
    red();
    cout << "#==============================================================================#\n";
    cout << "Choose >> ";
}

void display()
{
    system("cls");
    yellow();
    std::cout << "Score: " << score << "\tHigh Score: " << highscore << std::endl;

    for (size_t i = 0; i < 21; i++)
    {
        for (size_t j = 0; j < 12; j++)
        {
            switch (field[i][j])
            {
                case 0:
                std::cout << " " << std::flush;
                break;
                case 9:  
                green();
                std::cout << "@" << std::flush;
                break;
           
                default:
                cyan();
                std::cout << "#" << std::flush;
                break;
            }
        }
        std::cout << std::endl;
    }

    magenta();
    std::cout << "\n\tLeft Arrow : Move left\n\tRight Arrow  : Move right\n\tUp Arrow : Rotate piece\n\tDown Arrow : Soft drop\n\tSpacebar: Hard drop\n\tEscape (ESC): Pause or Quit\n";

    if (gameover)
    {
        system("cls");
        gameOver();
    }
}

void initGame()
{
    for (size_t i = 0; i <= 20; i++)
    {
        for (size_t j = 0; j <= 11; j++)
        {
            if ((j == 0) || (j == 11) || (i == 20))
            {
                field[i][j] = stage[i][j] = 9;
            }
            else
            {
                field[i][j] = stage[i][j] = 0;
            }
        }
    }

    makeBlocks();
    display();
}

void moveBlock(int x2, int y2)
{

    //Remove block
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            field[y + i][x + j] -= block[i][j];
        }
    }
    //Update coordinates
    x = x2;
    y = y2;

    // assign a block with the updated value
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            field[y + i][x + j] += block[i][j];
        }
    }

    display();
}

void collidable()
{
    for (size_t i = 0; i<21; i++)
    {
        for (size_t j = 0; j<12; j++)
        {
            stage[i][j] = field[i][j];
        }
    }
}

bool isCollide(int x2, int y2)
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (block[i][j] && stage[y2 + i][x2 + j] != 0)
            {
                return true;
            }
        }
    }
    return false;
}

bool makeBlocks()
{
    x = 4;
    y = 0;

    int blockType = rand() % 7; // Use rand() to generate a random block type

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            block[i][j] = 0;
            block[i][j] = block_list[blockType][i][j];
        }
    }

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            field[i][j + 4] = stage[i][j + 4] + block[i][j];

            if (field[i][j + 4] > 1)
            {
                gameover = true;
                return true;
            }
        }
    }
    return false;
}

bool rotateBolck()
{
    std::vector<std::vector<int>> tmp(4, std::vector<int>(4, 0));

    for (size_t i = 0; i < 4; i++)
    { //Save temporarily block
        for (size_t j = 0; j < 4; j++)
        {
            tmp[i][j] = block[i][j];
        }
    }

    for (size_t i = 0; i < 4; i++)
    { //Rotate
        for (size_t j = 0; j < 4; j++)
        {
            block[i][j] = tmp[3 - j][i];
        }
    }

    if (isCollide(x, y))
    { // And stop if it overlaps not be rotated
        for (size_t i = 0; i < 4; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                block[i][j] = tmp[i][j];
            }
        }
        return true;
    }

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            field[y + i][x + j] -= tmp[i][j];
            field[y + i][x + j] += block[i][j];
        }
    }

    display();

    return false;
}

void spwanBlock()
{
    if (!isCollide(x, y + 1))
    {
        moveBlock(x, y + 1);
    }
    else
    {
        collidable();
        checkfullrows();
        makeBlocks();
        display();
    }
}

void userInput()
{
    char key = getch();

    switch (key)
    {
    case 75: // Left arrow key
        if (!isCollide(x - 1, y))
        {
            moveBlock(x - 1, y);
        }
        break;
    case 77: // Right arrow key
        if (!isCollide(x + 1, y))
        {
            moveBlock(x + 1, y);
        }
        break;
    case 80: // Down arrow key
        if (!isCollide(x, y + 1))
        {
            moveBlock(x, y + 1);
        }
        break;
    case 72: // Up arrow key
        rotateBolck();
        break;
    case 32: // Spacebar
        while (!isCollide(x, y + 1))
        {
            moveBlock(x, y + 1);
        }
        break;
    case 27: // Escape key
        gameover = true;
        break;

    }
}