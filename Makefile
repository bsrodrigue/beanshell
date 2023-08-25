CC = gcc
CFLAGS = -Wall -Wextra
EXECUTABLE = beanshell

all: $(EXECUTABLE)

$(EXECUTABLE): main.c
	$(CC) $(CFLAGS) -o $@ $<

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -r $(EXECUTABLE)
