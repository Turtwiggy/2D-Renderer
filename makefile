CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN		:= build
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:= -lglew32 -lglfw3 -lopengl32 -lglu32 -lassimp
EXECUTABLE	:= main

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	@echo "🚀 Executing..."
	./$(BIN)/$(EXECUTABLE)

#apparentl https://stackoverflow.com/questions/4036191/sources-from-subdirectories-in-makefile/4038459
#but i couldnt get it to work

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/*/*.cpp $(SRC)/*/*/*.cpp
	@echo "🚧 Building..."
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	@echo "🧹 Clearing..."
	-rm $(BIN)/*