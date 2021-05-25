icsh: icsh.o exRunner.o signalProcess.o
	g++ -o icsh icsh.o exRunner.o signalProcess.o

icsh.o: icsh.cpp
	g++ -c icsh.cpp

exRunner.o: exRunner.cpp
	g++ -c exRunner.cpp

signalProcess.o: signalProcess.cpp
	g++ -c signalProcess.cpp

clean:
	rm *.o icsh