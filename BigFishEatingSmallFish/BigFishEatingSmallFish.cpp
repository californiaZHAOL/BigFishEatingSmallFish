#include"BigFishEatingSmallFish.h"

int main()
{
	initgraph(LENGTH, HIGH);

	// ��ȡ���ھ��
	HWND hwnd = GetHWnd();
	//����Ϸ���������ڴ���
	SetWindowText(hwnd, _T("�����С��"));

	//�������ֵĲ���
	mciSendString(_T("open D:\\BigFishEatingSmallFish\\bkmusic.wma alias mybkmusic"), NULL, 0, NULL);
	mciSendString(_T("play mybkmusic repeat"), NULL, 0, NULL);    

	//������ͼ��ʼ
	BeginBatchDraw();

	//��ʼ����Ϸ����
	loop: loadimage(&imgMenu, _T("D:\\BigFishEatingSmallFish\\init.jpg"));   
	putimage(0, 0, &imgMenu);   

	//����ʼ����Ϸ����
	FlushBatchDraw();
	MOUSEMSG m;

	chose();//���û�ѡ��ģʽ������������Ч���ڣ������ǰ���																						 
	init();//��ʼ��
	
	while (1)
	{ 
		//ʱ��ģʽ���ж���Ϸ�Ƿ����
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
		//����ģʽ���ж���Ϸ�Ƿ����
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

		//����������Ϸ����
		draw();

		//ʶ������Ƿ�������ͣ��ť
		if (MouseHit())
		{
			m = GetMouseMsg();
			//��������
			if (m.mkLButton == true)
			{
				//������������ͣ��ť
				if (m.x > 900 && m.x < 1000 && m.y > 10 && m.y < 60)
				{
					//�����Ч�����ǿ��ŵģ����ŵ����Ч
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);
						mciSendString(_T("play click"), NULL, 0, NULL); 
					}
					pauseStartTime = time(NULL);
					//��ʾ��Ϸ��ͣ����
					gamepause();
					pauseEndTime = time(NULL);
					//������ͣ����ʱ��
					PauseTotalTime += (pauseEndTime - pauseStartTime);
					//����û�ѡ��������Ϸ
					if (again == 1)
					{
						again = 0;
						//��ת����ʼ����
						goto loop;
					}
				}
			}
		}

		//����м�����Ϣ
		if (_kbhit())
		{
			//�ı���ҵ���ķ���
			ChangeDir();
			//��ҵ����ƶ�
			playermove();
		}
		//���е�ʳ������ƶ�
		foodmove();

		//�ж�����Ƿ��ʳ��͵�����ײ
		judge();

		//�ж�ѭ���Ѿ�������һ�ˣ��������Ϸʧ������ж�
		flag = 1;
	}
	//ֹͣ������ͼ
	EndBatchDraw();
	return 0; 
}

void chose()
{
	// ���� XOR ��ͼģʽ
	setwritemode(R2_XORPEN); 
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255,1,255));
	//���ݱ������ֵĿ��ؾ����Ƿ񻭳�б��
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	//���ݱ�����Ч�Ŀ��ؾ����Ƿ񻭳�б��
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();

	MOUSEMSG m;
	while (1)
	{
		//��������
		if (MouseHit())
		{
			m = GetMouseMsg();
			//������������
			if (m.mkLButton == true)
			{
				//ʶ�𱳾�����ͼ����
				if (m.x > 13 && m.x < 79 && m.y > 11 && m.y < 75)
				{
					//�����Ч�����ǿ��ŵģ����ŵ����Ч
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);
						mciSendString(_T("play click"), NULL, 0, NULL);  
					}
					//���ߣ�XORģʽ����������ʧ��
					line(27, 20, 63, 69);
					if (bkmusic == 1)
					{
						//���ԭ���Ǵ򿪵ģ����ڹر�
						mciSendString(_T("close mybkmusic"), NULL, 0, NULL);
					}
					else
					{
						//���ԭ���ǹرյģ����ڴ�
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\bkmusic.wma alias mybkmusic"), NULL, 0, NULL);
						mciSendString(_T("play mybkmusic repeat"), NULL, 0, NULL);    
					}
					bkmusic = -bkmusic;
				}
				//ʶ����Чͼ����
				else if (m.x > 13 && m.x < 79 && m.y > 98 && m.y < 155)
				{
					//�����Ч�����ǿ��ŵģ����ŵ����Ч
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					//���ߣ�XORģʽ����������ʧ��
					line(19, 107, 53, 152);
					//��Ч�ɿ���Ϊ������������Ϊ��
					acoustics = -acoustics;
				}
				//����ʱ��ģʽ
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
				//��������ģʽ
				else if (m.x > 440 && m.x < 549 && m.y > 447 && m.y < 548)
				{
					//�����Ч�����ǿ��ŵģ����ŵ����Ч
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					//ģʽ��ʶ
					mode = 1;
					break;
				}
				//��ʾ������Ϣ
				else if (m.x > 601 && m.x < 702 && m.y > 444 && m.y < 554)
				{
					//�����Ч�����ǿ��ŵģ����ŵ����Ч
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					//��ʾ������Ϣ
					showhelp();
				}
			}
		}
		FlushBatchDraw();
	}
}

