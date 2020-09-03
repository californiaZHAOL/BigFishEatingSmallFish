#include<conio.h>
#include <graphics.h> 
#include<stdio.h>
#include<time.h>
#pragma comment(lib,"Winmm.lib")

#define LENGTH 1000//游戏画面宽度
#define HIGH 688//游戏画面高度

void chose();//识别用户的选择
void showhelp();//展示帮助信息
void init();//初始化数据
void ChangeDir();//改变玩家的鱼的方向
void draw();//画出整个游戏界面
void playermove();//更新玩家的鱼的信息
void foodmove();//更新玩家的鱼以外的东西的信息
void gameover();//游戏结束画面
void gamepause();//游戏暂停
void judge();//判断

int mode = 0;//游戏模式，0为时间模式，1为生命模式
int bkmusic = 1;//背景乐默认打开,-1为关闭
int acoustics = 1;//音效默认打开,-1为关闭
char playerDir = 'D';//玩家的鱼的初始移动方向向右
char headDir = 'D';//玩家的鱼的初始鱼头方向向右
int score = 0;//玩家的得分
double  size = 40;//定义一个变量来储存玩家鱼的尺寸
double  start_size = 40;//定义一个变量来储存玩家的初始大小
time_t gametime = 120;//剩余的游戏时间
time_t alltime = 120;//总时间
time_t beginTime, nowTime, pauseStartTime, pauseEndTime, PauseTotalTime = 0;
int gamelife = 3;//剩余的游戏生命条数
int flag = 0;//标志游戏是否刚刚开始
int again = 0;//游戏是否继续

struct fish
{
	double position_x;//横坐标
	double position_y;//纵坐标
	double v_x;//水平速度
	double v_y;//竖直速度
};

struct fish Player;//玩家
struct fish Timer, LTimer;//闹钟
struct fish Zhadan1, Zhadan2;
struct fish Yao1, Yao2;
struct fish Lfood1, Rfood1;//1号鱼
struct fish Lfood2, Rfood2;//2号鱼
struct fish Lfood3, Rfood3;//3号鱼
struct fish Lfood4, Rfood4;//4号鱼
struct fish Lfood5, Rfood5;//5号鱼

IMAGE imgMenu;//初始界面
IMAGE timebk, lifebk;//时间模式，生命模式游戏时的背景图
IMAGE end;//一局游戏结束时的背景
IMAGE pause;//游戏暂停时的背景
IMAGE L_bkfish_player, L_fish_player;//左向玩家的鱼
IMAGE R_bkfish_player, R_fish_player;//右向玩家的鱼
IMAGE timer, timerbk;//时钟的背景
IMAGE zhadan1, zhadan1bk, zhadan2bk, zhadan2;
IMAGE yao1, yao1bk, yao2bk, yao2;
IMAGE L_food1bk, L_food1, R_food1bk, R_food1;//左右朝向的一号背景鱼的图片
IMAGE L_food2bk, L_food2, R_food2bk, R_food2;//左右朝向的二号背景鱼的图片
IMAGE L_food3bk, L_food3, R_food3bk, R_food3;//左右朝向的三号背景鱼的图片
IMAGE L_food4bk, L_food4, R_food4bk, R_food4;//左右朝向的四号背景鱼的图片
IMAGE L_food5bk, L_food5, R_food5bk, R_food5;//左右朝向的五号背景鱼的图片