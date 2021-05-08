icsh: icsh.o
	g++ icsh.o -o icsh

icsh.o: icsh.cpp
	g++ -c icsh.cpp

clean:
	rm *.o icsh