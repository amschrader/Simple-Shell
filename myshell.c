/*
 * myshell.c
 *
 *  Created on: Apr 17, 2009
 *      Author: Allyson Schrader
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>


extern char **getline();
void execute_cmd(char** arglist, int pipe_in[2] , int background , int pipe_in_flag);
int check_background(char** arglist);


int main() {
	char **args;

	while(1){
		printf("$ ");
		args = getline();

		if(strcmp(args[0] , "exit") == 0){
			exit(0);
		}
		execute_cmd(args , NULL , check_background(args) , 0);
	}
}
int check_background(char** arglist){
	int i;
	for(i = 0 ; arglist[i] != NULL ; i++){
		if(strcmp(arglist[i] , "&") == 0){
			return 1;
		}
	}
	return 0;
}


void execute_cmd(char** arglist, int pipe_in[2] ,  int background , int pipe_in_flag){
	int i=0;
	int r_fd, w_fd;
	pid_t pid;
	int status;
	char* read_file;
	char* write_file;
	int write_flag =0, read_flag = 0;
	int arg_placeholder;
	int pipe_out_flag =0;
	int pipe_out[2];
	char** pipe_args;
/*
 * Stage One: parse through the args, if < or > is found
 * set file names and flags, if | is found, set flags, copy argslist
 * and end parse
 */
	for(i=0; arglist[i] != NULL;i++){
		if(strcmp(arglist[i] ,"<") == 0 ){
			arglist[i]=NULL;
			read_flag = 1;
			read_file = arglist[i+1];
		}else if(strcmp(arglist[i] ,">") == 0){
			arglist[i]=NULL;
			write_flag = 1;
			write_file = arglist[i+1];
		}else if(strcmp(arglist[i] , "|") == 0){
			arglist[i] = NULL;
			pipe_out_flag = 1;
			arg_placeholder = i+1;
			if(pipe(pipe_out) < 0){
				perror("failedo to open pipe");
				exit(errno);
			}
			break;
		}
	}
/*
 * Stage Two:  fork,  check for flags, dup files/pipes
 * where necessary, execute
 */
	pid = fork();
	if (pid < 0) {
	  fprintf(stderr, "Fork failed!\n");
	  exit(0);
	}
	else if (pid==0) {
	  if(read_flag){
		r_fd = open(read_file, O_RDONLY);
		if (r_fd < 0) {
		  printf("could not open file for read %s\n" , arglist[i]);
		  exit(1);
		}
		if (dup2(r_fd, 0) != 0) {
		  perror("could not dup2 for read\n");
		  exit(1);
		}
		close(r_fd);
	  }
	if(write_flag){
		w_fd = open(write_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (w_fd < 0) {
			perror("could not open file for write\n");
			exit(2);
		}
		if (dup2(w_fd, 1) != 1) {
			perror("could not dup2 for write\n");
			exit(1);
		}
		close(w_fd);
	}
	if(pipe_out_flag || pipe_in_flag){

		if(pipe_in_flag){
			if (dup2(pipe_in[0], 0) != 0) {
			  perror("could not dup2 for read\n");
			  exit(1);
			}
		}
		if(pipe_out_flag){
			if (dup2(pipe_out[1], 1) != 1) {
				perror("could not dup2 for write\n");
				exit(1);
			}
		}
		close(pipe_out[0]);
		close(pipe_in[1]);
	}
	  execvp(arglist[0] , arglist);

	}
/*
 * Stage Three: check pipe flags, then recursively call
 * execute_cmd on the array following the | OR wait for
 * process to finish.
 */

	else {

		if(pipe_out_flag){
			close(pipe_out[1]);
			if(!background){
				waitpid(pid, &status, 0);
			}
			execute_cmd(&arglist[arg_placeholder] , pipe_out ,  background , pipe_out_flag);


		}
		else{
			if(!background){
				waitpid(pid, &status, 0);
			}
		}
	}
	close(pid);


}
