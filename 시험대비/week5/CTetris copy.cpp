
void init(int **setOfColorBlockArrays, int nTypes, int nDegrees) {
  setOfColorBlockObjects = new Matrix**[nTypes];
  int t,d;

  for (t=0; t<nTypes; t++)
    setOfColorBlockObjects[t] = new Matrix*[nDegrees];
  

  for (t=0; t<nTypes; t++) {
    int size = setOfBlockObjects[t*nTypes];

    for (d=0; d<nDegrees; d++)
      setOfColorBlockObjects[t][d] = new Matrix (setOfColorBlockArrays[t*nTypes+d],size,size);
  }
}

void deinit(void) {
  for (int t=0; t<nTypes; t++)
    for (int d=0; d<nDegrees; d++)
      delete setOfColorBlockObjects[t][d];

  for (int t=0; t<nTypes; t++)
    delete[] setOfColorBlockObjects[t];
  
  delete[] setOfColorBlockObjects;
}

CTetris(int cy, int cx) : Tetris(cy, cx) {
  iCScreen = new Matrix(iScreen);
  oCScreen = new Matrix(oCScreen);
  currCBlk = NULL;
}

~CTetris() { 
  delete iCScreen;
  delete oCScreen;
}

TetrisState CTetris::accept(char key) :  {
  TetrisState _state = Tetris::accept(key);
  currCBlk = setOfColorBlockObjects[type][degree];

  Matrix* tempBlk;
  Matrix* tempBlk2;
  tempBlk = iCScreen->clip(top, left, top+currCBlk->get_dy(), left+currCBlk->get_dy());
  tempBlk2 = tempBlk->add(currCBlk);
  delete tempBlk;

  oCScreen->paste(iScreen,0,0);
  oCScreen->paste(tempBlk2, top, left);
  delete tempBlk2;

  if (_state==TetrisState::NewBlock) {
    oCScreen = MydeleteFullLines(oCScreen, currCBlk, top, wall_depth);
    iCScreen->paste(oCScreen, 0 ,0);
  }

  return _state;
}