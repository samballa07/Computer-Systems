/******************/
/*  Seth Amballa  */
/* UID: 115884939 */
/*    samballa    */
/******************/

#include <stdio.h>
#include <math.h>
#define MAX_ARR_LEN 200
#define MAX_ASSIGNMENTS 50

/* function prototypes */
void print_info(int *assignments, int arr_len);
double calc_score(int *assignments, int arr_len, int penalty);
double calc_mean(int *assignments, int arr_len, int penalty);
double calc_deviation(int *assignments, int arr_len, double mean,int penalty);
void drop_lowest(int *og_arr, int to_drop, int num_assignments);
int is_in_array(int *arr, int value, int arr_len);

/****************************************************************************/
/* Simulates a grade calculating program based on assignment info and other */
/* stats. Will take in user input for assignment info and calculate the     */
/* score, mean, and standard deviation. It will take into account the late  */
/* penalty, number of assignments dropped and weight of assignments         */
/* This info is then outputted to the user                                  */
/****************************************************************************/
int main() {
   /* variables declarations and initializations */
   int num_assignments, late_penalty, to_drop, i, j = 1, arr_len, weights = 0;
   char stat_info;
   int assignment_info[MAX_ARR_LEN], after_drop[MAX_ARR_LEN];
   double score, mean;

   /* gets user input */
   scanf("%d %d %c", &late_penalty, &to_drop, &stat_info);
   scanf("%d", &num_assignments);
   arr_len = num_assignments * 4;

   /* takes in the user assignment info */
   for (i = 0; i < arr_len; i++) {
      scanf("%d,", &assignment_info[i]);
      /* for every third element read in, weight is incremented */
      if (j == 3) {
         weights += assignment_info[i];
      }
      if (j == 4) {
         j = 1;
      } else {
         j++;
      }
   }
   /* if the weight is not 100, then the program exits and msg is shown */
   if (weights != 100) {
      printf("ERROR: Invalid values provided\n");

   } else {
      /* checks if there are are assignments to be dropped */
      if (to_drop > 0) {

         /* makes after_drop a copy of the original array */
         for (i = 0; i < arr_len; i++) {
            after_drop[i] = assignment_info[i];
         }
         /* calls drop and calculates score with the after_drop array */
         drop_lowest(after_drop, to_drop, num_assignments);
         score = calc_score(after_drop, arr_len, late_penalty);

      } else {
         /* calculates score by sending the regular array */
         score = calc_score(assignment_info, arr_len, late_penalty);
      }

      /* displays the output message with the correct info */
      printf("Numeric Score: %5.4f\n", score);
      printf("Points Penalty Per Day Late: %d\n", late_penalty);
      printf("Number of Assignments Dropped: %d\n", to_drop);
      printf("Values Provided:\n");
      printf("Assignment, Score, Weight, Days Late\n");

      /* prints the assignments in order of assignment number */
      print_info(assignment_info, arr_len);

      /* checks that the user wants means and standard deviation info */
      if (stat_info == 'Y' || stat_info == 'y') {
         mean = calc_mean(assignment_info, arr_len, late_penalty);

         /* calculates and prints the mean and standard deviation */
         printf("Mean: %5.4f,", mean);
         printf(" Standard Deviation: %5.4f\n",
                calc_deviation(assignment_info, arr_len, mean, late_penalty));
      }
   }
   return 0;
}

/***************************************************************************/
/*   Takes in the array of assignments and the length of the array.        */
/* Will print out the information in ascending order of the assignment num */
/***************************************************************************/
void print_info(int *assignments, int arr_len) {
   int in_order = 1, i;

   /* uses in_order to make sure that the assignments are printed in order */
   /* interates until in_order reaches the amount of assignments */
   while (in_order <= (arr_len / 4)) {

      /* loops through the array to find correct assignment num */
      for (i = 0; i < arr_len; i += 4) {

         /* prinst the info when that correct assignment is found */
         if (assignments[i] == in_order) {

            printf("%d, %d, %d, %d\n",
                   assignments[i], assignments[i + 1],
                   assignments[i + 2], assignments[i + 3]);
            in_order++;
         }
      }
   }
}

