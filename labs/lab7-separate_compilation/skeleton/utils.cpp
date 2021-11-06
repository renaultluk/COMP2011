#include <iostream> /* COMP2011 Lab 7: utils.cpp */
#include <cstring>
#include "my_include.h"

void doubleSwap(double &x, double &y) {
   double temp = x;
   x = y;
   y = temp;
}

void strSwap(char x[MAX_ARRAY_LENGTH+1], char y[MAX_ARRAY_LENGTH+1]) {
   char temp[MAX_ARRAY_LENGTH+1];
   strcpy(temp, x);
   strcpy(x, y);
   strcpy(y, temp);
}

/* Task One */
void bubbleSort(double arr[], int n, bool ascending)
{
   for (int i = 0; i < n; i++) {
      for (int j = 0; j < n-i-1; j++) {
         if (ascending) {
            if (arr[j] > arr[j+1]) {
               doubleSwap(arr[j], arr[j+1]);
            }
         } else {
            if (arr[j] < arr[j+1]) {
               doubleSwap(arr[j], arr[j+1]);
            }
         }
      }
   }
}



/* Task Two */
void bubbleSort(char arr[][MAX_ARRAY_LENGTH+1], int n, bool ascending)
{
   for (int i = 0; i < n; i++) {
      for (int j = 0; j < n-i-1; j++) {
         if (ascending) {
            if (strcmp(arr[j], arr[j+1]) > 0) {
               strSwap(arr[j], arr[j+1]);
            }
         } else {
            if (strcmp(arr[j], arr[j+1]) < 0) {
               strSwap(arr[j], arr[j+1]);
            }
         }
      }
   }
}



/* Given Functions */
void printArray(const double arr[], int size)
{
   for (int i = 0; i < size; i++)
      cout << arr[i] << " ";
   cout << endl;
}

void printArray(const char arr[][MAX_ARRAY_LENGTH+1], int size)
{
   for (int i = 0; i < size; i++)
      cout << arr[i] << endl;
   cout << endl;
}

