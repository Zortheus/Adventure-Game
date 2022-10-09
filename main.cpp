// ADVENTURE GAME
// by AJP
// ASCII art by Joan G. Stark [asciiart.eu]
// v 0.0.1

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <ncurses.h>

using namespace std;
#define RESET   "\033[0m"
#define BOLDRED     "\033[1m\033[31m"
#define BOLDGREEN   "\033[1m\033[32m"

// Creating some universal constants and a boolean for whether the game is over
bool gameOver;
const int width = 40;
const int height = 20;
int x, y, coinX, coinY, fruitX, fruitY, score; // creating our variables for positioning
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;
int encounterTimer;
int encounterChance = 100;
int hp;
int encounterNum;


// MONSTER LIST
vector<string> monsterList{"Rat", "Goblin", "Demon"};
int monsterListSize = monsterList.size();
int monsterIndex = rand() % monsterListSize;
string monsterName = monsterList[monsterIndex];
//

void Setup()
{
    initscr();   // these functions set the ncurses library settings

    //color settings
    if(has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_GREEN, COLOR_GREEN);
        init_pair(5, COLOR_BLACK, COLOR_BLACK);

    clear();
    noecho();
    cbreak();
    curs_set(0);

    srand(time(NULL));
    
    gameOver = false;  // the initial game conditions
    dir = STOP;
    x = width / 2;
    y = height / 2;
    coinX = (rand() % width)+1;
    coinY = (rand() % height)+1;
    fruitX = (rand() % width)+1;
    fruitY = (rand() % height)+1;
        do {
            fruitX = (rand() % width)+1;
            fruitY = (rand() % height)+1;
        } while (fruitX == coinX && fruitY == coinY);

    hp = 10;
    encounterTimer = (rand() % encounterChance)+1;
    bool encountersOn = true;

}

void Draw()
{
    clear(); // clear the terminal as the game begins

    for (int i = 0; i < width + 2; i++) // 
        mvprintw(0,i,"#");

    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            if (i == 0 || i == 21) // printing the ceiling
                mvprintw(i,j,"#");
            else if (j == 0 || j == 41) // printing the walls
                mvprintw(i,j,"#");
            else if (i == y && j == x) // printing the head
            { 
                attron(COLOR_PAIR(4));
                mvprintw(i,j,"&");
                attroff(COLOR_PAIR(4));
            }
            else if (i == coinY && j == coinX) // printing the coin
            {    
                attron(COLOR_PAIR(3));
                mvprintw(i,j,"$");
                attroff(COLOR_PAIR(3));
            }    
            else if (i == fruitY && j == fruitX) // printing the fruit
            {    
                attron(COLOR_PAIR(2));
                mvprintw(i,j,"@");
                attroff(COLOR_PAIR(3));
            }    
        }
    }

    //Draw Sidebar Menu
    mvprintw(7, 49, "MOVE: DIRECTION KEYS");
    mvprintw(9, 52, "   ^ ");
    mvprintw(10, 52, "<  v  >");

    attron(COLOR_PAIR(2));
    mvprintw(16, 49, "HEALTH: %d", hp);
    attron(COLOR_PAIR(3));
    mvprintw(18, 49, "GOLD : %d", score);
    attroff(COLOR_PAIR(3));
    mvprintw(20, 49, "TIMER : %d", encounterTimer);

    encounterNum = (rand() % 2);

    if (encounterTimer == 0)
    {
        clear();
        sleep(1);

        string encounterMessage;
        mvprintw(2, 2, "BATTLE TIME!");

        if (encounterNum == 0) {
            mvprintw(3, 2, "Attacked by feral RAT! Lost 3 hp.");
            hp = hp - 3;
        }

        refresh();
        sleep(3);
        encounterTimer = (rand() % encounterChance)+1;

        Draw();
    }

    refresh();
}

void Input()
{
  
    keypad(stdscr, TRUE);
    halfdelay(1);

    int c = getch();

    switch(c)
    {
        case KEY_LEFT:
            if (x > 1)
                dir = LEFT;
            break;
        case KEY_RIGHT:
            if (x < width)
                dir = RIGHT;
            break;
        case KEY_UP:
            if (y > 1)
                dir = UP;
            break;
        case KEY_DOWN:
            if (y < height)
                dir = DOWN;
            break;
        case 113:
            gameOver = true;
            break;
    }

}

void Logic()
{

    switch(dir) // logic for controls; note that up is negative and down is positive since it reads from top left to bottom right.
    {
        case LEFT:
            x--;
            encounterTimer--;
            dir = STOP;
            break;
        case RIGHT:
            x++;
            encounterTimer--;
            dir = STOP;
            break;
        case UP:
            y--;
            encounterTimer--;
            dir = STOP;
            break;
        case DOWN:
            y++;
            encounterTimer--;
            dir = STOP;
            break;
        default:
            break;
    }

    if (encounterTimer < 0)
        encounterTimer = 0;

    if (x > width || x < 1 || y > height || y < 1) // if x reaches boundaries, game over
        gameOver = true;

    if (x == coinX && y == coinY) // When you collect, add one to score, assign a new rand pos to fruit
    {
        score++;
        coinX = (rand() % width) +1;
        coinY = (rand() % height) +1;
        do {
            coinX = (rand() % width)+1;
            coinY = (rand() % height)+1;
        } while (fruitX == coinX && fruitY == coinY);
    }
    
    if (x == fruitX && y == fruitY) // When you collect, add one to score, assign a new rand pos to fruit
    {
        hp++;
        fruitX = (rand() % width) +1;
        fruitY = (rand() % height) +1;
        do {
            fruitX = (rand() % width)+1;
            fruitY = (rand() % height)+1;
        } while (fruitX == coinX && fruitY == coinY);
    }

    if (hp < 1)
        gameOver = true;
}

