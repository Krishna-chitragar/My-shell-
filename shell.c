#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
 
/* Function Declarations for builtin shell commands: */

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_remove(char **args);
int lsh_date(char **args);
int lsh_fct(char **args);
int lsh_act(char **args);
//int lsh_quit(char **args);

/* List of builtin commands, followed by their corresponding functions. */

char *builtin_str[] = {
 "cd",
 "help",
 "exit",
 "remove",
 "date",
 "fct",
 "act",
// "quit",
};

int (*builtin_func[]) (char **) = {
 &lsh_cd,
 &lsh_help,
 &lsh_exit,
 &lsh_remove,
 &lsh_date,
 &lsh_fct,
 &lsh_act,
// &lsh_quit,
};


int lsh_act(char **args)
{
struct stat st;
struct tm dt;
struct stat stats;
size_t i;
char *gdir , *dir, *to;
char buf[1024];
struct dirent **namelist;
int counter =1;
struct dirent *entry;
DIR *dp;
if(strcmp(args[1],"ls")==0)
{
if(gdir = (char *)malloc(sizeof(buf))!=NULL)
gdir = getcwd(buf,sizeof(buf));
dp = opendir(gdir);
if(dp==NULL)
{
perror("opendir");
return 1;
}
while((entry = readdir(dp)))
{
puts(entry->d_name);
if(stat(entry->d_name, &stats)!=0)
perror(entry->d_name);
else
{
printf("\nFile access: ");
if(stats.st_mode & R_OK)
printf("read");
if(stats.st_mode & W_OK)
printf("write");
if(stats.st_mode & X_OK)
printf("execute");
printf("\nFile size : %ld bytes",stats.st_size);
printf("\nFile name : %s ",entry->d_name);
printf("\n Mode of file %d ", stats.st_mode);
dt = *(gmtime(&stats.st_ctime));
printf("\nCreated on: %02d-%02d-%02d %02d:%02d:%02d",dt.tm_mday, dt.tm_mon + 1,dt.tm_year + 1900, dt.tm_hour+5,dt.tm_min+30,dt.tm_sec);

dt = *(gmtime(&stats.st_atime));
printf("\nModified on : %02d-%02d-%02d %02d:%02d:%02d\n\n",dt.tm_mday,dt.tm_mon + 1,dt.tm_year + 1900, dt.tm_hour+5, dt.tm_min+30, dt.tm_sec);
}
}
closedir(dp);
return 1;
}
else
printf("comand not found");
return 1;
}


int lsh_fct(char **args)
{
struct stat st;
struct tm dt;
struct stat stats;
size_t i;
if(stat(args[1], &stats)!=0)
perror(args[1]);
else{
printf("\nFile access: ");
if(stats.st_mode & R_OK)
printf("read");
if(stats.st_mode & W_OK)
printf("write");
if(stats.st_mode & X_OK)
printf("execute");

printf("\nFile size : %ld bytes",stats.st_size);
printf("\nFile Serial number %ld ",stats.st_ino);
printf("\n Mode of file %d ", stats.st_mode);
dt = *(gmtime(&stats.st_ctime));
printf("\nCreated on: %02d-%02d-%02d %02d:%02d:%02d",dt.tm_mday, dt.tm_mon + 1,dt.tm_year + 1900, dt.tm_hour+5,dt.tm_min+30,dt.tm_sec);

dt = *(gmtime(&stats.st_atime));
printf("\nModified on : %02d-%02d-%02d %02d:%02d:%02d\n",dt.tm_mday,dt.tm_mon + 1,dt.tm_year + 1900, dt.tm_hour+5, dt.tm_min+30, dt.tm_sec);
}
return 1;
}

