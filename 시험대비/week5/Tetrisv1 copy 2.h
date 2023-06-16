enum class TetrisState {
    NewBlock,
    Running,
    Finished,
}

class Tetris {
protected:
    //static
    //screen
    static int wall_depth;
    //blk
    static int numTypes;
    static int numDegrees;
    //tetris
    static Matrix*** setofBlockObjects;

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

public:
    //blk
    static void init(int** setOfBlockArrays, int ntypes, int nDegrees);
    static void deinit(void);
    static int get_numTypes(void) {return numTypes};

    //screen
    static int get_wallDepth(void) {return wall_depth};

    //tetris
    Tetris(int cy, int cx);
    ~Tetris();
    Matrix* get_oScreen(void) {return oScreen};

    TetrisState accept(char key);
}