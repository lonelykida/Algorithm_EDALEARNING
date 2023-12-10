#include "head.hpp"

/*  �����㷨
    1.�ֿ飬���С���60*60
    2.����㿪ʼ��infection,Ҫ��¼���յ��xy���꣬�ֱ���sx,sy,ex,ey��Ҫ��¼ʱ��
    3.���ݣ�ֱ�Ӵ�������յ��ң�Ҫ��¼ʱ��
    4.�������¼���ļ���
*/

//�����Ҫ����
#define BLOCKXSIZE 60   //������xy
#define BLOCKYSIZE 60
int SX,SY,EX,EY;        //���յ�x��y����
int INFECTION_NON_TIME = 0,INFECTION_TIME = 0; //�ǵݹ�͵ݹ�ĸ�Ⱦʱ��
int FEEDBACK_TIME = 0;  //����ʱ��
int TIMES = 0;          //��¼�ڼ�������
int BLOCK_X = 1,BLOCK_Y = 1;    //��¼�������еķֿ���
std::string INIT_ARR_FILE = "./INIT_ARR_"+std::to_string(TIMES)+".txt";             //��¼��ʼ������
std::string INFECTION_ARR_FILE = "./INFECTION_ARR_"+std::to_string(TIMES)+".txt";   //��¼��Ⱦ��ľ�����
std::string FEEDBACK_ARR_FILE = "./FEEDBACK_ARR_"  +std::to_string(TIMES)+".txt";   //��¼���ݺ�ľ�����
std::string ROAD_FILE = "./ROAD_"+std::to_string(TIMES)+".txt";         //������¼�����ݵ�·��
std::string BLOCK_INFO_FILE = "./BLOCK_INFO_"+std::to_string(TIMES)+".txt"; //������¼�ֿ���Ϣ
std::vector<std::vector<int>> ROAD_XY;    //��¼�����ݵ�·��xy����
std::vector<std::vector<int>> TMP_XY;     //��¼������ʱ��Ϣ-FeedBack��
std::string TIME_FILE = "./TIME_"+std::to_string(TIMES)+".txt"; //������¼ʱ��




bool infection(std::vector<std::vector<int>>&arr,int LeftUpX,int LeftUpY,int RightDownX,int RightDownY,int x,int y) {//�ݹ��Ⱦ
    if(x<LeftUpX || x>RightDownX || y<LeftUpY || y>RightDownY)return false; //Խ���
    if(arr[x][y]>0 && arr[x][y] != INT_MAX)return false;                    //�Ѿ�����Ⱦ
    if(arr[x][y] == -1)return false;        //�ϰ�
    if(arr[x][y] == INT_MAX)return true;    //�ߵ��յ㣬����true
    //���������������پ���
    
    if(arr[x][y]!=INT_MIN)arr[x][y] = abs(x - SX) + abs(y - SY);
    bool right = infection(arr,LeftUpX,LeftUpY,RightDownX,RightDownY,x+1,y);//����
    // if (right)return true;  //�ߵ��յ�ͽ���
    bool down = infection(arr,LeftUpX,LeftUpY,RightDownX,RightDownY,x,y+1); //����
    // if(down) return true;   //�ߵ��յ�ͽ���
    bool left = infection(arr,LeftUpX,LeftUpY,RightDownX,RightDownY,x-1,y); //����
    // if(left) return true;   //�ߵ��յ�ͽ���
    bool up = infection(arr,LeftUpX,LeftUpY,RightDownX,RightDownY,x,y-1);   //����
    return up||right||down||left?true:false;   //�ߵ��յ�ͽ���,���򷵻�false
}

bool infection(std::vector<std::vector<int>>&arr,int LeftUpX,int LeftUpY,int RightDownX,int RightDownY) {    //�ǵݹ��Ⱦ
    for(int i = LeftUpX;i <= RightDownX;++i) {   //ֱ�ӱ�������������и�Ⱦ
        for(int j = LeftUpY;j <= RightDownY;++j) {
            if(arr[i][j] == 0)
                arr[i][j] = abs(i - SX) + abs(j - SY);
            else if(arr[i][j] == INT_MAX)return true;   //�ߵ��յ㷵��true
        }
    }
    return false;
}

