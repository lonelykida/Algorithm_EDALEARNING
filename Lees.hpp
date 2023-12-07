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

bool infection(std::vector<std::vector<int>>&arr,int LeftUpX,int LeftUpY,int RightDownX,int RightDownY,int x,int y){//�ݹ��Ⱦ
    if(x < LeftUpX || x > RightDownX || y < LeftUpY || y > RightDownY)return false;  //Խ��ֱ�ӷ���false
    if(arr[x][y] != 0 && arr[x][y] != INT_MAX && arr[x][y] != INT_MIN)return false;  //��ͨ·ֱ�ӷ���false
    if(arr[x][y] == INT_MAX) return true;   //�ߵ��յ�,����true
    if(arr[x][y] == INT_MIN) return false;  //�߻����,����false
    //���������پ���
    arr[x][y] = abs(x - SX) + abs(y - SY);
    bool right = infection(arr,LeftUpX,LeftUpY,RightDownX,RightDownY,x+1,y);//����
    if (right)return true;  //�ߵ��յ�ͽ���
    bool down = infection(arr,LeftUpX,LeftUpY,RightDownX,RightDownY,x,y+1); //����
    if(down) return true;   //�ߵ��յ�ͽ���
    bool left = infection(arr,LeftUpX,LeftUpY,RightDownX,RightDownY,x-1,y); //����
    if(left) return true;   //�ߵ��յ�ͽ���
    bool up = infection(arr,LeftUpX,LeftUpY,RightDownX,RightDownY,x,y-1);   //����
    return up?true:false;   //�ߵ��յ�ͽ���,���򷵻�false
}

bool infection(std::vector<std::vector<int>>&arr,int LeftUpX,int LeftUpY,int RightDownX,int RightDownY){    //�ǵݹ��Ⱦ
    for(int i = LeftUpX;i <= RightDownX;++i){   //ֱ�ӱ�������������и�Ⱦ
        for(int j = LeftUpY;j <= RightDownY;++j){
            if(arr[i][j] == 0)
                arr[i][j] = abs(i - SX) + abs(j - SY);
            else if(arr[i][j] == INT_MAX)return true;   //�ߵ��յ㷵��true
        }
    }
}

