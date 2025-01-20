/*
项目源地址：https://github.com/wasabijc/Chinese-Chess.git
转载请注明出处
*/

#define _CRT_SECURE_NO_WARNINGS

#undef UNICODE
#undef _UNICODE

#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<graphics.h>
#include <mmsystem.h> 
#include <thread>

#pragma comment(lib, "winmm.lib") 

using namespace std;


#define BUTTONTEXT RGB(30,30,30)      //开始界面字体颜色
#define OVERBUTTON RGB(60,60,60)      //开始界面鼠标经过按钮字体变灰
#define BUTTONCOLOR RGB(200,130,100)  //开始界面按钮颜色
#define BGColor RGB(200,130,100)       //游戏界面棋子颜色
//游戏界面窗口规格
#define Height 500
#define Width 452
#define Margin 30      //背景图片棋盘的边距
#define GZ 49          //棋盘格子宽度
#define ChessSize 20   //棋子半径


class UI;
class Chess;
class ChessBoard;

class Chess//棋子类
{
public:
	Chess(int type, int posX, int posY)
	{
		this->type = type;
		this->posX = posX;
		this->posY = posY;
	}

	bool operator==(const Chess& obj)
	{
		if (this->type == obj.type && this->posX == obj.posX && this->posY == obj.posY)
			return true;
	}

	int type;//棋子类型
	int posX, posY;//棋子位置
};

class ChessBoard//棋盘类
{
public:
	friend UI;

	ChessBoard()
	{
		height = 10;
		width = 9;
		//先把所有棋子类型设为0
		Board.assign(height, vector<int>(width, 0));

		//载入棋子信息
		BoardInit();
		ChessInit();
		//更新棋盘
		BoardUpdate();
	}

	//将红黑两个列表更新入Board
	void BoardUpdate()
	{
		//这里先对两个列表清零
		Board.assign(height, vector<int>(width, 0));
		//更新剩余棋子
		for (vector<Chess>::iterator i = RedList.begin(); i != RedList.end(); ++i)
		{
			Board[i->posX][i->posY] = i->type;
		}
		for (vector<Chess>::iterator i = BlackList.begin(); i != BlackList.end(); ++i)
		{
			Board[i->posX][i->posY] = i->type;
		}

	}

	//游戏开始初始化界面
	void initGraph()
	{
		initgraph(Width, Height);
		//画棋盘和棋子
		drawMap();
		drawPiece();

		mciSendString("stop init2", NULL, 0, NULL);
		mciSendString("close init2", NULL, 0, NULL);
		// 打开音频文件
		mciSendString("open sounds/chuji.mp3 alias init2", NULL, 0, NULL);
		if (isFirstTime)
		{
			//第一次打开游戏页面，由于开始页面点击的按钮有三秒的音频，所以游戏页面的开局声音延迟三秒播放
			//游戏结束后重开则会立即播放
			this_thread::sleep_for(chrono::seconds(3));
		}
		// 播放音频文件
		mciSendString("play init2 ", NULL, 0, NULL);

	}
	//游戏运行函数
	bool game(ExMessage& operation)
	{
		//鼠标左键按下进行的操作
		if (operation.message == WM_LBUTTONDOWN)
		{
			if (!isSelected)
			{
				//如果没有棋子被选择，进入选择模式
				MessageProcessFirst(operation);
			}
			else
			{
				// 若已有棋子被选择，进入移动模式
				MessageProcessSecond(operation);
			}
		}

		if (WinCheck())
		{
			return true;
		}
		return false;
	}

private:
	int height, width;//棋盘大小，固定为10*9
	vector<vector<int> > Board;//棋盘，存储棋盘中每个点的状态

	map<int, string> reflex;//棋子的类型与汉字名称对应表
	vector<Chess> RedList, BlackList;//保存红方和黑方的剩余棋子

	int SelectX = -1, SelectY = -1;//选择到的棋子的坐标
	bool isSelected;        //是否有棋子被选择
	bool isFirstTime = true;  //是否是第一次游戏
	bool isRed = true;      //是否是红方回合
	bool WasRule = false;   //鼠标的操作是否符合移动规则

	//棋子类型名称对应表，用来画棋子
	void BoardInit()
	{
		reflex[0] = "   ";
		reflex[1] = "帅";
		reflex[2] = "仕";
		reflex[3] = "相";
		reflex[4] = "马";
		reflex[5] = "车";
		reflex[6] = "仕";
		reflex[7] = "相";
		reflex[8] = "马";
		reflex[9] = "车";
		reflex[10] = "炮";
		reflex[11] = "炮";
		reflex[12] = "卒";
		reflex[13] = "卒";
		reflex[14] = "卒";
		reflex[15] = "卒";
		reflex[16] = "卒";

		reflex[17] = "将";
		reflex[18] = "士";
		reflex[19] = "象";
		reflex[20] = "馬";
		reflex[21] = "車";
		reflex[22] = "士";
		reflex[23] = "象";
		reflex[24] = "馬";
		reflex[25] = "車";
		reflex[26] = "砲";
		reflex[27] = "砲";
		reflex[28] = "兵";
		reflex[29] = "兵";
		reflex[30] = "兵";
		reflex[31] = "兵";
		reflex[32] = "兵";
	}
	void ChessInit()//棋子初始化
	{
		//(编号,行,列)
		
		//红方 编号<=16
		//红帅
		RedList.emplace_back(1, 0, 4);
		//红士
		RedList.emplace_back(2, 0, 3);
		RedList.emplace_back(6, 0, 5);
		//红相
		RedList.emplace_back(3, 0, 2);
		RedList.emplace_back(7, 0, 6);
		//红马
		RedList.emplace_back(4, 0, 1);
		RedList.emplace_back(8, 0, 7);
		//红车
		RedList.emplace_back(5, 0, 0);
		RedList.emplace_back(9, 0, 8);
		//红炮
		RedList.emplace_back(10, 2, 1);
		RedList.emplace_back(11, 2, 7);
		//红兵
		RedList.emplace_back(12, 3, 0);
		RedList.emplace_back(13, 3, 2);
		RedList.emplace_back(14, 3, 4);
		RedList.emplace_back(15, 3, 6);
		RedList.emplace_back(16, 3, 8);


		//黑方 编号>16
		//黑将
		BlackList.emplace_back(17, 9, 4);
		//黑士
		BlackList.emplace_back(18, 9, 3);
		BlackList.emplace_back(22, 9, 5);
		//黑相
		BlackList.emplace_back(19, 9, 2);
		BlackList.emplace_back(23, 9, 6);
		//黑马
		BlackList.emplace_back(20, 9, 1);
		BlackList.emplace_back(24, 9, 7);
		//黑车
		BlackList.emplace_back(21, 9, 0);
		BlackList.emplace_back(25, 9, 8);
		//黑炮
		BlackList.emplace_back(26, 7, 1);
		BlackList.emplace_back(27, 7, 7);
		//黑兵
		BlackList.emplace_back(28, 6, 0);
		BlackList.emplace_back(29, 6, 2);
		BlackList.emplace_back(30, 6, 4);
		BlackList.emplace_back(31, 6, 6);
		BlackList.emplace_back(32, 6, 8);
	}

