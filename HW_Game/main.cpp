#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <windows.h>

#define mapWidth 120
#define mapHeight 25

typedef struct SObject {
    float x, y;
    float width, height;
    float vertSpeed;
    bool IsFly;
    char cType;
    float horizonSpeed;
} TObject;

char map[mapHeight][mapWidth+1];
TObject mario;
TObject *brick = NULL;
TObject *moving = NULL;
int movingLength;
int brickLength;
int level = 1;

void ClearMap(){
    for (int i = 0; i < mapWidth; ++i)
        map[0][i] = ' ';
    map[0][mapWidth] = '\0';
    for (int j = 0; j < mapHeight; ++j)
        sprintf(map[j], map[0]);
}

void ShowMap(){
    map[mapHeight-1][mapWidth-1] = '\0';
    for (int j = 0; j < mapHeight; ++j)
        printf("%s\n", map[j]);
}

void SetObjectPos(TObject *obj, float xPos, float yPos){
    (*obj).x = xPos;
    (*obj).y = yPos;
}

bool IsCollision(TObject o1, TObject o2);
void CreateLevel(int lvl);

void VertMoveObject(TObject *obj){
    (*obj).IsFly = TRUE;
    (*obj).vertSpeed += 0.05;
    SetObjectPos(obj, (*obj).x, (*obj).y + (*obj).vertSpeed);


    for (int i = 0; i < brickLength; ++i){
        if (IsCollision(*obj, brick[i])){
            (*obj).y -= (*obj).vertSpeed;
            (*obj).vertSpeed = 0;
            (*obj).IsFly = FALSE;
            if (brick[i].cType == '+'){
                ++level;
                if (level > 2) level = 1;
                CreateLevel(level);
                Sleep(1000);
            }
            break;
        }
    }
}

void InitObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType){
    SetObjectPos(obj, xPos, yPos);
    (*obj).width = oWidth;
    (*obj).height = oHeight;
    (*obj).vertSpeed = 0;
    (*obj).cType = inType;
    (*obj).horizonSpeed = 0.2;
}

void HorizonMoveObject(TObject *obj){
    obj[0].x += obj[0].horizonSpeed;
    for (int i = 0; i < brickLength; ++i)
        if (IsCollision(obj[0], brick[i])){
            obj[0].x -= obj[0].horizonSpeed;
            obj[0].horizonSpeed = -obj[0].horizonSpeed;
            return;
        }

    TObject tmp = *obj;
    VertMoveObject(&tmp);
    if (tmp.IsFly == TRUE){
        obj[0].x -= obj[0].horizonSpeed;
        obj[0].horizonSpeed = -obj[0].horizonSpeed;
    }
}

bool IsPosInMap(int x, int y){
    return ((x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight));
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
    for (int i = 0; i < brickLength; ++i){
        if (IsCollision(mario, brick[i])){
            mario.x += dx;
            return;
        }
    }
    mario.x += dx;

    for (int i = 0; i < brickLength; ++i){
        brick[i].x += dx;
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
        brickLength = 6;
        brick = new TObject[brickLength];       //realloc
        InitObject(brick+0, 20, 20, 40, 5, '#');
        InitObject(brick+1, 60, 15, 10, 10, '#');
        InitObject(brick+2, 80, 20, 20, 5, '#');
        InitObject(brick+3, 120, 15, 10, 10, '#');
        InitObject(brick+4, 150, 20, 40, 5, '#');
        InitObject(brick+5, 210, 15, 10, 10, '+');

        movingLength = 1;
        moving = new TObject[movingLength];     //realloc
        InitObject(moving+0, 25, 10, 3, 2, 'o');
    }
    if (lvl == 2){
        brickLength = 4;
        brick = new TObject[brickLength];   //realloc
        InitObject(brick+0, 20, 20, 40, 5, '#');
        InitObject(brick+1, 80, 20, 15, 5, '#');
        InitObject(brick+2, 120, 15, 15, 10, '#');
        InitObject(brick+3, 160, 10, 15, 15, '+');
    }
}

int main(){
    //HideCursor();
    CreateLevel(level);
    system("color 9F");

    while (GetKeyState(VK_ESCAPE) >= 0){
        ClearMap();

        if ((mario.IsFly == FALSE) && (GetKeyState(VK_SPACE) < 0)) mario.vertSpeed = -1;
        if (GetKeyState('A') < 0) HorizonMoveMap(1);
        if (GetKeyState('D') < 0) HorizonMoveMap(-1);

        if (mario.y > mapHeight) CreateLevel(level);

        VertMoveObject(&mario);
        for (int i = 0; i < brickLength; ++i){
            PutObjectOnMap(brick[i]);
        }
        for (int i = 0; i < movingLength; ++i){
            VertMoveObject(moving + i);
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