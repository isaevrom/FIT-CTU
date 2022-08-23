LD=g++
LDFLAGS=-std=c++17 -Wall -pedantic -fsanitize=address -g
CXX=g++
CXXFLAGS=-std=c++17 -Wall -pedantic -fsanitize=address -g

all: compile doc

compile: isaevrom

isaevrom: src/main.o src/CMatrixCalculator.o src/CDatabase.o src/CMatrixHolder.o src/IMatrix.o src/CDenseMatrix.o src/CSparseMatrix.o src/CIdentity.o src/CAddition.o src/CInverse.o src/CMerge.o src/CScan.o src/CSubstraction.o src/CRank.o src/CGEM.o src/CCut.o src/CBinaryOperation.o src/CUnaryOperation.o src/CDeterminant.o src/CTranspose.o src/COperation.o src/CMultiplication.o
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: compile
	./isaevrom

doc: src/main.o src/CMatrixCalculator.o src/CDatabase.o src/CMatrixHolder.o src/IMatrix.o src/CDenseMatrix.o src/CSparseMatrix.o src/CIdentity.o src/CAddition.o src/CInverse.o src/CMerge.o src/CScan.o src/CSubstraction.o src/CRank.o src/CGEM.o src/CCut.o src/CBinaryOperation.o src/CUnaryOperation.o src/CDeterminant.o src/CTranspose.o src/COperation.o src/CMultiplication.o
	doxygen ./Doxyfile

clean:
	rm -rf src/*.o isaevrom doc/* Makefile.d

#automatically generated using: g++ -MM *cpp
deps:
	$(CXX) -MM src/*cpp > Makefile.d

-include Makefile.d
