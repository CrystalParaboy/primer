# 第十二章 动态内存

## 练习12.7
编写函数，返回一个动态分配的`int` 的`vector`。将此`vector`传递给另一个函数，这个函数读取标准输入，将读入的值保存在`vector`元素中。再将`vector`传递给另一个函数，打印读入的值。
记得在恰当的时刻delete vector。(使用智能指针所以不用）
```cpp    
shared_ptr<vector<int>> alloc_vec(){
    return make_shared<vector<int>>();
}

void assign(shared_ptr<vector<int>> p){
    int i=0;
    while(cin>>i){
        p->push_back(i);
    }
}

void print_vec(shared_ptr<vector<int>> p){
    for(auto i:*p){
        cout<<i<<' ';
    }
}

int main(){
    auto p=alloc_vec();
    assign(p);
    print_vec(p);
    return 0;
}
```
## 练习12.10

> 下面的代码调用了第413页中定义的`process` 函数，解释此调用是否正确。如果不正确，应如何修改？

```cpp
shared_ptr<int> p(new int(42));
process(shared_ptr<int>(p));
```

解：

正确。`shared_ptr<int>(p)` 会创建一个临时的智能指针，这个智能指针与 `p` 引用同一个对象，此时引用计数为 2。当表达式结束时，临时的智能指针被销毁，此时引用计数为 1。

## 练习12.11

> 如果我们像下面这样调用 `process`，会发生什么？

```cpp
process(shared_ptr<int>(p.get()));
```

解：

这样会创建一个新的智能指针，它的引用计数为 1，这个智能指针所指向的空间与 `p` 相同。在表达式结束后，这个临时智能指针会被销毁，引用计数为 0，所指向的内存空间也会被释放。而导致 `p` 所指向的空间被释放，使得 p` 成为一个空悬指针。

## 练习12.12

> `p` 和 `sp` 的定义如下，对于接下来的对 `process` 的每个调用，如果合法，解释它做了什么，如果不合法，解释错误原因：

```cpp
auto p = new int();
auto sp = make_shared<int>();
(a) process(sp);
(b) process(new int());
(c) process(p);
(d) process(shared_ptr<int>(p));
```

解：

* (a) 合法。将`sp` 拷贝给 `process`函数的形参，在函数里面引用计数为 2，函数结束后引用计数为 1。
* (b) 不合法。不能从内置指针隐式转换为智能指针。
* (c) 不合法。不能从内置指针隐式转换为智能指针。
* (d) 合法。但是智能指针和内置指针一起使用可能会出现问题，在表达式结束后智能指针会被销毁，它所指向的对象也被释放。而此时内置指针 `p` 依旧指向该内存空间。之后对内置指针 `p` 的操作可能会引发错误。

## 练习12.13

> 如果执行下面的代码，会发生什么？

```cpp
auto sp = make_shared<int>();
auto p = sp.get();
delete p;
```

解：

智能指针 `sp` 所指向空间已经被释放，再对 `sp` 进行操作会出现错误。

## 练习12.15（12.1.4，p443）

>编写你自己版本的用 shared_ptr 管理 connection 的函数

```cpp
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

struct destination
{
    int port;
    destination(int p):port(p){}
}; 

struct con
{
    int port;
    con(int p):port(p){}
};

con connect(destination* pDes){
    shared_ptr<con> pCon(new con(pDes->port));
    cout<<"connection creating: "<<pCon.use_count()<<endl;
    return *pCon;
}

void disconnect(con pCon){
    cout<<"disconnect: "<<pCon.port<<endl;
}

void f(destination &des){
    con pcon=connect(&des);
    shared_ptr<con> p(&pcon,[](con* p){disconnect(*p);});
    cout<<"connection now("<<p.use_count()<<")"<<endl;

}

int main(){
    destination des(1942);
    f(des);
    return 0;
}
```

## 练习12.17

> 下面的 `unique_ptr` 声明中，哪些是合法的，哪些可能导致后续的程序错误？解释每个错误的问题在哪里。

```cpp
int ix = 1024, *pi = &ix, *pi2 = new int(2048);
typedef unique_ptr<int> IntP;
(a) IntP p0(ix);
(b) IntP p1(pi);
(c) IntP p2(pi2);
(d) IntP p3(&ix);
(e) IntP p4(new int(2048));
(f) IntP p5(p2.get());
```

解：

* (a) 不合法。在定义一个 `unique_ptr` 时，需要将其绑定到一个`new` 返回的指针上。
* (b) 不合法。理由同上。
* (c) 合法。但是也可能会使得 `pi2` 成为空悬指针。
* (d) 不合法。当 `p3` 被销毁时，它试图释放一个栈空间的对象。
* (e) 合法。
* (f) 不合法。`p5` 和 `p2` 指向同一个对象，当 `p5` 和 `p2` 被销毁时，会使得同一个指针被释放两次。

## 练习12.18

> `shared_ptr` 为什么没有 `release` 成员？

`release` 成员的作用是放弃控制权并返回指针，因为在某一时刻只能有一个 `unique_ptr` 指向某个对象，`unique_ptr` 不能被赋值，所以要使用 `release` 成员将一个 `unique_ptr` 的指针的所有权传递给另一个 `unique_ptr`。而 `shared_ptr` 允许有多个 `shared_ptr` 指向同一个对象，因此不需要 `release` 成员。
