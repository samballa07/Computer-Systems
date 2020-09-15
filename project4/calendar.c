/******************/
/*  Seth Amballa  */
/* UID: 115884939 */
/*    samballa    */
/******************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calendar.h"
#include "event.h"

/************************************************************************/
/*           Initializes the calendar with the data provided            */
/************************************************************************/
int init_calendar(const char *name, int days,
                  int (*comp_func) (const void *ptr1, const void *ptr2),
                  void (*free_info_func) (void *ptr), Calendar ** calendar){
   
   if(calendar == NULL || name == NULL || days < 1){
      return FAILURE;
   }

   /* allocates memory for correct entities */
   *calendar = malloc(sizeof(Calendar));
   (*calendar)->name = malloc(strlen(name) + 1);
   (*calendar)->events = calloc(days, sizeof(Event));
   if(calendar == NULL || (*calendar)->name == NULL || 
      (*calendar)->events == NULL){

      return FAILURE;
   }

   /* assigns calendar information */
   strcpy((*calendar)->name, name);
   (*calendar)->comp_func = comp_func;
   (*calendar)->free_info_func = free_info_func;
   (*calendar)->days = days;
   (*calendar)->total_events = 0;

   return SUCCESS;
}

/************************************************************************/
/*                Prints the contents of the calendar                   */
/************************************************************************/
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all){
   int i;
   Event *curr_day;
   if(calendar == NULL || output_stream == NULL){
      return FAILURE;
   }

   /* displays calendar info when print_all is not 0 */
   if(print_all != 0){
      fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
      fprintf(output_stream, "Days: %d\n", calendar->days);
      fprintf(output_stream, "Total Events: %d\n", calendar->total_events);
      printf("\n");
   }
   printf("**** Events ****\n");

   /* loops through all the days of the calendar */ 
   for(i = 0; i < calendar->days; i++){
      curr_day = calendar->events[i];

      if(calendar->total_events > 0){
         printf("Day %d\n", i + 1);

         /* prints the event info */
         while (curr_day != NULL){
            fprintf(output_stream, "Event's Name: \"%s\", ", curr_day->name);
            fprintf(output_stream, "Start_time: %d, ", curr_day->start_time);
            fprintf(output_stream, "Duration: %d\n", curr_day->duration_minutes);
            curr_day = curr_day->next;
         }
      }
   }
   return SUCCESS;
}

/************************************************************************/
/*   Traverses the calendar and checks that an event with same name     */
/*   does not exist. Creates an event and initializes all the data.     */
/*   Adds that node to the day specified in the correct location.       */
/************************************************************************/
int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day){

   Event *prev = NULL, *curr, *event, *dummy;

   if(calendar == NULL || name == NULL || (start_time > 2400 || start_time < 0)
      || duration_minutes <= 0 || day < 1 || day > calendar->days ||
      find_event(calendar, name, &dummy) == SUCCESS){

      return FAILURE;
   }
   curr = calendar->events[day - 1];

   /* allocates memory for required entities */
   event = malloc(sizeof(Event));
   event->name = malloc(strlen(name) + 1);
   if(event == NULL || event->name == NULL){
      
      return FAILURE;
   }

   /* assigns the info to the new event */
   event->info = info;
   event->duration_minutes = duration_minutes;
   strcpy(event->name, name);
   event->start_time = start_time;

   /* adding to an empty list */
   if(curr == NULL){
      calendar->events[day - 1] = event;
      event->next = NULL;

   }else{
      prev = NULL;

      /* uses the compare function to determine where the node will stay */
      while (curr != NULL && calendar->comp_func(curr, event) <= 0){
         prev = curr;
         curr = curr->next;
      }
      /* checks that your adding to front of list of not */
      if(prev != NULL){
         prev->next = event;
         event->next = curr;

      }else{
         calendar->events[day - 1] = event;
         event->next = curr;
      }
   }
   calendar->total_events += 1;
   return SUCCESS;
}

/************************************************************************/
/*   Traverses the calendar and finds the event with the name           */
/*   Returns the event in the event parameter and SUCESS if found       */
/************************************************************************/
int find_event(Calendar *calendar, const char *name, Event **event){
   Event *curr;
   int day;

   if(calendar == NULL || name == NULL){
      return FAILURE;
   }

   for(day = 0; day < calendar->days; day++){
      curr = calendar->events[day];

      while(curr != NULL){

         if(strcmp(curr->name, name) == 0){

            /* assigns the event parameter if event is not equal to null */
            if(event != NULL){
               *event = curr;
            }
            return SUCCESS;
         }
         curr = curr->next;
      }
   }
   return FAILURE;
}

