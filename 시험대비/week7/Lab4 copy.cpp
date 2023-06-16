#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

int main() {
    vector<char> container;
    
    container.push_back('A');
    container.push_back('B');
    container.push_back('C');
    container.push_back('D');

    vector<char>::iterator p;

    p=container.begin();

    cout << container[2] <<endl;
    cout << p[2] << endl;
    cout << *(p+2) << endl;
    
    return 0;
}  