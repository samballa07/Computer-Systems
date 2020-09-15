/******************/
/*  Seth Amballa  */
/* UID: 115884939 */
/*    samballa    */
/******************/

#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <stdlib.h>
#include "document.h"
#define VALID_LINE_LEN 1024

/* function prototypes */
static int execute_command(Document *doc, const char *command, const char *line);
static int run_add_paragraph(Document *doc, const char *line);
static int run_print(Document *doc, const char * line);
static int run_exit_or_quit(const char *line);
static int run_add_line(Document *doc, const char *line);
static int run_append(Document *doc, const char *line);
static int run_load_file(Document *doc, const char *line);
static int run_remove_line(Document *doc, const char *line);
static int run_replace(Document *doc, const char *line);
static int run_highlight_or_remove(Document *doc, const char * line);
static int run_save(Document *doc, const char *line);
static int run_reset(Document *doc, const char *line);

/*************************************************************************/
/*   Parses the line string to get the correct values. Calls the         */
/*   reset_document function and displays fail message if necassary.     */
/*   Will return false if anything is invalid                            */
/*************************************************************************/
static int run_reset(Document *doc, const char *line){
   char command[VALID_LINE_LEN + 1], invalid[VALID_LINE_LEN + 1];
   int values;

   values = sscanf(line, " %s%s", command, invalid);

   if(values == 1){
      if(reset_document(doc) == FAILURE){
         printf("reset_document failed\n");
      }
      return SUCCESS;
   }
   return FAILURE;
}

/*************************************************************************/
/*   Parses the line string to get the correct values. Calls the         */
/*   save_document function and displays fail message if necassary. Will */
/*   save doc to the filename specified in command                       */
/*************************************************************************/
static int run_save(Document *doc, const char *line){
   char command[VALID_LINE_LEN + 1], invalid[VALID_LINE_LEN + 1];
   char filename[MAX_STR_SIZE + 1];
   int values;

   values = sscanf(line, " %s%s%s", command, filename, invalid);

   if(values == 2){
      if(save_document(doc, filename) == FAILURE){
         printf("save_document failed\n");
      }
      return SUCCESS;
   }
   return FAILURE;
}

/*************************************************************************/
/*   Parses the line string to get the correct values. Calls the         */
/*   highlight_text or remove_text depending on which command is called  */
/*   Will return false if anything is invalid                            */
/*************************************************************************/
static int run_highlight_or_remove(Document *doc, const char * line){
   char command[1024], target[81];
   int i, quote_found = 0, target_add = 0;

   sscanf(line, " %s", command);

   for(i = strlen(command) + 1; i < (int)strlen(line); i++){
      if(line[i] == '"' && quote_found == 1){
         quote_found = -1;
         continue;

      }else if(line[i] == '"'){
         if(quote_found != -1){
            quote_found = 1;
         }
         continue;
      }  
      if (quote_found == 1){
         target[target_add++] = line[i];
      } 
   }
   target[target_add] = '\0';

   if(quote_found == -1){
      if(strcmp(command, "highlight_text") == 0){
         highlight_text(doc, target);

      }else if(strcmp(command, "remove_text") == 0){
         remove_text(doc, target);

      }
      return SUCCESS;
   }
   return FAILURE;
}

/*************************************************************************/
/*   Parses the line string to get the correct values. Calls the         */
/*   replace_text function and displays fail message if necassary. Will  */
/*   return false if anything is invalid                                 */
/*************************************************************************/
static int run_replace(Document *doc, const char *line){
   char command[1024], target[81], replace[81];
   int i, quote_found = 0, quote_2_found = 0, target_add = 0, replace_add = 0;

   sscanf(line, " %s", command);

   /* parses string to find the strings within the quotes */
   for(i = strlen(command) + 1; i < (int)strlen(line); i++){
      if(line[i] == '"' && quote_found == 1){
         quote_found = -1;
         i++;
         continue;

      }else if(line[i] == '"' && quote_2_found == 1){
         quote_2_found = -1;
         continue;

      }else if(line[i] == '"'){
         if(quote_found != -1){
            quote_found = 1;
         }else if(quote_2_found != -1){
            quote_2_found = 1;
         }
         continue;
      }  
      if (quote_found == 1){
         target[target_add++] = line[i];

      } else if(quote_2_found == 1){
         replace[replace_add++] = line[i];
      }
   }
   target[target_add] = '\0';
   replace[replace_add] = '\0';

   if(quote_found == -1 && quote_2_found == -1){
      if(replace_text(doc, target, replace) == FAILURE){
         printf("replace_text failed\n");
      }
      return SUCCESS;
   }
   return FAILURE;
}

