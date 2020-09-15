#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Exercise Description";
   int paragraph_number, line_number = 0;

   if (init_document(&doc, doc_name) == FAILURE) {
      printf("Initialization failed\n");
   } 

   /* Adding first paragraph */
   paragraph_number = 0;
   add_paragraph_after(&doc, paragraph_number);

   add_line_after(&doc, 1, line_number, "First Paragraph, First line");
   add_line_after(&doc, 1, line_number + 1, "First Paragraph, Second line");
   add_line_after(&doc, 1, line_number + 2, "First Paragraph, Third line");
   print_document(&doc);

   /* tests remove line */
   remove_line(&doc, 1, 1);
   print_document(&doc);

   append_line(&doc, 1, "Appended Line");
   print_document(&doc);

   append_line(&doc, 1, "This is a newline");
   remove_line(&doc, 1, 4);
   print_document(&doc);


   return 0;
}