void showhelp()
{
	//���û��߷��
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	//����������Ч�Ŀ��ؾ����Ƿ���
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	//�����ʼ������
	loadimage(&imgMenu, _T("D:\\BigFishEatingSmallFish\\init.jpg"));   
	setbkmode(TRANSPARENT);	//�������屳��ɫΪ͸��
	settextcolor(RED);//����������ɫΪRED
	settextstyle(50, 0, _T("����"));
	//���������ʾ������Ϣ
	outtextxy(200, 50, "ʱ��ģʽ�ڹ涨ʱ������Ϸ");
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	Sleep(1000);
	cleardevice();//����
	putimage(0, 0, &imgMenu);   
	outtextxy(200, 50, "����ģʽ�ڹ涨����ֵ����Ϸ");
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	//����һ��ʱ�䣬���û���ʱ�����������Ϣ
	Sleep(2000);
	cleardevice();//����
	putimage(0, 0, &imgMenu);   
	outtextxy(200, 50, "AWDS��JILK�����ƶ��Ե�С��");
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	Sleep(2000);
	cleardevice();//����
	putimage(0, 0, &imgMenu);   
	outtextxy(310, 50, "�Ե���������ʱ��");
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	Sleep(2000);
	cleardevice();//����
	putimage(0, 0, &imgMenu);   
	outtextxy(310, 50, "�Ե�ҩˮ��������ֵ");
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	Sleep(2000);
	cleardevice();//����
	putimage(0, 0, &imgMenu);   
	outtextxy(150, 50, "�Ե�ը�����ߴ������ʱ�������ֵ");
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	if (bkmusic == -1)
		line(27, 20, 63, 69);
	if (acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	Sleep(2000);
	cleardevice();//����
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
	playerDir = 'D';//��ҵ���ĳ�ʼ�ƶ���������
	headDir = 'D';//��ҵ���ĳ�ʼ��ͷ��������
	score = 0;//��ҵĵ÷�
	size = 40;//����һ�����������������ĳߴ�
	start_size = 40;//����һ��������������ҵĳ�ʼ��С
	gametime = 120;//ʣ�����Ϸʱ��
	alltime = 120;//��ʱ��
	gamelife = 3;//ʣ�����Ϸ��������
	flag = 0;//��־��Ϸ�Ƿ�ոտ�ʼ
	PauseTotalTime = 0;

	//�������ͼƬ
	loadimage(&timebk, _T("D:\\BigFishEatingSmallFish\\timeBackground.jpg"));   
	loadimage(&timebk, _T("D:\\BigFishEatingSmallFish\\timeBackground.jpg"));   
	loadimage(&lifebk, _T("D:\\BigFishEatingSmallFish\\lifeBackground.jpg"));   
	
	loadimage(&end, _T("D:\\BigFishEatingSmallFish\\end.jpg"));   

	//�������������ֵ
	srand((unsigned)time(NULL));

	//�������ͼƬ������ʵ���С
	loadimage(&timer, _T("D:\\BigFishEatingSmallFish\\timer.jpg"), 35, 35, true);
	loadimage(&timerbk, _T("D:\\BigFishEatingSmallFish\\timerbk.jpg"), 35, 35, true);//����

	loadimage(&yao1, _T("D:\\BigFishEatingSmallFish\\yao1.jpg"), 35, 35, true);
	loadimage(&yao1bk, _T("D:\\BigFishEatingSmallFish\\yao1bk.jpg"), 35, 35, true);//ҩˮ

	loadimage(&yao2, _T("D:\\BigFishEatingSmallFish\\yao2.jpg"), 35, 35, true);
	loadimage(&yao2bk, _T("D:\\BigFishEatingSmallFish\\yao2bk.jpg"), 35, 35, true);//ҩˮ

	loadimage(&zhadan1, _T("D:\\BigFishEatingSmallFish\\zhadan1.jpg"), 40, 40, true);
	loadimage(&zhadan1bk, _T("D:\\BigFishEatingSmallFish\\zhadan1bk.jpg"), 40, 40, true);//ը��
	loadimage(&zhadan2, _T("D:\\BigFishEatingSmallFish\\zhadan2.jpg"), 40, 40, true);
	loadimage(&zhadan2bk, _T("D:\\BigFishEatingSmallFish\\zhadan2bk.jpg"), 40, 40, true);//ը��

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

	//��ʼ����ҵ���
	Player.position_x = LENGTH / 2.0;//��Һ�����
	Player.position_y = HIGH / 2.0;//���������
	Player.v_x = 8;//���ˮƽ�ٶ�
	Player.v_y = 8;//�����ֱ�ٶ�

	//����ÿ����������ĺ�����������ı�ÿ��ʳ������ǵ��ߵĳ���Ƶ��
	LTimer.position_x = 1100;//���Ӻ�����
	LTimer.position_y = rand() % 633 + 50;//��������������趨
	LTimer.v_x = 0.6;//�����ٶ�
	LTimer.v_y = (rand() % 2) / 10.0;//�����ٶ�

	Timer.position_x = -1000;//���Ӻ�����
	Timer.position_y = rand() % 633 + 50;//��������������趨
	Timer.v_x = 0.6;//�����ٶ�
	Timer.v_y = (rand() % 2) / 10.0;//�����ٶ�

	Yao1.position_x = 1100;
	Yao1.position_y = rand() % 633 + 50;
	Yao1.v_x = 0.6;
	Yao1.v_y = (rand() % 2) / 10.0;

	Yao2.position_x = -1000;
	Yao2.position_y = rand() % 633 + 50;
	Yao2.v_x = 0.6;
	Yao2.v_y = (rand() % 2) / 10.0;

	Zhadan1.position_x = 1800;//������
	Zhadan1.position_y = rand() % 633 + 50;//����������趨
	Zhadan1.v_x = 0.6;//�ٶ�
	Zhadan1.v_y = (rand() % 2) / 10.0;//�ٶ�

	Zhadan2.position_x = -1100;//������
	Zhadan2.position_y = rand() % 633 + 50;//����������趨
	Zhadan2.v_x = 0.6;//�ٶ�
	Zhadan2.v_y = (rand() % 2) / 10.0;//�ٶ�

	Lfood1.position_x = 1000;//����1���������
	Lfood1.position_y = rand() % 633 + 50;//����������������趨
	Lfood1.v_x = 0.4;//����1�����ٶ�
	Rfood1.position_x = -(rand() % 100);//����1���������
	Rfood1.position_y = rand() % 633 + 50;//����1��������������趨
	Rfood1.v_x = 0.3;//����1�����ٶ�

	Lfood2.position_x = 1300;//����2���������
	Lfood2.position_y = rand() % 633 + 50;//����2��������������趨
	Lfood2.v_x = 0.45;//����2�����ٶ�
	Rfood2.position_x = -rand() % 300;//����2���������
	Rfood2.position_y = rand() % 633 + 50;//����2��������������趨
	Rfood2.v_x = 0.55;//����2�����ٶ�

	Lfood3.position_x = 1800;//����3���������
	Lfood3.position_y = rand() % 600 + 88;//����3��������������趨
	Lfood3.v_x = 0.43;//����3�����ٶ�
	Rfood3.position_x = - rand() % 1800;//����3���������
	Rfood3.position_y = rand() % 600 + 88;//����3��������������趨
	Rfood3.v_x = 0.43;//����3�����ٶ�

	Lfood4.position_x = 2500;//����4���������
	Lfood4.position_y = rand() % 560 + 128;//����4��������������趨
	Lfood4.v_x = 0.42;//����4�����ٶ�
	Rfood4.position_x = - rand() % 2500;//����4���������
	Rfood4.position_y = rand() % 560 + 128;//����4��������������趨
	Rfood4.v_x = 0.42;//����4�����ٶ�

	Lfood5.position_x = 3000;//����5���������
	Lfood5.position_y = rand() % 433 + 250;//����5��������������趨
	Lfood5.v_x = 0.4;//����5�����ٶ�
	Rfood5.position_x = -rand() % 3000;//����5���������
	Rfood5.position_y = rand() % 433 + 250;//����5��������������趨
	Rfood5.v_x = 0.4;//����5�����ٶ�

	//�����ʱ��ģʽ�¾ͼ�ʱ
	if (mode == 0)
	{
		beginTime = time(NULL);
	}
}

void ChangeDir()
{
	//�ж��û�������
		switch (_getch())
		{
		case 'A':
		case 'a':
		case 'J':
		case 'j':
			if (headDir == 'D') headDir = 'A'; //�ж���ͷ�Ƿ�ת��
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
	setbkmode(TRANSPARENT);	//�������屳��ɫΪ͸��
	settextstyle(45, 0, _T("����"));
	settextcolor(RED);

	//���ݾ����ģʽѡ�����ı���ͼƬ
	if (mode == 0)
	{
		putimage(0, 0, &timebk);
	}
	else
	{
		putimage(0, 0, &lifebk);
	}
	outtextxy(900, 5, _T("��ͣ"));

	//������ҵ���ͷ�ķ�����ʾ��ҵ���
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

	//�������ѡ���ģʽ��ѡ������ʾ����ͼƬ����ҩˮͼƬ
	if (mode == 0)
	{
		putimage((int)Timer.position_x, (int)Timer.position_y, &timerbk, NOTSRCERASE);//��ʾ����
		putimage((int)Timer.position_x, (int)Timer.position_y, &timer, SRCINVERT);
		putimage((int)LTimer.position_x, (int)LTimer.position_y, &timerbk, NOTSRCERASE);//��ʾ����
		putimage((int)LTimer.position_x, (int)LTimer.position_y, &timer, SRCINVERT);
	}
	else
	{
		putimage((int)Yao1.position_x, (int)Yao1.position_y, &yao1bk, SRCAND);//��ʾҩ
		putimage((int)Yao1.position_x, (int)Yao1.position_y, &yao1, SRCPAINT);
		putimage((int)Yao2.position_x, (int)Yao2.position_y, &yao2bk, SRCAND);//��ʾҩ
		putimage((int)Yao2.position_x, (int)Yao2.position_y, &yao2, SRCPAINT);
	}

	putimage((int)Zhadan1.position_x, (int)Zhadan1.position_y, &zhadan1bk, SRCAND);//��ʾը��
	putimage((int)Zhadan1.position_x, (int)Zhadan1.position_y, &zhadan1, SRCPAINT);
	putimage((int)Zhadan2.position_x, (int)Zhadan2.position_y, &zhadan2bk, SRCAND);//��ʾը��
	putimage((int)Zhadan2.position_x, (int)Zhadan2.position_y, &zhadan2, SRCPAINT);

	putimage((int)Lfood1.position_x, (int)Lfood1.position_y, &L_food1bk, SRCAND);//��ʾ1����
	putimage((int)Lfood1.position_x, (int)Lfood1.position_y, &L_food1, SRCPAINT);
	putimage((int)Rfood1.position_x, (int)Rfood1.position_y, &R_food1bk, SRCAND);//��ʾ����1����
	putimage((int)Rfood1.position_x, (int)Rfood1.position_y, &R_food1, SRCPAINT);

	putimage((int)Lfood2.position_x, (int)Lfood2.position_y, &L_food2bk, SRCAND);//��ʾ2����
	putimage((int)Lfood2.position_x, (int)Lfood2.position_y, &L_food2, SRCPAINT);
	putimage((int)Rfood2.position_x, (int)Rfood2.position_y, &R_food2bk, NOTSRCERASE);//��ʾ����2����
	putimage((int)Rfood2.position_x, (int)Rfood2.position_y, &R_food2, SRCINVERT);

	putimage((int)Lfood3.position_x, (int)Lfood3.position_y, &L_food3bk, SRCAND);//��ʾ3����
	putimage((int)Lfood3.position_x, (int)Lfood3.position_y, &L_food3, SRCPAINT);
	putimage((int)Rfood3.position_x, (int)Rfood3.position_y, &R_food3bk, SRCAND);//��ʾ����3����
	putimage((int)Rfood3.position_x, (int)Rfood3.position_y, &R_food3, SRCPAINT);

	putimage((int)Lfood4.position_x, (int)Lfood4.position_y, &L_food4bk, SRCAND);//��ʾ4����
	putimage((int)Lfood4.position_x, (int)Lfood4.position_y, &L_food4, SRCPAINT);
	putimage((int)Rfood4.position_x, (int)Rfood4.position_y, &R_food4bk, SRCAND);//��ʾ����4����
	putimage((int)Rfood4.position_x, (int)Rfood4.position_y, &R_food4, SRCPAINT);

	putimage((int)Lfood5.position_x, (int)Lfood5.position_y, &L_food5bk, SRCAND);//��ʾ5����
	putimage((int)Lfood5.position_x, (int)Lfood5.position_y, &L_food5, SRCPAINT);
	putimage((int)Rfood5.position_x, (int)Rfood5.position_y, &R_food5bk, NOTSRCERASE);//��ʾ����5����
	putimage((int)Rfood5.position_x, (int)Rfood5.position_y, &R_food5, SRCINVERT);

	if (headDir == 'D')
	{
		putimage((int)Player.position_x, (int)Player.position_y, &R_bkfish_player, SRCAND);//��ʾ���
		putimage((int)Player.position_x, (int)Player.position_y, &R_fish_player, SRCPAINT);
	}
	else
	{
		putimage((int)Player.position_x, (int)Player.position_y, &L_bkfish_player, SRCAND);//��ʾ���
		putimage((int)Player.position_x, (int)Player.position_y, &L_fish_player, SRCPAINT);
	}
	
	setbkmode(TRANSPARENT);	//�������屳��ɫΪ͸��
	settextstyle(45, 0, _T("����"));
	settextcolor(RED);

	//ʱ��ģʽ����ʾ��Ϸʣ���ʱ��
	if (mode == 0)
	{
		nowTime = time(NULL);
		CHAR times[10];
		gametime = alltime - (nowTime - beginTime) + PauseTotalTime;
		wsprintf(times, "%ld", (gametime));
		outtextxy(160, 4, times);
	}
	//����ģʽ����ʾʣ�������
	else
	{
		CHAR lifes[10];
		wsprintf(lifes, "%d", gamelife);
		outtextxy(160, 4, lifes);
	}

	//��ʾ��ҵķ���
	score = (int)(size - start_size);
	CHAR scores[10];
	wsprintf(scores, "%d", score);
	outtextxy(830, 4, scores);

	FlushBatchDraw();
}

void playermove()
{
	//������ҵ��������ı���ҵ����������
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
	//�ж���ҵ����Ƿ��ε��˽���ľ�ͷ
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
	//����ʳ����͵��ߵ�����ı�
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

	//���ʳ��������ǵ����γ��߽�
	//�����������һ��
	if (Lfood1.position_x <= -40)
	{
		Lfood1.position_x = 1000;//����1���������
		Lfood1.position_y = rand() % 633 + 50;//����������������趨
	}
	if (Rfood1.position_x >= 1000)
	{
		Rfood1.position_x = -(rand() % 10);//����1���������
		Rfood1.position_y = rand() % 633 + 50;//����1��������������趨
	}

	if (Lfood2.position_x <= -65)
	{
		Lfood2.position_x = 1300;//����2���������
		Lfood2.position_y = rand() % 600 + 40;//����2��������������趨
	}
	if (Rfood2.position_x >= 1000)
	{
		Rfood2.position_x = -rand() % 300;//����2���������
		Rfood2.position_y = rand() % 600 + 40;//����2��������������趨
	}

	if (Lfood3.position_x <= -80)
	{
		Lfood3.position_x = 1800;//����3���������
		Lfood3.position_y = rand() % 600 + 48;//����3��������������趨
	}
	if (Rfood3.position_x >= 1800)
	{
		Rfood3.position_x = -rand() % 1800;//����3���������
		Rfood3.position_y = rand() % 600 + 48;//����3��������������趨
	}

	if (Lfood4.position_x <= -120)
	{
		Lfood4.position_x = 2500;//����4���������
		Lfood4.position_y = rand() % 560 + 58;//����4��������������趨
	}
	if (Rfood4.position_x >= 1000)
	{
		Rfood4.position_x = - rand() % 2500;//����4���������
		Rfood4.position_y = rand() % 560 + 58;//����4��������������趨
	}


	if (Lfood5.position_x <= -250)
	{
		Lfood5.position_x = 3000;//����5���������
		Lfood5.position_y = rand() % 400;//����5��������������趨
	}
	if (Rfood5.position_x >= 1000)
	{
		Rfood5.position_x = -rand() % 3000;//����5���������
		Rfood5.position_y = rand() % 400;//����5��������������趨
	}

	if (Timer.position_x >= 1000)
	{
		Timer.position_x = -1000;//���Ӻ�����
		Timer.position_y = rand() % 500;//��������������趨
		Timer.v_y = (rand() % 2) / 10.0;//�����ٶ�
	}

	if (LTimer.position_x <= -35)
	{
		LTimer.position_x = 1600;//���Ӻ�����
		LTimer.position_y = rand() % 500;//��������������趨
		LTimer.v_y = (rand() % 2) / 10.0;//�����ٶ�
	}

	if (Yao2.position_x >= 1000)
	{
		Yao2.position_x = -1000;//ҩˮ������
		Yao2.position_y = rand() % 500;//ҩˮ����������趨
		Yao2.v_y = (rand() % 2) / 10.0;//ҩˮ�ٶ�
	}

	if (Yao1.position_x <= -35)
	{
		Yao1.position_x = 1600;//ҩˮ������
		Yao1.position_y = rand() % 500;//ҩˮ����������趨
		Yao1.v_y = (rand() % 2) / 10.0;//ҩˮ�ٶ�
	}

	if (Zhadan1.position_x <= -40)
	{
		Zhadan1.position_x = 1600;//������
		Zhadan1.position_y = rand() % 500;//����������趨
		Zhadan1.v_y = (rand() % 2) / 10.0;//�ٶ�
	}
	if (Zhadan2.position_x >= 1000)
	{
		Zhadan2.position_x = -800;//������
		Zhadan2.position_y = rand() % 500;//����������趨
		Zhadan2.v_y = (rand() % 2) / 10.0;//�ٶ�
	}
}

void gameover()
{
	cleardevice();//����
	//��ʾ���������ͼƬ
	putimage(0, 0, &end);
	//����÷�
	score = (int)(size - start_size);
	CHAR scores[15];
	//�жϷ����Ƿ�ﵽͨ�ص�����
	if (score >= 320)
	{
		if (acoustics == 1)
		{
			mciSendString(_T("close win"), NULL, 0, NULL);
			mciSendString(_T("open D:\\BigFishEatingSmallFish\\win.wma alias win"), NULL, 0, NULL);  
			mciSendString(_T("play win"), NULL, 0, NULL);    
		}
		wsprintf(scores, "%d ��ͨ���ˣ�", score);
	}
	else
		wsprintf(scores, "%d", score);
	outtextxy(580, 310, scores);

	FlushBatchDraw();

	MOUSEMSG m;
	//ʶ�����ĵ��
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			if (m.mkLButton == true)
			{
				//ѡ�������һ����Ϸ
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
				//ѡ�������Ϸ
				else if (m.x > 440 && m.x < 580 && m.y > 540 && m.y < 620)
				{
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					mciSendString(_T("close mybkmusic"), NULL, 0, NULL);
					MessageBox(GetHWnd(), "            ��Ϸ����", "SORRY", MB_OK);
					exit(0);
				}
			}
		}
	}
}

