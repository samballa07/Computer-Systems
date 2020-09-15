#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Test highlight and remove";
   int paragraph_number, line_number = 0;
   

   if (init_document(&doc, doc_name) == FAILURE) {
      printf("Initialization failed\n");
   } 
   /* Adding first paragraph and line */
   paragraph_number = 0;
   add_paragraph_after(&doc, paragraph_number);
   add_line_after(&doc, 1, line_number, "hellohello my name is seth hello");
   add_line_after(&doc, 1, line_number+1, "this is my test for replace");

   print_document(&doc);
   /* testing highlight */
   highlight_text(&doc, "hello");
   highlight_text(&doc, "test");
   print_document(&doc);

   /* testing remove */
   remove_text(&doc, "name");
   remove_text(&doc, "this");
   remove_text(&doc, "epl");
   print_document(&doc);

   return 0;
}