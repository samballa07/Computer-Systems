#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Test replace_in_line()";
   int paragraph_number, line_number = 0;
   

   if (init_document(&doc, doc_name) == FAILURE) {
      printf("Initialization failed\n");
   } 
   /* Adding first paragraph and line */
   paragraph_number = 0;
   add_paragraph_after(&doc, paragraph_number);
   add_line_after(&doc, 1, line_number, "hello my name is seth sethssethseth");
   add_line_after(&doc, 1, line_number+1, "this is my test for replace");

   print_document(&doc);

   replace_text(&doc, "seth", "nelson");
   replace_text(&doc, "test", "check");

   print_document(&doc);

   return 0;
}