bool Lee(std::vector<std::vector<int>>&arr,int count,int sx,int sy,int ex,int ey){  //count���ڱ�ʾ���ǵڼ�������
    SX = sx;    //��¼���յ�����
    SY = sy;
    EX = ex;
    EY = ey;
    TIMES = count;  //���¾������
    //����tmp����ִ�еݹ��infection,˳�������ʼ������
    INIT_ARR_FILE = "./INIT_ARR_"+std::to_string(TIMES)+".txt"; //��¼��ʼ������
    std::ofstream INIT_ARR(INIT_ARR_FILE);  //�򿪳�ʼ�����ļ�
    if(!INIT_ARR.is_open()){                //�ļ���ʧ��
        cout<<"FILE_INIT_ARR_OPEN_ERROR"<<endl;
        exit(-1);
    }
    std::vector<std::vector<int>>tmp(arr.size(),std::vector<int>(arr[0].size(),0));
    for(int i = 0;i < arr.size();i++){
        for(int j = 0;j < arr[0].size();j++){
            tmp[i][j] = arr[i][j];
            INIT_ARR<<tmp[i][j]<<"\t";      //д���ļ�
        }
        INIT_ARR<<endl;                     //����
    }
    INIT_ARR.close();
    
    //1.�ֿ�
    BLOCK_X = BLOCK_Y = 1;  //��ʼ��Ϊ1
    if(arr.size()<=BLOCKYSIZE)  BLOCK_Y = 1;            //Y����ֳ�1����
    else while(arr.size()/(++BLOCK_Y)>BLOCKYSIZE);      //�ҵ����ʵ�����ֿ���        
    if(arr[0].size()<=BLOCKXSIZE) BLOCK_X = 1;          //X����ֳ�1����
    else while(arr[0].size()/(++BLOCK_X)>BLOCKXSIZE);   //�ҵ����ʵĺ���ֿ���
    BLOCK_INFO_FILE = "./BLOCK_INFO_"+std::to_string(TIMES)+".txt"; //���ֿ���Ϣд���ļ�
    std::ofstream BLOCK_INFO(BLOCK_INFO_FILE);
    if(!BLOCK_INFO.is_open()){
        cout<<"FILE_BLOCK_INFO_OPEN_ERROR"<<endl;
        exit(-1);
    }
    BLOCK_INFO<<"X����ķֿ��� = "<<BLOCK_X<<endl;
    BLOCK_INFO<<"Y����ķֿ��� = "<<BLOCK_Y<<endl;
    BLOCK_INFO.close();

    //2.����㿪ʼ��ÿ�������ǵݹ�infection
    INFECTION_ARR_FILE = "./INFECTION_NON_ARR_"+std::to_string(TIMES)+".txt";   //����ǵݹ���
    std::ofstream INFECTION_NON_ARR(INFECTION_ARR_FILE);
    if(!INFECTION_NON_ARR.is_open()){
        cout<<"FILE_INFECTION_NON_ARR_OPEN_ERROR"<<endl;
        exit(-1);
    }
    auto infection_start_time = std::chrono::steady_clock::now();   //��ʼʱ��
    for(int i=0;i<BLOCK_X;++i){     //X����ĵ�i����
        for(int j=0;j<BLOCK_Y;++j){ //Y����ĵ�j����
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
    for(int i = 0;i < arr.size();i++){
        for(int j = 0;j < arr[0].size();j++){
            INFECTION_NON_ARR<<arr[i][j]<<"\t";      //д���ļ�
        }
        INFECTION_NON_ARR<<endl;                     //����
    }
    INFECTION_NON_ARR.close();
    INFECTION_NON_TIME = (std::chrono::duration_cast<std::chrono::milliseconds>(infection_end_time-infection_start_time)).count();
    
    //3.����㿪ʼ��ÿ�������ݹ�infection
    INFECTION_ARR_FILE = "./INFECTION_ARR_"+std::to_string(TIMES)+".txt";   //����ǵݹ���
    std::ofstream INFECTION_ARR(INFECTION_ARR_FILE);
    if(!INFECTION_ARR.is_open()){
        cout<<"FILE_INFECTION_NON_ARR_OPEN_ERROR"<<endl;
        exit(-1);
    }
    infection_start_time = std::chrono::steady_clock::now();   //��ʼʱ��
    for(int i=0;i<BLOCK_X;++i){     //X����ĵ�i����
        for(int j=0;j<BLOCK_Y;++j){ //Y����ĵ�j����
            //2.1����÷ֿ�����ϽǺ����½ǵ���y��xֵ
            int left_up_x = i*BLOCKYSIZE;   //����x
            int left_up_y = j*BLOCKXSIZE;   //����y
            int right_down_x = (i+1)*BLOCKYSIZE-1;  //����x
            int right_down_y = (j+1)*BLOCKXSIZE-1;  //����y
            right_down_x = right_down_x>=arr[0].size()?arr[0].size()-1:right_down_x;
            right_down_y = right_down_y>=arr.size()?arr.size()-1:right_down_y;
            //2.2�Ը÷ֿ���infection
            infection(arr,left_up_x,left_up_y,right_down_x,right_down_y,left_up_x,left_up_y);   //�ݹ��Ⱦ
        }
    }
    infection_end_time = std::chrono::steady_clock::now();     //����ʱ��
    for(int i = 0;i < arr.size();i++){
        for(int j = 0;j < arr[0].size();j++){
            INFECTION_ARR<<arr[i][j]<<"\t";      //д���ļ�
        }
        INFECTION_ARR<<endl;                     //����
    }
    INFECTION_ARR.close();
    INFECTION_TIME = (std::chrono::duration_cast<std::chrono::milliseconds>(infection_end_time-infection_start_time)).count();
    
    //4.����
    auto feedback_time = std::chrono::steady_clock::now();   //��ʼʱ��
    //����
    auto feedback_end_time = std::chrono::steady_clock::now();     //����ʱ��
    FEEDBACK_TIME = (std::chrono::duration_cast<std::chrono::milliseconds>(feedback_end_time-feedback_time)).count();
    
    //5.�������м�����¼���ļ���


    return true;
}




