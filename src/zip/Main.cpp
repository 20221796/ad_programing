#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <termios.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "colors.h"
#include "Matrix.h"

using namespace std;


/**************************************************************/
/**************** Linux System Functions **********************/
/**************************************************************/

char saved_key = 0;
int tty_raw(int fd);	/* put terminal into a raw mode */
int tty_reset(int fd);	/* restore terminal's mode */
  
/* Read 1 character - echo defines echo mode */
char getch() {
  char ch;
  int n;
  while (1) {
    tty_raw(0);
    n = read(0, &ch, 1);
    tty_reset(0);
    if (n > 0)
      break;
    else if (n < 0) {
      if (errno == EINTR) {
        if (saved_key != 0) {
          ch = saved_key;
          saved_key = 0;
          break;
        }
      }
    }
  }
  return ch;
}

void sigint_handler(int signo) {
  // cout << "SIGINT received!" << endl;
  // do nothing;
}

void sigalrm_handler(int signo) {
  alarm(1);
  saved_key = 's';
}

void registerInterrupt() {
  struct sigaction act, oact;
  act.sa_handler = sigint_handler;
  sigemptyset(&act.sa_mask);
#ifdef SA_INTERRUPT
  act.sa_flags = SA_INTERRUPT;
#else
  act.sa_flags = 0;
#endif
  if (sigaction(SIGINT, &act, &oact) < 0) {
    cerr << "sigaction error" << endl;
    exit(1);
  }
}

void registerAlarm() {
  struct sigaction act, oact;
  act.sa_handler = sigalrm_handler;
  sigemptyset(&act.sa_mask);
#ifdef SA_INTERRUPT
  act.sa_flags = SA_INTERRUPT;
#else
  act.sa_flags = 0;
#endif
  if (sigaction(SIGALRM, &act, &oact) < 0) {
    cerr << "sigaction error" << endl;
    exit(1);
  }
  alarm(1);
}

/**************************************************************/
/**************** Tetris Blocks Definitions *******************/
/**************************************************************/
#define MAX_BLK_TYPES 7
#define MAX_BLK_DEGREES 4

int T0D0[] = { 1, 1, 1, 1, -1 };
int T0D1[] = { 1, 1, 1, 1, -1 };
int T0D2[] = { 1, 1, 1, 1, -1 };
int T0D3[] = { 1, 1, 1, 1, -1 };

int T1D0[] = { 0, 1, 0, 1, 1, 1, 0, 0, 0, -1 };
int T1D1[] = { 0, 1, 0, 0, 1, 1, 0, 1, 0, -1 };
int T1D2[] = { 0, 0, 0, 1, 1, 1, 0, 1, 0, -1 };
int T1D3[] = { 0, 1, 0, 1, 1, 0, 0, 1, 0, -1 };

int T2D0[] = { 1, 0, 0, 1, 1, 1, 0, 0, 0, -1 };
int T2D1[] = { 0, 1, 1, 0, 1, 0, 0, 1, 0, -1 };
int T2D2[] = { 0, 0, 0, 1, 1, 1, 0, 0, 1, -1 };
int T2D3[] = { 0, 1, 0, 0, 1, 0, 1, 1, 0, -1 };

int T3D0[] = { 0, 0, 1, 1, 1, 1, 0, 0, 0, -1 };
int T3D1[] = { 0, 1, 0, 0, 1, 0, 0, 1, 1, -1 };
int T3D2[] = { 0, 0, 0, 1, 1, 1, 1, 0, 0, -1 };
int T3D3[] = { 1, 1, 0, 0, 1, 0, 0, 1, 0, -1 };

int T4D0[] = { 0, 1, 0, 1, 1, 0, 1, 0, 0, -1 };
int T4D1[] = { 1, 1, 0, 0, 1, 1, 0, 0, 0, -1 };
int T4D2[] = { 0, 1, 0, 1, 1, 0, 1, 0, 0, -1 };
int T4D3[] = { 1, 1, 0, 0, 1, 1, 0, 0, 0, -1 };

int T5D0[] = { 0, 1, 0, 0, 1, 1, 0, 0, 1, -1 };
int T5D1[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, -1 };
int T5D2[] = { 0, 1, 0, 0, 1, 1, 0, 0, 1, -1 };
int T5D3[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, -1 };

