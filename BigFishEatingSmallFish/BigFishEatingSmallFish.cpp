#include"BigFishEatingSmallFish.h"

int main()
{
	initgraph(LENGTH, HIGH);

	// 获取窗口句柄
	HWND hwnd = GetHWnd();
	//把游戏名称设置在窗口
	SetWindowText(hwnd, _T("大鱼吃小鱼"));

	//背景音乐的播放
	mciSendString(_T("open D:\\BigFishEatingSmallFish\\bkmusic.wma alias mybkmusic"), NULL, 0, NULL);
	mciSendString(_T("play mybkmusic repeat"), NULL, 0, NULL);    

	//批量制图开始
	BeginBatchDraw();

	//初始的游戏画面
	loop: loadimage(&imgMenu, _T("D:\\BigFishEatingSmallFish\\init.jpg"));   
	putimage(0, 0, &imgMenu);   

	//画初始的游戏画面
	FlushBatchDraw();
	MOUSEMSG m;

	chose();//让用户选择模式或者是音量音效调节，或者是帮助																						 
	init();//初始化
	
	while (1)
	{ 
		//时间模式下判断游戏是否结束
		if (mode == 0)
		{
			if ((gametime <= 0 || score >= 320) && flag != 0)
			{
				gameover();
				if (again == 1)
				{
					again = 0;
					goto loop;
				}
			}
		}
		//生命模式下判断游戏是否结束
		else
		{
			if ((gamelife <= 0 || score >= 320) && flag != 0)
			{
				gameover();
				if (again == 1)
				{
					again = 0;
					goto loop;
				}
			}
		}

		//画出整个游戏界面
		draw();

		//识别鼠标是否点击了暂停按钮
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				//如果点击到了暂停按钮
				if (m.x > 900 && m.x < 1000 && m.y > 10 && m.y < 60)
				{
					//如果音效开关是开着的，播放点击音效
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);
						mciSendString(_T("play click"), NULL, 0, NULL); 
					}
					pauseStartTime = time(NULL);
					//显示游戏暂停界面
					gamepause();
					pauseEndTime = time(NULL);
					//计算暂停的总时间
					PauseTotalTime += (pauseEndTime - pauseStartTime);
					//如果用户选择重新游戏
					if (again == 1)
					{
						again = 0;
						//跳转到初始界面
						goto loop;
					}
				}
			}
		}

		//如果有键盘消息
		if (_kbhit())
		{
			//改变玩家的鱼的方向
			ChangeDir();
			//玩家的鱼移动
			playermove();
		}
		//所有的食物道具移动
		foodmove();

		//判断玩家是否和食物和道具相撞
		judge();

		//判断循环已经经过了一此，以免对游戏失误的误判断
		flag = 1;
	}
	//停止批量绘图
	EndBatchDraw();
	return 0; 
}

void chose()
{
	// 设置 XOR 绘图模式
	setwritemode(R2_XORPEN); 
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255,1,255));
	//根据背景音乐的开关决定是否画出斜线
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	//根据背景音效的开关决定是否画出斜线
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();

	MOUSEMSG m;
	while (1)
	{
		//如果鼠标点击
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标左键点击
			if (m.mkLButton == true)
			{
				//识别背景音乐图标点击
				if (m.x > 13 && m.x < 79 && m.y > 11 && m.y < 75)
				{
					//如果音效开关是开着的，播放点击音效
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);
						mciSendString(_T("play click"), NULL, 0, NULL);  
					}
					//画线（XOR模式，画两次消失）
					line(27, 20, 63, 69);
					if (bkmusic == 1)
					{
						//如果原来是打开的，现在关闭
						mciSendString(_T("close mybkmusic"), NULL, 0, NULL);
					}
					else
					{
						//如果原来是关闭的，现在打开
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\bkmusic.wma alias mybkmusic"), NULL, 0, NULL);
						mciSendString(_T("play mybkmusic repeat"), NULL, 0, NULL);    
					}
					bkmusic = -bkmusic;
				}
				//识别音效图标点击
				else if (m.x > 13 && m.x < 79 && m.y > 98 && m.y < 155)
				{
					//如果音效开关是开着的，播放点击音效
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					//画线（XOR模式，画两次消失）
					line(19, 107, 53, 152);
					//音效由开变为不开，不开变为开
					acoustics = -acoustics;
				}
				//进入时间模式
				else if (m.x > 289 && m.x < 403 && m.y > 441 && m.y < 552)
				{
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					mode = 0;
					break;
				}
				//进入生命模式
				else if (m.x > 440 && m.x < 549 && m.y > 447 && m.y < 548)
				{
					//如果音效开关是开着的，播放点击音效
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					//模式标识
					mode = 1;
					break;
				}
				//显示帮助信息
				else if (m.x > 601 && m.x < 702 && m.y > 444 && m.y < 554)
				{
					//如果音效开关是开着的，播放点击音效
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					//显示帮助信息
					showhelp();
				}
			}
		}
		FlushBatchDraw();
	}
}

void showhelp()
{
	//设置画线风格
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	//根据音乐音效的开关决定是否画线
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	//载入初始化界面
	loadimage(&imgMenu, _T("D:\\BigFishEatingSmallFish\\init.jpg"));   
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	settextcolor(RED);//设置字体颜色为RED
	settextstyle(50, 0, _T("宋体"));
	//下面逐句显示帮助信息
	outtextxy(200, 50, "时间模式在规定时间内游戏");
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	Sleep(1000);
	cleardevice();//清屏
	putimage(0, 0, &imgMenu);   
	outtextxy(200, 50, "生命模式在规定生命值内游戏");
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	//空余一段时间，让用户有时间读到帮助信息
	Sleep(2000);
	cleardevice();//清屏
	putimage(0, 0, &imgMenu);   
	outtextxy(200, 50, "AWDS或JILK控制移动吃掉小鱼");
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	Sleep(2000);
	cleardevice();//清屏
	putimage(0, 0, &imgMenu);   
	outtextxy(310, 50, "吃掉闹钟增加时间");
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	Sleep(2000);
	cleardevice();//清屏
	putimage(0, 0, &imgMenu);   
	outtextxy(310, 50, "吃掉药水增加生命值");
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	Sleep(2000);
	cleardevice();//清屏
	putimage(0, 0, &imgMenu);   
	outtextxy(150, 50, "吃到炸弹或者大鱼减少时间或生命值");
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	Sleep(2000);
	cleardevice();//清屏
	putimage(0, 0, &imgMenu);   
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
}