	//胜利结算画面
	bool WinCheck()
	{
		int ret = 0;
		if (RedList[0].type != 1 || RedList.size() == 0)
		{
			ret = 1;   //红胜
		}
		else if (BlackList[0].type != 17 || BlackList.size() == 0)
		{
			ret = 2;   //黑胜
		}
		if (ret != 0)
		{
			int checkHeight = 300;
			int checkWidth = 400;
			initgraph(checkWidth, checkHeight);

			// 播放音频
			mciSendString("open sounds/victory.mp3 alias victory", NULL, 0, NULL);
			mciSendString("play victory ", NULL, 0, NULL);


			IMAGE background;//定义一个图片名.
			loadimage(&background, "images/wincheck.png", checkWidth, checkHeight, 1);//从图片文件获取图像
			putimage(0, 0, &background);

			settextcolor(RGB(240, 230, 30));//红底金色的结算界面
			LOGFONT f;
			gettextstyle(&f);						// 获取当前字体设置
			f.lfHeight = 40;						// 设置字体高度为 40
			_tcscpy_s(f.lfFaceName, _T("黑体"));	// 设置字体为“黑体”
			f.lfQuality = ANTIALIASED_QUALITY;		// 输出效果为抗锯齿
			// 设置倾斜度
			f.lfEscapement = -190;           //每个字体围绕中心旋转角度
			f.lfOrientation = -190;          //整一行字倾斜
			setbkmode(TRANSPARENT);                 //透明背景
			settextstyle(&f);

			if (ret == 1)
			{
				outtextxy(145, 92, _T("黑方胜利"));
			}
			else if (ret == 2)
			{
				outtextxy(145, 92, _T("红方胜利"));
			}

			f.lfHeight = 30;						// 设置字体高度为 30
			// 恢复倾斜度为0
			f.lfEscapement = 0;
			f.lfOrientation = 0;
			settextstyle(&f);
			settextcolor(RGB(148, 144, 236));
			outtextxy(checkWidth / 2 - 135, checkHeight / 2 + 60, _T("按下回车键重新开始"));
			outtextxy(checkWidth / 2 - 135, checkHeight / 2 + 100, _T("按下ESC键退出"));
			//获取键盘操作
			ExMessage isDown;
			while (1)
			{
				peekmessage(&isDown, -1, 1);
				if (isDown.vkcode == VK_RETURN)
				{
					restart();
					break;
				}
				else if (isDown.vkcode == VK_ESCAPE)
				{
					return true;
				}
			}
		}
		return false;
	}



	//绘制棋盘
	void drawMap()
	{
		IMAGE background;//定义一个图片名.
		loadimage(&background, "images/chessboard.png", 452, 500, 1);//从图片文件获取图像
		putimage(0, 0, &background);  //加载图像
	}

