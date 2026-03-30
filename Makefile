CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2
LIBS    = -lraylib -lm

# Platform detection
ifeq ($(OS),Windows_NT)
    TARGET  = RadoGames.exe
    LIBS   += -lopengl32 -lgdi32 -lwinmm
    RM      = del /Q
else
    TARGET  = RadoGames
    UNAME   := $(shell uname -s)
    ifeq ($(UNAME),Darwin)
        LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    else
        LIBS += -lGL -lpthread -ldl -lrt -lX11
    endif
    RM      = rm -f
endif

# Only compile menu.c — it #includes snake.c, TTT.c, minesweeper.c
SRC = menu.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC) snake.c TTT.c minesweeper.c
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET)
