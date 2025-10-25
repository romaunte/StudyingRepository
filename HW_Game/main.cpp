#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <windows.h>

const int MAP_HEIGHT = 25;
const int MAP_WIDTH = 120;

typedef struct SObject {
    float x, y;
    float width, height;
    float vertSpeed;
    bool IsFly;
    char cType;
    float horizonSpeed;
} TObject;

// Change global variables

char map[MAP_HEIGHT][MAP_WIDTH+1];
TObject mario;
TObject *bricks = NULL;
TObject *moving = NULL;
int movingLength;
int brick_count;
int level = 1;

void clearMap(){
    for (int i = 0; i < MAP_WIDTH; ++i)
        map[0][i] = ' ';
    map[0][MAP_WIDTH] = '\0';

    for (int j = 0; j < MAP_HEIGHT; ++j)
        sprintf(map[j], map[0]);
}

void ShowMap(){
    map[MAP_HEIGHT-1][MAP_WIDTH-1] = '\0';
    for (int j = 0; j < MAP_HEIGHT; ++j)
        printf("%s\n", map[j]);
}

void SetObjectPos(TObject *obj, float xPos, float yPos){
    obj->x = xPos;
    obj->y = yPos;
}

bool IsCollision(TObject o1, TObject o2);
void CreateLevel(int lvl);

void move_obj_vertically(TObject *obj){
    obj->IsFly = TRUE;
    obj->vertSpeed += 0.05;
    SetObjectPos(obj, obj->x, obj->y + obj->vertSpeed);


    for (int i = 0; i < brick_count; ++i){
        if (IsCollision(*obj, bricks[i])){
            obj->y -= obj->vertSpeed;
            obj->vertSpeed = 0;
            obj->IsFly = FALSE;
            if (bricks[i].cType == '+'){
                ++level;
                if (level > 2) level = 1;
                CreateLevel(level);
                Sleep(1000);
            }
            break;
        }
    }
}

// Remove Hardcode
void InitObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType){
    SetObjectPos(obj, xPos, yPos);
    obj->width = oWidth;
    obj->height = oHeight;
    obj->vertSpeed = 0;
    obj->cType = inType;
    obj->horizonSpeed = 0.2;
}

void DeleteMoving(int i){
    --movingLength;
    moving[i] = moving[movingLength];
    moving = new TObject[movingLength];
}

void MarioCollision(){
    for (int i = 0; i < movingLength; ++i){
        if (IsCollision(mario, moving[i])){
            if (mario.IsFly == TRUE && mario.vertSpeed > 0 && mario.y + mario.height < moving[i].y + moving[i].height * 0.5){
                DeleteMoving(i);
                --i;
                continue;
            } else {
                CreateLevel(level);
            }
        }
    }
}

void HorizonMoveObject(TObject *obj){
    obj[0].x += obj[0].horizonSpeed;
    for (int i = 0; i < brick_count; ++i)
        if (IsCollision(obj[0], bricks[i])){
            obj[0].x -= obj[0].horizonSpeed;
            obj[0].horizonSpeed = -obj[0].horizonSpeed;
            return;
        }

    TObject tmp = *obj;
    move_obj_vertically(&tmp);
    if (tmp.IsFly == TRUE){
        obj[0].x -= obj[0].horizonSpeed;
        obj[0].horizonSpeed = -obj[0].horizonSpeed;
    }
}

bool IsPosInMap(int x, int y){
    return x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT;
}

void PutObjectOnMap(TObject obj){
    int ix = (int)round(obj.x);
    int iy = (int)round(obj.y);
    int iWidth = (int)round(obj.width);
    int iHeight = (int)round(obj.height);

    for (int i = ix; i < (ix + iWidth); ++i)
        for (int j = iy; j < (iy + iHeight); j++)
            if (IsPosInMap(i, j))
                map[j][i] = obj.cType;
}

void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void setCur(int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void HorizonMoveMap(float dx){

    mario.x -= dx;
    for (int i = 0; i < brick_count; ++i){
        if (IsCollision(mario, bricks[i])){
            mario.x += dx;
            return;
        }
    }
    mario.x += dx;

    for (int i = 0; i < brick_count; ++i){
        bricks[i].x += dx;
    }
    for (int i = 0; i < movingLength; ++i){
        moving[i].x += dx;
    }
}

bool IsCollision(TObject o1, TObject o2){
    return (((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width))
            && ((o1.y + o1.height) > o2.y) && (o1.y < (o2.y + o2.height)));
}

void CreateLevel(int lvl){

    InitObject(&mario, 39, 10, 3, 3, '@');

    if (lvl == 1){
        brick_count = 6;
        bricks = new TObject[brick_count];       //realloc
        InitObject(bricks+0, 20, 20, 40, 5, '#');
        InitObject(bricks+1, 60, 15, 10, 10, '#');
        InitObject(bricks+2, 80, 20, 20, 5, '#');
        InitObject(bricks+3, 120, 15, 10, 10, '#');
        InitObject(bricks+4, 150, 20, 40, 5, '#');
        InitObject(bricks+5, 210, 15, 10, 10, '+');

        movingLength = 1;
        moving = new TObject[movingLength];     //realloc
        InitObject(moving+0, 25, 10, 3, 2, 'o');
    }
    if (lvl == 2){
        brick_count = 4;
        bricks = new TObject[brick_count];   //realloc
        InitObject(bricks+0, 20, 20, 40, 5, '#');
        InitObject(bricks+1, 80, 20, 15, 5, '#');
        InitObject(bricks+2, 120, 15, 15, 10, '#');
        InitObject(bricks+3, 160, 10, 15, 15, '+');
    }
}

int main(){
    //HideCursor();
    CreateLevel(level);
    system("color 9F");

    while (GetKeyState(VK_ESCAPE) >= 0){
        clearMap();

        if ((mario.IsFly == FALSE) && (GetKeyState(VK_SPACE) < 0)) mario.vertSpeed = -1;
        if (GetKeyState('A') < 0) HorizonMoveMap(1);
        if (GetKeyState('D') < 0) HorizonMoveMap(-1);

        if (mario.y > MAP_HEIGHT) CreateLevel(level);

        move_obj_vertically(&mario);
        MarioCollision();
        for (int i = 0; i < brick_count; ++i){
            PutObjectOnMap(bricks[i]);
        }
        for (int i = 0; i < movingLength; ++i){
            move_obj_vertically(moving + i);
            HorizonMoveObject(moving + i);
            PutObjectOnMap(moving[i]);
        }
        PutObjectOnMap(mario);
        setCur(0,0);
        ShowMap();

        Sleep(10);
    }

    return 0;
}