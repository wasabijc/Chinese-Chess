/*
��ĿԴ��ַ��https://github.com/wasabijc/Chinese-Chess.git
ת����ע������
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


#define BUTTONTEXT RGB(30,30,30)      //��ʼ����������ɫ
#define OVERBUTTON RGB(60,60,60)      //��ʼ������꾭����ť������
#define BUTTONCOLOR RGB(200,130,100)  //��ʼ���水ť��ɫ
#define BGColor RGB(200,130,100)       //��Ϸ����������ɫ
//��Ϸ���洰�ڹ��
#define Height 500
#define Width 452
#define Margin 30      //����ͼƬ���̵ı߾�
#define GZ 49          //���̸��ӿ��
#define ChessSize 20   //���Ӱ뾶


class UI;
class Chess;
class ChessBoard;

class Chess//������
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

	int type;//��������
	int posX, posY;//����λ��
};

class ChessBoard//������
{
public:
	friend UI;

	ChessBoard()
	{
		height = 10;
		width = 9;
		//�Ȱ���������������Ϊ0
		Board.assign(height, vector<int>(width, 0));

		//����������Ϣ
		BoardInit();
		ChessInit();
		//��������
		BoardUpdate();
	}

	//����������б������Board
	void BoardUpdate()
	{
		//�����ȶ������б�����
		Board.assign(height, vector<int>(width, 0));
		//����ʣ������
		for (vector<Chess>::iterator i = RedList.begin(); i != RedList.end(); ++i)
		{
			Board[i->posX][i->posY] = i->type;
		}
		for (vector<Chess>::iterator i = BlackList.begin(); i != BlackList.end(); ++i)
		{
			Board[i->posX][i->posY] = i->type;
		}

	}

	//��Ϸ��ʼ��ʼ������
	void initGraph()
	{
		initgraph(Width, Height);
		//�����̺�����
		drawMap();
		drawPiece();

		mciSendString("stop init2", NULL, 0, NULL);
		mciSendString("close init2", NULL, 0, NULL);
		// ����Ƶ�ļ�
		mciSendString("open sounds/chuji.mp3 alias init2", NULL, 0, NULL);
		if (isFirstTime)
		{
			//��һ�δ���Ϸҳ�棬���ڿ�ʼҳ�����İ�ť���������Ƶ��������Ϸҳ��Ŀ��������ӳ����벥��
			//��Ϸ�������ؿ������������
			this_thread::sleep_for(chrono::seconds(3));
		}
		// ������Ƶ�ļ�
		mciSendString("play init2 ", NULL, 0, NULL);

	}
	//��Ϸ���к���
	bool game(ExMessage& operation)
	{
		//���������½��еĲ���
		if (operation.message == WM_LBUTTONDOWN)
		{
			if (!isSelected)
			{
				//���û�����ӱ�ѡ�񣬽���ѡ��ģʽ
				MessageProcessFirst(operation);
			}
			else
			{
				// ���������ӱ�ѡ�񣬽����ƶ�ģʽ
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
	int height, width;//���̴�С���̶�Ϊ10*9
	vector<vector<int> > Board;//���̣��洢������ÿ�����״̬

	map<int, string> reflex;//���ӵ������뺺�����ƶ�Ӧ��
	vector<Chess> RedList, BlackList;//����췽�ͺڷ���ʣ������

	int SelectX = -1, SelectY = -1;//ѡ�񵽵����ӵ�����
	bool isSelected;        //�Ƿ������ӱ�ѡ��
	bool isFirstTime = true;  //�Ƿ��ǵ�һ����Ϸ
	bool isRed = true;      //�Ƿ��Ǻ췽�غ�
	bool WasRule = false;   //���Ĳ����Ƿ�����ƶ�����

	//�����������ƶ�Ӧ������������
	void BoardInit()
	{
		reflex[0] = "   ";
		reflex[1] = "˧";
		reflex[2] = "��";
		reflex[3] = "��";
		reflex[4] = "��";
		reflex[5] = "��";
		reflex[6] = "��";
		reflex[7] = "��";
		reflex[8] = "��";
		reflex[9] = "��";
		reflex[10] = "��";
		reflex[11] = "��";
		reflex[12] = "��";
		reflex[13] = "��";
		reflex[14] = "��";
		reflex[15] = "��";
		reflex[16] = "��";

		reflex[17] = "��";
		reflex[18] = "ʿ";
		reflex[19] = "��";
		reflex[20] = "�R";
		reflex[21] = "܇";
		reflex[22] = "ʿ";
		reflex[23] = "��";
		reflex[24] = "�R";
		reflex[25] = "܇";
		reflex[26] = "�h";
		reflex[27] = "�h";
		reflex[28] = "��";
		reflex[29] = "��";
		reflex[30] = "��";
		reflex[31] = "��";
		reflex[32] = "��";
	}
	void ChessInit()//���ӳ�ʼ��
	{
		//(���,��,��)
		
		//�췽 ���<=16
		//��˧
		RedList.emplace_back(1, 0, 4);
		//��ʿ
		RedList.emplace_back(2, 0, 3);
		RedList.emplace_back(6, 0, 5);
		//����
		RedList.emplace_back(3, 0, 2);
		RedList.emplace_back(7, 0, 6);
		//����
		RedList.emplace_back(4, 0, 1);
		RedList.emplace_back(8, 0, 7);
		//�쳵
		RedList.emplace_back(5, 0, 0);
		RedList.emplace_back(9, 0, 8);
		//����
		RedList.emplace_back(10, 2, 1);
		RedList.emplace_back(11, 2, 7);
		//���
		RedList.emplace_back(12, 3, 0);
		RedList.emplace_back(13, 3, 2);
		RedList.emplace_back(14, 3, 4);
		RedList.emplace_back(15, 3, 6);
		RedList.emplace_back(16, 3, 8);


		//�ڷ� ���>16
		//�ڽ�
		BlackList.emplace_back(17, 9, 4);
		//��ʿ
		BlackList.emplace_back(18, 9, 3);
		BlackList.emplace_back(22, 9, 5);
		//����
		BlackList.emplace_back(19, 9, 2);
		BlackList.emplace_back(23, 9, 6);
		//����
		BlackList.emplace_back(20, 9, 1);
		BlackList.emplace_back(24, 9, 7);
		//�ڳ�
		BlackList.emplace_back(21, 9, 0);
		BlackList.emplace_back(25, 9, 8);
		//����
		BlackList.emplace_back(26, 7, 1);
		BlackList.emplace_back(27, 7, 7);
		//�ڱ�
		BlackList.emplace_back(28, 6, 0);
		BlackList.emplace_back(29, 6, 2);
		BlackList.emplace_back(30, 6, 4);
		BlackList.emplace_back(31, 6, 6);
		BlackList.emplace_back(32, 6, 8);
	}

	//ʤ�����㻭��
	bool WinCheck()
	{
		int ret = 0;
		if (RedList[0].type != 1 || RedList.size() == 0)
		{
			ret = 1;   //��ʤ
		}
		else if (BlackList[0].type != 17 || BlackList.size() == 0)
		{
			ret = 2;   //��ʤ
		}
		if (ret != 0)
		{
			int checkHeight = 300;
			int checkWidth = 400;
			initgraph(checkWidth, checkHeight);

			// ������Ƶ
			mciSendString("open sounds/victory.mp3 alias victory", NULL, 0, NULL);
			mciSendString("play victory ", NULL, 0, NULL);


			IMAGE background;//����һ��ͼƬ��.
			loadimage(&background, "images/wincheck.png", checkWidth, checkHeight, 1);//��ͼƬ�ļ���ȡͼ��
			putimage(0, 0, &background);

			settextcolor(RGB(240, 230, 30));//��׽�ɫ�Ľ������
			LOGFONT f;
			gettextstyle(&f);						// ��ȡ��ǰ��������
			f.lfHeight = 40;						// ��������߶�Ϊ 40
			_tcscpy_s(f.lfFaceName, _T("����"));	// ��������Ϊ�����塱
			f.lfQuality = ANTIALIASED_QUALITY;		// ���Ч��Ϊ�����
			// ������б��
			f.lfEscapement = -190;           //ÿ������Χ��������ת�Ƕ�
			f.lfOrientation = -190;          //��һ������б
			setbkmode(TRANSPARENT);                 //͸������
			settextstyle(&f);

			if (ret == 1)
			{
				outtextxy(145, 92, _T("�ڷ�ʤ��"));
			}
			else if (ret == 2)
			{
				outtextxy(145, 92, _T("�췽ʤ��"));
			}

			f.lfHeight = 30;						// ��������߶�Ϊ 30
			// �ָ���б��Ϊ0
			f.lfEscapement = 0;
			f.lfOrientation = 0;
			settextstyle(&f);
			settextcolor(RGB(148, 144, 236));
			outtextxy(checkWidth / 2 - 135, checkHeight / 2 + 60, _T("���»س������¿�ʼ"));
			outtextxy(checkWidth / 2 - 135, checkHeight / 2 + 100, _T("����ESC���˳�"));
			//��ȡ���̲���
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



	//��������
	void drawMap()
	{
		IMAGE background;//����һ��ͼƬ��.
		loadimage(&background, "images/chessboard.png", 452, 500, 1);//��ͼƬ�ļ���ȡͼ��
		putimage(0, 0, &background);  //����ͼ��
	}

	//��������
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
					//��������
					_stprintf(s, _T("%s"), reflex[Board[i][j]].c_str());

					setfillcolor(BGColor);        //���ӵ���ɫ
					fillcircle(Margin + GZ * j, Margin + GZ * i, ChessSize);

					LOGFONT f;
					gettextstyle(&f);						// ��ȡ��ǰ��������
					f.lfHeight = 20;						// ��������߶�Ϊ 20
					_tcscpy_s(f.lfFaceName, _T("����"));		// ��������Ϊ�����塱
					f.lfQuality = ANTIALIASED_QUALITY;		// �������Ч��Ϊ�����
					setbkmode(TRANSPARENT);
					settextstyle(&f);
					outtextxy(Margin + GZ * j - 10, Margin + GZ * i - 10, s);
				}
			}
		}
	}


	//����ѡ��ģʽ������������ĺ���
	void MessageProcessFirst(ExMessage operation)
	{
		int posx, posy;
		int mx, my;
		mx = (operation.x - Margin - GZ / 2);
		my = (operation.y - Margin - GZ / 2);
		//�����������ת��Ϊ����������ϵ����ꡣ
		if (mx > 0)
			//����������ڸ��ӵ���߾�
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

		//��������Ƿ�������������ͬ����ͬ��ȡ��ѡ��
		if (SelectX == (my / GZ + 1) && SelectY == (mx / GZ + 1))
		{
			isSelected = 0;
			SelectPiece(posx, posy, isSelected);
			return;
		}
		if (Board[my / GZ + 1][mx / GZ + 1] != 0 && Board[my / GZ + 1][mx / GZ + 1] <= 16 && isRed)
		{
			//����ں췽�غϵ���췽�������ѡ��
			//ѡ������ɫ������ʾ
			isSelected = 1;
			SelectPiece(posx, posy,isSelected);
			SelectX = my / GZ + 1;
			SelectY = mx / GZ + 1;
		}
		else if (Board[my / GZ + 1][mx / GZ + 1] > 16 && !isRed)
		{
			//����ںڷ��غϵ���ڷ��������ѡ��
			isSelected = 1;
			SelectPiece(posx, posy,isSelected);
			SelectX = my / GZ + 1;
			SelectY = mx / GZ + 1;
		}

	}

	//�����ƶ�ģʽ������������ĺ���
	void MessageProcessSecond(ExMessage operation)
	{
		int posx, posy;
		int mx, my;
		//��ͬ�Ĳ������������������ת������������̵�����
		mx = (operation.x - Margin - GZ / 2);
		my = (operation.y - Margin - GZ / 2);
		if (mx > 0)
			posx = Margin + GZ * ((mx / GZ) + 1);    //����������ڸ��ӵ���߾�
		else
		{
			posx = Margin;         //������̵��ⲿ����߾�������̵ı߾�
			mx = -GZ;
		}
		if (my > 0)
			posy = Margin + GZ * ((my / GZ) + 1);    //����������ڸ��ӵ��ϱ߾�
		else
		{
			posy = Margin;
			my = -GZ;
		}
		//����Ե�����ת������Ե��к���
		int i = my / GZ + 1;
		int j = mx / GZ + 1;
		if (SelectX == i && SelectY == j)
		{
			isSelected = 1;
			return;
		}

		//��֮ǰѡ�е������ƶ�������ѡ�еĵط�
		movePiece(SelectX, SelectY, i, j, operation);
		SelectX = i;
		SelectY = j;
	}


	//����ɫ������ʾѡ�е�����
	void SelectPiece(int posx, int posy,int isSelected)
	{
		BeginBatchDraw();
		FlushBatchDraw();

		//�������̺�����
		drawMap();
		drawPiece();

		//���������ʽ
		if (isSelected)
		{
			setlinecolor(BLUE);
			rectangle(posx - 20, posy - 20, posx + 20, posy + 20);
		}


		EndBatchDraw();
	}

	//����ʱ������Ч
	void jiang() {

		for (int m = 1; m < RedList.size(); m++)    //�����췽ʣ�������
		{
			//��ÿ�������λ����Ϊ��ǰλ��
			int x = RedList[m].posX;
			int y = RedList[m].posY;
			//�Ѻڽ���λ����ΪĿ��λ��
			int i = BlackList[0].posX;
			int j = BlackList[0].posY;

			//�ѳ���˧���������������ڽ������괫������жϺڽ��Ƿ���ܵ���в���ܵ���в�ᷢ������
			if (Rule(x, y, i, j))
			{
				//�ر��ϴβ���
				mciSendString("stop jun", NULL, 0, NULL);
				mciSendString("close jun", NULL, 0, NULL);
				//���ű���
				mciSendString("open sounds/jiangjun.mp3 alias jun", NULL, 0, NULL);
				mciSendString("play jun ", NULL, 0, NULL);
			}

		}

		for (int m = 1; m < BlackList.size(); m++)    //�����ڷ�ʣ�������
		{
			//��ÿ�������λ����Ϊ��ǰλ��
			int x = BlackList[m].posX;
			int y = BlackList[m].posY;
			//�Ѻ�˧��λ�����ó�Ŀ��λ��
			int i = RedList[0].posX;
			int j = RedList[0].posY;

			//�ѳ��˽�����������������˧�����괫���ƶ������жϺ�˧�Ƿ���ܵ���в���ܵ���в�ᷢ������
			if (Rule(x, y, i, j))
			{
				//�ر��ϴβ���
				mciSendString("stop jun", NULL, 0, NULL);
				mciSendString("close jun", NULL, 0, NULL);
				//���ű���
				mciSendString("open sounds/jiangjun.mp3 alias jun", NULL, 0, NULL);
				mciSendString("play jun ", NULL, 0, NULL);
			}

		}

	}


	//�ƶ����Ӻ���
	void movePiece(int x, int y, int i, int j, ExMessage& operation)
	{
		//[x,y]��ѡ��Ҫ�ƶ������ӵ�λ�� 
		//[i,j]��Ŀ���λ��

		BeginBatchDraw();
		FlushBatchDraw();

		//�췽�ƶ�
		if (Board[x][y] <= 16 && Board[x][y] != 0)  //ѡ�е��Ǻ췽���Ҵ�������
		{
			if (Board[i][j] == 0)        //Ŀ��λ��û����������
			{
				for (int m = 0; m < RedList.size(); m++)    //����ʣ�������
				{
					if (RedList[m].type == Board[x][y])     //��鵱ǰ�����ӵ������Ƿ�����ʼλ���ϵ�����������ƥ��
					{
						if (Rule(x, y, i, j))               //��������������ӵĲ��������Ǿ��ƶ�
						{
							WasRule = 1;           //�ƶ�������Ч���
							RedList[m].posX = i;
							RedList[m].posY = j;
							isRed = !isRed;

							break;
						}
						else
							WasRule = 0;         //�ƶ�������Ч
					}
				}
			}
			else if (Board[i][j] > 16)         //Ŀ��λ���Ǻڷ�������
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
								if (BlackList[n].type == Board[i][j])      //���֮ǰʣ�µĺڷ����������ĸ�������
								{
									BlackList.erase(BlackList.begin() + n);    //����������Ե�����
									isRed = !isRed;             //�췽�ƶ��������ֵ��ڷ��ƶ�


									//���ų�������Ч
									//�ر��ϴβ���
									mciSendString("stop eat", NULL, 0, NULL);
									mciSendString("close eat", NULL, 0, NULL);
									//���ű���
									mciSendString("open sounds/eat.mp3 alias eat", NULL, 0, NULL);
									mciSendString("play eat ", NULL, 0, NULL);



									break;
								}
							}
							break;
						}
						else
							WasRule = 0;      //�ƶ�������Ч
					}
				}
			}
		}

		//�ڷ��ƶ�
		else if (Board[x][y] > 16 && Board[x][y] != 0)    //ѡ�е��Ǻڷ����Ҵ�������
		{
			//ͬ��
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


									//���ų�������Ч
									//�ر��ϴβ���
									mciSendString("stop eat", NULL, 0, NULL);
									mciSendString("close eat", NULL, 0, NULL);
									//���ű���
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

		isSelected = 0;     //�ƶ�������ѡ�е�״̬��Ϊ0

		//��������
		BoardUpdate();

		//����
		cleardevice();
		//�ػ�
		drawMap();
		drawPiece();
		//ֻ�гɹ��ƶ��ż���Ƿ񽫾�
		if (WasRule==1 && isSelected==0 )
		{
			jiang();
		}
		EndBatchDraw();
	}


	//ÿ�����ӵ��ƶ�����
	bool Rule(int x, int y, int i, int j)
	{
		bool ret = false;    //����ֵ
		switch (Board[x][y])
		{

			//����˧
		case 1://˧���������
		{
			if (i <= 2)
			{
				if (j >= 3 && j <= 5) //�������ָ����ƶ�
				{

					if ((abs(j - y) == 1 && x == i) || (abs(i - x) == 1 && j == y))//�޶�һ��һ����ֻ��ֱ��
					{
						ret = true;
					}
				}
			}
			if (BlackList[0].posY == y)   //˫�������棬���ԳԵ��Է��Ľ�
			{
				//���������֮����û���赲������
				bool isBlock = false;
				for (int m = x + 1; m < BlackList[0].posX; m++)
				{
					if (Board[m][y] != 0)
					{
						isBlock = true;
					}
				}
				//���ڵ��Ϳ��Գ�
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
		case 17://���Ĺ���ͬ��
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


		//�췽
		//����
		case 2://����1�Ĺ���
		{
			if (i <= 2)
			{
				if (j >= 3 && j <= 5)     //�޶����ָ�
				{
					if (abs(j - y) == 1 && abs(i - x) == 1)  //ֻ��б����
						ret = true;
				}
			}
			break;
		}
		case 6://����2�Ĺ���ͬ��
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

		//����
		case 3://����1�Ĺ���
		{
			if (i <= 4)
			{
				if (abs(j - y) == 2 && abs(i - x) == 2)     //ֻ��������
				{
					//���ܱ������
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
		case 7://����2�Ĺ���ͬ��
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

		//����
		case 4://����1�Ĺ���
		{
			if (abs(j - y) == 2 && abs(i - x) == 1)//ˮƽ����
			{
				if (j > y && Board[x][j - 1] == 0)//���ܱ����
				{
					ret = true;
				}
				if (j < y && Board[x][j + 1] == 0)
				{
					ret = true;
				}
			}
			else if (abs(j - y) == 1 && abs(i - x) == 2)//��ֱ����
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
		case 8://����2�Ĺ���ͬ��
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

		//�쳵
		case 5://�쳵1�Ĺ���
		{
			if (j == y) //��ֱ����
			{
				bool isBlock = false;
				if (i > x)//ǰ��(����)
				{
					for (int m = x + 1; m < i; m++)
					{
						if (Board[m][y] != 0)
						{
							isBlock = true;
						}
					}
				}
				else if (i < x)//����(����)
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
			else if (i == x)//ˮƽ����
			{
				bool isBlock = false;
				if (j > y)//����
				{
					for (int m = y + 1; m < j; m++)
					{
						if (Board[x][m] != 0)
						{
							isBlock = true;
						}
					}
				}
				else if (j < y)//����
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
		case 9://�쳵2�Ĺ���ͬ��
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

		//����
		case 10://����1�Ĺ���
		{
			if (j == y)          //�����복�Ĺ�����ͬ�����ƶ��ͳ����������
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

				if (Board[i][j] == 0 && cnt == 0)//���ƶ�ʱ���м䲻�������赲
				{
					ret = true;
				}
				else if (Board[i][j] != 0 && cnt == 1)//����ʱ���м������һ�������赲
				{
					ret = true;
				}
			}
			else if (i == x)//ˮƽ�ƶ�
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
		case 11://����2�Ĺ���ͬ��
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

		//���
		case 12://���1�Ĺ���
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
				if ((abs(j - y) == 1 && x == i) || (i - x == 1 && j == y))//һ��һ�� ���Ҳ��ܺ���
				{
					ret = true;
				}
			}
			break;
		}
		case 13://���2�Ĺ���ͬ��
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
		case 14://���3�Ĺ���ͬ��
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
		case 15://���4�Ĺ���ͬ��
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
		case 16://���5�Ĺ���ͬ��
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


		//�ڷ�
		//��ʿ
		case 18://��ʿ1�Ĺ���
		{
			if (i >= 7)
			{
				if (j >= 3 && j <= 5)//�޶����ָ�
				{
					if (abs(j - y) == 1 && abs(i - x) == 1)   //ֻ��б����
					{
						ret = true;
					}
				}
			}
			break;
		}
		case 22: //��ʿ2�Ĺ���ͬ��
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

		//����
		case 19: //����1�Ĺ���
		{
			if (i >= 5)
			{
				if (abs(j - y) == 2 && abs(i - x) == 2)  //������
				{
					//���ܱ������
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
		case 23://����2�Ĺ���ͬ��
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

		//����
		case 20:    //����1�Ĺ���
		{
			if (abs(j - y) == 2 && abs(i - x) == 1) //ˮƽ����
			{
				//���ܱ������
				if (j > y && Board[x][j - 1] == 0)
				{
					ret = true;
				}
				if (j < y && Board[x][j + 1] == 0)
				{
					ret = true;
				}
			}
			else if (abs(j - y) == 1 && abs(i - x) == 2) //��ֱ����
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

		case 24: //����2�Ĺ���ͬ��
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

		//�ڳ�
		case 21://�ڳ�1�Ĺ���
		{
			if (j == y)//��ֱ����
			{
				bool isBlock = false;
				if (i > x)//����(����)
				{
					for (int m = x + 1; m < i; m++)
					{
						if (Board[m][y] != 0)
						{
							isBlock = true;
						}
					}
				}
				else if (i < x)//ǰ��(����)
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
			else if (i == x)//ˮƽ����
			{
				bool isBlock = false;
				if (j > y)//����
				{
					for (int m = y + 1; m < j; m++)
					{
						if (Board[x][m] != 0)
						{
							isBlock = true;
						}
					}
				}
				else if (j < y)//����
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
		case 25://�ڳ�2�Ĺ���ͬ��
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

		//����
		case 26://����1�Ĺ���
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
				if (Board[i][j] == 0 && cnt == 0)  //�ƶ�ʱ���м䲻�������赲
				{
					ret = true;
				}
				else if (Board[i][j] != 0 && cnt == 1)  //����ʱ���м������һ������
				{
					ret = true;
				}
			}
			else if (i == x)//ˮƽ�ƶ�
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
		case 27://����2�Ĺ���ͬ��
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
		//����
		case 28://����1�Ĺ���
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
				if ((abs(j - y) == 1 && x == i) || (x - i == 1 && j == y))//һ��һ��, ���Ҳ��ܺ���
				{
					ret = true;
				}
			}
			break;
		}
		case 29://����2�Ĺ���ͬ��
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
		case 30://����3�Ĺ���ͬ��
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
		case 31://����4�Ĺ���ͬ��
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
		case 32://����5�Ĺ���ͬ��
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

	//��Ϸ�����ؿ�
	void restart()
	{
		//ȫ�����
		RedList.clear();
		BlackList.clear();
		isFirstTime = false;
		//�ٳ�ʼ��
		ChessInit();
		BoardUpdate();
		initGraph();
		isRed = true;
	}

};

//��ʼҳ��
class UI
{
public:
	UI()
	{
		initgraph(400, 400);
		loadimage(&background, "images/background.png", 711, 400, 1);    //��ͼƬ�ļ���ȡͼ��
		putimage(0, 0, &background);

		//���ư�ť
		drawButton(100, 200, 300, 150);
		drawButton(100, 230, 300, 280);
		drawButton(100, 310, 300, 360);

		//�����ַ�
		LOGFONT f;
		gettextstyle(&f);						// ��ȡ��ǰ��������
		f.lfHeight = 20;						// ��������߶�Ϊ 20
		_tcscpy_s(f.lfFaceName, _T("����"));	// ��������Ϊ�����塱
		f.lfQuality = ANTIALIASED_QUALITY;		// �������Ч��Ϊ�����
		setbkmode(TRANSPARENT);
		settextstyle(&f);
		settextcolor(BUTTONTEXT);

		//�����ť�ϵ��ַ�
		START();
		QUIT();
		BieDian();
	}

	//�����������ͬ�İ�ť
	bool Process(ExMessage& operation)
	{
		if (operation.message == WM_LBUTTONDOWN)
		{
			//��㰴ť
			if (operation.x >= 100 && operation.x <= 300 && operation.y <= 200 && operation.y >= 150)
			{
				//������Ƶ
				mciSendString("stop niganma", NULL, 0, NULL);
				mciSendString("close niganma", NULL, 0, NULL);
				mciSendString("open sounds/ganma.mp3 alias niganma", NULL, 0, NULL);
				mciSendString("play niganma ", NULL, 0, NULL);

				click = true;

				//�ı䰴ť��ɫ���ַ�
				setfillcolor(RGB(148, 144, 236));
				solidroundrect(100, 200, 300, 150, 5, 5);
				outtextxy(167, 165, _T("�����"));

			}
			//�˳���ť
			else if (operation.x >= 100 && operation.x <= 300 && operation.y <= 360 && operation.y >= 310)
			{
				return true;

			}
			//��Ϸ��ť
			else if (operation.x >= 100 && operation.x <= 300 && operation.y <= 280 && operation.y >= 230)
			{
				//������Ƶ
				mciSendString("stop click", NULL, 0, NULL);
				mciSendString("close click", NULL, 0, NULL);
				mciSendString("open sounds/click.mp3 alias click", NULL, 0, NULL);
				mciSendString("play click ", NULL, 0, NULL);
				//����������
				ChessBoard A;
				A.initGraph();
				while (1)
				{
					peekmessage(&operation, -1, 1);//��������Ϣ
					if (A.game(operation))//�˳���Ϸ�����߼�
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
	IMAGE background;//����һ��ͼƬ��
	//ԭ��ť��Ϣ
	void drawButton(int i, int j, int x, int y)
	{
		setfillcolor(BUTTONCOLOR);
		solidroundrect(i, j, x, y, 5, 5);
	}
	//��꾭��ʱ��ť��Ϣ
	void overButton(int i, int j, int x, int y)
	{
		setfillcolor(BUTTONCOLOR + (30, 30, 30));
		solidroundrect(i, j, x, y, 5, 5);
	}


	//��꾭����ť�����Ĳ���
	bool isBieDianButtonHovered = false;
	bool isStartButtonHovered = false;
	bool isQuitButtonHovered = false;
	bool click = false;

	void UIUpdate(ExMessage& operation)
	{
		BeginBatchDraw();
		FlushBatchDraw();
		//��꾭����ť��ʱ���ɫ������
		if (operation.x >= 100 && operation.x <= 300 && operation.y <= 200 && operation.y >= 150)
		{
			if (!click)
			{
				//"���"��ť
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
			//��ʼ��ť

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
			//�˳���ť
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
		//û�о����Ļ��ͻ���ȥ
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
		outtextxy(160, 165, _T("��Ҫ����"));
	}
	void START()
	{
		outtextxy(160, 245, _T("��ʼ��Ϸ"));
	}
	void QUIT()
	{
		outtextxy(160, 325, _T("�˳���Ϸ"));
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
