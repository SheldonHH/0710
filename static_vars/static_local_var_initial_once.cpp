#include <iostream>  
  
void function() {  
    static int count = 0; // 静态局部变量  
    count++;  
    std::cout << "Function called " << count << " times.\n";  
}  
  
int main() {  
    function(); // 输出：Function called 1 times.  
    function(); // 输出：Function called 2 times.  
    return 0;  
}