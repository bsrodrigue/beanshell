#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
#define BUFSIZE 1024

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*
  Function Declarations for builtin shell commands:
 */
int beanshell_cd(char **args);
int beanshell_help(char **args);
int beanshell_exit(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &beanshell_cd,
  &beanshell_help,
  &beanshell_exit
};

int beanshell_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/
int beanshell_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "beanshell: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("beanshell");
    }
  }
  return 1;
}

int beanshell_help(char **args)
{
  int i;
  printf("Badini Rodrigue's beanshell\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < beanshell_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int beanshell_exit(char **args)
{
  return 0;
}

int launch(char **args){
  pid_t pid,wpid;
  int status;

  // Reality splits in this dimension, this is awesome
  pid = fork();

  // New process
  if (pid == 0){
    if (execvp(args[0], args) == -1) {
      perror("beanshell");
    }
    exit(EXIT_FAILURE);
  }

  else if (pid < 0){
    // Error forking
    perror("beanshell");
  }

  else {
      // We want to keep the parent alive while the child is busy.
  do {
        wpid = waitpid(pid, &status, WUNTRACED);
      } while(!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

char **split_line(char *line){
  int bufsize = TOK_BUFSIZE;
  int position = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token;

  if(!tokens){
    fprintf(stderr, "beanshell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line,TOK_DELIM);

  while (token != NULL) {
    tokens[position] = token;
    position++;


    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE; 
      tokens = realloc(tokens,  bufsize * sizeof(char *));

      if (!tokens){
        fprintf(stderr, "beanshell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOK_DELIM);
  }

  tokens[position] = NULL;
  return tokens;
}

char *read_line(void) {
  char *line = NULL;

  size_t bufsize = 0;

  size_t count = getline(&line, &bufsize, stdin);

  if(count != -1) return line;

  if(feof(stdin)) {
    exit(EXIT_SUCCESS);
  }

  else {
    perror("readline");
    exit(EXIT_FAILURE);
  }
}


int execute(char **args){
  int i;

  if(args[0] == NULL){
    return 1;
  }

  for(i = 0 ; i < beanshell_num_builtins(); i++){
    if(strcmp(args[0], builtin_str[i]) == 0){
        return (*builtin_func[i])(args);
    }
  }

  return launch(args);
}

void loop(void) {
  char *line = NULL;
  char **args = NULL;
  int status;
  char *cwd = malloc(sizeof(char) * 1024);

  do {
    getcwd(cwd, sizeof(char) * 1024);
    printf("%s >_ ", cwd);
    line = read_line();
    args = split_line(line);
    status = execute(args);

    free(line);
    free(args);
    free(cwd);
  } while (status);
}


int main(int argc, char *argv[]) {
  loop();
  return EXIT_SUCCESS;
}
