# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Libraries to link against
LIBS = -lcurl

# Target executable
TARGET = url_extractor

# Source files
SRC = url_extractor.cpp

# Object files (generated from the source files)
OBJ = $(SRC:.cpp=.o)

# Default rule to build the target executable
all: $(TARGET)

# Rule to build the target executable from object files
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

# Rule to build the object files from the source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up the build files
clean:
	rm -f $(OBJ) $(TARGET)

# Rebuild the project from scratch
rebuild: clean all