int T6D0[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1 };
int T6D1[] = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, -1 };
int T6D2[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1 };
int T6D3[] = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, -1 };
  
int *setOfBlockArrays[] = {
  T0D0, T0D1, T0D2, T0D3,
  T1D0, T1D1, T1D2, T1D3,
  T2D0, T2D1, T2D2, T2D3,
  T3D0, T3D1, T3D2, T3D3,
  T4D0, T4D1, T4D2, T4D3,
  T5D0, T5D1, T5D2, T5D3,
  T6D0, T6D1, T6D2, T6D3,
};

void drawScreen(Matrix *screen, int wall_depth)
{
  int dy = screen->get_dy();
  int dx = screen->get_dx();
  int dw = wall_depth;
  int **array = screen->get_array();

  for (int y = 0; y < dy - dw + 1; y++) {
    for (int x = dw - 1; x < dx - dw + 1; x++) {
      if (array[y][x] == 0)
	      cout << "□ ";
      else if (array[y][x] == 1)
	      cout << "■ ";
      else if (array[y][x] == 10)
	      cout << "◈ ";
      else if (array[y][x] == 20)
	      cout << "★ ";
      else if (array[y][x] == 30)
	      cout << "● ";
      else if (array[y][x] == 40)
	      cout << "◆ ";
      else if (array[y][x] == 50)
	      cout << "▲ ";
      else if (array[y][x] == 60)
	      cout << "♣ ";
      else if (array[y][x] == 70)
	      cout << "♥ ";
      else
	      cout << "X ";
    }
    cout << endl;
  }
}
  
/**************************************************************/
/******************** Tetris Main Loop ************************/
/**************************************************************/

#define SCREEN_DY  10
#define SCREEN_DX  10
#define SCREEN_DW  1

#define ARRAY_DY (SCREEN_DY + 3*SCREEN_DW)
#define ARRAY_DX (SCREEN_DX + 6*SCREEN_DW)

