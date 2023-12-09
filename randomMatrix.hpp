/*
    ���������С�ľ���
*/
#include "head.hpp"

std::vector<std::vector<std::vector<int>>> MATRIX;  //�洢��ά����
std::vector<int> ROW;   //�洢ÿ�����������
std::vector<int> COL;   //�洢ÿ�����������
std::vector<int> OBS;   //�洢ÿ�������ϰ��ĸ���
std::vector<int> OBSLEN;//�洢ÿ�������ϰ�����󳤶�
std::vector<std::vector<int>>OBSINFO;   //��һ����ά����洢ÿ�������ϰ�����Ϣ
std::vector<std::vector<int>> STARTANDEND;           //�洢���յ����� sx sy ex ey
/*
    OBSINFO�洢�������Ϣ���¸�ʽ������ÿһ�б�ʾһ������:  //4��һ��
        [�ϰ���ʼX����][�ϰ���ʼY����][�ϰ�����][�ϰ�����][�ϰ���ʼX����][�ϰ���ʼY����][�ϰ�����][�ϰ�����]... //��һ���������Ϣ
        [�ϰ���ʼX����][�ϰ���ʼY����][�ϰ�����][�ϰ�����][�ϰ���ʼX����][�ϰ���ʼY����][�ϰ�����][�ϰ�����]... //�ڶ����������Ϣ
        ...
    STARTANDEND�洢��������յ㣬����ÿһ�б�ʾһ������:
        [�������X����][�������Y����][�����յ�X����][�����յ�Y����]
        [�������X����][�������Y����][�����յ�X����][�����յ�Y����]
        ...
*/