/*************************************************************************/
/*   Parses the line string to get the correct values. Calls the         */
/*   remove_line function and displays fail message if necassary. Will   */
/*   return false if anything is invalid                                 */
/*************************************************************************/
static int run_remove_line(Document *doc, const char *line){
   char command[VALID_LINE_LEN + 1], invalid[VALID_LINE_LEN + 1];
   int values, para_num, line_num;

   values = sscanf(line, " %s%d%d%s", command, &para_num, &line_num, invalid);

   if (values == 3){
      if(para_num >= 0 && line_num >= 0){
         if(remove_line(doc, para_num, line_num) == FAILURE){
            printf("remove_line failed\n");
         }
         return SUCCESS;
      }
   }
   return FAILURE;
}

/*************************************************************************/
/*   Parses the line string to get the correct values. Calls the         */
/*   load_file function and displays fail message if necassary. Will     */
/*   return false if anything is invalid                                 */
/*************************************************************************/
static int run_load_file(Document *doc, const char *line){
   char command[VALID_LINE_LEN + 1], filename[MAX_STR_SIZE + 1];
   char invalid[VALID_LINE_LEN + 1];
   int values;

   values = sscanf(line, " %s%s%s", command, filename, invalid);

   if (values == 2){
      if(load_file(doc, filename) == FAILURE){
         printf("load_file failed\n");
      }
      return SUCCESS;
   }
   return FAILURE;
}

/*************************************************************************/
/*   Parses the line string to get the correct values. Calls the         */
/*   append function and displays fail message if necassary. Will        */
/*   return false if anything is invalid                                 */
/*************************************************************************/
static int run_append(Document *doc, const char *line){
   char command[VALID_LINE_LEN + 1], add_line[MAX_STR_SIZE];
   char line_or_star[MAX_STR_SIZE + 1];
   int values, para_num, star_index, i, add_index = 0;

   values = sscanf(line, " %s%d%s", command, &para_num, line_or_star);

   for(i = 0; i < (int)strlen(line); i++){
      if (line[i] == '*'){
         star_index = i;
         break;
      }
   }
   if(values == 3 && line_or_star[0] == '*'){
      if(para_num >= 0){

         for(i = star_index + 1; i < (int)strlen(line); i++){
            add_line[add_index++] = line[i];
         }
         add_line[add_index] = '\0';

         if(append_line(doc, para_num, add_line) == FAILURE){
            printf("append_line failed\n");
         }
         return SUCCESS;
      }
   }
   return FAILURE;
}

/*************************************************************************/
/*   Parses the line string to check that the values are valid and       */
/*   if they are, its calls add_line_after with those values. Function   */
/*   will display fail message if the call fails and                     */
/*************************************************************************/
static int run_add_line(Document *doc, const char *line){
   char command[VALID_LINE_LEN + 1], add_line[MAX_STR_SIZE];
   char line_or_star[MAX_STR_SIZE + 1];
   int values, para_num, line_num, star_index, i, add_index = 0;
   char star;

   values = sscanf(line, " %s%d%d%s", command, &para_num, &line_num, 
                  line_or_star);
   
   for(i = 0; i < (int)strlen(line); i++){
      if (line[i] == '*'){
         star_index = i;
         break;
      }
   }
   if(values == 4){
      star = line_or_star[0];
      if(para_num >= 0 && line_num >= 0 && star == '*'){

         for(i = star_index + 1; i < (int)strlen(line); i++){
            add_line[add_index++] = line[i];
         }
         add_line[add_index] = '\0';

         if(add_line_after(doc, para_num, line_num, add_line) == FAILURE){
            printf("add_line_after failed\n");
         }
         return SUCCESS;
      }
   }
   return FAILURE;
}

/*************************************************************************/
/*   Parses the line string for the quit or exit command and             */
/*   determines if it was entered correctly. Returns 0 if so             */
/*************************************************************************/
static int run_exit_or_quit(const char *line){
   char command[VALID_LINE_LEN + 1], invalid_data[VALID_LINE_LEN + 1];
   int values;

   values = sscanf(line, " %s %s", command, invalid_data);
   if(values == 1){
      return SUCCESS;
   }
   return FAILURE;
}

