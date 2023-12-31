#include <cstdlib>
#include <ncurses.h>
#include <chrono>
#include <thread>

using namespace std;

bool gameOver;
const int width = 30;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirecton dir;
const int GameSpeed = 100;

void Setup() {
    initscr(); // Initialize ncurses
    clear(); // Clear the screen
    noecho(); // Don't echo keypresses
    cbreak(); // Disable line buffering
    curs_set(0); // Hide the cursor

    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw() {
    clear();
    for (int i = 0; i < width + 2; i++)
        mvprintw(0, i, "#");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                mvprintw(i + 1, 0, "#");
            if (i == y && j == x)
                mvprintw(i + 1, j + 1, "O");
            else if (i == fruitY && j == fruitX)
                mvprintw(i + 1, j + 1, "F");
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        mvprintw(i + 1, j + 1, "o");
                        print = true;
                    }
                }
                if (!print) mvprintw(i + 1, j + 1, " ");
            }
            if (j == width - 1)
                mvprintw(i + 1, width + 1, "#");
        }
    }
    for (int i = 0; i < width + 2; i++)
        mvprintw(height + 1, i, "#");
    mvprintw(height + 2, 0, "Score: %d", score);
    mvprintw(height +3, 0, "Speed: %d",GameSpeed - ((score  * GameSpeed) / (score + GameSpeed)));
    refresh();
}

void Input() {
    keypad(stdscr, TRUE); // Enable keyboard mapping
    halfdelay(1); // Set a timeout for waiting for input

    int c = getch();
    switch (c) {
    case KEY_LEFT:
        dir = LEFT;
    case 'a':
        dir = LEFT;
        break;
    case KEY_RIGHT:
        dir = RIGHT;
        break;
    case 'd':
        dir = RIGHT;
        break;
    case KEY_UP:
        dir = UP;
        break;
    case 'w':
        dir = UP;
        break;
    case KEY_DOWN:
        dir = DOWN;
        break;
    case 's':
        dir = DOWN;
        break;
    case 'x':
        gameOver = true;
        break;
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        this_thread::sleep_for(chrono::milliseconds(GameSpeed - ((score  * GameSpeed) / (score + GameSpeed))));
    }
    endwin(); // Restore normal terminal behavior
    return 0;
}
