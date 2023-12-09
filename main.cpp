#include "func.hpp"

int main(){
    //1.LeeÊÏËã·¨
    randomMatrixCreate();   //³õÊ¼»¯¾ØÕó
    for(int i = 0;i < OBSINFO.size();i++){
        Lee(MATRIX[i],CreationTimes+i,STARTANDEND[i][0],STARTANDEND[i][1],STARTANDEND[i][2],STARTANDEND[i][3]);
    }
    return 0;
}