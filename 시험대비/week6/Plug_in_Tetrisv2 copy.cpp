
int findOpIdxByKey(char key) {
  for (int idx = 0; operations[idx]!=NULL; idx++)
    if (operations[idx]->key == key)
      return idx;
  return -1;
}

void setOperation(char ch, Tetrisstate s0, ActionHandler* h1, Tetrisstate s1, ActionHandler* h2, Tetrisstate s2) {
  int idx = findOpIdxByKey(ch);

  if (idx>0) {
    delete operations[idx];
    operations[idx] = new TetrisOperation(ch, s0, h1, s1, h2, s2);
  }

  else {
    if (nOps == MAX_TETRIS_OPERATIONS)
      //full op
      return
    
    operations[nOps] = new TetrisOperation(ch, s0, h1, s1, h2, s2);
    nOps++;
  }
}

TetrisState accept(char key) {
  int idx = findOpIdxByKey(key);

  if (idx < 0) {
    cout<<"wrong key!"<<endl;
    return state;
  }

  TetrisOperation *op = operations[idx];

  if (state != op->preState) {
    cout<<"wrong state"<<endl;
    return state;
  }
  else {
    op->hAction->run(this, key);
    Matrix *tempBlk = overlap_currBlk(void);

    if(anyConflict(tempBlk)) {
      op->hCounterAction->run(this, key);
      delete tempBlk;
      tempBlk = overlap_currBlk(void);
      state = op->postCState;
    }
    else {
      state = op->postAState;
    }
    
    update_oScreen(tempBlk, top, left);
    delete tempBlk;
    return state;
  }
}

Matrix* overlap_currBlk(void) {
  Matrix* tBlk1, tBlk2;
  tBlk1 = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
  tBlk2 = tBlk1->add(currBlk);
  delete tBlk1;

  return tBlk2;
}

void update_oScreen(Matrix* tempBlk, int top, int left) {
  oScreen->paste(iScreen,0,0);
  oScreen->paste(tempBlk, top, left);
}