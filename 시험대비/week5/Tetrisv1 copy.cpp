
void init(int nTypes, int nDegrees, int **setOfBlockArrays) {
  int t,d;
  setofBlockObjects = new Matrix**[nTypes];

  for (t=0; t<nTypes; t++)
    setOfBlockObjects[t] = new Matrix*[nDegrees];
  
  for (t=0; t<nTypes; t++) {
    int *array1 = setOfBlockArrays[t*nTypes];
    int len, size;

    for(len=0; array1[len]!=-1; len++);
    for(size=0; size*size<len; size++);
    if (wall_depth<size) wall_depth = size;
    
    for (d=0; d<nDegrees; d++) {
      int array2[size][size];
      
      for (int i=0; i<size; i++)
        for (int j=0; j<size; j++)
          array2[i][j] = (setOfBlockArrays[t*nTypes+d][i*size+j]==0 ? 0:1);

      setofBlockObjects[t][d] = new Matrix((int*)array2, size, size);    
    } 
  }
}

void deinit(void) {
  for (int t=0; t<numTypes; t++)
    for (int d=0; d<numDegrees; d++)
      delete setofBlockObjects[t][d];
  
  for (int t=0; t<numTypes; t++)
    delete[] setofBlockObjects[t];
  
  delete[] setofBlockObjects;
  setofBlockObjects = NULL;
}

int *allocArrayScreen(int dy, int dx, int dw) {
  int y,x;
  int **array2d = new int*[dy + dw];

  for (y=0; y<dy+dw; y++)
    array2d[y] = new int[dx+2*dw];
  
  for (y=0; y<dy+dw; y++) {
    for (x=0; x<dw; x++) {
      array2d[y][x] = 1;
      array2d[y][dw+dx+x] = 1;
    }

    if (y<dy) {
      for (x=0; x<dx; x++) 
        array2d[y][dw+x] = 0;
    }
    else {
      for (x=0; x<dx; x++)
        array2d[y][dw+x] = 1;
    }
  }

  int *array1d = new int[(dy+dw)*(dx+2*dw)];

  for (y=0; y<dy+dw; y++)
    for (x=0; x<dx+2*dw; x++)
      array1d[y*(dy+dw)+x] = array2d[y][x];
  

  for (y=0; y<dy+dw; y++)
    delete[] array2d[y];
  
  delete[] array2d;

  return array1d;
}

void deallocArrayScreen(int* arrayScreen) {
  delete[] arrayScreen;
  arrayScreen = NULL;
}

Tetris::Tetris(int cy, int cx) {
  cols = cy + wall_depth;
  rows = cx + 2*wall_depth
  type = -1;
  degree = 0;
  top = 0;
  left = cols/2-wall_depth/2;
  
  int *arrayScreen = allocArrayScreen(cy, cx, wall_depth);
  iScreen = new Matrix(arrayScreen, cols, rows);
  oScreen = new Matrix(iScreen);
  currBlk = NULL;

  deallocArrayScreen(arrayScreen);
  state = TetrisState::NewBlock;
}

~Tetris() {
  delete iScreen;
  delete oScreen;
}

TetrisState::accept(char key) {
  if (state == TetrisState::Finished)
    return state;

  else if (state == TetrisState::NewBlock) {
    int idx = key - '0';

    type = idx;
    degree = 0;
    top = 0;
    left = cols/2 - wall_depth/2;
    state = TetrisState::Running;

    currBlk = setofBlockObjects[type][degree];
    tempBlk = clip(iScreen, top, left, top+currBlk->get_dy(), left+currBlk->get_dx());
    tempBlk2 = tempBlk->add(currBlk);
    delete tempBlk;

    oScreen->paste(iScreen,0,0);
    oScreen->paste(tempBlk2, top, left);
    
    if (tempBlk2->anyGreaterThan(1))
      state = TetrisState::Finished;

    delete tempBlk2;
    return state;
  }
  else if (state == TetrisState::Running) {
    
  }
}