//count_animals_recursion.cpp
#include <iostream>
#include <cstdio>
using namespace std;
const int BUFFER_SIZE = 100;
bool count_animals(const double weights[], int types, double remaining, char to_print[], double total) {
     if (types == 1) {
	double sum = remaining;
	while (sum > 0)
	    sum -= weights[0];
	if (sum != 0)  
	    return false;
	cout << to_print << remaining / weights[0] << " * " << weights[0] << " = " << total << endl;
	return true;
     }
     bool solvable = false;
     char buffer[BUFFER_SIZE];
     for (int i = 0; weights[types - 1] * i <= remaining; i++) {
	sprintf(buffer, "%s%d * %.2f + ", to_print, i, weights[types - 1]);
	if  (count_animals(weights, types - 1, remaining - weights[types - 1] * i, buffer, total)) {
	    solvable = true;
	}
     }
     return solvable;
}


int main() {
    double w[] = {10.5, 4.5, 5.5, 3.5, 2};
    double total = 36.5;
    char buffer[BUFFER_SIZE] = {};
    if (!count_animals(w, 5, total, buffer, total)) 
	cout << "No solution" << endl;
}
