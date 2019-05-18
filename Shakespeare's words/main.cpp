#include<iostream>
#include<string>
#include<pthread.h>
#include<semaphore.h>
#include<string.h>
#include<stdlib.h>
#include<map>
#include<unordered_map>
using namespace std;

#define N 5784591               //总字节数

struct position{
    int begin;
    int end;
    position(){
        begin = 0;
        end = 0;
    }
};
position* index_pos;

int n;                          //线程数
int count_done = 0;             //已完成的count
char buffer[N + 1000] = "\0";   //txt文本
//int* index_buf;                 //存放读与处理的buffer下标
int is_word[128] = { 0 };
unordered_map<string, int> maps; //Hashmap
unordered_map<string, int>::iterator it;

int read_index = 0;     //指示当前读了几个
int count_index = 0;    //指示当前处理了几个
sem_t read_mutex;       //读指针互斥
sem_t count_enable;     //处理使能
//sem_t count_mutex;      //处理指针互斥

void* read(void* arg);
void* count(void* arg);
void handle(int begin, int end);
int match(char* buf, int begin, int end, int& flag, int& word_end);
void change(char* buf); //大写转小写
void topten();
//void position_decide(char* buf, int off, int& begin, int& end);        //确定起始位置与结束位置

int main(int argc, char* argv[]){
    //---------------------- 初始化符号表 -----------------------------------------
    for(int i = 'a'; i <= 'z'; ++i){    //a~z
        is_word[i] = 1;
    }
    for(int i = 'A'; i <= 'Z'; ++i){    //A~Z
        is_word[i] = 1;
    }
    for(int i = '0'; i <= '9'; ++i){    //0~9
        is_word[i] = 1;
    }
    is_word['-'] = 1;                   //-
    //----------------------------------------------------------------------------
    //----------------------- 初始化信号量 ----------------------------------------
    sem_init(&read_mutex, 0, 1);        //初始化为1，用作mutex
    //sem_init(&count_mutex, 0, 1);     //初始化为1，用作mutex
    sem_init(&count_enable, 0, 0);      //初始化为0，直接等待
    //----------------------------------------------------------------------------

    n = atoi(argv[1]);                  //最大线程个数
    int* args = new int[n];             //传入read线程参数
    /*
    index_buf = new int[n];             //初始化为全-1
    for(int i = 0; i < n; ++i){
        index_buf[i] = -1;
    }*/
    index_pos = new position[n];

    pthread_t* tid_read = (pthread_t*)malloc(sizeof(pthread_t) * n);    //读线程
    pthread_t tid_count;                                                //计数线程
    pthread_create(&tid_count, NULL, count, NULL);
    for(int i = 0; i < n; ++i){
        args[i] = i;
        pthread_create(&tid_read[i], NULL, read, &args[i]);
    }

    pthread_join(tid_count, NULL);      //等待count结束
    return 0;
}

void* read(void* arg){
    int index = *(int* )arg;            //参数，读取的大概位置
    int begin_pos = 0;                  //起始位置
    int end_pos = 0;                    //结束位置
    FILE* fp = fopen("sh.txt", "rb");
    if(fp == NULL){
        cerr << "open error!\n";
        exit(0);
    }
    int num = N / n;                    //需要读取的量
    int offset = index * num;           //偏移量

    //-------------------- 测试边界，防止切分单词 ---------------------------
    char temp;                          //测试边界
    int i = 0;                          //测试边界
    fseek(fp, offset, SEEK_SET);        //跳转到对应偏移位置
    temp = fgetc(fp);
    while(temp < 128 && temp >= 0 && is_word[temp]){
        temp = fgetc(fp);               //前边界，算作他人
        i++;
    }
    begin_pos = i + offset;
    fseek(fp, offset + num, SEEK_SET);
    i = 0;
    temp = fgetc(fp);
    while(temp < 128 && temp >= 0 && is_word[temp]){
        temp = fgetc(fp);               //后边界，算作自己
        i++;
    }
    end_pos = i + offset + num;
    if(index == n - 1){                 //最后一段，后边界为文件末尾
        end_pos = N;
    }
    //----------------------------------------------------------------------
    
    //fread(buffer + offset, 1, num, fp);
    fseek(fp, begin_pos, SEEK_SET);
    fread(buffer + begin_pos, 1, end_pos - begin_pos, fp);
    fclose(fp);

    //position_decide(buffer, offset, begin_pos, end_pos);
    //cout << buffer + offset << endl;
    
    //--------------- 存入此线程读入的index，读指针加一 ----------------------
    sem_wait(&read_mutex);
    //index_buf[read_index] = index;
    index_pos[read_index].begin = begin_pos;
    index_pos[read_index].end = end_pos;
    read_index = read_index + 1;
    sem_post(&read_mutex);
    //----------------------------------------------------------------------
    sem_post(&count_enable);            //唤醒一个等待的count线程
    //cout << "over, arg = " << *(int* )arg << endl;
}