void init()
{
	playerDir = 'D';//玩家的鱼的初始移动方向向右
	headDir = 'D';//玩家的鱼的初始鱼头方向向右
	score = 0;//玩家的得分
	size = 40;//定义一个变量来储存玩家鱼的尺寸
	start_size = 40;//定义一个变量来储存玩家的初始大小
	gametime = 120;//剩余的游戏时间
	alltime = 120;//总时间
	gamelife = 3;//剩余的游戏生命条数
	flag = 0;//标志游戏是否刚刚开始
	PauseTotalTime = 0;

	//载入各个图片
	loadimage(&timebk, _T("D:\\BigFishEatingSmallFish\\timeBackground.jpg"));   
	loadimage(&timebk, _T("D:\\BigFishEatingSmallFish\\timeBackground.jpg"));   
	loadimage(&lifebk, _T("D:\\BigFishEatingSmallFish\\lifeBackground.jpg"));   
	
	loadimage(&end, _T("D:\\BigFishEatingSmallFish\\end.jpg"));   

	//设置随机的种子值
	srand((unsigned)time(NULL));

	//载入各个图片并变成适当大小
	loadimage(&timer, _T("D:\\BigFishEatingSmallFish\\timer.jpg"), 35, 35, true);
	loadimage(&timerbk, _T("D:\\BigFishEatingSmallFish\\timerbk.jpg"), 35, 35, true);//闹钟

	loadimage(&yao1, _T("D:\\BigFishEatingSmallFish\\yao1.jpg"), 35, 35, true);
	loadimage(&yao1bk, _T("D:\\BigFishEatingSmallFish\\yao1bk.jpg"), 35, 35, true);//药水

	loadimage(&yao2, _T("D:\\BigFishEatingSmallFish\\yao2.jpg"), 35, 35, true);
	loadimage(&yao2bk, _T("D:\\BigFishEatingSmallFish\\yao2bk.jpg"), 35, 35, true);//药水

	loadimage(&zhadan1, _T("D:\\BigFishEatingSmallFish\\zhadan1.jpg"), 40, 40, true);
	loadimage(&zhadan1bk, _T("D:\\BigFishEatingSmallFish\\zhadan1bk.jpg"), 40, 40, true);//炸弹
	loadimage(&zhadan2, _T("D:\\BigFishEatingSmallFish\\zhadan2.jpg"), 40, 40, true);
	loadimage(&zhadan2bk, _T("D:\\BigFishEatingSmallFish\\zhadan2bk.jpg"), 40, 40, true);//炸弹

	loadimage(&L_fish_player, _T("D:\\BigFishEatingSmallFish\\L_fish_player.jpg"), (int)start_size, (int)start_size, true);
	loadimage(&L_bkfish_player, _T("D:\\BigFishEatingSmallFish\\L_bkfish_player.jpg"), (int)start_size, (int)start_size, true);
	loadimage(&R_fish_player, _T("D:\\BigFishEatingSmallFish\\R_fish_player.jpg"), (int)start_size, (int)start_size, true);
	loadimage(&R_bkfish_player, _T("D:\\BigFishEatingSmallFish\\R_bkfish_player.jpg"), (int)start_size, (int)start_size, true);

	loadimage(&L_food1, _T("D:\\BigFishEatingSmallFish\\L_food1.jpg"), 40, 30, true);
	loadimage(&L_food1bk, _T("D:\\BigFishEatingSmallFish\\L_food1bk.jpg"), 40, 30, true);
	loadimage(&R_food1, _T("D:\\BigFishEatingSmallFish\\R_food1.jpg"), 40, 30, true);
	loadimage(&R_food1bk, _T("D:\\BigFishEatingSmallFish\\R_food1bk.jpg"), 40, 30, true);

	loadimage(&L_food2, _T("D:\\BigFishEatingSmallFish\\L_food2.jpg"), 65, 40, true);
	loadimage(&L_food2bk, _T("D:\\BigFishEatingSmallFish\\L_food2bk.jpg"), 65, 40, true);
	loadimage(&R_food2, _T("D:\\BigFishEatingSmallFish\\R_food2.jpg"), 65, 40, true);
	loadimage(&R_food2bk, _T("D:\\BigFishEatingSmallFish\\R_food2bk.jpg"), 65, 40, true);

	loadimage(&L_food3, _T("D:\\BigFishEatingSmallFish\\L_food3.jpg"), 80, 80, true);
	loadimage(&L_food3bk, _T("D:\\BigFishEatingSmallFish\\L_food3bk.jpg"), 80, 80, true);
	loadimage(&R_food3, _T("D:\\BigFishEatingSmallFish\\R_food3.jpg"), 80, 80, true);
	loadimage(&R_food3bk, _T("D:\\BigFishEatingSmallFish\\R_food3bk.jpg"), 80, 80, true);

	loadimage(&L_food4, _T("D:\\BigFishEatingSmallFish\\L_food4.jpg"), 120, 110, true);
	loadimage(&L_food4bk, _T("D:\\BigFishEatingSmallFish\\L_food4bk.jpg"), 120, 110, true);
	loadimage(&R_food4, _T("D:\\BigFishEatingSmallFish\\R_food4.jpg"), 120, 110, true);
	loadimage(&R_food4bk, _T("D:\\BigFishEatingSmallFish\\R_food4bk.jpg"), 120, 110, true);

	loadimage(&L_food5, _T("D:\\BigFishEatingSmallFish\\L_food5.jpg"), 250, 125, true);
	loadimage(&L_food5bk, _T("D:\\BigFishEatingSmallFish\\L_food5bk.jpg"), 250, 125, true);
	loadimage(&R_food5, _T("D:\\BigFishEatingSmallFish\\R_food5.jpg"), 250, 125, true);
	loadimage(&R_food5bk, _T("D:\\BigFishEatingSmallFish\\R_food5bk.jpg"), 250, 125, true);

	//初始化玩家的鱼
	Player.position_x = LENGTH / 2.0;//玩家横坐标
	Player.position_y = HIGH / 2.0;//玩家纵坐标
	Player.v_x = 8;//玩家水平速度
	Player.v_y = 8;//玩家竖直速度

	//根据每个具体事物的横坐标可以来改变每个食物或者是道具的出现频率
	LTimer.position_x = 1100;//闹钟横坐标
	LTimer.position_y = rand() % 633 + 50;//闹钟纵坐标随机设定
	LTimer.v_x = 0.6;//闹钟速度
	LTimer.v_y = (rand() % 2) / 10.0;//闹钟速度

	Timer.position_x = -1000;//闹钟横坐标
	Timer.position_y = rand() % 633 + 50;//闹钟纵坐标随机设定
	Timer.v_x = 0.6;//闹钟速度
	Timer.v_y = (rand() % 2) / 10.0;//闹钟速度

	Yao1.position_x = 1100;
	Yao1.position_y = rand() % 633 + 50;
	Yao1.v_x = 0.6;
	Yao1.v_y = (rand() % 2) / 10.0;

	Yao2.position_x = -1000;
	Yao2.position_y = rand() % 633 + 50;
	Yao2.v_x = 0.6;
	Yao2.v_y = (rand() % 2) / 10.0;

	Zhadan1.position_x = 1800;//横坐标
	Zhadan1.position_y = rand() % 633 + 50;//纵坐标随机设定
	Zhadan1.v_x = 0.6;//速度
	Zhadan1.v_y = (rand() % 2) / 10.0;//速度

	Zhadan2.position_x = -1100;//横坐标
	Zhadan2.position_y = rand() % 633 + 50;//纵坐标随机设定
	Zhadan2.v_x = 0.6;//速度
	Zhadan2.v_y = (rand() % 2) / 10.0;//速度

	Lfood1.position_x = 1000;//左向1号鱼横坐标
	Lfood1.position_y = rand() % 633 + 50;//左向鱼纵坐标随机设定
	Lfood1.v_x = 0.4;//左向1号鱼速度
	Rfood1.position_x = -(rand() % 100);//反向1号鱼横坐标
	Rfood1.position_y = rand() % 633 + 50;//右向1号鱼纵坐标随机设定
	Rfood1.v_x = 0.3;//右向1号鱼速度

	Lfood2.position_x = 1300;//左向2号鱼横坐标
	Lfood2.position_y = rand() % 633 + 50;//左向2号鱼纵坐标随机设定
	Lfood2.v_x = 0.45;//左向2号鱼速度
	Rfood2.position_x = -rand() % 300;//右向2号鱼横坐标
	Rfood2.position_y = rand() % 633 + 50;//右向2号鱼纵坐标随机设定
	Rfood2.v_x = 0.55;//右向2号鱼速度

	Lfood3.position_x = 1800;//左向3号鱼横坐标
	Lfood3.position_y = rand() % 600 + 88;//左向3号鱼纵坐标随机设定
	Lfood3.v_x = 0.43;//左向3号鱼速度
	Rfood3.position_x = - rand() % 1800;//右向3号鱼横坐标
	Rfood3.position_y = rand() % 600 + 88;//右向3号鱼纵坐标随机设定
	Rfood3.v_x = 0.43;//右向3号鱼速度

	Lfood4.position_x = 2500;//左向4号鱼横坐标
	Lfood4.position_y = rand() % 560 + 128;//左向4号鱼纵坐标随机设定
	Lfood4.v_x = 0.42;//左向4号鱼速度
	Rfood4.position_x = - rand() % 2500;//右向4号鱼横坐标
	Rfood4.position_y = rand() % 560 + 128;//右向4号鱼纵坐标随机设定
	Rfood4.v_x = 0.42;//右向4号鱼速度

	Lfood5.position_x = 3000;//左向5号鱼横坐标
	Lfood5.position_y = rand() % 433 + 250;//右向5号鱼纵坐标随机设定
	Lfood5.v_x = 0.4;//左向5号鱼速度
	Rfood5.position_x = -rand() % 3000;//右向5号鱼横坐标
	Rfood5.position_y = rand() % 433 + 250;//右向5号鱼纵坐标随机设定
	Rfood5.v_x = 0.4;//右向5号鱼速度

	//如果是时间模式下就计时
	if (mode == 0)
	{
		beginTime = time(NULL);
	}
}

