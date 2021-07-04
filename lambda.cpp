#include <iostream>

using namespace std;


int main() {

    int i = 1;
    cout<<"i addr:"<<&i<<endl;
    //值捕获，拷贝
    auto f = [i](int j)->int {
        cout<<"addr:"<<&i<<" ,count:"<<i+j<<endl;
        return i+j;
    };

    //引用捕获，引用
    auto f2 = [&i](int j)->int {
        cout<<"addr:"<<&i<<" ,count:"<<i+j<<endl;
        return i+j;
    };

    f(1);
    f2(1);

}