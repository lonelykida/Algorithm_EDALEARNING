#include "head.hpp"

/*  李氏算法
    1.分块，块大小最大60*60
    2.从起点开始做infection,要记录起终点的xy坐标，分别是sx,sy,ex,ey，要记录时间
    3.回溯，直接从起点往终点找，要记录时间
    4.将结果记录到文件中
*/

//定义必要变量
#define BLOCKXSIZE 60   //块的最大xy
#define BLOCKYSIZE 60
int SX,SY,EX,EY;        //起终点x、y坐标
int INFECTION_NON_TIME = 0,INFECTION_TIME = 0; //非递归和递归的感染时间
int FEEDBACK_TIME = 0;  //回溯时间
int TIMES = 0;          //记录第几个矩阵
int BLOCK_X = 1,BLOCK_Y = 1;    //记录矩阵行列的分块数
std::string INIT_ARR_FILE = "./INIT_ARR_"+std::to_string(TIMES)+".txt";             //记录初始矩阵结果
std::string INFECTION_ARR_FILE = "./INFECTION_ARR_"+std::to_string(TIMES)+".txt";   //记录感染后的矩阵结果
std::string FEEDBACK_ARR_FILE = "./FEEDBACK_ARR_"  +std::to_string(TIMES)+".txt";   //记录回溯后的矩阵结果
std::string ROAD_FILE = "./ROAD_"+std::to_string(TIMES)+".txt";         //用来记录最后回溯的路径
std::string BLOCK_INFO_FILE = "./BLOCK_INFO_"+std::to_string(TIMES)+".txt"; //用来记录分块信息
std::vector<std::vector<int>> ROAD_XY;    //记录最后回溯的路径xy坐标
std::string TIME_FILE = "./TIME_"+std::to_string(TIMES)+".txt"; //用来记录时间

bool infection(std::vector<std::vector<int>>&arr,int LeftUpX,int LeftUpY,int RightDownX,int RightDownY,int x,int y) {//递归感染
    if(x < LeftUpX || x > RightDownX || y < LeftUpY || y > RightDownY)return false;  //越界直接返回false
    if(arr[x][y] == INT_MAX){
        cout<<"走到终点:"<<endl;
        cout<<"LeftUpX = "<<LeftUpX<<",LeftUpY = "<<LeftUpY<<",RightDownX = "<<RightDownX<<",RightDownY = "<<RightDownY<<endl;
    }
    if(arr[x][y] != 0 && arr[x][y] != INT_MAX && arr[x][y] != INT_MIN)return false;  //非通路直接返回false
    if(arr[x][y] == INT_MAX) return true;   //走到终点,返回true
    // if(arr[x][y] == INT_MIN) return false;  //走回起点,返回false
    //计算曼哈顿距离
    if(arr[x][y]!=INT_MIN)arr[x][y] = abs(x - SX) + abs(y - SY);            //不是起点时
    bool right = infection(arr,LeftUpX,LeftUpY,RightDownX,RightDownY,x+1,y);//向右
    if (right)return true;  //走到终点就结束
    bool down = infection(arr,LeftUpX,LeftUpY,RightDownX,RightDownY,x,y+1); //向下
    if(down) return true;   //走到终点就结束
    bool left = infection(arr,LeftUpX,LeftUpY,RightDownX,RightDownY,x-1,y); //向左
    if(left) return true;   //走到终点就结束
    bool up = infection(arr,LeftUpX,LeftUpY,RightDownX,RightDownY,x,y-1);   //向上
    return up?true:false;   //走到终点就结束,否则返回false
}

bool infection(std::vector<std::vector<int>>&arr,int LeftUpX,int LeftUpY,int RightDownX,int RightDownY) {    //非递归感染
    for(int i = LeftUpX;i <= RightDownX;++i) {   //直接遍历整个数组进行感染
        for(int j = LeftUpY;j <= RightDownY;++j) {
            if(arr[i][j] == 0)
                arr[i][j] = abs(i - SX) + abs(j - SY);
            else if(arr[i][j] == INT_MAX)return true;   //走到终点返回true
        }
    }
    return false;
}

