#include<conio.h>
#include <graphics.h> 
#include<stdio.h>
#include<time.h>
#pragma comment(lib,"Winmm.lib")

#define LENGTH 1000//��Ϸ������
#define HIGH 688//��Ϸ����߶�

void chose();//ʶ���û���ѡ��
void showhelp();//չʾ������Ϣ
void init();//��ʼ������
void ChangeDir();//�ı���ҵ���ķ���
void draw();//����������Ϸ����
void playermove();//������ҵ������Ϣ
void foodmove();//������ҵ�������Ķ�������Ϣ
void gameover();//��Ϸ��������
void gamepause();//��Ϸ��ͣ
void judge();//�ж�

int mode = 0;//��Ϸģʽ��0Ϊʱ��ģʽ��1Ϊ����ģʽ
int bkmusic = 1;//������Ĭ�ϴ�,-1Ϊ�ر�
int acoustics = 1;//��ЧĬ�ϴ�,-1Ϊ�ر�
char playerDir = 'D';//��ҵ���ĳ�ʼ�ƶ���������
char headDir = 'D';//��ҵ���ĳ�ʼ��ͷ��������
int score = 0;//��ҵĵ÷�
double  size = 40;//����һ�����������������ĳߴ�
double  start_size = 40;//����һ��������������ҵĳ�ʼ��С
time_t gametime = 120;//ʣ�����Ϸʱ��
time_t alltime = 120;//��ʱ��
time_t beginTime, nowTime, pauseStartTime, pauseEndTime, PauseTotalTime = 0;
int gamelife = 3;//ʣ�����Ϸ��������
int flag = 0;//��־��Ϸ�Ƿ�ոտ�ʼ
int again = 0;//��Ϸ�Ƿ����

struct fish
{
	double position_x;//������
	double position_y;//������
	double v_x;//ˮƽ�ٶ�
	double v_y;//��ֱ�ٶ�
};

struct fish Player;//���
struct fish Timer, LTimer;//����
struct fish Zhadan1, Zhadan2;
struct fish Yao1, Yao2;
struct fish Lfood1, Rfood1;//1����
struct fish Lfood2, Rfood2;//2����
struct fish Lfood3, Rfood3;//3����
struct fish Lfood4, Rfood4;//4����
struct fish Lfood5, Rfood5;//5����

IMAGE imgMenu;//��ʼ����
IMAGE timebk, lifebk;//ʱ��ģʽ������ģʽ��Ϸʱ�ı���ͼ
IMAGE end;//һ����Ϸ����ʱ�ı���
IMAGE pause;//��Ϸ��ͣʱ�ı���
IMAGE L_bkfish_player, L_fish_player;//������ҵ���
IMAGE R_bkfish_player, R_fish_player;//������ҵ���
IMAGE timer, timerbk;//ʱ�ӵı���
IMAGE zhadan1, zhadan1bk, zhadan2bk, zhadan2;
IMAGE yao1, yao1bk, yao2bk, yao2;
IMAGE L_food1bk, L_food1, R_food1bk, R_food1;//���ҳ����һ�ű������ͼƬ
IMAGE L_food2bk, L_food2, R_food2bk, R_food2;//���ҳ���Ķ��ű������ͼƬ
IMAGE L_food3bk, L_food3, R_food3bk, R_food3;//���ҳ�������ű������ͼƬ
IMAGE L_food4bk, L_food4, R_food4bk, R_food4;//���ҳ�����ĺű������ͼƬ
IMAGE L_food5bk, L_food5, R_food5bk, R_food5;//���ҳ������ű������ͼƬ