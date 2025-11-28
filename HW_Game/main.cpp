#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

const int MAP_HEIGHT = 25;
const int MAP_WIDTH = 120;

typedef struct SObject {
    float x, y;
    float width, height;
    float vertical_speed;
    bool is_flying;
    char obj_type;
    float horizontal_speed;
} Obj;

typedef struct {
    char map[MAP_HEIGHT][MAP_WIDTH + 1];
    Obj mario;
    Obj *bricks;
    Obj *movings;
    int movable_count;
    int brick_count;
    int level;
    int score;
    int max_level;
} GameState;

const char MARIO = '@';
const char BRICK = '#';
const char ENEMY = 'o';
const char BOX = '?';
const char EMPTY = '-';
const char COIN = '$';
const char WIN_ZONE = '+';

// Объявления функций в алфавитном порядке
void clear_map(GameState *state);
void create_level(GameState *state);
void death(GameState *state);
void delete_movings(GameState *state, const int i);
void hide_cursor();
void horizontal_move_map(GameState *state, const float dx);
void init_obj(Obj *obj, const float x_pos, const float y_pos, const float o_width, const float o_height, const char init_type);
bool is_collision(const Obj o1, const Obj o2);
bool is_pos_in_map(const int x, const int y);
void move_obj_horizontally(GameState *state, Obj *obj);
void move_obj_vertically(GameState *state, Obj *obj);
void player_collision(GameState *state);
void put_obj_on_map(GameState *state, const Obj obj);
void put_score_on_map(GameState *state);
void set_cursor(const int x, const int y);
void set_obj_pos(Obj *obj, const float x_pos, const float y_pos);
void show_map(GameState *state);

int main() {
    GameState state;
    memset(&state, 0, sizeof(GameState));
    state.bricks = NULL;
    state.movings = NULL;
    state.level = 1;
    state.score = 0;
    state.max_level = 0;

    hide_cursor();
    create_level(&state);

    while (GetKeyState(VK_ESCAPE) >= 0) {
        clear_map(&state);

        if ((state.mario.is_flying == FALSE) && (GetKeyState(VK_SPACE) < 0))
            state.mario.vertical_speed = -1;

        if (GetKeyState('A') < 0)
            horizontal_move_map(&state, 1);

        if (GetKeyState('D') < 0)
            horizontal_move_map(&state, -1);

        if (state.mario.y > MAP_HEIGHT)
            death(&state);

        move_obj_vertically(&state, &state.mario);
        player_collision(&state);

        for (int i = 0; i < state.brick_count; ++i) {
            put_obj_on_map(&state, state.bricks[i]);
        }

        for (int i = 0; i < state.movable_count; ++i) {
            move_obj_vertically(&state, state.movings + i);
            move_obj_horizontally(&state, state.movings + i);
            if (state.movings[i].y > MAP_HEIGHT) {
                delete_movings(&state, i);
                --i;
                continue;
            }
            put_obj_on_map(&state, state.movings[i]);
        }

        put_obj_on_map(&state, state.mario);
        put_score_on_map(&state);
        set_cursor(0, 0);
        show_map(&state);

        Sleep(30);
    }

    if (state.bricks) {
        delete[] state.bricks;
    }
    if (state.movings) {
        delete[] state.movings;
    }

    return 0;
}

// Реализации функций в алфавитном порядке

void clear_map(GameState *state) {
    for (int j = 0; j < MAP_HEIGHT; ++j) {
        memset(state->map[j], ' ', MAP_WIDTH);
    }
}

