// C++ program to find factorial using loops
#include <bits/stdc++.h>
using namespace std;

// function to find factorial
int factorial(int n)
{
    int fact = 1;
    while (n > 1) {
        fact *= n;
        n--;
    }

    return fact;
}

// driver code
int main()
{
    int num = 5;

    cout << factorial(num);

    return 0;
}
