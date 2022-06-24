all: fault_injector.so

fault_injector.so: fault_injector.cc
	g++ -c -fpic $< -o fault_injector.o
	g++ -ldl -lrt -shared -o $@ fault_injector.o

clean:
	rm -f fault_injector.so fault_injector.o
