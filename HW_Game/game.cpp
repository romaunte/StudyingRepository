#include <windows.h>
#include <cmath>
#include <cstdio>
#include <conio.h>

const int MAP_WIDTH  = 240;
const int MAP_HEIGHT = 50;

const char PLAYER_CHAR = '@';
const char BLOCK_CHAR  = '#';
const char ENEMY_CHAR = 'o';
const char COIN_CHAR = '$';
const char BOX_CHAR  = '?';
const char EMPTY_CHAR = ' ';
const char WIN_CHAR    = 'W';

const float GRAVITY = 0.05f;
const float PLAYER_SPEED = 0.5f;
const float JUMP_SPEED = -1.0f;
const float ENEMY_SPEED = 0.2f;
const float COIN_UP_SPEED = -0.7f;

const int NUM_BLOCKS = 60;
const int NUM_ENEMIES = 5;
const int NUM_COINS = 10;
const int NUM_BOXES = 5;

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

        void handleInput(float &dx) {
            dx = 0.0f;
            if (GetKeyState('A') & 0x8000) dx = -PLAYER_SPEED;
            if (GetKeyState('D') & 0x8000) dx = PLAYER_SPEED;
            if (onGround && (GetKeyState(VK_SPACE) & 0x8000)) {
                vy = JUMP_SPEED;
                onGround = false;
            }
        }
};

