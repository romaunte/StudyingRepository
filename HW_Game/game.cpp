#include <windows.h>
#include <cmath>
#include <cstdio>
#include <conio.h>

const int MAP_WIDTH  = 80;
const int MAP_HEIGHT = 25;

const char PLAYER_CHAR = '@';
const char BLOCK_CHAR  = '█';
const char EMPTY_CHAR  = ' ';

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