void ChangeDir()
{
	//判断用户的输入
		switch (_getch())
		{
		case 'A':
		case 'a':
		case 'J':
		case 'j':
			if (headDir == 'D') headDir = 'A'; //判断鱼头是否转向
			playerDir = 'A';
			break;
		case 'D':
		case 'd':
		case 'L':
		case 'l':
			if (headDir == 'A') headDir = 'D';
			playerDir = 'D';
			break;
		case 'W':
		case 'w':
		case 'I':
		case 'i':
			playerDir = 'W';
			break;
		case 'S':
		case 's':
		case 'K':
		case 'k':
			playerDir = 'S';
			break;
		default:
			break;
		}
}

void draw()
{
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	settextstyle(45, 0, _T("宋体"));
	settextcolor(RED);

	//根据具体的模式选择具体的背景图片
	if (mode == 0)
	{
		putimage(0, 0, &timebk);
	}
	else
	{
		putimage(0, 0, &lifebk);
	}
	outtextxy(900, 5, _T("暂停"));

	//根据玩家的鱼头的方向显示玩家的鱼
	if (headDir == 'D')
	{
		loadimage(&R_fish_player, _T("D:\\BigFishEatingSmallFish\\R_fish_player.jpg"), (int)size, (int)size, true);
		loadimage(&R_bkfish_player, _T("D:\\BigFishEatingSmallFish\\R_bkfish_player.jpg"), (int)size, (int)size, true);
	}
	else
	{
		loadimage(&L_fish_player, _T("D:\\BigFishEatingSmallFish\\L_fish_player.jpg"), (int)size, (int)size, true);
		loadimage(&L_bkfish_player, _T("D:\\BigFishEatingSmallFish\\L_bkfish_player.jpg"), (int)size, (int)size, true);
	}

	//根据玩家选择的模式来选择是显示闹钟图片还是药水图片
	if (mode == 0)
	{
		putimage((int)Timer.position_x, (int)Timer.position_y, &timerbk, NOTSRCERASE);//显示闹钟
		putimage((int)Timer.position_x, (int)Timer.position_y, &timer, SRCINVERT);
		putimage((int)LTimer.position_x, (int)LTimer.position_y, &timerbk, NOTSRCERASE);//显示闹钟
		putimage((int)LTimer.position_x, (int)LTimer.position_y, &timer, SRCINVERT);
	}
	else
	{
		putimage((int)Yao1.position_x, (int)Yao1.position_y, &yao1bk, SRCAND);//显示药
		putimage((int)Yao1.position_x, (int)Yao1.position_y, &yao1, SRCPAINT);
		putimage((int)Yao2.position_x, (int)Yao2.position_y, &yao2bk, SRCAND);//显示药
		putimage((int)Yao2.position_x, (int)Yao2.position_y, &yao2, SRCPAINT);
	}

	putimage((int)Zhadan1.position_x, (int)Zhadan1.position_y, &zhadan1bk, SRCAND);//显示炸弹
	putimage((int)Zhadan1.position_x, (int)Zhadan1.position_y, &zhadan1, SRCPAINT);
	putimage((int)Zhadan2.position_x, (int)Zhadan2.position_y, &zhadan2bk, SRCAND);//显示炸弹
	putimage((int)Zhadan2.position_x, (int)Zhadan2.position_y, &zhadan2, SRCPAINT);

	putimage((int)Lfood1.position_x, (int)Lfood1.position_y, &L_food1bk, SRCAND);//显示1号鱼
	putimage((int)Lfood1.position_x, (int)Lfood1.position_y, &L_food1, SRCPAINT);
	putimage((int)Rfood1.position_x, (int)Rfood1.position_y, &R_food1bk, SRCAND);//显示反向1号鱼
	putimage((int)Rfood1.position_x, (int)Rfood1.position_y, &R_food1, SRCPAINT);

	putimage((int)Lfood2.position_x, (int)Lfood2.position_y, &L_food2bk, SRCAND);//显示2号鱼
	putimage((int)Lfood2.position_x, (int)Lfood2.position_y, &L_food2, SRCPAINT);
	putimage((int)Rfood2.position_x, (int)Rfood2.position_y, &R_food2bk, NOTSRCERASE);//显示反向2号鱼
	putimage((int)Rfood2.position_x, (int)Rfood2.position_y, &R_food2, SRCINVERT);

	putimage((int)Lfood3.position_x, (int)Lfood3.position_y, &L_food3bk, SRCAND);//显示3号鱼
	putimage((int)Lfood3.position_x, (int)Lfood3.position_y, &L_food3, SRCPAINT);
	putimage((int)Rfood3.position_x, (int)Rfood3.position_y, &R_food3bk, SRCAND);//显示反向3号鱼
	putimage((int)Rfood3.position_x, (int)Rfood3.position_y, &R_food3, SRCPAINT);

	putimage((int)Lfood4.position_x, (int)Lfood4.position_y, &L_food4bk, SRCAND);//显示4号鱼
	putimage((int)Lfood4.position_x, (int)Lfood4.position_y, &L_food4, SRCPAINT);
	putimage((int)Rfood4.position_x, (int)Rfood4.position_y, &R_food4bk, SRCAND);//显示反向4号鱼
	putimage((int)Rfood4.position_x, (int)Rfood4.position_y, &R_food4, SRCPAINT);

	putimage((int)Lfood5.position_x, (int)Lfood5.position_y, &L_food5bk, SRCAND);//显示5号鱼
	putimage((int)Lfood5.position_x, (int)Lfood5.position_y, &L_food5, SRCPAINT);
	putimage((int)Rfood5.position_x, (int)Rfood5.position_y, &R_food5bk, NOTSRCERASE);//显示反向5号鱼
	putimage((int)Rfood5.position_x, (int)Rfood5.position_y, &R_food5, SRCINVERT);

	if (headDir == 'D')
	{
		putimage((int)Player.position_x, (int)Player.position_y, &R_bkfish_player, SRCAND);//显示玩家
		putimage((int)Player.position_x, (int)Player.position_y, &R_fish_player, SRCPAINT);
	}
	else
	{
		putimage((int)Player.position_x, (int)Player.position_y, &L_bkfish_player, SRCAND);//显示玩家
		putimage((int)Player.position_x, (int)Player.position_y, &L_fish_player, SRCPAINT);
	}
	
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	settextstyle(45, 0, _T("宋体"));
	settextcolor(RED);

	//时间模式下显示游戏剩余的时间
	if (mode == 0)
	{
		nowTime = time(NULL);
		CHAR times[10];
		gametime = alltime - (nowTime - beginTime) + PauseTotalTime;
		wsprintf(times, "%ld", (gametime));
		outtextxy(160, 4, times);
	}
	//生命模式下显示剩余的生命
	else
	{
		CHAR lifes[10];
		wsprintf(lifes, "%d", gamelife);
		outtextxy(160, 4, lifes);
	}

	//显示玩家的分数
	score = (int)(size - start_size);
	CHAR scores[10];
	wsprintf(scores, "%d", score);
	outtextxy(830, 4, scores);

	FlushBatchDraw();
}

