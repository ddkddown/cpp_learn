#include <iostream>

using namespace std;

//常量表达式是指值不会改变并且在编译过程中就能得到计算结果的表达式。

int main() {
    const int max_files = 20; // 是常量表达式, 值不会变，编译时就可确定。
    const int limit = max_files + 1; //是常量表达式，值不会变，编译时就可确定。
    int staff_size = 27; //不是常量表达式，值允许在运行时发生改变。

    //contexpr 向编译器表明修饰的变量或函数是常量表达式，可以放心在编译时进行优化，无需等到运行时。
    //但是编译器不会完全相信程序员，只是对contexpr作为一个参考。

    constexpr int test = 3;

    //这一步会编译出错
    //test = 4;

    // contexpr修饰的指针表示这是一个常量指针，指针存储地址不可变，而不是指向值不可变。
    constexpr int *p = &staff_size; 
    *p = 5;
    cout<<"test: "<<test<<" ,staff_size:"<<staff_size<<endl;
}