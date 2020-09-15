/******************/
/*  Seth Amballa  */
/* UID: 115884939 */
/*    samballa    */
/******************/

#include <stdio.h>
#include <string.h>
#include "document.h"

/* prototypes */
void replace_in_line(const char *target, const char *replacement,
                     char* line);
static int contains_only_whitepaces(const char *line);


/*************************************************************************/
/* Initializes the document by setting the name and number of paragraphs */
/*                              to zero                                  */
/*************************************************************************/
int init_document(Document *doc, const char *name){
   if (doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE){
      return FAILURE;
   }
   strcpy(doc->name, name);
   doc->number_of_paragraphs = 0;
   return SUCCESS;
}

/*************************************************************************/
/* Resets the document by getting rid of all paragraphs and lines        */
/*     is done by setting the number of paragraphs to 0                  */
/*************************************************************************/
int reset_document(Document *doc){
   if(doc == NULL){
      return FAILURE;
   }
   doc->number_of_paragraphs = 0;
   return SUCCESS;
}

/*******************************************************************/
/* displays the contents of the doc starting with the doc name and */
/* number of paragraphs. Alls the lines are printed with a spaces  */
/*                 indicating a new paragraph                      */
/*******************************************************************/
int print_document(Document *doc){
   int i, j;

   if(doc == NULL){
      return FAILURE;
   }

   printf("Document name: \"%s\"\n", doc->name);
   printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

   for(i = 0; i < doc->number_of_paragraphs; i++){
      for(j = 0; j < doc->paragraphs[i].number_of_lines; j++){
         /* prints the contents of each line of each paragraph */
         printf("%s\n", doc->paragraphs[i].lines[j]);
        
      }
      /* seperates each paragraph with an empty line */
      if(i < doc->number_of_paragraphs - 1){
         printf("\n");

      }
   }
   return SUCCESS;
}

/*************************************************************************/
/* adds a new paragraph to the paragraphs array after the specified      */
/* number. Will shift array based on the position of the added paragraph */
/*************************************************************************/
int add_paragraph_after(Document *doc, int paragraph_number){
   int i;
   Paragraph *par_arr; /* stores the Paragraph array in document struct */

   if(doc == NULL || doc->number_of_paragraphs >= MAX_PARAGRAPHS || 
      paragraph_number > doc->number_of_paragraphs){
         return FAILURE;
   }
   
   par_arr = doc->paragraphs;

   /* adds a paragraph to the front of the array when its empty */
   if(paragraph_number == 0 && doc->number_of_paragraphs == 0){
      par_arr[paragraph_number].number_of_lines = 0;

   /* adds element to the end of array */
   }else if(paragraph_number == doc->number_of_paragraphs){
      par_arr[paragraph_number].number_of_lines = 0;

   /* moves all the elements after the inserted paragraph to the right */
   }else{ 
      for(i = doc->number_of_paragraphs - 1; i >= paragraph_number; i--){
         par_arr[i + 1] = par_arr[i];
      }
      par_arr[paragraph_number].number_of_lines = 0;
   }
   doc->number_of_paragraphs++;
   return SUCCESS;
}

/*************************************************************************/
/* adds a line to the specified paragraph number and line number         */
/* will shift elements in the array to the right based on added position */
/*************************************************************************/
int add_line_after(Document *doc, int paragraph_number, int line_number,
		            const char *new_line){
   int i;
   Paragraph *this_par;

   if(doc == NULL || paragraph_number > doc->number_of_paragraphs || 
      doc->paragraphs[paragraph_number - 1].number_of_lines >= 
      MAX_PARAGRAPH_LINES || line_number > 
      doc->paragraphs[paragraph_number - 1].number_of_lines ||
      new_line == NULL){

      return FAILURE;
   }

   /* stores address of Paragraph struct at that index */
   this_par = &doc->paragraphs[paragraph_number - 1];

   /* adds line at the front of the array when paragraph is empty */
   if(line_number == 0 && this_par->number_of_lines == 0){
      strcpy(this_par->lines[line_number], new_line);

   /* adds line to the end of the array */
   }else if(line_number == this_par->number_of_lines){
      strcpy(this_par->lines[line_number], new_line);

   /* shifts elements over accordingly and adds line in the middle */
   } else{

      for(i = this_par->number_of_lines - 1; i >= line_number; i--){
         strcpy(this_par->lines[i + 1], this_par->lines[i]);
      }
      strcpy(this_par->lines[line_number], new_line);
   }
   this_par->number_of_lines++;
   return SUCCESS;
}