	//绘制棋子
	void drawPiece()
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (Board[i][j] != 0)
				{
					if (Board[i][j] > 16)
					{
						setlinecolor(BLACK);
						settextcolor(BLACK);
					}
					else
					{
						setlinecolor(RED);
						settextcolor(RED);
					}
					TCHAR s[8];
					//棋子名称
					_stprintf(s, _T("%s"), reflex[Board[i][j]].c_str());

					setfillcolor(BGColor);        //棋子的颜色
					fillcircle(Margin + GZ * j, Margin + GZ * i, ChessSize);

					LOGFONT f;
					gettextstyle(&f);						// 获取当前字体设置
					f.lfHeight = 20;						// 设置字体高度为 20
					_tcscpy_s(f.lfFaceName, _T("黑体"));		// 设置字体为“黑体”
					f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿
					setbkmode(TRANSPARENT);
					settextstyle(&f);
					outtextxy(Margin + GZ * j - 10, Margin + GZ * i - 10, s);
				}
			}
		}
	}


	//处理选择模式下鼠标点击操作的函数
	void MessageProcessFirst(ExMessage operation)
	{
		int posx, posy;
		int mx, my;
		mx = (operation.x - Margin - GZ / 2);
		my = (operation.y - Margin - GZ / 2);
		//鼠标点击的坐标转换为相对于棋盘上的坐标。
		if (mx > 0)
			//相对坐标所在格子的左边距
			posx = Margin + GZ * ((mx / GZ) + 1);
		else
		{
			posx = Margin;
			mx = -GZ;
		}
		if (my > 0)
			posy = Margin + GZ * ((my / GZ) + 1);
		else
		{
			posy = Margin;
			my = -GZ;
		}

		//检测棋子是否与点击的坐标相同，相同则取消选中
		if (SelectX == (my / GZ + 1) && SelectY == (mx / GZ + 1))
		{
			isSelected = 0;
			SelectPiece(posx, posy, isSelected);
			return;
		}
		if (Board[my / GZ + 1][mx / GZ + 1] != 0 && Board[my / GZ + 1][mx / GZ + 1] <= 16 && isRed)
		{
			//如果在红方回合点击红方棋子则会选中
			//选中有蓝色方框提示
			isSelected = 1;
			SelectPiece(posx, posy,isSelected);
			SelectX = my / GZ + 1;
			SelectY = mx / GZ + 1;
		}
		else if (Board[my / GZ + 1][mx / GZ + 1] > 16 && !isRed)
		{
			//如果在黑方回合点击黑方棋子则会选中
			isSelected = 1;
			SelectPiece(posx, posy,isSelected);
			SelectX = my / GZ + 1;
			SelectY = mx / GZ + 1;
		}

	}

	//处理移动模式下鼠标点击操作的函数
	void MessageProcessSecond(ExMessage operation)
	{
		int posx, posy;
		int mx, my;
		//相同的操作：把鼠标点击的坐标转化成相对于棋盘的坐标
		mx = (operation.x - Margin - GZ / 2);
		my = (operation.y - Margin - GZ / 2);
		if (mx > 0)
			posx = Margin + GZ * ((mx / GZ) + 1);    //相对坐标所在格子的左边距
		else
		{
			posx = Margin;         //点击棋盘的外部，左边距就是棋盘的边距
			mx = -GZ;
		}
		if (my > 0)
			posy = Margin + GZ * ((my / GZ) + 1);    //相对坐标所在格子的上边距
		else
		{
			posy = Margin;
			my = -GZ;
		}
		//把相对的坐标转换成相对的行和列
		int i = my / GZ + 1;
		int j = mx / GZ + 1;
		if (SelectX == i && SelectY == j)
		{
			isSelected = 1;
			return;
		}

		//把之前选中的棋子移动到现在选中的地方
		movePiece(SelectX, SelectY, i, j, operation);
		SelectX = i;
		SelectY = j;
	}


	//用蓝色方框提示选中的棋子
	void SelectPiece(int posx, int posy,int isSelected)
	{
		BeginBatchDraw();
		FlushBatchDraw();

		//加载棋盘和棋子
		drawMap();
		drawPiece();

		//设置蓝框格式
		if (isSelected)
		{
			setlinecolor(BLUE);
			rectangle(posx - 20, posy - 20, posx + 20, posy + 20);
		}


		EndBatchDraw();
	}

	//将军时发出音效
	void jiang() {

		for (int m = 1; m < RedList.size(); m++)    //遍历红方剩余的棋子
		{
			//把每个红棋的位置设为当前位置
			int x = RedList[m].posX;
			int y = RedList[m].posY;
			//把黑将的位置设为目标位置
			int i = BlackList[0].posX;
			int j = BlackList[0].posY;

			//把除了帅以外的其他棋子与黑将的坐标传入规则，判断黑将是否会受到威胁，受到威胁会发出声音
			if (Rule(x, y, i, j))
			{
				//关闭上次播放
				mciSendString("stop jun", NULL, 0, NULL);
				mciSendString("close jun", NULL, 0, NULL);
				//播放本次
				mciSendString("open sounds/jiangjun.mp3 alias jun", NULL, 0, NULL);
				mciSendString("play jun ", NULL, 0, NULL);
			}

		}

		for (int m = 1; m < BlackList.size(); m++)    //遍历黑方剩余的棋子
		{
			//把每个黑棋的位置设为当前位置
			int x = BlackList[m].posX;
			int y = BlackList[m].posY;
			//把红帅的位置设置成目标位置
			int i = RedList[0].posX;
			int j = RedList[0].posY;

			//把除了将以外的其他棋子与红帅的坐标传入移动规则，判断红帅是否会受到威胁，受到威胁会发出声音
			if (Rule(x, y, i, j))
			{
				//关闭上次播放
				mciSendString("stop jun", NULL, 0, NULL);
				mciSendString("close jun", NULL, 0, NULL);
				//播放本次
				mciSendString("open sounds/jiangjun.mp3 alias jun", NULL, 0, NULL);
				mciSendString("play jun ", NULL, 0, NULL);
			}

		}

	}


	//移动棋子函数
	void movePiece(int x, int y, int i, int j, ExMessage& operation)
	{
		//[x,y]是选中要移动的棋子的位置 
		//[i,j]是目标的位置

		BeginBatchDraw();
		FlushBatchDraw();

		//红方移动
		if (Board[x][y] <= 16 && Board[x][y] != 0)  //选中的是红方并且存活的棋子
		{
			if (Board[i][j] == 0)        //目标位置没有其他棋子
			{
				for (int m = 0; m < RedList.size(); m++)    //遍历剩余的棋子
				{
					if (RedList[m].type == Board[x][y])     //检查当前的棋子的类型是否与起始位置上的棋子类型相匹配
					{
						if (Rule(x, y, i, j))               //如果符合这类棋子的操作规则那就移动
						{
							WasRule = 1;           //移动规则生效标记
							RedList[m].posX = i;
							RedList[m].posY = j;
							isRed = !isRed;

							break;
						}
						else
							WasRule = 0;         //移动规则不生效
					}
				}
			}
			else if (Board[i][j] > 16)         //目标位置是黑方的棋子
			{
				for (int m = 0; m < RedList.size(); m++)
				{
					if (RedList[m].type == Board[x][y])
					{
						if (Rule(x, y, i, j))
						{
							WasRule = 1;

							RedList[m].posX = i;
							RedList[m].posY = j;
							for (int n = 0; n < BlackList.size(); n++)
							{
								if (BlackList[n].type == Board[i][j])      //检查之前剩下的黑方棋子中是哪个被吃了
								{
									BlackList.erase(BlackList.begin() + n);    //消除这个被吃的棋子
									isRed = !isRed;             //红方移动结束，轮到黑方移动


									//播放吃棋子音效
									//关闭上次播放
									mciSendString("stop eat", NULL, 0, NULL);
									mciSendString("close eat", NULL, 0, NULL);
									//播放本次
									mciSendString("open sounds/eat.mp3 alias eat", NULL, 0, NULL);
									mciSendString("play eat ", NULL, 0, NULL);



									break;
								}
							}
							break;
						}
						else
							WasRule = 0;      //移动规则不生效
					}
				}
			}
		}

		//黑方移动
		else if (Board[x][y] > 16 && Board[x][y] != 0)    //选中的是黑方并且存活的棋子
		{
			//同理
			if (Board[i][j] == 0)
			{
				for (int m = 0; m < BlackList.size(); m++)
				{
					if (BlackList[m].type == Board[x][y])
					{
						if (Rule(x, y, i, j))
						{
							WasRule = 1;
							BlackList[m].posX = i;
							BlackList[m].posY = j;
							isRed = !isRed;


							break;
						}
						else
							WasRule = 0;
					}
				}
			}
			else if (Board[i][j] <= 16)
			{
				for (int m = 0; m < BlackList.size(); m++)
				{
					if (BlackList[m].type == Board[x][y])
					{
						if (Rule(x, y, i, j))
						{
							WasRule = 1;
							BlackList[m].posX = i;
							BlackList[m].posY = j;
							for (int n = 0; n < RedList.size(); n++)
							{
								if (RedList[n].type == Board[i][j])
								{
									RedList.erase(RedList.begin() + n);
									isRed = !isRed;


									//播放吃棋子音效
									//关闭上次播放
									mciSendString("stop eat", NULL, 0, NULL);
									mciSendString("close eat", NULL, 0, NULL);
									//播放本次
									mciSendString("open sounds/eat.mp3 alias eat", NULL, 0, NULL);
									mciSendString("play eat ", NULL, 0, NULL);


									break;
								}
							}
							break;
						}
						else
							WasRule = 0;
					}
				}
			}
		}

		isSelected = 0;     //移动结束后选中的状态变为0

		//更新棋盘
		BoardUpdate();

		//清屏
		cleardevice();
		//重画
		drawMap();
		drawPiece();
		//只有成功移动才检查是否将军
		if (WasRule==1 && isSelected==0 )
		{
			jiang();
		}
		EndBatchDraw();
	}


	//每个棋子的移动规则
	bool Rule(int x, int y, int i, int j)
	{
		bool ret = false;    //返回值
		switch (Board[x][y])
		{

			//将与帅
		case 1://帅的走棋规则
		{
			if (i <= 2)
			{
				if (j >= 3 && j <= 5) //限制田字格内移动
				{

					if ((abs(j - y) == 1 && x == i) || (abs(i - x) == 1 && j == y))//限定一次一格并且只能直走
					{
						ret = true;
					}
				}
			}
			if (BlackList[0].posY == y)   //双方将见面，可以吃掉对方的将
			{
				//检查两个将之间有没有阻挡的棋子
				bool isBlock = false;
				for (int m = x + 1; m < BlackList[0].posX; m++)
				{
					if (Board[m][y] != 0)
					{
						isBlock = true;
					}
				}
				//无遮挡就可以吃
				if (!isBlock)
				{
					if (i == BlackList[0].posX && j == BlackList[0].posY)
					{
						ret = true;
					}
				}
			}
			break;
		}
		case 17://将的规则，同上
		{
			if (i >= 7)
			{
				if (j >= 3 && j <= 5)
				{
					if ((abs(j - y) == 1 && x == i) || (abs(i - x) == 1 && j == y))
					{
						ret = true;
					}
				}
			}
			if (RedList[0].posY == y)
			{
				bool isBlock = false;
				for (int m = x + 1; m < RedList[0].posX; m++)
				{
					if (Board[m][y] != 0)
					{
						isBlock = true;
					}

				}
				if (!isBlock)
				{
					if (i == RedList[0].posX && j == RedList[0].posY)
					{
						ret = true;

					}
				}
			}
			break;
		}


		//红方
		//红仕
		case 2://红仕1的规则
		{
			if (i <= 2)
			{
				if (j >= 3 && j <= 5)     //限定田字格
				{
					if (abs(j - y) == 1 && abs(i - x) == 1)  //只能斜着走
						ret = true;
				}
			}
			break;
		}
		case 6://红仕2的规则，同上
		{
			if (i <= 2)
			{
				if (j >= 3 && j <= 5)
				{
					if (abs(j - y) == 1 && abs(i - x) == 1)
						ret = true;
				}
			}
			break;
		}

		//红相
		case 3://红相1的规则
		{
			if (i <= 4)
			{
				if (abs(j - y) == 2 && abs(i - x) == 2)     //只能走田字
				{
					//不能被别相脚
					if (j > y && i > x && Board[i - 1][j - 1] == 0)
						ret = true;
					if (j < y && i > x && Board[i - 1][j + 1] == 0)
						ret = true;
					if (j > y && i < x && Board[i + 1][j - 1] == 0)
						ret = true;
					if (j < y && i < x && Board[i + 1][j + 1] == 0)
						ret = true;
				}
			}
			break;
		}
		case 7://红相2的规则，同上
		{
			if (i <= 4)
			{
				if (abs(j - y) == 2 && abs(i - x) == 2)
				{
					if (j > y && i > x && Board[i - 1][j - 1] == 0)
					{
						ret = true;
					}
					if (j < y && i > x && Board[i - 1][j + 1] == 0)
					{
						ret = true;
					}
					if (j > y && i < x && Board[i + 1][j - 1] == 0)
					{
						ret = true;
					}
					if (j < y && i < x && Board[i + 1][j + 1] == 0)
					{
						ret = true;
					}
				}
			}
			break;
		}

		//红马
		case 4://红马1的规则
		{
			if (abs(j - y) == 2 && abs(i - x) == 1)//水平日字
			{
				if (j > y && Board[x][j - 1] == 0)//不能别马脚
				{
					ret = true;
				}
				if (j < y && Board[x][j + 1] == 0)
				{
					ret = true;
				}
			}
			else if (abs(j - y) == 1 && abs(i - x) == 2)//竖直日字
			{
				if (i > x && Board[i - 1][y] == 0)
				{
					ret = true;
				}
				if (i < x && Board[i + 1][y] == 0)
				{
					ret = true;
				}
			}
			break;
		}
		case 8://红马2的规则，同上
		{
			if (abs(j - y) == 2 && abs(i - x) == 1)
			{
				if (j > y && Board[x][j - 1] == 0)
				{
					ret = true;
				}
				if (j < y && Board[x][j + 1] == 0)
				{
					ret = true;
				}
			}
			else if (abs(j - y) == 1 && abs(i - x) == 2)
			{
				if (i > x && Board[i - 1][y] == 0)
				{
					ret = true;
				}
				if (i < x && Board[i + 1][y] == 0)
				{
					ret = true;
				}
			}
			break;
		}

		//红车
		case 5://红车1的规则
		{
			if (j == y) //竖直方向
			{
				bool isBlock = false;
				if (i > x)//前进(向下)
				{
					for (int m = x + 1; m < i; m++)
					{
						if (Board[m][y] != 0)
						{
							isBlock = true;
						}
					}
				}
				else if (i < x)//后退(向上)
				{
					for (int m = x - 1; m > i; m--)
					{
						if (Board[m][y] != 0)
						{
							isBlock = true;
						}
					}
				}
				if (!isBlock)
					ret = true;
			}
			else if (i == x)//水平方向
			{
				bool isBlock = false;
				if (j > y)//向右
				{
					for (int m = y + 1; m < j; m++)
					{
						if (Board[x][m] != 0)
						{
							isBlock = true;
						}
					}
				}
				else if (j < y)//向左
				{
					for (int m = y - 1; m > j; m--)
					{
						if (Board[x][m] != 0)
						{
							isBlock = true;
						}
					}
				}
				if (!isBlock)
				{
					ret = true;
				}
			}
			break;
		}
		case 9://红车2的规则，同上
		{
			if (j == y)
			{
				bool isBlock = false;
				if (i > x)
				{
					for (int m = x + 1; m < i; m++)
					{
						if (Board[m][y] != 0)
						{
							isBlock = true;
						}
					}
				}
				else if (i < x)
				{
					for (int m = x - 1; m > i; m--)
					{
						if (Board[m][y] != 0)
						{
							isBlock = true;
						}
					}
				}
				if (!isBlock)
				{
					ret = true;
				}
			}
			else if (i == x)
			{
				bool isBlock = false;
				if (j > y)
				{
					for (int m = y + 1; m < j; m++)
					{
						if (Board[x][m] != 0)
						{
							isBlock = true;
						}
					}
				}
				else if (j < y)
				{
					for (int m = y - 1; m > j; m--)
					{
						if (Board[x][m] != 0)
						{
							isBlock = true;
						}
					}
				}
				if (!isBlock)
				{
					ret = true;
				}
			}
			break;
		}

		//红炮
		case 10://红炮1的规则
		{
			if (j == y)          //基本与车的规则相同，分移动和吃子两种情况
			{
				int cnt = 0;
				if (i > x)
				{
					for (int m = x + 1; m < i; m++)
					{
						if (Board[m][y] != 0)
						{
							cnt++;
						}
					}
				}
				else if (i < x)
				{
					for (int m = x - 1; m > i; m--)
					{
						if (Board[m][y] != 0)
						{
							cnt++;
						}
					}
				}

				if (Board[i][j] == 0 && cnt == 0)//仅移动时，中间不可以有阻挡
				{
					ret = true;
				}
				else if (Board[i][j] != 0 && cnt == 1)//吃棋时，中间必须有一个棋子阻挡
				{
					ret = true;
				}
			}
			else if (i == x)//水平移动
			{
				int cnt = 0;
				if (j > y)
				{
					for (int m = y + 1; m < j; m++)
					{
						if (Board[x][m] != 0)
						{
							cnt++;
						}
					}
				}
				else if (j < y)
				{
					for (int m = y - 1; m > j; m--)
					{
						if (Board[x][m] != 0)
						{
							cnt++;
						}
					}
				}
				if (Board[i][j] == 0 && cnt == 0)
				{
					ret = true;
				}
				else if (Board[i][j] != 0 && cnt == 1)
				{
					ret = true;
				}
			}
			break;
		}
		case 11://红炮2的规则，同上
		{
			if (j == y)
			{
				int cnt = 0;
				if (i > x)
				{
					for (int m = x + 1; m < i; m++)
					{
						if (Board[m][y] != 0)
						{
							cnt++;
						}
					}
				}
				else if (i < x)
				{
					for (int m = x - 1; m > i; m--)
						if (Board[m][y] != 0)
							cnt++;
				}
				if (Board[i][j] == 0 && cnt == 0)
				{
					ret = true;
				}
				else if (Board[i][j] != 0 && cnt == 1)
				{
					ret = true;
				}
			}
			else if (i == x)
			{
				int cnt = 0;
				if (j > y)
				{
					for (int m = y + 1; m < j; m++)
					{
						if (Board[x][m] != 0)
						{
							cnt++;
						}
					}
				}
				else if (j < y)
				{
					for (int m = y - 1; m > j; m--)
					{
						if (Board[x][m] != 0)
						{
							cnt++;
						}
					}
				}
				if (Board[i][j] == 0 && cnt == 0)
				{
					ret = true;
				}
				else if (Board[i][j] != 0 && cnt == 1)
				{
					ret = true;
				}
			}
			break;
		}

		//红兵
		case 12://红兵1的规则
		{
			if (x <= 4)
			{
				if (j == y && i - x == 1)
				{
					ret = true;
				}
			}
			else
			{
				if ((abs(j - y) == 1 && x == i) || (i - x == 1 && j == y))//一次一格 并且不能后退
				{
					ret = true;
				}
			}
			break;
		}
		case 13://红兵2的规则，同上
		{
			if (x <= 4)
			{
				if (j == y && i - x == 1)
				{
					ret = true;
				}
			}
			else
			{
				if ((abs(j - y) == 1 && x == i) || (i - x == 1 && j == y))
				{
					ret = true;
				}
			}
			break;
		}
		case 14://红兵3的规则，同上
		{
			if (x <= 4)
			{
				if (j == y && i - x == 1)
				{
					ret = true;
				}
			}
			else
			{
				if ((abs(j - y) == 1 && x == i) || (i - x == 1 && j == y))
				{
					ret = true;
				}
			}
			break;
		}
		case 15://红兵4的规则，同上
		{
			if (x <= 4)
			{
				if (j == y && i - x == 1)
				{
					ret = true;
				}
			}
			else
			{
				if ((abs(j - y) == 1 && x == i) || (i - x == 1 && j == y))
				{
					ret = true;
				}
			}
			break;
		}
		case 16://红兵5的规则，同上
		{
			if (x <= 4)
			{
				if (j == y && i - x == 1)
				{
					ret = true;
				}
			}
			else
			{
				if ((abs(j - y) == 1 && x == i) || (i - x == 1 && j == y))
				{
					ret = true;
				}
			}
			break;
		}


		//黑方
		//黑士
		case 18://黑士1的规则
		{
			if (i >= 7)
			{
				if (j >= 3 && j <= 5)//限定田字格
				{
					if (abs(j - y) == 1 && abs(i - x) == 1)   //只能斜着走
					{
						ret = true;
					}
				}
			}
			break;
		}
		case 22: //黑士2的规则，同上
		{
			if (i >= 7)
			{
				if (j >= 3 && j <= 5)
				{
					if (abs(j - y) == 1 && abs(i - x) == 1)
					{
						ret = true;
					}
				}
			}
			break;
		}

		//黑象
		case 19: //黑象1的规则
		{
			if (i >= 5)
			{
				if (abs(j - y) == 2 && abs(i - x) == 2)  //走田字
				{
					//不能被别相脚
					if (j > y && i > x && Board[i - 1][j - 1] == 0)
					{
						ret = true;
					}
					if (j < y && i > x && Board[i - 1][j + 1] == 0)
					{
						ret = true;
					}
					if (j > y && i < x && Board[i + 1][j - 1] == 0)
					{
						ret = true;
					}
					if (j < y && i < x && Board[i + 1][j + 1] == 0)
					{
						ret = true;
					}
				}
			}
			break;
		}
		case 23://黑象2的规则，同上
		{
			if (i >= 5)
			{
				if (abs(j - y) == 2 && abs(i - x) == 2)
				{
					if (j > y && i > x && Board[i - 1][j - 1] == 0)
					{
						ret = true;
					}
					if (j < y && i > x && Board[i - 1][j + 1] == 0)
					{
						ret = true;
					}
					if (j > y && i < x && Board[i + 1][j - 1] == 0)
					{
						ret = true;
					}
					if (j < y && i < x && Board[i + 1][j + 1] == 0)
					{
						ret = true;
					}
				}
			}
			break;
		}

		//黑马
		case 20:    //黑马1的规则
		{
			if (abs(j - y) == 2 && abs(i - x) == 1) //水平日字
			{
				//不能被别马脚
				if (j > y && Board[x][j - 1] == 0)
				{
					ret = true;
				}
				if (j < y && Board[x][j + 1] == 0)
				{
					ret = true;
				}
			}
			else if (abs(j - y) == 1 && abs(i - x) == 2) //竖直日字
			{
				if (i > x && Board[i - 1][y] == 0)
				{
					ret = true;
				}
				if (i < x && Board[i + 1][y] == 0)
				{
					ret = true;
				}
			}
			break;

		case 24: //黑马2的规则，同上
			if (abs(j - y) == 2 && abs(i - x) == 1)
			{
				if (j > y && Board[x][j - 1] == 0)
				{
					ret = true;
				}
				if (j < y && Board[x][j + 1] == 0)
				{
					ret = true;
				}
			}
			else if (abs(j - y) == 1 && abs(i - x) == 2)
			{
				if (i > x && Board[i - 1][y] == 0)
				{
					ret = true;
				}
				if (i < x && Board[i + 1][y] == 0)
				{
					ret = true;
				}
			}
			break;
		}

		//黑车
		case 21://黑车1的规则
		{
			if (j == y)//竖直方向
			{
				bool isBlock = false;
				if (i > x)//后退(向上)
				{
					for (int m = x + 1; m < i; m++)
					{
						if (Board[m][y] != 0)
						{
							isBlock = true;
						}
					}
				}
				else if (i < x)//前进(向下)
				{
					for (int m = x - 1; m > i; m--)
					{
						if (Board[m][y] != 0)
						{
							isBlock = true;
						}
					}
				}
				if (!isBlock)
				{
					ret = true;
				}
			}
			else if (i == x)//水平方向
			{
				bool isBlock = false;
				if (j > y)//向右
				{
					for (int m = y + 1; m < j; m++)
					{
						if (Board[x][m] != 0)
						{
							isBlock = true;
						}
					}
				}
				else if (j < y)//向左
				{
					for (int m = y - 1; m > j; m--)
					{
						if (Board[x][m] != 0)
						{
							isBlock = true;
						}
					}
				}
				if (!isBlock)
				{
					ret = true;
				}
			}
			break;
		}
		case 25://黑车2的规则，同上
		{
			if (j == y)
			{
				bool isBlock = false;

				if (i > x)
				{
					for (int m = x + 1; m < i; m++)
					{
						if (Board[m][y] != 0)
						{
							isBlock = true;
						}
					}
				}
				else if (i < x)
				{
					for (int m = x - 1; m > i; m--)
						if (Board[m][y] != 0)
							isBlock = true;
				}
				if (!isBlock)
				{
					ret = true;
				}
			}
			else if (i == x)
			{
				bool isBlock = false;

				if (j > y)
				{
					for (int m = y + 1; m < j; m++)
					{
						if (Board[x][m] != 0)
						{
							isBlock = true;
						}
					}
				}
				else if (j < y)
				{
					for (int m = y - 1; m > j; m--)
					{
						if (Board[x][m] != 0)
						{
							isBlock = true;
						}
					}
				}
				if (!isBlock)
				{
					ret = true;
				}
			}
			break;
		}

		//黑炮
		case 26://黑炮1的规则
		{
			if (j == y)
			{
				int cnt = 0;

				if (i > x)
				{
					for (int m = x + 1; m < i; m++)
					{
						if (Board[m][y] != 0)
						{
							cnt++;
						}
					}
				}
				else if (i < x)
				{
					for (int m = x - 1; m > i; m--)
					{
						if (Board[m][y] != 0)
						{
							cnt++;
						}
					}
				}
				if (Board[i][j] == 0 && cnt == 0)  //移动时，中间不可以有阻挡
				{
					ret = true;
				}
				else if (Board[i][j] != 0 && cnt == 1)  //吃子时，中间必须有一个棋子
				{
					ret = true;
				}
			}
			else if (i == x)//水平移动
			{
				int cnt = 0;
				if (j > y)
				{
					for (int m = y + 1; m < j; m++)
					{
						if (Board[x][m] != 0)
						{
							cnt++;
						}
					}
				}
				else if (j < y)
				{
					for (int m = y - 1; m > j; m--)
					{
						if (Board[x][m] != 0)
						{
							cnt++;
						}
					}
				}
				if (Board[i][j] == 0 && cnt == 0)
				{
					ret = true;
				}
				else if (Board[i][j] != 0 && cnt == 1)
				{
					ret = true;
				}
			}
			break;
		}
		case 27://黑炮2的规则，同上
		{
			if (j == y)
			{
				int cnt = 0;
				if (i > x)
				{
					for (int m = x + 1; m < i; m++)
					{
						if (Board[m][y] != 0)
						{
							cnt++;
						}
					}
				}
				else if (i < x)
				{
					for (int m = x - 1; m > i; m--)
					{
						if (Board[m][y] != 0)
						{
							cnt++;
						}
					}
				}
				if (Board[i][j] == 0 && cnt == 0)
				{
					ret = true;
				}
				else if (Board[i][j] != 0 && cnt == 1)
				{
					ret = true;
				}
			}
			else if (i == x)
			{
				int cnt = 0;
				if (j > y)
				{
					for (int m = y + 1; m < j; m++)
					{
						if (Board[x][m] != 0)
						{
							cnt++;
						}
					}
				}
				else if (j < y)
				{
					for (int m = y - 1; m > j; m--)
					{
						if (Board[x][m] != 0)
						{
							cnt++;
						}
					}
				}
				if (Board[i][j] == 0 && cnt == 0)
				{
					ret = true;
				}
				else if (Board[i][j] != 0 && cnt == 1)
				{
					ret = true;
				}
			}
			break;
		}
		//黑卒
		case 28://黑卒1的规则
		{
			if (x >= 5)
			{
				if (j == y && x - i == 1)
				{
					ret = true;
				}
			}
			else
			{
				if ((abs(j - y) == 1 && x == i) || (x - i == 1 && j == y))//一次一格, 并且不能后退
				{
					ret = true;
				}
			}
			break;
		}
		case 29://黑卒2的规则，同上
		{
			if (x >= 5)
			{
				if (j == y && x - i == 1)
				{
					ret = true;
				}
			}
			else
			{
				if ((abs(j - y) == 1 && x == i) || (x - i == 1 && j == y))
				{
					ret = true;
				}
			}
			break;
		}
		case 30://黑卒3的规则，同上
		{
			if (x >= 5)
			{
				if (j == y && x - i == 1)
				{
					ret = true;
				}
			}
			else
			{
				if ((abs(j - y) == 1 && x == i) || (x - i == 1 && j == y))
				{
					ret = true;
				}
			}
			break;
		}
		case 31://黑卒4的规则，同上
		{
			if (x >= 5)
			{
				if (j == y && x - i == 1)
				{
					ret = true;
				}
			}
			else
			{
				if ((abs(j - y) == 1 && x == i) || (x - i == 1 && j == y))
				{
					ret = true;
				}
			}
			break;
		}
		case 32://黑卒5的规则，同上
		{
			if (x >= 5)
			{
				if (j == y && x - i == 1)
				{
					ret = true;
				}
			}
			else
			{
				if ((abs(j - y) == 1 && x == i) || (x - i == 1 && j == y))
				{
					ret = true;
				}
			}
			break;
		}
		default:ret = false;
			break;
		}

		return ret;
	}

	//游戏结束重开
	void restart()
	{
		//全部清除
		RedList.clear();
		BlackList.clear();
		isFirstTime = false;
		//再初始化
		ChessInit();
		BoardUpdate();
		initGraph();
		isRed = true;
	}

};

