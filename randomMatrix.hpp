/*
    生成随机大小的矩阵
*/
#include "head.hpp"

std::vector<std::vector<std::vector<int>>> MATRIX;  //存储二维矩阵
std::vector<int> ROW;   //存储每个矩阵的行数
std::vector<int> COL;   //存储每个矩阵的列数
std::vector<int> OBS;   //存储每个矩阵障碍的个数
std::vector<int> OBSLEN;//存储每个矩阵障碍的最大长度
std::vector<std::vector<int>>OBSINFO;   //用一个二维数组存储每个矩阵障碍的信息
std::vector<std::vector<int>> STARTANDEND;           //存储起终点坐标 sx sy ex ey
/*
    OBSINFO存储矩阵的信息如下格式，其中每一行表示一个矩阵:  //4个一组
        [障碍起始X坐标][障碍起始Y坐标][障碍方向][障碍长度][障碍起始X坐标][障碍起始Y坐标][障碍方向][障碍长度]... //第一个矩阵的信息
        [障碍起始X坐标][障碍起始Y坐标][障碍方向][障碍长度][障碍起始X坐标][障碍起始Y坐标][障碍方向][障碍长度]... //第二个矩阵的信息
        ...
    STARTANDEND存储矩阵的起终点，其中每一行表示一个矩阵:
        [矩阵起点X坐标][矩阵起点Y坐标][矩阵终点X坐标][矩阵终点Y坐标]
        [矩阵起点X坐标][矩阵起点Y坐标][矩阵终点X坐标][矩阵终点Y坐标]
        ...
*/

