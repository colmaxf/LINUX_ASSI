# compiler C++
CXX = g++

# flag common
CXXFLAGS = -Wall -Wextra -std=c++11

# library
STATIC_LIB = libstrutils.a
SHARED_LIB = libstrutils.so

# .o files
OBJ_STRUTILS = strutils.o
OBJ_MAIN = main.o

# executable files
EXEC_STATIC = main_static
EXEC_SHARED = main_shared

#  build both versions
.PHONY: all
all: $(EXEC_STATIC) $(EXEC_SHARED)

# Obuild static library and static test program
.PHONY: static
static: $(EXEC_STATIC)

# build shared library and shared test program
.PHONY: shared
shared: $(EXEC_SHARED)

# creating strutils.o object files
$(OBJ_STRUTILS): strutils.cpp strutils.h
	$(CXX) $(CXXFLAGS) -c strutils.cpp -o $(OBJ_STRUTILS)

# enerating strutils.o object file with -fPIC flag for shared library
strutils_shared.o: strutils.cpp strutils.h
	$(CXX) $(CXXFLAGS) -fPIC -c strutils.cpp -o strutils_shared.o

# creating main.o object file
$(OBJ_MAIN): main.cpp strutils.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o $(OBJ_MAIN)

# creating static libraries (.a)
$(STATIC_LIB): $(OBJ_STRUTILS)
	ar rcs $(STATIC_LIB) $(OBJ_STRUTILS)

# creating shared libraries (.so)
$(SHARED_LIB): strutils_shared.o
	$(CXX) $(CXXFLAGS) -shared strutils_shared.o -o $(SHARED_LIB)

# creating executables that link with static libraries
$(EXEC_STATIC): $(OBJ_MAIN) $(STATIC_LIB)
	$(CXX) $(CXXFLAGS) $(OBJ_MAIN) -L. -lstrutils -o $(EXEC_STATIC)

# creating executables that link with shared libraries
$(EXEC_SHARED): $(OBJ_MAIN) $(SHARED_LIB)
	$(CXX) $(CXXFLAGS) $(OBJ_MAIN) -L. -lstrutils -o $(EXEC_SHARED)

# Delete all files created during the build process
.PHONY: clean
clean:
	rm -f $(OBJ_STRUTILS) strutils_shared.o $(OBJ_MAIN) $(STATIC_LIB) $(SHARED_LIB) $(EXEC_STATIC) $(EXEC_SHARED)