int lsh_date(char **args)
{
time_t T=time(NULL);
struct tm tm = *localtime(&T);

printf("System Date is : %02d/%02d/%04d\n",tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
printf("System Time is : %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
return 1;
}

int lsh_num_builtins() {
 return sizeof(builtin_str) / sizeof(char *);
}

/* Builtin function implementations.*/
/** @brief Bultin command: change directory. @param args List of args. args[0] is "cd". args[1] is the directory. @return Always returns 1, to continue executing. */
int lsh_remove(char **args)
{
//char *file = &name;
if(args[1] != NULL)
{
if(remove(args[1])==0)
printf("file removed\n");
else
printf("file name not exist\n");
}
}

int lsh_cd(char **args){

 if (args[1] == NULL)
 {
 fprintf(stderr, "lsh: expected argument to \"cd\"\n");
 } else {
 if (chdir(args[1]) != 0) {
 perror("lsh");
 }
 }
 return 1;
}

/** @brief Builtin command: print help.
 @param args List of args. Not examined.
 @return Always returns 1, to continue executing.
 */
int lsh_help(char **args){
 int i;
 printf("Stephen Brennan's LSH\n");
 printf("Type program names and arguments, and hit enter.\n");
 printf("The following are built in:\n");
 for (i = 0; i < lsh_num_builtins(); i++)
 {
 printf(" %s\n", builtin_str[i]);
 }
 printf("Use the man command for information on other programs.\n");
 return 1;
}

/** @brief Builtin command: exit.
 @param args List of args. Not examined.
 @return Always returns 0, to terminate execution.
 */

int lsh_exit(char **args)
{
 printf("\ngoodbye thanks for using our shell\n\n");
 return 0;
}

/*int lsh_quit(char **args)
{
 printf("\ngoodbye thanks for using our shell\n\n");
 return 0;
}*/
/** @brief Launch a program and wait for it to terminate.
 @param args Null terminated list of arguments (including program).
 @return Always returns 1, to continue execution.
 */
int lsh_launch(char **args)
{
 int pid;
 int status;
 pid = fork();
 if (pid == 0) {
 //Child process
 if (execvp(args[0], args) == -1) {
 perror("lsh");
 }
 exit(EXIT_FAILURE);
 } else if (pid < 0) {
 // Error forking
 perror("lsh");
 } else {
 // Parent process
 do {
 waitpid(pid, &status, WUNTRACED);
 } while (!WIFEXITED(status) && !WIFSIGNALED(status));
 }
 return 1;
}
/** @brief Execute shell built-in or launch program.
 @param args Null terminated list of arguments.
 @return 1 if the shell should continue running, 0 if it should terminate 
*/
int lsh_execute(char **args)
{
 int i;

 if (args[0] == NULL) {
 // An empty command was entered.
 return 1;
 }

 for (i = 0; i < lsh_num_builtins(); i++){
 if (strcmp(args[0], builtin_str[i]) == 0) {
 return (*builtin_func[i])(args);
 }
 }
 return lsh_launch(args);
}
/** @brief Read a line of input from stdin. 
@return The line from stdin.
 */
char *lsh_read_line(void)
 {
 char *line;
 char *gdir , *dir, *to;
 char buf[1024];

 if(gdir = (char *)malloc(sizeof(buf))!=NULL)
 gdir = getcwd(buf,sizeof(buf));
 dir = strcat(gdir, "$");
 //to = strcat(dir,args[1]);
 printf("%s",dir);
 line = readline("krishna$ ");
 if(line!=NULL){
 add_history(line);}
return line;
}

/*char *lsh_read_line(void)
{
#ifdef LSH_USE_STD_GETLINE
 char *line = NULL;
 ssize_t bufsize = 0; // have getline allocate a buffer for us
 if (getline(&line, &bufsize, stdin) == -1) {
 if (feof(stdin)) {
 exit(EXIT_SUCCESS); // We received an EOF
 } else {
 perror("lsh: getline\n");
 exit(EXIT_FAILURE);
 }
 }
 return line;
#else
#define LSH_RL_BUFSIZE 1024
 int bufsize = LSH_RL_BUFSIZE;
 int position = 0;
 char *buffer = malloc(sizeof(char) * bufsize);
 int c;
 if (!buffer) {
 fprintf(stderr, "lsh: allocation error\n");
 exit(EXIT_FAILURE);
 }
 while (1) {
 // Read a character
 c = getchar();
 if (c == EOF)
 {
 exit(EXIT_SUCCESS);
 } else if (c == '\n') {
 buffer[position] = '\0';
 return buffer;
 } else {
 buffer[position] = c;
 }
 position++;
 
// If we have exceeded the buffer, reallocate.
 if (position >= bufsize) {
 bufsize += LSH_RL_BUFSIZE;
 buffer = realloc(buffer, bufsize);
 if (!buffer) {
 fprintf(stderr, "lsh: allocation error\n");
 exit(EXIT_FAILURE); 
}
}
}
#endif
}*/

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

/**
 @brief Split a line into tokens (very naively).
 @param line The line.
 @return Null-terminated array of tokens.
 */
char **lsh_split_line(char *line)
{
 int bufsize = LSH_TOK_BUFSIZE, position = 0;
 char **tokens = malloc(bufsize * sizeof(char*));
 char *token;
 char **tokens_backup;
 if (!tokens) {
 fprintf(stderr, "lsh: allocation error\n");
 exit(EXIT_FAILURE);
 }
 token = strtok(line, LSH_TOK_DELIM);
 while (token != NULL){
 tokens[position] = token;
 position++;
 if (position >= bufsize) {
 bufsize += LSH_TOK_BUFSIZE;
 tokens_backup = tokens;
 tokens = realloc(tokens, bufsize * sizeof(char*));
 if (!tokens) {
		free(tokens_backup);
 fprintf(stderr, "lsh: allocation error\n");
 exit(EXIT_FAILURE);
 } 
} 
token = strtok(NULL, LSH_TOK_DELIM);
 } tokens[position] = NULL;
 return tokens;
}
/**
 @brief Loop getting input and executing. 
*/
void lsh_loop(void)
{
 char *line;
 char **args;
 int status;
 
do {
 line = lsh_read_line();
 args = lsh_split_line(line);
 status = lsh_execute(args);
 free(line);
 free(args);
 } while (status);
}


// @brief Main entry point.
// @param argc Argument count.
// @param argv Argument vector.
//@return status code

int main(int argc, char **argv)
{
 // Load config files, if any.
 // Run commandloop.
 lsh_loop();
 // Perform any shutdown/cleanup.
 return EXIT_SUCCESS;
}

