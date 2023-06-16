//Demonstration of the stringstream class. This program takes
//a string with a name followed by scores. It uses a
//stringstream to extract the name as a string, the scores
//as integers, then calculates the average score. The name
//and average are placed into a new string.
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
int main( )
{
    stringstream ss;
    ss.str("");
    ss.clear();

    string scores = "name 10 20 30";

    ss << scores;

    string name = "";
    ss >> name;
    int score;
    while (ss >> score) {
        total+=score
        cnt ++;
    }
}