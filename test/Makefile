CPPFLAGS := -I.. -I.
CXXFLAGS := -g

.PHONY: all
all: testCalculator

.PHONY: clean
clean:
	rm *.o testCalculator

testCalculator: calculator.o test.o
	$(CXX) $(CXXFLAGS) -o $@ $^

calculator.o: ../calculator.h ../calculator.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ ../calculator.cpp

test.o: ../calculator.h test.cpp
