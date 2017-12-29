// Author: Sean Davis
#include <iostream>
#include "cpu.h"
#include "mynew.h"

using namespace std;

CPU::CPU()
{
  table_size = 4499;
  table = new cache[table_size];
  current_status = 8;
  mark = 0;
} // CPU()
void CPU::insert(unsigned addr, int value)
{
  int i = addr%table_size;
  table[i].address = addr;
  table[i].value = value;
  return;
}
bool CPU::find(unsigned &addr)
{
  int i = addr%table_size;
  if (table[i].address != addr)
  {
    if (table[i].dirty == false)
    {
      flag = false;
    }
    else
    {
      flag = true;
    }
    return false;
  }
  else
  {
    return true;
  }
}
void CPU::add(int &buffer, int v1, int v2)
{
  buffer = v1 + v2;
}
void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer)
{
  switch (opCode)
  {
    case ADD  :
    {
      if (current_status == 8)
      {
        a1 = instruction.addr1;
        a2 = instruction.addr2;
        a3 = instruction.addr3;
        current_status = 0;
      }
      if (current_status == 0)
      {
        int i = a1%table_size;


        current_status = 1;
        if (find(a1) == false)
        {
          if (flag == false)
          {
            instruction.addr1 = a1;
            opCode = LOAD;
            return;
          }
          else
          {
            opCode = STORE;
            instruction.addr1 = table[i].address;
            buffer = table[i].value;
            table[i].dirty = false;
            current_status--;
            return;
          }
        }
      }
      if (current_status == 1)
      {
        int i = a2%table_size;
        v1 = table[a1%table_size].value;
        current_status = 2;

        if (find(a2) == false)
        {
          if (flag == false)
          {
            opCode = LOAD;
            instruction.addr1 = a2;
            return;
          }
          else
          {
            opCode = STORE;
            instruction.addr1 = table[i].address;
            buffer = table[i].value;
            table[i].dirty = false;
            current_status--;
            return;
          }
        }
      }
      if (current_status == 2)
      {
        v2 = table[a2%table_size].value;
        current_status = 3;
        if (find(a3) == false)
        {
          if (flag == false)
          {
            opCode = LOAD;
            instruction.addr1 = a3;
            return;
          }
          else
          {
            opCode = STORE;
            instruction.addr1 = table[a3%table_size].address;
            buffer = table[a3%table_size].value;
            table[a3%table_size].dirty = false;
            current_status--;
            return;
          }
        }
      }
        if (current_status == 3)
        {
          int i = a3%table_size;
          add(buffer, v1, v2);
          table[i].value = buffer;
          table[i].dirty = true;
          opCode = ADD;
          current_status = 8;
          return;
        }
      if (current_status == 4)
      {
        int i;
        for(i = mark; i<table_size; i++)
        {
          if (table[i].dirty == true)
          {
            mark = i;
            table[i].dirty = false;
            break;
          }
          if (i == table_size-1)
          {
            opCode = DONE;
            current_status = 0;
            delete [] table;
            return;
          }
        }
        opCode = STORE;
        instruction.addr1 = table[mark].address;
        buffer = table[mark].value;
        return;
      }
    }// *addr3 = *addr1 + *addr2
    case LOAD :
    {
      insert(instruction.addr1, buffer);
      opCode = ADD;
      operation(opCode, instruction, buffer);
      return;
    }  // buffer contains int requested from RAM
    case STORE:
    {
      opCode = ADD;
      operation(opCode, instruction, buffer);
      return;
    } // Sent by RAM after a STORE
    case DONE :
    {
      current_status = 4;
      opCode = ADD;
      operation(opCode, instruction, buffer);

      return;
    }//  All ADDs in file have been sent.  Time to STORE dirty cache.
  } // switch
} // operation()
