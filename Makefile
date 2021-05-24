icsh: icsh.o exRunner.o
	g++ -o icsh icsh.o exRunner.o

icsh.o: icsh.cpp
	g++ -c icsh.cpp

exRunner.o: exRunner.cpp
	g++ -c exRunner.cpp

clean:
	rm *.o icsh