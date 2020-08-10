my_grep: main.o MatchAndPrintScannedLine.o InputParser.o RegularExpressions.o
	gcc -o my_grep main.o MatchAndPrintScannedLine.o InputParser.o RegularExpressions.o

main.o: main.c MatchAndPrintScannedLine.h InputParser.h
	gcc -c main.c

MatchAndPrintScannedLine.o: MatchAndPrintScannedLine.c RegularExpressions.h InputParser.h
	gcc -c MatchAndPrintScannedLine.c

InputParser.o: InputParser.c InputParser.h RegularExpressions.h
	gcc -c InputParser.c

RegularExpressions.o: RegularExpressions.c RegularExpressions.h
	gcc -c RegularExpressions.c

.PHONY: all clean test
clean:
	-rm *.o my_grep

all: my_grep
		
test:
	run_all.sh