void create_level(GameState *state) {
    system("color 9F");
    state->brick_count = 0;
    state->movable_count = 0;

    if (state->bricks) {
        delete[] state->bricks;
        state->bricks = NULL;
    }
    if (state->movings) {
        delete[] state->movings;
        state->movings = NULL;
    }

    init_obj(&state->mario, 39.0f, 10.0f, 3.0f, 3.0f, MARIO);
    state->score = 0;

    int lvl = state->level;

    if (lvl == 1) {
        state->brick_count = 13;
        state->bricks = new Obj[state->brick_count];

        init_obj(state->bricks + 0, 30, 10, 5, 3, BOX);
        init_obj(state->bricks + 1, 50, 10, 5, 3, BOX);
        init_obj(state->bricks + 2, 70, 5, 5, 3, BOX);
        init_obj(state->bricks + 3, 80, 5, 5, 3, BOX);

        init_obj(state->bricks + 4, 60, 5, 10, 3, EMPTY);
        init_obj(state->bricks + 5, 75, 5, 5, 3, EMPTY);
        init_obj(state->bricks + 6, 85, 5, 10, 3, EMPTY);

        init_obj(state->bricks + 7, 20, 20, 40, 5, BRICK);
        init_obj(state->bricks + 8, 60, 15, 40, 10, BRICK);
        init_obj(state->bricks + 9, 100, 20, 20, 5, BRICK);
        init_obj(state->bricks + 10, 120, 15, 10, 10, BRICK);
        init_obj(state->bricks + 11, 150, 20, 40, 5, BRICK);

        init_obj(state->bricks + 12, 210, 15, 10, 10, WIN_ZONE);

        state->movable_count = 6;
        state->movings = new Obj[state->movable_count];

        init_obj(state->movings + 0, 25, 10, 3, 2, ENEMY);
        init_obj(state->movings + 1, 80, 10, 3, 2, ENEMY);
    }

    if (lvl == 2) {
        state->brick_count = 6;
        state->bricks = new Obj[state->brick_count];

        init_obj(state->bricks + 0, 20, 20, 40, 5, BRICK);
        init_obj(state->bricks + 1, 60, 15, 10, 10, BRICK);
        init_obj(state->bricks + 2, 80, 20, 20, 5, BRICK);
        init_obj(state->bricks + 3, 120, 15, 10, 10, BRICK);
        init_obj(state->bricks + 4, 150, 20, 40, 5, BRICK);

        init_obj(state->bricks + 5, 210, 15, 10, 10, WIN_ZONE);

        state->movable_count = 6;
        state->movings = new Obj[state->movable_count];

        init_obj(state->movings + 0, 25, 10, 3, 2, ENEMY);
        init_obj(state->movings + 1, 80, 10, 3, 2, ENEMY);
        init_obj(state->movings + 2, 65, 10, 3, 2, ENEMY);
        init_obj(state->movings + 3, 120, 10, 3, 2, ENEMY);
        init_obj(state->movings + 4, 160, 10, 3, 2, ENEMY);
        init_obj(state->movings + 5, 175, 10, 3, 2, ENEMY);
    }

    if (lvl == 3) {
        state->brick_count = 4;
        state->bricks = new Obj[state->brick_count];

        init_obj(state->bricks + 0, 20, 20, 40, 5, BRICK);
        init_obj(state->bricks + 1, 80, 20, 15, 5, BRICK);
        init_obj(state->bricks + 2, 120, 15, 15, 10, BRICK);
        init_obj(state->bricks + 3, 160, 10, 15, 15, WIN_ZONE);

        state->movable_count = 6;
        state->movings = new Obj[state->movable_count];

        init_obj(state->movings + 0, 25, 10, 3, 2, ENEMY);
        init_obj(state->movings + 1, 50, 10, 3, 2, ENEMY);
        init_obj(state->movings + 2, 80, 10, 3, 2, ENEMY);
        init_obj(state->movings + 3, 90, 10, 3, 2, ENEMY);
        init_obj(state->movings + 4, 120, 10, 3, 2, ENEMY);
        init_obj(state->movings + 5, 130, 10, 3, 2, ENEMY);
    }

    state->max_level = 3;
}

void death(GameState *state) {
    system("color 4F");
    Sleep(500);
    create_level(state);
}

void delete_movings(GameState *state, const int i) {
    for (int j = i; j < state->movable_count; ++j) {
        state->movings[j] = state->movings[j + 1];
    }
}

void hide_cursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void horizontal_move_map(GameState *state, const float dx) {
    state->mario.x -= dx;
    for (int i = 0; i < state->brick_count; ++i) {
        if (is_collision(state->mario, state->bricks[i])) {
            state->mario.x += dx;
            return;
        }
    }
    state->mario.x += dx;

    for (int i = 0; i < state->brick_count; ++i)
        state->bricks[i].x += dx;
    for (int i = 0; i < state->movable_count; ++i)
        state->movings[i].x += dx;
}

