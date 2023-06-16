// Program to demonstrate STL iterators.
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

int main() {
    vector<int> container;

    for (int i=0; i<5; i++)
        container.push_back(i);

    vector<int>::iterator p;
    vector<int>::reverse_iterator k;
    for (p=container.begin(), p!=container.end(), p++)
        cout<<*p<<endl;
    
    for (k=container.rbegin(), p!=container.rend(), k++)

    for (p=container.begin(), p!=container.end(), p++)
        *p = 0;

    return 0;
}