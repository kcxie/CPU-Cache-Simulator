// Author: Sean Davis
#ifndef cpuH
#define cpuH

#include "CPURunner.h"

class cache
{
public:
  unsigned address;
  int value;
  bool dirty;
  cache()
  {
    address = 0;
    value = 0;
    dirty = false;
  }
};

class CPU
{
public:
  cache *table;
  int table_size;
  int current_status;
  unsigned a1,a2,a3;
  int v1,v2,v3,mark;
  bool flag;
  CPU();
  void operation(OpCode &opCode, Instruction &instruction, int &buffer);
  void add(int &buffer, int v1, int v2);
  bool find(unsigned &addr);
  void insert(unsigned addr, int value);
};
#endif
