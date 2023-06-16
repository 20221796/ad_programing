
class ActionHandler {
public:
    virtual void run(Tetris *t, char key) = 0; //내용 없을 시 0
}

class TetrisOperation {
public:
    char key;
    TetrisState preState;
    ActionHandler hAction;
    TetrisState postAState;
    ActionHandler hCounterAction;
    TetrisState postCState;

    TetrisOperation(char ch, TetrisState s0, ActionHandler h1, TetrisState s1, ActionHandler h2, TetrisState s2) {
        key = ch;
        preState = s0;
        hAction = h1;
        postAState = s1;
        hCounterAction h2;
        postCState = s2;
    }

    ~TetrisOperation() {
        delete hAction;
        delete hCounterAction;
    }
}

class Tetris {
protected://static
    static int nOps;
    ActionHandler *operations[MAX_TETRIS_OPERATIONS];
    static int findOpIdxByKey(char key);
    static void setDefaultOperations(void);

public:
    static void setOperation(char ch, TetrisState s0, ActionHandler h1, TetrisState s1, ActionHandler h2, TetrisState s2);    
}