void init_obj(Obj *obj, const float x_pos, const float y_pos, const float o_width, const float o_height, const char init_type) {
    set_obj_pos(obj, x_pos, y_pos);
    obj->width = o_width;
    obj->height = o_height;
    obj->vertical_speed = 0.0f;
    obj->obj_type = init_type;
    obj->horizontal_speed = 0.2f;
}

bool is_collision(const Obj o1, const Obj o2) {
    return (
        (o1.x + o1.width) > o2.x &&
        o1.x < (o2.x + o2.width) &&
        (o1.y + o1.height) > o2.y &&
        o1.y < (o2.y + o2.height)
    );
}

bool is_pos_in_map(const int x, const int y) {
    return x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT;
}

void move_obj_horizontally(GameState *state, Obj *obj) {
    obj->x += obj->horizontal_speed;
    for (int i = 0; i < state->brick_count; ++i) {
        if (is_collision(*obj, state->bricks[i])) {
            obj->x -= obj->horizontal_speed;
            obj->horizontal_speed = -obj->horizontal_speed;
            return;
        }
    }

    if (obj->obj_type == ENEMY) {
        Obj tmp = *obj;
        move_obj_vertically(state, &tmp);
        if (tmp.is_flying == TRUE) {
            obj->x -= obj->horizontal_speed;
            obj->horizontal_speed = -obj->horizontal_speed;
        }
    }
}

void move_obj_vertically(GameState *state, Obj *obj) {
    obj->is_flying = TRUE;
    obj->vertical_speed += 0.05f;
    set_obj_pos(obj, obj->x, obj->y + obj->vertical_speed);

    for (int i = 0; i < state->brick_count; ++i) {
        if (is_collision(*obj, state->bricks[i])) {
            if (obj->vertical_speed > 0) {
                obj->is_flying = FALSE;
            }
            if (state->bricks[i].obj_type == BOX && obj->vertical_speed < 0 && obj == &state->mario) {
                state->bricks[i].obj_type = EMPTY;
                init_obj(state->movings + i + 2, state->bricks[i].x, state->bricks[i].y - 3, 3, 2, COIN);
                state->movings[i + 2].vertical_speed = -0.7f;
            }
            obj->y -= obj->vertical_speed;
            obj->vertical_speed = 0.0f;
            if (state->bricks[i].obj_type == WIN_ZONE) {
                ++state->level;
                if (state->level > state->max_level)
                    state->level = 1;
                system("color 2F");
                Sleep(500);
                create_level(state);
            }
            break;
        }
    }
}

void player_collision(GameState *state) {
    for (int i = 0; i < state->movable_count; ++i) {
        if (is_collision(state->mario, state->movings[i])) {
            if (state->movings[i].obj_type == ENEMY) {
                if (state->mario.is_flying == TRUE &&
                    state->mario.vertical_speed > 0 &&
                    state->mario.y + state->mario.height < state->movings[i].y + state->movings[i].height * 0.5f) {
                    state->score += 50;
                    delete_movings(state, i);
                    --i;
                    continue;
                } else {
                    death(state);
                }
            }
            if (state->movings[i].obj_type == COIN) {
                state->score += 100;
                delete_movings(state, i);
                --i;
                continue;
            }
        }
    }
}

void put_obj_on_map(GameState *state, const Obj obj) {
    int i_x = (int)round(obj.x);
    int i_y = (int)round(obj.y);
    int i_width = (int)round(obj.width);
    int i_height = (int)round(obj.height);

    for (int i = i_x; i < (i_x + i_width); ++i) {
        for (int j = i_y; j < (i_y + i_height); ++j) {
            if (is_pos_in_map(i, j)) {
                state->map[j][i] = obj.obj_type;
            }
        }
    }
}

void put_score_on_map(GameState *state) {
    char c[30];
    sprintf(c, "Score %d", state->score);
    int len = (int)strlen(c);
    for (int i = 0; i < len; ++i)
        state->map[1][i + 5] = c[i];
}

void set_cursor(const int x, const int y) {
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void set_obj_pos(Obj *obj, const float x_pos, const float y_pos) {
    obj->x = x_pos;
    obj->y = y_pos;
}

void show_map(GameState *state) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int j = 0; j < MAP_HEIGHT; ++j) {
        DWORD written;
        WriteConsoleOutputCharacterA(hConsole, state->map[j], MAP_WIDTH, (COORD){ 0, (SHORT)j }, &written);
    }
}