void* count(void* arg){
    while(count_done < n){
        int index;
        sem_wait(&count_enable);        //等待处理
        //---------------- 处理一个部分，处理指针加一 ----------------------------
        //if(index_buf[count_index] != -1){
        //    index = index_buf[count_index];
        //}
        int begin = index_pos[count_index].begin;
        int end = index_pos[count_index].end;
        count_index = count_index + 1;
        //-----------------------------------------------------------------------

        //---------------- 处理index对应的buffer ---------------------------------
        //handle(index);
        handle(begin, end);
        //-----------------------------------------------------------------------
        count_done++;
    }
    topten();//输出前十多的
}

void handle(int begin, int end){
    int flag = 0;
    int len = N / n;
    int buf_begin = begin;    //总开始
    int buf_end = end;//总末尾
    //cout << buffer + buf_begin << endl;
    int i = buf_begin;              //单词开始
    char words[60] = "\0";          //存放单词
    string str;                     //string存放单词
    int next_begin;                 //下一个单词开头
    int word_end = i;               //当前单词末尾
    while(i < buf_end){
        next_begin = match(buffer, i, buf_end, flag, word_end);
        //cout << "begin = " << i << "  end = " << word_end << endl;
        if(flag){
            //printf("buffer=%.29s\n", buffer + i);
            strncpy(words, buffer + i, word_end - i);   //剥离单词
            words[word_end - i] = '\0';
            //cout << "words: " << words << endl;
            change(words);                              //转化为小写
            str = words;                                //转化为string
            maps[str]++;                                //存入hashtable
            //cout << "str: " << str << endl;
            //cout << str << endl;
            flag = 0;
        }
        i = next_begin;
    }
}

int match(char* buf, int begin, int end, int& flag, int& word_end){
    int i = begin;
    if(buf[i] >= 128 || (!is_word[buf[i]]))    flag = 0;   //起始不是单词
    //--------------- 匹配这一个单词 ------------------------------
    while(i < end){
        if(buf[i] < 128 && (is_word[buf[i]])){
            i++;
        }
        else{
            break;
        }
    }
    flag = 1;
    word_end = i;
    //---------------- 匹配到下一个单词处 --------------------------
    while(i < end){
        if(buf[i] >= 128 || (!is_word[buf[i]])){
            i++;
        }
        else{
            break;
        }
    }
    //-------------------------------------------------------------
    return i;
}

void change(char* buf){                         //大写转小写
    int i = 0;
    while(buf[i] != '\0'){
        if(buf[i] >= 'A' && buf[i] <= 'Z'){
            buf[i] = buf[i] + 32;
        }
        ++i;
    }
}

void topten(){
    int num;                                    //单词总个数
    string str;                                 //单词名称
    for(int i = 0; i < 10; i++){
        num = 0;
        it = maps.begin();
        while(it != maps.end()){
            if(it->second > num){               //记录当前最大个数
                num = it->second;
                str = it->first;
            }
            it++;
        }
        cout << "Number " << i << ": " << str << ", number:" << num << endl;
        maps[str] = 0;                          //数量置为0
    }
}

/*void position_decide(char* buf, int off, int& begin, int& end){
    int i = off;
    int len = N / n;
    while(buf[i] < 128 && (is_word[buf[i]])){
        i++;
    }
    begin = i;
    i = off + len;
    while(buf[i] < 128 && (is_word[buf[i]])){
        i++;
    }
    end = i;
}*/