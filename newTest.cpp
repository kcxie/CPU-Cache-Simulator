#include <iostream>
using namespace std;

typedef struct
{
  unsigned address;
  int value;
  bool dirty;
  bool stuff;
  bool stuff2;
} CacheLine;

int main()
{
  for(int i = 1; i < 20; i++)
  {
    int *ptr = new int[i];
    cout << i << ' ' << ptr[-1] << endl;
  }

  cout << "CacheLine size with sizeof: " << sizeof(CacheLine) << endl;
  CacheLine *cptr = new CacheLine;
  cout << "CacheLine size with new: " << ((int*) cptr)[-1] << endl; 

  return 0;
}
