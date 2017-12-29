// Author: Sean Davis
#include <iostream>
#include "cpu.h"
#include "mynew.h"

using namespace std;

CPU::CPU()
{
  table_size = 3700;
  table1 = new cache[table_size];
  table2 = new cache[table_size];
  current_status = 0;
} // CPU()
void CPU::insert(unsigned &addr, int value)
{
  int i = addr%table_size-1;
  table1[i] = table2[i];
  table2[i].address = addr;
  table2[i].value = value;
}
bool CPU::check(unsigned &addr)
{
//  cout<<"check"<<endl;
  int i = addr%table_size-1;
  if (table2[i].address != addr)
  {
    if (table1[i].address != addr)
    {
      return false;
    }
    else
    {
      temp = &table1[i];
      table1[i] = table2[i];
      table2[i] = *temp;
      return true;
    }
  }
  else
  {
    return true;
  }
}
void CPU::add(int &buffer, unsigned a1, unsigned a2)
{
  if ((a1%table_size) == (a2%table_size))
  {
    int j = a1%table_size-1;
    buffer = table1[j].value + table2[j].value;
  }
  else
  {
    int i = a1%table_size-1;
    int j = a2%table_size-1;
    buffer = table2[i].value + table2[j].value;
  }
}
void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer)
{
//  cout<<current_status<<"   curent-status"<<endl;
  switch (opCode)
  {
    case ADD  :
    {
  //    cout<<"add"<<endl;
      if (current_status == 0)
      {
//        cout<<"a1   "<<a1<<endl;
        a1 = instruction.addr1;
        a2 = instruction.addr2;
        a3 = instruction.addr3;
        current_status = 1;
        if (check(a1) == false)
        {
          opCode = LOAD;
          return;
        }
      }
      if (current_status == 1)
      {
        current_status = 2;
        if (check(a2) == false)
        {
  //        cout<<"a2   "<<a2<<endl;
          opCode = LOAD;
          instruction.addr1 = instruction.addr2;
          return;
        }
      }
      if (current_status == 2)
      {
        add(buffer, a1, a2);
        if (check(a3) == false)
        {
    //      cout<<"a3   "<<a3<<endl;
          opCode = STORE;
          insert(a3, buffer);
          instruction.addr1 = instruction.addr3;
          return;
        }
        else
        {
          int i = a3%table_size-1;
          if (table2[i].value == buffer)
          {
            opCode = ADD;
            current_status = 0;
            return;
          }
          else
          {
            table2[i].value = buffer;
            instruction.addr1 = instruction.addr3;
            opCode = STORE;
            return;
          }
        }
      }
    }// *addr3 = *addr1 + *addr2
    case LOAD :
    {
    //  cout<<"load"<<endl;
      insert(instruction.addr1, buffer);
      opCode = ADD;
      operation(opCode, instruction, buffer);
      return;
    }  // buffer contains int requested from RAM
    case STORE:
    {
  //    cout<<"store"<<endl;
      opCode = ADD;
      current_status = 0;
      return;
    } // Sent by RAM after a STORE
    case DONE :
    {
    //  cout<<"done"<<endl;
      return;
    }//  All ADDs in file have been sent.  Time to STORE dirty cache.
  } // switch
} // operation()
