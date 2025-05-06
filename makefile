all: ./a.out

compRun:
	g++ -std=c++11 *.cpp -o r.out

run: clean compRun; ./r.out

clean:
	rm -f *.out
