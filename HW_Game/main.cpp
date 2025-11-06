#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

char map[MAP_HEIGHT][MAP_WIDTH+1];
Obj mario;
Obj *bricks = NULL;
Obj *moving = NULL;
int movable_count;
int brick_count;
int level = 1;
int score;
int max_level;

void clear_map(){
    for (int i = 0; i < MAP_WIDTH; ++i)
        map[0][i] = ' ';
    map[0][MAP_WIDTH] = '\0';

    for (int j = 0; j < MAP_HEIGHT; ++j)
        sprintf(map[j], map[0]);
}

void show_map(){
    map[MAP_HEIGHT-1][MAP_WIDTH-1] = '\0';
    for (int j = 0; j < MAP_HEIGHT; ++j)
        printf("%s\n", map[j]);
}

void set_obj_pos(Obj *obj, float x_pos, float y_pos){
    obj->x = x_pos;
    obj->y = y_pos;
}

bool is_collision(Obj o1, Obj o2){
    return (((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width))
            && ((o1.y + o1.height) > o2.y) && (o1.y < (o2.y + o2.height)));
}

void create_level(int lvl){

    system("color 9F");
    brick_count = 0;
    movable_count = 0;
    delete bricks;
    delete moving;
    init_obj(&mario, 39, 10, 3, 3, '@');
    score = 0;

        if (lvl == 1){
        brick_count = 13;
        bricks = new Obj[brick_count];

        init_obj(bricks+0, 30, 10, 5, 3, '?');
        init_obj(bricks+1, 50, 10, 5, 3, '?');
        init_obj(bricks+2, 70, 5, 5, 3, '?');
        init_obj(bricks+3, 80, 5, 5, 3, '?');

        init_obj(bricks+4, 60, 5, 10, 3, '-');
        init_obj(bricks+5, 75, 5, 5, 3, '-');
        init_obj(bricks+6, 85, 5, 10, 3, '-');

        init_obj(bricks+7, 20, 20, 40, 5, '#');
        init_obj(bricks+8, 60, 15, 40, 10, '#');
        init_obj(bricks+9, 100, 20, 20, 5, '#');
        init_obj(bricks+10, 120, 15, 10, 10, '#');
        init_obj(bricks+11, 150, 20, 40, 5, '#');
        init_obj(bricks+12, 210, 15, 10, 10, '+');

        movable_count = 6;
        moving = new Obj[movable_count];

        init_obj(moving+0, 25, 10, 3, 2, 'o');
        init_obj(moving+1, 80, 10, 3, 2, 'o');
        
    }

    if (lvl == 2){
        brick_count = 6;
        bricks = new Obj[brick_count];       //realloc
        init_obj(bricks+0, 20, 20, 40, 5, '#');
        init_obj(bricks+1, 60, 15, 10, 10, '#');
        init_obj(bricks+2, 80, 20, 20, 5, '#');
        init_obj(bricks+3, 120, 15, 10, 10, '#');
        init_obj(bricks+4, 150, 20, 40, 5, '#');
        init_obj(bricks+5, 210, 15, 10, 10, '+');

        movable_count = 6;
        moving = new Obj[movable_count];     //realloc
        init_obj(moving+0, 25, 10, 3, 2, 'o');
        init_obj(moving+1, 80, 10, 3, 2, 'o');
        init_obj(moving+2, 65, 10, 3, 2, 'o');
        init_obj(moving+3, 120, 10, 3, 2, 'o');
        init_obj(moving+4, 160, 10, 3, 2, 'o');
        init_obj(moving+5, 175, 10, 3, 2, 'o');
    }
    if (lvl == 3){
        brick_count = 4;
        bricks = new Obj[brick_count];   //realloc
        init_obj(bricks+0, 20, 20, 40, 5, '#');
        init_obj(bricks+1, 80, 20, 15, 5, '#');
        init_obj(bricks+2, 120, 15, 15, 10, '#');
        init_obj(bricks+3, 160, 10, 15, 15, '+');

        movable_count = 6;
        moving = new Obj[movable_count];     //realloc
        init_obj(moving+0, 25, 10, 3, 2, 'o');
        init_obj(moving+1, 50, 10, 3, 2, 'o');
        init_obj(moving+2, 80, 10, 3, 2, 'o');
        init_obj(moving+3, 90, 10, 3, 2, 'o');
        init_obj(moving+4, 120, 10, 3, 2, 'o');
        init_obj(moving+5, 130, 10, 3, 2, 'o');
    }
    max_level = 3;
}

void init_obj(Obj *obj, float x_pos, float y_pos, float oWidth, float oHeight, char inType){
    set_obj_pos(obj, x_pos, y_pos);
    obj->width = oWidth;
    obj->height = oHeight;
    obj->vertical_speed = 0;
    obj->obj_type = inType;
    obj->horizontal_speed = 0.2;
}

