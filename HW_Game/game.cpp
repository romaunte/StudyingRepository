#include <windows.h>
#include <cmath>
#include <cstdio>
#include <conio.h>

const int MAP_WIDTH  = 80;
const int MAP_HEIGHT = 25;

const char PLAYER_CHAR = '@';
const char BLOCK_CHAR  = '#';
const char ENEMY_CHAR = 'o';
const char COIN_CHAR = '$';
const char BOX_CHAR  = '?';
const char EMPTY_CHAR = ' ';

const float GRAVITY = 0.05f;
const float PLAYER_SPEED = 0.5f;
const float JUMP_SPEED = -1.0f;
const float ENEMY_SPEED = 0.2f;
const float COIN_UP_SPEED = -0.7f;

const int NUM_BLOCKS = 40;
const int NUM_ENEMIES = 5;
const int NUM_COINS = 10;
const int NUM_BOXES   = 5;

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
        void init(float x_, float y_) { x=x_; y=y_; w=2; h=2; vx=-ENEMY_SPEED; vy=0; active=true; }
};

class Coin : public GameObject {
    public:
        Coin() : GameObject(0, 0, 1, 1, COIN_CHAR) {}
        void init(float x_, float y_) { x=x_; y=y_; w=1; h=1; vy=0, active=true; }
};

class Box : public GameObject {
    public:
        bool used;
        Box() : GameObject( 0, 0, 2, 2, BOX_CHAR), used(false) {}
        void init(float x_, float y_) { x=x_; y=y_; w=2; h=2; symbol=BOX_CHAR; used=false; active=true; }
};

void hideCursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void setCursor(int x, int y) {
    COORD c={(SHORT)x,(SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

class Game {
    public:
        char map[MAP_HEIGHT][MAP_WIDTH + 1];
        Player player;
        Block blocks[NUM_BLOCKS];
        Box boxes[NUM_BOXES];
        Enemy enemies[NUM_ENEMIES];
        Coin coins[NUM_COINS * 2];
        int score;

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

            for (int i = 0; i < NUM_BOXES; ++i) {
                boxes[i].init(10 + i * 12, MAP_HEIGHT - 6);
            }

            for (int i = 0; i < NUM_ENEMIES; ++i) {
                enemies[i].init(20.0f + i * 15, MAP_HEIGHT - 4);
            }

            player = Player();

            for(int i = 0;i < NUM_COINS; ++i) {
                coins[i].init(5.0f + i * 8, MAP_HEIGHT - 5);
            }

            for (int i = NUM_COINS; i < NUM_COINS * 2; ++i) {
                coins[i].active=false;
            }
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

            for (int i = 0; i < NUM_BOXES; ++i) {
                Box &b = boxes[i];
                if (!b.active) continue;

                if (player.vy < 0 && player.intersects(b) && !b.used) {
                    b.used = true;
                    b.symbol = BLOCK_CHAR;
                    spawnCoinFromBox(b.x, b.y - 1);
                    player.vy = 0;
                    player.y = b.y + b.h;
                }

                if (player.vy > 0 && player.intersects(b)) {
                    player.y = b.y - player.h;
                    player.vy = 0;
                    player.onGround = true;
                }
            }
        }

        void spawnCoinFromBox(float x, float y){
            for(int i = NUM_COINS; i < NUM_COINS*2; ++i){
                if(!coins[i].active) {
                    coins[i].init(x,y);
                    coins[i].vy = COIN_UP_SPEED;
                    break;
                }
            }
        }

        void updateCoins() {
            for (int i = 0; i < NUM_COINS * 2; ++i){
                Coin &c = coins[i];
                if (!c.active) continue;
                c.vy += GRAVITY;
                c.y += c.vy;
                if (c.y+ c.h >= MAP_HEIGHT) {
                    c.active=false;
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

                for (int b = 0; b < NUM_BLOCKS; ++b) { 
                    if (e.intersects(blocks[b]) && e.vy>0) {
                        e.y=blocks[b].y-e.h; 
                        e.vy=0; 
                    }
                }

                for (int b = 0;b < NUM_BOXES; ++b) {
                    if (e.intersects(boxes[b]) && e.vy>0) {
                        e.y=boxes[b].y-e.h; 
                        e.vy=0; 
                    } 
                }

                e.x += e.vx;
                for (int b = 0; b < NUM_BLOCKS; ++b) {
                    if (e.intersects(blocks[b])) {
                        e.x-=e.vx; 
                        e.vx=-e.vx; 
                        break; 
                    }
                }

                for (int b = 0; b < NUM_BOXES; ++b) {
                    if (e.intersects(boxes[b])) {
                        e.x-=e.vx; 
                        e.vx=-e.vx; 
                        break; 
                    }
                }
            }
        }

        void checkCollisions() {
            for(int i = 0; i < NUM_ENEMIES; ++i){
                Enemy &e = enemies[i];
                if (!e.active) continue;
                if (player.intersects(e)) {
                    if (player.vy > 0 && player.y + player.h <= e.y + e.h*0.5f) {
                        e.active=false;
                    } else { 
                        initLevel();
                        return; 
                    }
                }
            }

            for(int i = 0; i < NUM_COINS; ++i){
                Coin &c = coins[i];
                if (!c.active) continue;
                if (player.intersects(c)) {
                    c.active=false;
                    score+=100;
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

            for (int i = 0; i < NUM_BOXES; ++i) {
                if (boxes[i].active) drawObject(boxes[i]);
            }

            for (int i = 0; i < NUM_ENEMIES; ++i) {
                if (enemies[i].active) drawObject(enemies[i]);
            }

            for (int i = 0; i < NUM_COINS; ++i) {
                if (coins[i].active) drawObject(coins[i]);
            }

            drawObject(player);

            char buf[20];
            sprintf(buf,"Score: %d",score);
            for (int i = 0; i < strlen(buf); ++i) {
                map[0][i] = buf[i];
            }

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
                updateCoins();
                checkCollisions();
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