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

unitconv.o: ../unitconv.h ../unitconv.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ ../unitconv.cpp

testUnitConv: unitconv.o testUnits.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test.o: ../calculator.h test.cpp

testUnits.o: ../unitconv.h testUnits.cpp
