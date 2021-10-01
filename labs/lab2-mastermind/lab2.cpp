// https://magisterrex.files.wordpress.com/2014/07/mastermindrules.pdf
// Cindy https://www.youtube.com/watch?v=dMHxyulGrEk

#include <iostream> 
#include <cstdlib>
#include <time.h>  
using namespace std;

int main() 
{
    srand( time(NULL) );
    //Codemaker's choice of a random 4-color combination among six colors 
    // 1 Yellow, 2 Orange, 3 black, 4 Green, 5 Blue, 6 Purple
    int p0 = rand() % 6 + 1; // 1 to 6
    int p1 = rand() % 6 + 1; // 1 to 6
    int p2 = rand() % 6 + 1; // 1 to 6
    int p3 = rand() % 6 + 1; // 1 to 6

    // variable for code breaker's guess
    int g0;
    int g1;
    int g2;
    int g3;

    // variable for codemaker's response
    // red: correct color and correct position
    // white: correct color but wrong position
    // red overrides white
    int redpin;
    int whitepin;

    //you may declare additional variables here  to help you solve the problem:
    int answerArray[4]; // array to store code maker's colors
    int guessArray[4];  // array to store the player's guesses
    bool pinArray[4] = {false, false, false, false};    // array to store whether the position in the code corresponds to a red or white pin
    bool gameWon = false;   // whether the player has won the game
    int attempts = 0;   // number of attempts by the player
    
    bool debug = true;

    cout << "==============================" << endl;   //title
    cout << "Welcome to the Mastermind Game" << endl;
    cout << "==============================" << endl;

    if (debug){
        cout << "Coder maker's colors:" << endl;
        cout << "p0=" << p0 << "\tp1=" << p1 << "\tp2=" << p2 << "\tp3=" << p3 << endl << endl;
    }

    cout << "Code maker has made his choice." << endl;
    cout << "Code breaker, you have 12 attempts to crack his code." << endl;
    cout << "Game starts!" << endl << endl;

    
    // Write your code below this section:
    answerArray[0] = p0;    // transfer the colors into the answerArray array
    answerArray[1] = p1;
    answerArray[2] = p2;
    answerArray[3] = p3;
    

    while ((attempts < 12) && (!gameWon)){  // the loop ends in 2 conditions: either the player has won, or they have run out of attempts
        redpin = 0; // reset variables in each iteration
        whitepin = 0;
        for (int i = 0; i < 4; i++) {
            pinArray[i] = false;
        }

        cout << "Attempt " << attempts+1 << endl;   // display number of attempts
        cout << "Please input your guess:" << endl;
        cin >> g0 >> g1 >> g2 >> g3;    // read player input

        guessArray[0] = g0; // transfer user input into the guessArray array
        guessArray[1] = g1;
        guessArray[2] = g2;
        guessArray[3] = g3;

        for (int i = 0; i < 4; i++){    // check if any red pin can be placed
            if (guessArray[i] == answerArray[i]){   // if the color and the position matches exactly
                pinArray[i] = true;
                redpin++;
            }
        }

        if (redpin == 4){   // check to see whether the player guessed all 4 colors correctly, i.e. having 4 red pins
            gameWon = true;
            break;
        }

        for (int i = 0; i < 4; i++){    // check if any white pin can be placed
            for (int j = 0; j < 4; j++){
                if ((guessArray[i] == answerArray[j]) && (pinArray[j] == false)){   // search for the color in the answer, and check if there is already a corresponding pin placed
                    pinArray[j] = true;
                    whitepin++;
                }
            }
        }

        cout << "Red pins: " << redpin << ", White pins: " << whitepin << endl << endl; // end the turn by displaying number of red and white pins placed for the attempt
        attempts++; // increment the attempt counter
    }


    if (gameWon){   // the case where the player wins
        cout << "Red pins: " << redpin << ", White pins: " << whitepin << endl << endl;
        cout << "Congratulations!" << endl;
        cout << "Code maker's colors:" << endl;
        cout << "p0=" << p0 << "\tp1=" << p1 << "\tp2=" << p2 << "\tp3=" << p3 << endl;
        cout << "You cracked the code with " << attempts+1 << " attempts" << endl;
    } else {
        // Player runs out of attempts and loses 
        cout << "Sorry, you ran out of turns :( "<< endl; 
    }


    return 0;
}