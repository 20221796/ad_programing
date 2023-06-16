class ActionHandler {
public:
    virtual void run(Tetris *t, char key) = 0;
}

class TetrisOperation {
public:
    char key;
    Tetrisstate preState;
    ActionHandler* hAction;
    Tetrisstate postAState;
    ActionHandler* hCounterAction;
    Tetrisstate postCState;

    TetrisOperation(char ch, Tetrisstate s0, ActionHandler* h1, Tetrisstate s1, ActionHandler* h2, Tetrisstate s2) {
        key = ch;
        preState = s0;
        hAction = h1;
        postAState = s1;
        hCounterAction = h2;
        postCState = s2;
    }

    ~TetrisOperation() {
        delete hAction;
        delete hCounterAction;
    }
}

class Tetris {
protected:
    //static
    static int nOps;
    TetrisOperation* operations[MAX_TETRIS_OPERATIONS];
    static int findOpIdxByKey(char key);
    static void setDefaultOperations(void);

public:
    void setOperation(char ch, Tetrisstate s0, ActionHandler* h1, Tetrisstate s1, ActionHandler* h2, Tetrisstate s2);
    //나머지 동일

    Matrix* overlap_currBlk(void);

    void update_oScreen(Matrix* tempBlk, int top, int left);

}