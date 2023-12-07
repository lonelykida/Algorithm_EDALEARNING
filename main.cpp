#include "func.hpp"

int sum(int a,int b){
    return a+b;
}

int(*funcp)(int,int);

int main(){
    funcp = sum;
    cout<<funcp(3,5)<<endl;
    return 0;
}