void Feedback(std::vector<std::vector<int>>&arr) {  //����
    TMP_XY.clear();     //����ռ�
    std::vector<int>x,y;
    int i = EX,j = EY;  //���յ㿪ʼ
    int min = arr[i][j];
    int dir = -1;   //�÷���λ
    while((i != SX && j != SY)) {
        arr[i][j] = -2; //�÷���λ
        //2.���������ҵķ���dir
        if(i-1 >=0) {  //��
            if(arr[i-1][j] < min && arr[i-1][j] != -1 && arr[i-1][j] != -2) {
                min = arr[i-1][j];
                dir = 0;
            }
        }
        if(j-1 >= 0) {  //��
            if(arr[i][j-1] < min && arr[i][j-1] != -1 && arr[i][j-1] != -2) {
                min = arr[i][j-1];
                dir = 1;
            }
        }
        if(i+1 < arr.size()) {  //��
            if(arr[i+1][j] < min && arr[i+1][j] != -1 && arr[i+1][j] != -2) {
                min = arr[i+1][j];
                dir = 2;
            }
        }
        if(j+1 < arr[0].size()) {  //��
            if(arr[i][j+1] < min && arr[i][j+1] != -1 && arr[i][j+1] != -2) {
                min = arr[i][j+1];
                dir = 3;
            }
        }
        //3.����dir�޸ĵ�ǰ����
        if(dir!=-1){    //�ҵ�����
            x.push_back(i);
            y.push_back(j);
            switch(dir) {
                case 0:
                    i--;
                    break;
                case 1:
                    j--;
                    break;
                case 2:
                    i++;
                    break;
                case 3:
                    j++;
                    break;
            }
        }else{          //δ�ҵ�����
            if(x.empty()){
                cout<<"XY����ջ��,δ�ҵ�����·��!\n";
                break;
            }
            i = x.back();
            j = y.back();
            x.pop_back();
            y.pop_back();
            min = INT_MAX;
        }
        dir = -1;       //��ʼ��dir
    }
    if(!x.empty()){
        x.push_back(SX);
        y.push_back(SY);
    }
    TMP_XY.push_back(x);
    TMP_XY.push_back(y);
}

