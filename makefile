CXX		  := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wformat
CXXRELEASE := -O2 -s
CXXDEBUG := -g -Og 

EXE = game.exe

INCLUDES	:= -Iinclude -Iinclude/imgui

SOURCES = include/imgui/examples/imgui_impl_glfw.cpp 
SOURCES += include/imgui/examples/imgui_impl_opengl3.cpp 
SOURCES += include/imgui/misc/freetype/imgui_freetype.cpp 
SOURCES += include/imgui/imgui.cpp
SOURCES += include/imgui/imgui_widgets.cpp
SOURCES += include/imgui/imgui_draw.cpp
SOURCES += src/main.cpp

DEFS = -DSUBPIXEL_FONT_RENDERING -DIMGUI_IMPL_OPENGL_LOADER_GLEW
LIBS	:= -lglew32 -lglfw3 -lopengl32 -lglu32 -lfreetype
LDFLAGS = -s

CPPFLAGS = $(CXXFLAGS) $(CXXRELEASE) $(INCLUDES) $(DEFS)

#OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))

OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

all: $(EXE)
	@echo Build complete for $(EXE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)

#$(OBJS):$(SOURCES)
%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	@echo "🧹 Clearing..."
	-rm $(TARGET_EXEC) $(OBJS)