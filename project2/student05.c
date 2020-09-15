#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Student Loading Document Test";
   int data_lines = 6;
   char data[20][MAX_STR_SIZE + 1] = {
        "My name is seth",
	"I am taking cmsc 216",
	"I took cmsc132 last spring",
	"",
	"I am working on a project",
	"This project uses C."
   };

   init_document(&doc, doc_name);
   load_document(&doc, data, data_lines);
   print_document(&doc);
   
   load_document(&doc, data, 2);
   print_document(&doc);

   init_document(&doc, "Resetted Doc");
   print_document(&doc);


   return 0;
}