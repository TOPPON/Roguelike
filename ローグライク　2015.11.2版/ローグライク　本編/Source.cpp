#include"DxLib.h"
#include"math.h"
#include"stdio.h"
#define ENEMYMAX 50
#define TURNTIME 15//一マス動く分のコマ数
#define ROOMX 30
#define ROOMY 30
#define DIFFICULT 3//1-5くらいまで
void Init();
void GameInit();
void ReInit();
void messagedraw();
void messagesend();
void message2draw();
void draw();
void Enemymove();
void Itemuse();
struct MASS
{
	int type;//床、0:壁　1:床　2:部屋　3:階段//廃止　4:壁　5:部屋壁
	bool thing;
	bool apeear;//踏んで見えたか
	int item;//アイテムの種類、0はなし
	int itemk;//数
	bool itemf;//アイテムが見つかったか
	int trap;//0:なし1:落とし穴 2:ワープ 3:ダメージ 4: 5: 6: 7: 8: 9: 10:
	bool trapf;
};
MASS mass[ROOMX][ROOMY];
struct ENEMYTYPE
{
	int hp;//基本部分
	int hpr;//ランダム部分
	int atk;//基本部分
	int atkr;//ランダム部分
	int type;//攻撃のタイプ,1:物理 2:弓
	int speciala;//どんな特殊を持っているか
	int specialap;//確率、%
	char name[20];
	char describe[100];
	int ghandle;
};
ENEMYTYPE enemytype[30];
struct ROOM
{
	int x;
	int y;
	int width;
	int length;
};
ROOM room[20];
struct ROAD 
{
	int sx,sy;
	int ex,ey;
	int k1x,k1y;
	int k2x,k2y;
	int sr,er;//部屋
};
ROAD road[20];
struct ENEMY
{
	int x;
	int y;
	int wayx;//向き
	int wayy;//向き
	double angle;
	int hp;
	int hpm;
	int atk;
	int type;//種類,0はなし
	bool notice;//気づいてるかどうか
	bool attacked;//そのターンに攻撃されたかどうか
	bool strike;
};
ENEMY enemy[ENEMYMAX];
struct ITEM
{
	bool kab;//数えられるか
	int type;//特性の種類,0は消費アイテムではない
	int pal;//パラメータ
	int rare;//レア度
	int imp;//アイテム、武器、防具、アクセサリ、弓、大切なもので0,1,2,3,4,5
	int spe;//アクセサリのみ、どこに効くか 1:攻撃2:自分のＨＰ3:防御4:◯5:×6:回避率7:命中率8:tseeab9:eseeab10:t攻撃を耐える率11:視野
	int spe1,spal1;//装備した時の特殊効果、1:自分の攻撃2:自分のＨＰ3:自分の防御4:自分の○5:自分の×6:回避率7:命中率7:tseeab
	int spe2,spal2;//装備した時の特殊効果、1:自分の攻撃2:自分のＨＰ3:自分の防御4:自分の○5:自分の×6:回避率7:命中率7:tseeab
	char name[30];//名前
	char describe1[50];
	char describe2[50];
};
ITEM item[50];
struct MESSAGE
{
	int type;//0:なし　1:相手にダメージ　2:自分にダメージ　3:アイテム拾った　4:アイテム使った　5:アイテム置いた　6:レベルアップ　7:状態変化　8:動けない　9:死んだ　10:セーブした　11:敵を倒した　12:経験値を得た　13:装備した　14:投げた　15:罠
	int val;//値
	int val2;
	int target;//誰に、誰から
};
MESSAGE message[20];
struct DAMAGE
{
	double x;
	double y;
	double dx;
	double dy;
	int life;
	int ylimit;
	int amount;
};
DAMAGE damage[10];
struct STATAS
{
	char name[20];//毒　火傷　攻撃　防御　tseeab　
	bool become;//状態異常になっているかどうか
	int lturn;//残りターン数
};
STATAS statas[18];
struct ME
{
	int floa;
	int x,y;
	int atk,hp,hpm;
	bool attacked,strike;
	int level;//レベル
	int expr;//経験値
	int wayx,wayy;
	double angle;
	int pwayx,pwayy;
	int inroom;//どこの部屋にいるか,-1はいない
	int hitem[30];//0はなし
	int hitemk[30];
	int mhitem;//合計所持数
	int menu;//１つ目のメニュー画面,0はなし 1:アイテム 2:セーブ 3:
	int menu1;//２つ目のメニュー画面,0はなし 1〜30:アイテム番号　か　1〜3:セーブデータの番号
	int menu2;//３つ目のメニュー画面,0はなし 1:使う 2:捨てる 3:やめる　か　1:はい 2:いいえ
	int screen;//画面番号
	int messagetype;//0:ひとつ　1:全部　2:３つ 
	int move[2000];//2:動ける 1:動けない
	int turns;
	int tseeab;
	int movemax;//変動する値
	int mmax;
	int mlimit;
	int elimit;
	int ilimit;
	int equip1,equip2,equip3,equip4;//アイテム名、１は武器、２は防具、３はアクセサリ、４は矢
	int equipk1,equipk2,equipk3,equipk4;//持ち物の中の番号、１は武器、２は防具、３はアクセサリ、４は矢
	int score;//スコア
	bool strdir;//殴られた時にそちらを向くかどうか
	int siya;//視野、4*視野
	int def;//防御力
	int kaihi;//8-1*kaihi
	int metyu;//8+0.5*metyu
	int tkoutai;//t攻撃の耐性、1で60、２で100
	int bgm;//BGM
	int se;//効果音
	int wt;//移動時間
	int bt;//ボタンの感知時間
	double scrolx,scroly;
	bool eseeab;
	int color;//自分の体力の色
	int kx,ky;//階段の位置
	int ka;//回復の小数点以下
};//自分のデータ
ME me;
int massn[ROOMX][ROOMY];
int massm[ROOMX][ROOMY];
bool pu,pd,pr,pl;
bool ps,pz,px,pls;//space,z,x,lshift
bool ea;//敵が何かしたかどうか
bool ma;//自分が何かしたかどうか
bool mm;//自分が？
int t;
int loading;//ロード中にかかる時間
bool dm;//降りるところかどうか
int selecting;
int turntime;
int ghandle[30];
int time;//総括の時間
int mitem=17;//作った最大のアイテム数
bool error;
int mtype;//一時的メッセージ、0:なし　1:アイテム説明　2:階段
int color[]={GetColor(50,0,255),GetColor(100,255,100),GetColor(100,200,100),GetColor(150,200,255),GetColor(50,100,255),0,0,0,GetColor(150,150,150)};
void sta(int number,int type);
//strikeはなぐる
//attackedはなぐられる
int WinMain(HINSTANCE hI,HINSTANCE hP,LPSTR lpC,int nC)
{
	ChangeWindowMode(true);
	if(DxLib_Init()==-1) return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	Init();
	while(ProcessMessage()==0)
	{
		ClsDrawScreen();
		time++;
		if (time>31416)time-=31416;
		switch(me.screen)
		{
		case 0://タイトル
			if (CheckHitKey(KEY_INPUT_Z)==1)
			{
				if (pz==false)
				{
					me.screen=1;
					pz=true;
					SetFontSize(16);
					if (selecting==0)
					{
						GameInit();
						ReInit();
					}
					else
					{
						FILE *fp;
						switch(selecting)
						{
						case 1:
							fp=fopen("save\\save1.dat","rb");
							break;
						case 2:
							fp=fopen("save\\save2.dat","rb");
							break;
						case 3:
							fp=fopen("save\\save3.dat","rb");
							break;
						}
						if (fp!=NULL)
						{
							fread(&me,sizeof(me),1,fp);
							fread(&mass,sizeof(MASS),ROOMX*ROOMY,fp);
							fread(&room,sizeof(ROOM),20,fp);
							fread(&message,sizeof(MESSAGE),8,fp);
							fread(&statas,sizeof(STATAS),18,fp);
							fread(&enemy,sizeof(ENEMY),18,fp);
							fclose(fp);
						}
						else me.screen=0;
					}
				}
			}
			else pz=false;
			if (CheckHitKey(KEY_INPUT_UP)==1)
			{
				if (pu==false)
				{
					pu=true;
					selecting--;
					if (selecting<0)selecting+=4;
				}
			}
			else pu=false;
			if (CheckHitKey(KEY_INPUT_DOWN)==1)
			{
				if (pd==false)
				{
					pd=true;
					selecting++;
					if (selecting>3)selecting-=4;
				}
			}
			else pd=false;
			DrawGraph(0,0,ghandle[6],1);
			SetFontSize(40);
			switch(selecting)
			{
			case 0:
				DrawString(320-GetDrawStringWidth("最初から",8)/2,200,"最初から",GetColor(255,255,255));
				break;
			case 1:
				DrawString(320-GetDrawStringWidth("セーブデータ１",14)/2,250,"セーブデータ１",GetColor(255,255,255));
				break;
			case 2:
				DrawString(320-GetDrawStringWidth("セーブデータ２",14)/2,300,"セーブデータ２",GetColor(255,255,255));
				break;
			case 3:
				DrawString(320-GetDrawStringWidth("セーブデータ３",14)/2,350,"セーブデータ３",GetColor(255,255,255));
				break;
			}
			for (int i=0;i<4;i++)	
			{
				if (i==selecting)i++;
				switch(i)
				{
				case 0:
					DrawString(320-GetDrawStringWidth("最初から",8)/2,200,"最初から",GetColor(155,155,155));
					break;
				case 1:
					DrawString(320-GetDrawStringWidth("セーブデータ１",14)/2,250,"セーブデータ１",GetColor(155,155,155));
					break;
				case 2:
					DrawString(320-GetDrawStringWidth("セーブデータ２",14)/2,300,"セーブデータ２",GetColor(155,155,155));
					break;
				case 3:
					DrawString(320-GetDrawStringWidth("セーブデータ３",14)/2,350,"セーブデータ３",GetColor(155,155,155));
					break;
				}
			}
			break;
		case 1://ゲーム画面
			if (((turntime==0)&&(me.move[me.turns]==2))||((turntime>=ea*20+me.wt-me.bt+ma*20)&&(me.move[me.turns+1]==2))||(dm==true)||(me.menu!=0))//動ける時
			{
				/*if ((GetMouseInput()& MOUSE_INPUT_LEFT)==1)
				{
					ReInit();
				}*/
				/*for (int i=0;i<20;i++)
				{
					if (road[i].k1x>0)
					{
						int c=0;
					}
				}*/
				if (CheckHitKey(KEY_INPUT_UP)==1)
				{
					if (pu==false){
						pu=true;
						if (dm==true){
							selecting=1-selecting;}
						else if (me.menu!=0){
							if (me.menu1!=0){
								if (me.menu2!=0){
									me.menu2--;
									if (me.menu2<1){
										switch(me.menu)
										{
										case 1:
											me.menu2+=3;
											break;
										case 2:
											me.menu2+=2;
											break;
										case 3:
											me.menu2+=4;
											break;
										}
									}
								}
								else{
									me.menu1--;
									if (me.menu1<1){
										switch(me.menu)
										{
										case 1:
											me.menu1+=me.mhitem;
											break;
										case 2:
											me.menu1+=3;
											break;
										case 3:
											me.menu1+=5;
											break;
										}
									}
								}
							}
							else{
								me.menu--;
								if (me.menu<1)me.menu+=3;}}
						else {
							if ((pd==false)&&(pl==false)&&(pr==false)){
								me.wayx=0;
								me.wayy=0;}
							me.wayy=-1;
							if (me.wayx==1)me.angle=3.141592/4*1;
							else if (me.wayx==0)me.angle=3.141592/4*0;
							else me.angle=3.141592/4*7;}}
				}
				else pu=false;
				if (CheckHitKey(KEY_INPUT_DOWN)==1)
				{
					if (pd==false){
						pd=true;
						if (dm==true){
							selecting=1-selecting;}
						else if (me.menu!=0)
						{if (me.menu1!=0)
							{if (me.menu2!=0)
								{me.menu2++;
									switch (me.menu)
									{
									case 1:
										if (me.menu2>3)me.menu2-=3;
										break;
									case 2:
										if (me.menu2>2)me.menu2-=2;
										break;
									case 3:
										if (me.menu2>4)me.menu2-=4;
										break;
									/*case 2:
										if (me.menu2>3)me.menu2-=3;*/
									}
								}
								else{
									me.menu1++;
									switch(me.menu)
									{
									case 1:
										if (me.menu1>me.mhitem) me.menu1-=me.mhitem;
										break;
									case 2:
										if (me.menu1>3) me.menu1-=3;
										break;
									case 3:
										if (me.menu1>5) me.menu1-=5;
										break;
									}
								}
							}
							else{
								me.menu++;
								if (me.menu>3)me.menu-=3;}}
						else {
							if ((pu==false)&&(pl==false)&&(pr==false)){
								me.wayx=0;
								me.wayy=0;}
							me.wayy=1;
							if (me.wayx==1)me.angle=3.141592/4*3;
							else if (me.wayx==0)me.angle=3.141592/4*4;
							else me.angle=3.141592/4*5;}}
				}
				else pd=false;
				if (CheckHitKey(KEY_INPUT_LEFT)==1)
				{
					if (pl==false)
					{
						pl=true;
						if (dm==true)
						{
							selecting=1-selecting;
						}
						else if (me.menu==0)
						{
							if ((pd==false)&&(pu==false)&&(pr==false))
							{
								me.wayx=0;
								me.wayy=0;
							}
							me.wayx=-1;
							if (me.wayy==1)me.angle=3.141592/4*5;
							else if (me.wayy==0)me.angle=3.141592/4*6;
							else me.angle=3.141592/4*7;
						}
					}
				}
				else pl=false;
				if (CheckHitKey(KEY_INPUT_RIGHT)==1)
				{
					if (pr==false)
					{
						pr=true;
						if (dm==true)
						{
							selecting=1-selecting;
						}
						else if (me.menu==0)
						{
							if ((pd==false)&&(pu==false)&&(pl==false))
							{
								me.wayx=0;
								me.wayy=0;
							}
							me.wayx=1;
							if (me.wayy==1)me.angle=3.141592/4*3;
							else if (me.wayy==0)me.angle=3.141592/4*2;
							else me.angle=3.141592/4*1;
						}
					}
				}
				else pr=false;
				if ((me.menu!=0)||(dm==true))t=0;
				if ((pu==true)||(pd==true)||(pr==true)||(pl==true)) t++;
				else t=0;
			}
			if (((turntime==0)&&(me.move[me.turns]==2))||(dm==true)||(me.menu!=0))//動ける時
			{
				if ((t>me.bt)&&(dm==false)&&(me.menu==0))
				{
					t=0;
					pu=false;
					pd=false;
					pr=false;
					pl=false;
					if ((mass[me.x+me.wayx][me.y+me.wayy].type>=1)&&(mass[me.x+me.wayx][me.y+me.wayy].type<=3)&&(mass[me.x+me.wayx][me.y+me.wayy].thing==false))
					{
						if (me.wayx*me.wayy!=0)
						{
							if ((me.x+me.wayx<ROOMX)&&(me.x+me.wayx>=0)&&(me.y+me.wayy<ROOMY)&&(me.y+me.wayy>=0)) if ((mass[me.x][me.y+me.wayy].type>=1)&&(mass[me.x][me.y+me.wayy].type<=3)&&(mass[me.x][me.y+me.wayy].thing==false)&&
								(mass[me.x+me.wayx][me.y].type>=1)&&(mass[me.x+me.wayx][me.y].type<=3)&&(mass[me.x+me.wayx][me.y].thing==false))
							{
								mass[me.x][me.y].thing=false;
								me.x+=me.wayx;
								me.y+=me.wayy;
								me.pwayx=me.wayx;
								me.pwayy=me.wayy;
								mass[me.x][me.y].thing=true;
								if (mass[me.x][me.y].item!=0)
								{
									if (me.hitem[29]==0)
									{
										for (int i=0;i<30;i++)
										{
											if (me.hitem[i]==0)
											{
												me.hitem[i]=mass[me.x][me.y].item;
												me.hitemk[i]=mass[me.x][me.y].itemk;
												mass[me.x][me.y].item=0;
												mass[me.x][me.y].itemk=0;
												me.mhitem++;
												messagesend();
												message[0].type=3;
												message[0].val=me.hitem[i];
												if (item[me.hitem[i]-1].kab==true)
												{
													message[0].val2=me.hitemk[i];
												}
												break;
											}
											else if (me.hitem[i]==mass[me.x][me.y].item)
											{
												if (item[me.hitem[i]-1].kab==true)
												{
													if (me.hitemk[i]+mass[me.x][me.y].itemk>99)
													{
														messagesend();
														message[0].type=3;
														message[0].val=mass[me.x][me.y].item;
														message[0].val2=99-me.hitemk[i];
														mass[me.x][me.y].itemk-=99-me.hitemk[i];
														me.hitemk[i]=99;
													}
													else 
													{
														messagesend();
														message[0].type=3;
														message[0].val=mass[me.x][me.y].item;
														message[0].val2=mass[me.x][me.y].itemk;
														me.hitemk[i]+=mass[me.x][me.y].itemk;
														mass[me.x][me.y].itemk=0;
														mass[me.x][me.y].item=0;
														break;
													}
												}
											}
										}
									}
								}
								turntime=1;
								/*if (mass[me.x][me.y].type==3)//階段
								{
									dm=true;
									turntime=me.wt;
									mtype=2;
									if (me.hpm>me.hp) me.hp++;
								}
								else
								{
								}*/
							}
						}
						else if ((me.x+me.wayx<ROOMX)&&(me.x+me.wayx>=0)&&(me.y+me.wayy<ROOMY)&&(me.y+me.wayy>=0))
						{
							mass[me.x][me.y].thing=false;
							me.x+=me.wayx;
							me.y+=me.wayy;
							me.pwayx=me.wayx;
							me.pwayy=me.wayy;
							mass[me.x][me.y].thing=true;
							if (mass[me.x][me.y].item!=0)
							{
								if (me.hitem[29]==0)
								{
									for (int i=0;i<30;i++)
									{
										if (me.hitem[i]==0)
										{
											me.hitem[i]=mass[me.x][me.y].item;
											me.hitemk[i]=mass[me.x][me.y].itemk;
											mass[me.x][me.y].item=0;
											mass[me.x][me.y].itemk=0;
											me.mhitem++;
											messagesend();
											message[0].type=3;
											message[0].val=me.hitem[i];
											if (item[me.hitem[i]-1].kab==true)
											{
												message[0].val2=me.hitemk[i];
											}
											break;
										}
										else if (me.hitem[i]==mass[me.x][me.y].item)
										{
											if (item[me.hitem[i]-1].kab==true)
											{
												if (me.hitemk[i]+mass[me.x][me.y].itemk>99)
												{
													messagesend();
													message[0].type=3;
													message[0].val=mass[me.x][me.y].item;
													message[0].val2=99-me.hitemk[i];
													mass[me.x][me.y].itemk-=99-me.hitemk[i];
													me.hitemk[i]=99;
												}
												else 
												{
													messagesend();
													message[0].type=3;
													message[0].val=mass[me.x][me.y].item;
													message[0].val2=mass[me.x][me.y].itemk;
													me.hitemk[i]+=mass[me.x][me.y].itemk;
													mass[me.x][me.y].itemk=0;
													mass[me.x][me.y].item=0;
													break;
												}
											}
										}
									}
								}
							}
							turntime=1;
							/*if (mass[me.x][me.y].type==3)//階段
							{
								dm=true;
								turntime=me.wt;
								mtype=2;
								if (me.hpm>me.hp) me.hp++;
							}
							else
							{
							}*/
						}
						//画面が明るくなるとこ
						if (mass[me.x][me.y].type==2)
						{
							for (int i=0;i<20;i++)
							{
								if ((me.x>=room[i].x)&&(me.x<room[i].x+room[i].width)&&(me.y>=room[i].y)&&(me.y<room[i].y+room[i].length))
								{
									me.inroom=i;
									break;
								}
							}
							for (int i=room[me.inroom].x-1;i<room[me.inroom].width+room[me.inroom].x+1;i++)
							{
								for (int j=room[me.inroom].y-1;j<room[me.inroom].length+room[me.inroom].y+1;j++)
								{
									mass[i][j].apeear=true;
									mass[i][j].itemf=true;
								}
							}
						}
						else
						{
							for (int i=0;i<ROOMX;i++)
							{
								for (int j=0;j<ROOMY;j++)
								{
									if ((me.x-i)*(me.x-i)+(me.y-j)*(me.y-j)<4*me.siya)
									{
										mass[i][j].apeear=true;
										mass[i][j].itemf=true;
									}
								}
							}
						}
						/*if ((me.x>0)&&(me.x<ROOMX-1)) if ((me.y>0)&&(me.y<ROOMY-1)) for (int i=me.x-1;i<me.x+2;i++)	for (int j=me.y-1;j<me.y+2;j++)	mass[i][j].apeear=true,mass[i][j].itemf=true;
							else if (me.y==0) for (int i=me.x-1;i<me.x+2;i++)	for (int j=0;j<me.y+2;j++)	mass[i][j].apeear=true,mass[i][j].itemf=true;
								else for (int i=me.x-1;i<me.x+2;i++)	for (int j=me.y-1;j<ROOMY;j++)	mass[i][j].apeear=true,mass[i][j].itemf=true;
						else if(me.x==0) if ((me.y>0)&&(me.y<ROOMY-1)) for (int i=0;i<me.x+2;i++)	for (int j=me.y-1;j<me.y+2;j++)	mass[i][j].apeear=true,mass[i][j].itemf=true;
								else if (me.y==0) for (int i=0;i<me.x+2;i++)	for (int j=0;j<me.y+2;j++)	mass[i][j].apeear=true,mass[i][j].itemf=true;
									else for (int i=0;i<me.x+2;i++)	for (int j=me.y-1;j<ROOMY;j++)	mass[i][j].apeear=true,mass[i][j].itemf=true;
							else if ((me.y>0)&&(me.y<ROOMY-1)) for (int i=me.x-1;i<ROOMX;i++)	for (int j=me.y-1;j<me.y+2;j++)	mass[i][j].apeear=true,mass[i][j].itemf=true;
								else if (me.y==0) for (int i=me.x-1;i<ROOMX;i++)	for (int j=0;j<me.y+2;j++)	mass[i][j].apeear=true,mass[i][j].itemf=true;
									else for (int i=me.x-1;i<ROOMX;i++)	for (int j=me.y-1;j<ROOMY;j++)	mass[i][j].apeear=true,mass[i][j].itemf=true;*/
						//画面が明るくなるとこ
						
						if (me.x>me.scrolx+10) me.scrolx++;
						if (me.x<me.scrolx+6) me.scrolx--;
						if (me.y>me.scroly+7) me.scroly++;
						if (me.y<me.scroly+3) me.scroly--;
					}
				}
				if (CheckHitKey(KEY_INPUT_SPACE)==1)
				{
					if (ps==false)
					{
						ps=true;
						if ((dm==false)&&(me.menu==0))
						{
							turntime=1;
						}
					}
				}
				else ps=false;
				if (CheckHitKey(KEY_INPUT_X)==1)
				{
					if (px==false)
					{
						px=true;
						if (dm==true)
						{
							dm=false;
							mtype=0;
							turntime=1;
						}
						else
						{
							if (me.menu==0) me.menu=1;
							else if (me.menu1==0) me.menu=0;
							else if (me.menu2==0) 
							{
								me.menu1=0;
								mtype=0;
							}
							else me.menu2=0;
						}
					}
				}
				else px=false;
				if (CheckHitKey(KEY_INPUT_LSHIFT)==1)//矢
				{
					if (pls==false)
					{
						pls=true;
						if (me.equip4!=-1)
						{
							messagesend();
							message[0].type=14;
							message[0].val=me.hitem[me.equipk4];
							message[0].val2=item[me.hitem[me.equipk4]-1].imp;
							for (int i=1;i<6;i++)
							{
								//if ((me.x+me.wayx*i<ROOMX)&&(me.x+me.wayx*i>0)&&(me.y+me.wayy*i>0)&&(me.y+me.wayy*i<ROOMY))
								{
									if (mass[me.x+me.wayx*i][me.y+me.wayy*i].thing==true)
									{
										for (int j=0;j<ENEMYMAX;j++)
										{
											if ((enemy[j].x==me.x+me.wayx*i)&&(enemy[j].y==me.y+me.wayy*i)&&(enemy[j].hp>0)&&(enemy[j].type!=0))
											{
												int d=int((me.atk+item[me.hitem[me.equipk4]-1].pal)*(GetRand(10)*0.015+0.6));
												enemy[j].hp-=d;
												messagesend();
												message[0].type=1;
												message[0].target=enemy[j].type;
												message[0].val=d;enemy[j].attacked=true;
												me.score+=d/3;
												me.strike=true;
												if (enemy[j].hp<=0)
												{
													messagesend();
													message[0].type=11;
													message[0].target=enemy[j].type;
													enemy[j].type=0;
													mass[enemy[j].x][enemy[j].y].thing=false;
													me.expr+=enemy[j].atk/2+enemy[j].hpm/5;
													messagesend();
													message[0].type=12;
													message[0].val=enemy[j].atk/2+enemy[j].hpm/5;
													me.score+=enemy[j].atk/4.0+enemy[j].hpm/10.0;
													if (me.expr>=9*me.level-4)
													{
														messagesend();
														message[0].type=6;
														me.expr-=9*me.level-4;
														me.level++;
														me.atk+=GetRand(1)+1;
														int uhp=GetRand(1)+3+me.hpm/(20+GetRand(10)*10);
														me.hp+=uhp;
														me.hpm+=uhp;
													}
												}
												break;
											}
										}
										break;
									}
									if ((mass[me.x+me.wayx*i][me.y+me.wayy*i].type>=4)||(mass[me.x+me.wayx*i][me.y+me.wayy*i].type==0))
									{
										if (mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)].item==0)
										{
											mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)].item=me.hitem[me.equipk4];
											mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)].itemk=1;
											mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)].itemf=true;
										}
										else if (mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)].item==me.hitem[me.equipk4])
										{
											mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)].itemk+=1;
										}
										else
										{
											if ((item[me.hitem[me.equipk4]-1].kab==true)&&
												(((mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)+1].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)+1].type<=2)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)+1].type>=1))||
												((mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)].type<=2)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)].type>=1))||
												((mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)-1].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)-1].type<=2)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)-1].type>=1))||
												((mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)+1].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)+1].type<=2)&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)+1].type>=1))||
												((mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)-1].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)-1].type<=2)&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)-1].type>=1))||
												((mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)+1].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)+1].type<=2)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)+1].type>=1))||
												((mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)].type<=2)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)].type>=1))||
												((mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)-1].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)-1].type<=2)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)-1].type>=1))))
											{
												while(1)
												{
													int wx=GetRand(2)-1,wy=GetRand(2)-1;
													if ((mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].type<=2)&&(mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].type>=1))
													{
														if (mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].item==0)
														{
															mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].item=me.hitem[me.equipk4];
															mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].itemk=1;
															mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].itemf=true;
															break;
														}
														else if (mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].item==me.hitem[me.equipk4])
														{
															mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].itemk+=1;
															break;
														}
													}
												}
											}
										}
										break;
									}
									if (i==5)
									{
										if (mass[me.x+me.wayx*i][me.y+me.wayy*i].item==0)
										{
											mass[me.x+me.wayx*i][me.y+me.wayy*i].item=me.hitem[me.equipk4];
											mass[me.x+me.wayx*i][me.y+me.wayy*i].itemk=1;
											mass[me.x+me.wayx*i][me.y+me.wayy*i].itemf=true;
										}
										else if (mass[me.x+me.wayx*i][me.y+me.wayy*i].item==me.hitem[me.equipk4])
										{
											mass[me.x+me.wayx*i][me.y+me.wayy*i].itemk+=1;
										}
										else
										{
											if ((item[me.hitem[me.equipk4]-1].kab==true)&&
												(((mass[me.x+me.wayx*i-1][me.y+me.wayy*i+1].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i+1].type<=2)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i+1].type>=1))||
												((mass[me.x+me.wayx*i-1][me.y+me.wayy*i].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i].type<=2)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i].type>=1))||
												((mass[me.x+me.wayx*i-1][me.y+me.wayy*i-1].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i-1].type<=2)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i-1].type>=1))||
												((mass[me.x+me.wayx*i][me.y+me.wayy*i+1].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*i][me.y+me.wayy*i+1].type<=2)&&(mass[me.x+me.wayx*i][me.y+me.wayy*i+1].type>=1))||
												((mass[me.x+me.wayx*i][me.y+me.wayy*i-1].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*i][me.y+me.wayy*i-1].type<=2)&&(mass[me.x+me.wayx*i][me.y+me.wayy*i-1].type>=1))||
												((mass[me.x+me.wayx*i+1][me.y+me.wayy*i+1].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i+1].type<=2)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i+1].type>=1))||
												((mass[me.x+me.wayx*i+1][me.y+me.wayy*i].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i].type<=2)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i].type>=1))||
												((mass[me.x+me.wayx*i+1][me.y+me.wayy*i-1].item==me.hitem[me.equipk4])&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i-1].type<=2)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i-1].type>=1))))
											{
												while(1)
												{
													int wx=GetRand(2)-1,wy=GetRand(2)-1;
													if ((mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].type<=2)&&(mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].type>=1))
													{
														if (mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].item==0)
														{
															mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].item=me.hitem[me.equipk4];
															mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].itemk=1;
															mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].itemf=true;
															break;
														}
														else if (mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].item==me.hitem[me.equipk4])
														{
															mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].itemk+=1;
															break;
														}
													}
												}
											}
										}
									}
								}
							}
							if (me.hitemk[me.equipk4]==1)
							{
								for (int i=me.equipk4;i<29;i++)
								{
									me.hitem[i]=me.hitem[i+1];
									me.hitemk[i]=me.hitemk[i+1];
									if (me.equipk1==i)me.equipk1--;
									if (me.equipk2==i)me.equipk2--;
									if (me.equipk3==i)me.equipk3--;
									if (me.equipk4==i)me.equipk4--;
								}
								me.equipk4=-1;
								me.equip4=-1;
								me.hitem[29]=0;
								me.hitemk[29]=0;
								me.mhitem--;
								turntime=1;
							}
							else
							{
								me.hitemk[me.equipk4]--;
								turntime=1;
							}
						}
					}
				}
				else pls=false;
				if (CheckHitKey(KEY_INPUT_Z)==1)//攻撃
				{
					if (pz==false)
					{
						pz=true;
						if (dm==true)
						{
							pu=false;
							pd=false;
							pr=false;
							pl=false;
							if (selecting==0)
							{
								turntime=me.wt-2;
							}
							else
							{
								me.floa++;
								ReInit();
								me.screen=3;
								me.score+=20*me.floa*me.floa+30*me.floa;
								time=0;
							}
							dm=false;
							mtype=0;
						}
						else if (me.menu!=0)
						{
							if (me.menu1!=0)
							{
								if (me.menu2!=0)
								{
									if (me.menu==1)
									{
										switch(me.menu2)
										{
										case 1://つかう
											messagesend();
											if (item[me.hitem[me.menu1-1]-1].imp!=0)
											{
												message[0].type=13;
												message[0].val=item[me.hitem[me.menu1-1]-1].imp;
												message[0].val2=me.hitem[me.menu1-1];
												if (item[me.hitem[me.menu1-1]-1].imp<5)//装備品
												{
													if ((me.equipk1!=me.menu1-1)&&(me.equipk2!=me.menu1-1)&&(me.equipk3!=me.menu1-1)&&(me.equipk4!=me.menu1-1))
													{
														switch(item[me.hitem[me.menu1-1]-1].imp)
														{
														case 1://武器
															me.equip1=me.hitem[me.menu1-1]-1;
															me.equipk1=me.menu1-1;
															break;
														case 2://防具
															me.equip2=me.hitem[me.menu1-1]-1;
															me.equipk2=me.menu1-1;
															break;
														case 3://アクセサリ
															me.equip3=me.hitem[me.menu1-1]-1;
															me.equipk3=me.menu1-1;
															break;
														case 4://矢
															me.equip4=me.hitem[me.menu1-1]-1;
															me.equipk4=me.menu1-1;
															break;
														}
														switch(item[me.hitem[me.menu1-1]-1].spe1)
														{
														case 0://なし
															break;
														case 1://攻撃
															me.atk+=item[me.hitem[me.menu1-1]-1].spal1;
															break;
														case 2://HP
															me.hpm+=item[me.hitem[me.menu1-1]-1].spal1;
															break;
														case 3://防御
															me.def+=item[me.hitem[me.menu1-1]-1].spal1;
															break;
														case 4://◯
															break;
														case 5://×
															break;
														case 6://回避率
															me.kaihi+=item[me.hitem[me.menu1-1]-1].spal1;
															break;
														case 7://命中率
															me.metyu+=item[me.hitem[me.menu1-1]-1].spal1;
															break;
														}
														message[0].target=1;
														me.menu=0;
														me.menu1=0;
														me.menu2=0;
														mtype=0;
														turntime=1;
													}
													else
													{
														switch(item[me.hitem[me.menu1-1]-1].imp)
														{
														case 1://武器
															me.equip1=-1;
															me.equipk1=-1;
															break;
														case 2://防具
															me.equip2=-1;
															me.equipk2=-1;
															break;
														case 3://アクセサリ
															me.equip3=-1;
															me.equipk3=-1;
															break;
														case 4://矢
															me.equip4=-1;
															me.equipk4=-1;
															break;
														}
														message[0].target=0;
														me.menu=0;
														me.menu1=0;
														me.menu2=0;
														mtype=0;
														turntime=1;
													}
												}
											}
											else 
											{
												message[0].type=4;
												message[0].val=me.hitem[me.menu1-1];
												switch(item[me.hitem[me.menu1-1]-1].type)
												{
												case 1://HP回復,値
													me.hp+=item[me.hitem[me.menu1-1]-1].pal;
													if (me.hpm<me.hp)me.hp=me.hpm;
													break;
												case 2://MHP増加,値
													me.hpm+=item[me.hitem[me.menu1-1]-1].pal;
													me.hp+=item[me.hitem[me.menu1-1]-1].pal;
													break;
												case 3://TP回復,値
													{
														int a=0;//動けない数
														for (int i=me.turns;i<me.mmax;i++)
														{
															if (me.move[i]==1)
															{
																a++;
															}
														}
														if (item[me.hitem[me.menu1-1]-1].pal>=a)
														{
															for (int i=me.turns;i<me.mmax;i++)
															{
																me.move[i]=2;
															}
														}
														else
														{
															for (int i=0;i<item[me.hitem[me.menu1-1]-1].pal;i++)
															{
																while(1)
																{
																	a=GetRand(me.mmax-me.turns)+me.turns;
																	if (me.move[a]==1)
																	{
																		me.move[a]=2;
																		break;
																	}
																}
															}
														}
													}
													break;
												case 4://TP追加,値
													{
														int a=0;
														if (me.mmax+item[me.hitem[me.menu1-1]].pal<2999)
														{
															for (int i=me.mmax;i<me.mmax+item[me.hitem[me.menu1-1]-1].pal;i++)
															{
																me.move[i]=2;
															}
															for (int i=0;i<item[me.hitem[me.menu1-1]-1].pal*0.2;i++)
															{
																while(1)
																{
																	a=GetRand(item[me.hitem[me.menu1-1]-1].pal-1)+me.mmax;
																	if (me.move[a]==1)
																	{
																		me.move[a]=2;
																		break;
																	}
																}
															}
															me.mmax+=item[me.hitem[me.menu1-1]-1].pal;
														}
													}
													break;
												case 8://脱出
													{
														me.floa=1;
														ReInit();
														me.score/=2;
														me.screen=3;
														me.score+=20*me.floa*me.floa+30*me.floa;
														time=0;
													}
													break;
												}
												if (item[me.hitem[me.menu1-1]-1].kab==false)
												{
													me.score+=item[me.hitem[me.menu1-1]-1].rare*10;
													for (int i=me.menu1-1;i<29;i++)
													{
														me.hitem[i]=me.hitem[i+1];
														me.hitemk[i]=me.hitemk[i+1];
														if (me.equipk1==i)me.equipk1--;
														if (me.equipk2==i)me.equipk2--;
														if (me.equipk3==i)me.equipk3--;
														if (me.equipk4==i)me.equipk4--;
													}
													me.hitem[29]=0;
													me.hitemk[29]=0;
													me.mhitem--;
													turntime=1;
													me.menu=0;
													me.menu1=0;
													me.menu2=0;
													mtype=0;
												}
												else
												{
													me.score+=item[me.hitem[me.menu1-1]-1].rare*3;
													if (me.hitemk[me.menu1-1]==1)
													{
														for (int i=me.menu1-1;i<29;i++)
														{
															me.hitem[i]=me.hitem[i+1];
															me.hitemk[i]=me.hitemk[i+1];
															if (me.equipk1==i)me.equipk1--;
															if (me.equipk2==i)me.equipk2--;
															if (me.equipk3==i)me.equipk3--;
															if (me.equipk4==i)me.equipk4--;
														}
														me.hitem[29]=0;
														me.hitemk[29]=0;
														me.mhitem--;
														turntime=1;
														me.menu=0;
														me.menu1=0;
														me.menu2=0;
														mtype=0;
													}
													else
													{
														me.hitemk[me.menu1-1]--;
														me.menu=0;
														me.menu1=0;
														me.menu2=0;
														mtype=0;
													}
												}
											}
											break;
										case 2://投げるor打つ
											messagesend();
											message[0].type=14;
											message[0].val=me.hitem[me.menu1-1];
											message[0].val2=item[me.hitem[me.menu1-1]-1].imp;
											for (int i=1;i<6;i++)
											{
												if (mass[me.x+me.wayx*i][me.y+me.wayy*i].thing==true)
												{
													for (int j=0;j<ENEMYMAX;j++)
													{
														if ((enemy[j].x==me.x+me.wayx*i)&&(enemy[j].y==me.y+me.wayy*i)&&(enemy[j].hp>0)&&(enemy[j].type!=0))
														{
															int d=int(me.atk*(GetRand(10)*0.02+0.3+item[me.hitem[me.menu1-1]-1].rare/10.0*2));
															if (item[me.hitem[me.menu1-1]-1].imp==4)
															{
																d=int((me.atk+item[me.hitem[me.menu1-1]-1].pal)*(GetRand(10)*0.015+0.6));
															}
															enemy[j].hp-=d;
															messagesend();
															message[0].type=1;
															message[0].target=enemy[i].type;
															message[0].val=d;enemy[i].attacked=true;
															me.score+=d/3;
															me.strike=true;
															if (enemy[j].hp<=0)
															{
																messagesend();
																message[0].type=11;
																message[0].target=enemy[j].type;
																enemy[j].type=0;
																mass[enemy[j].x][enemy[j].y].thing=false;
																me.expr+=enemy[j].atk/2+enemy[j].hpm/5;
																messagesend();
																message[0].type=12;
																message[0].val=enemy[j].atk/2+enemy[j].hpm/5;
																me.score+=enemy[j].atk/4.0+enemy[j].hpm/10.0;
																if (me.expr>=9*me.level-4)
																{
																	messagesend();
																	message[0].type=6;
																	me.expr-=9*me.level-4;
																	me.level++;
																	me.atk+=GetRand(1)+1;
																	int uhp=GetRand(1)+3+me.hpm/(20+GetRand(10)*10);
																	me.hp+=uhp;
																	me.hpm+=uhp;
																}
															}
															break;
														}
													}
													break;
												}
												if ((mass[me.x+me.wayx*i][me.y+me.wayy*i].type>=4)||(mass[me.x+me.wayx*i][me.y+me.wayy*i].type==0))
												{
													if (mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)].item==0)
													{
														mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)].item=me.hitem[me.menu1-1];
														mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)].itemk=1;
														mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)].itemf=true;
													}
													else if ((mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)].item==me.hitem[me.menu1-1])&&(item[me.hitem[me.menu1-1]-1].kab==true))
													{
														mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)].itemk+=1;
													}
													else
													{
														if (((mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)+1].item==0)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)+1].type<=2)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)+1].type>=1)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)+1].trap==0))||
															((mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)].item==0)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)].type<=2)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)].type>=1)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)].trap==0))||
															((mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)-1].item==0)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)-1].type<=2)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)-1].type>=1)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)-1].trap==0))||
															((mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)+1].item==0)&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)+1].type<=2)&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)+1].type>=1)&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)+1].trap==0))||
															((mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)-1].item==0)&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)-1].type<=2)&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)-1].type>=1)&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)-1].trap==0))||
															((mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)+1].item==0)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)+1].type<=2)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)+1].type>=1)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)+1].trap==0))||
															((mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)].item==0)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)].type<=2)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)].type>=1)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)].trap==0))||
															((mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)-1].item==0)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)-1].type<=2)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)-1].type>=1)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)-1].trap==0))||
															((item[me.hitem[me.menu1-1]-1].kab==true)&&
															(((mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)+1].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)+1].type<=2)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)+1].type>=1)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)+1].trap==0))||
															((mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)].type<=2)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)].type>=1)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)].trap==0))||
															((mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)-1].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)-1].type<=2)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)-1].type>=1)&&(mass[me.x+me.wayx*(i-1)-1][me.y+me.wayy*(i-1)-1].trap==0))||
															((mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)+1].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)+1].type<=2)&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)+1].type>=1)&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)+1].trap==0))||
															((mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)-1].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)-1].type<=2)&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)-1].type>=1)&&(mass[me.x+me.wayx*(i-1)][me.y+me.wayy*(i-1)-1].trap==0))||
															((mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)+1].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)+1].type<=2)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)+1].type>=1)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)+1].trap==0))||
															((mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)].type<=2)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)].type>=1)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)].trap==0))||
															((mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)-1].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)-1].type<=2)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)-1].type>=1)&&(mass[me.x+me.wayx*(i-1)+1][me.y+me.wayy*(i-1)-1].trap==0)))))
														{
															while(1)
															{
																int wx=GetRand(2)-1,wy=GetRand(2)-1;
																if ((mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].type<=2)&&(mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].type>=1)&&(mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].trap==0))
																{
																	if (mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].item==0)
																	{
																		mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].item=me.hitem[me.menu1-1];
																		mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].itemk=1;
																		mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].itemf=true;
																		break;
																	}
																	else if ((mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].item==me.hitem[me.menu1-1])&&(item[me.hitem[me.menu1-1]-1].kab==true))
																	{
																		mass[me.x+me.wayx*(i-1)+wx][me.y+me.wayy*(i-1)+wy].itemk+=1;
																		break;
																	}
																}
															}
														}
													}
													break;
												}
												if (i==5)
												{
													if (mass[me.x+me.wayx*i][me.y+me.wayy*i].item==0)
													{
														mass[me.x+me.wayx*i][me.y+me.wayy*i].item=me.hitem[me.menu1-1];
														mass[me.x+me.wayx*i][me.y+me.wayy*i].itemk=1;
														mass[me.x+me.wayx*i][me.y+me.wayy*i].itemf=true;
													}
													else if ((mass[me.x+me.wayx*i][me.y+me.wayy*i].item==me.hitem[me.menu1-1])&&(item[me.hitem[me.menu1-1]-1].kab==true))
													{
														mass[me.x+me.wayx*i][me.y+me.wayy*i].itemk+=1;
													}
													else
													{
														if (((mass[me.x+me.wayx*i-1][me.y+me.wayy*i+1].item==0)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i+1].type<=2)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i+1].type>=1)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i+1].trap==0))||
															((mass[me.x+me.wayx*i-1][me.y+me.wayy*i].item==0)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i].type<=2)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i].type>=1)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i].trap==0))||
															((mass[me.x+me.wayx*i-1][me.y+me.wayy*i-1].item==0)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i-1].type<=2)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i-1].type>=1)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i-1].trap==0))||
															((mass[me.x+me.wayx*i][me.y+me.wayy*i+1].item==0)&&(mass[me.x+me.wayx*i][me.y+me.wayy*i+1].type<=2)&&(mass[me.x+me.wayx*i][me.y+me.wayy*i+1].type>=1)&&(mass[me.x+me.wayx*i][me.y+me.wayy*i+1].trap==0))||
															((mass[me.x+me.wayx*i][me.y+me.wayy*i-1].item==0)&&(mass[me.x+me.wayx*i][me.y+me.wayy*i-1].type<=2)&&(mass[me.x+me.wayx*i][me.y+me.wayy*i-1].type>=1)&&(mass[me.x+me.wayx*i][me.y+me.wayy*i-1].trap==0))||
															((mass[me.x+me.wayx*i+1][me.y+me.wayy*i+1].item==0)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i+1].type<=2)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i+1].type>=1)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i+1].trap==0))||
															((mass[me.x+me.wayx*i+1][me.y+me.wayy*i].item==0)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i].type<=2)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i].type>=1)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i].trap==0))||
															((mass[me.x+me.wayx*i+1][me.y+me.wayy*i-1].item==0)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i-1].type<=2)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i-1].type>=1)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i-1].trap==0))||
															((item[me.hitem[me.menu1-1]-1].kab==true)&&
															(((mass[me.x+me.wayx*i-1][me.y+me.wayy*i+1].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i+1].type<=2)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i+1].type>=1)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i+1].trap==0))||
															((mass[me.x+me.wayx*i-1][me.y+me.wayy*i].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i].type<=2)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i].type>=1)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i].trap==0))||
															((mass[me.x+me.wayx*i-1][me.y+me.wayy*i-1].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i-1].type<=2)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i-1].type>=1)&&(mass[me.x+me.wayx*i-1][me.y+me.wayy*i-1].trap==0))||
															((mass[me.x+me.wayx*i][me.y+me.wayy*i+1].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*i][me.y+me.wayy*i+1].type<=2)&&(mass[me.x+me.wayx*i][me.y+me.wayy*i+1].type>=1)&&(mass[me.x+me.wayx*i][me.y+me.wayy*i+1].trap==0))||
															((mass[me.x+me.wayx*i][me.y+me.wayy*i-1].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*i][me.y+me.wayy*i-1].type<=2)&&(mass[me.x+me.wayx*i][me.y+me.wayy*i-1].type>=1)&&(mass[me.x+me.wayx*i][me.y+me.wayy*i-1].trap==0))||
															((mass[me.x+me.wayx*i+1][me.y+me.wayy*i+1].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i+1].type<=2)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i+1].type>=1)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i+1].trap==0))||
															((mass[me.x+me.wayx*i+1][me.y+me.wayy*i].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i].type<=2)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i].type>=1)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i].trap==0))||
															((mass[me.x+me.wayx*i+1][me.y+me.wayy*i-1].item==me.hitem[me.menu1-1])&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i-1].type<=2)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i-1].type>=1)&&(mass[me.x+me.wayx*i+1][me.y+me.wayy*i-1].trap==0)))))
														{
															while(1)
															{
																int wx=GetRand(2)-1,wy=GetRand(2)-1;
																if ((mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].type<=2)&&(mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].type>=1)&&(mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].trap==0))
																{
																	if (mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].item==0)
																	{
																		mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].item=me.hitem[me.menu1-1];
																		mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].itemk=1;
																		mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].itemf=true;
																		break;
																	}
																	else if ((mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].item==me.hitem[me.menu1-1])&&(item[me.hitem[me.menu1-1]-1].kab==true))
																	{
																		mass[me.x+me.wayx*i+wx][me.y+me.wayy*i+wy].itemk+=1;
																		break;
																	}
																}
															}
														}
													}
												}
											}
											if (item[me.hitem[me.menu1-1]-1].kab==false)
											{
												
												for (int i=me.menu1-1;i<29;i++)
												{
													me.hitem[i]=me.hitem[i+1];
													me.hitemk[i]=me.hitemk[i+1];
													if (me.equipk1==i)me.equipk1--;
													if (me.equipk2==i)me.equipk2--;
													if (me.equipk3==i)me.equipk3--;
													if (me.equipk4==i)me.equipk4--;
												}
												me.hitem[29]=0;
												me.hitemk[29]=0;
												me.mhitem--;
												turntime=1;
												me.menu=0;
												me.menu1=0;
												me.menu2=0;
												mtype=0;
											}
											else
											{
												if (me.hitemk[me.menu1-1]==1)
												{
													for (int i=me.menu1-1;i<29;i++)
													{
														me.hitem[i]=me.hitem[i+1];
														me.hitemk[i]=me.hitemk[i+1];
														if (me.equipk1==i)me.equipk1--;
														if (me.equipk2==i)me.equipk2--;
														if (me.equipk3==i)me.equipk3--;
														if (me.equipk4==i)me.equipk4--;
													}
													me.hitem[29]=0;
													me.hitemk[29]=0;
													me.mhitem--;
													turntime=1;
													me.menu=0;
													me.menu1=0;
													me.menu2=0;
													mtype=0;
												}
												else
												{
													me.hitemk[me.menu1-1]--;
													turntime=1;
													me.menu=0;
													me.menu1=0;
													me.menu2=0;
													mtype=0;
												}
											}
											break;
										case 3://置く
											if (mass[me.x][me.y].item==0)
											{
												messagesend();
												message[0].type=5;
												message[0].val=me.hitem[me.menu1-1];
												mass[me.x][me.y].item=me.hitem[me.menu1-1];
												mass[me.x][me.y].itemk=me.hitemk[me.menu1-1];
												if (me.menu1-1==me.equipk1)me.equipk1=-1,me.equip1=-1;
												if (me.menu1-1==me.equipk2)me.equipk2=-1,me.equip2=-1;
												if (me.menu1-1==me.equipk3)me.equipk3=-1,me.equip3=-1;
												if (me.menu1-1==me.equipk4)me.equipk4=-1,me.equip4=-1;
												for (int i=me.menu1-1;i<29;i++)
												{
													me.hitem[i]=me.hitem[i+1];
													me.hitemk[i]=me.hitemk[i+1];
													if (me.equipk1==i)me.equipk1--;
													if (me.equipk2==i)me.equipk2--;
													if (me.equipk3==i)me.equipk3--;
													if (me.equipk4==i)me.equipk4--;
												}
												me.hitem[29]=0;
												me.hitemk[29]=0;
												me.mhitem--;
												turntime=1;
												me.menu=0;
												me.menu1=0;
												me.menu2=0;
												mtype=0;
											}
											break;
										case 4://やめる
											me.menu2=0;
											break;
										}
									}
									else if (me.menu==2)
									{
										switch(me.menu2)
										{
										case 1://はい
											FILE *fp;
											switch(me.menu1)
											{
											case 1:
												fp=fopen("save\\save1.dat","wb");
												break;
											case 2:
												fp=fopen("save\\save2.dat","wb");
												break;
											case 3:
												fp=fopen("save\\save3.dat","wb");
												break;
											}
											me.menu=0;
											me.menu1=0;
											me.menu2=0;
											fwrite(&me,sizeof(me),1,fp);
											fwrite(&mass,sizeof(MASS),ROOMX*ROOMY,fp);
											fwrite(&room,sizeof(ROOM),20,fp);
											fwrite(&message,sizeof(MESSAGE),8,fp);
											fwrite(&statas,sizeof(STATAS),18,fp);
											fwrite(&enemy,sizeof(ENEMY),50,fp);
											fclose(fp);
											messagesend();
											message[0].type=10;
											message[0].val=me.menu1+1;
											break;
										case 2://いいえ
											me.menu2=0;
											break;
										}
									}
								}
								else 
								{
									if (me.menu==3)
									{
										switch(me.menu1)
										{
										case 1://向き
											me.strdir=1-me.strdir;
											break;
										case 2:
											me.bgm++;
											if (me.bgm>5)me.bgm=0;
											break;
										case 3:
											me.se++;
											if (me.se>5)me.se=0;
											break;
										case 4://ボタン
											me.bt++;
											if (me.bt>15)me.bt=5;
											break;
										case 5://１ターン
											me.wt++;
											if (me.wt>25)me.wt=10;
											break;
										}
									}
									else me.menu2=1;
								}
							}
							else
							{
								if ((me.menu!=1)||(me.mhitem!=0))
								{
									me.menu1=1;
									if (me.menu==1)mtype=1;
								}
							}
						}
						else//攻撃
						{
							for (int i=0;i<ENEMYMAX;i++)
							{
								if ((enemy[i].hp>0)&&(enemy[i].type!=0))
								{
									if ((enemy[i].x==me.x+me.wayx)&&(enemy[i].y==me.y+me.wayy))
									{
										if (((me.wayx*me.wayy!=0)&&(mass[me.x+me.wayx][me.y].type>=1)&&(mass[me.x+me.wayx][me.y].type<=3)&&(mass[me.x][me.y+me.wayy].type>=1)&&(mass[me.x][me.y+me.wayy].type<=3))||(me.wayx*me.wayy==0))
										{
											int d=int(me.atk*(GetRand(10)*0.02+0.9));
											if (me.equip1!=-1)
											{
												d+=item[me.equip1].pal;
												if (item[me.equip1].spe1==1) d+=item[me.equip1].spal1/2;
												if (item[me.equip1].spe2==1) d+=item[me.equip1].spal2/2;
											}
											if (me.equip2!=-1)
											{
												if (item[me.equip2].spe1==1) d+=item[me.equip2].spal1/2;
												if (item[me.equip2].spe2==1) d+=item[me.equip2].spal2/2;
											}
											if (me.equip3!=-1)
											{
												if (item[me.equip3].spe==1) d+=item[me.equip3].pal/2;
												if (item[me.equip3].spe1==1) d+=item[me.equip3].spal1/2;
												if (item[me.equip3].spe2==1) d+=item[me.equip3].spal2/2;
											}
											enemy[i].hp-=d;
											messagesend();
											message[0].type=1;
											message[0].target=enemy[i].type;
											message[0].val=d;
											for (int j=0;j<10;j++)
											{
												if (damage[j].life<=0)
												{
													damage[j].life=50;
													damage[j].x=(enemy[i].x-me.scrolx)*32+25-me.wayx*10;
													damage[j].y=(enemy[i].y-me.scroly)*32+25-me.wayy*10;
													damage[j].ylimit=(enemy[i].y-me.scroly)*32+25-GetRand(3)*5;
													double r=GetRand(314)*0.01;
													damage[j].dx=cos(r)*3;
													damage[j].dy=-sin(r)*3;
													damage[j].amount=d;
													break;
												}
											}
											enemy[i].attacked=true;
											me.score+=d/3;
											me.strike=true;
											if (enemy[i].hp<=0)
											{
												messagesend();
												message[0].type=11;
												message[0].target=enemy[i].type;
												enemy[i].type=0;
												mass[enemy[i].x][enemy[i].y].thing=false;
												me.expr+=enemy[i].atk/2+enemy[i].hpm/5;
												messagesend();
												message[0].type=12;
												message[0].val=enemy[i].atk/2+enemy[i].hpm/5;
												me.score+=enemy[i].atk/4.0+enemy[i].hpm/10.0;
												if (me.expr>=9*me.level-4)
												{
													messagesend();
													message[0].type=6;
													me.expr-=9*me.level-4;
													me.level++;
													me.atk+=GetRand(1)+1;
													int uhp=GetRand(1)+3+me.hpm/(20+GetRand(10)*10);
													me.hp+=uhp;
													me.hpm+=uhp;
												}
											}
										}
									}
								}
							}
							if (mass[me.x+me.wayx][me.y+me.wayy].trap!=0)
							{
								mass[me.x+me.wayx][me.y+me.wayy].trapf=true;
							}
							ma=true;
							turntime=1;
						}
					}
				}
				else pz=false;
			}
			else
			{
				if ((me.move[me.turns]==0)&&(dm==false))me.screen=2;
				if ((turntime==0)&&(me.move[me.turns]==1)) 
				{
					turntime=1;
					messagesend();
					message[0].type=8;
				}
				if (turntime==1) 
				{
					me.ka+=me.hpm*2;
					if (me.hp==me.hpm) me.ka=0;
					if (me.ka>=100)
					{
						if (me.hpm<=me.hp+me.ka/100) 
						{
							me.hp=me.hpm;
							me.ka=0;
						}
						else
						{
							me.hp+=me.ka/100;
							me.ka-=me.ka/100*100;
						}
					}
				}
				if (turntime<me.wt+ma*20+ea*20+(2-me.move[me.turns])*10+(mass[me.x][me.y].trap+3)/4*50) turntime++;
				else if (dm==false)
				{
					pu=false;
					pd=false;
					pr=false;
					pl=false;
					turntime=0;
					me.turns++;
					me.pwayx=0;
					me.pwayy=0;
					me.score++;
					ea=false;
					ma=false;
					for (int i=0;i<50;i++)
					{
						enemy[i].strike=false;
					}
					if (mass[me.x][me.y].trap!=0)//トラップ
					{
						switch(mass[me.x][me.y].trap)
						{
						case 1://落とし穴
							me.floa++;
							ReInit();
							me.screen=3;
							me.score+=20*me.floa*me.floa+30*me.floa;
							time=0;
							break;
						case 2://ワープ
							while(1)
							{
								int a=GetRand(ROOMX-1);
								int b=GetRand(ROOMY-1);
								if ((mass[a][b].type<=2)&&(mass[a][b].type>=1)&&(mass[a][b].thing==false)&&(mass[a][b].trap==0)&&(mass[a][b].item==0))
								{
									mass[me.x][me.y].thing=false;
									mass[a][b].thing=true;
									me.x=a;
									me.y=b;
									me.scrolx=me.x-8;
									me.scroly=me.y-5;
									if (mass[me.x][me.y].type==2)
									{
										for (int i=0;i<20;i++)
										{
											if ((me.x>=room[i].x)&&(me.x<room[i].x+room[i].width)&&(me.y>=room[i].y)&&(me.y<room[i].y+room[i].length))
											{
												me.inroom=i;
												break;
											}
										}
										for (int i=room[me.inroom].x-1;i<room[me.inroom].width+room[me.inroom].x+1;i++)
										{
											for (int j=room[me.inroom].y-1;j<room[me.inroom].length+room[me.inroom].y+1;j++)
											{
												mass[i][j].apeear=true;
												mass[i][j].itemf=true;
											}
										}
									}
									else
									{
										for (int i=0;i<ROOMX;i++)
										{
											for (int j=0;j<ROOMY;j++)
											{
												if ((me.x-i)*(me.x-i)+(me.y-j)*(me.y-j)<4*me.siya)
												{
													mass[i][j].apeear=true;
													mass[i][j].itemf=true;
												}
											}
										}
									}
									break;
								}
							}
							break;
						}
					}
				}
				if (turntime==me.wt-2)
				{
					if ((me.x==me.kx)&&(me.y==me.ky)&&((me.pwayx!=0)||(me.pwayy!=0)))
					{
						dm=true;
						mtype=2;
					}
					//トラップ
					if (mass[me.x][me.y].trap!=0)
					{
						mass[me.x][me.y].trapf=true;
						messagesend();
						message[0].type=15;
						message[0].val=mass[me.x][me.y].trap;
						if (mass[me.x][me.y].trap==3)
						{
							me.hp-=me.hp*0.5;
						}
					}
					//トラップ
				}
				if (turntime==me.wt+ma*20-1)
				{
					me.attacked=false;
					me.strike=false;
					if (mass[me.x][me.y].trap==3)//ダメージ
					{
						me.attacked=true;
					}
					//動く
					int d=0;
					/*for (int i=0;i<ROOMX;i++)
					{
						for (int j=0;j<ROOMY;j++)
						{
							if (mass[i][j].item!=0)
							{
								d++;
							}
						}
					}
					if (d<me.ilimit)
					{
						while (1)
						{
							int a=GetRand(ROOMX-1),b=GetRand(ROOMY-1);
							if ((mass[a][b].thing==false)&&(mass[a][b].item==0)&&((mass[a][b].type==1)||(mass[a][b].type==2))&&((a-me.x)*(a-me.x)+(b-me.y)*(b-me.y)>30))
							{
								mass[a][b].item=GetRand(mitem-1)+1;//アイテムの種類
								int c=0;
								if (me.floa>3)
								{
									for (int i=0;i<mitem;i++)
									{
										c+=(7-item[i].rare)*(7-item[i].rare)*(7-item[i].rare)+(7-item[i].rare)*(7-item[i].rare);
									}
									c=GetRand(c-1);
									for (int i=0;i<mitem;i++)
									{
										if (c<(7-item[i].rare)*(7-item[i].rare)*(7-item[i].rare)+(7-item[i].rare)*(7-item[i].rare))
										{
											mass[a][b].item=i+1;
											break;
										}
										else c-=(7-item[i].rare)*(7-item[i].rare)*(7-item[i].rare)+(7-item[i].rare)*(7-item[i].rare);
									}
									mass[a][b].itemk=1;
									mass[a][b].itemf=false;
									break;
								}
								else
								{
									for (int i=0;i<mitem;i++)
									{
										c+=(7-item[i].rare)*(7-item[i].rare);
									}
									c=GetRand(c-1);
									for (int i=0;i<mitem;i++)
									{
										if (c<(7-item[i].rare)*(7-item[i].rare))
										{
											mass[a][b].item=i+1;
											break;
										}
										else c-=(7-item[i].rare)*(7-item[i].rare);
									}
									mass[a][b].itemk=1;
									mass[a][b].itemf=false;
									break;
								}
							}
						}
					}
					d=0;*/
					for (int i=0;i<50;i++)
					{
						if (enemy[i].hp>0)
						{
							d++;
						}
					}
					if ((d<me.elimit)&&(GetRand(5)==0))
					{
						for (int i=0;i<50;i++)
						{
							if (enemy[i].hp<=0)
							{
								enemy[i].x=0;
								while ((mass[enemy[i].x][enemy[i].y].type==0)||(mass[enemy[i].x][enemy[i].y].thing==true)||(enemy[i].x==0)||((me.x-enemy[i].x)*(me.x-enemy[i].x)+(me.y-enemy[i].y)*(me.y-enemy[i].y)<40)||(mass[enemy[i].x][enemy[i].y].type>3))
								{
									enemy[i].x=GetRand(ROOMX-1);
									enemy[i].y=GetRand(ROOMY-1);
								}
								mass[enemy[i].x][enemy[i].y].thing=true;
								enemy[i].type=me.floa/3+GetRand(2);
								if (enemy[i].type<=0)enemy[i].type=1;
								sta(i,enemy[i].type);
								enemy[i].hp=enemy[i].hpm;
								enemy[i].notice=false;
								break;
							}
						}
					}
					Enemymove();
				}
			}	


			////////////////描画


			draw();
			messagedraw();
			message2draw();
			break;
		case 2://ゲームオーバー
			if (CheckHitKey(KEY_INPUT_Z)==1)
			{
				if (pz==false)
				{
					me.screen=0;
					pz=true;
				}
			}
			else pz=false;
			DrawGraph(0,0,ghandle[6],1);
			SetFontSize(50);
			DrawString(320-GetDrawStringWidth("GAME OVER",9)/2,260,"GAME OVER",GetColor(255,255,255));
			DrawFormatString(320-GetDrawFormatStringWidth("SCORE: %d",me.score)/2,340,GetColor(255,255,255),"SCORE: %d",me.score);
			break;
		case 3://BnF
			SetFontSize(200);
			DrawFormatString(320-GetDrawFormatStringWidth("B %d F",me.floa)/2,140,GetColor(255,255,255),"B %d F",me.floa);
			if (time>30)
			{
				me.screen=1;
				time=GetRand(31416);
			}
			break;
		}
		ScreenFlip();
	}
	DxLib_End();
}
void sta(int number,int type)
{
	int katk=0;
	int khp=0;
	switch(type)
	{
	case 1:
		katk=2+GetRand(2);
		khp=8+GetRand(3);
		break;
	case 2:
		katk=1+GetRand(2);
		khp=12+GetRand(5);
		break;
	case 3:
		katk=3+GetRand(3);
		khp=8+GetRand(4);
		break;
	case 4:
		katk=3+GetRand(1);
		khp=10+GetRand(5);
		break;
	case 5:
		katk=5+GetRand(3);
		khp=3+GetRand(2);
		break;
	case 6:
		katk=5+GetRand(1);
		khp=10+GetRand(5);
		break;
	case 7:
		katk=2+GetRand(2);
		khp=35+GetRand(10);
		break;
	case 8:
		katk=10+GetRand(5);
		khp=10+GetRand(5);
		break;
	case 9:
		katk=15+GetRand(15);
		khp=100+GetRand(25);
		break;
	}
	enemy[number].atk=katk*(1+0.4*me.floa);
	enemy[number].hpm=khp*(1+0.4*me.floa);
}
void messagedraw()
{
	if (me.messagetype==0)//一行だけ
	{
		SetFontSize(20);
		switch(message[0].type)
		{
		case 1:
			DrawFormatString(350,430,GetColor(255,255,255),"%sに%dのダメージ！",enemytype[message[0].target-1].name,message[0].val);
			break;
		case 2:
			DrawFormatString(350,430,GetColor(255,255,255),"%sから%dのダメージを受けた！",enemytype[message[0].target-1].name,message[0].val);
			break;
		case 3:
			if (item[message[0].val-1].kab==true)
			{
				DrawFormatString(350,430,GetColor(255,255,255),"%d個の%sを拾った",message[0].val2,item[message[0].val-1].name);
			}
			else
			{
				DrawFormatString(350,430,GetColor(255,255,255),"%sを拾った",item[message[0].val-1].name);
			}
			break;
		case 4:
			DrawFormatString(350,430,GetColor(255,255,255),"%sを使った！",item[message[0].val-1].name);
			break;
		case 5:
			DrawFormatString(350,430,GetColor(255,255,255),"%sを置いた！",item[message[0].val-1].name);
			break;
		case 6:
			DrawFormatString(350,430,GetColor(255,255,255),"レベルアップ！");
			break;
		case 7:
			if (message[0].val<4)
			{
				if (message[0].val2==1)//上昇
				{
					DrawFormatString(350,430,GetColor(255,255,255),"自分の%sが上昇した！",statas[message[0].val].name);
				}
				else
				{
					DrawFormatString(350,430,GetColor(255,255,255),"自分の%sが低下した！",statas[message[0].val].name);
				break;
				}
			}
		case 8:
			DrawFormatString(350,430,GetColor(255,255,255),"動けない！");
			break;
		case 9:
			DrawFormatString(350,430,GetColor(255,255,255),"死んでしまった！");
			break;
		case 10:
			DrawFormatString(350,430,GetColor(255,255,255),"セーブデータ%dにセーブ完了しました",message[0].val);
			break;
		case 11:
			DrawFormatString(350,430,GetColor(255,255,255),"%sを倒した！",enemytype[message[0].target-1].name);
			break;
		case 12:
			DrawFormatString(350,430,GetColor(255,255,255),"経験値%dを得た！",message[0].val);
			break;
		case 13:
			if(message[0].val<5)
			{
				if (message[0].target==1)
				{
					DrawFormatString(350,430,GetColor(255,255,255),"%sを装備した！",item[message[0].val2-1].name);
				}
				else 
				{
					DrawFormatString(350,430,GetColor(255,255,255),"%sをはずした！",item[message[0].val2-1].name);
				}
			}
			else
			{
				DrawFormatString(350,430,GetColor(255,255,255),"これは使えないぞ！");
			}
			break;
		case 14:
			if (message[0].val2==4)
			{
				DrawFormatString(350,430,GetColor(255,255,255),"%sを打った！",item[message[0].val-1].name);
			}
			else
			{
				DrawFormatString(350,430,GetColor(255,255,255),"%sを投げた！",item[message[0].val-1].name);
			}
			break;
		case 15://罠
			switch(message[0].val)
			{
			case 1:
				DrawFormatString(350,430,GetColor(255,255,255),"落とし穴だ！");
				break;
			case 2:
				DrawFormatString(350,430,GetColor(255,255,255),"ワープゾーンだ！");
				break;
			case 3:
				DrawFormatString(350,430,GetColor(255,255,255),"針の罠だ！");
				break;
			}
			break;
		}
	}
	else if (me.messagetype==2)//3つ
	{
		SetFontSize(15);
		for (int i=0;i<3;i++)
		{
			switch(message[i].type)
			{
			case 1:
				DrawFormatString(350,460-i*25,GetColor(255,255,255),"%sに%dのダメージ！",enemytype[message[i].target-1].name,message[i].val);
				break;
			case 2:
				DrawFormatString(350,460-i*25,GetColor(255,255,255),"%sから%dのダメージを受けた！",enemytype[message[i].target-1].name,message[i].val);
				break;
			case 3:
				if (item[message[i].val-1].kab==true)
				{
					DrawFormatString(350,460-i*25,GetColor(255,255,255),"%d個の%sを拾った",message[i].val2,item[message[i].val-1].name);
				}
				else
				{
					DrawFormatString(350,460-i*25,GetColor(255,255,255),"%sを拾った",item[message[i].val-1].name);
				}
				break;
			case 4:
				DrawFormatString(350,460-i*25,GetColor(255,255,255),"%sを使った！",item[message[i].val-1].name);
				break;
			case 5:
				DrawFormatString(350,460-i*25,GetColor(255,255,255),"%sを置いた！",item[message[i].val-1].name);
				break;
			case 6:
				DrawFormatString(350,460-i*25,GetColor(255,255,255),"レベルアップ！");
				break;
			case 7:
				if (message[i].val<4)
				{
					if (message[i].val2==1)//上昇
					{
						DrawFormatString(350,460-i*25,GetColor(255,255,255),"自分の%sが上昇した！",statas[message[i].val].name);
					}
					else
					{
						DrawFormatString(350,460-i*25,GetColor(255,255,255),"自分の%sが低下した！",statas[message[i].val].name);
					break;
					}
				}
			case 8:
				DrawFormatString(350,460-i*25,GetColor(255,255,255),"動けない！");
				break;
			case 9:
				DrawFormatString(350,460-i*25,GetColor(255,255,255),"死んでしまった！");
				break;
			case 10:
				DrawFormatString(350,460-i*25,GetColor(255,255,255),"セーブデータ%dにセーブ完了しました",message[i].val);
				break;
			case 11:
				DrawFormatString(350,460-i*25,GetColor(255,255,255),"%sを倒した！",enemytype[message[i].target-1].name);
				break;
			case 12:
				DrawFormatString(350,460-i*25,GetColor(255,255,255),"経験値%dを得た！",message[i].val);
				break;
			case 13:
				if(message[i].val<5)
				{
					if (message[i].target==1)
					{
						DrawFormatString(350,460-i*25,GetColor(255,255,255),"%sを装備した！",item[message[i].val2-1].name);
					}
					else 
					{
						DrawFormatString(350,460-i*25,GetColor(255,255,255),"%sをはずした！",item[message[i].val2-1].name);
					}
				}
				else
				{
					DrawFormatString(350,460-i*25,GetColor(255,255,255),"これは使えないぞ！");
				}
				break;
			case 14:
				if (message[i].val2==4)
				{
					DrawFormatString(350,460-i*25,GetColor(255,255,255),"%sを打った！",item[message[i].val-1].name);
				}
				else
				{
					DrawFormatString(350,460-i*25,GetColor(255,255,255),"%sを投げた！",item[message[i].val-1].name);
				}
				break;
			case 15://罠
				switch(message[i].val)
				{
				case 1:
					DrawFormatString(350,460-i*25,GetColor(255,255,255),"落とし穴だ！");
					break;
				case 2:
					DrawFormatString(350,460-i*25,GetColor(255,255,255),"ワープゾーンだ！");
					break;
				case 3:
					DrawFormatString(350,460-i*25,GetColor(255,255,255),"針の罠だ！");
					break;
				}
				break;
			}
		}
	}
}
void messagesend()
{
	for (int i=19;i>0;i--)
	{
		message[i].target=message[i-1].target;
		message[i].type=message[i-1].type;
		message[i].val=message[i-1].val;
		message[i].val2=message[i-1].val2;
	}
}
void message2draw()
{
	if (mtype!=0)
	{
		SetDrawBlendMode(DX_BLENDMODE_SUB,120);
		DrawBox(60,320,380,380,GetColor(255,255,255),1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
		DrawBox(61,325,379,376,GetColor(180,190,50),0);
		DrawBox(65,321,376,379,GetColor(180,190,50),0);
		SetFontSize(14);
		if (mtype==1)
		{
			DrawString(70,332,item[me.hitem[me.menu1-1]-1].describe1,GetColor(255,255,255));
			DrawString(70,354,item[me.hitem[me.menu1-1]-1].describe2,GetColor(255,255,255));
		}
		else 
		{
			DrawString(70,332,"階段をおりますか？",GetColor(255,255,255));
		}
		/*SetDrawBlendMode(DX_BLENDMODE_SUB,120);
		DrawBox(20,20,200,132,GetColor(255,255,255),1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
		DrawBox(21,25,199,128,GetColor(180,190,50),0);
		DrawBox(25,21,196,131,GetColor(180,190,50),0);*/
	}
}
void draw()
{
	DrawRotaGraph(320-cos((time-me.floa*300)*0.01)*60,200-sin((time-me.floa*300)*0.01)*60,1.8,(time+me.floa*300)*0.005,ghandle[6],1);
	for (int i=0;i<ROOMX;i++)
	{
		for (int j=0;j<ROOMY;j++)
		{
			if (mass[i][j].apeear==true)
			{
				if ((i==me.kx)&&(j==me.ky))
				{
					//DrawBox(i*32+10-me.scrolx*32,j*32+10-me.scroly*32,i*32+42-me.scrolx*32,j*32+42-me.scroly*32,GetColor(0,255,255),0);
					DrawGraph(i*32+10-me.scrolx*32,j*32+10-me.scroly*32,ghandle[5],0);
				}
				else if (mass[i][j].type==1)
				{
					if (((mass[me.x][me.y].type==2)&&(room[me.inroom].x-1<=i)&&(room[me.inroom].x+room[me.inroom].width+1>i)&&(room[me.inroom].y-1<=j)&&(room[me.inroom].y+room[me.inroom].length+1>j))||((i-me.x)*(i-me.x)+(j-me.y)*(j-me.y)<4*me.siya))
					{
						DrawGraph(i*32+10-me.scrolx*32,j*32+10-me.scroly*32,ghandle[3],0);
					}
					else
					{
						DrawGraph(i*32+10-me.scrolx*32,j*32+10-me.scroly*32,ghandle[4],0);
					}
				}
				else if (mass[i][j].type==2)
				{
					if (((mass[me.x][me.y].type==2)&&(room[me.inroom].x<=i)&&(room[me.inroom].x+room[me.inroom].width>i)&&(room[me.inroom].y<=j)&&(room[me.inroom].y+room[me.inroom].length>j))||((i-me.x)*(i-me.x)+(j-me.y)*(j-me.y)<4*me.siya))
					{
						DrawGraph(i*32+10-me.scrolx*32,j*32+10-me.scroly*32,ghandle[0],0);
					}
					else
					{
						DrawGraph(i*32+10-me.scrolx*32,j*32+10-me.scroly*32,ghandle[1],0);
					}
					if ((mass[i][j].trap!=0)&&(mass[i][j].trapf==true))
					{
						DrawCircle(i*32+26-me.scrolx*32,j*32+26-me.scroly*32,10,color[mass[i][j].trap-1],0);
						DrawBox(i*32+21-me.scrolx*32,j*32+21-me.scroly*32,i*32+31-me.scrolx*32,j*32+31-me.scroly*32,color[mass[i][j].trap-1],0);
					}
				}
				else 
				{
					DrawGraph(i*32+10-me.scrolx*32,j*32+10-me.scroly*32,ghandle[2],0);
				}
				if ((mass[i][j].item!=0)&&(mass[i][j].itemf==true))
				{
					switch (item[mass[i][j].item-1].imp)
					{
					case 0://消耗品
						DrawBox(i*32+20-me.scrolx*32,j*32+20-me.scroly*32,i*32+32-me.scrolx*32,j*32+32-me.scroly*32,color[item[mass[i][j].item-1].type],1);
						break;
					case 1://武器
						DrawGraph(i*32+10-me.scrolx*32,j*32+10-me.scroly*32,ghandle[10],1);
						break;
					case 2://防具
						DrawGraph(i*32+10-me.scrolx*32,j*32+10-me.scroly*32,ghandle[11],1);
						break;
					case 3://アクセサリー
						break;
					case 4://矢
						DrawGraph(i*32+10-me.scrolx*32,j*32+10-me.scroly*32,ghandle[12],1);
						break;
					case 5://大切なもの
						break;
					}
				}
					/*if (((mass[i][j-1].type==0)&&(j>0))||(j==0))DrawLine(i*32+9-me.scrolx*32,j*32+9-me.scroly*32,i*32+43-me.scrolx*32,j*32+9-me.scroly*32,GetColor(255,0,0));
					if (((mass[i][j+1].type==0)&&(j<19))||(j==19))DrawLine(i*32+9-me.scrolx*32,j*32+43-me.scroly*32,i*32+43-me.scrolx*32,j*32+43-me.scroly*32,GetColor(255,0,0));
					if (((mass[i-1][j].type==0)&&(i>0))||(i==0))DrawLine(i*32+9-me.scrolx*32,j*32+9-me.scroly*32,i*32+9-me.scrolx*32,j*32+43-me.scroly*32,GetColor(255,0,0));
					if (((mass[i+1][j].type==0)&&(i<19))||(i==19))DrawLine(i*32+43-me.scrolx*32,j*32+9-me.scroly*32,i*32+43-me.scrolx*32,j*32+43-me.scroly*32,GetColor(255,0,0));*/
			}
		}
	}
	SetFontSize(20);
	for (int i=0;i<50;i++)//敵の描画
	{
		if ((enemy[i].hp>0)&&((((turntime<=2+me.wt-10)||(turntime>=17+me.wt-10))||((turntime>=8+me.wt-10)&&(turntime<=11+me.wt-10)))||(enemy[i].attacked==false))&&((((mass[me.x][me.y].type==1)||(mass[me.x][me.y].type==3))&&((enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y)<4*me.siya))||
			((mass[me.x][me.y].type==2)&&(enemy[i].x>=room[me.inroom].x-1)&&
			(enemy[i].x<room[me.inroom].x+room[me.inroom].width+1)&&(enemy[i].y>=room[me.inroom].y-1)&&
			(enemy[i].y<room[me.inroom].y+room[me.inroom].length+1))||(me.eseeab==true)))
		{
			DrawRotaGraph(enemy[i].x*32+25-me.scrolx*32+enemy[i].strike*enemy[i].wayx*4,enemy[i].y*32+25-me.scroly*32+enemy[i].strike*enemy[i].wayy*4,1,enemy[i].angle,enemytype[enemy[i].type-1].ghandle,1,0);
			DrawBox(enemy[i].x*32+12-me.scrolx*32+enemy[i].strike*enemy[i].wayx*4,enemy[i].y*32+5-me.scroly*32+enemy[i].strike*enemy[i].wayy*4,
				enemy[i].x*32+12-me.scrolx*32+26.0*enemy[i].hp/enemy[i].hpm+enemy[i].strike*enemy[i].wayx*4,enemy[i].y*32+15-me.scroly*32+enemy[i].strike*enemy[i].wayy*4,GetColor(180,230,30),1);
			DrawBox(enemy[i].x*32+12-me.scrolx*32+enemy[i].strike*enemy[i].wayx*4,enemy[i].y*32+5-me.scroly*32+enemy[i].strike*enemy[i].wayy*4,
				enemy[i].x*32+38-me.scrolx*32+enemy[i].strike*enemy[i].wayx*4,enemy[i].y*32+15-me.scroly*32+enemy[i].strike*enemy[i].wayy*4,GetColor(255,255,255),0);
			/*if (enemy[i].notice==true)
			{
				DrawFormatString(enemy[i].x*32+25-me.scrolx*32-GetDrawFormatStringWidth("%d",i)/2+enemy[i].strike*enemy[i].wayx*4,enemy[i].y*32+15-me.scroly*32+enemy[i].strike*enemy[i].wayy*4,GetColor(0,0,0),"%d",i);
			}
			else
			{
				DrawFormatString(enemy[i].x*32+25-me.scrolx*32-GetDrawFormatStringWidth("%d",i)/2+enemy[i].strike*enemy[i].wayx*4,enemy[i].y*32+15-me.scroly*32+enemy[i].strike*enemy[i].wayy*4,GetColor(255,255,255),"%d",i);
			}*/
		}
	}

	DrawLine(100,100,100+me.wayx*50,100+me.wayy*50,GetColor(255,255,255));
	if ((me.attacked==false)||(((turntime<=me.wt+ma*20+2)||(turntime>=me.wt+ma*20+17))||((turntime>=me.wt+ma*20+8)&&(turntime<=me.wt+ma*20+11))))
	{
		if ((turntime>0)&&(turntime<=me.wt)&&((me.x!=me.kx)||(me.y!=me.ky)||(turntime<me.wt-2)))
		{
			if (me.x+me.pwayx>me.scrolx+10)
			{
				me.scrolx=me.x+me.pwayx*turntime*1.0/me.wt-10;
			}
			else if (me.x+me.pwayx<me.scrolx+6)
			{
				me.scrolx=me.x+me.pwayx*turntime*1.0/me.wt-6;
			}
			if (me.y+me.pwayy>me.scroly+7)
			{
				me.scroly=me.y+me.pwayy*turntime*1.0/me.wt-7;
			}
			else if (me.y+me.pwayy<me.scroly+3)
			{
				me.scroly=me.y+me.pwayy*turntime*1.0/me.wt-3;
			}
			//DrawBox(me.x*32+15-me.scrolx*32+me.strike*me.wayx*4-(me.wt-turntime)*me.pwayx*32.0/me.wt,me.y*32+15-me.scroly*32+me.strike*me.wayy*4-(me.wt-turntime)*me.pwayy*32.0/me.wt,me.x*32+35-me.scrolx*32+me.strike*me.wayx*4-(me.wt-turntime)*me.pwayx*32.0/me.wt,me.y*32+35-me.scroly*32+me.strike*me.wayy*4-(me.wt-turntime)*me.pwayy*32.0/me.wt,GetColor(255,0,0),1);
			DrawRotaGraph(me.x*32+25-me.scrolx*32+me.strike*me.wayx*4-(me.wt-turntime)*me.pwayx*32.0/me.wt,me.y*32+25-me.scroly*32+me.strike*me.wayy*4-(me.wt-turntime)*me.pwayy*32.0/me.wt,1,me.angle,ghandle[9],1,0);
		}
		else
		{
			if (me.x+me.pwayx>me.scrolx+10)
			{
				me.scrolx=me.x+me.pwayx-10;
			}
			else if (me.x+me.pwayx<me.scrolx+6)
			{
				me.scrolx=me.x+me.pwayx-6;
			}
			if (me.y+me.pwayy>me.scroly+7)
			{
				me.scroly=me.y+me.pwayy-7;
			}
			else if (me.y+me.pwayy<me.scroly+3)
			{
				me.scroly=me.y+me.pwayy-3;
			}
			//DrawBox(me.x*32+15-me.scrolx*32+me.strike*me.wayx*4,me.y*32+15-me.scroly*32+me.strike*me.wayy*4,me.x*32+35-me.scrolx*32+me.strike*me.wayx*4,me.y*32+35-me.scroly*32+me.strike*me.wayy*4,GetColor(255,0,0),1);
			DrawRotaGraph(me.x*32+25-me.scrolx*32+me.strike*me.wayx*4,me.y*32+25-me.scroly*32+me.strike*me.wayy*4,1,me.angle,ghandle[9],1,0);
		}
	}
	if (me.hp>me.hpm*0.5)me.color=GetColor(255,255,255);
	else if (me.hp>me.hpm*0.2)me.color=GetColor(255,255,0);
	else me.color=GetColor(255,0,0);
	if (me.hp>me.hpm*0.5)DrawBox(270,20,270+100.0*me.hp/me.hpm,40,GetColor(50,250,50),1);
	else if (me.hp>me.hpm*0.2)DrawBox(270,20,270+100.0*me.hp/me.hpm,40,GetColor(255,255,50),1);
	else DrawBox(270,20,270+100.0*me.hp/me.hpm,40,GetColor(255,50,50),1);
	DrawBox(270,20,370,40,me.color,0);
	DrawBox(270,45,270+100.0*me.expr/(9*me.level-4),55,GetColor(50,255,0),1);
	DrawBox(270,45,370,55,me.color,0);
	DrawFormatString(400,20,me.color,"HP : %d / %d",me.hp,me.hpm);
	DrawBox(0,400,640,480,GetColor(0,0,0),1);
	DrawFormatString(0,0,me.color,"Fl:%d  LV:%d",me.floa,me.level);
	for (int i=0;i<me.tseeab+1;i++)
	{
		if (turntime<me.wt)
		{
			DrawBox(i*40+25-40*turntime/me.wt,425,i*40+55-40*turntime/me.wt,455,GetColor(255,255,255),0);
			if (me.move[me.turns+i]==1)
			{
				DrawString(i*40+40-GetDrawStringWidth("X",1)/2-40*turntime/me.wt,430,"X",GetColor(255,255,255));
			}
			else if (me.move[me.turns+i]==2)
			{
				DrawString(i*40+40-GetDrawStringWidth("O",1)/2-40*turntime/me.wt,430,"O",GetColor(255,255,255));
			}
		}
		else
		{
			DrawBox(i*40-15,425,i*40+15,455,GetColor(255,255,255),0);
			if (me.move[me.turns+i]==1)
			{
				DrawString(i*40-GetDrawStringWidth("X",1)/2,430,"X",GetColor(255,255,255));
			}
			else if (me.move[me.turns+i]==2)
			{
				DrawString(i*40-GetDrawStringWidth("O",1)/2,430,"O",GetColor(255,255,255));
			}
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,150);//ミニマップ
	for (int i=0;i<ROOMX;i++)
	{
		for (int j=0;j<ROOMY;j++)
		{
			if ((mass[i][j].apeear==true)&&((mass[i][j].type>=1)&&(mass[i][j].type<=3)))
			{
				DrawBox(i*10+300,100+j*10,i*10+310,110+j*10,GetColor(255,255,0),1);
				if ((i==me.kx)&&(j==me.ky))
				{
					DrawBox(i*10+302,102+j*10,i*10+308,108+j*10,GetColor(100,100,255),0);
					DrawBox(i*10+303,103+j*10,i*10+307,107+j*10,GetColor(100,100,255),0);
				}
				if ((mass[i][j].itemf==true)&&(mass[i][j].item!=0))
				{
					DrawBox(i*10+302,102+j*10,i*10+308,108+j*10,GetColor(0,200,200),1);
				}
				if ((mass[i][j].trapf==true)&&(mass[i][j].trap!=0))
				{
					DrawLine(i*10+302,102+j*10,i*10+308,108+j*10,GetColor(255,0,255));
					DrawLine(i*10+302,108+j*10,i*10+308,102+j*10,GetColor(255,0,255));
				}
			}
		}	
	}	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
	DrawBox(me.x*10+301,101+me.y*10,me.x*10+309,109+me.y*10,GetColor(0,0,255),1);
	for (int i=0;i<50;i++)
	{
		if ((enemy[i].hp>0)&&((((mass[me.x][me.y].type==1)||(mass[me.x][me.y].type==3))&&((enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y)<4*me.siya))||
			((mass[me.x][me.y].type==2)&&(mass[enemy[i].x][enemy[i].y].type==2)&&(enemy[i].x>=room[me.inroom].x)&&
			(enemy[i].x<room[me.inroom].x+room[me.inroom].width)&&(enemy[i].y>=room[me.inroom].y)&&
			(enemy[i].y<room[me.inroom].y+room[me.inroom].length))||(me.eseeab==true)))
		{
			DrawBox(enemy[i].x*10+301,101+enemy[i].y*10,enemy[i].x*10+309,109+enemy[i].y*10,GetColor(255,0,0),1);
		}
	}
	{
		int a=0;
		for (int i=me.turns+me.tseeab;i<me.mmax;i++)
		{
			if (me.move[i]==2) a++;
		}	
		DrawBox(0,400,20,480,GetColor(0,0,0),1);
		DrawBox(20,420,60,460,GetColor(255,255,0),0);
		SetFontSize(16);
		for (int i=0;i<10;i++)
		{
			if (damage[i].life>0)
			{
				damage[i].life--;
				damage[i].x+=damage[i].dx;
				damage[i].y+=damage[i].dy;
				damage[i].dy+=0.3;
				if (damage[i].y>damage[i].ylimit)
				{
					damage[i].y=damage[i].ylimit;
					damage[i].dy*=-0.4;
					damage[i].dx*=0.5;
				}
				DrawFormatString(damage[i].x-GetDrawFormatStringWidth("%d",damage[i].amount),damage[i].y-8,GetColor(255,255,255),"%d",damage[i].amount);
			}
		}
		SetFontSize(20);
		DrawBox(20+me.tseeab*40,400,640,480,GetColor(0,0,0),1);
		if (a>0)
		{
			DrawFormatString(20+me.tseeab*40+5,418,GetColor(255,255,255),"O : %d",a);
		}
		else
		{
			DrawFormatString(20+me.tseeab*40+5,418,GetColor(255,255,255),"O : -");
		}
		if (me.mmax-me.turns-me.tseeab-a>0)
		{
			DrawFormatString(20+me.tseeab*40+5,442,GetColor(255,255,255),"X : %d",me.mmax-me.turns-me.tseeab-a);
		}
		else
		{
			DrawFormatString(20+me.tseeab*40+5,442,GetColor(255,255,255),"X : -");
		}
	}
	if (dm==true)
	{
		SetDrawBlendMode(DX_BLENDMODE_SUB,120);
		DrawBox(430,310,610,422,GetColor(255,255,255),1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
		DrawBox(431,315,609,418,GetColor(180,190,50),0);
		DrawBox(435,311,606,421,GetColor(180,190,50),0);
		SetFontSize(30);
		DrawString(528-GetDrawStringWidth("はい",4)/2,331,"はい",GetColor(255,255,255));
		DrawString(528-GetDrawStringWidth("いいえ",6)/2,371,"いいえ",GetColor(255,255,255));
		//DrawBox(515,400-30*selecting,585,430-30*selecting,GetColor(255,255,255),0);
		DrawGraph(440,378-selecting*40,ghandle[8],1);
		SetDrawBlendMode(DX_BLENDMODE_ADD,255);
		DrawGraph(460+(time)%60*2,370-selecting*40,ghandle[7],1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
	}
	if (me.menu!=0)
	{
		SetFontSize(30);
		SetDrawBlendMode(DX_BLENDMODE_SUB,120);
		DrawBox(20,20,240,172,GetColor(255,255,255),1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
		DrawBox(21,25,239,168,GetColor(180,190,50),0);
		DrawBox(25,21,236,171,GetColor(180,190,50),0);
		if (me.mhitem==0)
		{
			DrawString(138-GetDrawStringWidth("アイテム",8)/2,41,"アイテム",GetColor(100,100,100));
		}
		else
		{
			DrawString(138-GetDrawStringWidth("アイテム",8)/2,41,"アイテム",GetColor(255,255,255));
		}
		DrawString(138-GetDrawStringWidth("セーブ",6)/2,81,"セーブ",GetColor(255,255,255));
		DrawString(138-GetDrawStringWidth("オプション",10)/2,121,"オプション",GetColor(255,255,255));
		DrawGraph(30,me.menu*40+8,ghandle[8],1);
		if (me.menu1!=0)
		{
			SetDrawBlendMode(DX_BLENDMODE_SUB,120);
			switch (me.menu)
			{
			case 1://アイテム
				DrawBox(50,40,280,312,GetColor(255,255,255),1);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
				DrawBox(51,45,279,308,GetColor(180,190,50),0);
				DrawBox(55,41,276,311,GetColor(180,190,50),0);
				for (int i=(me.menu1-1)/6*6;i<(me.menu1-1)/6*6+6;i++)
				{
					if ((me.hitem[i]!=0)&&(i<30))
					{
						SetFontSize(16);
						if (item[me.hitem[i]-1].kab==false)
						{
							DrawFormatString(173-GetDrawFormatStringWidth("%s",item[me.hitem[i]-1].name)/2,68+(i%6)*40,GetColor(255,255,255),"%s",item[me.hitem[i]-1].name);
						}
						else
						{
							DrawFormatString(173-GetDrawFormatStringWidth("%d個の%s",me.hitemk[i],item[me.hitem[i]-1].name)/2,68+(i%6)*40,GetColor(255,255,255),"%d個の%s",me.hitemk[i],item[me.hitem[i]-1].name);
						}
						if ((me.equipk1==i)||(me.equipk2==i)||(me.equipk3==i)||(me.equipk4==i))
						{
							SetFontSize(10);
							DrawString(143-GetDrawStringWidth("E",1)/2,60+(i%6)*40,"E",GetColor(255,255,255));
						}
					}
				}
				DrawGraph(60,(me.menu1-1)%6*40+68,ghandle[8],1);
				if (me.menu2!=0)
				{
					SetFontSize(30);
					SetDrawBlendMode(DX_BLENDMODE_SUB,120);
					DrawBox(150,70,330,262,GetColor(255,255,255),1);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
					DrawBox(151,75,329,258,GetColor(180,190,50),0);
					DrawBox(155,71,326,261,GetColor(180,190,50),0);
					switch(item[me.hitem[me.menu1-1]-1].imp)
					{
					case 0:
						DrawString(248-GetDrawStringWidth("つかう",6)/2,91,"つかう",GetColor(255,255,255));
						break;
					case 1:
						if (me.equipk1!=me.menu1-1)	DrawString(248-GetDrawStringWidth("装備",4)/2,91,"装備",GetColor(255,255,255));
						else DrawString(248-GetDrawStringWidth("はずす",6)/2,91,"はずす",GetColor(255,255,255));
						break;
					case 2:
						if (me.equipk2!=me.menu1-1) DrawString(248-GetDrawStringWidth("装備",4)/2,91,"装備",GetColor(255,255,255));
						else DrawString(248-GetDrawStringWidth("はずす",6)/2,91,"はずす",GetColor(255,255,255));
						break;
					case 3:
						if (me.equipk3!=me.menu1-1) DrawString(248-GetDrawStringWidth("装備",4)/2,91,"装備",GetColor(255,255,255));
						else DrawString(248-GetDrawStringWidth("はずす",6)/2,91,"はずす",GetColor(255,255,255));
						break;
					case 4:
						if (me.equipk4!=me.menu1-1) DrawString(248-GetDrawStringWidth("装填",4)/2,91,"装填",GetColor(255,255,255));
						else DrawString(248-GetDrawStringWidth("はずす",6)/2,91,"はずす",GetColor(255,255,255));
						break;
					}
					if (item[me.hitem[me.menu1-1]-1].imp==4) DrawString(248-GetDrawStringWidth("うつ",4)/2,131,"うつ",GetColor(255,255,255));
					else DrawString(248-GetDrawStringWidth("なげる",6)/2,131,"なげる",GetColor(255,255,255));
					DrawString(248-GetDrawStringWidth("おく",4)/2,171,"おく",GetColor(255,255,255));
					DrawString(248-GetDrawStringWidth("やめる",6)/2,211,"やめる",GetColor(255,255,255));
					DrawGraph(160,me.menu2*40+58,ghandle[8],1);
					SetDrawBlendMode(DX_BLENDMODE_ADD,255);
					DrawGraph(180+(time)%60*2,me.menu2*40+50,ghandle[7],1);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
				}
				else
				{
					SetDrawBlendMode(DX_BLENDMODE_ADD,255);
					DrawGraph(80+(time)%60*17/6,(me.menu1-1)%6*40+60,ghandle[7],1);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
				}
				break;
			case 2://セーブ
				DrawBox(50,40,230,192,GetColor(255,255,255),1);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
				DrawBox(51,45,229,188,GetColor(180,190,50),0);
				DrawBox(55,41,226,191,GetColor(180,190,50),0);
				DrawString(148-GetDrawStringWidth("データ１",8)/2,61,"データ１",GetColor(255,255,255));
				DrawString(148-GetDrawStringWidth("データ２",8)/2,101,"データ２",GetColor(255,255,255));
				DrawString(148-GetDrawStringWidth("データ３",8)/2,141,"データ３",GetColor(255,255,255));
				DrawGraph(60,me.menu1*40+28,ghandle[8],1);
				if (me.menu2!=0)
				{
					SetDrawBlendMode(DX_BLENDMODE_SUB,120);
					DrawBox(100,70,280,182,GetColor(255,255,255),1);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
					DrawBox(101,75,279,178,GetColor(180,190,50),0);
					DrawBox(105,71,276,181,GetColor(180,190,50),0);
					DrawString(198-GetDrawStringWidth("はい",4)/2,91,"はい",GetColor(255,255,255));
					DrawString(198-GetDrawStringWidth("いいえ",6)/2,131,"いいえ",GetColor(255,255,255));
					DrawGraph(110,me.menu2*40+58,ghandle[8],1);
					SetDrawBlendMode(DX_BLENDMODE_ADD,255);
					DrawGraph(130+(time)%60*2,me.menu2*40+50,ghandle[7],1);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
				}
				else
				{
					SetDrawBlendMode(DX_BLENDMODE_ADD,255);
					DrawGraph(80+(time)%60*2,me.menu1*40+20,ghandle[7],1);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
				}
				break;
			case 3://オプション
				DrawBox(50,40,370,272,GetColor(255,255,255),1);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
				DrawBox(51,45,369,268,GetColor(180,190,50),0);
				DrawBox(55,41,366,271,GetColor(180,190,50),0);
				SetFontSize(14);
				if (me.strdir==false)
				{
					DrawString(218-GetDrawStringWidth("攻撃されたらその向きを向く：なし",32)/2,69,"攻撃されたらその向きを向く：なし",GetColor(255,255,255));
				}
				else
				{
					DrawString(218-GetDrawStringWidth("攻撃されたらその向きを向く：あり",32)/2,69,"攻撃されたらその向きを向く：あり",GetColor(255,255,255));
				}
				DrawFormatString(218-GetDrawFormatStringWidth("BGM:(%d/5)",me.bgm)/2,108,GetColor(255,255,255),"BGM:(%d/5)",me.bgm);
				DrawFormatString(218-GetDrawFormatStringWidth("SE:(%d/5)",me.se)/2,148,GetColor(255,255,255),"SE:(%d/5)",me.se);
				DrawFormatString(218-GetDrawFormatStringWidth("BT:(%d/(5-15))",me.bt)/2,188,GetColor(255,255,255),"BT:(%d/(5-15))",me.bt);
				DrawFormatString(218-GetDrawFormatStringWidth("WT:(%d/(10-25))",me.wt)/2,228,GetColor(255,255,255),"WT:(%d/(10-25))",me.wt);
				DrawGraph(60,me.menu1*40+28,ghandle[8],1);
				SetDrawBlendMode(DX_BLENDMODE_ADD,255);
				DrawGraph(80+(time)%60*13/3,me.menu1*40+20,ghandle[7],1);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
				break;
			}
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_ADD,255);
			DrawGraph(50+(time)%60*8/3,me.menu*40,ghandle[7],1);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); 
	}
}
void Enemymove()
{
	for (int i=0;i<50;i++)
	{
		if (enemy[i].hp>0)
		{
			enemy[i].attacked=false;
			enemy[i].wayx=0;
			enemy[i].wayy=0;
			for (int j=0;j<ROOMX;j++)
			{
				for (int k=0;k<ROOMY;k++)
				{
					massn[j][k]=-1;
					massm[j][k]=-1;
				}
			}
			if (enemy[i].notice==true)
			{
				massn[me.x][me.y]=0;
				massm[me.x][me.y]=0;
				if ((me.x-enemy[i].x)*(me.x-enemy[i].x)+(me.y-enemy[i].y)*(me.y-enemy[i].y)>36)
				{
					enemy[i].notice=false;
				}
			}
			else
			{
				if ((me.x-enemy[i].x)*(me.x-enemy[i].x)+(me.y-enemy[i].y)*(me.y-enemy[i].y)<25) 
				{
					enemy[i].notice=true;
					massn[me.x][me.y]=0;
					massm[me.x][me.y]=0;
				}
				else while(1)
				{
					int a=GetRand(ROOMX-1);
					int b=GetRand(ROOMY-1);
					if ((mass[a][b].type>=1)&&(mass[a][b].type<=3))
					{
						massn[a][b]=0;
						massm[a][b]=0;
						break;
					}
				}
			}
			if (((me.x-enemy[i].x)*(me.x-enemy[i].x)+(me.y-enemy[i].y)*(me.y-enemy[i].y)<3)&&
				(((((enemy[i].x-me.x)*(enemy[i].y-me.y)!=0)&&(mass[enemy[i].x][me.y].type>=1)&&(mass[enemy[i].x][me.y].type<=3)&&
				(mass[me.x][enemy[i].y].type>=1)&&(mass[me.x][enemy[i].y].type<=3)))||((enemy[i].x-me.x)*(enemy[i].y-me.y)==0)))
			{
				int d=int(enemy[i].atk*(GetRand(10)*0.02+0.9));
				if (me.equip1!=-1)
				{
					if (item[me.equip1].spe1==3) d-=item[me.equip1].spal1/2;
					if (item[me.equip1].spe2==3) d-=item[me.equip1].spal2/2;
				}
				if (me.equip2!=-1)
				{
					d-=item[me.equip2].pal;
					if (item[me.equip2].spe1==3) d-=item[me.equip2].spal1/2;
					if (item[me.equip2].spe2==3) d-=item[me.equip2].spal2/2;
				}
				if (me.equip3!=-1)
				{
					if (item[me.equip3].spe==3) d-=item[me.equip3].pal/2;
					if (item[me.equip3].spe1==3) d-=item[me.equip3].spal1/2;
					if (item[me.equip3].spe2==3) d-=item[me.equip3].spal2/2;
				}
				if (d<=1)d=GetRand(2+d);
				if (d<0)d=0;
				me.hp-=d;
				messagesend();
				message[0].type=2;
				message[0].target=enemy[i].type;
				message[0].val=d;
				enemy[i].wayx=me.x-enemy[i].x;
				enemy[i].wayy=me.y-enemy[i].y;
				if (me.strdir==true)
				{
					me.wayx=-enemy[i].wayx;
					me.wayy=-enemy[i].wayy;
				}
				for (int j=0;j<10;j++)
				{
					if (damage[j].life<=0)
					{
						damage[j].life=50;
						damage[j].x=(me.x-me.scrolx)*32+25-enemy[i].wayx*10;
						damage[j].y=(me.y-me.scroly)*32+25-enemy[i].wayy*10;
						damage[j].ylimit=(me.y-me.scroly)*32+25-GetRand(3)*5;
						double r=GetRand(314)*0.01;
						damage[j].dx=cos(r)*3;
						damage[j].dy=-sin(r)*3;
						damage[j].amount=d;
						break;
					}
				}
				ea=true;
				me.attacked=true;
				enemy[i].strike=true;
				me.score+=d/5;
				if (enemy[i].wayx==1){
					if (enemy[i].wayy==1)enemy[i].angle=3.141592/4*3;
					else if (enemy[i].wayy==0)enemy[i].angle=3.141592/4*2;
					else enemy[i].angle=3.141592/4*1;}
				else if (enemy[i].wayx==0){
					if (enemy[i].wayy==1)enemy[i].angle=3.141592/4*4;
					else enemy[i].angle=3.141592/4*0;}
				else{
					if (enemy[i].wayy==1)enemy[i].angle=3.141592/4*5;
					else if (enemy[i].wayy==0)enemy[i].angle=3.141592/4*6;
					else enemy[i].angle=3.141592/4*7;}
				if (me.strdir==true) me.angle=enemy[i].angle+3.141592;
				if (me.hp<1) 
				{
					messagesend();
					message[0].type=9;
					me.screen=2;
				}
			}
			else
			{
				int roop=0;
				while(roop<220)
				{
					roop++;
					for (int j=0;j<ROOMX;j++)
					{
						for (int k=0;k<ROOMY;k++)
						{
							if ((massn[j][k]==-1)&&(mass[j][k].type>=1)&&(mass[j][k].type<=3))
							{
								if ((massn[j-1][k-1]!=-1)&&(massn[j-1][k-1]!=roop)&&(j>0)&&(k>0)&&
									(mass[j][k-1].type>=1)&&(mass[j][k-1].type<=3)&&(mass[j-1][k].type>=1)&&(mass[j-1][k].type<=3))
								{
									massn[j][k]=roop;
								}
								else if ((massn[j-1][k]!=-1)&&(massn[j-1][k]!=roop)&&(j>0))
								{
									massn[j][k]=roop;
								}
								else if ((massn[j-1][k+1]!=-1)&&(massn[j-1][k+1]!=roop)&&(j>0)&&(k<ROOMY-1)&&
									(mass[j][k+1].type>=1)&&(mass[j][k+1].type<=3)&&(mass[j-1][k].type>=1)&&(mass[j-1][k].type<=3))
								{
									massn[j][k]=roop;
								}
								else if ((massn[j][k-1]!=-1)&&(massn[j][k-1]!=roop)&&(k>0))
								{
									massn[j][k]=roop;
								}
								else if ((massn[j+1][k-1]!=-1)&&(massn[j+1][k-1]!=roop)&&(j<ROOMX-1)&&(k>0)&&
									(mass[j][k-1].type>=1)&&(mass[j][k-1].type<=3)&&(mass[j+1][k].type>=1)&&(mass[j+1][k].type<=3))
								{
									massn[j][k]=roop;
								}
								else if ((massn[j][k+1]!=-1)&&(massn[j][k+1]!=roop)&&(k<ROOMY-1))
								{
									massn[j][k]=roop;
								}
								else if ((massn[j+1][k+1]!=-1)&&(massn[j+1][k+1]!=roop)&&(j<ROOMX-1)&&(k<ROOMY-1)&&
									(mass[j][k+1].type>=1)&&(mass[j][k+1].type<=3)&&(mass[j+1][k].type>=1)&&(mass[j+1][k].type<=3))
								{
									massn[j][k]=roop;
								}
								else if ((massn[j+1][k]!=-1)&&(massn[j+1][k]!=roop)&&(j<ROOMX-1))
								{
									massn[j][k]=roop;
								}
							}
						}
					}
					//if (massn[enemy[i].x][enemy[i].y]!=-1)break;
				}
				roop=0;
				while(roop<220)
				{
					roop++;
					for (int j=0;j<ROOMX;j++)
					{
						for (int k=0;k<ROOMY;k++)
						{
							if ((massm[j][k]==-1)&&(mass[j][k].type>=1)&&(mass[j][k].type<=3))
							{
								if ((massm[j-1][k-1]!=-1)&&(massm[j-1][k-1]!=roop)&&(j>0)&&(k>0)&&(mass[j][k-1].type>=1)&&(mass[j][k-1].type<=3)&&(mass[j-1][k].type>=1)&&(mass[j-1][k].type<=3)&&((mass[j-1][k-1].thing==false)||((j-1==me.x)&&(k-1==me.y))))
								{
									massm[j][k]=roop;
								}
								else if ((massm[j-1][k]!=-1)&&(massm[j-1][k]!=roop)&&(j>0)&&((mass[j-1][k].thing==false)||((j-1==me.x)&&(k==me.y))))
								{
									massm[j][k]=roop;
								}
								else if ((massm[j-1][k+1]!=-1)&&(massm[j-1][k+1]!=roop)&&(j>0)&&(k<ROOMY-1)&&(mass[j][k+1].type>=1)&&(mass[j][k+1].type<=3)&&(mass[j-1][k].type>=1)&&(mass[j-1][k].type<=3)&&((mass[j-1][k+1].thing==false)||((j-1==me.x)&&(k+1==me.y))))
								{
									massm[j][k]=roop;
								}
								else if ((massm[j][k-1]!=-1)&&(massm[j][k-1]!=roop)&&(k>0)&&((mass[j][k-1].thing==false)||((j==me.x)&&(k-1==me.y))))
								{
									massm[j][k]=roop;
								}
								else if ((massm[j+1][k-1]!=-1)&&(massm[j+1][k-1]!=roop)&&(j<ROOMX-1)&&(k>0)&&(mass[j][k-1].type>=1)&&(mass[j][k-1].type<=3)&&(mass[j+1][k].type>=1)&&(mass[j+1][k].type<=3)&&((mass[j+1][k-1].thing==false)||((j+1==me.x)&&(k-1==me.y))))
								{
									massm[j][k]=roop;
								}
								else if ((massm[j][k+1]!=-1)&&(massm[j][k+1]!=roop)&&(k<ROOMY-1)&&((mass[j][k+1].thing==false)||((j==me.x)&&(k+1==me.y))))
								{
									massm[j][k]=roop;
								}
								else if ((massm[j+1][k+1]!=-1)&&(massm[j+1][k+1]!=roop)&&(j<ROOMX-1)&&(k<ROOMY-1)&&(mass[j][k+1].type>=1)&&(mass[j][k+1].type<=3)&&(mass[j+1][k].type>=1)&&(mass[j+1][k].type<=3)&&((mass[j+1][k+1].thing==false)||((j+1==me.x)&&(k+1==me.y))))
								{
									massm[j][k]=roop;
								}
								else if ((massm[j+1][k]!=-1)&&(massm[j+1][k]!=roop)&&(j<ROOMX-1)&&((mass[j+1][k].thing==false)||((j+1==me.x)&&(k==me.y))))
								{
									massm[j][k]=roop;
								}
							}
						}
					}
					//if (massm[enemy[i].x][enemy[i].y]!=-1)break;
				}
				if (massm[enemy[i].x][enemy[i].y]==-1)
				{
					bool go[8]={false,false,false,false,false,false,false,false};
					int v=1000000;
					int r=10000;
					if ((enemy[i].x>0)&&(enemy[i].y>0))
					{
						if ((v>=massn[enemy[i].x-1][enemy[i].y-1])&&(massn[enemy[i].x-1][enemy[i].y-1]!=-1)&&
							(mass[enemy[i].x-1][enemy[i].y].type>=1)&&(mass[enemy[i].x-1][enemy[i].y].type<=3)&&
							(mass[enemy[i].x][enemy[i].y-1].type>=1)&&(mass[enemy[i].x][enemy[i].y-1].type<=3))
						{
							if (v==massn[enemy[i].x-1][enemy[i].y-1])
							{
								if ((r==0)||(r>=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y)))
								{	
									if (r>(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y);
									}
									go[0]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x-1][enemy[i].y-1];
								r=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y);
								go[0]=true;
							}
						}
					}
					if (enemy[i].x>0)
					{
						if ((v>=massn[enemy[i].x-1][enemy[i].y])&&(massn[enemy[i].x-1][enemy[i].y]!=-1))
						{
							if (v==massn[enemy[i].x-1][enemy[i].y])
							{
								if ((r==0)||(r>=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y)))
								{
									if (r>(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y);
									}
									go[1]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x-1][enemy[i].y];
								r=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y);
								go[1]=true;
							}
						}
					}
					if ((enemy[i].x>0)&&(enemy[i].y<ROOMY-1))
					{
						if ((v>=massn[enemy[i].x-1][enemy[i].y+1])&&(massn[enemy[i].x-1][enemy[i].y+1]!=-1)&&
							(mass[enemy[i].x-1][enemy[i].y].type>=1)&&(mass[enemy[i].x-1][enemy[i].y].type<=3)&&
							(mass[enemy[i].x][enemy[i].y+1].type>=1)&&(mass[enemy[i].x][enemy[i].y+1].type<=3))
						{
							if (v==massn[enemy[i].x-1][enemy[i].y+1])
							{
								if ((r==0)||(r>=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y)))
								{
									if (r>(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y);
									}
									go[2]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x-1][enemy[i].y+1];
								r=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y);
								go[2]=true;
							}
						}
					}
					if (enemy[i].y>0)
					{
						if ((v>=massn[enemy[i].x][enemy[i].y-1])&&(massn[enemy[i].x][enemy[i].y-1]!=-1))
						{
							if (v==massn[enemy[i].x][enemy[i].y-1])
							{
								if ((r==0)||(r>=(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y)))
								{
									if (r>(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y);
									}
									go[3]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x][enemy[i].y-1];
								r=(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y);
								go[3]=true;
							}
						}
					}
					if (enemy[i].y<ROOMY-1)
					{
						if ((v>=massn[enemy[i].x][enemy[i].y+1])&&(massn[enemy[i].x][enemy[i].y+1]!=-1))
						{
							if (v==massn[enemy[i].x][enemy[i].y+1])
							{
								if ((r==0)||(r>=(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y)))
								{
									if (r>(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y);
									}
									go[4]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x][enemy[i].y+1];
								r=(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y);
								go[4]=true;
							}
						}
					}
					if ((enemy[i].x<ROOMX-1)&&(enemy[i].y>0))
					{
						if ((v>=massn[enemy[i].x+1][enemy[i].y-1])&&(massn[enemy[i].x+1][enemy[i].y-1]!=-1)&&
							(mass[enemy[i].x+1][enemy[i].y].type>=1)&&(mass[enemy[i].x+1][enemy[i].y].type<=3)&&
							(mass[enemy[i].x][enemy[i].y-1].type>=1)&&(mass[enemy[i].x][enemy[i].y-1].type<=3))
						{
							if (v==massn[enemy[i].x+1][enemy[i].y-1])
							{
								if ((r==0)||(r>=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y)))
								{
									if (r>(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y);
									}
									go[5]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x+1][enemy[i].y-1];
								r=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y);
								go[5]=true;
							}
						}
					}
					if (enemy[i].x<ROOMX-1)
					{
						if ((v>=massn[enemy[i].x+1][enemy[i].y])&&(massn[enemy[i].x+1][enemy[i].y]!=-1))
						{
							if (v==massn[enemy[i].x+1][enemy[i].y])
							{
								if ((r==0)||(r>=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y)))
								{
									if (r>(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y);
									}
									go[6]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x+1][enemy[i].y];
								r=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y);
								go[6]=true;
							}
						}
					}
					if ((enemy[i].x<ROOMX-1)&&(enemy[i].y<ROOMY-1))
					{
						if ((v>=massn[enemy[i].x+1][enemy[i].y+1])&&(massn[enemy[i].x+1][enemy[i].y+1]!=-1)&&
							(mass[enemy[i].x+1][enemy[i].y].type>=1)&&(mass[enemy[i].x+1][enemy[i].y].type<=3)&&
							(mass[enemy[i].x][enemy[i].y+1].type>=1)&&(mass[enemy[i].x][enemy[i].y+1].type<=3))
						{
							if (v==massn[enemy[i].x+1][enemy[i].y+1])
							{
								if ((r==0)||(r>=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y)))
								{
									if (r>(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y);
									}
									go[7]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x+1][enemy[i].y+1];
								r=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y);
								go[7]=true;
							}
						}
					}
					v=0;
					for (int j=0;j<8;j++)
					{
						if (go[j]==true)
						{
							v++;
						}
					}
					if (v>0)
					{
						v=GetRand(v-1);
						for (int j=0;j<8;j++)
						{
							if (go[j]==true)
							{
								if (v>0)v--;
								else 
								{
									mass[enemy[i].x][enemy[i].y].thing=false;
									switch(j)
									{
									case 0:
										if (mass[enemy[i].x-1][enemy[i].y-1].thing==false)
										{
											enemy[i].x--;
											enemy[i].y--;
											enemy[i].wayx=-1;
											enemy[i].wayy=-1;
											enemy[i].angle=3.141592/4*7;
										}
										break;
									case 1:
										if (mass[enemy[i].x-1][enemy[i].y].thing==false)
										{
											enemy[i].x--;
											enemy[i].wayx=-1;
											enemy[i].wayy=0;
											enemy[i].angle=3.141592/4*6;
										}
										break;
									case 2:
										if (mass[enemy[i].x-1][enemy[i].y+1].thing==false)
										{
											enemy[i].x--;
											enemy[i].y++;
											enemy[i].wayx=-1;
											enemy[i].wayy=1;
											enemy[i].angle=3.141592/4*5;
										}
										break;
									case 3:
										if (mass[enemy[i].x][enemy[i].y-1].thing==false)
										{
											enemy[i].y--;
											enemy[i].wayx=0;
											enemy[i].wayy=-1;
											enemy[i].angle=3.141592/4*0;
										}
										break;
									case 4:
										if (mass[enemy[i].x][enemy[i].y+1].thing==false)
										{
											enemy[i].y++;
											enemy[i].wayx=0;
											enemy[i].wayy=1;
											enemy[i].angle=3.141592/4*4;
										}
										break;
									case 5:
										if (mass[enemy[i].x+1][enemy[i].y-1].thing==false)
										{
											enemy[i].x++;
											enemy[i].y--;
											enemy[i].wayx=1;
											enemy[i].wayy=-1;
											enemy[i].angle=3.141592/4*1;
										}
										break;
									case 6:
										if (mass[enemy[i].x+1][enemy[i].y].thing==false)
										{
											enemy[i].x++;
											enemy[i].wayx=1;
											enemy[i].wayy=0;
											enemy[i].angle=3.141592/4*2;
										}
										break;
									case 7:
										if (mass[enemy[i].x+1][enemy[i].y+1].thing==false)
										{
											enemy[i].x++;
											enemy[i].y++;
											enemy[i].wayx=1;
											enemy[i].wayy=1;
											enemy[i].angle=3.141592/4*3;
										}
										break;
									}
									mass[enemy[i].x][enemy[i].y].thing=true;
									break;
								}
							}
						}
					}
				}
				else
				{
					bool go[8]={false,false,false,false,false,false,false,false};
					int v=1000000;
					int r=10000;
					if ((enemy[i].x>0)&&(enemy[i].y>0))
					{
						if ((v>=massn[enemy[i].x-1][enemy[i].y-1]*massm[enemy[i].x-1][enemy[i].y-1]*massm[enemy[i].x-1][enemy[i].y-1])
							&&(massm[enemy[i].x-1][enemy[i].y-1]!=-1)&&(massn[enemy[i].x-1][enemy[i].y-1]!=-1)&&
							(mass[enemy[i].x-1][enemy[i].y-1].thing==false)&&
							(mass[enemy[i].x-1][enemy[i].y].type>=1)&&(mass[enemy[i].x-1][enemy[i].y].type<=3)&&
							(mass[enemy[i].x][enemy[i].y-1].type>=1)&&(mass[enemy[i].x][enemy[i].y-1].type<=3))
						{
							if (v==massn[enemy[i].x-1][enemy[i].y-1]*massm[enemy[i].x-1][enemy[i].y-1]*massm[enemy[i].x-1][enemy[i].y-1])
							{
								if ((r==0)||(r>=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y)))
								{	
									if (r>(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y);
									}
									go[0]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x-1][enemy[i].y-1]*massm[enemy[i].x-1][enemy[i].y-1]*massm[enemy[i].x-1][enemy[i].y-1];
								r=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y);
								go[0]=true;
							}
						}
					}
					if (enemy[i].x>0)
					{
						if ((v>=massn[enemy[i].x-1][enemy[i].y]*massm[enemy[i].x-1][enemy[i].y]*massm[enemy[i].x-1][enemy[i].y])
							&&(massm[enemy[i].x-1][enemy[i].y]!=-1)&&(massn[enemy[i].x-1][enemy[i].y]!=-1)&&
							(mass[enemy[i].x-1][enemy[i].y].thing==false))
						{
							if (v==massn[enemy[i].x-1][enemy[i].y]*massm[enemy[i].x-1][enemy[i].y]*massm[enemy[i].x-1][enemy[i].y])
							{
								if ((r==0)||(r>=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y)))
								{
									if (r>(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y);
									}
									go[1]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x-1][enemy[i].y]*massm[enemy[i].x-1][enemy[i].y]*massm[enemy[i].x-1][enemy[i].y];
								r=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y);
								go[1]=true;
							}
						}
					}
					if ((enemy[i].x>0)&&(enemy[i].y<ROOMY-1))
					{
						if ((v>=massn[enemy[i].x-1][enemy[i].y+1]*massm[enemy[i].x-1][enemy[i].y+1]*massm[enemy[i].x-1][enemy[i].y+1])
							&&(massm[enemy[i].x-1][enemy[i].y+1]!=-1)&&(massn[enemy[i].x-1][enemy[i].y+1]!=-1)&&
							(mass[enemy[i].x-1][enemy[i].y+1].thing==false)&&
							(mass[enemy[i].x-1][enemy[i].y].type>=1)&&(mass[enemy[i].x-1][enemy[i].y].type<=3)&&
							(mass[enemy[i].x][enemy[i].y+1].type>=1)&&(mass[enemy[i].x][enemy[i].y+1].type<=3))
						{
							if (v==massn[enemy[i].x-1][enemy[i].y+1]*massm[enemy[i].x-1][enemy[i].y+1]*massm[enemy[i].x-1][enemy[i].y+1])
							{
								if ((r==0)||(r>=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y)))
								{
									if (r>(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y);
									}
									go[2]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x-1][enemy[i].y+1]*massm[enemy[i].x-1][enemy[i].y+1]*massm[enemy[i].x-1][enemy[i].y+1];
								r=(enemy[i].x-1-me.x)*(enemy[i].x-1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y);
								go[2]=true;
							}
						}
					}
					if (enemy[i].y>0)
					{
						if ((v>=massn[enemy[i].x][enemy[i].y-1]*massm[enemy[i].x][enemy[i].y-1]*massm[enemy[i].x][enemy[i].y-1])
							&&(massm[enemy[i].x][enemy[i].y-1]!=-1)&&(massn[enemy[i].x][enemy[i].y-1]!=-1)&&(mass[enemy[i].x][enemy[i].y-1].thing==false))
						{
							if (v==massn[enemy[i].x][enemy[i].y-1]*massm[enemy[i].x][enemy[i].y-1]*massm[enemy[i].x][enemy[i].y-1])
							{
								if ((r==0)||(r>=(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y)))
								{
									if (r>(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y);
									}
									go[3]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x][enemy[i].y-1]*massm[enemy[i].x][enemy[i].y-1]*massm[enemy[i].x][enemy[i].y-1];
								r=(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y);
								go[3]=true;
							}
						}
					}
					if (enemy[i].y<ROOMY-1)
					{
						if ((v>=massn[enemy[i].x][enemy[i].y+1]*massm[enemy[i].x][enemy[i].y+1]*massm[enemy[i].x][enemy[i].y+1])
							&&(massm[enemy[i].x][enemy[i].y+1]!=-1)&&(massn[enemy[i].x][enemy[i].y+1]!=-1)&&(mass[enemy[i].x][enemy[i].y+1].thing==false))
						{
							if (v==massn[enemy[i].x][enemy[i].y+1]*massm[enemy[i].x][enemy[i].y+1]*massm[enemy[i].x][enemy[i].y+1])
							{
								if ((r==0)||(r>=(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y)))
								{
									if (r>(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y);
									}
									go[4]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x][enemy[i].y+1]*massm[enemy[i].x][enemy[i].y+1]*massm[enemy[i].x][enemy[i].y+1];
								r=(enemy[i].x-me.x)*(enemy[i].x-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y);
								go[4]=true;
							}
						}
					}
					if ((enemy[i].x<ROOMX-1)&&(enemy[i].y>0))
					{
						if ((v>=massn[enemy[i].x+1][enemy[i].y-1]*massm[enemy[i].x+1][enemy[i].y-1]*massm[enemy[i].x+1][enemy[i].y-1])
							&&(massm[enemy[i].x+1][enemy[i].y-1]!=-1)&&(massn[enemy[i].x+1][enemy[i].y-1]!=-1)&&
							(mass[enemy[i].x+1][enemy[i].y-1].thing==false)&&(enemy[i].x<ROOMX-1)&&(enemy[i].y>0)&&
							(mass[enemy[i].x+1][enemy[i].y].type>=1)&&(mass[enemy[i].x+1][enemy[i].y].type<=3)&&
							(mass[enemy[i].x][enemy[i].y-1].type>=1)&&(mass[enemy[i].x][enemy[i].y-1].type<=3))
						{
							if (v==massn[enemy[i].x+1][enemy[i].y-1]*massm[enemy[i].x+1][enemy[i].y-1]*massm[enemy[i].x+1][enemy[i].y-1])
							{
								if ((r==0)||(r>=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y)))
								{
									if (r>(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y);
									}
									go[5]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x+1][enemy[i].y-1]*massm[enemy[i].x+1][enemy[i].y-1]*massm[enemy[i].x+1][enemy[i].y-1];
								r=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-1-me.y)*(enemy[i].y-1-me.y);
								go[5]=true;
							}
						}
					}
					if (enemy[i].x<ROOMX-1)
					{
						if ((v>=massn[enemy[i].x+1][enemy[i].y]*massm[enemy[i].x+1][enemy[i].y]*massm[enemy[i].x+1][enemy[i].y])
							&&(massm[enemy[i].x+1][enemy[i].y]!=-1)&&(massn[enemy[i].x+1][enemy[i].y]!=-1)&&(mass[enemy[i].x+1][enemy[i].y].thing==false))
						{
							if (v==massn[enemy[i].x+1][enemy[i].y]*massm[enemy[i].x+1][enemy[i].y]*massm[enemy[i].x+1][enemy[i].y])
							{
								if ((r==0)||(r>=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y)))
								{
									if (r>(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y);
									}
									go[6]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x+1][enemy[i].y]*massm[enemy[i].x+1][enemy[i].y]*massm[enemy[i].x+1][enemy[i].y];
								r=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y-me.y)*(enemy[i].y-me.y);
								go[6]=true;
							}
						}
					}
					if ((enemy[i].x<ROOMX-1)&&(enemy[i].y<ROOMY-1))
					{
						if ((v>=massn[enemy[i].x+1][enemy[i].y+1]*massm[enemy[i].x+1][enemy[i].y+1]*massm[enemy[i].x+1][enemy[i].y+1])
							&&(massm[enemy[i].x+1][enemy[i].y+1]!=-1)&&(massn[enemy[i].x+1][enemy[i].y+1]!=-1)&&
							(mass[enemy[i].x+1][enemy[i].y+1].thing==false)&&
							(mass[enemy[i].x+1][enemy[i].y].type>=1)&&(mass[enemy[i].x+1][enemy[i].y].type<=3)&&
							(mass[enemy[i].x][enemy[i].y+1].type>=1)&&(mass[enemy[i].x][enemy[i].y+1].type<=3))
						{
							if (v==massn[enemy[i].x+1][enemy[i].y+1]*massm[enemy[i].x+1][enemy[i].y+1]*massm[enemy[i].x+1][enemy[i].y+1])
							{
								if ((r==0)||(r>=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y)))
								{
									if (r>(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y))
									{
										for (int j=0;j<8;j++)
										{
											go[j]=false;
										}
										r=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y);
									}
									go[7]=true;
								}
							}
							else
							{
								for (int j=0;j<8;j++)
								{
									go[j]=false;
								}
								v=massn[enemy[i].x+1][enemy[i].y+1]*massm[enemy[i].x+1][enemy[i].y+1]*massm[enemy[i].x+1][enemy[i].y+1];
								r=(enemy[i].x+1-me.x)*(enemy[i].x+1-me.x)+(enemy[i].y+1-me.y)*(enemy[i].y+1-me.y);
								go[7]=true;
							}
						}
					}
					v=0;
					for (int j=0;j<8;j++)
					{
						if (go[j]==true)
						{
							v++;
						}
					}
					if (v>0)
					{
						v=GetRand(v-1);
						for (int j=0;j<8;j++)
						{
							if (go[j]==true)
							{
								if (v>0)v--;
								else
								{
									mass[enemy[i].x][enemy[i].y].thing=false;
									switch(j)
									{
									case 0:
										enemy[i].x--;
										enemy[i].y--;
										enemy[i].wayx=-1;
										enemy[i].wayy=-1;
										enemy[i].angle=3.141592/4*7;
										break;
									case 1:
										enemy[i].x--;
										enemy[i].wayx=-1;
										enemy[i].wayy=0;
										enemy[i].angle=3.141592/4*6;
										break;
									case 2:
										enemy[i].x--;
										enemy[i].y++;
										enemy[i].wayx=-1;
										enemy[i].wayy=1;
										enemy[i].angle=3.141592/4*5;
										break;
									case 3:
										enemy[i].y--;
										enemy[i].wayx=0;
										enemy[i].wayy=-1;
										enemy[i].angle=3.141592/4*0;
										break;
									case 4:
										enemy[i].y++;
										enemy[i].wayx=0;
										enemy[i].wayy=1;
										enemy[i].angle=3.141592/4*4;
										break;
									case 5:
										enemy[i].x++;
										enemy[i].y--;
										enemy[i].wayx=1;
										enemy[i].wayy=-1;
										enemy[i].angle=3.141592/4*1;
										break;
									case 6:
										enemy[i].x++;
										enemy[i].wayx=1;
										enemy[i].wayy=0;
										enemy[i].angle=3.141592/4*2;
										break;
									case 7:
										enemy[i].x++;
										enemy[i].y++;
										enemy[i].wayx=1;
										enemy[i].wayy=1;
										enemy[i].angle=3.141592/4*3;
										break;
									}
									mass[enemy[i].x][enemy[i].y].thing=true;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}
void Init()
{
	ghandle[0]=LoadGraph("pic\\floor7.bmp");
	ghandle[1]=LoadGraph("pic\\floord7.bmp");
	ghandle[2]=LoadGraph("pic\\wall.bmp");
	ghandle[3]=LoadGraph("pic\\floor2.bmp");
	ghandle[4]=LoadGraph("pic\\floord2.bmp");
	ghandle[5]=LoadGraph("pic\\stair2.bmp");
	ghandle[6]=LoadGraph("pic\\背景.bmp");
	ghandle[7]=LoadGraph("pic\\select.bmp");
	ghandle[8]=LoadGraph("pic\\select2.bmp");
	ghandle[9]=LoadGraph("pic\\me2.bmp");
	ghandle[10]=LoadGraph("pic\\剣.bmp");
	ghandle[11]=LoadGraph("pic\\盾.bmp");
	ghandle[12]=LoadGraph("pic\\矢.bmp");
	enemytype[0].ghandle=LoadGraph("pic\\E1.bmp");
	enemytype[1].ghandle=LoadGraph("pic\\E2.bmp");
	enemytype[2].ghandle=LoadGraph("pic\\E3.bmp");
	enemytype[3].ghandle=LoadGraph("pic\\E4.bmp");
	enemytype[4].ghandle=LoadGraph("pic\\E5.bmp");
	enemytype[5].ghandle=LoadGraph("pic\\E6.bmp");
	enemytype[6].ghandle=LoadGraph("pic\\E7.bmp");
	me.wt=15;
	me.bt=10;
	me.siya=1;
	item[0].pal=20;
	strcpy(item[0].name,"ポーション（小）");
	item[0].kab=false;
	item[0].type=1;
	item[0].rare=1;
	strcpy(item[0].describe1,"瓶に入った小さなポーションだ");
	strcpy(item[0].describe2,"体力を20回復できるぞ");
	item[1].pal=2;
	strcpy(item[1].name,"α-ポーション（小）");
	item[1].kab=false;
	item[1].type=2;
	item[1].rare=2;
	strcpy(item[1].describe1,"不思議な色をした小さなポーションだ");
	strcpy(item[1].describe2,"最大体力を2増やせるぞ");
	item[2].pal=5;
	strcpy(item[2].name,"謎の時計（小）");
	item[2].kab=false;
	item[2].type=3;
	item[2].rare=3;
	strcpy(item[2].describe1,"針がたくさんあって何を示しているかわから");
	strcpy(item[2].describe2,"ない。行動できる◯の数を最大5増やせるぞ");
	item[3].pal=20;
	strcpy(item[3].name,"解脱の巻物");
	item[3].kab=false;
	item[3].type=8;
	item[3].rare=5;
	strcpy(item[3].describe1,"中には複雑な呪印があるがよくわからない");
	strcpy(item[3].describe2,"そのままの装備で一階に戻れるぞ");
	item[4].pal=50;
	strcpy(item[4].name,"ポーション（中）");
	item[4].kab=false;
	item[4].type=1;
	item[4].rare=2;
	strcpy(item[4].describe1,"瓶に入ったまあまあのポーションだ");
	strcpy(item[4].describe2,"体力を20回復できるぞ");
	item[5].pal=10;
	strcpy(item[5].name,"謎の時計（大）");
	item[5].kab=false;
	item[5].type=3;
	item[5].rare=4;
	strcpy(item[5].describe1,"針がたくさんあって何を示しているかわから");
	strcpy(item[5].describe2,"ない。行動できる◯の数を最大10増やせるぞ");
	item[6].pal=5;
	strcpy(item[6].name,"α-ポーション（中）");
	item[6].kab=false;
	item[6].type=2;
	item[6].rare=3;
	strcpy(item[6].describe1,"不思議な色をしたまあまあのポーションだ");
	strcpy(item[6].describe2,"最大体力を5増やせるぞ");
	item[7].pal=15;
	strcpy(item[7].name,"α-ポーション（大）");
	item[7].kab=false;
	item[7].type=2;
	item[7].rare=4;
	strcpy(item[7].describe1,"不思議な色をした大きなポーションだ");
	strcpy(item[7].describe2,"最大体力を15増やせるぞ");
	item[8].pal=100;
	strcpy(item[8].name,"ポーション（大）");
	item[8].kab=false;
	item[8].type=1;
	item[8].rare=3;
	strcpy(item[8].describe1,"片手では持てないほどの大きなポーションだ");
	strcpy(item[8].describe2,"体力を100回復できるぞ");
	item[9].pal=200;
	strcpy(item[9].name,"ポーション（特大）");
	item[9].kab=false;
	item[9].type=1;
	item[9].rare=4;
	strcpy(item[9].describe1,"これまでにないほど大きなポーションだ");
	strcpy(item[9].describe2,"体力を200回復できるぞ");
	item[10].pal=500;
	strcpy(item[10].name,"ポーション（究極）");
	item[10].kab=false;
	item[10].type=1;
	item[10].rare=5;
	strcpy(item[10].describe1,"虹色に光るポーションだ");
	strcpy(item[10].describe2,"体力を500回復できるぞ");
	item[11].pal=5;
	strcpy(item[11].name,"木の剣");
	item[11].kab=false;
	item[11].imp=1;
	item[11].rare=3;
	item[11].type=0;
	strcpy(item[11].describe1,"木でできた剣だ");
	strcpy(item[11].describe2,"基本の武器だぞ");
	item[12].pal=8;
	strcpy(item[12].name,"石の剣");
	item[12].kab=false;
	item[12].imp=1;
	item[12].rare=4;
	item[12].type=0;
	strcpy(item[12].describe1,"石でできた剣だ");
	strcpy(item[12].describe2,"少し重い武器だぞ");
	item[13].pal=10;
	strcpy(item[13].name,"鉄の剣");
	item[13].kab=false;
	item[13].imp=1;
	item[13].rare=4;
	item[13].type=0;
	strcpy(item[13].describe1,"鉄でできた剣だ");
	strcpy(item[13].describe2,"重いため動きづらくなるぞ");
	item[14].pal=3;
	strcpy(item[14].name,"木の盾");
	item[14].kab=false;
	item[14].imp=2;
	item[14].rare=3;
	item[14].type=0;
	strcpy(item[14].describe1,"木でできた盾だ");
	strcpy(item[14].describe2,"基本の防具だぞ");
	item[15].pal=5;
	strcpy(item[15].name,"木の矢");
	item[15].kab=true;
	item[15].imp=4;
	item[15].rare=1;
	item[15].type=0;
	strcpy(item[15].describe1,"木でできた矢だ");
	strcpy(item[15].describe2,"遠距離から攻撃できるぞ");
	item[16].pal=7;
	strcpy(item[16].name,"石の矢");
	item[16].kab=true;
	item[16].imp=4;
	item[16].rare=2;
	item[16].type=0;
	strcpy(item[16].describe1,"石でできた矢だ");
	strcpy(item[16].describe2,"やじりが鋭くなっているぞ");
	strcpy(enemytype[0].name,"スライム");
	strcpy(enemytype[1].name,"スライメ");
	strcpy(enemytype[2].name,"スライモ");
	strcpy(enemytype[3].name,"緑風船");
	strcpy(enemytype[4].name,"黄風船");
	strcpy(enemytype[5].name,"赤風船");
	strcpy(enemytype[6].name,"青風船");
}
void GameInit()
{
	if (GetRand(5)==0){ghandle[6]=LoadGraph("pic\\背景.bmp");}
	else if (GetRand(4)==0){ghandle[6]=LoadGraph("pic\\背景２.bmp");}
	else if (GetRand(3)==0){ghandle[6]=LoadGraph("pic\\背景３.bmp");}
	else if (GetRand(2)==0){ghandle[6]=LoadGraph("pic\\背景４.bmp");}
	else if (GetRand(1)==0){ghandle[6]=LoadGraph("pic\\背景５.bmp");}
	else {ghandle[6]=LoadGraph("pic\\背景６.bmp");}
	for (int i=0;i<8;i++)
	{
		message[i].target=0;
		message[i].val2=0;
		message[i].val=0;
		message[i].type=0;
	}
	for (int i=0;i<10;i++)
	{
		damage[i].life=0;
	}
	me.floa=1;
	me.tseeab=5;
	me.mmax=(ROOMX*ROOMY*0.2+ROOMX+ROOMY+70)*(20-DIFFICULT*0.5)*0.05;
	me.mlimit=(ROOMX*ROOMY*0.2+ROOMX+ROOMY+50)*(19-DIFFICULT)*0.05;
	me.ilimit=5;
	me.x=4;
	me.y=4;
	me.elimit=10;//10
	me.atk=4+(GetRand(30)+(20-DIFFICULT)*0.5)*0.1;////////////////////4
	me.hp=15+(GetRand(50)+20-DIFFICULT)*0.1;/////////////////////////////////////15
	me.hpm=me.hp;
	me.expr=0;
	me.level=1;
	me.mhitem=0;
	me.messagetype=2;
	me.score=0;
	me.equipk1=-1;
	me.equipk2=-1;
	me.equipk3=-1;
	me.equipk4=-1;
	me.equip1=-1;
	me.equip2=-1;
	me.equip3=-1;
	me.equip4=-1;
	for (int i=0;i<30;i++)
	{
		me.hitem[i]=0;
		me.hitemk[i]=0;
	}
	for (int i=1;i<ROOMX-1;i++)
	{
		for (int j=1;j<ROOMY-1;j++)
		{
			mass[i][j].type=2;//GetRand(5)/4.0+0.6;
			mass[i][j].itemf==false;
		}
	}
	room[0].x=1;
	room[0].y=1;
	room[0].length=18;
	room[0].width=18;
	while(1)
	{
		int a=0,b=0;
		a=GetRand(ROOMX-1);
		b=GetRand(ROOMY-1);
		if (((mass[a][b].type==1)&&(GetRand(1)==0))||(mass[a][b].type==2))
		{
			mass[a][b].type=3;
			break;
		}
	}
	/*for (int i=0;i<GetRand(5)+5;i++)
	{
		enemy[i].x=-1;
		while ((mass[enemy[i].x][enemy[i].y].type==0)||(mass[enemy[i].x][enemy[i].y].thing==true)||(enemy[i].x==-1))
		{
			enemy[i].x=GetRand(19);
			enemy[i].y=GetRand(19);
		}
		mass[enemy[i].x][enemy[i].y].thing=true;
		enemy[i].hpm=10;
		enemy[i].hp=enemy[i].hpm;
		enemy[i].notice=bool(GetRand(2)/2);
		enemy[i].atk=GetRand(3)+2;
		enemy[i].type=1;
	}*/
	for (int i=0;i<3;i++)
	{	
		for (int j=0;j<3;j++)
		{
			mass[me.x-1+i][me.y-1+j].apeear=true;
		}
	}
	me.scrolx=me.x-8;
	me.scroly=me.y-5;
	me.move[0]=2;
	turntime=0;
	for (int i=1;i<me.mmax;i++)
	{
		me.move[i]=1;//動けない
	}
	for (int i=1;i<me.mlimit;i++)
	{
		while(1)
		{
			int a=GetRand(me.mmax-1)+1;
			if (me.move[a]==1)
			{
				me.move[a]=2;
				break;
			}
		}
	}
	mass[me.x][me.y].thing=true;
}
void ReInit()
{
	me.turns=0;
	me.pwayx=0;
	me.pwayy=0;
	me.wayx=0;
	me.wayy=-1;
	me.angle=0;
	for (int i=0;i<ROOMX;i++)
	{
		for (int j=0;j<ROOMY;j++)
		{
			mass[i][j].apeear=false;
			mass[i][j].trap=0;
			mass[i][j].trapf=false;
			mass[i][j].type=0;
			mass[i][j].thing=false;
			mass[i][j].item=0;
			mass[i][j].itemk=0;
			mass[i][j].itemf=false;
		}
	}
	for (int i=0;i<20;i++)
	{
		road[i].sx=-1;
		road[i].sy=-1;
		road[i].ex=-1;
		road[i].ey=-1;
		road[i].k1x=-1;
		road[i].k1y=-1;
		road[i].k2x=-1;
		road[i].k2y=-1;
		room[i].x=-1;
		room[i].y=-1;
		room[i].width=-1;
		room[i].length=-1;
	}
	int a=GetRand(2)+3;
	while(1)
	{
		room[0].x=GetRand(ROOMX-6)+1;
		room[0].y=GetRand(ROOMY-6)+1;
		room[0].width=GetRand(3)+3;
		room[0].length=GetRand(3)+3;
		if ((room[0].x+room[0].width<ROOMX-1)&&(room[0].y+room[0].length<ROOMY-1)) 
		{
			for (int i=0;i<room[0].width;i++)
			{
				for (int j=0;j<room[0].length;j++)
				{
					mass[room[0].x+i][room[0].y+j].type=2;
				}
				mass[room[0].x+i][room[0].y-1].type=5;
				mass[room[0].x+i][room[0].y+room[0].length].type=5;
			}
			for (int i=0;i<room[0].length;i++)
			{
				mass[room[0].x+room[0].width][room[0].y+i].type=5;
				mass[room[0].x-1][room[0].y+i].type=5;
			}
			break;
		}
	}
	int b=-1;
	int cx=0,cy=0;
	int d=0;//壁に突入した数、二以上でアウト
	int e=0;//
	int count2=0;//全体の回数
	while(1)
	{
		b++;
		while(1)
		{
			count2++;
			loading++;
			if (loading>100000)loading=0;
			d=0;
			e=0;
			road[b].k1x=-1;
			road[b].k1y=-1;
			road[b].k2x=-1;
			road[b].k2y=-1;
			int bx=GetRand(ROOMX-3)+1,by=GetRand(ROOMY-3)+1;
			error=false;
			if (mass[bx][by].type==5)
			{
				if ((mass[bx+1][by].type!=1)&&(mass[bx-1][by].type!=1)&&(mass[bx][by+1].type!=1)&&(mass[bx][by-1].type!=1))
				{
					road[b].sx=bx;
					road[b].sy=by;
					if (mass[road[b].sx+1][road[b].sy].type==2)
					{
						cx=-1;
						cy=0;
					}
					else if (mass[road[b].sx-1][road[b].sy].type==2)
					{
						cx=1;
						cy=0;
					}
					else if (mass[road[b].sx][road[b].sy+1].type==2)
					{
						cx=0;
						cy=-1;
					}
					else if (mass[road[b].sx][road[b].sy-1].type==2)
					{
						cx=0;
						cy=1;
					}
					cx*=GetRand(7)+3;
					cy*=GetRand(7)+3;
					if ((road[b].sx+cx<ROOMX)&&(road[b].sx+cx>0)&&(road[b].sy+cy<ROOMY)&&(road[b].sy+cy>0))
					{
						if ((GetRand(1)==1))//一回も曲がらない時//////////////////////////////////////////////////////////////////////////////
						{
							road[b].ex=road[b].sx+cx;
							road[b].ey=road[b].sy+cy;
							if (mass[road[b].ex][road[b].ey].type==2)
							{
								if (cx>0) while(mass[road[b].ex][road[b].ey].type==2) cx--,road[b].ex--;
								if (cx<0) while(mass[road[b].ex][road[b].ey].type==2) cx++,road[b].ex++;
								if (cx>0) while(mass[road[b].ex][road[b].ey].type==2) cy--,road[b].ey--;
								if (cy<0) while(mass[road[b].ex][road[b].ey].type==2) cy++,road[b].ey++;
							}
							if ((mass[road[b].ex][road[b].ey].type!=2)&&(mass[road[b].ex][road[b].ey].type!=4))
							{
								if (cx>0)
								{
									for (int i=0;i<cx+1;i++)
									{
										if ((mass[road[b].sx+i][road[b].sy].type==2)||(mass[road[b].sx+i][road[b].sy].type==4)||(mass[road[b].sx+i][road[b].sy].type==5))d++;
									}
									if (d>=2+((mass[road[b].ex][road[b].ey].type==1)||(mass[road[b].ex+1][road[b].ey].type==2)))error=true;
									if (error==false)
									{
										for (int i=1;i<cx;i++)
										{
											if ((mass[road[b].sx+i][road[b].sy+1].type==1)||(mass[road[b].sx+i][road[b].sy+1].type==2))
											{
												if ((mass[road[b].sx+i+1][road[b].sy+1].type==1)||(mass[road[b].sx+i+1][road[b].sy+1].type==2))
												{
													error=true;
													break;
												}
											}
											if ((mass[road[b].sx+i][road[b].sy-1].type==1)||(mass[road[b].sx+i][road[b].sy-1].type==1))
											{
												if ((mass[road[b].sx+i+1][road[b].sy-1].type==1)||(mass[road[b].sx+i+1][road[b].sy-1].type==1))
												{
													error=true;
													break;
												}
											}
										}
										if (error==false)
										{
											if ((mass[road[b].ex][road[b].ey].type!=1)&&(mass[road[b].ex+1][road[b].ey].type!=2))
											{
												int count=0;
												while(1)
												{
													error=false;
													count++;
													int fx=GetRand(14)+1,fy=GetRand(14)+1;
													int fw=GetRand(3)+3,fl=GetRand(3)+3;
													if ((fx+fw<ROOMX-1)&&(fy+fl<ROOMY-1))
													{
														for (int i=fx;i<fx+fw;i++)
														{	
															for (int j=fy;j<fy+fl;j++)
															{
																if (mass[i][j].type!=0)
																{
																	error=true;
																	break;
																}
															}
															if (error==true)break;
														}
														for (int i=0;i<20;i++)
														{
															if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
															{
																road[b].sr=i;
															}
														}
														for (int i=0;i<20;i++)
														{
															if (room[i].x==-1)
															{
																road[b].er=i;
																break;
															}
														}
														for (int i=0;i<20;i++)
														{
															if ((road[i].sr!=-1)&&(b!=i))
															{
																if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
															}
														}
														if ((error==false)&&(((fx<=road[b].ex)&&(fx+fw>road[b].ex)&&((road[b].ey==fy-1)||(road[b].ey==fy+fl)))||
															((fy<=road[b].ey)&&(fy+fl>road[b].ey)&&((road[b].ex==fx-1)||(road[b].ex==fx+fw))))&&(road[b].ex+1==fx))
														{
															for (int i=fx;i<fx+fw;i++)
															{	
																for (int j=fy;j<fy+fl;j++)
																{
																	mass[i][j].type=2;
																}
																if (mass[i][fy-1].type==0)
																{
																	mass[i][fy-1].type=5;
																}
																if (mass[i][fy+fl].type==0)
																{
																	mass[i][fy+fl].type=5;
																}
															}
															for (int i=fy;i<fy+fl;i++)
															{	
																if (mass[fx-1][i].type==0)
																{
																mass[fx-1][i].type=5;
																}
																if (mass[fx+fw][i].type==0)
																{
																	mass[fx+fw][i].type=5;
																}
															}
															for (int i=0;i<20;i++)
															{
																if (room[i].x==-1)
																{
																	room[i].x=fx;
																	room[i].y=fy;
																	room[i].width=fw;
																	room[i].length=fl;
																	break;
																}
															}
															break;
														}
													}
													if (count>10000)
													{
														error=true;
														break;
													}
												}
												if (error==false)
												{
													for (int i=0;i<cx+1;i++)
													{
														mass[road[b].sx+i][road[b].sy].type=1;
														if ((mass[road[b].sx+i][road[b].sy-1].type!=1)&&(mass[road[b].sx+i][road[b].sy-1].type!=2)&&(mass[road[b].sx+i][road[b].sy-1].type!=5))
														{
															mass[road[b].sx+i][road[b].sy-1].type=4;
														}
														if ((mass[road[b].sx+i][road[b].sy+1].type!=1)&&(mass[road[b].sx+i][road[b].sy+1].type!=2)&&(mass[road[b].sx+i][road[b].sy+1].type!=5))
														{
															mass[road[b].sx+i][road[b].sy+1].type=4;
														}
													}
													break;
												}
											}
											else 
											{
												for (int i=0;i<20;i++)
												{
													if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
													{
														road[b].sr=i;
													}
													if ((road[b].ex<=room[i].x+room[i].width)&&(road[b].ex>=room[i].x-1)&&(road[b].ey<=room[i].y+room[i].length)&&(road[b].ey>=room[i].y-1))
													{
														road[b].er=i;
													}
												}
												for (int i=0;i<20;i++)
												{
													if ((road[i].sr!=-1)&&(b!=i)&&(road[b].er!=-1))
													{
														if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
													}
												}
												if (error==false)
												{
													for (int i=0;i<cx+1;i++)
													{
														mass[road[b].sx+i][road[b].sy].type=1;
														if ((mass[road[b].sx+i][road[b].sy-1].type!=1)&&(mass[road[b].sx+i][road[b].sy-1].type!=2)&&(mass[road[b].sx+i][road[b].sy-1].type!=5))
														{
															mass[road[b].sx+i][road[b].sy-1].type=4;
														}
														if ((mass[road[b].sx+i][road[b].sy+1].type!=1)&&(mass[road[b].sx+i][road[b].sy+1].type!=2)&&(mass[road[b].sx+i][road[b].sy+1].type!=5))
														{
															mass[road[b].sx+i][road[b].sy+1].type=4;
														}
													}
													break;
												}
											}
										}
									}
								}
								else if (cx<0)
								{
									for (int i=0;i<-cx+1;i++)
									{
										if ((mass[road[b].sx-i][road[b].sy].type==2)||(mass[road[b].sx-i][road[b].sy].type==4)||(mass[road[b].sx-i][road[b].sy].type==5))d++;
									}
									if (d>=2+((mass[road[b].ex][road[b].ey].type==1)||(mass[road[b].ex-1][road[b].ey].type==2)))error=true;
									if (error==false)
									{
										for (int i=1;i<-cx;i++)
										{
											if ((mass[road[b].sx-i][road[b].sy+1].type==1)||(mass[road[b].sx-i][road[b].sy+1].type==2))
											{
												if ((mass[road[b].sx-i-1][road[b].sy+1].type==1)||(mass[road[b].sx-i-1][road[b].sy+1].type==2))
												{
													error=true;
													break;
												}
											}
											if ((mass[road[b].sx-i][road[b].sy-1].type==1)||(mass[road[b].sx-i][road[b].sy-1].type==2))
											{
												if ((mass[road[b].sx-i-1][road[b].sy-1].type==1)||(mass[road[b].sx-i-1][road[b].sy-1].type==2))
												{
													error=true;
													break;
												}
											}
										}
										if (error==false)
										{
											if ((mass[road[b].ex][road[b].ey].type!=1)&&(mass[road[b].ex-1][road[b].ey].type!=2))
											{
												int count=0;
												while(1)
												{
													error=false;
													count++;
													int fx=GetRand(14)+1,fy=GetRand(14)+1;
													int fw=GetRand(3)+3,fl=GetRand(3)+3;
													if ((fx+fw<ROOMX-1)&&(fy+fl<ROOMY-1))
													{
														for (int i=fx;i<fx+fw;i++)
														{	
															for (int j=fy;j<fy+fl;j++)
															{
																if (mass[i][j].type!=0)
																{
																	error=true;
																	break;
																}
															}
															if (error==true)break;
														}
														for (int i=0;i<20;i++)
														{
															if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
															{
																road[b].sr=i;
															}
														}
														for (int i=0;i<20;i++)
														{
															if (room[i].x==-1)
															{
																road[b].er=i;
																break;
															}
														}
														for (int i=0;i<20;i++)
														{
															if ((road[i].sr!=-1)&&(b!=i))
															{
																if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
															}
														}
														if ((error==false)&&(((fx<=road[b].ex)&&(fx+fw>road[b].ex)&&((road[b].ey==fy-1)||(road[b].ey==fy+fl)))||
															((fy<=road[b].ey)&&(fy+fl>road[b].ey)&&((road[b].ex==fx-1)||(road[b].ex==fx+fw))))&&(road[b].ex==fx+fw))
														{
															for (int i=fx;i<fx+fw;i++)
															{	
																for (int j=fy;j<fy+fl;j++)
																{
																	mass[i][j].type=2;
																}
																if (mass[i][fy-1].type==0)
																{
																	mass[i][fy-1].type=5;
																}
																if (mass[i][fy+fl].type==0)
																{
																	mass[i][fy+fl].type=5;
																}
															}
															for (int i=fy;i<fy+fl;i++)
															{	
																if (mass[fx-1][i].type==0)
																{
																mass[fx-1][i].type=5;
																}
																if (mass[fx+fw][i].type==0)
																{
																	mass[fx+fw][i].type=5;
																}
															}
															for (int i=0;i<20;i++)
															{
																if (room[i].x==-1)
																{
																	room[i].x=fx;
																	room[i].y=fy;
																	room[i].width=fw;
																	room[i].length=fl;
																	break;
																}
															}
															break;
														}
													}
													if (count>10000)
													{
														error=true;
														break;
													}
												}
												if (error==false)
												{
													for (int i=0;i<-cx+1;i++)
													{
														mass[road[b].sx-i][road[b].sy].type=1;
														if ((mass[road[b].sx-i][road[b].sy-1].type!=1)&&(mass[road[b].sx-i][road[b].sy-1].type!=2)&&(mass[road[b].sx-i][road[b].sy-1].type!=5))
														{
															mass[road[b].sx-i][road[b].sy-1].type=4;
														}
														if ((mass[road[b].sx-i][road[b].sy+1].type!=1)&&(mass[road[b].sx-i][road[b].sy+1].type!=2)&&(mass[road[b].sx-i][road[b].sy+1].type!=5))
														{
															mass[road[b].sx-i][road[b].sy+1].type=4;
														}
													}
													break;
												}
											}
											else 
											{
												for (int i=0;i<20;i++)
												{
													if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
													{
														road[b].sr=i;
													}
													if ((road[b].ex<=room[i].x+room[i].width)&&(road[b].ex>=room[i].x-1)&&(road[b].ey<=room[i].y+room[i].length)&&(road[b].ey>=room[i].y-1))
													{
														road[b].er=i;
													}
												}
												for (int i=0;i<20;i++)
												{
													if ((road[i].sr!=-1)&&(b!=i)&&(road[b].er!=-1))
													{
														if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
													}
												}
												if (error==false)
												{
													for (int i=0;i<-cx+1;i++)
													{
														mass[road[b].sx-i][road[b].sy].type=1;
														if ((mass[road[b].sx-i][road[b].sy-1].type!=1)&&(mass[road[b].sx-i][road[b].sy-1].type!=2)&&(mass[road[b].sx-i][road[b].sy-1].type!=5))
														{
															mass[road[b].sx-i][road[b].sy-1].type=4;
														}
														if ((mass[road[b].sx-i][road[b].sy+1].type!=1)&&(mass[road[b].sx-i][road[b].sy+1].type!=2)&&(mass[road[b].sx-i][road[b].sy+1].type!=5))
														{
															mass[road[b].sx-i][road[b].sy+1].type=4;
														}
													}
													break;
												}
											}
										}
									}
								}
								else if (cy>0)
								{
									for (int i=0;i<cy+1;i++)
									{
										if ((mass[road[b].sx][road[b].sy+i].type==2)||(mass[road[b].sx][road[b].sy+i].type==4)||(mass[road[b].sx][road[b].sy+i].type==5))d++;
									}
									if (d>=2+((mass[road[b].ex][road[b].ey].type==1)||(mass[road[b].ex][road[b].ey+1].type==2)))error=true;
									if (error==false)
									{
										for (int i=1;i<cy;i++)
										{
											if ((mass[road[b].sx+1][road[b].sy+i].type==1)||(mass[road[b].sx+1][road[b].sy+i].type==2))
											{
												if ((mass[road[b].sx+1][road[b].sy+i+1].type==1)||(mass[road[b].sx+1][road[b].sy+i+1].type==2))
												{
													error=true;
													break;
												}
											}
											if ((mass[road[b].sx+1][road[b].sy+i].type==1)||(mass[road[b].sx+1][road[b].sy+i].type==2))
											{
												if ((mass[road[b].sx-1][road[b].sy+i+1].type==1)||(mass[road[b].sx-1][road[b].sy+i+1].type==2))
												{
													error=true;
													break;
												}
											}
										}
										if (error==false)
										{
											if ((mass[road[b].ex][road[b].ey].type!=1)&&(mass[road[b].ex][road[b].ey+1].type!=2))
											{
												int count=0;
												while(1)
												{
													error=false;
													count++;
													int fx=GetRand(ROOMX-6)+1,fy=GetRand(ROOMY-6)+1;
													int fw=GetRand(3)+3,fl=GetRand(3)+3;
													if ((fx+fw<ROOMX-1)&&(fy+fl<ROOMY-1))
													{
														for (int i=fx;i<fx+fw;i++)
														{	
															for (int j=fy;j<fy+fl;j++)
															{
																if (mass[i][j].type!=0)
																{
																	error=true;
																	break;
																}
															}
															if (error==true)break;
														}
														for (int i=0;i<20;i++)
														{
															if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
															{
																road[b].sr=i;
															}
														}
														for (int i=0;i<20;i++)
														{
															if (room[i].x==-1)
															{
																road[b].er=i;
																break;
															}
														}
														for (int i=0;i<20;i++)
														{
															if ((road[i].sr!=-1)&&(b!=i))
															{
																if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
															}
														}
														if ((error==false)&&(((fx<=road[b].ex)&&(fx+fw>road[b].ex)&&((road[b].ey==fy-1)||(road[b].ey==fy+fl)))||
															((fy<=road[b].ey)&&(fy+fl>road[b].ey)&&((road[b].ex==fx-1)||(road[b].ex==fx+fw))))&&(road[b].ey+1==fy))
														{
															for (int i=fx;i<fx+fw;i++)
															{	
																for (int j=fy;j<fy+fl;j++)
																{
																	mass[i][j].type=2;
																}
																if (mass[i][fy-1].type==0)
																{
																	mass[i][fy-1].type=5;
																}
																if (mass[i][fy+fl].type==0)
																{
																	mass[i][fy+fl].type=5;
																}
															}
															for (int i=fy;i<fy+fl;i++)
															{	
																if (mass[fx-1][i].type==0)
																{
																mass[fx-1][i].type=5;
																}
																if (mass[fx+fw][i].type==0)
																{
																	mass[fx+fw][i].type=5;
																}
															}
															for (int i=0;i<20;i++)
															{
																if (room[i].x==-1)
																{
																	room[i].x=fx;
																	room[i].y=fy;
																	room[i].width=fw;
																	room[i].length=fl;
																	break;
																}
															}
															break;
														}
													}
													if (count>10000)
													{
														error=true;
														break;
													}
												}
												if (error==false)
												{
													for (int i=0;i<cy+1;i++)
													{
														mass[road[b].sx][road[b].sy+i].type=1;
														if ((mass[road[b].sx-1][road[b].sy+i].type!=1)&&(mass[road[b].sx-1][road[b].sy+i].type!=2)&&(mass[road[b].sx-1][road[b].sy+i].type!=5))
														{
															mass[road[b].sx-1][road[b].sy+i].type=4;
														}
														if ((mass[road[b].sx+1][road[b].sy+i].type!=1)&&(mass[road[b].sx+1][road[b].sy+i].type!=2)&&(mass[road[b].sx+1][road[b].sy+i].type!=5))
														{
															mass[road[b].sx+1][road[b].sy+i].type=4;
														}
													}
													break;
												}
											}
											else 
											{
												for (int i=0;i<20;i++)
												{
													if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
													{
														road[b].sr=i;
													}
													if ((road[b].ex<=room[i].x+room[i].width)&&(road[b].ex>=room[i].x-1)&&(road[b].ey<=room[i].y+room[i].length)&&(road[b].ey>=room[i].y-1))
													{
														road[b].er=i;
													}
												}
												for (int i=0;i<20;i++)
												{
													if ((road[i].sr!=-1)&&(b!=i)&&(road[b].er!=-1))
													{
														if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
													}
												}
												if (error==false)
												{
													for (int i=0;i<cy+1;i++)
													{
														mass[road[b].sx][road[b].sy+i].type=1;
														if ((mass[road[b].sx-1][road[b].sy+i].type!=1)&&(mass[road[b].sx-1][road[b].sy+i].type!=2)&&(mass[road[b].sx-1][road[b].sy+i].type!=5))
														{
															mass[road[b].sx-1][road[b].sy+i].type=4;
														}
														if ((mass[road[b].sx+1][road[b].sy+i].type!=1)&&(mass[road[b].sx+1][road[b].sy+i].type!=2)&&(mass[road[b].sx+1][road[b].sy+i].type!=5))
														{
															mass[road[b].sx+1][road[b].sy+i].type=4;
														}
													}
												}
												break;
											}
										}
									}
								}
								else if (cy<0)
								{
									for (int i=0;i<-cy+1;i++)
									{
										if ((mass[road[b].sx][road[b].sy-i].type==2)||(mass[road[b].sx][road[b].sy-i].type==4)||(mass[road[b].sx][road[b].sy-i].type==5))d++;
									}
									if (d>=2+((mass[road[b].ex][road[b].ey].type==1)||(mass[road[b].ex][road[b].ey-1].type==2)))error=true;
									if (error==false)
									{
										for (int i=1;i<-cy;i++)
										{
											if ((mass[road[b].sx+1][road[b].sy-i].type==1)||(mass[road[b].sx+1][road[b].sy-i].type==2))
											{
												if ((mass[road[b].sx+1][road[b].sy-i-1].type==1)||(mass[road[b].sx+1][road[b].sy-i-1].type==2))
												{
													error=true;
													break;
												}
											}
											if ((mass[road[b].sx-1][road[b].sy-i].type==1)||(mass[road[b].sx-1][road[b].sy-i].type==2))
											{
												if ((mass[road[b].sx-1][road[b].sy-i-1].type==1)||(mass[road[b].sx-1][road[b].sy-i-1].type==2))
												{
													error=true;
													break;
												}
											}
										}
										if (error==false)
										{
											if ((mass[road[b].ex][road[b].ey].type!=1)&&(mass[road[b].ex][road[b].ey-1].type!=2))
											{
												int count=0;
												while(1)
												{
													error=false;
													count++;
													int fx=GetRand(ROOMX-6)+1,fy=GetRand(ROOMY-6)+1;
													int fw=GetRand(3)+3,fl=GetRand(3)+3;
													if ((fx+fw<ROOMX-1)&&(fy+fl<ROOMY-1))
													{
														for (int i=fx;i<fx+fw;i++)
														{	
															for (int j=fy;j<fy+fl;j++)
															{
																if (mass[i][j].type!=0)
																{
																	error=true;
																	break;
																}
															}
															if (error==true)break;
														}
														for (int i=0;i<20;i++)
														{
															if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
															{
																road[b].sr=i;
															}
														}
														for (int i=0;i<20;i++)
														{
															if (room[i].x==-1)
															{
																road[b].er=i;
																break;
															}
														}
														for (int i=0;i<20;i++)
														{
															if ((road[i].sr!=-1)&&(b!=i))
															{
																if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
															}
														}
														if ((error==false)&&(((fx<=road[b].ex)&&(fx+fw>road[b].ex)&&((road[b].ey==fy-1)||(road[b].ey==fy+fl)))||
															((fy<=road[b].ey)&&(fy+fl>road[b].ey)&&((road[b].ex==fx-1)||(road[b].ex==fx+fw))))&&(road[b].ey==fy+fl))
														{
															for (int i=fx;i<fx+fw;i++)
															{	
																for (int j=fy;j<fy+fl;j++)
																{
																	mass[i][j].type=2;
																}
																if (mass[i][fy-1].type==0)
																{
																	mass[i][fy-1].type=5;
																}
																if (mass[i][fy+fl].type==0)
																{
																	mass[i][fy+fl].type=5;
																}
															}
															for (int i=fy;i<fy+fl;i++)
															{	
																if (mass[fx-1][i].type==0)
																{
																	mass[fx-1][i].type=5;
																}
																if (mass[fx+fw][i].type==0)
																{
																	mass[fx+fw][i].type=5;
																}
															}
															for (int i=0;i<20;i++)
															{
																if (room[i].x==-1)
																{
																	room[i].x=fx;
																	room[i].y=fy;
																	room[i].width=fw;
																	room[i].length=fl;
																	break;
																}
															}
															break;
														}
													}
													if (count>3000)
													{
														error=true;
														break;
													}
												}
												if (error==false)
												{
													if (-cy>road[b].sy)
													{
														me.scrolx=0;
													}
													for (int i=0;i<-cy+1;i++)
													{
														mass[road[b].sx][road[b].sy-i].type=1;
														if ((mass[road[b].sx-1][road[b].sy-i].type!=1)&&(mass[road[b].sx-1][road[b].sy-i].type!=2)&&(mass[road[b].sx-1][road[b].sy-i].type!=5))
														{
															mass[road[b].sx-1][road[b].sy-i].type=4;
														}
														if ((mass[road[b].sx+1][road[b].sy-i].type!=1)&&(mass[road[b].sx+1][road[b].sy-i].type!=2)&&(mass[road[b].sx+1][road[b].sy-i].type!=5))
														{
															mass[road[b].sx+1][road[b].sy-i].type=4;
														}
													}
													break; 
												}
											}
											else 
											{
												for (int i=0;i<20;i++)
												{
													if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
													{
														road[b].sr=i;
													}
													if ((road[b].ex<=room[i].x+room[i].width)&&(road[b].ex>=room[i].x-1)&&(road[b].ey<=room[i].y+room[i].length)&&(road[b].ey>=room[i].y-1))
													{
														road[b].er=i;
													}
												}
												for (int i=0;i<20;i++)
												{
													if ((road[i].sr!=-1)&&(b!=i)&&(road[b].er!=-1))
													{
														if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
													}
												}
												if (error==false)
												{
													if (-cy>road[b].sy)
													{
														me.scrolx=0;
													}
													for (int i=0;i<-cy+1;i++)
													{
														mass[road[b].sx][road[b].sy-i].type=1;
														if ((mass[road[b].sx-1][road[b].sy-i].type!=1)&&(mass[road[b].sx-1][road[b].sy-i].type!=2)&&(mass[road[b].sx-1][road[b].sy-i].type!=5))
														{
															mass[road[b].sx-1][road[b].sy-i].type=4;
														}
														if ((mass[road[b].sx+1][road[b].sy-i].type!=1)&&(mass[road[b].sx+1][road[b].sy-i].type!=2)&&(mass[road[b].sx+1][road[b].sy-i].type!=5))
														{
															mass[road[b].sx+1][road[b].sy-i].type=4;
														}
													}
													break;
												}
											}
										}
									}
								}
							}
							else error=true;
						}
						else //曲がる時
						{
							road[b].k1x=road[b].sx+cx;
							road[b].k1y=road[b].sy+cy;
							//if ((mass[road[b].k1x][road[b].k1y].type!=0)||(mass[road[b].k1x][road[b].k1y].type!=4));road[b].ex=road[b].sx+cx;
							if ((mass[road[b].k1x][road[b].k1y].type!=1)&&(mass[road[b].k1x][road[b].k1y].type!=2)&&(mass[road[b].k1x][road[b].k1y].type!=4)&&(mass[road[b].k1x][road[b].k1y].type!=5))
							{
								if (cx>0)
								{
									for (int i=0;i<cx+1;i++)
									{
										if ((mass[road[b].sx+i][road[b].sy].type==4)||(mass[road[b].sx+i][road[b].sy].type==5))d++;
										if (mass[road[b].sx+i][road[b].sy].type==1)e++;
										if (mass[road[b].sx+i][road[b].sy].type==2)error=true;
									}
									if (d>=4)error=true;
									if (e>=2)error=true;
									if (error==false)
									{
										for (int i=1;i<cx;i++)
										{
											if (mass[road[b].sx+i][road[b].sy+1].type==1)
											{
												if (mass[road[b].sx+i+1][road[b].sy+1].type==1)
												{
													error=true;
													break;
												}
											}
											if (mass[road[b].sx+i][road[b].sy-1].type==1)
											{
												if (mass[road[b].sx+i+1][road[b].sy-1].type==1)
												{
													error=true;
													break;
												}
											}
										}
										int m=GetRand(1);
										if (cx>0){
											switch(m){
											case 0:cy=1;break;
											case 1:cy=-1;break;}
										}
										else if (cx<0){
											switch(m){
											case 0:cy=1;break;
											case 1:cy=-1;break;}
										}
										else if (cy>0){
											switch(m){
											case 0:cx=1;break;
											case 1:cx=-1;break;}
										}
										else if (cy<0){
											switch(m){
											case 0:cx=1;break;
											case 1:cx=-1;break;}
										}
										cx=0;
										cx*=GetRand(7)+3;
										cy*=GetRand(7)+3;
										d=0;
										e=0;
										if ((road[b].k1x+cx<ROOMX)&&(road[b].k1x+cx>0)&&(road[b].k1y+cy<ROOMY)&&(road[b].k1y+cy>0))
										{
											if (GetRand(0)==0)//もう曲がらない時
											{
												road[b].ex=road[b].k1x+cx;
												road[b].ey=road[b].k1y+cy;
												if (mass[road[b].ex][road[b].ey].type==2)
												{
													if (cx>0) while(mass[road[b].ex][road[b].ey].type==2) cx--,road[b].ex--;
													if (cx<0) while(mass[road[b].ex][road[b].ey].type==2) cx++,road[b].ex++;
													if (cx>0) while(mass[road[b].ex][road[b].ey].type==2) cy--,road[b].ey--;
													if (cy<0) while(mass[road[b].ex][road[b].ey].type==2) cy++,road[b].ey++;
												}
												if ((mass[road[b].ex][road[b].ey].type!=2)&&(mass[road[b].ex][road[b].ey].type!=4))
												{
													if (cy>0)
													{
														for (int i=0;i<cy+1;i++)
														{
															if (mass[road[b].k1x][road[b].k1y+i].type==4) d++;
															if (mass[road[b].k1x][road[b].k1y+i].type==1) e++;
															if ((mass[road[b].k1x][road[b].k1y+i].type==2)||((mass[road[b].k1x][road[b].k1y+i].type==5)&&(i!=cy))) error=true;
														}
														if (d>=3+((mass[road[b].ex][road[b].ey].type==1)||(mass[road[b].ex][road[b].ey+1].type==2))) error=true;
														if (e>=2)error=true;
														if (error==false)
														{
															for (int i=1;i<cy;i++)
															{
																if (mass[road[b].k1x+1][road[b].k1y+i].type==1)
																{
																	if (mass[road[b].k1x+1][road[b].k1y+i+1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
																if (mass[road[b].k1x-1][road[b].k1y+i].type==1)
																{
																	if (mass[road[b].k1x-1][road[b].k1y+i+1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
															}
															if (error==false)
															{
																if ((mass[road[b].ex][road[b].ey].type!=1)&&(mass[road[b].ex][road[b].ey+1].type!=2))
																{
																	int count=0;
																	while(1)
																	{
																		error=false;
																		count++;
																		int fx=GetRand(14)+1,fy=GetRand(14)+1;
																		int fw=GetRand(3)+3,fl=GetRand(3)+3;
																		if ((fx+fw<ROOMX-1)&&(fy+fl<ROOMY-1))
																		{
																			for (int i=fx;i<fx+fw;i++)
																			{	
																				for (int j=fy;j<fy+fl;j++)
																				{
																					if (mass[i][j].type!=0)
																					{
																						error=true;
																						break;
																					}
																				}
																				if (error==true)break;
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																				{
																					road[b].sr=i;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if (room[i].x==-1)
																				{
																					road[b].er=i;
																					break;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[i].sr!=-1)&&(b!=i))
																				{
																					if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																				}
																			}
																			if ((error==false)&&(fx<=road[b].ex)&&(fx+fw>road[b].ex)&&(road[b].ey+1==fy))
																			{
																				for (int i=fx;i<fx+fw;i++)
																				{	
																					for (int j=fy;j<fy+fl;j++)
																					{
																						mass[i][j].type=2;
																					}
																					if (mass[i][fy-1].type==0)
																					{
																						mass[i][fy-1].type=5;
																					}
																					if (mass[i][fy+fl].type==0)
																					{
																						mass[i][fy+fl].type=5;
																					}
																				}
																				for (int i=fy;i<fy+fl;i++)
																				{	
																					if (mass[fx-1][i].type==0)
																					{
																						mass[fx-1][i].type=5;
																					}
																					if (mass[fx+fw][i].type==0)
																					{
																						mass[fx+fw][i].type=5;
																					}
																				}
																				for (int i=0;i<20;i++)
																				{
																					if (room[i].x==-1)
																					{
																						room[i].x=fx;
																						room[i].y=fy;
																						room[i].width=fw;
																						room[i].length=fl;
																						break;
																					}
																				}
																				break;
																			}
																		}
																		if (count>10000)
																		{
																			error=true;
																			break;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].k1x-road[b].sx+1;i++)
																		{
																			mass[road[b].sx+i][road[b].sy].type=1;
																			if ((mass[road[b].sx+i][road[b].sy-1].type!=1)&&(mass[road[b].sx+i][road[b].sy-1].type!=2)&&(mass[road[b].sx+i][road[b].sy-1].type!=5))
																			{
																				mass[road[b].sx+i][road[b].sy-1].type=4;
																			}
																			if ((mass[road[b].sx+i][road[b].sy+1].type!=1)&&(mass[road[b].sx+i][road[b].sy+1].type!=2)&&(mass[road[b].sx+i][road[b].sy+1].type!=5))
																			{
																				mass[road[b].sx+i][road[b].sy+1].type=4;
																			}
																		}
																		for (int i=0;i<road[b].ey-road[b].k1y+1;i++)
																		{
																			mass[road[b].k1x][road[b].k1y+i].type=1;
																			if ((mass[road[b].k1x-1][road[b].k1y+i].type!=1)&&(mass[road[b].k1x-1][road[b].k1y+i].type!=2)&&(mass[road[b].k1x-1][road[b].k1y+i].type!=5))
																			{
																				mass[road[b].k1x-1][road[b].k1y+i].type=4;
																			}
																			if ((mass[road[b].k1x+1][road[b].k1y+i].type!=1)&&(mass[road[b].k1x+1][road[b].k1y+i].type!=2)&&(mass[road[b].k1x+1][road[b].k1y+i].type!=5))
																			{
																				mass[road[b].k1x+1][road[b].k1y+i].type=4;
																			}
																		}
																		break;
																	}
																}
																else 
																{
																	for (int i=0;i<20;i++)
																	{
																		if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																		{
																			road[b].sr=i;
																		}
																		if ((road[b].ex<=room[i].x+room[i].width)&&(road[b].ex>=room[i].x-1)&&(road[b].ey<=room[i].y+room[i].length)&&(road[b].ey>=room[i].y-1))
																		{
																			road[b].er=i;
																		}
																	}
																	for (int i=0;i<20;i++)
																	{
																		if ((road[i].sr!=-1)&&(b!=i)&&(road[b].er!=-1))
																		{
																			if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].k1x-road[b].sx+1;i++)
																		{
																			mass[road[b].sx+i][road[b].sy].type=1;
																			if ((mass[road[b].sx+i][road[b].sy-1].type!=1)&&(mass[road[b].sx+i][road[b].sy-1].type!=2)&&(mass[road[b].sx+i][road[b].sy-1].type!=5))
																			{
																				mass[road[b].sx+i][road[b].sy-1].type=4;
																			}
																			if ((mass[road[b].sx+i][road[b].sy+1].type!=1)&&(mass[road[b].sx+i][road[b].sy+1].type!=2)&&(mass[road[b].sx+i][road[b].sy+1].type!=5))
																			{
																				mass[road[b].sx+i][road[b].sy+1].type=4;
																			}
																		}
																		for (int i=0;i<road[b].ey-road[b].k1y+1;i++)
																		{
																			mass[road[b].k1x][road[b].k1y+i].type=1;
																			if ((mass[road[b].k1x-1][road[b].k1y+i].type!=1)&&(mass[road[b].k1x-1][road[b].k1y+i].type!=2)&&(mass[road[b].k1x-1][road[b].k1y+i].type!=5))
																			{
																				mass[road[b].k1x-1][road[b].k1y+i].type=4;
																			}
																			if ((mass[road[b].k1x+1][road[b].k1y+i].type!=1)&&(mass[road[b].k1x+1][road[b].k1y+i].type!=2)&&(mass[road[b].k1x+1][road[b].k1y+i].type!=5))
																			{
																				mass[road[b].k1x+1][road[b].k1y+i].type=4;
																			}
																		}
																		break;
																	}
																}	
															}
														}
													}
													else if (cy<0)
													{
														for (int i=0;i<-cy+1;i++)
														{
															if (mass[road[b].k1x][road[b].k1y-i].type==4) d++;
															if (mass[road[b].k1x][road[b].k1y-i].type==1) e++;
															if ((mass[road[b].k1x][road[b].k1y-i].type==2)||((mass[road[b].k1x][road[b].k1y-i].type==5)&&(i!=-cy))) error=true;
														}
														if (d>=3+((mass[road[b].ex][road[b].ey].type==1)||(mass[road[b].ex][road[b].ey-1].type==2))) error=true;
														if (e>=2)error=true;
														if (error==false)
														{
															for (int i=1;i<-cy;i++)
															{
																if (mass[road[b].k1x+1][road[b].k1y-i].type==1)
																{
																	if (mass[road[b].k1x+1][road[b].k1y-i-1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
																if (mass[road[b].k1x-1][road[b].k1y-i].type==1)
																{
																	if (mass[road[b].k1x-1][road[b].k1y-i-1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
															}
															if (error==false)
															{
																if ((mass[road[b].ex][road[b].ey].type!=1)&&(mass[road[b].ex][road[b].ey-1].type!=2))
																{
																	int count=0;
																	while(1)
																	{
																		error=false;
																		count++;
																		int fx=GetRand(14)+1,fy=GetRand(14)+1;
																		int fw=GetRand(3)+3,fl=GetRand(3)+3;
																		if ((fx+fw<ROOMX-1)&&(fy+fl<ROOMY-1))
																		{
																			for (int i=fx;i<fx+fw;i++)
																			{	
																				for (int j=fy;j<fy+fl;j++)
																				{
																					if (mass[i][j].type!=0)
																					{
																						error=true;
																						break;
																					}
																				}
																				if (error==true)break;
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																				{
																					road[b].sr=i;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if (room[i].x==-1)
																				{
																					road[b].er=i;
																					break;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[i].sr!=-1)&&(b!=i))
																				{
																					if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																				}
																			}
																			if ((error==false)&&(fx<=road[b].ex)&&(fx+fw>road[b].ex)&&(road[b].ey==fy+fl))
																			{
																				for (int i=fx;i<fx+fw;i++)
																				{	
																					for (int j=fy;j<fy+fl;j++)
																					{
																						mass[i][j].type=2;
																					}
																					if (mass[i][fy-1].type==0)
																					{
																						mass[i][fy-1].type=5;
																					}
																					if (mass[i][fy+fl].type==0)
																					{
																						mass[i][fy+fl].type=5;
																					}
																				}
																				for (int i=fy;i<fy+fl;i++)
																				{	
																					if (mass[fx-1][i].type==0)
																					{
																						mass[fx-1][i].type=5;
																					}
																					if (mass[fx+fw][i].type==0)
																					{
																						mass[fx+fw][i].type=5;
																					}
																				}
																				for (int i=0;i<20;i++)
																				{
																					if (room[i].x==-1)
																					{
																						room[i].x=fx;
																						room[i].y=fy;
																						room[i].width=fw;
																						room[i].length=fl;
																						break;
																					}
																				}
																				break;
																			}
																		}
																		if (count>10000)
																		{
																			error=true;
																			break;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].k1x-road[b].sx+1;i++)
																		{
																			mass[road[b].sx+i][road[b].sy].type=1;
																			if ((mass[road[b].sx+i][road[b].sy-1].type!=1)&&(mass[road[b].sx+i][road[b].sy-1].type!=2)&&(mass[road[b].sx+i][road[b].sy-1].type!=5))
																			{
																				mass[road[b].sx+i][road[b].sy-1].type=4;
																			}
																			if ((mass[road[b].sx+i][road[b].sy+1].type!=1)&&(mass[road[b].sx+i][road[b].sy+1].type!=2)&&(mass[road[b].sx+i][road[b].sy+1].type!=5))
																			{
																				mass[road[b].sx+i][road[b].sy+1].type=4;
																			}
																		}
																		for (int i=0;i<road[b].k1y-road[b].ey+1;i++)
																		{
																			mass[road[b].k1x][road[b].k1y-i].type=1;
																			if ((mass[road[b].k1x-1][road[b].k1y-i].type!=1)&&(mass[road[b].k1x-1][road[b].k1y-i].type!=2)&&(mass[road[b].k1x-1][road[b].k1y-i].type!=5))
																			{
																				mass[road[b].k1x-1][road[b].k1y-i].type=4;
																			}
																			if ((mass[road[b].k1x+1][road[b].k1y-i].type!=1)&&(mass[road[b].k1x+1][road[b].k1y-i].type!=2)&&(mass[road[b].k1x+1][road[b].k1y-i].type!=5))
																			{
																				mass[road[b].k1x+1][road[b].k1y-i].type=4;
																			}
																		}
																		break;
																	}
																}
																else 
																{
																	for (int i=0;i<20;i++)
																	{
																		if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																		{
																			road[b].sr=i;
																		}
																		if ((road[b].ex<=room[i].x+room[i].width)&&(road[b].ex>=room[i].x-1)&&(road[b].ey<=room[i].y+room[i].length)&&(road[b].ey>=room[i].y-1))
																		{
																			road[b].er=i;
																		}
																	}
																	for (int i=0;i<20;i++)
																	{
																		if ((road[i].sr!=-1)&&(b!=i)&&(road[b].er!=-1))
																		{
																			if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].k1x-road[b].sx+1;i++)
																		{
																			mass[road[b].sx+i][road[b].sy].type=1;
																			if ((mass[road[b].sx+i][road[b].sy-1].type!=1)&&(mass[road[b].sx+i][road[b].sy-1].type!=2)&&(mass[road[b].sx+i][road[b].sy-1].type!=5))
																			{
																				mass[road[b].sx+i][road[b].sy-1].type=4;
																			}
																			if ((mass[road[b].sx+i][road[b].sy+1].type!=1)&&(mass[road[b].sx+i][road[b].sy+1].type!=2)&&(mass[road[b].sx+i][road[b].sy+1].type!=5))
																			{
																				mass[road[b].sx+i][road[b].sy+1].type=4;
																			}
																		}
																		for (int i=0;i<road[b].k1y-road[b].ey+1;i++)
																		{
																			mass[road[b].k1x][road[b].k1y-i].type=1;
																			if ((mass[road[b].k1x-1][road[b].k1y-i].type!=1)&&(mass[road[b].k1x-1][road[b].k1y-i].type!=2)&&(mass[road[b].k1x-1][road[b].k1y-i].type!=5))
																			{
																				mass[road[b].k1x-1][road[b].k1y-i].type=4;
																			}
																			if ((mass[road[b].k1x+1][road[b].k1y-i].type!=1)&&(mass[road[b].k1x+1][road[b].k1y-i].type!=2)&&(mass[road[b].k1x+1][road[b].k1y-i].type!=5))
																			{
																				mass[road[b].k1x+1][road[b].k1y-i].type=4;
																			}
																		}
																		break;
																	}
																}	
															}
														}
													}
												}
											}
											else//２回曲がる時
											{
											}
										}
									}
								}
								else if (cx<0)
								{
									for (int i=0;i<-cx+1;i++)
									{
										if ((mass[road[b].sx-i][road[b].sy].type==4)||(mass[road[b].sx-i][road[b].sy].type==5))d++;
										if (mass[road[b].sx-i][road[b].sy].type==1)e++;
										if (mass[road[b].sx-i][road[b].sy].type==2)error=true;
									}
									if (d>=4)error=true;
									if (e>=2)error=true;
									if (error==false)
									{
										for (int i=1;i<-cx;i++)
										{
											if (mass[road[b].sx-i][road[b].sy+1].type==1)
											{
												if (mass[road[b].sx-i-1][road[b].sy+1].type==1)
												{
													error=true;
													break;
												}
											}
											if (mass[road[b].sx-i][road[b].sy-1].type==1)
											{
												if (mass[road[b].sx-i-1][road[b].sy-1].type==1)
												{
													error=true;
													break;
												}
											}
										}
										int m=GetRand(1);
										if (cx>0){
											switch(m){
											case 0:cy=1;break;
											case 1:cy=-1;break;}
										}
										else if (cx<0){
											switch(m){
											case 0:cy=1;break;
											case 1:cy=-1;break;}
										}
										else if (cy>0){
											switch(m){
											case 0:cx=1;break;
											case 1:cx=-1;break;}
										}
										else if (cy<0){
											switch(m){
											case 0:cx=1;break;
											case 1:cx=-1;break;}
										}
										cx=0;
										cx*=GetRand(7)+3;
										cy*=GetRand(7)+3;
										d=0;
										e=0;
										if ((road[b].k1x+cx<ROOMX)&&(road[b].k1x+cx>0)&&(road[b].k1y+cy<ROOMY)&&(road[b].k1y+cy>0))
										{
											if (GetRand(0)==0)//もう曲がらない時
											{
												road[b].ex=road[b].k1x+cx;
												road[b].ey=road[b].k1y+cy;
												if (mass[road[b].ex][road[b].ey].type==2)
												{
													if (cx>0) while(mass[road[b].ex][road[b].ey].type==2) cx--,road[b].ex--;
													if (cx<0) while(mass[road[b].ex][road[b].ey].type==2) cx++,road[b].ex++;
													if (cx>0) while(mass[road[b].ex][road[b].ey].type==2) cy--,road[b].ey--;
													if (cy<0) while(mass[road[b].ex][road[b].ey].type==2) cy++,road[b].ey++;
												}
												if ((mass[road[b].ex][road[b].ey].type!=2)&&(mass[road[b].ex][road[b].ey].type!=4))
												{
													if (cy>0)
													{
														for (int i=0;i<cy+1;i++)
														{
															if (mass[road[b].k1x][road[b].k1y+i].type==4) d++;
															if (mass[road[b].k1x][road[b].k1y+i].type==1) e++;
															if ((mass[road[b].k1x][road[b].k1y+i].type==2)||((mass[road[b].k1x][road[b].k1y+i].type==5)&&(i!=cy))) error=true;
														}
														if (d>=3+((mass[road[b].ex][road[b].ey].type==1)||(mass[road[b].ex][road[b].ey+1].type==2))) error=true;
														if (e>=2)error=true;
														if (error==false)
														{
															for (int i=1;i<cy;i++)
															{
																if (mass[road[b].k1x+1][road[b].k1y+i].type==1)
																{
																	if (mass[road[b].k1x+1][road[b].k1y+i+1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
																if (mass[road[b].k1x-1][road[b].k1y+i].type==1)
																{
																	if (mass[road[b].k1x-1][road[b].k1y+i+1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
															}
															if (error==false)
															{
																if ((mass[road[b].ex][road[b].ey].type!=1)&&(mass[road[b].ex][road[b].ey+1].type!=2))
																{
																	int count=0;
																	while(1)
																	{
																		error=false;
																		count++;
																		int fx=GetRand(14)+1,fy=GetRand(14)+1;
																		int fw=GetRand(3)+3,fl=GetRand(3)+3;
																		if ((fx+fw<ROOMX-1)&&(fy+fl<ROOMY-1))
																		{
																			for (int i=fx;i<fx+fw;i++)
																			{	
																				for (int j=fy;j<fy+fl;j++)
																				{
																					if (mass[i][j].type!=0)
																					{
																						error=true;
																						break;
																					}
																				}
																				if (error==true)break;
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																				{
																					road[b].sr=i;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if (room[i].x==-1)
																				{
																					road[b].er=i;
																					break;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[i].sr!=-1)&&(b!=i))
																				{
																					if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																				}
																			}
																			if ((error==false)&&(fx<=road[b].ex)&&(fx+fw>road[b].ex)&&(road[b].ey+1==fy))
																			{
																				for (int i=fx;i<fx+fw;i++)
																				{	
																					for (int j=fy;j<fy+fl;j++)
																					{
																						mass[i][j].type=2;
																					}
																					if (mass[i][fy-1].type==0)
																					{
																						mass[i][fy-1].type=5;
																					}
																					if (mass[i][fy+fl].type==0)
																					{
																						mass[i][fy+fl].type=5;
																					}
																				}
																				for (int i=fy;i<fy+fl;i++)
																				{	
																					if (mass[fx-1][i].type==0)
																					{
																						mass[fx-1][i].type=5;
																					}
																					if (mass[fx+fw][i].type==0)
																					{
																						mass[fx+fw][i].type=5;
																					}
																				}
																				for (int i=0;i<20;i++)
																				{
																					if (room[i].x==-1)
																					{
																						room[i].x=fx;
																						room[i].y=fy;
																						room[i].width=fw;
																						room[i].length=fl;
																						break;
																					}
																				}
																				break;
																			}
																		}
																		if (count>10000)
																		{
																			error=true;
																			break;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].sx-road[b].k1x+1;i++)
																		{
																			mass[road[b].sx-i][road[b].sy].type=1;
																			if ((mass[road[b].sx-i][road[b].sy-1].type!=1)&&(mass[road[b].sx-i][road[b].sy-1].type!=2)&&(mass[road[b].sx-i][road[b].sy-1].type!=5))
																			{
																				mass[road[b].sx-i][road[b].sy-1].type=4;
																			}
																			if ((mass[road[b].sx-i][road[b].sy+1].type!=1)&&(mass[road[b].sx-i][road[b].sy+1].type!=2)&&(mass[road[b].sx-i][road[b].sy+1].type!=5))
																			{
																				mass[road[b].sx-i][road[b].sy+1].type=4;
																			}
																		}
																		for (int i=0;i<road[b].ey-road[b].k1y+1;i++)
																		{
																			mass[road[b].k1x][road[b].k1y+i].type=1;
																			if ((mass[road[b].k1x-1][road[b].k1y+i].type!=1)&&(mass[road[b].k1x-1][road[b].k1y+i].type!=2)&&(mass[road[b].k1x-1][road[b].k1y+i].type!=5))
																			{
																				mass[road[b].k1x-1][road[b].k1y+i].type=4;
																			}
																			if ((mass[road[b].k1x+1][road[b].k1y+i].type!=1)&&(mass[road[b].k1x+1][road[b].k1y+i].type!=2)&&(mass[road[b].k1x+1][road[b].k1y+i].type!=5))
																			{
																				mass[road[b].k1x+1][road[b].k1y+i].type=4;
																			}
																		}
																		break;
																	}
																}
																else 
																{
																	for (int i=0;i<20;i++)
																	{
																		if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																		{
																			road[b].sr=i;
																		}
																		if ((road[b].ex<=room[i].x+room[i].width)&&(road[b].ex>=room[i].x-1)&&(road[b].ey<=room[i].y+room[i].length)&&(road[b].ey>=room[i].y-1))
																		{
																			road[b].er=i;
																		}
																	}
																	for (int i=0;i<20;i++)
																	{
																		if ((road[i].sr!=-1)&&(b!=i)&&(road[b].er!=-1))
																		{
																			if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].sx-road[b].k1x+1;i++)
																		{
																			mass[road[b].sx-i][road[b].sy].type=1;
																			if ((mass[road[b].sx-i][road[b].sy-1].type!=1)&&(mass[road[b].sx-i][road[b].sy-1].type!=2)&&(mass[road[b].sx-i][road[b].sy-1].type!=5))
																			{
																				mass[road[b].sx-i][road[b].sy-1].type=4;
																			}
																			if ((mass[road[b].sx-i][road[b].sy+1].type!=1)&&(mass[road[b].sx-i][road[b].sy+1].type!=2)&&(mass[road[b].sx-i][road[b].sy+1].type!=5))
																			{
																				mass[road[b].sx-i][road[b].sy+1].type=4;
																			}
																		}
																		for (int i=0;i<road[b].ey-road[b].k1y+1;i++)
																		{
																			mass[road[b].k1x][road[b].k1y+i].type=1;
																			if ((mass[road[b].k1x-1][road[b].k1y+i].type!=1)&&(mass[road[b].k1x-1][road[b].k1y+i].type!=2)&&(mass[road[b].k1x-1][road[b].k1y+i].type!=5))
																			{
																				mass[road[b].k1x-1][road[b].k1y+i].type=4;
																			}
																			if ((mass[road[b].k1x+1][road[b].k1y+i].type!=1)&&(mass[road[b].k1x+1][road[b].k1y+i].type!=2)&&(mass[road[b].k1x+1][road[b].k1y+i].type!=5))
																			{
																				mass[road[b].k1x+1][road[b].k1y+i].type=4;
																			}
																		}
																		break;
																	}
																}	
															}
														}
													}
													else if (cy<0)
													{
														for (int i=0;i<-cy+1;i++)
														{
															if (mass[road[b].k1x][road[b].k1y-i].type==4)d++;
															if (mass[road[b].k1x][road[b].k1y-i].type==1) e++;
															if ((mass[road[b].k1x][road[b].k1y-i].type==2)||((mass[road[b].k1x][road[b].k1y-i].type==5)&&(i!=-cy))) error=true;
														}
														if (d>=3+((mass[road[b].ex][road[b].ey].type==1)||(mass[road[b].ex][road[b].ey-1].type==2))) error=true;
														if (e>=2)error=true;
														if (error==false)
														{
															for (int i=1;i<-cy;i++)
															{
																if (mass[road[b].k1x+1][road[b].k1y-i].type==1)
																{
																	if (mass[road[b].k1x+1][road[b].k1y-i-1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
																if (mass[road[b].k1x-1][road[b].k1y-i].type==1)
																{
																	if (mass[road[b].k1x-1][road[b].k1y-i-1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
															}
															if (error==false)
															{
																if ((mass[road[b].ex][road[b].ey].type!=1)&&(mass[road[b].ex][road[b].ey-1].type!=2))
																{
																	int count=0;
																	while(1)
																	{
																		error=false;
																		count++;
																		int fx=GetRand(14)+1,fy=GetRand(14)+1;
																		int fw=GetRand(3)+3,fl=GetRand(3)+3;
																		if ((fx+fw<ROOMX-1)&&(fy+fl<ROOMY-1))
																		{
																			for (int i=fx;i<fx+fw;i++)
																			{	
																				for (int j=fy;j<fy+fl;j++)
																				{
																					if (mass[i][j].type!=0)
																					{
																						error=true;
																						break;
																					}
																				}
																				if (error==true)break;
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																				{
																					road[b].sr=i;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if (room[i].x==-1)
																				{
																					road[b].er=i;
																					break;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[i].sr!=-1)&&(b!=i))
																				{
																					if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																				}
																			}
																			if ((error==false)&&(fx<=road[b].ex)&&(fx+fw>road[b].ex)&&(road[b].ey==fy+fl))
																			{
																				for (int i=fx;i<fx+fw;i++)
																				{	
																					for (int j=fy;j<fy+fl;j++)
																					{
																						mass[i][j].type=2;
																					}
																					if (mass[i][fy-1].type==0)
																					{
																						mass[i][fy-1].type=5;
																					}
																					if (mass[i][fy+fl].type==0)
																					{
																						mass[i][fy+fl].type=5;
																					}
																				}
																				for (int i=fy;i<fy+fl;i++)
																				{	
																					if (mass[fx-1][i].type==0)
																					{
																						mass[fx-1][i].type=5;
																					}
																					if (mass[fx+fw][i].type==0)
																					{
																						mass[fx+fw][i].type=5;
																					}
																				}
																				for (int i=0;i<20;i++)
																				{
																					if (room[i].x==-1)
																					{
																						room[i].x=fx;
																						room[i].y=fy;
																						room[i].width=fw;
																						room[i].length=fl;
																						break;
																					}
																				}
																				break;
																			}
																		}
																		if (count>10000)
																		{
																			error=true;
																			break;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].sx-road[b].k1x+1;i++)
																		{
																			mass[road[b].sx-i][road[b].sy].type=1;
																			if ((mass[road[b].sx-i][road[b].sy-1].type!=1)&&(mass[road[b].sx-i][road[b].sy-1].type!=2)&&(mass[road[b].sx-i][road[b].sy-1].type!=5))
																			{
																				mass[road[b].sx-i][road[b].sy-1].type=4;
																			}
																			if ((mass[road[b].sx-i][road[b].sy+1].type!=1)&&(mass[road[b].sx-i][road[b].sy+1].type!=2)&&(mass[road[b].sx-i][road[b].sy+1].type!=5))
																			{
																				mass[road[b].sx-i][road[b].sy+1].type=4;
																			}
																		}
																		for (int i=0;i<road[b].k1y-road[b].ey+1;i++)
																		{
																			mass[road[b].k1x][road[b].k1y-i].type=1;
																			if ((mass[road[b].k1x-1][road[b].k1y-i].type!=1)&&(mass[road[b].k1x-1][road[b].k1y-i].type!=2)&&(mass[road[b].k1x-1][road[b].k1y-i].type!=5))
																			{
																				mass[road[b].k1x-1][road[b].k1y-i].type=4;
																			}
																			if ((mass[road[b].k1x+1][road[b].k1y-i].type!=1)&&(mass[road[b].k1x+1][road[b].k1y-i].type!=2)&&(mass[road[b].k1x+1][road[b].k1y-i].type!=5))
																			{
																				mass[road[b].k1x+1][road[b].k1y-i].type=4;
																			}
																		}
																		break;
																	}
																}
																else 
																{
																	for (int i=0;i<20;i++)
																	{
																		if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																		{
																			road[b].sr=i;
																		}
																		if ((road[b].ex<=room[i].x+room[i].width)&&(road[b].ex>=room[i].x-1)&&(road[b].ey<=room[i].y+room[i].length)&&(road[b].ey>=room[i].y-1))
																		{
																			road[b].er=i;
																		}
																	}
																	for (int i=0;i<20;i++)
																	{
																		if ((road[i].sr!=-1)&&(b!=i)&&(road[b].er!=-1))
																		{
																			if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].sx-road[b].k1x+1;i++)
																		{
																			mass[road[b].sx-i][road[b].sy].type=1;
																			if ((mass[road[b].sx-i][road[b].sy-1].type!=1)&&(mass[road[b].sx-i][road[b].sy-1].type!=2)&&(mass[road[b].sx-i][road[b].sy-1].type!=5))
																			{
																				mass[road[b].sx-i][road[b].sy-1].type=4;
																			}
																			if ((mass[road[b].sx-i][road[b].sy+1].type!=1)&&(mass[road[b].sx-i][road[b].sy+1].type!=2)&&(mass[road[b].sx-i][road[b].sy+1].type!=5))
																			{
																				mass[road[b].sx-i][road[b].sy+1].type=4;
																			}
																		}
																		for (int i=0;i<road[b].k1y-road[b].ey+1;i++)
																		{
																			mass[road[b].k1x][road[b].k1y-i].type=1;
																			if ((mass[road[b].k1x-1][road[b].k1y-i].type!=1)&&(mass[road[b].k1x-1][road[b].k1y-i].type!=2)&&(mass[road[b].k1x-1][road[b].k1y-i].type!=5))
																			{
																				mass[road[b].k1x-1][road[b].k1y-i].type=4;
																			}
																			if ((mass[road[b].k1x+1][road[b].k1y-i].type!=1)&&(mass[road[b].k1x+1][road[b].k1y-i].type!=2)&&(mass[road[b].k1x+1][road[b].k1y-i].type!=5))
																			{
																				mass[road[b].k1x+1][road[b].k1y-i].type=4;
																			}
																		}
																		break;
																	}
																}	
															}
														}
													}
												}
											}
											else//２回曲がる時
											{
											}
										}
									}
								}
								else if (cy>0)
								{
									for (int i=0;i<cy+1;i++)
									{
										if ((mass[road[b].sx][road[b].sy+i].type==4)||(mass[road[b].sx][road[b].sy+i].type==5))d++;
										if (mass[road[b].sx][road[b].sy+i].type==1)e++;
										if (mass[road[b].sx][road[b].sy+i].type==2)error=true;
									}
									if (d>=4)error=true;
									if (e>=2)error=true;
									if (error==false)
									{
										for (int i=1;i<cy;i++)
										{
											if (mass[road[b].sx+1][road[b].sy+i].type==1)
											{
												if (mass[road[b].sx+1][road[b].sy+i+1].type==1)
												{
													error=true;
													break;
												}
											}
											if (mass[road[b].sx-1][road[b].sy+i].type==1)
											{
												if (mass[road[b].sx-1][road[b].sy+i+1].type==1)
												{
													error=true;
													break;
												}
											}
										}
										int m=GetRand(1);
										if (cx>0){
											switch(m){
											case 0:cy=1;break;
											case 1:cy=-1;break;}
										}
										else if (cx<0){
											switch(m){
											case 0:cy=1;break;
											case 1:cy=-1;break;}
										}
										else if (cy>0){
											switch(m){
											case 0:cx=1;break;
											case 1:cx=-1;break;}
										}
										else if (cy<0){
											switch(m){
											case 0:cx=1;break;
											case 1:cx=-1;break;}
										}
										cy=0;
										cx*=GetRand(7)+3;
										cy*=GetRand(7)+3;
										d=0;
										e=0;
										if ((road[b].k1x+cx<ROOMX)&&(road[b].k1x+cx>0)&&(road[b].k1y+cy<ROOMY)&&(road[b].k1y+cy>0))
										{
											if (GetRand(0)==0)//もう曲がらない時
											{
												road[b].ex=road[b].k1x+cx;
												road[b].ey=road[b].k1y+cy;
												if (mass[road[b].ex][road[b].ey].type==2)
												{
													if (cx>0) while(mass[road[b].ex][road[b].ey].type==2) cx--,road[b].ex--;
													if (cx<0) while(mass[road[b].ex][road[b].ey].type==2) cx++,road[b].ex++;
													if (cx>0) while(mass[road[b].ex][road[b].ey].type==2) cy--,road[b].ey--;
													if (cy<0) while(mass[road[b].ex][road[b].ey].type==2) cy++,road[b].ey++;
												}
												if ((mass[road[b].ex][road[b].ey].type!=2)&&(mass[road[b].ex][road[b].ey].type!=4))
												{
													if (cx>0)
													{
														for (int i=0;i<cx+1;i++)
														{
															if (mass[road[b].k1x+i][road[b].k1y].type==4) d++;
															if (mass[road[b].k1x+i][road[b].k1y].type==1) e++;
															if ((mass[road[b].k1x+i][road[b].k1y].type==2)||((mass[road[b].k1x+i][road[b].k1y].type==5)&&(i!=cx))) error=true;
														}
														if (d>=3+((mass[road[b].ex][road[b].ey].type==1)||(mass[road[b].ex+1][road[b].ey].type==2))) error=true;
														if (e>=2)error=true;
														if (error==false)
														{
															for (int i=1;i<cx;i++)
															{
																if (mass[road[b].k1x+i][road[b].k1y+1].type==1)
																{
																	if (mass[road[b].k1x+i+1][road[b].k1y+1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
																if (mass[road[b].k1x+i][road[b].k1y-1].type==1)
																{
																	if (mass[road[b].k1x+i+1][road[b].k1y-1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
															}
															if (error==false)
															{
																if ((mass[road[b].ex][road[b].ey].type!=1)&&(mass[road[b].ex+1][road[b].ey].type!=2))
																{
																	int count=0;
																	while(1)
																	{
																		error=false;
																		count++;
																		int fx=GetRand(14)+1,fy=GetRand(14)+1;
																		int fw=GetRand(3)+3,fl=GetRand(3)+3;
																		if ((fx+fw<ROOMX-1)&&(fy+fl<ROOMY-1))
																		{
																			for (int i=fx;i<fx+fw;i++)
																			{	
																				for (int j=fy;j<fy+fl;j++)
																				{
																					if (mass[i][j].type!=0)
																					{
																						error=true;
																						break;
																					}
																				}
																				if (error==true)break;
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																				{
																					road[b].sr=i;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if (room[i].x==-1)
																				{
																					road[b].er=i;
																					break;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[i].sr!=-1)&&(b!=i))
																				{
																					if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																				}
																			}
																			if ((error==false)&&(fy<=road[b].ey)&&(fy+fl>road[b].ey)&&(road[b].ex+1==fx))
																			{
																				for (int i=fx;i<fx+fw;i++)
																				{	
																					for (int j=fy;j<fy+fl;j++)
																					{
																						mass[i][j].type=2;
																					}
																					if (mass[i][fy-1].type==0)
																					{
																						mass[i][fy-1].type=5;
																					}
																					if (mass[i][fy+fl].type==0)
																					{
																						mass[i][fy+fl].type=5;
																					}
																				}
																				for (int i=fy;i<fy+fl;i++)
																				{	
																					if (mass[fx-1][i].type==0)
																					{
																						mass[fx-1][i].type=5;
																					}
																					if (mass[fx+fw][i].type==0)
																					{
																						mass[fx+fw][i].type=5;
																					}
																				}
																				for (int i=0;i<20;i++)
																				{
																					if (room[i].x==-1)
																					{
																						room[i].x=fx;
																						room[i].y=fy;
																						room[i].width=fw;
																						room[i].length=fl;
																						break;
																					}
																				}
																				break;
																			}
																		}
																		if (count>10000)
																		{
																			error=true;
																			break;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].k1y-road[b].sy+1;i++)
																		{
																			mass[road[b].sx][road[b].sy+i].type=1;
																			if ((mass[road[b].sx-1][road[b].sy+i].type!=1)&&(mass[road[b].sx-1][road[b].sy+i].type!=2)&&(mass[road[b].sx-1][road[b].sy+i].type!=5))
																			{
																				mass[road[b].sx-1][road[b].sy+i].type=4;
																			}
																			if ((mass[road[b].sx+1][road[b].sy+i].type!=1)&&(mass[road[b].sx+1][road[b].sy+i].type!=2)&&(mass[road[b].sx+1][road[b].sy+i].type!=5))
																			{
																				mass[road[b].sx+1][road[b].sy+i].type=4;
																			}
																		}
																		for (int i=0;i<road[b].ex-road[b].k1x+1;i++)
																		{
																			mass[road[b].k1x+i][road[b].k1y].type=1;
																			if ((mass[road[b].k1x+i][road[b].k1y-1].type!=1)&&(mass[road[b].k1x+i][road[b].k1y-1].type!=2)&&(mass[road[b].k1x+i][road[b].k1y-1].type!=5))
																			{
																				mass[road[b].k1x+i][road[b].k1y-1].type=4;
																			}
																			if ((mass[road[b].k1x+i][road[b].k1y+1].type!=1)&&(mass[road[b].k1x+i][road[b].k1y+1].type!=2)&&(mass[road[b].k1x+i][road[b].k1y+1].type!=5))
																			{
																				mass[road[b].k1x+i][road[b].k1y+1].type=4;
																			}
																		}
																		break;
																	}
																}
																else 
																{
																	for (int i=0;i<20;i++)
																	{
																		if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																		{
																			road[b].sr=i;
																		}
																		if ((road[b].ex<=room[i].x+room[i].width)&&(road[b].ex>=room[i].x-1)&&(road[b].ey<=room[i].y+room[i].length)&&(road[b].ey>=room[i].y-1))
																		{
																			road[b].er=i;
																		}
																	}
																	for (int i=0;i<20;i++)
																	{
																		if ((road[i].sr!=-1)&&(b!=i)&&(road[b].er!=-1))
																		{
																			if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].k1y-road[b].sy+1;i++)
																		{
																			mass[road[b].sx][road[b].sy+i].type=1;
																			if ((mass[road[b].sx-1][road[b].sy+i].type!=1)&&(mass[road[b].sx-1][road[b].sy+i].type!=2)&&(mass[road[b].sx-1][road[b].sy+i].type!=5))
																			{
																				mass[road[b].sx-1][road[b].sy+i].type=4;
																			}
																			if ((mass[road[b].sx+1][road[b].sy+i].type!=1)&&(mass[road[b].sx+1][road[b].sy+i].type!=2)&&(mass[road[b].sx+1][road[b].sy+i].type!=5))
																			{
																				mass[road[b].sx+1][road[b].sy+i].type=4;
																			}
																		}
																		for (int i=0;i<road[b].ex-road[b].k1x+1;i++)
																		{
																			mass[road[b].k1x+i][road[b].k1y].type=1;
																			if ((mass[road[b].k1x+i][road[b].k1y-1].type!=1)&&(mass[road[b].k1x+i][road[b].k1y-1].type!=2)&&(mass[road[b].k1x+i][road[b].k1y-1].type!=5))
																			{
																				mass[road[b].k1x+i][road[b].k1y-1].type=4;
																			}
																			if ((mass[road[b].k1x+i][road[b].k1y+1].type!=1)&&(mass[road[b].k1x+i][road[b].k1y+1].type!=2)&&(mass[road[b].k1x+i][road[b].k1y+1].type!=5))
																			{
																				mass[road[b].k1x+i][road[b].k1y+1].type=4;
																			}
																		}
																		break;
																	}
																}	
															}
														}
													}
													else if (cx<0)
													{
														for (int i=0;i<-cx+1;i++)
														{
															if (mass[road[b].k1x-i][road[b].k1y].type==4) d++;
															if (mass[road[b].k1x-i][road[b].k1y].type==1) e++;
															if ((mass[road[b].k1x-i][road[b].k1y].type==2)||((mass[road[b].k1x-i][road[b].k1y].type==5)&&(i!=-cx))) error=true;
														}
														if (d>=3+((mass[road[b].ex][road[b].ey].type==1)||(mass[road[b].ex-1][road[b].ey].type==2))) error=true;
														if (e>=2)error=true;
														if (error==false)
														{
															for (int i=1;i<-cx;i++)
															{
																if (mass[road[b].k1x-i][road[b].k1y+1].type==1)
																{
																	if (mass[road[b].k1x-i-1][road[b].k1y+1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
																if (mass[road[b].k1x-i][road[b].k1y-1].type==1)
																{
																	if (mass[road[b].k1x-i-1][road[b].k1y-1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
															}
															if (error==false)
															{
																if ((mass[road[b].ex][road[b].ey].type!=1)&&(mass[road[b].ex-1][road[b].ey].type!=2))
																{
																	int count=0;
																	while(1)
																	{
																		error=false;
																		count++;
																		int fx=GetRand(14)+1,fy=GetRand(14)+1;
																		int fw=GetRand(3)+3,fl=GetRand(3)+3;
																		if ((fx+fw<ROOMX-1)&&(fy+fl<ROOMY-1))
																		{
																			for (int i=fx;i<fx+fw;i++)
																			{	
																				for (int j=fy;j<fy+fl;j++)
																				{
																					if (mass[i][j].type!=0)
																					{
																						error=true;
																						break;
																					}
																				}
																				if (error==true)break;
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																				{
																					road[b].sr=i;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if (room[i].x==-1)
																				{
																					road[b].er=i;
																					break;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[i].sr!=-1)&&(b!=i))
																				{
																					if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																				}
																			}
																			if ((error==false)&&(fy<=road[b].ey)&&(fy+fl>road[b].ey)&&(road[b].ex==fx+fw))
																			{
																				for (int i=fx;i<fx+fw;i++)
																				{	
																					for (int j=fy;j<fy+fl;j++)
																					{
																						mass[i][j].type=2;
																					}
																					if (mass[i][fy-1].type==0)
																					{
																						mass[i][fy-1].type=5;
																					}
																					if (mass[i][fy+fl].type==0)
																					{
																						mass[i][fy+fl].type=5;
																					}
																				}
																				for (int i=fy;i<fy+fl;i++)
																				{	
																					if (mass[fx-1][i].type==0)
																					{
																						mass[fx-1][i].type=5;
																					}
																					if (mass[fx+fw][i].type==0)
																					{
																						mass[fx+fw][i].type=5;
																					}
																				}
																				for (int i=0;i<20;i++)
																				{
																					if (room[i].x==-1)
																					{
																						room[i].x=fx;
																						room[i].y=fy;
																						room[i].width=fw;
																						room[i].length=fl;
																						break;
																					}
																				}
																				break;
																			}
																		}
																		if (count>10000)
																		{
																			error=true;
																			break;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].k1y-road[b].sy+1;i++)
																		{
																			mass[road[b].sx][road[b].sy+i].type=1;
																			if ((mass[road[b].sx-1][road[b].sy+i].type!=1)&&(mass[road[b].sx-1][road[b].sy+i].type!=2)&&(mass[road[b].sx-1][road[b].sy+i].type!=5))
																			{
																				mass[road[b].sx-1][road[b].sy+i].type=4;
																			}
																			if ((mass[road[b].sx+1][road[b].sy+i].type!=1)&&(mass[road[b].sx+1][road[b].sy+i].type!=2)&&(mass[road[b].sx+1][road[b].sy+i].type!=5))
																			{
																				mass[road[b].sx+1][road[b].sy+i].type=4;
																			}
																		}
																		for (int i=0;i<road[b].k1x-road[b].ex+1;i++)
																		{
																			mass[road[b].k1x-i][road[b].k1y].type=1;
																			if ((mass[road[b].k1x-i][road[b].k1y-1].type!=1)&&(mass[road[b].k1x-i][road[b].k1y-1].type!=2)&&(mass[road[b].k1x-i][road[b].k1y-1].type!=5))
																			{
																				mass[road[b].k1x-i][road[b].k1y-1].type=4;
																			}
																			if ((mass[road[b].k1x-i][road[b].k1y+1].type!=1)&&(mass[road[b].k1x-i][road[b].k1y+1].type!=2)&&(mass[road[b].k1x-i][road[b].k1y+1].type!=5))
																			{
																				mass[road[b].k1x-i][road[b].k1y+1].type=4;
																			}
																		}
																		break;
																	}
																}
																else 
																{
																	for (int i=0;i<20;i++)
																	{
																		if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																		{
																			road[b].sr=i;
																		}
																		if ((road[b].ex<=room[i].x+room[i].width)&&(road[b].ex>=room[i].x-1)&&(road[b].ey<=room[i].y+room[i].length)&&(road[b].ey>=room[i].y-1))
																		{
																			road[b].er=i;
																		}
																	}
																	for (int i=0;i<20;i++)
																	{
																		if ((road[i].sr!=-1)&&(b!=i)&&(road[b].er!=-1))
																		{
																			if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].k1y-road[b].sy+1;i++)
																		{
																			mass[road[b].sx][road[b].sy+i].type=1;
																			if ((mass[road[b].sx-1][road[b].sy+i].type!=1)&&(mass[road[b].sx-1][road[b].sy+i].type!=2)&&(mass[road[b].sx+i][road[b].sy-1].type!=5))
																			{
																				mass[road[b].sx-1][road[b].sy+i].type=4;
																			}
																			if ((mass[road[b].sx+1][road[b].sy+i].type!=1)&&(mass[road[b].sx+1][road[b].sy+i].type!=2)&&(mass[road[b].sx+i][road[b].sy+1].type!=5))
																			{
																				mass[road[b].sx+1][road[b].sy+i].type=4;
																			}
																		}
																		for (int i=0;i<road[b].k1x-road[b].ex+1;i++)
																		{
																			mass[road[b].k1x-i][road[b].k1y].type=1;
																			if ((mass[road[b].k1x-i][road[b].k1y-1].type!=1)&&(mass[road[b].k1x-i][road[b].k1y-1].type!=2)&&(mass[road[b].k1x-i][road[b].k1y-1].type!=5))
																			{
																				mass[road[b].k1x-i][road[b].k1y-1].type=4;
																			}
																			if ((mass[road[b].k1x-i][road[b].k1y+1].type!=1)&&(mass[road[b].k1x-i][road[b].k1y+1].type!=2)&&(mass[road[b].k1x-i][road[b].k1y+1].type!=5))
																			{
																				mass[road[b].k1x-i][road[b].k1y+1].type=4;
																			}
																		}
																		break;
																	}
																}	
															}
														}
													}
												}
											}
											else//２回曲がる時
											{
											}
										}
									}
								}
								else if (cy<0)
								{
									for (int i=0;i<-cy+1;i++)
									{
										if ((mass[road[b].sx][road[b].sy-i].type==4)||(mass[road[b].sx][road[b].sy-i].type==5))d++;
										if (mass[road[b].sx][road[b].sy-i].type==1)e++;
										if (mass[road[b].sx][road[b].sy-i].type==2)error=true;
									}
									if (d>=4)error=true;
									if (e>=2)error=true;
									if (error==false)
									{
										for (int i=1;i<-cy;i++)
										{
											if (mass[road[b].sx+1][road[b].sy-i].type==1)
											{
												if (mass[road[b].sx+1][road[b].sy-i-1].type==1)
												{
													error=true;
													break;
												}
											}
											if (mass[road[b].sx-1][road[b].sy-i].type==1)
											{
												if (mass[road[b].sx-1][road[b].sy-i-1].type==1)
												{
													error=true;
													break;
												}
											}
										}
										int m=GetRand(1);
										if (cx>0){
											switch(m){
											case 0:cy=1;break;
											case 1:cy=-1;break;}
										}
										else if (cx<0){
											switch(m){
											case 0:cy=1;break;
											case 1:cy=-1;break;}
										}
										else if (cy>0){
											switch(m){
											case 0:cx=1;break;
											case 1:cx=-1;break;}
										}
										else if (cy<0){
											switch(m){
											case 0:cx=1;break;
											case 1:cx=-1;break;}
										}
										cy=0;
										cx*=GetRand(7)+3;
										cy*=GetRand(7)+3;
										d=0;
										e=0;
										if ((road[b].k1x+cx<ROOMX)&&(road[b].k1x+cx>0)&&(road[b].k1y+cy<ROOMY)&&(road[b].k1y+cy>0))
										{
											if (GetRand(0)==0)//もう曲がらない時
											{
												road[b].ex=road[b].k1x+cx;
												road[b].ey=road[b].k1y+cy;
												if (mass[road[b].ex][road[b].ey].type==2)
												{
													if (cx>0) while(mass[road[b].ex][road[b].ey].type==2) cx--,road[b].ex--;
													if (cx<0) while(mass[road[b].ex][road[b].ey].type==2) cx++,road[b].ex++;
													if (cx>0) while(mass[road[b].ex][road[b].ey].type==2) cy--,road[b].ey--;
													if (cy<0) while(mass[road[b].ex][road[b].ey].type==2) cy++,road[b].ey++;
												}
												if ((mass[road[b].ex][road[b].ey].type!=2)&&(mass[road[b].ex][road[b].ey].type!=4))
												{
													if (cx>0)
													{
														for (int i=0;i<cx+1;i++)
														{
															if (mass[road[b].k1x+i][road[b].k1y].type==4) d++;
															if (mass[road[b].k1x+i][road[b].k1y].type==1) e++;
															if ((mass[road[b].k1x+i][road[b].k1y].type==2)||((mass[road[b].k1x+i][road[b].k1y].type==5)&&(i!=cx))) error=true;
														}
														if (d>=3+((mass[road[b].ex][road[b].ey].type==1)||(mass[road[b].ex+1][road[b].ey].type==2))) error=true;
														if (e>=2)error=true;
														if (error==false)
														{
															for (int i=1;i<cx;i++)
															{
																if (mass[road[b].k1x+i][road[b].k1y+1].type==1)
																{
																	if (mass[road[b].k1x+i+1][road[b].k1y+1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
																if (mass[road[b].k1x+i][road[b].k1y-1].type==1)
																{
																	if (mass[road[b].k1x+i+1][road[b].k1y-1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
															}
															if (error==false)
															{
																if ((mass[road[b].ex][road[b].ey].type!=1)&&(mass[road[b].ex+1][road[b].ey].type!=2))
																{
																	int count=0;
																	while(1)
																	{
																		error=false;
																		count++;
																		int fx=GetRand(14)+1,fy=GetRand(14)+1;
																		int fw=GetRand(3)+3,fl=GetRand(3)+3;
																		if ((fx+fw<ROOMX-1)&&(fy+fl<ROOMY-1))
																		{
																			for (int i=fx;i<fx+fw;i++)
																			{	
																				for (int j=fy;j<fy+fl;j++)
																				{
																					if (mass[i][j].type!=0)
																					{
																						error=true;
																						break;
																					}
																				}
																				if (error==true)break;
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																				{
																					road[b].sr=i;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if (room[i].x==-1)
																				{
																					road[b].er=i;
																					break;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[i].sr!=-1)&&(b!=i))
																				{
																					if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																				}
																			}
																			if ((error==false)&&(fy<=road[b].ey)&&(fy+fl>road[b].ey)&&(road[b].ex+1==fx))
																			{
																				for (int i=fx;i<fx+fw;i++)
																				{	
																					for (int j=fy;j<fy+fl;j++)
																					{
																						mass[i][j].type=2;
																					}
																					if (mass[i][fy-1].type==0)
																					{
																						mass[i][fy-1].type=5;
																					}
																					if (mass[i][fy+fl].type==0)
																					{
																						mass[i][fy+fl].type=5;
																					}
																				}
																				for (int i=fy;i<fy+fl;i++)
																				{	
																					if (mass[fx-1][i].type==0)
																					{
																						mass[fx-1][i].type=5;
																					}
																					if (mass[fx+fw][i].type==0)
																					{
																						mass[fx+fw][i].type=5;
																					}
																				}
																				for (int i=0;i<20;i++)
																				{
																					if (room[i].x==-1)
																					{
																						room[i].x=fx;
																						room[i].y=fy;
																						room[i].width=fw;
																						room[i].length=fl;
																						break;
																					}
																				}
																				break;
																			}
																		}
																		if (count>10000)
																		{
																			error=true;
																			break;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].sy-road[b].k1y+1;i++)
																		{
																			mass[road[b].sx][road[b].sy-i].type=1;
																			if ((mass[road[b].sx-1][road[b].sy-i].type!=1)&&(mass[road[b].sx-1][road[b].sy-i].type!=2)&&(mass[road[b].sx-1][road[b].sy-i].type!=5))
																			{
																				mass[road[b].sx-1][road[b].sy-i].type=4;
																			}
																			if ((mass[road[b].sx+1][road[b].sy-i].type!=1)&&(mass[road[b].sx+1][road[b].sy-i].type!=2)&&(mass[road[b].sx+1][road[b].sy-i].type!=5))
																			{
																				mass[road[b].sx+1][road[b].sy-i].type=4;
																			}
																		}
																		for (int i=0;i<road[b].ex-road[b].k1x+1;i++)
																		{
																			mass[road[b].k1x+i][road[b].k1y].type=1;
																			if ((mass[road[b].k1x+i][road[b].k1y-1].type!=1)&&(mass[road[b].k1x+i][road[b].k1y-1].type!=2)&&(mass[road[b].k1x+i][road[b].k1y-1].type!=5))
																			{
																				mass[road[b].k1x+i][road[b].k1y-1].type=4;
																			}
																			if ((mass[road[b].k1x+i][road[b].k1y+1].type!=1)&&(mass[road[b].k1x+i][road[b].k1y+1].type!=2)&&(mass[road[b].k1x+i][road[b].k1y+1].type!=5))
																			{
																				mass[road[b].k1x+i][road[b].k1y+1].type=4;
																			}
																		}
																		break;
																	}
																}
																else 
																{
																	for (int i=0;i<20;i++)
																	{
																		if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																		{
																			road[b].sr=i;
																		}
																		if ((road[b].ex<=room[i].x+room[i].width)&&(road[b].ex>=room[i].x-1)&&(road[b].ey<=room[i].y+room[i].length)&&(road[b].ey>=room[i].y-1))
																		{
																			road[b].er=i;
																		}
																	}
																	for (int i=0;i<20;i++)
																	{
																		if ((road[i].sr!=-1)&&(b!=i)&&(road[b].er!=-1))
																		{
																			if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].sy-road[b].k1y+1;i++)
																		{
																			mass[road[b].sx][road[b].sy-i].type=1;
																			if ((mass[road[b].sx-1][road[b].sy-i].type!=1)&&(mass[road[b].sx-1][road[b].sy-i].type!=2)&&(mass[road[b].sx-1][road[b].sy-i].type!=5))
																			{
																				mass[road[b].sx-1][road[b].sy-i].type=4;
																			}
																			if ((mass[road[b].sx+1][road[b].sy-i].type!=1)&&(mass[road[b].sx+1][road[b].sy-i].type!=2)&&(mass[road[b].sx+1][road[b].sy-i].type!=5))
																			{
																				mass[road[b].sx+1][road[b].sy-i].type=4;
																			}
																		}
																		for (int i=0;i<road[b].ex-road[b].k1x+1;i++)
																		{
																			mass[road[b].k1x+i][road[b].k1y].type=1;
																			if ((mass[road[b].k1x+i][road[b].k1y-1].type!=1)&&(mass[road[b].k1x+i][road[b].k1y-1].type!=2)&&(mass[road[b].k1x+i][road[b].k1y-1].type!=5))
																			{
																				mass[road[b].k1x+i][road[b].k1y-1].type=4;
																			}
																			if ((mass[road[b].k1x+i][road[b].k1y+1].type!=1)&&(mass[road[b].k1x+i][road[b].k1y+1].type!=2)&&(mass[road[b].k1x+i][road[b].k1y+1].type!=5))
																			{
																				mass[road[b].k1x+i][road[b].k1y+1].type=4;
																			}
																		}
																		break;
																	}
																}	
															}
														}
													}
													else if (cx<0)
													{
														for (int i=0;i<-cx+1;i++)
														{
															if (mass[road[b].k1x-i][road[b].k1y].type==4)d++;
															if (mass[road[b].k1x-i][road[b].k1y].type==1) e++;
															if ((mass[road[b].k1x-i][road[b].k1y].type==2)||((mass[road[b].k1x-i][road[b].k1y].type==5)&&(i!=-cx))) error=true;
														}
														if (d>=3+((mass[road[b].ex][road[b].ey].type==1)||(mass[road[b].ex-1][road[b].ey].type==2))) error=true;
														if (e>=2)error=true;
														if (error==false)
														{
															for (int i=1;i<-cx;i++)
															{
																if (mass[road[b].k1x-i][road[b].k1y+1].type==1)
																{
																	if (mass[road[b].k1x-i-1][road[b].k1y+1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
																if (mass[road[b].k1x-i][road[b].k1y-1].type==1)
																{
																	if (mass[road[b].k1x-i-1][road[b].k1y-1].type==1)
																	{
																		error=true;
																		break;
																	}
																}
															}
															if (error==false)
															{
																if ((mass[road[b].ex][road[b].ey].type!=1)&&(mass[road[b].ex-1][road[b].ey].type!=2))
																{
																	int count=0;
																	while(1)
																	{
																		error=false;
																		count++;
																		int fx=GetRand(14)+1,fy=GetRand(14)+1;
																		int fw=GetRand(3)+3,fl=GetRand(3)+3;
																		if ((fx+fw<ROOMX-1)&&(fy+fl<ROOMY-1))
																		{
																			for (int i=fx;i<fx+fw;i++)
																			{	
																				for (int j=fy;j<fy+fl;j++)
																				{
																					if (mass[i][j].type!=0)
																					{
																						error=true;
																						break;
																					}
																				}
																				if (error==true)break;
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																				{
																					road[b].sr=i;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if (room[i].x==-1)
																				{
																					road[b].er=i;
																					break;
																				}
																			}
																			for (int i=0;i<20;i++)
																			{
																				if ((road[i].sr!=-1)&&(b!=i))
																				{
																					if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																				}
																			}
																			if ((error==false)&&(fy<=road[b].ey)&&(fy+fl>road[b].ey)&&(road[b].ex==fx+fw))
																			{
																				for (int i=fx;i<fx+fw;i++)
																				{	
																					for (int j=fy;j<fy+fl;j++)
																					{
																						mass[i][j].type=2;
																					}
																					if (mass[i][fy-1].type==0)
																					{
																						mass[i][fy-1].type=5;
																					}
																					if (mass[i][fy+fl].type==0)
																					{
																						mass[i][fy+fl].type=5;
																					}
																				}
																				for (int i=fy;i<fy+fl;i++)
																				{	
																					if (mass[fx-1][i].type==0)
																					{
																						mass[fx-1][i].type=5;
																					}
																					if (mass[fx+fw][i].type==0)
																					{
																						mass[fx+fw][i].type=5;
																					}
																				}
																				for (int i=0;i<20;i++)
																				{
																					if (room[i].x==-1)
																					{
																						room[i].x=fx;
																						room[i].y=fy;
																						room[i].width=fw;
																						room[i].length=fl;
																						break;
																					}
																				}
																				break;
																			}
																		}
																		if (count>10000)
																		{
																			error=true;
																			break;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].sy-road[b].k1y+1;i++)
																		{
																			mass[road[b].sx][road[b].sy-i].type=1;
																			if ((mass[road[b].sx-1][road[b].sy-i].type!=1)&&(mass[road[b].sx-1][road[b].sy-i].type!=2)&&(mass[road[b].sx-1][road[b].sy-i].type!=5))
																			{
																				mass[road[b].sx-1][road[b].sy-i].type=4;
																			}
																			if ((mass[road[b].sx+1][road[b].sy-i].type!=1)&&(mass[road[b].sx+1][road[b].sy-i].type!=2)&&(mass[road[b].sx+1][road[b].sy-i].type!=5))
																			{
																				mass[road[b].sx+1][road[b].sy-i].type=4;
																			}
																		}
																		for (int i=0;i<road[b].k1x-road[b].ex+1;i++)
																		{
																			mass[road[b].k1x-i][road[b].k1y].type=1;
																			if ((mass[road[b].k1x-i][road[b].k1y-1].type!=1)&&(mass[road[b].k1x-i][road[b].k1y-1].type!=2)&&(mass[road[b].k1x-i][road[b].k1y-1].type!=5))
																			{
																				mass[road[b].k1x-i][road[b].k1y-1].type=4;
																			}
																			if ((mass[road[b].k1x-i][road[b].k1y+1].type!=1)&&(mass[road[b].k1x-i][road[b].k1y+1].type!=2)&&(mass[road[b].k1x-i][road[b].k1y+1].type!=5))
																			{
																				mass[road[b].k1x-i][road[b].k1y+1].type=4;
																			}
																		}
																		break;
																	}
																}
																else 
																{
																	for (int i=0;i<20;i++)
																	{
																		if ((road[b].sx<=room[i].x+room[i].width)&&(road[b].sx>=room[i].x-1)&&(road[b].sy<=room[i].y+room[i].length)&&(road[b].sy>=room[i].y-1))
																		{
																			road[b].sr=i;
																		}
																		if ((road[b].ex<=room[i].x+room[i].width)&&(road[b].ex>=room[i].x-1)&&(road[b].ey<=room[i].y+room[i].length)&&(road[b].ey>=room[i].y-1))
																		{
																			road[b].er=i;
																		}
																	}
																	for (int i=0;i<20;i++)
																	{
																		if ((road[i].sr!=-1)&&(b!=i)&&(road[b].er!=-1))
																		{
																			if ((road[i].sr+road[i].er==road[b].sr+road[b].er)&&(abs(road[i].sr-road[i].er)==abs(road[b].sr-road[b].er)))error=true;
																		}
																	}
																	if (error==false)
																	{
																		for (int i=0;i<road[b].sy-road[b].k1y+1;i++)
																		{
																			mass[road[b].sx][road[b].sy-i].type=1;
																			if ((mass[road[b].sx-1][road[b].sy-i].type!=1)&&(mass[road[b].sx-1][road[b].sy-i].type!=2)&&(mass[road[b].sx-1][road[b].sy-i].type!=5))
																			{
																				mass[road[b].sx-1][road[b].sy-i].type=4;
																			}
																			if ((mass[road[b].sx+1][road[b].sy-i].type!=1)&&(mass[road[b].sx+1][road[b].sy-i].type!=2)&&(mass[road[b].sx+1][road[b].sy-i].type!=5))
																			{
																				mass[road[b].sx+1][road[b].sy-i].type=4;
																			}
																		}
																		for (int i=0;i<road[b].k1x-road[b].ex+1;i++)
																		{
																			mass[road[b].k1x-i][road[b].k1y].type=1;
																			if ((mass[road[b].k1x-i][road[b].k1y-1].type!=1)&&(mass[road[b].k1x-i][road[b].k1y-1].type!=2)&&(mass[road[b].k1x-i][road[b].k1y-1].type!=5))
																			{
																				mass[road[b].k1x-i][road[b].k1y-1].type=4;
																			}
																			if ((mass[road[b].k1x-i][road[b].k1y+1].type!=1)&&(mass[road[b].k1x-i][road[b].k1y+1].type!=2)&&(mass[road[b].k1x-i][road[b].k1y+1].type!=5))
																			{
																				mass[road[b].k1x-i][road[b].k1y+1].type=4;
																			}
																		}
																		break;
																	}
																}	
															}
														}
													}
												}
											}
											else//２回曲がる時
											{
											}
										}
									}
								}
							}
						}
					}
				}
			}
			if (count2>20000)/////////////
			{
				count2=0;
				b=0;
				for (int i=0;i<ROOMX;i++)
				{
					for (int j=0;j<ROOMY;j++)
					{
						mass[i][j].apeear=false;
						mass[i][j].type=0;
						mass[i][j].thing=false;
					}
				}
				for (int i=0;i<20;i++)
				{
					road[i].sx=-1;
					road[i].sy=-1;
					road[i].ex=-1;
					road[i].ey=-1;
					road[i].k1x=-1;
					road[i].k1y=-1;
					road[i].k2x=-1;
					road[i].k2y=-1;
					road[i].sr=-1;
					road[i].er=-1;
					room[i].x=-1;
					room[i].y=-1;
					room[i].width=-1;
					room[i].length=-1;
				}
				int a=GetRand(2)+3;
				while(1)
				{
					room[0].x=GetRand(ROOMX-6)+1;
					room[0].y=GetRand(ROOMY-6)+1;
					room[0].width=GetRand(3)+3;
					room[0].length=GetRand(3)+3;
					if ((room[0].x+room[0].width<ROOMX-1)&&(room[0].y+room[0].length<ROOMY-1)) 
					{
						for (int i=0;i<room[0].width;i++)
						{
							for (int j=0;j<room[0].length;j++)
							{
								mass[room[0].x+i][room[0].y+j].type=2;
							}
							mass[room[0].x+i][room[0].y-1].type=5;
							mass[room[0].x+i][room[0].y+room[0].length].type=5;
						}
						for (int i=0;i<room[0].length;i++)
						{
							mass[room[0].x+room[0].width][room[0].y+i].type=5;
							mass[room[0].x-1][room[0].y+i].type=5;
						}
						break;
					}
				}
			}
			/*
			if (count2%75==0)
			{
				DrawBox(0,0,640,480,(0,0,0),1);
				DrawFormatString(0,0,GetColor(255,255,255),"%d",b);
				DrawCircle(620+cos(loading*0.003141592)*10,460+sin(loading*0.003141592)*10,5,GetColor(255,255,255),1);
				DrawCircle(620,460,10,GetColor(255,255,255),0);
				DrawCircle(620,460,9,GetColor(255,255,255),0);
				ScreenFlip();
			}*/
		}
		if (b>sqrt(ROOMX*ROOMY*0.03))break;
	}
	while(1)
	{
		int a=0,b=0;
		a=GetRand(ROOMX-1);
		b=GetRand(ROOMY-1);
		if (((mass[a][b].type==1)&&(GetRand(2)==0))||(mass[a][b].type==2))
		{
			me.kx=a;
			me.ky=b;
			break;
		}
	}
	int c=0;
	while(me.ilimit-1-DIFFICULT/5+GetRand(sqrt(20.0-DIFFICULT))>c)
	{
		int a=GetRand(ROOMX-1),b=GetRand(ROOMY-1);
		if ((mass[a][b].item==0)&&((mass[a][b].type==1)||(mass[a][b].type==2)))
		{
			//mass[a][b].item=GetRand(2)+1;
			//mass[a][b].itemk=1;
			mass[a][b].item=GetRand(mitem-1)+1;//アイテムの種類
			int d=0;
			if (me.floa>3)
			{
				for (int i=0;i<mitem;i++)
				{
					d+=(7-item[i].rare)*(7-item[i].rare)*(7-item[i].rare)+(7-item[i].rare)*(7-item[i].rare)+item[i].rare*me.floa/10;
				}
				d=GetRand(d-1);
				for (int i=0;i<mitem;i++)
				{
					if (d<(7-item[i].rare)*(7-item[i].rare)*(7-item[i].rare)+(7-item[i].rare)*(7-item[i].rare)+item[i].rare*me.floa/10)
					{
						mass[a][b].item=i+1;
						break;
					}
					else d-=(7-item[i].rare)*(7-item[i].rare)*(7-item[i].rare)+(7-item[i].rare)*(7-item[i].rare)+item[i].rare*me.floa/10;
				}
				if (item[mass[a][b].item-1].imp==4)
				{
					mass[a][b].itemk=GetRand(3)*GetRand(4)*GetRand(2)+GetRand(3)+1;
				}
				else
				{
					mass[a][b].itemk=1;
				}
				mass[a][b].itemf=false;
				//break;
			}
			else
			{
				for (int i=0;i<mitem;i++)
				{
					d+=(7-item[i].rare)*(7-item[i].rare);
				}
				d=GetRand(d-1);
				for (int i=0;i<mitem;i++)
				{
					if (d<(7-item[i].rare)*(7-item[i].rare))
					{
						mass[a][b].item=i+1;
						break;
					}
					else d-=(7-item[i].rare)*(7-item[i].rare);
				}
				if (item[mass[a][b].item-1].imp==4)
				{
					mass[a][b].itemk=GetRand(3)*GetRand(4)*GetRand(2)+GetRand(3)+1;
				}
				else
				{
					mass[a][b].itemk=1;
				}
				mass[a][b].itemf=false;
				//break;
			}
			c++;
		}
	}
	while(1)
	{
		me.x=GetRand(ROOMX-3)+1;
		me.y=GetRand(ROOMY-3)+1;
		if ((mass[me.x][me.y].type<=2)&&(mass[me.x][me.y].type>=1)&&(mass[me.x][me.y].item==0))
		{
			mass[me.x][me.y].thing=true;
			break;
		}
	}
	for (int i=0;i<sqrt(me.floa*1.0)+2;i++)
	{
		while(1)
		{
			int a=0;
			int b=0;
			a=GetRand(ROOMX-1);
			b=GetRand(ROOMY-1);
			if ((mass[a][b].trap==0)&&(mass[a][b].thing==false)&&(mass[a][b].type==2)&&(mass[a][b].item==0))
			{
				if ((mass[a-1][b].type!=1)&&(mass[a+1][b].type!=1)&&(mass[a][b-1].type!=1)&&(mass[a][b+1].type!=1))
				{
					mass[a][b].trapf=false;
					mass[a][b].trap=GetRand(2)+1;
					break;
				}
			}
		}
	}
	if (mass[me.x][me.y].type==2)
	{
		for (int i=0;i<20;i++)
		{
			if ((me.x>=room[i].x)&&(me.x<room[i].x+room[i].width)&&(me.y>=room[i].y)&&(me.y<room[i].y+room[i].length))
			{
				me.inroom=i;
				break;
			}
		}
		for (int i=room[me.inroom].x-1;i<room[me.inroom].x+room[me.inroom].width+1;i++)
		{
			for (int j=room[me.inroom].y-1;j<room[me.inroom].y+room[me.inroom].length+1;j++)
			{
				mass[i][j].apeear=true;
				mass[i][j].itemf=true;
			}
		}
	}
	else 
	{
		for (int i=0;i<ROOMX;i++)
		{
			for (int j=0;j<ROOMY;j++)
			{
				if ((i-me.x)*(i-me.x)+(j-me.y)*(j-me.y)<me.siya*4)
				{
					mass[i][j].apeear=true;
					mass[i][j].itemf=true;
				}
			}
		}
	}
	for (int i=0;i<50;i++)
	{
		enemy[i].hp=0;
	}
	/*for (int i=0;i<GetRand(5)+5;i++)
	{
		enemy[i].x=-1;
		while ((mass[enemy[i].x][enemy[i].y].type==0)||(mass[enemy[i].x][enemy[i].y].thing==true)||(enemy[i].x==-1))
		{
			enemy[i].x=GetRand(19);
			enemy[i].y=GetRand(19);
		}
		mass[enemy[i].x][enemy[i].y].thing=true;
		enemy[i].hpm=8+GetRand(5);
		enemy[i].hp=enemy[i].hpm;
		enemy[i].notice=false;
		enemy[i].atk=GetRand(3+me.floa/2)+2;
		enemy[i].type=1;
	}*/
	me.scrolx=me.x-8;
	me.scroly=me.y-5;
	me.move[0]=2;
	for (int i=1;i<2000;i++)
	{
		me.move[i]=0;
	}
	for (int i=1;i<me.mmax;i++)
	{
		me.move[i]=1;//動けない
	}
	for (int i=0;i<me.mlimit-1;i++)
	{
		while(1)
		{
			int a=GetRand(me.mmax-1)+1;
			if (me.move[a]==1)
			{
				me.move[a]=2;
				break;
			}
		}
	}
	me.movemax=me.mmax;
	/*for (int i=0;i<ROOMX;i++)
	{
		for (int j=0;j<ROOMY;j++)
		{
			mass[i][j].apeear=true;
			mass[i][j].trapf=true;
			mass[i][j].itemf=true;
			//mass[i][j].type=1;
		}
	}
	me.eseeab=true;*/
}
void tinc(int type,int pal)
{
	int a=0;
	if (pal>0)
	{
		for (int i=me.turns;i<me.movemax;i++)
		{
			int b=GetRand(me.movemax-me.turns-1)+me.turns+1;
			for (int j=me.movemax;j>b;j--)
			{
				me.move[j]=me.move[j-1];
			}
			me.movemax++;
			me.move[b]=type;
		}
	}
	else//減らす
	{
		for (int i=me.turns;i<me.movemax;i++)
		{
			while(1)
			{
				int b=GetRand(me.movemax-me.turns-2)+me.turns+1;
				if (me.move[b]==3-type)
				{
					for (int j=b;j<me.movemax;j++)
					{
						me.move[j]=me.move[j+1];
					}
					me.movemax--;
					break;
				}
			}
		}
	}
}
void tcha(int type,int pal)//1:動けない,2:動ける
{
	int a=0;
	if (pal>0)
	{
		for (int i=me.turns;i<me.movemax;i++)
		{
			if (me.move[i]==3-type)
			{
				a++;
			}
		}
		if (a>pal)
		{
			for (int i=0;i<pal;i++)
			{
				while(1)
				{
					int b=GetRand(me.movemax-me.turns-1)+me.turns;
					if (me.move[b]==3-type)
					{
						me.move[b]=type;
						break;
					}
				}
			}
		}
		else
		{
			for (int i=me.turns;i<me.movemax;i++)
			{
				me.move[i]=type;
			}
		}
	}
}