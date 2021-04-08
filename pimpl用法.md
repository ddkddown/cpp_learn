作为库作者，有时候希望将类成员隐藏，但往往类成员又需要在.h文件中声明。
例如
```
class test {
private:
  int i_am_shy;
};
```
此时不希望暴露i_am_shy成员,可以采用impl写法

```
class test {
public:
  test();
private:
  struct impl; //前置声明
  unique_ptr<impl> pImpl;
}

```

在.cpp文件中
```
struct impl {
  int i_am_shy;
};

test::test() {
  pImpl.reset(new impl());
}
```

这样就可将类成员隐藏起来了。