/*****************************************************************/
/* returns the number of lines in the specified paragraph number */
/*****************************************************************/
int get_number_lines_paragraph(Document *doc, int paragraph_number, 
                                 int *number_of_lines){

   if (doc == NULL || number_of_lines == NULL || 
      paragraph_number > doc->number_of_paragraphs){
         return FAILURE;
   }
   *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
   return SUCCESS;

}

/*****************************************************************/
/* appends the line to the end of the specified paragraph number */
/*****************************************************************/
int append_line(Document *doc, int paragraph_number, const char *new_line){
   int paragraph_lines;
   Paragraph *this_par; /* holds a pointer to a Paragraph struct */

   if(doc == NULL || paragraph_number > doc->number_of_paragraphs ||
      doc->paragraphs[paragraph_number - 1].number_of_lines >= 
      MAX_PARAGRAPH_LINES || new_line == NULL){
      
      return FAILURE;
   }
   paragraph_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
   this_par = &doc->paragraphs[paragraph_number - 1];

   strcpy(this_par->lines[paragraph_lines], new_line);
   this_par->number_of_lines++;
   return SUCCESS;
}

/***********************************************************************/
/*    removes the line at the specified paragraph and line number.     */
/* will shift the elements in the lines array to the right accordingly */
/***********************************************************************/
int remove_line(Document *doc, int paragraph_number, int line_number){
   Paragraph *this_par;
   int i, last_index;

   if(doc == NULL || paragraph_number > doc->number_of_paragraphs ||
      line_number > doc->paragraphs[paragraph_number - 1].number_of_lines ||
      line_number == 0){

      return FAILURE;
   }
   
   this_par = &doc->paragraphs[paragraph_number - 1];
   
   /* removes the last line in the paragraph */
   if(line_number == this_par->number_of_lines){
      strcpy(this_par->lines[line_number - 1], "");

   /* shifts the elements to the right of the line_number 
      this will get rid of the intended line */
   }else{
      last_index = this_par->number_of_lines;
      for(i = line_number - 1; i < last_index - 1; i++){

         strcpy(this_par->lines[i], this_par->lines[i + 1]);
      }
   }
   this_par->number_of_lines--;
   return SUCCESS;
   
}

/***********************************************************************/
/* Takes in an array of strings and adds the lines indicated by line   */
/* number to the document. Will add a new paragraph if empty string    */
/***********************************************************************/
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], 
                  int data_lines){
   int i = 0, paragraph_number;

   if(doc == NULL || data == NULL || data_lines == 0){
      return FAILURE;
   }    
   paragraph_number = doc->number_of_paragraphs;

   /* adds a paragraph if document is empty */
   if(doc->number_of_paragraphs == 0){
      add_paragraph_after(doc, 0);
      paragraph_number++;
   }

   /* adds line at the end of the document or creates a new paragraph if
      there is an empty string */
   while(i < data_lines){
      if(strcmp(data[i], "") == 0){
         paragraph_number++;
         add_paragraph_after(doc, doc->number_of_paragraphs);
      } else{
         append_line(doc, paragraph_number, data[i]);
      }
      i++;
   }
   return SUCCESS;
}

/***********************************************************************/
/* Finds target string everywhere in the documen and replaces it with  */
/* the replacement string. Will return FAILURE if any param is null    */
/***********************************************************************/
int replace_text(Document *doc, const char *target, const char *replacement){
   Paragraph *this_par;
   int i, j;

   if(doc == NULL || target == NULL || replacement == NULL){
      return FAILURE;
   }

   /* runs through every line in the paragraph */
   for(i = 0; i < doc->number_of_paragraphs; i++){
      this_par = &doc->paragraphs[i];

      for(j = 0; j < this_par->number_of_lines; j++){

         /* calls the helper function */
         replace_in_line(target, replacement, this_par->lines[j]);
      }
   }

   return SUCCESS;
}