void move_obj_vertically(Obj *obj){
    obj->is_flying = TRUE;
    obj->vertical_speed += 0.05;
    set_obj_pos(obj, obj->x, obj->y + obj->vertical_speed);


    for (int i = 0; i < brick_count; ++i){
        if (is_collision(*obj, bricks[i])){
            if (obj[0].vertical_speed > 0){
                obj[0].is_flying = FALSE;
            }
            if (bricks[i].obj_type == '?' && obj[0].vertical_speed < 0 && obj == &mario){
                bricks[i].obj_type = '-';
                init_obj(moving+i+2, bricks[i].x, bricks[i].y - 3, 3, 2, '$');
                moving[i+2].vertical_speed = -0.7;
            }
            obj->y -= obj->vertical_speed;
            obj->vertical_speed = 0;
            if (bricks[i].obj_type == '+'){
                ++level;
                if (level > max_level) level = 1;
                system("color 2F");
                Sleep(500);
                create_level(level);
            }
            break;
        }
    }
}

void death(){
    system("color 4F");
    Sleep(500);
    create_level(level);
}

void delete_moving(int i){
    for (int j = i; j < movable_count; ++j){
        moving[j] = moving[j + 1];
    }
}

void player_collision(){
    for (int i = 0; i < movable_count; ++i){
        if (is_collision(mario, moving[i])){
            if (moving[i].obj_type == 'o'){
                if (mario.is_flying == TRUE && mario.vertical_speed > 0 && mario.y + mario.height < moving[i].y + moving[i].height * 0.5){
                    score += 50;
                    delete_moving(i);
                    --i;
                    continue;
                } else {
                    death();
                }
            }
            if (moving[i].obj_type == '$'){
                score += 100;
                delete_moving(i);
                --i;
                continue;
            }
        }
    }
}

void horizontal_move_obj(Obj *obj){
    obj[0].x += obj[0].horizontal_speed;
    for (int i = 0; i < brick_count; ++i)
        if (is_collision(obj[0], bricks[i])){
            obj[0].x -= obj[0].horizontal_speed;
            obj[0].horizontal_speed = -obj[0].horizontal_speed;
            return;
        }
    
    if (obj[0].obj_type == 'o'){        
        Obj tmp = *obj;
        move_obj_vertically(&tmp);
        if (tmp.is_flying == TRUE){
            obj[0].x -= obj[0].horizontal_speed;
            obj[0].horizontal_speed = -obj[0].horizontal_speed;
        }
    }

}

bool is_pos_in_map(int x, int y){
    return x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT;
}

void put_obj_on_map(Obj obj){
    int ix = (int)round(obj.x);
    int iy = (int)round(obj.y);
    int iWidth = (int)round(obj.width);
    int iHeight = (int)round(obj.height);

    for (int i = ix; i < (ix + iWidth); ++i)
        for (int j = iy; j < (iy + iHeight); j++)
            if (is_pos_in_map(i, j))
                map[j][i] = obj.obj_type;
}

void hide_cursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void set_cursor(int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void horizontal_move_map(float dx){

    mario.x -= dx;
    for (int i = 0; i < brick_count; ++i){
        if (is_collision(mario, bricks[i])){
            mario.x += dx;
            return;
        }
    }
    mario.x += dx;

    for (int i = 0; i < brick_count; ++i){
        bricks[i].x += dx;
    }
    for (int i = 0; i < movable_count; ++i){
        moving[i].x += dx;
    }
}

void put_score_on_map(){
    char c[30];
    sprintf(c, "Score %d", score);
    int len = strlen(c);
    for (int i = 0; i < len; ++i){
        map[1][i+5] = c[i];
    }
}


int main(){
    create_level(level);

    while (GetKeyState(VK_ESCAPE) >= 0){
        clear_map();

        if ((mario.is_flying == FALSE) && (GetKeyState(VK_SPACE) < 0)) mario.vertical_speed = -1;
        if (GetKeyState('A') < 0) horizontal_move_map(1);
        if (GetKeyState('D') < 0) horizontal_move_map(-1);

        if (mario.y > MAP_HEIGHT) death();

        move_obj_vertically(&mario);
        player_collision();
        for (int i = 0; i < brick_count; ++i){
            put_obj_on_map(bricks[i]);
        }
        for (int i = 0; i < movable_count; ++i){
            move_obj_vertically(moving + i);
            horizontal_move_obj(moving + i);
            if (moving[i].y > MAP_HEIGHT){
                delete_moving(i);
                --i;
                continue;
            }
            put_obj_on_map(moving[i]);
        }
        put_obj_on_map(mario);
        put_score_on_map();
        set_cursor(0,0);
        show_map();

        Sleep(10);
    }

    return 0;
}