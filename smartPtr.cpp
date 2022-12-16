/**
 * 1、定义一个类
1）需要存放对象指针（类型不确定），所以是一个模板类；

2）统计对象的引用数量，需要一个计数器；

构造函数------每次构造一个智能指针，引用计数都从1开始
4）析构函数----当引用计数减1 等于0 的时候，需要释放智能指针内部的对象和计数器指针
*/
#include <iostream>
using namespace std;
template<typename T>
class smartptr
{
private:
    T *object;
    int *count;
public:
    smartptr(){
        count=new int(1);
        object=nullptr;
    }

    smartptr(T *ob):object(ob){
        count=new int(1);
    }

    smartptr(smartptr<T> &p){
        (*p.count)++;
        object=p.object;
        count=p.count;
    }
    //运算符重载
    smartptr<T> &operator=(const smartptr<T> &p) {
        //因为运算符重载，左侧的对象已经创建好了，所以需要释放
        if (--(*count) == 0) {
            if (object) {
                delete object;
            }
            delete count;
        }

        ++(*p.count);
        object = p.object;
        count = p.count;
        return *this;
    }

    ~smartptr(){
        cout<<"~ptr"<<endl;
        if(--(*count)==0){
            if(object){ delete object;}
            delete count;
            object=NULL;
            count=NULL;
        }
    }
    int use_count(){
        return *(this->count);
    }
};

class Person
{
private:
    /* data */
public:
    ~ Person(){
        cout<<"Xi gou person"<<endl;
    };
};

int main(){
    
    /**
     * 手写智能指针
     */

    Person* person=new Person();
    Person* person1=new Person();

    /**
     * 智能指针的构造函数，计数+1
     */
    cout<<"-----1-------"<<endl;
    smartptr<Person> smartptr1(person);
    cout<<"构造函数执行后："<<smartptr1.use_count()<<endl;

    /**
     * 拷贝构造函数   计数+1
     */
    cout<<"-----2-------"<<endl;
    smartptr<Person> smartptr2(smartptr1);
    cout<<"构造函数执行后："<<smartptr2.use_count()<<endl;

    /**
     * 拷贝构造函数，计数+1
     */
    cout<<"-----3-------"<<endl;
    smartptr<Person> smartptr3=smartptr1;
    cout<<"构造函数执行后："<<smartptr3.use_count()<<endl;

    /**
     * 运算符重载  计数+1（会先创建一个对象，然后再运算符重载中释放已经创建好的默认对象，然后再赋值）
     */
    cout<<"-----4-------"<<endl;
    smartptr<Person> smartptr4;
    smartptr4=smartptr1;
    cout<<smartptr4.use_count()<<endl;//
    return 0;
}


//输出结果就自己去玩了
