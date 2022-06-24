all: rcopy

OBJECTS=rcopy.o Buffer.o Exception.o File.o

rcopy: $(OBJECTS)
	g++ $(OBJECTS) -o rcopy

%.o: %.cc *.h
	g++ $(CXXFLAGS) -c $<
	
clean:
	rm -f rcopy *.o
	