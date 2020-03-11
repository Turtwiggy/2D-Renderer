CXX		  := g++
CXX_FLAGS := -std=c++17 -Wall -Wextra 
CXX_RELEASE := -O2 -s
CXX_DEBUG := -g -Og 

TARGET_EXEC = game.exe
BUILD_DIR ?= ./build
SRC_DIRS = src

# SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
# OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
# DEPS := $(OBJS:.o=.d)

INCLUDES	:= -Iinclude -Iinclude/imgui

IMGUI = include/imgui/examples/imgui_impl_glfw.cpp 
IMGUI += include/imgui/examples/imgui_impl_opengl3.cpp 
IMGUI += include/imgui/misc/freetype/imgui_freetype.cpp 
IMGUI += include/imgui/imgui.cpp
IMGUI += include/imgui/imgui_widgets.cpp
IMGUI += include/imgui/imgui_draw.cpp

DEFS = -DSUBPIXEL_FONT_RENDERING -DIMGUI_IMPL_OPENGL_LOADER_GLEW
LIBS	:= -lglew32 -lglfw3 -lopengl32 -lglu32 -lfreetype

all: $(BUILD_DIR)/$(TARGET_EXEC)

run: clean all
	clear
	@echo "🚀 Executing..."
	./$(BUILD_DIR)/$(TARGET_EXEC)

# apparentl https://stackoverflow.com/questions/4036191/sources-from-subdirectories-in-makefile/4038459
# but i couldnt get it to work

$(BUILD_DIR)/$(TARGET_EXEC): $(SRC_DIRS)/*.cpp $(SRC_DIRS)/**/*.cpp
	@echo "🚧 Building..."
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $(IMGUI) $(DEFS) $(LIBS) $^ -o $@

clean:
	@echo "🧹 Clearing..."
	-rm $(TARGET_EXEC)/*