//开始页面
class UI
{
public:
	UI()
	{
		initgraph(400, 400);
		loadimage(&background, "images/background.png", 711, 400, 1);    //从图片文件获取图像
		putimage(0, 0, &background);

		//绘制按钮
		drawButton(100, 200, 300, 150);
		drawButton(100, 230, 300, 280);
		drawButton(100, 310, 300, 360);

		//设置字符
		LOGFONT f;
		gettextstyle(&f);						// 获取当前字体设置
		f.lfHeight = 20;						// 设置字体高度为 20
		_tcscpy_s(f.lfFaceName, _T("黑体"));	// 设置字体为“黑体”
		f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿
		setbkmode(TRANSPARENT);
		settextstyle(&f);
		settextcolor(BUTTONTEXT);

		//输出按钮上的字符
		START();
		QUIT();
		BieDian();
	}

	//鼠标左键点击不同的按钮
	bool Process(ExMessage& operation)
	{
		if (operation.message == WM_LBUTTONDOWN)
		{
			//别点按钮
			if (operation.x >= 100 && operation.x <= 300 && operation.y <= 200 && operation.y >= 150)
			{
				//播放音频
				mciSendString("stop niganma", NULL, 0, NULL);
				mciSendString("close niganma", NULL, 0, NULL);
				mciSendString("open sounds/ganma.mp3 alias niganma", NULL, 0, NULL);
				mciSendString("play niganma ", NULL, 0, NULL);

				click = true;

				//改变按钮颜色和字符
				setfillcolor(RGB(148, 144, 236));
				solidroundrect(100, 200, 300, 150, 5, 5);
				outtextxy(167, 165, _T("你干嘛"));

			}
			//退出按钮
			else if (operation.x >= 100 && operation.x <= 300 && operation.y <= 360 && operation.y >= 310)
			{
				return true;

			}
			//游戏按钮
			else if (operation.x >= 100 && operation.x <= 300 && operation.y <= 280 && operation.y >= 230)
			{
				//播放音频
				mciSendString("stop click", NULL, 0, NULL);
				mciSendString("close click", NULL, 0, NULL);
				mciSendString("open sounds/click.mp3 alias click", NULL, 0, NULL);
				mciSendString("play click ", NULL, 0, NULL);
				//创建棋盘类
				ChessBoard A;
				A.initGraph();
				while (1)
				{
					peekmessage(&operation, -1, 1);//监测鼠标消息
					if (A.game(operation))//退出游戏部分逻辑
						break;
				}
				return true;
			}
		}
		else
			UIUpdate(operation);
		return false;
	}
private:
	IMAGE background;//定义一个图片名
	//原按钮信息
	void drawButton(int i, int j, int x, int y)
	{
		setfillcolor(BUTTONCOLOR);
		solidroundrect(i, j, x, y, 5, 5);
	}
	//鼠标经过时按钮信息
	void overButton(int i, int j, int x, int y)
	{
		setfillcolor(BUTTONCOLOR + (30, 30, 30));
		solidroundrect(i, j, x, y, 5, 5);
	}


