CPU.out: cpu.o CPURunner.o mynew.o
	g++ -Wall -ansi -g -o CPU.out cpu.o CPURunner.o mynew.o 

cpu.o: cpu.cpp cpu.h 
	g++ -Wall -ansi -g -c cpu.cpp

CPURunner.o: CPURunner.cpp CPUTimer.h cpu.h CPURunner.h mynew.h 
	g++ -Wall -ansi -g -c CPURunner.cpp

mynew.o: mynew.cpp mynew.h 
	g++ -Wall -ansi -g -c mynew.cpp

clean:
	rm -f CPU.out cpu.o CPURunner.o mynew.o 
