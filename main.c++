// -------------------
// prog/deque/main.c++
// -------------------

// --------
// includes
// --------

#include <iostream> // cout, endl

#include "Deque.h"
#include "DequeTest.h"

// ----
// main
// ----

/**
 * function main is a driver of function deque_test instantiated with class Deque
 */
int main () {
    using namespace std;
    using namespace dt::prog::deque;
    deque_test< Deque<int> >();
    cout << "Done." << endl;
    return 0;}