	//鼠标经过按钮触发的操作
	bool isBieDianButtonHovered = false;
	bool isStartButtonHovered = false;
	bool isQuitButtonHovered = false;
	bool click = false;

	void UIUpdate(ExMessage& operation)
	{
		BeginBatchDraw();
		FlushBatchDraw();
		//鼠标经过按钮的时候变色并发声
		if (operation.x >= 100 && operation.x <= 300 && operation.y <= 200 && operation.y >= 150)
		{
			if (!click)
			{
				//"别点"按钮
				overButton(100, 150, 300, 200);
				settextcolor(OVERBUTTON);
				BieDian();
				if (!isBieDianButtonHovered)
				{
					mciSendString("open sounds/ji.mp3 alias myMusic", NULL, 0, NULL);
					mciSendString("play myMusic ", NULL, 0, NULL);
					isBieDianButtonHovered = true;
				}
			}
		}
		else if (operation.x >= 100 && operation.x <= 300 && operation.y <= 280 && operation.y >= 230)
		{
			//开始按钮

			overButton(100, 280, 300, 230);
			settextcolor(OVERBUTTON);
			START();

			if (!isStartButtonHovered)
			{
				mciSendString("open sounds/ji.mp3 alias myMusic", NULL, 0, NULL);
				mciSendString("play myMusic ", NULL, 0, NULL);
				isStartButtonHovered = true;

			}

		}

		else if (operation.x >= 100 && operation.x <= 300 && operation.y <= 360 && operation.y >= 310)
		{
			//退出按钮
			overButton(100, 310, 300, 360);
			settextcolor(OVERBUTTON);
			QUIT();

			if (!isQuitButtonHovered)
			{
				mciSendString("open sounds/ji.mp3 alias myMusic", NULL, 0, NULL);
				mciSendString("play myMusic ", NULL, 0, NULL);
				isQuitButtonHovered = true;

			}
		}
		//没有经过的话就画回去
		else
		{

			settextcolor(BUTTONTEXT);
			if (!click)
			{
				drawButton(100, 200, 300, 150);
				BieDian();
			}
			drawButton(100, 230, 300, 280);
			START();

			drawButton(100, 310, 300, 360);
			QUIT();

			isBieDianButtonHovered = false;
			isStartButtonHovered = false;
			isQuitButtonHovered = false;

			mciSendString("stop myMusic", NULL, 0, NULL);
			mciSendString("close myMusic", NULL, 0, NULL);
		}
		EndBatchDraw();

	}

	void BieDian()
	{
		outtextxy(160, 165, _T("不要点我"));
	}
	void START()
	{
		outtextxy(160, 245, _T("开始游戏"));
	}
	void QUIT()
	{
		outtextxy(160, 325, _T("退出游戏"));
	}

};


int main()
{

	UI Start;
	ExMessage operation;

	while (1)
	{
		peekmessage(&operation, -1, 1);
		if (Start.Process(operation))
		{
			break;
		}

	}

}