int arrayScreen[ARRAY_DY][ARRAY_DX] = {
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

int arrayBlk[3][3] = {
  { 0, 1, 0 },
  { 1, 1, 1 },
  { 0, 0, 0 },
};

Matrix deleteFullLines(Matrix *screen)
{
  Matrix *space = new Matrix(1,10);

  for (int line = 9; line >= 0; line--) {
    Matrix *check_line = screen->clip(line, 3, line+1, 13); //체크할 라인을 자르기
    while((check_line->sum()) == 10) {
        for (int y=line-1; y>=0; y--) {
          Matrix *temp = screen->clip(y, 3, y+1, 13); //만약 모든 원소가 1이면 한 줄씩 내리기
          screen->paste(temp, y+1, 3);
          delete temp;
        }
      screen->paste(space, 0, 3);
      delete check_line;
      Matrix *check_line = screen->clip(line, 3, line+1, 13);
    }
    delete check_line;
  }
  
  delete space;
  return screen;
}

// Matrix deleteFullLines(Matrix *screen)
// {
//   Matrix *space = new Matrix(1,10);

//   for (int line = 9; line >= 0; line--) {
//       Matrix *check_line = screen->clip(line, 3, line+1, 13);
//       if((check_line->sum()) == 10) {
//         for (int y=line-1; y>=0; y--) {
//           Matrix *temp = screen->clip(y, 3, y+1, 13);
//           screen->paste(temp, y+1, 3);
//           delete temp;
//         }      
//         screen->paste(space, 0, 3);
//       }
//     delete check_line;
//   }
//   delete space;
//   cout << *screen << endl;
//   return screen;
// }


int main(int argc, char *argv[]) {
  srand((unsigned int)time(NULL));
  char key;
  int blkType;
  int top = 0, left = 4;
  int is_on_floar = 0;
  int degree_cnt=0;
  int degree = 0;

  Matrix *setOfBlockObjects[7][4];

  //이중 반복문으로 매트릭스에 setOfBlockArrays의 원소 1개씩 넣기
  
  int blk_sizes[] = {2,3,3,3,3,3,4}; //블록의 사이즈들

  for(int blk_type=0; blk_type<7; blk_type++) {
      for(int degree=0; degree<4; degree++) {
        int size = blk_sizes[blk_type];
        int temp_blk[size][size]; //temp_blk을 블록 사이즈 크기의 2차원 배열로 생성
        int *cur_blk = setOfBlockArrays[blk_type*4+degree];

        for (int i=0; i<size; i++) {
          for (int j=0; j<size; j++) {
            temp_blk[i][j] = cur_blk[i*size+j]; //원소에 대응되는 array 원소를 넣기
          }}

        setOfBlockObjects[blk_type][degree] = new Matrix((int *) temp_blk, size, size); //2차원 배열을 통해 Matrix생성
      }
    }


  Matrix *iScreen = new Matrix((int *) arrayScreen, ARRAY_DY, ARRAY_DX);
  blkType = rand() % MAX_BLK_TYPES;
  Matrix *currBlk = setOfBlockObjects[blkType][0]; //랜덤으로 currBlk 할당받기
  Matrix *tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
  Matrix *tempBlk2 = tempBlk->add(currBlk);
  delete tempBlk;

  Matrix *oScreen = new Matrix(iScreen);
  oScreen->paste(tempBlk2, top, left);
  delete tempBlk2;
  drawScreen(oScreen, SCREEN_DW);
  delete oScreen;

  while ((key = getch()) != 'q') {
    switch (key) {
      case 'a': left--; break;
      case 'd': left++; break;
      case 's': top++; break;
      case 'w':
      degree_cnt++;
      degree = degree_cnt%4;
      currBlk = setOfBlockObjects[blkType][degree];      
      break;
      case ' ':
      while(1) {
        top++;
        tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
        tempBlk2 = tempBlk->add(currBlk);
        delete tempBlk;
        if(tempBlk2->anyGreaterThan(1)) {
          top--;
          is_on_floar++;
          break;
        }
        delete tempBlk2;
      }
      delete tempBlk2;
      break;
      default: cout << "wrong key input" << endl;
    }
    
    tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
    tempBlk2 = tempBlk->add(currBlk); //여기가 코드 덤프되는 부분
    delete tempBlk;

    if(tempBlk2->anyGreaterThan(1)) {
      switch (key) {
        case 'a': left++; break;
        case 'd': left--; break;
        case 's': top--; is_on_floar++; break;
        case 'w':
          degree_cnt--;
          degree = degree_cnt%4;
          currBlk = setOfBlockObjects[blkType][degree]; break;
        case ' ': break;
      }
      delete tempBlk2;
      tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
      tempBlk2 = tempBlk->add(currBlk);
      delete tempBlk;
    }

    oScreen = new Matrix(iScreen);
    oScreen->paste(tempBlk2, top, left);
    delete tempBlk2;
    drawScreen(oScreen, SCREEN_DW);
    
    if(is_on_floar)
    {
      degree_cnt=0;
      int full=0;
      is_on_floar--;
      top = 0, left = 4;

      delete iScreen;
      iScreen = new Matrix(deleteFullLines(oScreen));
      delete oScreen;

      blkType = rand() % MAX_BLK_TYPES;
      currBlk = setOfBlockObjects[blkType][0]; //랜덤으로 currBlk 할당받기
      tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
      tempBlk2 = tempBlk->add(currBlk);
      delete tempBlk;

      if(tempBlk2->anyGreaterThan(1))
      {
        delete tempBlk2;
        cout << "Game over!" << endl; //새로운 블록이 바로 충돌하면 게임 끝내기
        break;
      }

      oScreen = new Matrix(iScreen);
      oScreen->paste(tempBlk2, top, left);
      delete tempBlk2;
      drawScreen(oScreen, SCREEN_DW);
    }
    delete oScreen;
  }
  
  for (int i=0; i<7; i++) {
    for (int j=0; j<4; j++) {
      delete setOfBlockObjects[i][j];
    }
  }

  delete iScreen;
  // delete currBlk;

  cout << "(nAlloc, nFree) = (" << Matrix::get_nAlloc() << ',' << Matrix::get_nFree() << ")" << endl;  
  cout << "Program terminated!" << endl;

  return 0;
}