
/* 115884939 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <fcntl.h>
#include <err.h>
#include "executor.h"
#include "command.h"
#define EXECUTE_EXIT -1

/*static void print_tree(struct tree *t);*/
int execute_aux(struct tree *t, int in_fd, int out_fd);
int exec_none(struct tree *t, int in_fd, int out_fd);
int exec_pipe(struct tree *t, int in_fd, int out_fd);
void exec_and(struct tree *t, int in_pd, int out_pd);
void exec_subshell(struct tree *t, int in_fd, int out_fd);

 
int execute(struct tree *t) {
   
   if(t){
      return execute_aux(t, STDIN_FILENO, STDOUT_FILENO);
   }
   return 0;
}

int execute_aux(struct tree *t, int in_fd, int out_fd){
   int sig = 0;

   if (t->conjunction == NONE){
      if((sig = exec_none(t, in_fd, out_fd)) == EXECUTE_EXIT){
         exit(EXIT_SUCCESS);
      }
   }else if(t->conjunction == PIPE){
      sig = exec_pipe(t, in_fd, out_fd);

   }else if(t->conjunction == SUBSHELL){
      exec_subshell(t, in_fd, out_fd);

   }else if(t->conjunction == AND){
      exec_and(t, in_fd, out_fd);
   }
   return sig;
}

int exec_none(struct tree *t, int in_fd, int out_fd){
   pid_t pid;
   int status;

   if(strcmp(t->argv[0], "cd") == 0){
      if(chdir(t->argv[1]) < 0){
         perror(t->argv[1]);
      }

   }else if(strcmp(t->argv[0], "exit") == 0){
      return EXECUTE_EXIT;

   }else{
      if((pid = fork()) < 0){
         perror("fork error\n");
      }
      if(pid){ /*parent process */
         wait(&status);
         return status;

      } else if(pid == 0){

         if(t->input != NULL){
            if((in_fd = open(t->input, O_RDONLY)) < 0){
               perror("input file open failed\n");
               exit(EX_OSERR);
            }
            if(dup2(in_fd, STDIN_FILENO) < 0){
               perror("dup2 input failed\n");
               exit(EX_OSERR);
            }
            if(close(in_fd) < 0){
               perror("closing file failed\n");
               exit(EX_OSERR);
            }
         }
         if(t->output != NULL){
            if((out_fd = open(t->output, O_WRONLY | O_CREAT | O_TRUNC, 0664)) 
               < 0){
               perror("output file open failed\n");
               exit(EX_OSERR);
            }
            if(dup2(out_fd, STDOUT_FILENO) < 0){
               perror("dup2 output failed\n");
               exit(EX_OSERR);
            }
            if(close(out_fd) < 0){
               perror("closing file failed\n");
               exit(EX_OSERR);
            }
         }
         if(execvp(t->argv[0], t->argv) < 0){
            fprintf(stderr, "Failed to execute %s\n", t->argv[0]);
            fflush(stdout);
            exit(EX_OSERR);
         }
      }
   }
   return 0;
}

void exec_and(struct tree *t, int in_fd, int out_fd){

   if(t->input != NULL){
      if((in_fd = open(t->input, O_RDONLY)) < 0){
         perror("input file open failed\n");
         exit(EX_OSERR);
      }
   }
   if(t->output != NULL){
      if((out_fd = open(t->output, O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0){
         perror("output file open failed\n");
         exit(EX_OSERR);
      }
   }
   
   if(execute_aux(t->left, in_fd, out_fd) == 0){
      execute_aux(t->right, in_fd, out_fd);
   }
}

int exec_pipe(struct tree *t, int in_fd, int out_fd){
   int pipe_fd[2], status = 0;
   pid_t pid, pid2;

   if(t->left->output != NULL){
      fprintf(stdout, "Ambiguous output redirect.\n");
   } else if(t->right->input != NULL){
      fprintf(stdout, "Ambiguous input redirect.\n");

   }else{

      if(t->input != NULL){
         if((in_fd = open(t->input, O_RDONLY)) < 0){
            perror("input file open failed for pipe\n");
            exit(EX_OSERR);
         }
      }
      if(t->output != NULL){
         if((out_fd = open(t->output, O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0){
            perror("output file open failed\n");
            exit(EX_OSERR);
         }
      }
 
      if(pipe(pipe_fd) < 0){
         perror("pipe fail\n");
         exit(EX_OSERR);
      }
      if((pid = fork()) < 0){
         perror("fork error\n");
         exit(EX_OSERR);

      }
      if(pid){ /* parent code */ 
	 
	         if(close(pipe_fd[1]) < 0){/* dont need write end for right child */ 
               perror("closing file failed\n");
               exit(EX_OSERR);
            } 
            
            dup2(pipe_fd[0], in_fd);
      
	         if(execute_aux(t->right, pipe_fd[0], out_fd) != 0){
               close(pipe_fd[0]);
            }
            
	         wait(&status); /* reaping child process */ 
            if(WEXITSTATUS(status) < 0){
               return -1;
            }
	 
	      }else{ 
	         if(close(pipe_fd[0]) < 0){
               perror("closing file failed\n");
               exit(EX_OSERR);
            }

            dup2(out_fd, pipe_fd[1]);

	         if(execute_aux(t->left, in_fd, pipe_fd[1])){
               close(pipe_fd[1]);
               exit(EX_OSERR);
            }else{
               close(pipe_fd[1]);
               exit(EXIT_SUCCESS);
            }
         } 
	   } 
   return 0;
}


void exec_subshell(struct tree *t, int in_fd, int out_fd){
   pid_t pid;

   if(t->input != NULL){
      if((in_fd = open(t->input, O_RDONLY)) < 0){
         perror("output file open fail\n");
         exit(EX_OSERR);
      }
   }
   if(t->output != NULL){
      if((out_fd = open(t->output, O_RDONLY)) < 0){
         perror("input file open fail\n");
         exit(EX_OSERR);
      }
   }

   if((pid = fork()) < 0){
      perror("fork error\n");
   }
   if(pid){
      wait(NULL);
   }else{
      execute_aux(t->left, in_fd, out_fd);
      exit(EXIT_SUCCESS);
   }
}

/*
static void print_tree(struct tree *t) {
   if (t != NULL) {
      print_tree(t->left);

      if (t->conjunction == NONE) {
         printf("NONE: %s, ", t->argv[0]);
      } else {
         printf("%s, ", conj[t->conjunction]);
      }
      printf("IR: %s, ", t->input);
      printf("OR: %s\n", t->output);

      print_tree(t->right);
   }
}
*/