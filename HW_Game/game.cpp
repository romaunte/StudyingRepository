#include <windows.h>
#include <cmath>
#include <cstdio>
#include <conio.h>

const int MAP_WIDTH  = 80;
const int MAP_HEIGHT = 25;

const char PLAYER_CHAR = '@';
const char BLOCK_CHAR  = '#';
const char EMPTY_CHAR  = ' ';

const float GRAVITY        = 0.5f;
const float MAX_FALL_SPEED = 3.0f;

class GameObject {
    protected:
        float x, y;
        float w, h;
        char symbol;

    public:
        GameObject(float x_, float y_, float w_, float h_, char sym)
            : x(x_), y(y_), w(w_), h(h_), symbol(sym) {}

        virtual ~GameObject() {}

        virtual void draw(char buffer[MAP_HEIGHT][MAP_WIDTH]) const {
            int ix = (int)round(x);
            int iy = (int)round(y);
            int iw = (int)round(w);
            int ih = (int)round(h);

            for (int row = iy; row < iy + ih; ++row) {
                if (row < 0 || row >= MAP_HEIGHT) continue;
                for (int col = ix; col < ix + iw; ++col) {
                    if (col < 0 || col >= MAP_WIDTH) continue;
                    buffer[row][col] = symbol;
                }
            }
        }

        float getX() const { return x; }
        float getY() const { return y; }
        float getW() const { return w; }
        float getH() const { return h; }

        void setPosition(float nx, float ny) { x = nx; y = ny; }
};

class Block : public GameObject {
    public:
        Block() : GameObject(0, 0, 1, 1, BLOCK_CHAR) {}
        Block(float x_, float y_, float w_ = 1.0f, float h_ = 1.0f)
            : GameObject(x_, y_, w_, h_, BLOCK_CHAR) {}
    };

class Player : public GameObject {
    float vy;
    bool onGround;
    public:
        Player(float x_, float y_)
            : GameObject(x_, y_, 3, 3, PLAYER_CHAR), vy(0.0f), onGround(false) {}

        void update(const Block blocks[], int numBlocks) {
            if (GetAsyncKeyState('A') & 0x8000) {
                x -= 1.0f;
            }
            if (GetAsyncKeyState('D') & 0x8000) {
                x += 1.0f;
            }
            if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                if (onGround) {
                    vy = -6.0f;
                    onGround = false;
                }
            }

            vy += GRAVITY;
            if (vy > MAX_FALL_SPEED) vy = MAX_FALL_SPEED;
            y += vy;
            onGround = false;

            for (int i = 0; i < numBlocks; ++i) {
                const Block& b = blocks[i];

                bool overlapX = x < b.getX() + b.getW() && x + w > b.getX();
                bool overlapY = y < b.getY() + b.getH() && y + h > b.getY();

                if (overlapX && overlapY) {
                    if (vy > 0) {
                        y = b.getY() - h;
                        vy = 0;
                        onGround = true;
                    }
                    else if (vy < 0) {
                        y = b.getY() + b.getH();
                        vy = 0;
                    }
                }
            }
        }
};

void hideCursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void setCursor(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

int main() {
    static char screen[MAP_HEIGHT][MAP_WIDTH];
    Player mario(10, 5);


    const int NUM_BLOCKS = MAP_WIDTH;
    Block blocks[NUM_BLOCKS];
    for (int i = 0; i < NUM_BLOCKS; ++i) {
        blocks[i] = Block((float)i, MAP_HEIGHT - 2, 1.0f, 2.0f);
    }

    hideCursor();

    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;
        for (int j = 0; j < MAP_HEIGHT; ++j) {
            for (int i = 0; i < MAP_WIDTH; ++i) {
                screen[j][i] = EMPTY_CHAR;
            }
        }

        mario.update(blocks, NUM_BLOCKS);
        for (int i = 0; i < NUM_BLOCKS; ++i)
            blocks[i].draw(screen);
        mario.draw(screen);

        setCursor(0, 0);
        DWORD written;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        for (int j = 0; j < MAP_HEIGHT; ++j) {
            WriteConsoleOutputCharacterA(hConsole, screen[j], MAP_WIDTH, {0, (SHORT)j}, &written);
        }

        Sleep(30);
    }

    return 0;
}