void playermove()
{
	//根据玩家的输入来改变玩家的鱼儿的坐标
	switch (playerDir)
	{
	case 'A':
	case 'a':
	case 'J':
	case 'j':
		Player.position_x -= Player.v_x;
		break;
	case 'D':
	case 'd':
	case 'L':
	case 'l':
		Player.position_x += Player.v_x;
		break;
	case 'W':
	case 'w':
	case 'I':
	case 'i':
		Player.position_y -= Player.v_y;
		break;
	case 'S':
	case 's':
	case 'K':
	case 'k':
		Player.position_y += Player.v_y;
		break;
	default:
		break;
	}
	//判断玩家的鱼是否游到了界面的尽头
	if (Player.position_x <= 0)
	{
		Player.position_x = 0;
		headDir = 'D';
	}
	if (Player.position_x >= 1000 - size)
	{
		Player.position_x = 1000 - size;
		headDir = 'A';
	}
	if (Player.position_y <= 0)
	{
		Player.position_y = 0;
	}
	if (Player.position_y >= 688 - size)
	{
		Player.position_y = 688 - size;
	}
}

void foodmove()
{
	//各种食物鱼和道具的坐标改变
	Lfood1.position_x -= Lfood1.v_x;
	Lfood2.position_x -= Lfood2.v_x;
	Lfood3.position_x -= Lfood3.v_x;
	Lfood4.position_x -= Lfood4.v_x;
	Lfood5.position_x -= Lfood5.v_x;

	Rfood1.position_x += Rfood1.v_x;
	Rfood2.position_x += Rfood2.v_x;
	Rfood3.position_x += Rfood3.v_x;
	Rfood4.position_x += Rfood4.v_x;
	Rfood5.position_x += Rfood5.v_x;

	Timer.position_x += Timer.v_x;
	Timer.position_y -= Timer.v_y;

	LTimer.position_x -= Timer.v_x;
	LTimer.position_y += Timer.v_y;

	Zhadan1.position_x -= Zhadan1.v_x;
	Zhadan1.position_y += Zhadan1.v_y;

	Zhadan2.position_x += Zhadan2.v_x;
	Zhadan2.position_y -= Zhadan2.v_y;

	Yao1.position_x -= Yao1.v_x;
	Yao1.position_y += Yao1.v_y;

	Yao2.position_x += Yao2.v_x;
	Yao2.position_y -= Yao2.v_y;

	//如果食物鱼或者是道具游出边界
	//继续随机产生一个
	if (Lfood1.position_x <= -40)
	{
		Lfood1.position_x = 1000;//左向1号鱼横坐标
		Lfood1.position_y = rand() % 633 + 50;//左向鱼纵坐标随机设定
	}
	if (Rfood1.position_x >= 1000)
	{
		Rfood1.position_x = -(rand() % 10);//反向1号鱼横坐标
		Rfood1.position_y = rand() % 633 + 50;//右向1号鱼纵坐标随机设定
	}

	if (Lfood2.position_x <= -65)
	{
		Lfood2.position_x = 1300;//左向2号鱼横坐标
		Lfood2.position_y = rand() % 600 + 40;//左向2号鱼纵坐标随机设定
	}
	if (Rfood2.position_x >= 1000)
	{
		Rfood2.position_x = -rand() % 300;//右向2号鱼横坐标
		Rfood2.position_y = rand() % 600 + 40;//右向2号鱼纵坐标随机设定
	}

	if (Lfood3.position_x <= -80)
	{
		Lfood3.position_x = 1800;//左向3号鱼横坐标
		Lfood3.position_y = rand() % 600 + 48;//左向3号鱼纵坐标随机设定
	}
	if (Rfood3.position_x >= 1800)
	{
		Rfood3.position_x = -rand() % 1800;//右向3号鱼横坐标
		Rfood3.position_y = rand() % 600 + 48;//右向3号鱼纵坐标随机设定
	}

	if (Lfood4.position_x <= -120)
	{
		Lfood4.position_x = 2500;//左向4号鱼横坐标
		Lfood4.position_y = rand() % 560 + 58;//左向4号鱼纵坐标随机设定
	}
	if (Rfood4.position_x >= 1000)
	{
		Rfood4.position_x = - rand() % 2500;//右向4号鱼横坐标
		Rfood4.position_y = rand() % 560 + 58;//右向4号鱼纵坐标随机设定
	}


	if (Lfood5.position_x <= -250)
	{
		Lfood5.position_x = 3000;//左向5号鱼横坐标
		Lfood5.position_y = rand() % 400;//右向5号鱼纵坐标随机设定
	}
	if (Rfood5.position_x >= 1000)
	{
		Rfood5.position_x = -rand() % 3000;//右向5号鱼横坐标
		Rfood5.position_y = rand() % 400;//右向5号鱼纵坐标随机设定
	}

	if (Timer.position_x >= 1000)
	{
		Timer.position_x = -1000;//闹钟横坐标
		Timer.position_y = rand() % 500;//闹钟纵坐标随机设定
		Timer.v_y = (rand() % 2) / 10.0;//闹钟速度
	}

	if (LTimer.position_x <= -35)
	{
		LTimer.position_x = 1600;//闹钟横坐标
		LTimer.position_y = rand() % 500;//闹钟纵坐标随机设定
		LTimer.v_y = (rand() % 2) / 10.0;//闹钟速度
	}

	if (Yao2.position_x >= 1000)
	{
		Yao2.position_x = -1000;//药水横坐标
		Yao2.position_y = rand() % 500;//药水纵坐标随机设定
		Yao2.v_y = (rand() % 2) / 10.0;//药水速度
	}

	if (Yao1.position_x <= -35)
	{
		Yao1.position_x = 1600;//药水横坐标
		Yao1.position_y = rand() % 500;//药水纵坐标随机设定
		Yao1.v_y = (rand() % 2) / 10.0;//药水速度
	}

	if (Zhadan1.position_x <= -40)
	{
		Zhadan1.position_x = 1600;//横坐标
		Zhadan1.position_y = rand() % 500;//纵坐标随机设定
		Zhadan1.v_y = (rand() % 2) / 10.0;//速度
	}
	if (Zhadan2.position_x >= 1000)
	{
		Zhadan2.position_x = -800;//横坐标
		Zhadan2.position_y = rand() % 500;//纵坐标随机设定
		Zhadan2.v_y = (rand() % 2) / 10.0;//速度
	}
}

