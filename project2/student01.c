#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Exercise Description";
   int paragraph_number;

   if (init_document(&doc, doc_name) == FAILURE) {
      printf("Initialization failed\n");
   } 

   /* Adding first paragraph */
   paragraph_number = 0;
   add_paragraph_after(&doc, paragraph_number);
   print_document(&doc);

   add_paragraph_after(&doc, paragraph_number+1);
   print_document(&doc);

   add_paragraph_after(&doc, paragraph_number+1);
   print_document(&doc);

   return 0;
}