/************************************************************************/
/*  Traverses the events of the specified day from the parameter to     */
/*  find the event and assign it into the parameter event               */
/************************************************************************/
int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event){
   Event *curr;

   if(calendar == NULL || name == NULL  || (day < 1 || day > calendar->days)){
      return FAILURE;
   }
   
   curr = calendar->events[day - 1];

   while(curr != NULL){

      if(strcmp(curr->name, name) == 0){

         /* assigns event only when event is not null */
         if(event != NULL){
            *event = curr;
         }
         return SUCCESS;
      }
      curr = curr->next;
   }
   return FAILURE;
}

/************************************************************************/
/*   Loops through the entire calendar to find the event with the name  */
/*   from parameter. Will remove that event from the list and adjust it */
/************************************************************************/
int remove_event(Calendar *calendar, const char *name){
   Event *curr, *prev;
   int i;

   if(calendar == NULL || name == NULL){

      return FAILURE;
   }

   /* loops through the days of the calendar */
   for(i = 0; i < calendar->days; i++){

      /* checks that the day is not empty */
      if(calendar->events[i] != NULL){
         
         curr = calendar->events[i];
         prev = NULL;

         while(curr != NULL && strcmp(curr->name, name) != 0){
            prev = curr;
            curr = curr->next;
         }
         /* checks that list is not empty */
         if(curr == NULL){
            continue;

         /* deals with case whether we are removing the first node */
         }else{

            if(prev != NULL){
               prev->next = curr->next;
            }else{
               calendar->events[i] = curr->next;
            }

            /* deallocates all relevant memory associated w/ event */
            if(curr->info != NULL){
               if(calendar->free_info_func != NULL){
                  calendar->free_info_func(curr->info);
               }
            }
            free(curr->name);
            free(curr);

            calendar->total_events -= 1;
            return SUCCESS;
         }
      }
   }
   return FAILURE;
}

/************************************************************************/
/*   Retrieves the info associated with the event that has the name     */
/*    specifies in the parameter. Info is a void pointer                */
/************************************************************************/
void *get_event_info(Calendar *calendar, const char *name){
   Event *curr;
   int i;

   for(i = 0; i < calendar->days; i++){
      curr = calendar->events[i];
      while(curr != NULL){
         
         if(strcmp(curr->name, name) == 0){
            return curr->info;
         }
         curr = curr->next;
      }
   }
   /* returns NULL if no string name was found */
   return NULL;
}

/************************************************************************/
/*   Loops through the entire calendar and clears every event in each   */
/*    day. All memory which was allocated will be destroyed             */
/************************************************************************/
int clear_calendar(Calendar *calendar){
   Event *curr, *prev = NULL;
   int i;

   if(calendar == NULL){
      return FAILURE;
   }

   /* outer loop traverses the days of the calendar */
   for(i = 0; i < calendar->days; i++){
      curr = calendar->events[i];

      while(curr != NULL){
         prev = curr;
         curr = curr->next;

         remove_event(calendar, prev->name);   
      }
      calendar->events[i] = NULL;
   }
   calendar->total_events = 0;

   return SUCCESS;
}

/************************************************************************/
/*   Removes all the events in the specified day from the parameter     */
/************************************************************************/
int clear_day(Calendar *calendar, int day){
   Event *curr, *prev = NULL;
   
   if(calendar == NULL || day < 1 || day > calendar->days){
      return FAILURE;
   }

   curr = calendar->events[day - 1];

   /* loops through all the events of that day */
   while(curr != NULL){
      prev = curr;
      curr = curr->next;

      remove_event(calendar, prev->name); /* calls remove event */
   }
   calendar->events[day - 1] = NULL;

   return SUCCESS;
}

/************************************************************************/
/*   Takes in a calendar and frees all the memory associated with it    */
/*    so its information is now not retrievable                         */
/************************************************************************/
int destroy_calendar(Calendar *calendar){
   if(calendar == NULL){
      return FAILURE;
   }

   clear_calendar(calendar);
   free(calendar->name);
   free(calendar->events);
   free(calendar);
   calendar = NULL;

   return SUCCESS;
}