void Feedback(std::vector<std::vector<int>>&arr) {  //回溯
    int i = EX,j = EY;  //从终点开始 - 找值最小的点走
    int dir = -1;   //方向，-1表示没有路径，0表示向右，1表示向下，2表示向左，3表示向上
    std::vector<int>XY; //存放路径的XY坐标，格式:[x1][y1][x2][y2]...
    while(i != SX || j != SY) {
        XY.push_back(i);    //i,j记录路径
        XY.push_back(j);
        dir = -1;           //初始化方向
        int min = arr[i][j];    //当前值做最小值
        //开始找4个方向的最小值，并修改方向
        if(i+1<arr[0].size()){  //向右走
            if(arr[i+1][j] < min) { //更小的值
                min = arr[i+1][j];
                dir = 0;
            }
        }else if(i-1>=0){       //向左走
            if(arr[i-1][j] < min) {
                min = arr[i-1][j];
                dir = 2;
            }
        }else if(j+1<arr.size()){   //向下走
            if(arr[i][j+1] < min) {
                min = arr[i][j+1];
                dir = 1;
            }
        }else if(j-1>=0){       //向上走
            if(arr[i][j-1] < min) {
                min = arr[i][j-1];
                dir = 3;
            }
        }
        //修改方向
        if(dir == -1) break;    //无路径，结束
        else if(dir == 0) i++;  //向右走
        else if(dir == 1) j++;  //向下走
        else if(dir == 2) i--;  //向左走
        else if(dir == 3) j--;  //向上走
    }
    if(dir != -1){      //说明走到起点
        XY.push_back(SX);
        XY.push_back(SY);
    }
    ROAD_XY.push_back(XY);
}

