main: main.o FPGA.o BSearch.o random.o
	g++ -pthread -o main main.o FPGA.o BSearch.o random.o
random.o: random.cpp random.h VectorMatrix.h
	g++ -pthread -c -O3 random.cpp
FPGA.o: FPGA.cpp random.h FPGA.h
	g++ -pthread -c -O3 FPGA.cpp
BSearch.o: BSearch.cpp BSearch.h
	g++ -pthread -c -O3 BSearch.cpp
main.o: main.cpp FPGA.h BSearch.h
	g++ -pthread -c -O3 main.cpp
clean:
	rm *.o main