/***********************************************************************/
/* Helper function for replace_text. Will replace target everywhere in */
/* the line from parameter with the replacement string                 */
/***********************************************************************/
void replace_in_line(const char *target, const char *replacement, char* line){
   int target_len = strlen(target), replace_len = strlen(replacement);
   int i, start_index, end_index, diff, add_index = 0, j; 
   char temp[MAX_STR_SIZE + 1];
   char temp_line[MAX_STR_SIZE + 1];
 
   for(i = 0; i < (int)strlen(line); i++){

      /* finds the substring starting at i with length of target */
      strncpy(temp, line + i, target_len);
      temp[target_len] = '\0';

      /* checks that the string is the same as target */
      if(strcmp(temp, target) == 0){
         start_index = i;
         /* will replace every instance of target with the correct
            string when length of target is greater than replacement */
         if(target_len > replace_len){
            end_index = i + target_len;
            diff = target_len - replace_len;

            for(j = end_index; j < (int)strlen(line); j++){
               line[j - diff] = line[j];
            }
            line[j - diff] = '\0';            
            for(j = start_index; j < start_index + replace_len; j++){
               line[j] = replacement[add_index++];
            }

         /* will replace every instance of target with the correct
            string when length of target is less than replacement */
         }else if(target_len < replace_len){
            strcpy(temp_line, line);
            end_index = i + (replace_len - 1);
            diff = replace_len - target_len;

            for(j = (int)strlen(line) - 1; j >= start_index + target_len; j--){
               temp_line[j + diff] = temp_line[j];
            }
            temp_line[(int)strlen(line)+ diff] = '\0';
            for(j = start_index; j <= start_index + (replace_len - 1); j++){
               temp_line[j] = replacement[add_index++];
            }
            strcpy(line, temp_line);
            i += replace_len - 1;


         }else{
            /* replaces them when they are the same length */
            for(j = start_index; j < start_index + replace_len; j++){
               line[j] = replacement[add_index++];
            }
         }
         add_index = 0;
      }
   }
}

/***********************************************************************/
/* Will surround the target text everywhere in the doc with brackets   */
/***********************************************************************/
int highlight_text(Document *doc, const char *target){
   char new_target[MAX_STR_SIZE + 1] = "";
   int target_len;

   if(doc == NULL || target == NULL){
      return FAILURE;
   }
   target_len = strlen(target);

   /* the updated replacement string is created with conacatenation */
   strcat(new_target, HIGHLIGHT_START_STR);
   strcat(new_target, target);
   strcat(new_target, HIGHLIGHT_END_STR);
   new_target[target_len + 2] = '\0';

   /* calls replace_text on the old word and sends the the updated 
      word which now has brackets around it so that every instance of the old
      word is now replaced with brackets around it */
   replace_text(doc, target, new_target);
   return SUCCESS;
}

/***********************************************************************/
/*      Will remove the target string everywhere in the document       */
/***********************************************************************/
int remove_text(Document *doc, const char *target){
   if(doc == NULL || target == NULL){
      return FAILURE;
   }

   /* calls replace text and replaces target with an empty string */
   replace_text(doc, target, "");
   return SUCCESS;
}

int load_file(Document *doc, const char *filename){
   FILE *input; 
   char line[MAX_STR_SIZE];
   int curr_para = 1;

   if (doc == NULL || filename == NULL){
      return FAILURE;
   }

   input = fopen(filename, "r");
   if(input == NULL){
      return FAILURE;
   }

   add_paragraph_after(doc, 0);
   
   while(fgets(line, MAX_STR_SIZE + 1, input) != NULL){
      
      /* terminates the newline character at the end */
      if(line[strlen(line) - 1] == '\n'){
         line[strlen(line) - 1] = '\0';
      }

      if(contains_only_whitepaces(line) == 1){
         add_paragraph_after(doc, curr_para);
         curr_para++;

      } else{            
         append_line(doc, curr_para, line);
      }
   }
   fclose(input);
   return SUCCESS;
}

int save_document(Document *doc, const char *filename){
   FILE *output;
   Paragraph this_par;
   int i, j;
   if (doc == NULL || filename == NULL){
      return FAILURE;
   }

   output = fopen(filename, "w");
   if(output == NULL){
      return FAILURE;
   }

   for(i = 0; i < doc->number_of_paragraphs; i++){
      this_par = doc->paragraphs[i];
      for(j = 0; j < this_par.number_of_lines; j++){

         fputs(this_par.lines[j], output);
         fputs("\n", output);
      }
      if(i < doc->number_of_paragraphs - 1){
        fputs("\n", output);
      }
   }

   fclose(output);
   return SUCCESS; 

}

/* helper function for load_file */
static int contains_only_whitepaces(const char *line){
   int i;

   for(i = 0; i < (int)strlen(line); i++){
      if(line[i] != ' '){
         return 0;
      }
   }
   return 1;
}