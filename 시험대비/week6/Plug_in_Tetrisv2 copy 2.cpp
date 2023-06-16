int findOpIdxByKey(char key) {
    int idx;

    for (idx = 0; operations[idx]!=NULL; idx++)
        if (operations[idx]->key == key)
            return idx;
    
    return -1;
}

void setOperation(char ch, TetrisState s0, ActionHandler* h1, TetrisState s1, ActionHandler *h2, TetrisState s2) {
    int idx = findOpIdxByKey(ch);

    if (idx > 0) {
        delete operations[idx];
        operations[idx] = TetrisOperation(s0, h1, s1, h2, s2);
    }
    else {
        if (nOps == MAX_TETRIS_OPERATIONS) {
            return
        }
        operations[nOps] = TetrisOperation(s0, h1, s1, h2, s2);
        nOps++;
    }
}

Matrix *overlap_currBlk(void) {
    Matrix* tBlk1 = clip(iScreen, top, left, top+currBlk->get_dy(), left+currBlk->get_dx());
    Matrix* tBlk2 = tBlk1->add(currBlk);
    delete tBlk1;
    return tBlk2;
}

void update_oScreen(Matrix *blk, int top, int left) {
    oScreen->paste(iScreen, 0, 0);
    oScreen->paste(blk, top, left);
}

TetrisState::accept(char key) {
    int idx = findOpIdxByKey(ch);
    if (idx < 0) {
        cout << "Wrong key" <<endl;
        return state;
    }
    else {
        TetrisOperation *op = operations[idx];
        
        if (op->preState != state) {
            cerr << "wrong state" <<endl;
            return state;
        }

        op->hAction->run(this, key);
        Matrix *tempBlk = overlap_currBlk(void);
        if (anyConflict(tempBlk) == false) {
            state = op->postAState;
        }
        else{
            delete tempBlk;
            op->hCounterAction->run(this, key);
            tempBlk = overlap_currBlk(void);
            state = op->postCState;
        }
        update_oScreen(tempBlk, top, left);
        delete tempBlk;
        return state
    }
}