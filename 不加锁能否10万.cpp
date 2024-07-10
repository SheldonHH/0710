#include <iostream>
#include <thread>
#include <vector>

using namespace std;

int mycnt = 0;
void sum(){
    for(size_t i = 0; i < 10000; i++){
        mycnt++;       
    }
}


int main(){
    vector<thread> mybox;
    for(size_t i = 0; i < 10; i++){
        mybox.emplace_back(sum);
    }
}