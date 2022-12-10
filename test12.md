编写函数，返回一个动态分配的 int 的vector。将此vector传递给另一个函数，这个函数读取标准输入，将读入的值保存在 vector元素中。再将vector传递给另一个函数，打印读入的值。
记得在恰当的时刻delete vector。(使用智能指针所以不用）

/code 
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
