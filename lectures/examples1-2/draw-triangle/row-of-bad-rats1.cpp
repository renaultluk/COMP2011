#include <iostream>     /* File: row-of-bad-rats1.cpp */
using namespace std;

int main()
{
    cout << "Size of a triangle: " << endl;
    int size;
    cin >> size;

    // Find out the number of RATs in a row
    const int TOTAL_NUM_COLUMNS = 105; // Assumed screen width
    int num_RATs = TOTAL_NUM_COLUMNS / size; 

    for (int i = 1; i <= size; i++)
    {
        for (int n = 0; n < num_RATs; n++)
            for (int j = 0; j < i ; j++)
                cout << '*';

        cout << endl;   
    }

    return 0;
}