bool Lee(std::vector<std::vector<int>>&arr,int count,int sx,int sy,int ex,int ey) {  //count用于表示这是第几个矩阵
    SX = sx;    //记录起终点坐标
    SY = sy;
    EX = ex;
    EY = ey;
    TIMES = CreationTimes+count;  //更新矩阵计数
    //矩阵tmp用于执行递归的infection,顺带保存初始矩阵结果
    INIT_ARR_FILE = "./2_INIT_ARR_"+std::to_string(TIMES)+".txt"; //记录初始矩阵结果
    std::ofstream INIT_ARR(INIT_ARR_FILE);  //打开初始矩阵文件
    if(!INIT_ARR.is_open()) {                //文件打开失败
        cout<<"FILE_INIT_ARR_OPEN_ERROR"<<endl;
        exit(-1);
    }
    std::vector<std::vector<int>>tmp(arr.size(),std::vector<int>(arr[0].size(),0));
    for(int i = 0;i < arr.size();i++) {
        for(int j = 0;j < arr[0].size();j++) {
            tmp[i][j] = arr[i][j];
            INIT_ARR<<tmp[i][j]<<"\t";      //写入文件
        }
        INIT_ARR<<endl;                     //换行
    }
    INIT_ARR.close();
    bool isFinally = false;     //标记是否能走到终点
    
    //1.分块
    cout<<"f:Lee_1.开始第"<<count+1<<"个矩阵的分块...\n";
    BLOCK_X = BLOCK_Y = 1;  //初始化为1
    if(arr.size()<=BLOCKYSIZE)  BLOCK_Y = 1;            //Y方向分成1个块
    else while(arr.size()/(++BLOCK_Y)>BLOCKYSIZE);      //找到合适的纵向分块数        
    if(arr[0].size()<=BLOCKXSIZE) BLOCK_X = 1;          //X方向分成1个块
    else while(arr[0].size()/(++BLOCK_X)>BLOCKXSIZE);   //找到合适的横向分块数
    BLOCK_INFO_FILE = "./3_BLOCK_INFO_"+std::to_string(TIMES)+".txt"; //将分块信息写入文件
    std::ofstream BLOCK_INFO(BLOCK_INFO_FILE);
    if(!BLOCK_INFO.is_open()) {
        cout<<"FILE_BLOCK_INFO_OPEN_ERROR"<<endl;
        exit(-1);
    }
    BLOCK_INFO<<"X方向的分块数 = "<<BLOCK_X<<endl;
    BLOCK_INFO<<"Y方向的分块数 = "<<BLOCK_Y<<endl;
    BLOCK_INFO.close();
    cout<<"f:Lee_1.第"<<count+1<<"个矩阵的分块结束...\n";

    //2.从起点开始对每个块做非递归infection
    cout<<"f:Lee_2.开始第"<<count+1<<"个矩阵的非递归infection...\n";
    INFECTION_ARR_FILE = "./4_INFECTION_NON_ARR_"+std::to_string(TIMES)+".txt";   //保存非递归结果
    std::ofstream INFECTION_NON_ARR(INFECTION_ARR_FILE);
    if(!INFECTION_NON_ARR.is_open()) {
        cout<<"FILE_INFECTION_NON_ARR_OPEN_ERROR"<<endl;
        exit(-1);
    }
    auto infection_start_time = std::chrono::steady_clock::now();   //开始时间
    for(int i=0;i<BLOCK_X;++i) {     //X方向的第i个块
        for(int j=0;j<BLOCK_Y;++j) { //Y方向的第j个块
            //2.1计算该分块的左上角和右下角的行y列x值
            int left_up_x = i*BLOCKYSIZE;   //左上x
            int left_up_y = j*BLOCKXSIZE;   //左上y
            int right_down_x = (i+1)*BLOCKYSIZE-1;  //右下x
            int right_down_y = (j+1)*BLOCKXSIZE-1;  //右下y
            right_down_x = right_down_x>=arr[0].size()?arr[0].size()-1:right_down_x;
            right_down_y = right_down_y>=arr.size()?arr.size()-1:right_down_y;
            //2.2对该分块做infection
            infection(arr,left_up_x,left_up_y,right_down_x,right_down_y);   //非递归感染
        }
    }
    auto infection_end_time = std::chrono::steady_clock::now();     //结束时间
    for(int i = 0;i < arr.size();i++) {
        for(int j = 0;j < arr[0].size();j++) {
            INFECTION_NON_ARR<<arr[i][j]<<"\t";      //写入文件
        }
        INFECTION_NON_ARR<<endl;                     //换行
    }
    INFECTION_NON_ARR.close();
    INFECTION_NON_TIME = (std::chrono::duration_cast<std::chrono::nanoseconds>(infection_end_time-infection_start_time)).count();
    cout<<"f:Lee_2.结束第"<<count+1<<"个矩阵的非递归infection...\n";
    
    //3.从起点开始对每个块做递归infection
    cout<<"f:Lee_3.开始第"<<count+1<<"个矩阵的递归infection...\n";
    INFECTION_ARR_FILE = "./4_INFECTION_ARR_"+std::to_string(TIMES)+".txt";   //保存非递归结果到文件
    std::ofstream INFECTION_ARR(INFECTION_ARR_FILE);
    if(!INFECTION_ARR.is_open()) {
        cout<<"FILE_INFECTION_NON_ARR_OPEN_ERROR"<<endl;
        exit(-1);
    }
    infection_start_time = std::chrono::steady_clock::now();   //开始时间
    for(int i=0;i<BLOCK_X;++i) {     //X方向的第i个块
        for(int j=0;j<BLOCK_Y;++j) { //Y方向的第j个块
            //2.1计算该分块的左上角和右下角的行y列x值
            int left_up_x = i*BLOCKYSIZE;   //左上x
            int left_up_y = j*BLOCKXSIZE;   //左上y
            int right_down_x = (i+1)*BLOCKYSIZE-1;  //右下x
            int right_down_y = (j+1)*BLOCKXSIZE-1;  //右下y
            right_down_x = right_down_x>=arr[0].size()?arr[0].size()-1:right_down_x;
            right_down_y = right_down_y>=arr.size()?arr.size()-1:right_down_y;
            //2.2对该分块做infection
            cout<<"第"<<count+1<<"个矩阵的LUX="<<left_up_x<<",LUY="<<left_up_y<<",RDX="<<right_down_x<<",RDY="<<right_down_y<<endl;
            if(infection(arr,left_up_x,left_up_y,right_down_x,right_down_y,left_up_x,left_up_y)) isFinally = true;   //递归感染
        }
    }
    infection_end_time = std::chrono::steady_clock::now();     //结束时间
    for(int i = 0;i < arr.size();i++) {
        for(int j = 0;j < arr[0].size();j++) {
            INFECTION_ARR<<arr[i][j]<<"\t";      //写入文件
        }
        INFECTION_ARR<<endl;                     //换行
    }
    INFECTION_ARR.close();
    INFECTION_TIME = (std::chrono::duration_cast<std::chrono::nanoseconds>(infection_end_time-infection_start_time)).count();
    cout<<"f:Lee_3.结束第"<<count+1<<"个矩阵的递归infection...\n";
    
    //4.回溯
    cout<<"f:Lee_4.开始第"<<count+1<<"个矩阵的回溯...\n";
    auto feedback_time = std::chrono::steady_clock::now();   //开始时间
    if(isFinally) Feedback(arr);
    else cout<<"没有找到通路..."<<endl;
    auto feedback_end_time = std::chrono::steady_clock::now();     //结束时间
    FEEDBACK_TIME = (std::chrono::duration_cast<std::chrono::nanoseconds>(feedback_end_time-feedback_time)).count();
    cout<<"f:Lee_4.结束第"<<count+1<<"个矩阵的回溯...\n";
    
    //5.将所有中间结果记录到文件中
    cout<<"f:Lee_5.开始第"<<count+1<<"个矩阵的记录...\n";
    FEEDBACK_ARR_FILE = "./5_FEEDBACK_ARR_"  +std::to_string(TIMES)+".txt";   //记录回溯矩阵的文件
    std::ofstream FEEDBACK_ARR(FEEDBACK_ARR_FILE);
    if(isFinally){
        if(!FEEDBACK_ARR.is_open()) {
            cout<<"FILE_FEEDBACK_ARR_OPEN_ERROR"<<endl;
            exit(-1);
        }
        for(int i = 0;i < arr.size();i++) {
            for(int j = 0;j < arr[0].size();j++) {
                FEEDBACK_ARR<<arr[i][j]<<"\t";      //写入文件
            }
            FEEDBACK_ARR<<endl;                     //换行
        }
    }else FEEDBACK_ARR<<"未找到通路...\n";
    FEEDBACK_ARR.close();
    ROAD_FILE = "./6_ROAD_"+std::to_string(TIMES)+".txt";   //记录回溯路径的文件
    std::ofstream ROAD(ROAD_FILE);
    if(isFinally){
        if(!ROAD.is_open()) {
            cout<<"FILE_FEEDBACK_ARR_OPEN_ERROR"<<endl;
            exit(-1);
        }
        for(int i = 0;i < ROAD_XY[0].size();i++) 
            ROAD<<"("<<ROAD_XY[0][i]<<","<<ROAD_XY[1][i]<<")"<<endl;
        ROAD<<"一共走了"<<ROAD_XY[0].size()<<"步"<<endl;
    }else ROAD<<"未找到通路...\n";
    ROAD.close();
    TIME_FILE = "./7_TIME_"+std::to_string(TIMES)+".txt"; //记录时间的文件
    std::ofstream TIME(TIME_FILE);
    if(!TIME.is_open()) {
        cout<<"FILE_TIME_OPEN_ERROR"<<endl;
        exit(-1);
    }
    TIME<<"INFECTION_TIME: "<<INFECTION_TIME<<"μs"<<endl;
    TIME<<"FEEDBACK_TIME: "<<FEEDBACK_TIME<<"μs"<<endl;
    TIME<<"TOTAL_TIME: "<<INFECTION_TIME+FEEDBACK_TIME<<"μs"<<endl;
    TIME.close();
    cout<<"f:Lee_5.结束第"<<count+1<<"个矩阵的记录...\n";
    return true;
}

