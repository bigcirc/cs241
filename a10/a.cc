#include <iostream>
using namespace std;
int f(int a) { return a+4; }
int g(int a) { return a+9; }

int bar() {
  int * arr = NULL;
  arr = new int[5];
  *(arr+3)=2;
  delete[] arr;
  return 10; 
}

int foo(int a, int * b, int c, int d, int * e) {
  if (f(a) < g(c)) {
    cout << "0" << endl;
  } else {}

  if (f(a) > g(c)) {
    cout << "1" << endl;
  } else {}

  if (f(a) == g(c)) {
    cout << "2" << endl;
  } else {}

  if (f(a) != g(c)) {
    cout << "3" << endl;
  } else {}

  if (f(a) <= g(c)) {
    cout << "4" << endl;
  } else {}

  if (f(a) >= g(c)) {
    cout << "5" << endl;
  } else {}

  *(b+bar()) = bar();
  return a + *(b+10) + c + d + *e;
}

int main(){
  int a = 5;
  int b = 6;
  int * arr = NULL;
  arr = new int[40];
  cout <<  "fpoo"<<foo(a, arr, a, b, &a) << endl;
  return foo(a, arr, a, b, &a);
}
