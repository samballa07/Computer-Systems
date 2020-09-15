#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

/* Description here: This test checks ...  */
static int test1() {
   int days = 7;
   Calendar *calendar;

   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

/* testing add_event for all cases */
static int test2() {
   Calendar *calendar;

   if(init_calendar("Seth", 4, comp_minutes, NULL, &calendar) == SUCCESS){
      if(add_event(calendar, "1_event", 1200, 30, NULL, 1) == SUCCESS){
         if(add_event(calendar, "2_event", 1000, 40, NULL, 1) == SUCCESS){
            if(add_event(calendar, "3_event", 1100, 10, NULL, 1) == SUCCESS){
               if(add_event(calendar, "3_event", 1100, 70, NULL, 1) == FAILURE){
                  
               }
            }
         }
      }
   }

   
   if(add_event(calendar, "4_event", 1200, 30, NULL, 3) == SUCCESS){
      if(add_event(calendar, "5_event", 1000, 40, NULL, 4) == SUCCESS){
         if(add_event(calendar, "6_event", 1100, 10, NULL, 2) == SUCCESS){
           if(add_event(calendar, "7_event", 1100, 70, NULL, 3) == SUCCESS){

               return destroy_calendar(calendar);

            }
         }
      }
   }
   return FAILURE;
}

/* testing find event and find event in day */
static int test3(){
   Calendar *calendar;
   Event *event;
   if(init_calendar("Seth", 4, comp_minutes, NULL, &calendar) == SUCCESS){
      if(add_event(calendar, "1_event", 1200, 30, NULL, 1) == SUCCESS){
         if(add_event(calendar, "2_event", 1000, 40, NULL, 1) == SUCCESS){
            if(add_event(calendar, "3_event", 1100, 10, NULL, 1) == SUCCESS){
               
            }
         }
      }
   }
   if(find_event(calendar, "2_event", &event) == SUCCESS){
      if(strcmp(event->name, "2_event") == 0 && 
         find_event(calendar, "4_event", &event) == FAILURE){
      
         if(find_event_in_day(calendar, "2_event", 2, &event) == FAILURE){
            if(find_event_in_day(calendar, "2_event", 1, NULL) == SUCCESS){

               return destroy_calendar(calendar);
            }
         }
      }
   }
   return FAILURE;
}

/* testing remove_event */
static int test4(){
   Calendar *calendar;

   if(init_calendar("My calendar", 4, comp_minutes, NULL, &calendar) == SUCCESS){
      if(add_event(calendar, "1_event", 1200, 30, NULL, 1) == SUCCESS){
         if(add_event(calendar, "2_event", 1000, 40, NULL, 1) == SUCCESS){
            if(add_event(calendar, "3_event", 1100, 10, NULL, 1) == SUCCESS){
            }
         }
      }
   }

   if(remove_event(calendar, "2_event") == SUCCESS){
      add_event(calendar, "4_event", 2000, 5, NULL, 1);
      if(remove_event(calendar, "4_event") == SUCCESS){
         if(remove_event(calendar, "7_event") == FAILURE){

            return destroy_calendar(calendar);
         }
      }
   }
   return FAILURE;
}

/* tests clear day and clear calendar */
static int test5(){
   Calendar *calendar;

   if(init_calendar("My calendar", 4, comp_minutes, NULL, &calendar) == SUCCESS){
      add_event(calendar, "1_event", 1200, 40, NULL, 1);
      add_event(calendar, "2_event", 1300, 10, NULL, 1);
      add_event(calendar, "3_event", 1400, 20, NULL, 1);
      add_event(calendar, "4_event", 1500, 50, NULL, 1);
      add_event(calendar, "5_event", 1200, 40, NULL, 2);
      add_event(calendar, "6_event", 1300, 10, NULL, 3);
      add_event(calendar, "7_event", 1400, 20, NULL, 4);
      add_event(calendar, "8_event", 1500, 50, NULL, 4);
   }

   if(clear_day(calendar, 1) == SUCCESS){
      if(clear_calendar(calendar) == SUCCESS){
         return destroy_calendar(calendar);
;
      }
   }

   return FAILURE;
}

/* tests destroy calendar */
static int test6(){
   Calendar *calendar;

   if(init_calendar("My calendar", 4, comp_minutes, NULL, &calendar) == SUCCESS){
      add_event(calendar, "1_event", 1200, 40, NULL, 1);
      add_event(calendar, "2_event", 1300, 10, NULL, 1);
      add_event(calendar, "3_event", 1400, 20, NULL, 1);
      add_event(calendar, "4_event", 1500, 50, NULL, 1);
      add_event(calendar, "5_event", 1200, 40, NULL, 2);
      add_event(calendar, "6_event", 1300, 10, NULL, 3);
      add_event(calendar, "7_event", 1400, 20, NULL, 4);
      add_event(calendar, "8_event", 1500, 50, NULL, 4);
      print_calendar(calendar, stdout, 1);
   }

   return destroy_calendar(calendar);

   return FAILURE;
}

int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   if (test1() == FAILURE) result = FAILURE;
   if (test2() == FAILURE) result = FAILURE;
   if (test3() == FAILURE) result = FAILURE;
   if (test4() == FAILURE) result = FAILURE;
   if (test5() == FAILURE) result = FAILURE;
   if (test6() == FAILURE) result = FAILURE;

   
   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/
   
   if (result == FAILURE) {
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
