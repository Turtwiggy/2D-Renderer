CXX		  := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wformat
CXXRELEASE := -O2 -s
CXXDEBUG := -g -Og 

BUILD_PATH = objs

EXE = game.exe

INCLUDES := -Iinclude -Iinclude/imgui

SOURCES = include/imgui/examples/imgui_impl_glfw.cpp 
SOURCES += include/imgui/examples/imgui_impl_opengl3.cpp 
SOURCES += include/imgui/misc/freetype/imgui_freetype.cpp 
SOURCES += include/imgui/imgui.cpp
SOURCES += include/imgui/imgui_widgets.cpp
SOURCES += include/imgui/imgui_draw.cpp
SOURCES += src/main.cpp
SOURCES += src/sprite_renderer.cpp
SOURCES += src/camera.cpp
SOURCES += include/toolkit/base_serialisables.cpp
SOURCES += include/toolkit/clipboard.cpp
SOURCES += include/toolkit/clock.cpp
SOURCES += include/toolkit/fs_helpers.cpp
SOURCES += include/toolkit/opencl.cpp
SOURCES += include/toolkit/render_window.cpp
SOURCES += include/toolkit/stacktrace.cpp
SOURCES += include/toolkit/texture.cpp
SOURCES += include/networking/beast_compilation_unit.cpp
SOURCES += include/networking/networking.cpp
SOURCES += include/networking/serialisable.cpp
SOURCES += include/ndb/db_storage.cpp

DEFS = -DSUBPIXEL_FONT_RENDERING -DIMGUI_IMPL_OPENGL_LOADER_GLEW -DSFML_STATIC -DGLEW_STATIC -DGRAPHITE2_STATIC -DNO_OPENCL
LIBS	:= -l:libssl.a -l:libglfw3.a -l:libglew32.a -lsfml-audio-s -lsfml-graphics-s -lsfml-system-s -lsfml-window-s -l:libharfbuzz.a -l:libfreetype.a -l:libharfbuzz.a -l:libfreetype.a -l:libgraphite2.a -lopengl32 -l:libflac.a -l:libpng.a -l:libz.a -l:libbz2.a -l:librpcrt4.a -l:libopenal.a -logg -lole32 -ldbgeng -l:libcrypto.a -lbacktrace -lgdi32 -lws2_32 -lopencl -llmdb -lwinmm
LDFLAGS = -static-libstdc++ -static-libgcc -static

CPPFLAGS = $(CXXFLAGS) $(CXXRELEASE) $(INCLUDES) $(DEFS)

#OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))

OBJS := $(addprefix $(BUILD_PATH)/, $(patsubst %.cpp,%.o,$(SOURCES)))

#OBJS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)

DEPS = $(OBJS:.o=.d)

all: dirs $(EXE)
	@echo Build complete for $(EXE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)

-include $(DEPS)

$(BUILD_PATH)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MP -MMD -c -o $@ $<

clean:
	@echo "🧹 Clearing..."
	-rm $(TARGET_EXEC) $(OBJS)
	
.PHONY: dirs
dirs:
	@mkdir -p $(dir $(OBJS))
	@mkdir -p $(BUILD_PATH)
