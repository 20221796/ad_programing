enum class TetrisState {
    NewBlock,
    Running,
    Finished,
};

class Tetris {
protected:
    //static
    //screen
    static int wall_depth;
    //blk
    static int numTypes;
    static int numDegrees;
    static Matrix ***setofBlockObjects;
    //dynamic
    //screen
    int cols;
    int rows;
    //blk
    int type;
    int degree;
    int top;
    int left;
    
    //tetris
    Matrix *iScreen;
    Matrix *oScreen;
    Matrix *currBlk;
    TetrisState state; //까먹지 말기!

public:
    Tetris(int cy, int cx);
    ~Tetris();

    static void init(int nTypes, int nDegrees, int **setOfBlockArrays);
    static void deinit(void);

    static int get_wallDepth(void);
    static int get_numTypes(void);
    Matrix get_oScreen(void);

    TetrisState accept(char key);
}