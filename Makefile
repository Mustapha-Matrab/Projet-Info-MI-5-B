CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = cyconnect
SDL_TARGET = cyconnect_sdl
SOURCES = main.c board.c game.c input.c save.c ui.c
OBJECTS = $(SOURCES:.c=.o)
SDL_FRAMEWORK = vendor/SDL2.framework
SDL_LOCAL_FLAGS = -I$(SDL_FRAMEWORK)/Headers -Fvendor -framework SDL2 -Wl,-rpath,@executable_path/vendor

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(SDL_TARGET)

run: $(TARGET)
	./$(TARGET)

sdl:
	@if [ -d "$(SDL_FRAMEWORK)" ]; then \
		$(CC) $(CFLAGS) -DUSE_SDL $(SOURCES) sdl_ui.c -o $(SDL_TARGET) $(SDL_LOCAL_FLAGS); \
	elif command -v sdl2-config >/dev/null 2>&1; then \
		$(CC) $(CFLAGS) -DUSE_SDL $(SOURCES) sdl_ui.c -o $(SDL_TARGET) $$(sdl2-config --cflags --libs); \
	else \
		echo "SDL2 is not installed. Put SDL2.framework in vendor/ or install SDL2 system-wide."; \
		exit 1; \
	fi

run-sdl: sdl
	./$(SDL_TARGET)

.PHONY: all clean run sdl run-sdl
