// Author: Sean Davis
#include <iostream>
#include "cpu.h"
#include "mynew.h"

using namespace std;

CPU::CPU()
{
  table_size = 7000;
  table = new cache[table_size];
  current_status = 0;
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
//  cout<<"check"<<endl;
  int i = addr%table_size;
  if (table[i].address != addr)
  {
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
  //cout<<current_status<<"   curent-status"<<endl;
  switch (opCode)
  {
    case ADD  :
    {
    //  cout<<"add"<<endl;
      if (current_status == 0)
      {
  //      int i = a1%table_size;
  //      cout<<"a1   "<<a1<<endl;
        a1 = instruction.addr1;
        a2 = instruction.addr2;
        a3 = instruction.addr3;

        current_status = 1;
        if (find(a1) == false)
        {
    //      cout<<"don't find a1"<<endl;
            opCode = LOAD;
            return;
        }
    //    cout<<"find a1"<<endl;
      }
      if (current_status == 1)
      {
        v1 = table[a1%table_size].value;
        current_status = 2;

        if (find(a2) == false)
        {
      //    cout<<"don't find a2"<<endl;
            opCode = LOAD;
            instruction.addr1 = instruction.addr2;
            return;
      //   cout<<"a2   "<<a2<<endl;
        }
    //    cout<<"find a2"<<endl;
      }
      if (current_status == 2)
      {
        v2 = table[a2%table_size].value;
        if (find(a3) == false)
        {
        //  cout<<"don't find a3"<<endl;
          opCode = LOAD;
          instruction.addr1 = instruction.addr3;
          return;
        }
        else
        {
          add(buffer, v1, v2);
          opCode = STORE;
          instruction.addr1 = a3;
          table[a3%table_size].value = buffer;
          return;
        }
    //     cout<<"a3   "<<a3<<endl;
      }
    }// *addr3 = *addr1 + *addr2
    case LOAD :
    {
  //    cout<<"load"<<endl;
      insert(instruction.addr1, buffer);
      opCode = ADD;
      operation(opCode, instruction, buffer);
      return;
    }  // buffer contains int requested from RAM
    case STORE:
    {
      //cout<<"store"<<endl;
      opCode = ADD;
      current_status = 0;
      return;
    } // Sent by RAM after a STORE
    case DONE :
    {
  //    cout<<"done"<<endl;
      return;
    }//  All ADDs in file have been sent.  Time to STORE dirty cache.
  } // switch
} // operation()