void gameover()
{
	cleardevice();//清屏
	//显示结束界面的图片
	putimage(0, 0, &end);
	//计算得分
	score = (int)(size - start_size);
	CHAR scores[15];
	//判断分数是否达到通关的条件
	if (score >= 320)
	{
		if (acoustics == 1)
		{
			mciSendString(_T("close win"), NULL, 0, NULL);
			mciSendString(_T("open D:\\BigFishEatingSmallFish\\win.wma alias win"), NULL, 0, NULL);  
			mciSendString(_T("play win"), NULL, 0, NULL);    
		}
		wsprintf(scores, "%d 你通关了！", score);
	}
	else
		wsprintf(scores, "%d", score);
	outtextxy(580, 310, scores);

	FlushBatchDraw();

	MOUSEMSG m;
	//识别鼠标的点击
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			if (m.mkLButton == true)
			{
				//选择继续下一局游戏
				if (m.x > 340 && m.x < 650 && m.y > 400 && m.y < 490)
				{
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);
					}
					again = 1;
					break;
				}
				//选择结束游戏
				else if (m.x > 440 && m.x < 580 && m.y > 540 && m.y < 620)
				{
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					mciSendString(_T("close mybkmusic"), NULL, 0, NULL);
					MessageBox(GetHWnd(), "            游戏结束", "SORRY", MB_OK);
					exit(0);
				}
			}
		}
	}
}

