#include <iostream>
#include <functional>

using namespace std;

int test(int i) {
    cout<<i<<endl;
}

int main() {

    //bind 相当于创建一个闭包
    auto f = bind(test, 1);

    //_1 代表第一个占位符
    auto f2 = bind(test, placeholders::_1);
    f();
    f2(2);
}