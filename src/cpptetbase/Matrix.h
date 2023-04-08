#pragma once
#include <iostream>
#include <cstdlib>

using namespace std;

class Matrix {
private:
  static int nAlloc;
  static int nFree;
  int dy;
  int dx;
  int **array;
  void alloc(int cy, int cx);
public:
  static int get_nAlloc();
  static int get_nFree();
  int get_dy();
  int get_dx();
  int** get_array();
  Matrix();
  Matrix(int cy, int cx);
  Matrix(const Matrix *obj);
  Matrix(const Matrix &obj);
  Matrix(int *arr, int col, int row);
  ~Matrix();    // 소멸자
  Matrix *clip(int top, int left, int bottom, int right);
  void paste(const Matrix *obj, int top, int left);
  Matrix *add(const Matrix *obj);
  // 실습: 31~35
  // private멤버에 접근권한을 부여하기 위해 friend를 사용함
    // friend is only needed to give the function access to the class's private and protected members.
  // Otherwise if that is not needed then friend is not needed either.
  friend const Matrix operator+ (const Matrix& m1, const Matrix& m2);   // friend
  // Matrix operator + (const Matrix& m2);    // member

  int sum();
  void mulc(int coef);
  Matrix *int2bool();
  bool anyGreaterThan(int val);
  void print();
  friend ostream& operator<<(ostream& out, const Matrix& obj);
  Matrix& operator=(const Matrix& obj);
  friend const Matrix operator+ (const Matrix& m1, const Matrix& m2);
  // Matrix operator + (const Matrix& m2);
};
