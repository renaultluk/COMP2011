#include <iostream>   
#include <cstdlib>

using namespace std; 

const int MIN_HEIGHT = 120;
const int MAX_HEIGHT = 160;

struct kidNode 
{ 
    int height; 
    kidNode* next; 
}; 


int getRandomHeight(int min, int max){
    return (rand() % (max - min + 1)) + min;
}

void printLinkedList(kidNode* head){
    for (kidNode* p = head; p != nullptr; p = p->next){
        cout << p->height;
        cout << " ";
    }
    cout << endl;
}

kidNode* createNode(int height){
    kidNode* newNode = new kidNode;
    newNode->height = height;
    return newNode;
}

void insertNode(kidNode*& head, int height){
    kidNode* newNode = createNode(height);
    newNode->next = head;
    head = newNode;
}

void deleteNode(kidNode*& head, int height){
    kidNode* p = head;
    kidNode* prev = nullptr;
    while (p != nullptr && p->height != height){
        prev = p;
        p = p->next;
    }
    if (p == nullptr){
        cout << "No such height in the list" << endl;
        return;
    }
    if (prev == nullptr){
        head = p->next;
    }
    else{
        prev->next = p->next;
    }
    delete p;
}

/** Task 1:  create an initial linked list with kid_num nodes with random heights, each presenting a kid. **/
kidNode* createKidList(int kid_num, int min_height, int max_height){
// Please implement task 1 here.
    kidNode* headKid = createNode(getRandomHeight(min_height, max_height));
    kidNode* newKid = headKid;
    for (int i = 1; i < kid_num; i++){
        newKid->next = createNode(getRandomHeight(min_height, max_height));
        newKid = newKid->next;
    }
    newKid->next = nullptr;
    return headKid;
}

/**Task 2: reverse the initial linked list.**/
kidNode* reverseKidList(kidNode* head){
// Please implement task 2 here.
    if (head == nullptr || head->next == nullptr){
        return head;
    } else {
        kidNode* cur = head;
        kidNode* prev = nullptr;
        kidNode* next = nullptr;
        for (kidNode* p = head; p != nullptr; p = next){
            cur = p;
            next = p->next;
            cur->next = prev;
            prev = cur;
        }
        return cur;
    }
}

/**Task 3: line up the kids according to their heights in an ascending order.**/
kidNode* sortKidList(kidNode* head){
// Please implement task 3 here.
    if (head == nullptr || head->next == nullptr){
        return head;
    } else {
        kidNode* cur = head;
        kidNode* prev = nullptr;
        kidNode* next = nullptr;
        for (kidNode* p = head; p != nullptr;){
            cur = p;
            next = p->next;
            if (cur->height > next->height){
                int temp = cur->height;
                next->height = cur->height;
                cur->height = temp;
                prev = next;
                p = cur->next;
            } else {
                prev = cur;
                p = cur->next;
            }
        }
        return head;
    }
}


int main()
{   
   int kid_num;

   do {
        cout << "===========================================" << endl;
        cout << "Please input the number of kids in the class (no more than 20) or enter an alphabet to exit:" << endl;
        cin >> kid_num;
        if (cin.fail())
            return 0;

        if (kid_num <= 0 || kid_num > 20){
            cout << "Invalid number! Please input a number again." << endl;
            continue;
        }

        kidNode* initial_line_head = createKidList(kid_num, MIN_HEIGHT, MAX_HEIGHT);

        cout << endl;
        cout << "Output:" << endl;
        cout << "The initial list of kids' heights is:" << endl;
        printLinkedList(initial_line_head);

        kidNode* reversed_line_head = reverseKidList(initial_line_head);
        cout << "The reversed list of kids' heights is:" << endl;
        printLinkedList(reversed_line_head);

        kidNode* sorted_line_head = sortKidList(reversed_line_head);
        cout << "The ascending list of kids' heights is:" << endl;
        printLinkedList(sorted_line_head);
   } while(true);

   return 0;
}