void registration() // GAME OVER SCREEN
{
        system("clear");

        string ruserName; // define a userName, output a game over prompt and ask for name for high score screen
        cout << "Your score was " << score << endl;
        cout << "\t Enter the username : ";
        cin >> ruserName;

        ofstream f1("highscore.txt", ios::app); //append the file directory - ios::app
        f1 << ruserName << ' ' << score << endl;

        system("clear");
        clear();
}

void ScoreScreen(); // calling this now so it can be used in main()

int main()
{
    
    int d;
    cout << BOLDGREEN << R"(
      _,.
    ,` -.)
   ( _/-\\-._
  /,|`--._,-^|            ,
  \_| |`-._/||          ,'|
    |  `-, / |         /  /     
    |     || |        /  /          
     `r-._||/   __   /  /       
 __,-<_     )`-/  `./  /
'  \   `---'   \   /  /
    |           |./  /
    /           //  /    ADVENTURE GAME!
\_/' \         |/  /
 |    |   _,^-'/  /       ENTER 1 to PLAY
 |    , ``  (\/  /        ENTER 2 for SCORES
  \,.->._    \X-X/)       ENTER 3 to EXIT
  (  /   `-._//^`
   `Y-.____(__}
    |     {__)
          ()
)" << RESET << endl;

    cout << "\n\t   Please enter your choice: ";
    cin >> d;
    cout << endl;

    switch (d)
    {
        case 1:
            Setup();
            break;
        case 3:
            break;
        case 2:
            ScoreScreen();
            break;
        default:
            system("clear"); // clearing a screen on mac is not cls it's "clear"

    }
    
    while(!gameOver)  // the main game loop; draw, take inputs, determine logic, repeat...
    {
        Draw();
        Input();
        Logic();
        //Battle();
    }

    getch();
    sleep(1);
    endwin();

    // check to see what Menu selection was and diplay appropriate screen or exit game.
    if (d == 3 || d == 2)
    {
        clear();
    } else if (d == 1)
    {
        system("clear");
        cout << R"(
  ________    _____      _____  ___________
 /  _____/   /  _  \    /     \ \_   _____/
/   \  ___  /  /_\  \  /  \ /  \ |    __)_
\    \_\  \/    |    \/    Y    \|        \
 \______  /\____|__  /\____|__  /_______  /
        \/         \/         \/        \/
____________   _________________________
\_____  \   \ /   /\_   _____/\______   \
 /   |   \   Y   /  |    __)_  |       _/
/    |    \     /   |        \ |    |   \
\_______  /\___/   /_______  / |____|_  /
        \/                 \/         \/

        )";
        registration();
        main();
        clear();
    }

    system("clear");

    return 0;
}

void ScoreScreen()
{
    system("clear");

    string line; // this variable is our line
    ifstream f("highscore.txt");

    if (f.is_open()) { // read the file
        vector<pair<int, string> > score_vector; //create a vector pair called score_vector - int for score and string for name
        string name; // variable for name
        int score; // variable for score

        while (f >> name >> score) { // while loop using our file f - >> "name" >> "score" means our vector will read the first column's data as name and second as score
            score_vector.push_back(make_pair(score, name)); // this fills score_vector with 
            cout << line << '\n';
        }

        f.close(); // closes the file 

        system("clear");
        clear();

        cout << R"(
  __________________  ________ _____________________ _________
 /   _____/\_   ___ \ \_____  \\______   \_   _____//   _____/
 \_____  \ /    \  \/  /   |   \|       _/|    __|_ \_____  \
 /        \\     \____/    |    \    |   \|        \/        \
/_______  / \______  /\_______  /____|_  /_______  /_______  /
        \/         \/         \/       \/        \/        \/
        )";            
        cout << endl;                                                       

        cout << "#" << "    " << "Name" << "\t\t" << "Score" << endl;
        cout << "-----------------------------" << endl;

        sort(score_vector.begin(), score_vector.end()); // sort the vector from beginning to end (descending)
        reverse(score_vector.begin(), score_vector.end()); // reverse the order of the sort (ascending)

        int place = 1;
        int topTen = 0;
        for(auto it = score_vector.begin(); topTen < 10; ++it){ // starting at the first score, keep going as long as you're not at the end, +1 to it
            cout << place << "    " << it->second << "\t\t" << it->first << endl; // Print 
            place++;
            topTen++;
        }
    }
    else
        cout << "Unable to open text";
  
    int e = 0;
    cout << "\n ENTER 1 to go back to the menu.\n";
    cout << "\n ENTER 2 to exit game." << endl;
    cout << " ";
    cin >> e;

    switch (e)
    {
        case 1:
            system("clear");
            clear();
            main();
            break;
        case 2:
            system("clear");
            break;
        default:
            system("clear");
    }
}
