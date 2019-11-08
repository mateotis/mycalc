mycalc: main.o preprocessor.o evaluator.o
	g++ main.o preprocessor.o evaluator.o -o mycalc
evaluator.o: evaluator.cpp evaluator.h
	g++ -c evaluator.cpp
preprocessor.o: preprocessor.cpp preprocessor.h
	g++ -c preprocessor.cpp
main.o: main.cpp
	g++ -c main.cpp
clean:
	rm *.o mycalc