class CTetris : public Tetris {
private:
    Matrix*** setOfColorBlockObjects;

    Matrix *iCScreen;
    Matrix *oCScreen;
    Matrix *currCBlk;

public:
    static void init(int **setOfColorBlockArrays, int nTypes, int nDegrees);
    static void deinit(void);
    CTetris(int cy, int cx);
    ~CTetris();

    Matrix *get_oCScreen(void) { return oCScreen};

    TetrisState accept(char key);
}