class Block : public GameObject {
    public:
        Block() : GameObject(0, 0, 1, 1, BLOCK_CHAR) {}
        void init(float x_, float y_, float w_=1, float h_=1) {
            x = x_;
            y = y_;
            w = w_;
            h = h_;
            symbol = BLOCK_CHAR;
            active = true;
        }
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

class Coin : public GameObject {
public:
    Coin() : GameObject(0, 0, 1, 1, COIN_CHAR) {}
    void init(float x_, float y_) {
        x = x_;
        y = y_;
        w = 1;
        h = 1;
        vy = 0;
        active = true;
    }
};

class WinZone : public GameObject {
    public:
        WinZone() : GameObject(0, 0, 2, 2, WIN_CHAR) {}
        void init(float x_, float y_) {
            x = x_;
            y = y_;
            w = 2;
            h = 2;
            symbol = WIN_CHAR;
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
    COORD c={(SHORT)x,(SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

class Game {
    public:
        char map[MAP_HEIGHT][MAP_WIDTH + 1];
        Player player;
        Block blocks[NUM_BLOCKS];
        Enemy enemies[NUM_ENEMIES];
        Coin coins[NUM_COINS * 2];
        WinZone winZone;
        int score;
        int currentLevel;
        float levelLength;

        Game() {
            hideCursor();
            loadLevel(1);
        }

        void loadLevel(int lvl) {
            currentLevel = lvl;
            score = 0;

            for (int i = 0; i < NUM_BLOCKS; i++) blocks[i].active = false;
            for (int i = 0; i < NUM_ENEMIES; i++) enemies[i].active = false;
            for (int i = 0; i < NUM_COINS * 2; i++) coins[i].active = false;
            winZone.active = false;

            player.x = 5;
            player.y = MAP_HEIGHT - 6;
            player.vx = 0;
            player.vy = 0;
            player.onGround = false;

            switch (lvl) {
                case 1: {
                    levelLength = 220.0f;

                    blocks[0].init(0, MAP_HEIGHT - 2, 20, 2);
                    blocks[1].init(30, MAP_HEIGHT - 2, 20, 2);
                    blocks[2].init(60, MAP_HEIGHT - 4, 20, 2);
                    blocks[3].init(90, MAP_HEIGHT - 2, 20, 2);
                    blocks[4].init(130, MAP_HEIGHT - 6, 20, 2);
                    blocks[5].init(170, MAP_HEIGHT - 2, 20, 2);

                    coins[0].init(35, MAP_HEIGHT - 5);
                    coins[1].init(65, MAP_HEIGHT - 7);
                    coins[2].init(95, MAP_HEIGHT - 5);
                    coins[3].init(135, MAP_HEIGHT - 9);
                    coins[4].init(175, MAP_HEIGHT - 5);

                    enemies[0].init(5, blocks[0].y - enemies[0].h);
                    enemies[1].init(65, blocks[2].y - enemies[1].h);
                    enemies[2].init(140, blocks[4].y - enemies[2].h);

                    winZone.init(195, MAP_HEIGHT - 4);
                    break;
                }

                case 2: {
                    levelLength = 240.0f;

                    blocks[0].init(0, MAP_HEIGHT - 2, 25, 2);
                    blocks[1].init(35, MAP_HEIGHT - 4, 20, 2);
                    blocks[2].init(65, MAP_HEIGHT - 6, 15, 2);
                    blocks[3].init(90, MAP_HEIGHT - 2, 25, 2);
                    blocks[4].init(130, MAP_HEIGHT - 4, 20, 2);
                    blocks[5].init(170, MAP_HEIGHT - 2, 25, 2);

                    coins[0].init(40, MAP_HEIGHT - 6);
                    coins[1].init(70, MAP_HEIGHT - 8);
                    coins[2].init(95, MAP_HEIGHT - 4);
                    coins[3].init(135, MAP_HEIGHT - 6);
                    coins[4].init(175, MAP_HEIGHT - 4);

                    enemies[0].init(35, blocks[1].y - enemies[0].h);
                    enemies[1].init(130, blocks[4].y - enemies[1].h);
                    enemies[2].init(175, blocks[5].y - enemies[2].h);

                    winZone.init(200, MAP_HEIGHT - 4);
                    break;
                }

                case 3: {
                    levelLength = 240.0f;

                    blocks[0].init(0, MAP_HEIGHT - 2, 25, 2);
                    blocks[1].init(30, MAP_HEIGHT - 4, 20, 2);
                    blocks[2].init(60, MAP_HEIGHT - 6, 15, 2);
                    blocks[3].init(85, MAP_HEIGHT - 2, 25, 2);
                    blocks[4].init(120, MAP_HEIGHT - 4, 20, 2);
                    blocks[5].init(155, MAP_HEIGHT - 6, 20, 2);
                    blocks[6].init(190, MAP_HEIGHT - 2, 25, 2);

                    coins[0].init(35, MAP_HEIGHT - 5);
                    coins[1].init(65, MAP_HEIGHT - 7);
                    coins[2].init(90, MAP_HEIGHT - 3);
                    coins[3].init(125, MAP_HEIGHT - 5);
                    coins[4].init(160, MAP_HEIGHT - 7);
                    coins[5].init(195, MAP_HEIGHT - 3);

                    enemies[0].init(35, blocks[1].y - enemies[0].h);
                    enemies[1].init(125, blocks[4].y - enemies[1].h);
                    enemies[2].init(195, blocks[6].y - enemies[2].h);

                    winZone.init(220, MAP_HEIGHT - 4);
                    break;
                }
            }
        }



        void updatePlayerPhysics() {
            player.x += player.vx;

            for (int i = 0; i < NUM_BLOCKS; ++i) {
                Block &b = blocks[i];
                if (!b.active) continue;
                if (player.intersects(b)) {
                    if (player.vx > 0) {
                        player.x = b.x - player.w;
                    } else if (player.vx < 0) {
                        player.x = b.x + b.w;
                    }
                    player.vx = 0;
                }
            }

            player.vy += GRAVITY;
            player.y += player.vy;
            player.onGround = false;

            for (int i = 0; i < NUM_BLOCKS; ++i) {
                Block &b = blocks[i];
                if (!b.active) continue;

                if (player.intersects(b)) {
                    if (player.vy > 0) {
                        player.y = b.y - player.h;
                        player.vy = 0;
                        player.onGround = true;
                    } else if (player.vy < 0) {
                        player.y = b.y + b.h;
                        player.vy = 0;
                    }
                }
            }
        }

        void checkPlayerFall() {
            if (player.y > MAP_HEIGHT) {
                loadLevel(currentLevel);
            }
        }

        void updateCoins() {
            for (int i = 0; i < NUM_COINS * 2; i++) {
                Coin &c = coins[i];
                if (!c.active) continue;

                c.vy += GRAVITY;
                c.y += c.vy;

                for (int j = 0; j < NUM_BLOCKS; j++) {
                    if (!blocks[j].active) continue;
                    if (c.intersects(blocks[j])) {
                        if (c.vy > 0) {
                            c.y = blocks[j].y - c.h;
                            c.vy = 0;
                        } else if (c.vy < 0) {
                            c.y = blocks[j].y + blocks[j].h;
                            c.vy = 0;
                        }
                    }
                }

                if (c.y + c.h >= MAP_HEIGHT) {
                    c.y = MAP_HEIGHT - c.h;
                    c.vy = 0;
                }
            }
        }

        void updateEnemies() {
            for (int i = 0; i < NUM_ENEMIES; i++) {
                Enemy &e = enemies[i];
                if (!e.active) continue;

                e.vy += GRAVITY;
                e.y += e.vy;

                for (int b = 0; b < NUM_BLOCKS; b++) {
                    if (blocks[b].active && e.intersects(blocks[b])) {
                        if (e.vy > 0) {
                            e.y = blocks[b].y - e.h;
                            e.vy = 0;
                        }
                    }
                }

                e.x += e.vx;

                for (int b = 0; b < NUM_BLOCKS; b++) {
                    if (blocks[b].active && e.intersects(blocks[b])) {
                        e.x -= e.vx;
                        e.vx = -e.vx;
                        break;
                    }
                }

                float footX = (e.vx > 0) ? e.x + e.w : e.x - 0.1f;
                float footY = e.y + e.h + 0.1f;

                bool groundAhead = false;
                for (int b = 0; b < NUM_BLOCKS; b++) {
                    if (!blocks[b].active) continue;

                    Block &blk = blocks[b];
                    bool withinX = (footX >= blk.x) && (footX <= blk.x + blk.w);
                    bool justBelow = (footY >= blk.y) && (footY <= blk.y + blk.h);
                    if (withinX && justBelow) {
                        groundAhead = true;
                        break;
                    }
                }

                if (!groundAhead) {
                    e.vx = -e.vx;
                }
            }
        }

        void checkCollisions() {
            for(int i = 0; i < NUM_ENEMIES; ++i){
                Enemy &e = enemies[i];
                if (!e.active) continue;
                if (player.intersects(e)) {
                    if (player.vy > 0 && player.y + player.h <= e.y + e.h*0.5f) {
                        score += 50;
                        e.active=false;
                    } else { 
                        loadLevel(currentLevel);
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

            if (player.intersects(winZone)) {
                int nextLevel = currentLevel + 1;
                if (nextLevel > 3) nextLevel = 1;
                loadLevel(nextLevel);
            }

        }

        void scrollWorld(float dx) {
            float worldShift = dx;

            for (int i = 0; i < NUM_BLOCKS; i++) {
                if (blocks[i].active) blocks[i].x += worldShift;
            }
            for (int i = 0; i < NUM_ENEMIES; i++) {
                if (enemies[i].active) enemies[i].x += worldShift;
            }
            for (int i = 0; i < NUM_COINS * 2; i++) {
                if (coins[i].active) coins[i].x += worldShift;
            }
            if (winZone.active) winZone.x += worldShift;
        }

        void render() {
            // Сначала полностью очищаем карту
            for (int j = 0; j < MAP_HEIGHT; ++j) {
                for (int i = 0; i < MAP_WIDTH; ++i)
                    map[j][i] = EMPTY_CHAR;
                map[j][MAP_WIDTH] = '\0';
            }

            // Рисуем только активные объекты
            for (int i = 0; i < NUM_BLOCKS; ++i)
                if (blocks[i].active) drawObject(blocks[i]);

            for (int i = 0; i < NUM_ENEMIES; ++i)
                if (enemies[i].active) drawObject(enemies[i]);

            for (int i = 0; i < NUM_COINS; ++i)
                if (coins[i].active) drawObject(coins[i]);

            if (winZone.active) drawObject(winZone);

            drawObject(player);

            // Выводим Score и Level
            char buf[40];
            sprintf(buf, "Score: %d  Level: %d", score, currentLevel);
            for (int i = 0; i < strlen(buf); ++i)
                map[0][i] = buf[i];

            // Вывод карты в консоль
            setCursor(0, 0);
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD written;
            for (int j = 0; j < MAP_HEIGHT; ++j)
                WriteConsoleOutputCharacterA(h, map[j], MAP_WIDTH, {0, (SHORT)j}, &written);
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
                float dx;
                player.handleInput(dx);
                updatePlayerPhysics();
                updateEnemies();
                updateCoins();
                checkCollisions();
                checkPlayerFall();
                player.x += dx;
                scrollWorld(-dx);
                render();
                Sleep(15);
            }
        }
};

int main() {
    Game game;
    game.run();
    return 0;
}