/***************************************************************************/
/*  Takes in the array of assignments, length of the array, and penalty    */
/* Calculates the numeric score based on how many days each assignment was */
/* late and the weight of each assignment which does not necassarily       */
/* have to add up to 100. Returns total score as a double                  */
/***************************************************************************/
double calc_score(int *assignments, int arr_len, int penalty) {
   int i, assignment_score, weights = 0;
   double score = 0;

   /* determines weight sum incase any assignments where dropped */
   for (i = 0; i < arr_len; i += 4) {
      weights += assignments[i + 2];
   }

   if (weights == 0) {
      return 0;
   }

   for (i = 0; i < arr_len; i += 4) {
      assignment_score = assignments[i + 1];
      /* checks if the late days is greater than 0 */
      if (assignments[i + 3] > 0) {
         /* decrements the score accordingly */
         assignment_score -= (assignments[i + 3] * penalty);
      }
      /* if the score is 0 or less then the assignment is 0 */
      if (assignment_score <= 0) {
         assignment_score = 0;
      }
      /* is divided by the weights incase something was dropped */
      score += assignment_score * ((double) assignments[i + 2] / weights);
   }
   return score;
}

/***************************************************************************/
/* Takes in the array of assignments, length of the array, and penalty     */
/* Calculates the mean of the assignment scores while taking the days late */
/* and penalty into account. Returns mean as a double                      */
/***************************************************************************/
double calc_mean(int *assignments, int arr_len, int penalty) {
   double mean;
   int i, sum = 0;

   /* finds the sum of the assignments with the penalty applied */
   for (i = 0; i < arr_len; i += 4) {
      sum += assignments[i + 1];
      if (assignments[i + 3] > 0) {
         sum -= assignments[i + 3] * penalty;
      }
   }
   if (sum < 0) {
      sum = 0;
   }
   /* divide sum by num of assignments and returns mean */
   mean = sum / (arr_len * 0.25);
   return mean;
}

/***************************************************************************/
/* Takes in the array of assignments, length of arr, penalty, and mean     */
/* Calculates the standard deviation of the assignment scores while taking */
/* the days late and penalty into account. Returns mean as a double        */
/***************************************************************************/
double calc_deviation(int *assignments, int arr_len, double mean, int penalty) {
   int i, num_assignments = 0, assignment_score;
   double sum = 0, new_mean;

   /* iterates through every assignment in array */
   for (i = 0; i < arr_len; i += 4) {
      assignment_score = assignments[i + 1];

      /* adds penalty if needed */
      if (assignments[i + 3] > 0) {
         assignment_score -= assignments[i + 3] * penalty;

         if (assignment_score < 0) {
            assignment_score = 0;
         }
      }
      /*  finds the squared difference between score and mean of all the
         scores and adds it to sum */
      sum += pow((assignment_score - mean), 2);

      num_assignments++;
   }

   /* calculates the standard deviation and returns it */
   new_mean = sum / num_assignments;
   return sqrt(new_mean);
}

/***************************************************************************/
/* Takes in the array of assignments, # of assignments, number to drop.    */
/* Finds the assignments which are to be dropped and sets the weights of   */
/* these assignments to zero, which will change the declared array in main */
/***************************************************************************/
void drop_lowest(int *og_arr, int to_drop, int num_assignments) {
   int min_arr[MAX_ASSIGNMENTS] = { 0 };
   int count = 1, arr_len = num_assignments * 4, i, j;
   int min, min_arr_index = 0, value = 100, prev_index;
   double weight;

   /* iterates until all necassary values are "dropped" */
   while (count++ <= to_drop) {

      for (i = 0; i < arr_len; i += 4) {
         weight = og_arr[i + 2] * 0.01;

         /* checks that the current index value is smaller than what
            exists in the value variable. If both values are equal, then
            the lower assignment is chosen to be dropped */
         if ((og_arr[i + 1] * weight < (double) value &&
              is_in_array(min_arr, og_arr[i], to_drop) == 0) ||
             (og_arr[i + 1] * weight == (double) value &&
              og_arr[prev_index] > og_arr[i])) {

            min = og_arr[i];    /* stores the assignment num in min */
            value = og_arr[i + 1] * weight;

            /* keeps track of previous index incase values are the same */
            prev_index = i;
         }
      }

      /* the assignment number is stored in a separe array of assignments
         to be dropped */
      min_arr[min_arr_index++] = min;
      value = 100;
   }

   /* finds all the assignment numbers which exists in both arrays and 
      sets the weight to zero in the original array if so */
   for (i = 0; i < to_drop; i++) {
      for (j = 0; j < arr_len; j += 4) {
         if (min_arr[i] == og_arr[j]) {
            og_arr[j + 2] = 0;
         }
      }
   }
}

/***************************************************************************/
/* Takes in the array of ints, length of the array, and an integer         */
/* Checks if the integer value is in array and returns 1 if so, else 0     */
/***************************************************************************/
int is_in_array(int *arr, int value, int arr_len) {
   int i;

   /* loops through the array to find the value from the parameter */
   for (i = 0; i < arr_len; i++) {

      /* if found, function returns 1, else it returns 0 */
      if (arr[i] == value) {
         return 1;
      }
   }
   return 0;
}
