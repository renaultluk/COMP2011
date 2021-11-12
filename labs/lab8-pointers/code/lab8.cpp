#include <iostream>
#include <cstring>
#include "lab8.h"
using namespace std;

/* Task 0: calculate and set the priority for one person

    1. Priority associated with different job types
        1. Use M represent Medical staffs, they have highest priority of 3
        2. Use S represent Service workers, they have a priority value of 2
        3. Use O represent Other people, they have a priority value of 1
    2. Priority value with ages
        1. age >= 60 priority is 4
        2. age < 60 and age >= 40 priority is 3
        3. age < 40 and age >= 20 priority is 2
        4. age < 20 priority value is 1
    
    A person's priority = job priority + age priority
    For exmaple, a medical staff with age >=60 has priority 3 + 4 = 7
*/

void swap(Person* p1, Person* p2)
{
    Person* temp = p1;
    p1 = p2;
    p2 = temp;
}

void setPriority(Person& p){
	int jobPriority, agePriority;
    
    if (p.job == 'M'){
        jobPriority = 3;
    }
    else if (p.job == 'S'){
        jobPriority = 2;
    }
    else{
        jobPriority = 1;
    }

    if (p.age >= 60)
        agePriority = 4;
    else if (p.age < 60 && p.age >= 40)
        agePriority = 3;
    else if (p.age < 40 && p.age >= 20)
        agePriority = 2;
    else
        agePriority = 1;
    p.priority = jobPriority + agePriority;
}

/*
    Task 1. Sort the sign up queue via pointers and print the priority queue
    1a. Set the priority of everyone in the sign up queue 
    1b. Figure out the vaccine sequence by properly set up the pointer
    1c. Print the priority queue

    queue is the sign up queue, struct array
    pointerqueue is the priority queue, a pointer array pointing to elements of queue
    when do sorting, sign up queue remain unchanged!!! 
    the sorting is done by setup pointer array. 
    For example, pointerqueue[0] now points to the person with highest priority. 
*/
void prioritySort( Person queue[], Person* pointerqueue[], int size) {
   // Task 1a
    for (int i = 0; i < size; i++) {
         setPriority(queue[i]);
    }
    
    // Task 1b
    for (int i = 0; i < size+1; i++) {
        for (int j = 0; j < size-i; j++) {
            Person a = *(pointerqueue[j]);
            Person b = *(pointerqueue[j+1]);
            if (a.priority < b.priority) {
                Person* temp = pointerqueue[j];
                pointerqueue[j] = pointerqueue[j+1];
                pointerqueue[j+1] = temp;
                // pointerqueue[j] = &queue[j+1];
                // pointerqueue[j+1] = &queue[j];
            }
        }
    }
}

/* 
    Task 2. Assume that the the highest priority person is vaccinated 
    Remove him from the priority queue and print the updated priority queue
    Important: 
    DO NOT remove him from the sign up queue
*/
void vaccinateTopPriority(Person queue[], Person* pointerqueue[], int size) {
    (*pointerqueue[0]).vaccined = true;
    pointerqueue[0] = nullptr;

    for (int i = 1; i < size; i++) {
        pointerqueue[i-1] = pointerqueue[i];
    }
    pointerqueue[size-1] = nullptr;
}

/* 
    Task 3. Add a new person to the END of current sign up queue 
            Since the newly-added person can cut in line with his priority,
            most likely you need to update the priority queue 

    This function is used when setup the initial queues from file
*/
void addPerson(int id, const char name[], char job, int age, Person queue[], Person* pointerqueue[], int& size) {
//    cout << "entered addPerson" << endl;
   queue[size].id = id;
//    cout << "added id" << endl;
   queue[size].age = age;
//    cout << "added age" << endl;
   queue[size].job = job;
//    cout  << "added job" << endl;
   queue[size].vaccined = false;
   strcpy(queue[size].name, name);
//    cout << "added name" << endl;
   pointerqueue[size] = &queue[size];
//    cout << "added pointerqueue" << endl;

   setPriority(queue[size]);
   prioritySort(queue, pointerqueue, size);
}