/*
    //1.������������ÿ��������Ҫ���������������ϰ��������ϰ���󳤶�
    //2.����ÿ������
        //2.1.���ɾ���
        //2.2.����ָ������������ȵ��ϰ�������¼�ϰ���ʼ��X\Y���꼰��������,���ϰ�����ָ������
*/
void randomMatrixCreate() { //�����������
    int n = 0;              //Ҫ���ɵľ������
    int row = 0,col = 0;    //���������������
    int obs = 0,obslen = 0; //�ϰ��������ϰ���󳤶�
    bool flag = false;      //���о����Ƿ�ʹ����ͬ��Ϣ - Ĭ�Ϸ�
    int sx,sy,ex,ey;        //���յ�����
    //1.������Ϣ
    cout<<"������Ҫ���ɵľ������:";
    cin>>n;
    cout<<"�Ƿ�ʹ����ͬ�ľ�����Ϣ(0-��/1-��):";
    cin>>flag;
    if(flag){   //ʹ����ͬ������Ϣ
        cout<<"��������������������ϰ��������ϰ���󳤶�,ÿ������֮���Կո����:";
        cin>>row>>col>>obs>>obslen;
        for(int i = 0;i < n;++i) {  //�洢n��
            ROW.push_back(row);
            COL.push_back(col);
            OBS.push_back(obs);
            OBSLEN.push_back(obslen);
        }
    }else{      //ʹ�ò�ͬ������Ϣ
        for(int i = 0;i < n;++i){
            cout<<"�����"<<i+1<<"�������������������ϰ��������ϰ���󳤶�,ÿ������֮���Կո����:";
            cin>>row>>col>>obs>>obslen;
            ROW.push_back(row);
            COL.push_back(col);
            OBS.push_back(obs);
            OBSLEN.push_back(obslen);
        }
    }
    //�������յ���Ϣ
    flag = false;
    sx = sy = ex = ey = 0;
    cout<<"�Ƿ�ʹ����ͬ�����յ���Ϣ(0-��/1-��):";
    cin>>flag;
    if(flag){   //ʹ����ͬ�����յ���Ϣ
        cout<<"�������յ��X��Y����,ÿ�������ÿո����,��:sx sy ex ey:"<<endl;
        cin>>sx>>sy>>ex>>ey;
        std::vector<int>tmp;
        tmp.push_back(sx);
        tmp.push_back(sy);
        tmp.push_back(ex);
        tmp.push_back(ey);
        for(int i = 0;i < n;++i) STARTANDEND.push_back(tmp);
    }else{      //ʹ�ò�ͬ�����յ���Ϣ
        for(int i = 0;i < n;++i){
            cout<<"�����"<<i+1<<"����������յ��X��Y����,ÿ�������ÿո����,��:sx sy ex ey:"<<endl;
            cin>>sx>>sy>>ex>>ey;
            std::vector<int>tmp;
            tmp.push_back(sx);
            tmp.push_back(sy);
            tmp.push_back(ex);
            tmp.push_back(ey);
            STARTANDEND.push_back(tmp);
        }
    }
    //2.1.���ɾ���
    if(flag){   //���о�����Ϣ��ͬ
        std::vector<std::vector<int>>arr(row,std::vector<int>(col,0));
        for(int i = 0;i < n;++i) MATRIX.push_back(arr); //�洢n��
    }else{      //��ͬ������Ϣ
        for(int i = 0;i < n;++i) {
            std::vector<std::vector<int>>arr(ROW[i],std::vector<int>(COL[i],0));
            MATRIX.push_back(arr);
        }
    }
    //���յ���ֵ����
    for(int i = 0;i < n;i++){
        MATRIX[i][STARTANDEND[i][0]][STARTANDEND[i][1]] = INT_MIN;  //���������Сֵ
        MATRIX[i][STARTANDEND[i][2]][STARTANDEND[i][3]] = INT_MAX;  //�յ��������ֵ
    }
    //2.2.�ϰ�����
    //�ȹ������������
    srand(time(0));     //�����������
    for(int i = 0;i < n;++i) {  //��ÿ�����������ϰ�
    std::mt19937 obsx(static_cast<unsigned int>(time(nullptr)));    //���X����
    std::mt19937 obsy(static_cast<unsigned int>(time(nullptr)));    //���Y����
    std::mt19937 obsdrc(static_cast<unsigned int>(time(nullptr)));  //�������
    std::mt19937 obsl(static_cast<unsigned int>(time(nullptr)));    //�������
    std::uniform_int_distribution<unsigned int> Y(0,ROW[i]-1);      //�������Y
    std::uniform_int_distribution<unsigned int> X(0,COL[i]-1);      //�������X
    std::uniform_int_distribution<unsigned int> D(0,1);             //����������� 0���� 1����
    std::uniform_int_distribution<unsigned int> L(0,OBSLEN[i]-1);   //�����������
        std::vector<int> obsinfo;
        OBSINFO.push_back(obsinfo);
        for(int j = 0;j < OBS[i];++j) {  //����OBS[i]���ϰ�
            //�����ϰ��ĸ�����Ϣ
            int x = (X(obsx)*rand()+rand())%COL[i],     //�����ϰ���������
                y = (Y(obsy)*rand()+rand())%ROW[i],     //�����ϰ���������
                drc = (D(obsdrc)*rand()+rand()) %2,     //�����ϰ�����
                len = (L(obsl)*rand()+rand())%OBSLEN[i];//�����ϰ�����
            if(drc) if(y + len > ROW[i]) len = ROW[i] - y;  //��ֱ���򳬹�ʱ����������
            if(!drc)if(x + len > COL[i]) len = COL[i] - x;  //ˮƽ���򳬹�ʱ����������
            //�����ϰ���Ϣ
            OBSINFO[i].push_back(x);
            OBSINFO[i].push_back(y);
            OBSINFO[i].push_back(drc);
            OBSINFO[i].push_back(len);
            //�����ϰ�
            if(drc) for(int ix = x,jy = y;ix < x + len && ix<COL[i];++ix) MATRIX[i][ix][jy] = -1;   //����
            else    for(int ix = x,jy = y;jy < y + len && jy<ROW[i];++jy) MATRIX[i][ix][jy] = -1;   //����
        }
    }
    //�������������ϰ���Ϣ
    cout<<"f:randomMatrix.hpp_��ʼ��������ϰ������յ���Ϣ..."<<endl;
    std::vector<std::string> OBSINFO_FILE;
    std::vector<std::string> STARTANDEND_FILE;
    for(int i = 0;i < n;++i){   //����ÿ������OBS�ļ��ı���·��
        std::string fname = "./0_OBS_INFO_" + std::to_string(CreationTimes+i) + ".txt";
        std::string sefile = "./1_STARTANDEND_INFO_" + std::to_string(CreationTimes+i) + ".txt";
        OBSINFO_FILE.push_back(fname);
        STARTANDEND_FILE.push_back(sefile);
    }
    for(int i = 0;i < n;++i){   //����ÿ������OBS�ļ����ϰ���Ϣ
        std::ofstream fout(OBSINFO_FILE[i]);    //��OBS�ļ�
        std::ofstream fse(STARTANDEND_FILE[i]); //�����յ��ļ�
        if(!fout.is_open() || !fse.is_open()) {
            std::cout<<"�ļ���ʧ��"<<std::endl;
            return;
        }
        for(int j = 0;j < OBS[i];++j)  //�����i�������OBS��Ϣ
            fout<<"���X:"<<OBSINFO[i][j*4]<<"\t,���Y:"<<OBSINFO[i][j*4+1]<<"\t,����(0��1��):"<<OBSINFO[i][j*4+2]<<"\t,����:"<<OBSINFO[i][j*4+3]<<std::endl;
        fse<<STARTANDEND[i][0]<<","<<STARTANDEND[i][1]<<"\t"<<STARTANDEND[i][2]<<","<<STARTANDEND[i][3]<<std::endl;
        fout.close();
        fse.close();
    }
    cout<<"f:randomMatrix.hpp_�����ϰ������յ���Ϣ�������..."<<endl;
}