/*************************************************************************/
/*   parses line string for the correct values. returns -1 if any field  */
/*   is missing or incorrect and alls print_document depending on        */
/*   whether the parsed values are valid                                 */
/*************************************************************************/
static int run_print(Document *doc, const char * line){
   char command[VALID_LINE_LEN + 1], invalid_data[VALID_LINE_LEN + 1];
   int values;

   values = sscanf(line, " %s%s", command, invalid_data);
   if(values == 1){
      print_document(doc);
      return SUCCESS;
   }
   return FAILURE;
}

/*************************************************************************/
/* takes in the line and parses it for the correct values. Calls         */
/*   add_paragraph after and displays fail message if failed. Will       */
/*   return 0 and -1 depending on whether the values found are valid.     */
/*************************************************************************/
static int run_add_paragraph(Document *doc, const char *line){
   char command[VALID_LINE_LEN + 1], invalid_data[VALID_LINE_LEN + 1];
   int paragraph_num;

   if(sscanf(line, " %s %d%s", command, &paragraph_num, invalid_data) == 2){
      if(paragraph_num >= 0){
         if(add_paragraph_after(doc, paragraph_num) == FAILURE){

           printf("%s failed\n", command);
         }
      return SUCCESS;
      }
   }
   return FAILURE;
}

/*************************************************************************/
/* checks that command entered corresponds to a valid command and calls  */
/*   the correct function which will run it. Returns -1 when program     */
/*   is meant to be terminated                                           */
/*************************************************************************/
static int execute_command(Document *doc, const char *command, const char *line){
   int command_check = FAILURE, already_ran = 0;

   if(strcmp(command, "add_paragraph_after") == 0){
      command_check = run_add_paragraph(doc, line);

   }else if(strcmp(command, "print_document") == 0){
      command_check = run_print(doc, line);

   }else if(strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0){
      command_check = run_exit_or_quit(line);
      if(command_check == SUCCESS){
         return -1;
      }
      
   }else if(strcmp(command, "add_line_after") == 0){
      command_check = run_add_line(doc, line);

   }else if(strcmp(command, "append_line") == 0){
      command_check = run_append(doc, line);

   }else if (strcmp(command, "load_file") == 0){
      command_check = run_load_file(doc, line);

   }else if(strcmp(command, "remove_line") == 0){
      command_check = run_remove_line(doc, line);

   }else if(strcmp(command, "replace_text") == 0){
      command_check = run_replace(doc, line);

   }else if(strcmp(command, "highlight_text") == 0 ||
            strcmp(command, "remove_text") == 0){
      command_check = run_highlight_or_remove(doc, line);

   }else if(strcmp(command, "save_document") == 0){
      command_check = run_save(doc, line);

   }else if(strcmp(command, "reset_document") == 0){
      command_check = run_reset(doc, line);

   }else{
      printf("Invalid Command\n");
      already_ran = 1;
   }

   if (command_check == FAILURE && already_ran != 1){
      printf("Invalid Command\n");
   }
   return 0;
}

int main(int argc, char *argv[]){
   FILE *my_input;
   Document doc;
   char line[VALID_LINE_LEN + 1];
   char command[VALID_LINE_LEN + 1];
   int values;

   /* checks whether the arguments passed in are valid and assigns
      input according to whether a file was passed in or not */
   if(argc == 1){ 
      my_input = stdin;

   }else if (argc == 2){
      my_input = fopen(argv[1], "r");
      if(my_input == NULL){
         fprintf(stderr, "%s cannot be opened\n", argv[1]);
         exit(EX_OSERR);
 
      }
   }else{
      fprintf(stderr, "Usage: user_interface\n");
      fprintf(stderr, "Usage: user_interface <filename>\n");
      exit(EX_USAGE);     
   }
   init_document(&doc, "main_document");

   /* user prompt signal */
   if(my_input == stdin){
      printf("> ");
   }

   /* loops until the end of file */
   while(fgets(line, VALID_LINE_LEN + 1, my_input) != NULL){
      if(line[strlen(line) - 1] == '\n'){
         line[strlen(line) - 1] = '\0';
      }
      values = sscanf(line, " %s", command);
       
      /* value is EOF when is it a blank line */
      if(command[0] != '#' && values != EOF){
         
         /* -1 is returned if quit or exit was executed successfully */
         if(execute_command(&doc, command, line) == -1){
            fclose(my_input);
            exit(EXIT_SUCCESS);
         }
      }
      if(my_input == stdin){
         printf("> ");
      } 
   }
   printf("\n");
   return 0;
}
