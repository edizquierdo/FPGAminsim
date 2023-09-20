main: main.o FPGA.o TSearch.o random.o
	g++ -pthread -o main main.o FPGA.o TSearch.o random.o
random.o: random.cpp random.h VectorMatrix.h
	g++ -pthread -c -O3 random.cpp
FPGA.o: FPGA.cpp random.h FPGA.h
	g++ -pthread -c -O3 FPGA.cpp
TSearch.o: TSearch.cpp TSearch.h
	g++ -pthread -c -O3 TSearch.cpp
main.o: main.cpp FPGA.h TSearch.h
	g++ -pthread -c -O3 main.cpp
clean:
	rm *.o main
