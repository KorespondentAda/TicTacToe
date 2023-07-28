EXE = TicTacToe

OBJS  = main.o
OBJS += shadertools.o
OBJS += utils.o

CFLAGS = -std=c2x
CFLAGS += -Wall -Wextra
CFLAGS += -g -O0
#CFLAGS += -DDEBUG
LFLAGS = -g -O0

LFLAGS += -lm
LFLAGS += `pkg-config --libs glfw3`
LFLAGS += `pkg-config --libs gl`
LFLAGS += `pkg-config --libs glew`

all: ${EXE}

clean:
	rm ${OBJS} ${EXE}

TicTacToe: ${OBJS}
	gcc ${LFLAGS} ${OBJS} -o TicTacToe

%.o: %.c
	gcc -c ${CFLAGS} $< -o $@
