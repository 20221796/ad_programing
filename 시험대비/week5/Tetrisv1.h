#pragma once
#include <iostream>
#include <cstdlib>
#include "Matrix.h"

using namespace std;

enum class TetrisState {
    NewBlock,
    Running,
    Finished,
};
//열거형: 0부터 시작해서 1씩 증가하는 상수로 값이 자동으로 부여
//위 코드의 경우 0 1 2

extern Matrix *deleteFullLines(Matrix *screen, Matrix *blk, int top, int dw);
//extern int *allocArrayScreen(int dy, int dx, int dw);
//extern void deallocArrayScreen(int *array);

// public = 어디서든 접근이 가능.
// protected = 상속관계일 때 접근이 가능
// private = 해당 클래스에서만 접근이 가능

class Tetris {
protected:
    // static members
    static Matrix ***setOfBlockObjects;
    static int numTypes;
    static int numDegrees;
    static int wallDepth;

    // dynamic members
    int rows; // rows of screen = dy + wallDepth
    int cols; // columns of screen = dx + 2*wallDepth
    int type;
    int degree;
    int top; 
    int left;

    TetrisState state;
    Matrix *iScreen;
    Matrix *oScreen;
    Matrix *currBlk;

public:
    static void init(int **setOfBlockArrays, int nTypes, int nDegrees);
    static void deinit(void);
    Tetris(int cy, int cx);
    ~Tetris();

    // accessors
    static int get_wallDepth(void)  {  return wallDepth; }
    static int get_numTypes(void)  {  return numTypes; }
    Matrix *get_oScreen(void) const {  return oScreen; }

    // mutators
    TetrisState accept(char key);
};