EXEC = myprogram

CC = clang

CFLAGS = -std=c99 -g -O0 -Wall -lm -lncurses -MMD ${OPT}

SRC = $(wildcard *.c)

OBJECTS = $(SRC:.c=.o)

DEPENDS = $(OBJECTS:.o=.d)

${EXEC}: ${OBJECTS}
		${CC} ${CFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
		rm *.o *.d ${EXEC}