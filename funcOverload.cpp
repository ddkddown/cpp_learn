#include <iostream>

using namespace std;
//同一个作用域内的函数间名字相同但是入参列表不同，成为重载overload
//不能通过返回类型的不同来定义重载
void test(int i) {
    cout<<"test1:"<<i<<endl;
}

void test(int i, int j) {
    cout<<"test2:"<<i<<" ,"<<j<<endl;
}

int main(){
    test(1);
    test(1,2);

    return 0;
}