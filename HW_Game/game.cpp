#include <windows.h>
#include <cmath>
#include <cstdio>
#include <conio.h>

const int MAP_WIDTH  = 80;
const int MAP_HEIGHT = 25;

const char PLAYER_CHAR = '@';
const char BLOCK_CHAR  = '#';
const char ENEMY_CHAR = 'o';
const char EMPTY_CHAR  = ' ';

const float GRAVITY = 0.05f;
const float PLAYER_SPEED = 0.5f;
const float JUMP_SPEED = -1.2f;
const float ENEMY_SPEED = 0.2f;

const int NUM_BLOCKS = 40;
const int NUM_ENEMIES = 5;

class GameObject {
    public:
        float x, y;
        float w, h;
        float vx, vy;
        char symbol;
        bool active;

        GameObject(float x_=0, float y_=0, float w_=1, float h_=1, char s=EMPTY_CHAR)
            : x(x_), y(y_), w(w_), h(h_), vx(0), vy(0), symbol(s), active(true) {}

        bool intersects(const GameObject& other) const {
            return !(x + w <= other.x || x >= other.x + other.w ||
                    y + h <= other.y || y >= other.y + other.h);
        }
};

class Player : public GameObject {
public:
    bool onGround;

    Player() : GameObject(5, 5, 2, 2, PLAYER_CHAR), onGround(false) {}

    void handleInput() {
        if (GetKeyState('A') & 0x8000) x -= PLAYER_SPEED;
        if (GetKeyState('D') & 0x8000) x += PLAYER_SPEED;
        if (onGround && (GetKeyState(VK_SPACE) & 0x8000)) {
            vy = JUMP_SPEED;
            onGround = false;
        }
    }
};

class Block : public GameObject {
public:
    Block() : GameObject(0, 0, 1, 1, BLOCK_CHAR) {}
    Block(float x_, float y_, float w_=1, float h_=1)
        : GameObject(x_, y_, w_, h_, BLOCK_CHAR) {}
};

class Enemy : public GameObject {
public:
    Enemy() : GameObject(0, 0, 2, 2, ENEMY_CHAR) {}

    void init(float x_, float y_) {
        x = x_;
        y = y_;
        w = 2;
        h = 2;
        vx = -ENEMY_SPEED;
        vy = 0;
        active = true;
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

class Game {
    public:
        char map[MAP_HEIGHT][MAP_WIDTH + 1];
        Player player;
        Block blocks[NUM_BLOCKS];
        Enemy enemies[NUM_ENEMIES];

        Game() {
            hideCursor();
            initLevel();
        }

        void initLevel() {
            for (int j = 0; j < MAP_HEIGHT; ++j) {
                for (int i = 0; i < MAP_WIDTH; ++i) map[j][i] = EMPTY_CHAR;
                map[j][MAP_WIDTH] = '\0';
            }

            for (int i = 0; i < NUM_BLOCKS; ++i) {
                blocks[i] = Block((float)i * 2, MAP_HEIGHT - 2, 2, 2);
            }

            for (int i = 0; i < NUM_ENEMIES; ++i) {
                enemies[i].init(20.0f + i * 15, MAP_HEIGHT - 4);
            }

            player = Player();
        }

        void updatePlayerPhysics() {
            player.vy += GRAVITY;
            player.y += player.vy;
            player.onGround = false;

            for (int i = 0; i < NUM_BLOCKS; ++i) {
                if (player.intersects(blocks[i])) {
                    if (player.vy > 0) {
                        player.y = blocks[i].y - player.h;
                        player.vy = 0;
                        player.onGround = true;
                    } else if (player.vy < 0) {
                        player.y = blocks[i].y + blocks[i].h;
                        player.vy = 0;
                    }
                }
            }
        }

        void updateEnemies() {
            for (int i = 0; i < NUM_ENEMIES; ++i) {
                Enemy& e = enemies[i];
                if (!e.active) continue;

                e.vy += GRAVITY;
                e.y += e.vy;

                for (int b = 0; b < NUM_BLOCKS; ++b) {
                    if (e.intersects(blocks[b])) {
                        if (e.vy > 0) {
                            e.y = blocks[b].y - e.h;
                            e.vy = 0;
                        }
                    }
                }

                e.x += e.vx;

                for (int b = 0; b < NUM_BLOCKS; ++b) {
                    if (e.intersects(blocks[b])) {
                        e.x -= e.vx;
                        e.vx = -e.vx;
                        break;
                    }
                }
            }
        }

        void checkPlayerEnemyCollisions() {
            for (int i = 0; i < NUM_ENEMIES; ++i) {
                Enemy& e = enemies[i];
                if (!e.active) continue;
                if (player.intersects(e)) {
                    if (player.vy > 0 && player.y + player.h <= e.y + e.h * 0.5f) {
                        e.active = false;
                    } else {
                        initLevel();
                        return;
                    }
                }
            }
        }

        void render() {
            for (int j = 0; j < MAP_HEIGHT; ++j)
                for (int i = 0; i < MAP_WIDTH; ++i)
                    map[j][i] = EMPTY_CHAR;

            for (int i = 0; i < NUM_BLOCKS; ++i) {
                drawObject(blocks[i]);
            }

            for (int i = 0; i < NUM_ENEMIES; ++i) {
                if (enemies[i].active) drawObject(enemies[i]);
            }

            drawObject(player);

            setCursor(0, 0);
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD written;
            for (int j = 0; j < MAP_HEIGHT; ++j) {
                WriteConsoleOutputCharacterA(h, map[j], MAP_WIDTH, {0, (SHORT)j}, &written);
            }
        }

        void drawObject(const GameObject& obj) {
            for (int j = (int)obj.y; j < (int)(obj.y + obj.h); ++j) {
                for (int i = (int)obj.x; i < (int)(obj.x + obj.w); ++i) {
                    if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT) {
                        map[j][i] = obj.symbol;
                    }
                }
            }
        }

        void run() {
            while (GetKeyState(VK_ESCAPE) >= 0) {
                player.handleInput();
                updatePlayerPhysics();
                updateEnemies();
                checkPlayerEnemyCollisions();
                render();
                Sleep(30);
            }
        }
};

int main() {
    Game game;
    game.run();
    return 0;
}