void gamepause()
{
	//���벢����ʾ��Ϸ��ͣ�Ľ���
	loadimage(&pause, _T("D:\\BigFishEatingSmallFish\\pause.jpg"));   
	putimage(0, 0, &pause);   
	//����XOR��ͼģʽ
	setwritemode(R2_XORPEN); 
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	//�ж�������Ч�Ŀ��ز��һ�ͼ
	if(bkmusic == -1)
		line(27, 20, 63, 69);
	if(acoustics == -1)
		line(19, 107, 53, 152);
	FlushBatchDraw();
	MOUSEMSG m;
	while (1)
	{
		//ʶ�����ĵ��
		if (MouseHit())
		{
			m = GetMouseMsg();
			if (m.mkLButton == true)
			{
				//ʶ�𱳾�����ͼ����
				if (m.x > 13 && m.x < 79 && m.y > 11 && m.y < 75)
				{
					//�ж���Ч�Ƿ����������Ƿ񲥷����ĵ������
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					//XORģʽ���ٻ�һ�Σ��ߴ��޵��У����е���
					line(27, 20, 63, 69);
					//�ж������Ƿ����������Ƿ񲥷ű�������
					//��ǰ�Ǵ򿪵ģ����ھ͹ر�
					if (bkmusic == 1)
					{
						mciSendString(_T("close mybkmusic"), NULL, 0, NULL);
					}
					//��ǰ�ǹرյģ����ھʹ�
					else
					{
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\bkmusic.wma alias mybkmusic"), NULL, 0, NULL);
						mciSendString(_T("play mybkmusic repeat"), NULL, 0, NULL);    
					}
					bkmusic = -bkmusic;
				}
				if (m.x > 13 && m.x < 79 && m.y > 98 && m.y < 155)
				{
					//ʶ����Чͼ����
					//�ж���Ч�Ƿ����������Ƿ񲥷����ĵ������
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
					//�˳�������Ϸ
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					//����һ��
					again = 1;
					break;
				}
				else if (m.x > 346 && m.x < 582 && m.y > 267 && m.y < 336)
				{
					//�˳���Ϸ
					if (acoustics == 1)
					{
						mciSendString(_T("close click"), NULL, 0, NULL);
						mciSendString(_T("open D:\\BigFishEatingSmallFish\\click.wma alias click"), NULL, 0, NULL);  
						mciSendString(_T("play click"), NULL, 0, NULL);    
					}
					mciSendString(_T("close mybkmusic"), NULL, 0, NULL);
					bkmusic = 1;
					MessageBox(GetHWnd(), "            ��Ϸ����", "SORRY", MB_OK);
					exit(0);
				}
				else if (m.x > 348 && m.x < 582 && m.y > 393 && m.y < 460)
				{
					//������Ϸ
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
		//�ж������Ƿ񱻳�
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
		//�ж�ҩˮ�Ƿ񱻳�
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

	
	//�ж�ը���Ƿ񱻳�
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

	//�ж�һ�����Ƿ񱻳�
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


	//�ж�2�����Ƿ񱻳�
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
	//����������������ײ
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
	//����������������ײ
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


	//�ж�3�����Ƿ񱻳�
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
	//����������������ײ
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
	//����������������ײ
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


	//�ж�4�����Ƿ񱻳�
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
	//����������������ײ
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
	//����������������ײ
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
	//�ж�5�����Ƿ񱻳�
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
	//����������������ײ
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
	//����������������ײ
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