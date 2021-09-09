#include <iostream>     /* File: guess_number.cpp */
#include <stdlib.h>     // Needed for calling the rand() function
#include <time.h>       // May need for calling the time() function
using namespace std;

int main()   // 2 players, multiple rounds, fixed range, random number
{
    /* Random number generation */
    srand(time(0));                // (1) Seed the random number generator
    // srand(time(0)) C++11
                                    // number of seconds since 1970/01/01 00:00:00
    int number = rand() % 100 + 1; // (2) Generate a random no. in [1..100]
                                    // rand() returns an int 0 to 2^31-1

    int guess;
    int low = 1, high = 100;  
    int player = 1;     // Set Player 1 as the current player

    cout << "The generated number is: " << number << endl;
    do 
    {
        cout << "Player " << player
             << ", please enter your guess: " << endl;
        cin >> guess;

        while (guess < low || guess > high) // Input validation loop
        {
            cout << "Invalid input, please enter a number between " 
                 << low << " and " << high << endl;
            cin >> guess;
        }    

        if (guess == number)
            cout << "Player " << player <<", you win!!!" << endl;

        else if (guess < number) 
        {
            cout << "Sorry, the number is bigger than "
                 << guess << endl;
            low = guess + 1; // Update the lower bound of the range
        } 
        else 
        { 
            cout << "Sorry, the number is smaller than " 
                 << guess << endl; 
            high = guess - 1; // Update the upper bound of the range
        }

        player = (player % 2) + 1; // This makes 1 $\rightarrow$ 2 and 2 $\rightarrow$ 1
    } while (guess != number);

    return 0;
}
