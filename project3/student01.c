#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Test load file/save doc";
   int paragraph_number;

   if (init_document(&doc, doc_name) == FAILURE) {
      printf("Initialization failed\n");
   } 

   paragraph_number = 0;
   add_paragraph_after(&doc, paragraph_number);
   append_line(&doc, 1, "the original line in the doc");
   print_document(&doc);

   load_file(&doc, "doc1.txt");
   print_document(&doc);

   save_document(&doc, "studentdoc2.txt");

   return 0;
}