/*
    //1.输入矩阵个数及每个矩阵需要的行数、列数、障碍个数、障碍最大长度
    //2.生成每个矩阵
        //2.1.生成矩阵
        //2.2.生成指定个数随机长度的障碍，并记录障碍起始的X\Y坐标及横竖方向,将障碍插入指定矩阵
*/
void randomMatrixCreate() { //生成随机矩阵
    int n = 0;              //要生成的矩阵个数
    int row = 0,col = 0;    //矩阵的行数和列数
    int obs = 0,obslen = 0; //障碍个数和障碍最大长度
    bool flag = false;      //所有矩阵是否使用相同信息 - 默认否
    int sx,sy,ex,ey;        //起终点坐标
    //1.输入信息
    cout<<"请输入要生成的矩阵个数:";
    cin>>n;
    cout<<"是否使用相同的矩阵信息(0-否/1-是):";
    cin>>flag;
    if(flag){   //使用相同矩阵信息
        cout<<"输入矩阵行数、列数、障碍个数及障碍最大长度,每个参数之间以空格隔开:";
        cin>>row>>col>>obs>>obslen;
        for(int i = 0;i < n;++i) {  //存储n次
            ROW.push_back(row);
            COL.push_back(col);
            OBS.push_back(obs);
            OBSLEN.push_back(obslen);
        }
    }else{      //使用不同矩阵信息
        for(int i = 0;i < n;++i){
            cout<<"输入第"<<i+1<<"个矩阵行数、列数、障碍个数及障碍最大长度,每个参数之间以空格隔开:";
            cin>>row>>col>>obs>>obslen;
            ROW.push_back(row);
            COL.push_back(col);
            OBS.push_back(obs);
            OBSLEN.push_back(obslen);
        }
    }
    //输入起终点信息
    flag = false;
    sx = sy = ex = ey = 0;
    cout<<"是否使用相同的起终点信息(0-否/1-是):";
    cin>>flag;
    if(flag){   //使用相同的起终点信息
        cout<<"输入起终点的X、Y坐标,每个坐标用空格隔开,如:sx sy ex ey:"<<endl;
        cin>>sx>>sy>>ex>>ey;
        std::vector<int>tmp;
        tmp.push_back(sx);
        tmp.push_back(sy);
        tmp.push_back(ex);
        tmp.push_back(ey);
        for(int i = 0;i < n;++i) STARTANDEND.push_back(tmp);
    }else{      //使用不同的起终点信息
        for(int i = 0;i < n;++i){
            cout<<"输入第"<<i+1<<"个矩阵的起终点的X、Y坐标,每个坐标用空格隔开,如:sx sy ex ey:"<<endl;
            cin>>sx>>sy>>ex>>ey;
            std::vector<int>tmp;
            tmp.push_back(sx);
            tmp.push_back(sy);
            tmp.push_back(ex);
            tmp.push_back(ey);
            STARTANDEND.push_back(tmp);
        }
    }
    //2.1.生成矩阵
    if(flag){   //所有矩阵信息相同
        std::vector<std::vector<int>>arr(row,std::vector<int>(col,0));
        for(int i = 0;i < n;++i) MATRIX.push_back(arr); //存储n次
    }else{      //不同矩阵信息
        for(int i = 0;i < n;++i) {
            std::vector<std::vector<int>>arr(ROW[i],std::vector<int>(COL[i],0));
            MATRIX.push_back(arr);
        }
    }
    //起终点数值设置
    for(int i = 0;i < n;i++){
        MATRIX[i][STARTANDEND[i][0]][STARTANDEND[i][1]] = INT_MIN;  //起点设置最小值
        MATRIX[i][STARTANDEND[i][2]][STARTANDEND[i][3]] = INT_MAX;  //终点设置最大值
    }
    //2.2.障碍生成
    //先构建随机数环境
    srand(time(0));     //置随机数种子
    for(int i = 0;i < n;++i) {  //对每个矩阵，生成障碍
    std::mt19937 obsx(static_cast<unsigned int>(time(nullptr)));    //随机X坐标
    std::mt19937 obsy(static_cast<unsigned int>(time(nullptr)));    //随机Y坐标
    std::mt19937 obsdrc(static_cast<unsigned int>(time(nullptr)));  //随机方向
    std::mt19937 obsl(static_cast<unsigned int>(time(nullptr)));    //随机长度
    std::uniform_int_distribution<unsigned int> Y(0,ROW[i]-1);      //生成随机Y
    std::uniform_int_distribution<unsigned int> X(0,COL[i]-1);      //生成随机X
    std::uniform_int_distribution<unsigned int> D(0,1);             //生成随机方向 0：横 1：竖
    std::uniform_int_distribution<unsigned int> L(0,OBSLEN[i]-1);   //生成随机长度
        std::vector<int> obsinfo;
        OBSINFO.push_back(obsinfo);
        for(int j = 0;j < OBS[i];++j) {  //生成OBS[i]个障碍
            //生成障碍的各种信息
            int x = (X(obsx)*rand()+rand())%COL[i],     //生成障碍所在列数
                y = (Y(obsy)*rand()+rand())%ROW[i],     //生成障碍所在行数
                drc = (D(obsdrc)*rand()+rand()) %2,     //生成障碍方向
                len = (L(obsl)*rand()+rand())%OBSLEN[i];//生成障碍长度
            if(drc) if(y + len > ROW[i]) len = ROW[i] - y;  //垂直方向超过时，修正长度
            if(!drc)if(x + len > COL[i]) len = COL[i] - x;  //水平方向超过时，修正长度
            //保存障碍信息
            OBSINFO[i].push_back(x);
            OBSINFO[i].push_back(y);
            OBSINFO[i].push_back(drc);
            OBSINFO[i].push_back(len);
            //布置障碍
            if(drc) for(int ix = x,jy = y;ix < x + len && ix<COL[i];++ix) MATRIX[i][ix][jy] = -1;   //横向
            else    for(int ix = x,jy = y;jy < y + len && jy<ROW[i];++jy) MATRIX[i][ix][jy] = -1;   //纵向
        }
    }
    //保存各个矩阵的障碍信息
    cout<<"f:randomMatrix.hpp_开始保存矩阵障碍及起终点信息..."<<endl;
    std::vector<std::string> OBSINFO_FILE;
    std::vector<std::string> STARTANDEND_FILE;
    for(int i = 0;i < n;++i){   //生成每个矩阵OBS文件的保存路径
        std::string fname = "./0_OBS_INFO_" + std::to_string(CreationTimes+i) + ".txt";
        std::string sefile = "./1_STARTANDEND_INFO_" + std::to_string(CreationTimes+i) + ".txt";
        OBSINFO_FILE.push_back(fname);
        STARTANDEND_FILE.push_back(sefile);
    }
    for(int i = 0;i < n;++i){   //保存每个矩阵OBS文件的障碍信息
        std::ofstream fout(OBSINFO_FILE[i]);    //打开OBS文件
        std::ofstream fse(STARTANDEND_FILE[i]); //打开起终点文件
        if(!fout.is_open() || !fse.is_open()) {
            std::cout<<"文件打开失败"<<std::endl;
            return;
        }
        for(int j = 0;j < OBS[i];++j)  //保存第i个矩阵的OBS信息
            fout<<"起点X:"<<OBSINFO[i][j*4]<<"\t,起点Y:"<<OBSINFO[i][j*4+1]<<"\t,方向(0横1竖):"<<OBSINFO[i][j*4+2]<<"\t,长度:"<<OBSINFO[i][j*4+3]<<std::endl;
        fse<<STARTANDEND[i][0]<<","<<STARTANDEND[i][1]<<"\t"<<STARTANDEND[i][2]<<","<<STARTANDEND[i][3]<<std::endl;
        fout.close();
        fse.close();
    }
    cout<<"f:randomMatrix.hpp_矩阵障碍及起终点信息保存完毕..."<<endl;
}