void gamepause()
{
	//载入并且显示游戏暂停的界面
	loadimage(&pause, _T("D:\\BigFishEatingSmallFish\\pause.jpg"));   
	putimage(0, 0, &pause);   
	//设置XOR绘图模式
	setwritemode(R2_XORPEN); 
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	//判断音乐音效的开关并且画图
	if(bkmusic == -1)
		line(27, 20, 63, 69);
	if(acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	MOUSEMSG m;
	while (1)
	{
		//识别鼠标的点击
		if (MouseHit())
		{
			m = GetMouseMsg();
			if (m.mkLButton == true)
			{
				//识别背景音乐图标点击
				if (m.x > 13 && m.x < 79 && m.y > 11 && m.y < 75)
				{
					//判断音效是否开启来决定是否播放鼠标的点击声音
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					//XOR模式，再画一次，线从无到有，从有到无
					line(27, 20, 63, 69);
					//判断音乐是否开启来决定是否播放背景音乐
					//以前是打开的，现在就关闭
					if (bkmusic == 1)
					{
						mciSendString(_T("close mybkmusic"), NULL, 0, NULL);
					}
					//以前是关闭的，现在就打开
					else
					{
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\bkmusic.wma alias mybkmusic"), NULL, 0, NULL);
						mciSendString(_T("play mybkmusic repeat"), NULL, 0, NULL);    
					}
					bkmusic = -bkmusic;
				}
				if (m.x > 13 && m.x < 79 && m.y > 98 && m.y < 155)
				{
					//识别音效图标点击
					//判断音效是否开启来决定是否播放鼠标的点击声音
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					line(19, 107, 53, 152);
					acoustics = -acoustics;
				}
				
				if (m.x > 314 && m.x < 673 && m.y > 130 && m.y < 206)
				{
					//退出当局游戏
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					//再玩一次
					again = 1;
					break;
				}
				else if (m.x > 346 && m.x < 582 && m.y > 267 && m.y < 336)
				{
					//退出游戏
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					mciSendString(_T("close mybkmusic"), NULL, 0, NULL);
					bkmusic = 1;
					MessageBox(GetHWnd(), "            游戏结束", "SORRY", MB_OK);
					exit(0);
				}
				else if (m.x > 348 && m.x < 582 && m.y > 393 && m.y < 460)
				{
					//继续游戏
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					break;
				}
			}
		}
		FlushBatchDraw();
	}
}

void judge()
{
	if (mode == 0)
	{
		//判断闹钟是否被吃
		if (
			(Timer.position_x + 17.5 >= Player.position_x  &&
				Timer.position_x + 17.5 <= Player.position_x + size) &&
				(Timer.position_y + 17.5 >= Player.position_y &&
					Timer.position_y + 17.5 <= Player.position_y + size)
			)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eattimer"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eattimer.mp3 alias eattimer"), NULL, 0, NULL);  
				mciSendString(_T("play eattimer"), NULL, 0, NULL);    
			}
			Timer.position_x = -5000;
			Timer.position_y = rand() % 500;
			alltime += 8;
			gamelife += 1;
		}

		if (
			(LTimer.position_x + 17.5 >= Player.position_x  &&
				LTimer.position_x + 17.5 <= Player.position_x + size) &&
				(LTimer.position_y + 17.5 >= Player.position_y &&
					LTimer.position_y + 17.5 <= Player.position_y + size)
			)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eattimer"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eattimer.mp3 alias eattimer"), NULL, 0, NULL);  
				mciSendString(_T("play eattimer"), NULL, 0, NULL);    
			}
			LTimer.position_x = 2000;
			LTimer.position_y = rand() % 500;
			alltime += 8;
			gamelife += 1;
		}
	}
	else
	{
		//判断药水是否被吃
		if (
			(Yao1.position_x + 17.5 >= Player.position_x  &&
				Yao1.position_x + 17.5 <= Player.position_x + size) &&
				(Yao1.position_y + 17.5 >= Player.position_y &&
					Yao1.position_y + 17.5 <= Player.position_y + size)
			)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eattimer"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eattimer.mp3 alias eattimer"), NULL, 0, NULL);  
				mciSendString(_T("play eattimer"), NULL, 0, NULL);    
			}
			Yao1.position_x = -5000;
			Yao1.position_y = rand() % 500;
			alltime += 8;
			gamelife += 1;
		}

		if (
			(Yao2.position_x + 17.5 >= Player.position_x  &&
				Yao2.position_x + 17.5 <= Player.position_x + size) &&
				(Yao2.position_y + 17.5 >= Player.position_y &&
					Yao2.position_y + 17.5 <= Player.position_y + size)
			)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eattimer"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eattimer.mp3 alias eattimer"), NULL, 0, NULL);  
				mciSendString(_T("play eattimer"), NULL, 0, NULL);    
			}
			Yao2.position_x = 2000;
			Yao2.position_y = rand() % 500;
			alltime += 8;
			gamelife += 1;
		}
	}

	
	//判断炸弹是否被吃
	if (
		(Zhadan1.position_x + 20 >= Player.position_x &&
			Zhadan1.position_x + 20 <= Player.position_x + size) &&
			(Zhadan1.position_y + 20 >= Player.position_y &&
				Zhadan1.position_y + 20 <= Player.position_y + size)
		)
	{
		if (acoustics == 1)
		{
			mciSendString(_T("close eatzhadan"), NULL, 0, NULL);
			mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatzhadan.mp3 alias eatzhadan"), NULL, 0, NULL);  
			mciSendString(_T("play eatzhadan"), NULL, 0, NULL);    
		}
		Zhadan1.position_x = 4000;
		Zhadan1.position_y = rand() % 500;
		alltime -= 10;
		gamelife -= 1;
	}

	if (
		(Zhadan2.position_x + 20 >= Player.position_x &&
			Zhadan2.position_x + 20 <= Player.position_x + size) &&
			(Zhadan2.position_y + 20 >= Player.position_y &&
				Zhadan2.position_y + 20 <= Player.position_y + size)
		)
	{
		if (acoustics == 1)
		{
			mciSendString(_T("close eatzhadan"), NULL, 0, NULL);
			mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatzhadan.mp3 alias eatzhadan"), NULL, 0, NULL);  
			mciSendString(_T("play eatzhadan"), NULL, 0, NULL);    
		}
		Zhadan2.position_x = -3000;
		Zhadan2.position_y = rand() % 500;
		alltime -= 10;
		gamelife -= 1;
	}

	//判断一号鱼是否被吃
	if (Lfood1.position_x + 20 > Player.position_x &&
		Lfood1.position_x + 20 < Player.position_x + size &&
		Lfood1.position_y + 15 > Player.position_y &&
		Lfood1.position_y + 15 < Player.position_y + size)
	{
		if (acoustics == 1)
		{
			mciSendString(_T("close eatfood"), NULL, 0, NULL);
			mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatfood.mp3 alias eatfood"), NULL, 0, NULL);  
			mciSendString(_T("play eatfood"), NULL, 0, NULL);    
		}
		size += 2;
		Lfood1.position_x = 1100;
		Lfood1.position_y = rand() % (HIGH - 30);
		size += 2;
	}

	if (Rfood1.position_x + 20 > Player.position_x &&
		Rfood1.position_x + 20 < Player.position_x + size &&
		Rfood1.position_y + 15 > Player.position_y &&
		Rfood1.position_y + 15 < Player.position_y + size)
	{
		if (acoustics == 1)
		{
			mciSendString(_T("close eatfood"), NULL, 0, NULL);
			mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatfood.mp3 alias eatfood"), NULL, 0, NULL);  
			mciSendString(_T("play eatfood"), NULL, 0, NULL);    
		}
		size += 2;
		Rfood1.position_x = -100;
		Rfood1.position_y = rand() % (688 - 30);
	}


	//判断2号鱼是否被吃
	if (size > 45)
	{
		if (Lfood2.position_x + 32.5 > Player.position_x &&
			Lfood2.position_x + 32.5<Player.position_x + size &&
			Lfood2.position_y + 20>Player.position_y &&
			Lfood2.position_y + 20 < Player.position_y + size)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatfood"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatfood.mp3 alias eatfood"), NULL, 0, NULL);  
				mciSendString(_T("play eatfood"), NULL, 0, NULL);    
			}
			size += 4;
			Lfood2.position_x = 1100;
			Lfood2.position_y = rand() % (688 - 40);
		}
	}
	//玩家与比自身大的鱼相撞
	else
	{
		if (Player.position_x + size / 2.0 > Lfood2.position_x &&
			Player.position_x + size / 2.0<Lfood2.position_x + 65 &&
			Player.position_y + size / 2.0>Lfood2.position_y &&
			Player.position_y + size / 2.0 < Lfood2.position_y + 40)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatzhadan"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatzhadan.mp3 alias eatzhadan"), NULL, 0, NULL);  
				mciSendString(_T("play eatzhadan"), NULL, 0, NULL);    
			}
			size = start_size;
			gamelife -= 1;
			Player.position_x = rand() % (1000 - 40);
			Player.position_y = rand() % (688 - 40);
		}
	}

	if (size > 45)
	{
		if (Rfood2.position_x + 32.5 > Player.position_x &&
			Rfood2.position_x + 32.5<Player.position_x + size &&
			Rfood2.position_y + 20>Player.position_y &&
			Rfood2.position_y + 20 < Player.position_y + size)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatfood"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatfood.mp3 alias eatfood"), NULL, 0, NULL);  
				mciSendString(_T("play eatfood"), NULL, 0, NULL);    
			}
			size += 4;
			Rfood2.position_x = -100;
			Rfood2.position_y = rand() % (688 - 40);
		}
	}
	//玩家与比自身大的鱼相撞
	else
	{
		if (Player.position_x + size / 2.0 > Rfood2.position_x &&
			Player.position_x + size / 2.0 < Rfood2.position_x + 65 &&
			Player.position_y + size / 2.0 > Rfood2.position_y &&
			Player.position_y + size / 2.0 < Rfood2.position_y + 40)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatzhadan"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatzhadan.mp3 alias eatzhadan"), NULL, 0, NULL);  
				mciSendString(_T("play eatzhadan"), NULL, 0, NULL);    
			}
			size = start_size;
			gamelife -= 1;
			Player.position_x = rand() % (1000 - 40);
			Player.position_y = rand() % (688 - 40);
		}
	}


	//判断3号鱼是否被吃
	if (size > 70)
	{
		if (Lfood3.position_x + 40 > Player.position_x &&
			Lfood3.position_x + 40 < Player.position_x + size &&
			Lfood3.position_y + 40 > Player.position_y &&
			Lfood3.position_y + 40 < Player.position_y + size)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatfood"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatfood.mp3 alias eatfood"), NULL, 0, NULL);  
				mciSendString(_T("play eatfood"), NULL, 0, NULL);    
			}
			size += 6;
			Lfood3.position_x = 1100;
			Lfood3.position_y = rand() % (688 - 80);
		}
	}
	//玩家与比自身大的鱼相撞
	else
	{
		if (Player.position_x + size / 2.0 > Lfood3.position_x &&
			Player.position_x + size / 2.0 < Lfood3.position_x + 80 &&
			Player.position_y + size / 2.0 > Lfood3.position_y &&
			Player.position_y + size / 2.0 < Lfood3.position_y + 80)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatzhadan"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatzhadan.mp3 alias eatzhadan"), NULL, 0, NULL);  
				mciSendString(_T("play eatzhadan"), NULL, 0, NULL);    
			}
			size = start_size;
			gamelife -= 1;
			Player.position_x = rand() % (1000 - 40);
			Player.position_y = rand() % (688 - 40);
		}
	}

	if (size > 70)
	{
		if (Rfood3.position_x + 40 > Player.position_x &&
			Rfood3.position_x + 40 < Player.position_x + size &&
			Rfood3.position_y + 40 > Player.position_y &&
			Rfood3.position_y + 40 < Player.position_y + size)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatfood"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatfood.mp3 alias eatfood"), NULL, 0, NULL);  
				mciSendString(_T("play eatfood"), NULL, 0, NULL);    
			}
			size += 6;
			Rfood3.position_x = -100;
			Rfood3.position_y = rand() % (688 - 80);
		}
	}
	//玩家与比自身大的鱼相撞
	else
	{
		if (Player.position_x + size / 2.0 > Rfood3.position_x &&
			Player.position_x + size / 2.0 < Rfood3.position_x + 80 &&
			Player.position_y + size / 2.0 > Rfood3.position_y &&
			Player.position_y + size / 2.0 < Rfood3.position_y + 80)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatzhadan"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatzhadan.mp3 alias eatzhadan"), NULL, 0, NULL);  
				mciSendString(_T("play eatzhadan"), NULL, 0, NULL);    
			}
			size = start_size;
			gamelife -= 1;
			Player.position_x = rand() % (1000 - 40);
			Player.position_y = rand() % (688 - 40);
		}
	}


	//判断4号鱼是否被吃
	if (size > 100)
	{
		if (Lfood4.position_x + 60 > Player.position_x &&
			Lfood4.position_x + 60 < Player.position_x + size &&
			Lfood4.position_y + 50 > Player.position_y &&
			Lfood4.position_y + 50 < Player.position_y + size)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatfood"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatfood.mp3 alias eatfood"), NULL, 0, NULL);  
				mciSendString(_T("play eatfood"), NULL, 0, NULL);    
			}
			size += 5;
			Lfood4.position_x = 1100;
			Lfood4.position_y = rand() % (688 - 100);
		}
	}
	//玩家与比自身大的鱼相撞
	else
	{
		if (Player.position_x + size / 2.0 > Lfood4.position_x &&
			Player.position_x + size / 2.0 < Lfood4.position_x + 110 &&
			Player.position_y + size / 2.0 > Lfood4.position_y &&
			Player.position_y + size / 2.0 < Lfood4.position_y + 100)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatzhadan"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatzhadan.mp3 alias eatzhadan"), NULL, 0, NULL);  
				mciSendString(_T("play eatzhadan"), NULL, 0, NULL);    
			}
			size = start_size;
			gamelife -= 1;
			Player.position_x = rand() % (1000 - 40);
			Player.position_y = rand() % (688 - 40);
		}
	}

	if (size > 100)
	{
		if (Rfood4.position_x + 60 > Player.position_x &&
			Rfood4.position_x + 60 < Player.position_x + size &&
			Rfood4.position_y + 50 > Player.position_y &&
			Rfood4.position_y + 50 < Player.position_y + size)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatfood"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatfood.mp3 alias eatfood"), NULL, 0, NULL);  
				mciSendString(_T("play eatfood"), NULL, 0, NULL);    
			}
			size += 5;
			Rfood4.position_x = -100;
			Rfood4.position_y = rand() % (688 - 100);
		}
	}
	//玩家与比自身大的鱼相撞
	else
	{
		if (Player.position_x + size / 2.0 > Rfood4.position_x &&
			Player.position_x + size / 2.0 < Rfood4.position_x + 110 &&
			Player.position_y + size / 2.0 > Rfood4.position_y &&
			Player.position_y + size / 2.0 < Rfood4.position_y + 100)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatzhadan"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatzhadan.mp3 alias eatzhadan"), NULL, 0, NULL);  
				mciSendString(_T("play eatzhadan"), NULL, 0, NULL);    
			}
			size = start_size;
			gamelife -= 1;
			Player.position_x = rand() % (1000 - 40);
			Player.position_y = rand() % (688 - 40);
		}
	}
	//判断5号鱼是否被吃
	if (size > 185)
	{
		if (Lfood5.position_x + 125 > Player.position_x &&
			Lfood5.position_x + 125 < Player.position_x + size &&
			Lfood5.position_y + 62.5 > Player.position_y &&
			Lfood5.position_y + 62.5 < Player.position_y + size)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatfood"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatfood.mp3 alias eatfood"), NULL, 0, NULL);  
				mciSendString(_T("play eatfood"), NULL, 0, NULL);    
			}
			size += 2;
			Lfood5.position_x = 1100;
			Lfood5.position_y = rand() % (560);
		}
	}
	//玩家与比自身大的鱼相撞
	else
	{
		if (Player.position_x + size / 2.0 > Lfood5.position_x &&
			Player.position_x + size / 2.0 < Lfood5.position_x + 250 &&
			Player.position_y + size / 2.0 > Lfood5.position_y &&
			Player.position_y + size / 2.0 < Lfood5.position_y + 125)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatzhadan"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatzhadan.mp3 alias eatzhadan"), NULL, 0, NULL);  
				mciSendString(_T("play eatzhadan"), NULL, 0, NULL);    
			}
			size = start_size;
			gamelife -= 1;
			Player.position_x = rand() % (1000 - 40);
			Player.position_y = rand() % (688 - 40);
		}
	}
	if (size > 185)
	{
		if (Rfood5.position_x + 125 > Player.position_x &&
			Rfood5.position_x + 125 < Player.position_x + size &&
			Rfood5.position_y + 62.5 > Player.position_y &&
			Rfood5.position_y + 62.5 < Player.position_y + size)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatfood"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatfood.mp3 alias eatfood"), NULL, 0, NULL);  
				mciSendString(_T("play eatfood"), NULL, 0, NULL);    
			}
			size += 2;
			Rfood5.position_x = -100;
			Rfood5.position_y = rand() % (688 - 125);
		}
	}
	//玩家与比自身大的鱼相撞
	else
	{
		if (Player.position_x + size / 2.0 > Rfood5.position_x &&
			Player.position_x + size / 2.0 < Rfood5.position_x + 250 &&
			Player.position_y + size / 2.0 > Rfood5.position_y &&
			Player.position_y + size / 2.0 < Rfood5.position_y + 125)
		{
			if (acoustics == 1)
			{
				mciSendString(_T("close eatzhadan"), NULL, 0, NULL);
				mciSendString(_T("open D:\\BigFishEatingSmallFish\\eatzhadan.mp3 alias eatzhadan"), NULL, 0, NULL);  
				mciSendString(_T("play eatzhadan"), NULL, 0, NULL);    
			}
			size = start_size;
			gamelife -= 1;
			Player.position_x = rand() % (1000 - 40);
			Player.position_y = rand() % (600);
		}
	}

}