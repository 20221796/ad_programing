#include "CTetris.h"

using namespace std;

///**************************************************************
/// static member variables and functions
///**************************************************************

Matrix *** CTetris::setOfColorBlockObjects = NULL;
//***setOfColorBlockObjects -> **types -> *degrees -> 객체

void CTetris::init(int **setOfColorBlockArrays, int nTypes, int nDegrees) {
  if (setOfColorBlockObjects != NULL) // already allocated?
    deinit();

  Tetris::init(setOfColorBlockArrays, nTypes, nDegrees); // call superclass' function

  // write the rest of this function!!

  setOfColorBlockObjects = new Matrix**[numTypes]; // allocate 1d array of pointers to Matrix pointer arrays

  for (int t = 0; t < numTypes; t++)
    setOfColorBlockObjects[t] = new Matrix*[numDegrees]; // allocate 1d array of Matrix pointers
    
  for (int t = 0; t < numTypes; t++) {
    int idx;
    for (idx = 0; setOfColorBlockArrays[numDegrees * t][idx] == 0 ; idx++); // type에 맞는 색상값 찾기
    int color_number = setOfColorBlockArrays[numDegrees * t][idx];

    int cy = setOfBlockObjects[t][0] -> get_dy();
    int cx = setOfBlockObjects[t][0] -> get_dx();

    for (int d = 0; d < numDegrees; d++) {
      int array[cy][cx];

      for (int y = 0; y < cy; y++)
        for (int x = 0; x < cx; x++) 
          array[y][x] = setOfBlockObjects[t][d] -> get_array()[y][x] * color_number; //type별 Matrix에 색상값 곱하기
           
      setOfColorBlockObjects[t][d] = new Matrix((int *)array ,cy, cx);
    }    
  }

  // for (int t = 0; t < numTypes; t++)
  //   for (int d = 0; d < numDegrees; d++)
  //     cout << *setOfColorBlockObjects[t][d] << endl;

}

void CTetris::deinit(void) {
  Tetris::deinit(); // call superclass' function

  for (int t = 0; t < numTypes; t++)
    for (int d = 0; d < numDegrees; d++)
      delete setOfColorBlockObjects[t][d]; // deallocate matrix objects

  for (int t = 0; t < numTypes; t++)
    delete [] setOfColorBlockObjects[t]; // deallocate 1d array of Matrix pointers

  delete []  setOfColorBlockObjects; // deallocate 1d array of pointers to Matrix pointer arrays

  setOfColorBlockObjects = NULL;
}

///**************************************************************
/// dynamic member variables and functions
///**************************************************************

/// constructors and destructor

CTetris::CTetris(int cy, int cx) : Tetris(cy, cx) {
  iCScreen = new Matrix(iScreen);
  oCScreen = new Matrix(oScreen);
  currCBlk = NULL; //currCBlk은 포인터로 setOfColorBlockObjects[t][d]들 중 하나를 가리킴
}

CTetris::~CTetris() {
  delete iCScreen;
  delete oCScreen;
}

/// mutators
TetrisState CTetris::accept(char key) {

  TetrisState _state = Tetris::accept(key); // call superclass' function

  // write the rest of this function!!
  //Tetirs 클래스와 type degree / left top 값을 공유함
  //이동하는 거 트래킹만 해주면 됨
  
  if (_state == TetrisState::Finished)
    return _state;

  currCBlk = setOfColorBlockObjects[type][degree]; //Tetris에서 변경된 type이랑 degree로 currCBlk 결정
  Matrix *tempBlk = iCScreen->clip(top, left, top + currCBlk->get_dy(), left + currCBlk->get_dx());
  Matrix *tempBlk2 = tempBlk->add(currCBlk);
  delete tempBlk;
  oCScreen->paste(iCScreen, 0, 0);
  oCScreen->paste(tempBlk2, top, left);
  delete tempBlk2;

  // you can use the following code if you want to
  // oCScreen = deleteFullLines(oCScreen, currCBlk, top, wallDepth);

  if (_state == TetrisState::NewBlock) {
    oCScreen = deleteFullLines(oCScreen, currCBlk, top, wallDepth);
    iCScreen->paste(oCScreen, 0, 0);
  }

  // else if (_state == TetrisState::Running) {
  //   currCBlk = setOfColorBlockObjects[type][degree];
  //   Matrix *tempBlk = iCScreen->clip(top, left, top + currCBlk->get_dy(), left + currCBlk->get_dx());
  //   Matrix *tempBlk2 = tempBlk->add(currCBlk);
  //   delete tempBlk;
  //   oCScreen->paste(iCScreen, 0, 0);
  //   oCScreen->paste(tempBlk2, top, left);
  //   delete tempBlk2;
  // }

  return _state;
}