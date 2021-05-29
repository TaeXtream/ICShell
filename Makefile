icsh: icsh.o processHandler.o signalProcess.o
	g++ -o icsh icsh.o processHandler.o signalProcess.o

icsh.o: icsh.cpp
	g++ -c icsh.cpp

processHandler.o: processHandler.cpp
	g++ -c processHandler.cpp

signalProcess.o: signalProcess.cpp
	g++ -c signalProcess.cpp

clean:
	rm *.o icsh