bool Lee(std::vector<std::vector<int>>&arr,int count,int sx,int sy,int ex,int ey) {  //count���ڱ�ʾ���ǵڼ�������
    SX = sx;    //��¼���յ�����
    SY = sy;
    EX = ex;
    EY = ey;
    TIMES = CreationTimes+count;  //���¾������
    //����tmp����ִ�еݹ��infection,˳�������ʼ������
    INIT_ARR_FILE = "./2_INIT_ARR_"+std::to_string(TIMES)+".txt"; //��¼��ʼ������
    std::ofstream INIT_ARR(INIT_ARR_FILE);  //�򿪳�ʼ�����ļ�
    if(!INIT_ARR.is_open()) {                //�ļ���ʧ��
        cout<<"FILE_INIT_ARR_OPEN_ERROR"<<endl;
        exit(-1);
    }
    std::vector<std::vector<int>>tmp(arr.size(),std::vector<int>(arr[0].size(),0));
    INIT_ARR<<"tmp����:"<<endl;
    for(int i = 0;i < tmp.size();i++) {
        for(int j = 0;j < tmp[0].size();j++) {
            tmp[i][j] = arr[i][j];
            INIT_ARR<<tmp[i][j]<<"\t";      //д���ļ�
        }
        INIT_ARR<<endl;                     //����
    }
    INIT_ARR<<"\narr����:"<<endl;
    for(int i = 0;i < arr.size();i++) {
        for(int j = 0;j < arr[0].size();j++) {
            INIT_ARR<<tmp[i][j]<<"\t";      //д���ļ�
        }
        INIT_ARR<<endl;                     //����
    }
    INIT_ARR.close();
    bool isFinally = false;     //����Ƿ����ߵ��յ�

    //1.�ֿ�
    cout<<"f:Lee_1.��ʼ��"<<count+1<<"������ķֿ�...\n";
    BLOCK_X = BLOCK_Y = 1;  //��ʼ��Ϊ1
    if(arr.size()<=BLOCKYSIZE)  BLOCK_Y = 1;            //Y����ֳ�1����
    else while(arr.size()/(++BLOCK_Y)>BLOCKYSIZE);      //�ҵ����ʵ�����ֿ���        
    if(arr[0].size()<=BLOCKXSIZE) BLOCK_X = 1;          //X����ֳ�1����
    else while(arr[0].size()/(++BLOCK_X)>BLOCKXSIZE);   //�ҵ����ʵĺ���ֿ���
    BLOCK_INFO_FILE = "./3_BLOCK_INFO_"+std::to_string(TIMES)+".txt"; //���ֿ���Ϣд���ļ�
    std::ofstream BLOCK_INFO(BLOCK_INFO_FILE);
    if(!BLOCK_INFO.is_open()) {
        cout<<"FILE_BLOCK_INFO_OPEN_ERROR"<<endl;
        exit(-1);
    }
    BLOCK_INFO<<"X����ķֿ��� = "<<BLOCK_X<<endl;
    BLOCK_INFO<<"Y����ķֿ��� = "<<BLOCK_Y<<endl;
    BLOCK_INFO.close();
    cout<<"f:Lee_1.��"<<count+1<<"������ķֿ����...\n";

    //2.����㿪ʼ��ÿ�������ǵݹ�infection
    cout<<"f:Lee_2.��ʼ��"<<count+1<<"������ķǵݹ�infection...\n";
    INFECTION_ARR_FILE = "./4_INFECTION_NON_ARR_"+std::to_string(TIMES)+".txt";   //����ǵݹ���
    std::ofstream INFECTION_NON_ARR(INFECTION_ARR_FILE);
    if(!INFECTION_NON_ARR.is_open()) {
        cout<<"FILE_INFECTION_NON_ARR_OPEN_ERROR"<<endl;
        exit(-1);
    }
    auto infection_start_time = std::chrono::steady_clock::now();   //��ʼʱ��
    for(int i=0;i<BLOCK_X;++i) {     //X����ĵ�i����
        for(int j=0;j<BLOCK_Y;++j) { //Y����ĵ�j����
            //2.1����÷ֿ�����ϽǺ����½ǵ���y��xֵ
            int left_up_x = i*BLOCKYSIZE;   //����x
            int left_up_y = j*BLOCKXSIZE;   //����y
            int right_down_x = (i+1)*BLOCKYSIZE-1;  //����x
            int right_down_y = (j+1)*BLOCKXSIZE-1;  //����y
            right_down_x = right_down_x>=arr[0].size()?arr[0].size()-1:right_down_x;
            right_down_y = right_down_y>=arr.size()?arr.size()-1:right_down_y;
            //2.2�Ը÷ֿ���infection
            infection(arr,left_up_x,left_up_y,right_down_x,right_down_y);   //�ǵݹ��Ⱦ
        }
    }
    auto infection_end_time = std::chrono::steady_clock::now();     //����ʱ��
    for(int i = 0;i < arr.size();i++) {
        for(int j = 0;j < arr[0].size();j++) {
            INFECTION_NON_ARR<<arr[i][j]<<"\t";      //д���ļ�
        }
        INFECTION_NON_ARR<<endl;                     //����
    }
    INFECTION_NON_ARR.close();
    INFECTION_NON_TIME = (std::chrono::duration_cast<std::chrono::nanoseconds>(infection_end_time-infection_start_time)).count();
    cout<<"f:Lee_2.������"<<count+1<<"������ķǵݹ�infection...\n";
    
    //3.����㿪ʼ��ÿ�������ݹ�infection
    cout<<"f:Lee_3.��ʼ��"<<count+1<<"������ĵݹ�infection...\n";
    INFECTION_ARR_FILE = "./4_INFECTION_ARR_"+std::to_string(TIMES)+".txt";   //����ݹ������ļ�
    std::ofstream INFECTION_ARR(INFECTION_ARR_FILE);
    if(!INFECTION_ARR.is_open()) {
        cout<<"FILE_INFECTION_NON_ARR_OPEN_ERROR"<<endl;
        exit(-1);
    }
    infection_start_time = std::chrono::steady_clock::now();   //��ʼʱ��
    for(int i=0;i<BLOCK_X;++i) {     //X����ĵ�i����
        for(int j=0;j<BLOCK_Y;++j) { //Y����ĵ�j����
            //2.1����÷ֿ�����ϽǺ����½ǵ���y��xֵ
            int left_up_x = i*BLOCKYSIZE;   //����x
            int left_up_y = j*BLOCKXSIZE;   //����y
            int right_down_x = (i+1)*BLOCKYSIZE-1;  //����x
            int right_down_y = (j+1)*BLOCKXSIZE-1;  //����y
            right_down_x = right_down_x>=tmp[0].size()?tmp[0].size()-1:right_down_x;
            right_down_y = right_down_y>=tmp.size()?tmp.size()-1:right_down_y;
            //2.2�Ը÷ֿ���infection
            // cout<<"��"<<count+1<<"�������LUX="<<left_up_x<<",LUY="<<left_up_y<<",RDX="<<right_down_x<<",RDY="<<right_down_y<<endl;
            if(infection(tmp,left_up_x,left_up_y,right_down_x,right_down_y,left_up_x,left_up_y)) isFinally = true;   //�ݹ��Ⱦ
        }
    }
    infection_end_time = std::chrono::steady_clock::now();     //����ʱ��
    for(int i = 0;i < tmp.size();i++) {
        for(int j = 0;j < tmp[0].size();j++) {
            INFECTION_ARR<<tmp[i][j]<<"\t";      //д���ļ�
        }
        INFECTION_ARR<<endl;                     //����
    }
    INFECTION_ARR.close();
    INFECTION_TIME = (std::chrono::duration_cast<std::chrono::nanoseconds>(infection_end_time-infection_start_time)).count();
    cout<<"f:Lee_3.������"<<count+1<<"������ĵݹ�infection...\n";
    
    //4.����
    cout<<"f:Lee_4.��ʼ��"<<count+1<<"������Ļ���...\n";
    auto feedback_time = std::chrono::steady_clock::now();   //��ʼʱ��
    if(isFinally) Feedback(tmp);
    else cout<<"û���ҵ�ͨ·..."<<endl;
    auto feedback_end_time = std::chrono::steady_clock::now();     //����ʱ��
    FEEDBACK_TIME = (std::chrono::duration_cast<std::chrono::nanoseconds>(feedback_end_time-feedback_time)).count();
    cout<<"f:Lee_4.������"<<count+1<<"������Ļ���...\n";
    
    //5.�������м�����¼���ļ���
    cout<<"f:Lee_5.��ʼ��"<<count+1<<"������ļ�¼...\n";
    cout<<"f:Lee_5.1.��ʼ��¼���ݾ���...\n";
    FEEDBACK_ARR_FILE = "./5_FEEDBACK_ARR_"  +std::to_string(TIMES)+".txt";   //��¼���ݾ�����ļ�
    std::ofstream FEEDBACK_ARR(FEEDBACK_ARR_FILE);
    if(isFinally){
        if(!FEEDBACK_ARR.is_open()) {
            cout<<"FILE_FEEDBACK_ARR_OPEN_ERROR"<<endl;
            exit(-1);
        }
        for(int i = 0;i < tmp.size();i++) {
            for(int j = 0;j < tmp[0].size();j++) {
                FEEDBACK_ARR<<tmp[i][j]<<"\t";      //д���ļ�
            }
            FEEDBACK_ARR<<endl;                     //����
        }
    }else FEEDBACK_ARR<<"δ�ҵ�ͨ·...\n";
    FEEDBACK_ARR.close();
    cout<<"f:Lee_5.1.������¼���ݾ���...\n";
    cout<<"f:Lee_5.2.��ʼ��¼����·��...\n";
    ROAD_FILE = "./6_ROAD_"+std::to_string(TIMES)+".txt";   //��¼����·�����ļ�
    
    cout<<endl;
    std::ofstream ROAD(ROAD_FILE);
    std::vector<int>tmpxy;
    for(int i = 0;i < TMP_XY[0].size();i++){
        tmpxy.push_back(TMP_XY[0][i]);
        tmpxy.push_back(TMP_XY[1][i]);
    }
    ROAD_XY.push_back(tmpxy);
    if(isFinally){
        if(!ROAD.is_open()) {
            cout<<"FILE_FEEDBACK_ARR_OPEN_ERROR"<<endl;
            exit(-1);
        }
        for(int i = 0;i < ROAD_XY[count].size();i+=2) 
            ROAD<<"("<<ROAD_XY[count][i]<<","<<ROAD_XY[count][i+1]<<")"<<endl;
        ROAD<<"һ������"<<ROAD_XY[count].size()<<"��"<<endl;
    }else ROAD<<"δ�ҵ�ͨ·...\n";
    cout<<"������\n";
    ROAD.close();
    cout<<"f:Lee_5.2.������¼����·��...\n";
    cout<<"f:Lee_5.3.��ʼ��¼ʱ��...\n";
    TIME_FILE = "./7_TIME_"+std::to_string(TIMES)+".txt"; //��¼ʱ����ļ�
    std::ofstream TIME(TIME_FILE);
    if(!TIME.is_open()) {
        cout<<"FILE_TIME_OPEN_ERROR"<<endl;
        exit(-1);
    }
    TIME<<"INFECTION_TIME: "<<INFECTION_TIME<<"ns"<<endl;
    TIME<<"FEEDBACK_TIME: "<<FEEDBACK_TIME<<"ns"<<endl;
    TIME<<"TOTAL_TIME: "<<INFECTION_TIME+FEEDBACK_TIME<<"ns"<<endl;
    TIME.close();
    cout<<"f:Lee_5.3.������¼ʱ��...\n";
    cout<<"f:Lee_5.������"<<count+1<<"������ļ�¼...\n";
    return true;
}

