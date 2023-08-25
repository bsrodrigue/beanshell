#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
#define BUFSIZE 1024

#include <stdio.h>
#include <stdlib.h>

// char **split_line(char *line) {
//   int bufsize = TOK_BUFSIZE, position = 0;
//   char **tokens = malloc(bufsize * sizeof(char *));
//   char *token;
// }

char *read_line(void) {
  int position = 0;
  int bufsize = BUFSIZE;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "beanshell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();

    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    }

    else {
      buffer[position] = c;
    }

    position++;

    if (position >= bufsize) {
      bufsize += BUFSIZE;
      buffer = realloc(buffer, bufsize);

      if (!buffer) {
        fprintf(stderr, "beanshell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

void loop(void) {
  char *line;
  char **args;
  int status;

  do {
    printf(">_");
    line = read_line();
    // args = split_line(line);
    // status = execute(args);

    printf("%s", line);
    free(line);
    free(args);

  } while (status);
}

int main(int argc, char *argv[]) {

  loop();

  return EXIT_SUCCESS;
}
