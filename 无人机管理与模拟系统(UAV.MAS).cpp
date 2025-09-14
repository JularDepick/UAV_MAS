//无人机管理与模拟系统(UAV.MAS)
//UAV.MAS: Unmanned Aerial Vehicle. Management and Analog System

#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
using namespace std;

class Fly {//无人机类
	public:
		void print_details() {
			//输出信息
			cout << "\n名称:" << Name
			     << "\n编号:" << ID
			     << "\n型号:" << Version
			     << "\n状态:" << Avail
			     << "\n描述:" << Desc
			     << "\n备注:" << Note << endl;
		}
		void clean() {
			//重置无人机
			Name = "无";
			ID = "无";
			Version = "无";
			Avail = "无";
			Desc = "无";
			Note = "无";
		}

		Fly() {
			//构造函数(初始化)
			Name = "无";
			ID = "无";
			Version = "无";
			Avail = "无";
			Desc = "无";
			Note = "无";
		}

		string Name;     //无人机名称(可重复)
		string ID;       //无人机编号(唯一确定)
		string Version;  //无人机型号(可重复)
		string Avail;    //无人机状态(在岗/已退休)
		string Desc;     //Description无人机描述(不包含特殊字符的字符串)
		string Note;     //无人机备注(不包含特殊字符的字符串)
} Flyer;
vector<Fly> FV;   //无人机仓库承载容器(运行时储存在栈上)

/////////全局常量域///////////
string Head_Str =
    "—————————————————无人机管理与模拟系统(UAV.MAS)—————————————————\n\n";
string Please_Str = "\n>>请输入指令:\n";
string Found = "\n>>查找成功！\n";
string Unfound = "\n>>查找失败！\n";
string Right = ""; //该正确密令为密文
//////
HANDLE Winptr = GetStdHandle(STD_OUTPUT_HANDLE);	//当前控制台窗口句柄(指针)
int _Backcolor = 15; //控制台颜色·背景色
int _Fontcolor = 9; //控制台颜色·字色
const COORD Sizer = {100, 20000}; //缓冲区大小(const是因为没const的时候程序出bug导致进度丢失了qwq)
////////////////////////////

/////////函数声明域//////////
int SLP();   //系统加载
void SFD();  //保存数据
int MMP();   //主菜单
void WFD();  //查询无人机信息
void CFD();  //更改无人机信息
void ADFD(); //增删无人机信息
void FAG();  //无人机模拟小游戏
void SLKM(); //系统密令管理
void MF();   //更多功能
int GMr(int, int); //获取模式
void OE();       //执行结束
bool Lockor(string);    //密令保护
string Locker(string);  //密令加密算法
string tostring(int);  //整型转字符串
///////////////////////////

//////////主函数////////////
int main() {
	if (SLP()) {
		return 0;
	}
Begin:
	int Model = MMP(); //主菜单
	switch (Model) {
		case 0: {
			system("cls");
			cout << Head_Str
			     << "\n>>保存并退出！";
			SFD();
			fflush(stdin);
			getch();
			return 0;
			break;
		}
		case 1: {
			//查询无人机信息
			system("cls");
			WFD();
			goto Begin;
			break;
		}
		case 2: {
			//更改无人机信息
			system("cls");
			CFD();
			goto Begin;
			break;
		}
		case 3: {
			//无人机入岗或退休
			system("cls");
			ADFD();
			goto Begin;
			break;
		}
		case 4: {
			//无人机模拟小游戏
			system("cls");
			FAG();
			goto Begin;
			break;
		}
		case 5: {
			//系统密令管理
			system("cls");
			SLKM();
			goto Begin;
			break;
		}
		case 6: {
			//更多功能
			system("cls");
			MF();
			goto Begin;
			break;
		}
		default: {
			cout << "\n\n未知错误！";
			Sleep(2000);
			return 0;
			break;
		}
	}
	return 0;
}
///////////////////////////

//////////函数定义域///////////
int SLP() {
	//SLP: System_Loading_Page系统加载页面
	Winptr = GetStdHandle(STD_OUTPUT_HANDLE); //抓取窗口句柄
	system("color F9");
	system("cls");
	system("mode con: cols=100 lines=45");

	//保存的设置
	ifstream wfin("UAV_MAS_Setting_Datas.txt", ios::in);
	if (!wfin.is_open()) {
		//无外部文件:默认
		int _Backcolor = 15;
		int _Fontcolor = 9;
	} else {
		//外部设置
		wfin >> _Backcolor >> _Fontcolor; //窗口颜色
		if (_Backcolor < 0 || _Backcolor > 15 || _Fontcolor < 0 || _Fontcolor > 15 || _Backcolor == _Fontcolor) {
			_Backcolor = 15;
			_Fontcolor = 9; //有误:还原为默认
		}
	}
	wfin.close();
	wfin.clear();

	SetConsoleTitleA("无人机管理与模拟系统(UAV.MAS)    Unmanned Aerial Vehicle. Management and Analog System");
	{
		//控制台字体设置
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 10;
		cfi.dwFontSize.Y = 18;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_HEAVY;
		SetCurrentConsoleFontEx(Winptr, FALSE, &cfi); //启用
		SetConsoleScreenBufferSize(Winptr, Sizer);  //设置缓冲区
	}

	SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Fontcolor); //启用颜色
	system("cls");
	cout << Head_Str;
	Sleep(500);
	cout << "系统加载中，请稍候";
	for (int i = 0; i < 10; i++) {
		cout << ".";
		Sleep(50);
	}
	//前置文件流加载及其排查
	ifstream fina, finb;
	fina.open("UAV_MAS_working_datas.txt", ios::in);
	finb.open("UAV_MAS_retired_datas.txt", ios::in);
	if ((!fina.is_open()) || (!finb.is_open())) {
		return 0;
	}
	Sleep(300);
	system("cls");
	cout << Head_Str;
	cout << "\n>>前置文件正常！\n";
	string name, id, version, avail, desc, note;
	fflush(stdin);
	while (fina >> name >> id >> version >> avail >> desc >> note) {
		Flyer.Name = name;
		Flyer.ID = id;
		Flyer.Version = version;
		if (avail == "在岗") {
			//避免用户非法状态值
			Flyer.Avail = avail;
		} else {
			Flyer.Avail = "已退休";
		}
		Flyer.Desc = desc;
		Flyer.Note = note;
		FV.push_back(Flyer);
	}
	fina.close();
	fina.clear();
	fflush(stdin);
	while (finb >> name >> id >> version >> avail >> desc >> note) {
		Flyer.Name = name;
		Flyer.ID = id;
		Flyer.Version = version;
		if (avail == "在岗") {
			//避免用户非法状态值
			Flyer.Avail = avail;
		} else {
			Flyer.Avail = "已退休";
		}
		Flyer.Desc = desc;
		Flyer.Note = note;
		FV.push_back(Flyer);
	}
	finb.close();
	finb.clear();
	Sleep(100);
	cout << "\n>>加载成功！\n";
	Sleep(500);

	ifstream kin;
	kin.open("UAV_MAS_key_encoded.txt", ios::in);
	if (kin.is_open()) {
		kin >> Right;
	}
	kin.close();
	kin.clear();
	if (!int(Right.size())) {
		return 0;
	}
Begin_Cin_Key:
	system("cls");
	cout << Head_Str
	     << ">>请输入管理员密令:" << endl;
	string Key;
	while (1) {
		fflush(stdin);
		cin >> Key;
		if (Lockor(Locker(Key))) {
			Sleep(300);
			cout << "\n>>密令正确,载入系统！\n";
			Sleep(500);
			break;
		} else {
			cout << "\n>>密令错误！" << endl;
			Sleep(300);
			cout << "\n>>按1键重新输入，按其他键退出程序！" << endl;
			fflush(stdin);
			if (getch() != '1') {
				system("cls");
				cout << Head_Str
				     << ">>退出程序！" << endl;
				Sleep(1500);
				return -1;
			} else {
				goto Begin_Cin_Key;
			}
		}
	}
	system("cls");
	return 0;
}

void SFD() {
	//SFD: Save_Flys_Datas保存数据

	//保存无人机数据
	ofstream fouta, foutb;
	fouta.open("UAV_MAS_working_datas.txt", ios::out);
	foutb.open("UAV_MAS_retired_datas.txt", ios::out);
	if (int(FV.size())) {
		for (int i = 0; i < int(FV.size()); i++) {
			if (FV[i].Avail == "在岗") {
				Flyer = FV[i];
				fouta << Flyer.Name << endl
				      << Flyer.ID << endl
				      << Flyer.Version << endl
				      << Flyer.Avail << endl
				      << Flyer.Desc << endl
				      << Flyer.Note << endl << endl;
			} else {
				Flyer = FV[i];
				foutb << Flyer.Name << endl
				      << Flyer.ID << endl
				      << Flyer.Version << endl
				      << Flyer.Avail << endl
				      << Flyer.Desc << endl
				      << Flyer.Note << endl << endl;
			}
		}
	} else {
		fouta << "\n";
		foutb << "\n";
	}
	fouta.close();
	foutb.close();
	fouta.clear();
	foutb.clear();

	//保存密文
	ofstream kout;
	kout.open("UAV_MAS_key_encoded.txt", ios::out);
	if (int(Right.size())) {
		kout << Right;
	} else {
		kout << endl;
	}
	kout.close();
	kout.clear();

	//保存设置
	ofstream wfout("UAV_MAS_Setting_Datas.txt", ios::out);
	wfout << _Backcolor << " " << _Fontcolor << endl;
	wfout.close();
	wfout.clear();
}

int MMP() {
	//MMP: Main_Menu_Page主菜单页面
	system("cls");
	SetConsoleScreenBufferSize(Winptr, Sizer);
	cout << Head_Str
	     << "【主菜单】" << endl << endl
	     << "0.保存并退出" << endl
	     << "1.查询无人机信息" << endl
	     << "2.更改无人机信息" << endl
	     << "3.无人机入岗或退休" << endl
	     << "4.无人机模拟(小游戏)" << endl
	     << "5.系统密令管理" << endl
	     << "6.更多功能" << endl;
	cout << Please_Str;
	return GMr(48, 54);
}

void WFD() {
	//WFD: Watch_Fly_Datas查询无人机信息
	system("cls");
	cout << Head_Str
	     << "【查询无人机信息】\n" << endl
	     << "0.返回主菜单" << endl
	     << "1.查询全部无人机信息" << endl
	     << "2.按名称查询无人机信息" << endl
	     << "3.按编号查询无人机信息" << endl
	     << "4.按型号查询无人机信息" << endl
	     << "5.按状态查询无人机信息" << endl;
	cout << Please_Str;
	int Model = GMr(48, 53);
	switch (Model) {
		case 0:
			break;
		case 1: {
			system("cls");
			cout << Head_Str
			     << "【查询全部无人机信息】\n";
			Sleep(50);
			for (int pos = 0; pos < int(FV.size()); pos++) {
				FV[pos].print_details();
				Sleep(50);
			}
			OE();
			break;
		}
		case 2: {
			system("cls");
			cout << Head_Str
			     << "【按名称查询无人机信息】" << endl << endl;
			cout << ">>请输入无人机名称(请勿包含空格)：" << endl;
			string name;
			fflush(stdin);
			cin >> name;
			int has = 0;
			for (int pos = 0; pos < int(FV.size()); pos++) {
				if (FV[pos].Name == name) {
					has = 1;
					FV[pos].print_details();
					Sleep(50);
				}
			}
			if (!has) {
				system("cls");
				cout << Head_Str
				     << "【按名称查询无人机信息】" << endl;
				cout << Unfound;
			}
			OE();
			break;
		}
		case 3: {
			system("cls");
			cout << Head_Str
			     << "【按编号查询无人机信息】" << endl << endl;
			cout << ">>请输入无人机编号(请勿包含空格)：" << endl;
			string id;
			fflush(stdin);
			cin >> id;
			int has = 0;
			for (int pos = 0; pos < int(FV.size()); pos++) {
				if (FV[pos].ID == id) {
					has = 1;
					system("cls");
					cout << Head_Str
					     << "【按编号查询无人机信息】" << endl;
					cout << Found;
					FV[pos].print_details();
					Sleep(50);
					break;
				}
			}
			if (!has) {
				system("cls");
				cout << Head_Str
				     << "【按编号查询无人机信息】" << endl;
				cout << Unfound;
			}
			OE();
			break;
		}
		case 4: {
			system("cls");
			cout << Head_Str
			     << "【按型号查询无人机信息】" << endl << endl;
			cout << ">>请输入无人机型号(请勿包含空格)：" << endl;
			string version;
			fflush(stdin);
			cin >> version;
			int has = 0;
			for (int pos = 0; pos < int(FV.size()); pos++) {
				if (FV[pos].Version == version) {
					has = 1;
					FV[pos].print_details();
					Sleep(50);
				}
			}
			if (!has) {
				system("cls");
				cout << Head_Str
				     << "【按型号查询无人机信息】" << endl;
				cout << Unfound;
			}
			OE();
			break;
		}
		case 5: {
			system("cls");
			cout << Head_Str
			     << "【按状态查询无人机信息】" << endl
			     << "0.返回主菜单" << endl
			     << "1.查寻全部在岗无人机信息" << endl
			     << "2.查询全部已退休无人机信息" << endl;
			cout << Please_Str;
			int Model = GMr(48, 50);
			switch (Model) {
				case 0:
					break;
				case 1: {
					for (int pos = 0; pos < int(FV.size()); pos++) {
						if (FV[pos].Avail == "在岗") {
							cout << endl;
							FV[pos].print_details();
							Sleep(50);
						}
					}
					OE();
					break;
				}
				case 2: {
					for (int pos = 0; pos < int(FV.size()); pos++) {
						if (FV[pos].Avail == "已退休") {
							cout << endl;
							FV[pos].print_details();
							Sleep(50);
						}
					}
					OE();
					break;
				}
				default:
					break;
			}
			break;
		}
		default:
			break;
	}
}

void CFD() {
	string Content, Option, id;
	int pos = 0, has = 0;
	//WFD: Change_Fly_Datas更改无人机信息
Begin_CFD:
	system("cls");
	cout << Head_Str
	     << "【更改无人机信息】" << endl
	     << "\n>>按0键返回主菜单，按其他键继续更改无人机信息" << endl;
	if (getch() == '0')
		goto End_CFD;
	cout << "\n>>请输入无人机编号(请勿包含空格):" << endl;
	fflush(stdin);
	cin >> id;
	system("cls");
	cout << Head_Str
	     << "【更改无人机信息】" << endl;
	for (; pos < int(FV.size()); pos++) {
		if (FV[pos].ID == id) {
			has = 1;
			FV[pos].print_details();
			cout << endl << endl;
			break;
		}
	}
	if (!has) {
		cout << Unfound;
		cout << "\n>>按1键重新查找，按其他键返回主菜单\n";
		if (getch() == '1')
			goto Begin_CFD;
		else
			goto End_CFD;
	}
Begin2_CFD:
	cout << ">>请输入需要更改的信息项:\n";
	while (1) {
		fflush(stdin);
		cin >> Option;
		//不允许在此修改无人机状态(避免非法状态值)
		if (Option == "状态") {
			cout << "\n>>警告！\n  非法操作！！！\n    此处不允许更改无人机状态，请回车或换行，然后前往【无人机入岗或退休】进行状态更改！"
			     << endl;
			MessageBox(NULL, TEXT("非法操作！\n此处不允许更改无人机状态，请前往【无人机入岗或退休】更改！"), TEXT("警告"),
			           MB_ICONEXCLAMATION | MB_OK);
			while (1) {
				fflush(stdin);
				char a = getch();
				if (a == '\n' || a == '\r')
					break;
			}
			goto Begin_CFD;
		}
		if (Option == "名称" || Option == "编号" || Option == "型号" || Option == "描述" || Option == "备注")
			break;
		cout << "\n>>该信息项不存在，请重新输入:\n";
	}
	cout << "\n>>请输入新的信息内容(请勿包含空格):\n";

	fflush(stdin);
	cin >> Content;
	if (Option == "名称") {
		FV[pos].Name = Content;
	} else if (Option == "编号") {
		FV[pos].ID = Content;
	} else if (Option == "型号") {
		FV[pos].Version = Content;
	} else if (Option == "描述") {
		FV[pos].Desc = Content;
	} else if (Option == "备注") {
		FV[pos].Note = Content;
	}
	cout << "\n>>请仔细核实确保修改信息无误，按1键取消更改，按其他键确认更改\n";
	int yes;
	fflush(stdin);
	if (getch() == '1') {
		cout << "\n>>取消更改！\n";
		yes = 0;
	} else {
		cout << "\n更改成功！\n";
		yes = 1;
	}
	Sleep(1000);
	system("cls");
	cout << Head_Str
	     << "【更改无人机信息】" << endl
	     << "\n>>按1键" << (yes ? "继续更改其他无人机" : "重新更改") << "，按其他键返回主菜单\n";
	fflush(stdin);
	if (getch() == '1') {
		cout << "\n";
		FV[pos].print_details();
		goto Begin2_CFD;
	}
End_CFD:
	;
}

void ADFD() {
	//ADFD: Add_or_Delete_Fly_Datas增删无人机信息
	system("cls");
	cout << Head_Str
	     << "【无人机入岗或退休】" << endl
	     << "0.返回主菜单" << endl
	     << "1.无人机上岗(更改状态)" << endl
	     << "2.无人机退休(更改状态)" << endl
	     << "3.无人机入岗(新增个体)" << endl
	     << "4.摧毁无人机(彻底删除)" << endl
	     << "5.清空无人机仓库(密保)" << endl;
	cout << Please_Str;
	int Model = GMr(48, 53);
	switch (Model) {
		case 0:
			break;
		case 1: {
Begin2_ADFD:
			system("cls");
			cout << Head_Str
			     << "【无人机上岗】" << endl;
			cout << "\n\n>>请输入无人机编号:" << endl;
			string id;
			fflush(stdin);
			cin >> id;
			for (int pos = 0; pos < int(FV.size()); pos++) {
				if (FV[pos].ID == id) {
					cout << Found;
					FV[pos].print_details();
					if (FV[pos].Avail == "在岗") {
						cout << "\n\n>>该无人机已在岗，请勿重复上岗！" << endl;
						Sleep(500);
						goto Here1;
					}
					cout << "\n>>回车或换行确认上岗，按其他键取消上岗" << endl;
					fflush(stdin);
					char a;
					a = getch();
					if (a == '\n' || a == '\r') {
						FV[pos].Avail = "在岗";
						cout << "\n>>上岗成功！";
						Sleep(500);
Here1:
						;
						cout << "\n\n>>按1键继续上岗其他无人机，按其他键返回主菜单" << endl;
						fflush(stdin);
						if (getch() == '1') {
							goto Begin2_ADFD;
						} else
							goto End_ADFD2;
					}
				}
			}
			cout << Unfound;
			cout << "\n>>按1键重新输入查找，按其他键返回主菜单" << endl;
			fflush(stdin);
			if (getch() == '1')
				goto Begin3_ADFD;
End_ADFD2:
			;
			break;
		}
		case 2: {
Begin2_ADFD2:
			system("cls");
			cout << Head_Str
			     << "【无人机退休】" << endl;
			cout << "\n\n>>请输入无人机编号:" << endl;
			string id;
			fflush(stdin);
			cin >> id;
			for (int pos = 0; pos < int(FV.size()); pos++) {
				if (FV[pos].ID == id) {
					cout << Found;
					FV[pos].print_details();
					if (FV[pos].Avail == "已退休") {
						cout << "\n\n>>该无人机已退休，请勿重复退休！" << endl;
						Sleep(500);
						goto Here13;
					}
					cout << "\n>>回车或换行确认退休，按其他键取消退休" << endl;
					fflush(stdin);
					char a;
					a = getch();
					if (a == '\n' || a == '\r') {
						FV[pos].Avail = "已退休";
						cout << "\n>>退休成功！";
						Sleep(500);
Here13:
						;
						cout << "\n\n>>按1键继续退休其他无人机，按其他键返回主菜单" << endl;
						fflush(stdin);
						if (getch() == '1') {
							goto Begin2_ADFD2;
						} else
							goto End_ADFD22;
					} else {
						goto End_ADFD22;
					}
				}
			}
			cout << Unfound;
			cout << "\n>>按1键重新输入查找，按其他键返回主菜单" << endl;
			fflush(stdin);
			if (getch() == '1')
				goto Begin3_ADFD;
End_ADFD22:
			;
			break;
		}
		case 3: {
Begin3_ADFD:
			system("cls");
			cout << Head_Str
			     << "【无人机入岗】" << endl;
			cout << "\n请按提示逐步输入无人机入岗信息(请勿包含空格)" << endl << endl;
			cout << "名称:";
			fflush(stdin);
			cin >> Flyer.Name;
			cout << "编号:";
			fflush(stdin);
			cin >> Flyer.ID;
			cout << "型号:";
			fflush(stdin);
			cin >> Flyer.Version;
			Flyer.Avail = "在岗";
			cout << "描述:";
			fflush(stdin);
			cin >> Flyer.Desc;
			cout << "备注:";
			fflush(stdin);
			cin >> Flyer.Note;
			cout << endl;
			while (1) {
				for (int i = 0; i < int(FV.size()); i++) {
					if (FV[i].ID == Flyer.ID) {
						cout << "\n编号重复，请重新输入编号:\n";
						fflush(stdin);
						cin >> Flyer.ID;
						goto While_Here;
					}
				}
				break;
While_Here:
				;
			}
			cout << ">>请仔细核实确保信息无误，按1键取消入岗，按其他键确认入岗" << endl;
			fflush(stdin);
			if (getch() == '1') {
				cout << ">>取消无人机入岗！" << endl;
				Flyer.clean();
				cout << "\n\n>>按1键重新入岗，按其他键返回主菜单" << endl;
				fflush(stdin);
				if (getch() == '1') {
					goto Begin3_ADFD;
				}
				goto End_ADFD3;
			}
			FV.push_back(Flyer);
			Flyer.clean();
			cout << endl
			     << "\n>>无人机入岗成功！" << endl;
			OE();
End_ADFD3:
			;
			break;
		}
		case 4: {
Begin4_ADFD:
			int has = 0;
			system("cls");
			cout << Head_Str
			     << "【摧毁无人机】" << endl;
			cout << "\n\n>>请输入无人机编号:" << endl;
			string id;
			fflush(stdin);
			cin >> id;
			for (int pos = 0; pos < int(FV.size()); pos++) {
				if (FV[pos].ID == id) {
					has = 1;
					cout << Found;
					FV[pos].print_details();
					cout << "\n>>回车或换行确认摧毁，按其他键取消摧毁" << endl;
					fflush(stdin);
					char a = getch();
					if (a == '\n' || a == '\r') {
						for (int i = pos; i < int(FV.size()) - 1; i++) {
							FV[i] = FV[i + 1];
						}
						FV.pop_back();
						cout << "\n>>摧毁成功！";
						Sleep(500);
						cout << "\n\n>>按1键继续摧毁其他无人机，按其他键返回主菜单" << endl;
						fflush(stdin);
						if (getch() == '1') {
							goto Begin4_ADFD;
						} else
							goto End_ADFD4;
					}
				}
			}
			if (!has)
				cout << Unfound;
			cout << "\n>>按1键重新输入查找，按其他键返回主菜单" << endl;
			fflush(stdin);
			if (getch() == '1')
				goto Begin4_ADFD;
End_ADFD4:
			;
			break;
		}
		case 5: {
			system("cls");
			cout << Head_Str
			     << "【清空无人机仓库】" << endl;
			cout << "\n\n>>按1键确认清空，按其他键返回主菜单" << endl;
			fflush(stdin);
			if (getch() == '1') {
				if (!int(Right.size())) {
					cout << "\n>>清空成功！" << endl;
					OE();
					FV.clear();
					break;
				}
				cout << "\n请输入管理员密令" << endl;
				string Key;
				cin >> Key;
				if (Lockor(Locker(Key))) {
					cout << "\n密令正确，清空成功！" << endl;
					OE();
					FV.clear();
				} else {
					cout << "\n>>密令错误！！！" << endl;
					OE();
				}
			}
			break;
		}
		default:
			break;
	}
}

class FAer  {
	public:
		//操作型成员函数
		void Walk() {
			//前进一格

			//记录当前位置信息以修复轨迹
			int posxxx = pos_x;
			int posyyy = pos_y;
			int posdottt = pos_dot;
			switch (dir) {
				case 0:
					break;
				case 1: {
					if (pos_h >= int(_map[pos_y][pos_x + 1]) - 48) {
						pos_x++;
					} else if (pos_y == end_y && pos_x + 1 == end_x && pos_h >= end_h) {
						pos_x++;
					}
					break;
				}
				case 2: {
					if (pos_h >= int(_map[pos_y - 1][pos_x]) - 48)
						pos_y--;
					break;
				}
				case 3: {
					if (pos_h >= int(_map[pos_y][pos_x - 1]) - 48)
						pos_x--;
					break;
				}
				case 4: {
					if (pos_h >= int(_map[pos_y + 1][pos_x]) - 48)
						pos_y++;
				}
				default:
					break;
			}
			Fixed();//位置修正
			_map[posyyy][posxxx] = char(posdottt + 48);
			pos_dot = int(_map[pos_y][pos_x]) - 48;
		}
		void TLeft() {
			dir++;
			Fixed();
		}
		void TRight() {
			dir--;
			Fixed();
		}
		void TBehind() {
			dir += 2;
			Fixed();
		}
		void WUp() {
			//向上飞行1格
			pos_h++;
			Fixed();
		}
		void WDown() {
			//向下飞行1格
			pos_h--;
			Fixed();
		}
		//调试型成员函数
		void Fixed() {
			//位置修正(防止出界)+方向修正
			if (pos_x >= mapsize)
				pos_x = mapsize - 1;
			if (pos_x < 0)
				pos_x = 0;
			if (pos_y >= mapsize)
				pos_y = mapsize - 1;
			if (pos_y < 0)
				pos_y = 0;
			if (pos_h < pos_dot)
				pos_h = pos_dot;
			if (pos_h > 9)
				pos_h = 9; //限高9
			if (dir > 4)
				dir -= 4;
			if (dir < 1)
				dir += 4;
		}
		void Pre_map() {
			//初始化地图及无人机起终点等
			for (int i = 0; i < mapsize; i++) {
				for (int j = 0; j < mapsize; j++) {
					srand(time(0) + i * j + i + j);
					_map[i][j] = char(abs(rand() % 10) + 48);
				}
			}
			srand(time(0) + 10086);
			pos_x = rand() % mapsize;
			srand(time(0) * 123 + 10086);
			pos_y = rand() % mapsize;
			pos_h = int(_map[pos_y][pos_x]) - 48;
			dir = 1;
			for (int i = 1;; i++) {
				//随机设置终点
				srand(time(0) * 12 * i);
				end_x = rand() % mapsize;
				srand(time(0) * 23 * i + i);
				end_y = rand() % mapsize;
				if ((pos_x != end_x) && (pos_y != end_y))
					break;
			}
			end_h = int(_map[end_y][end_x]) - 48;
		}
		void Out_map() {
			//更新并输出地图
			char thiser;
			switch (dir) {
				case 1: {
					thiser = '>';
					break;
				}
				case 2: {
					thiser = 'A';
					break;
				}
				case 3: {
					thiser = '<';
					break;
				}
				case 4: {
					thiser = 'V';
					break;
				}
			}
			int _Show_Fontcolor = ((_Backcolor + _Fontcolor) + 10086) % 16; //防止撞色
			if (_Backcolor == 15 && _Fontcolor == 9) {
				_Show_Fontcolor = 12;
			}
			for (int i = 0; i < mapsize; i++) {
				for (int j = 0; j < mapsize; j++) {
					if (i == pos_y && j == pos_x) {
						SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Show_Fontcolor); //显色提示
						cout << thiser << ' ';
						SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Fontcolor);
					} else if (i == end_y && j == end_x) {
						//判断顺序保证了终点标志会被无人机标志覆盖掉
						SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Show_Fontcolor);
						cout << '+' << ' ';
						SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Fontcolor);
					} else {
						cout << _map[i][j] << ' ';
					}
				}
				cout << endl;
			}
			cout << endl
			     << "当前飞行高度:";
			SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Show_Fontcolor);
			cout << pos_h << endl;
			SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Fontcolor);
			cout << "当前地形高度:";
			SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Show_Fontcolor);
			cout << pos_dot << endl;
			SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Fontcolor);
			cout << "终点地形高度:";
			SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Show_Fontcolor);
			cout << end_h << endl;
			SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Fontcolor);
		}
		FAer() {
			pos_x = 0;
			pos_y = 0;
			pos_h = 0;
			pos_dot = 0;
			dir = 1;
			mapsize = 10;
			end_x = 0;
			end_y = 0;
		}

		int pos_x;  //无人机当前位置
		int pos_y;
		int pos_h;  //当前飞行高度
		int pos_dot;//当前地形高度
		int dir;    //无人机当前朝向(1朝东、2朝北、3朝西、4朝南)
		int mapsize;//无人机地图大小
		int end_x;  //无人机终点
		int end_y;
		int end_h;
		char _map[30][30];
} FARR;

void FAG() {
	FARR.mapsize = 10;
Begin_moni:
	//FAG: Fly_Analog_Game无人机模拟小游戏
Begin_FAG:
	system("cls");
	cout << Head_Str << endl
	     << "【无人机模拟·菜单】" << endl << endl
	     << "0.返回系统主菜单" << endl
	     << "1.操作指南" << endl
	     << "2.设置地图" << endl
	     << "3.开始游戏" << endl;
	int Model = GMr(48, 51);
	switch (Model) {
		case 0:
			goto END654;
		case 1: {
			system("cls");
			cout << Head_Str << endl
			     << "【无人机模拟·操作指南】" << endl
			     << "\n>>【版权声明】" << endl
			     << ">>本游戏Made By 李文芳 1724834368@qq.com，内嵌于UAV.MAS系统\n" << endl
			     << "Ⅰ.先在【菜单·设置地图】中设置游戏地图大小，不作设置默认大小为10*10" << endl
			     << "Ⅱ.然后【菜单·开始游戏】即可开始游戏，游戏时存在以下几种操作按键:" << endl
			     << "    W键:前进一格" << endl
			     << "    A键:左转向" << endl
			     << "    D键:右转向" << endl
			     << "    S键:后转向" << endl
			     << "    P键:向上飞行一格" << endl
			     << "    L键:向下飞行一格" << endl
			     << "    E键:结束游戏并返回菜单" << endl
			     << "Ⅲ.移动无人机到终点(且需降落，即需使飞行高度等于地形高度)即游戏胜利，后将随机奖励一架无人机，存入系统无人机仓库内(奖励可能失效)"
			     << endl
			     << "Ⅳ.游戏地图以数字为背景(数字即对应点地形高度)，以箭头表示无人机(箭头朝向即无人机朝向)，以+号为终点，当无人机当前飞行高度不小于前方一格的地形高度时，无人机才能向前飞行"
			     << endl
			     << "Ⅴ.如果出现游戏地图矩阵显示异常，请尝试更改地图大小或控制台大小" << endl;
			MessageBox(NULL, TEXT("\
【版权声明】\n\
本游戏程序 Made By 李文芳 1724834368@qq.com，内嵌于UAV.MAS系统\n\n\
Ⅰ.先在【菜单·设置地图】中设置游戏地图大小，不作设置默认大小为10*10\n\
Ⅱ.然后【菜单·开始游戏】即可开始游戏，游戏时存在以下几种操作按键:\n\
W键:前进一格\n\
A键:左转向\n\
D键:右转向\n\
S键:后转向\n\
P键:向上飞行一格\n\
L键:向下飞行一格\n\
E键:结束游戏并返回菜单\n\
Ⅲ.移动无人机到终点(且需降落，即需使飞行高度等于地形高度)即游戏胜利，后将随机奖励一架无人机，存入系统无人机仓库内(奖励可能失效)\n\
Ⅳ.游戏地图以数字为背景(数字即对应点地形高度)，以箭头表示无人机(箭头朝向即无人机朝向)，以+号为终点，当无人机当前飞行高度不小于前方一格的地形高度时，无人机才能向前飞行\n\
Ⅴ.如果出现游戏地图矩阵显示异常，请尝试更改地图大小或控制台大小\
"), TEXT("【UAV.MAS 无人机模拟·操作指南】"), MB_OK);

			cout << "\n>>回车或换行返回游戏菜单" << endl;
			while (1) {
				fflush(stdin);
				char a = getch();
				if (a == '\n' || a == '\r')
					goto Begin_FAG;
			}
			break;
		}
		case 2: {
			system("cls");
			cout << Head_Str << endl
			     << "【无人机模拟·设置地图】" << endl;
			cout << "请输入地图大小(一个10~30的正整数):" << endl;
			int sizer;
			while (1) {
				fflush(stdin);
				cin >> sizer;
				if (sizer >= 10 && sizer <= 30) {
					FARR.mapsize = sizer;
					cout << "\n>>设置成功！" << endl;
					cout << "\n\n>>回车或换行返回游戏菜单" << endl;
					while (1) {
						fflush(stdin);
						char a = getch();
						if (a == '\n' || a == '\r')
							goto Begin_FAG;
					}
					break;
				}
				cout << "\n\n>>输入数字不符合要求，请重新输入:" << endl;
			}
			break;
		}
		case 3: {
			FARR.Pre_map();
			for (;;) {
				//循环
				system("cls");
				cout << Head_Str << endl
				     << "【无人机模拟·游戏中】\n\n" << endl;
				FARR.Out_map();   //更新并输出地图
				if (FARR.pos_x == FARR.end_x && FARR.pos_y == FARR.end_y && FARR.end_h == FARR.pos_h) {
					cout << "\n\n>>游戏胜利，获得一架无人机！\n";
					//奖励机制
					srand(time(0) + 10086);
					if (!(int(FV.size()))) {
						Flyer.Name = "无敌号";
						for (int i = 11;; i++) {
here02:
							srand(time(0)*i + i);
							Flyer.ID = tostring(rand() % 1000001);
							for (int j = 0; j < int(FV.size()); j++) {
								if (Flyer.ID == FV[j].ID) {
									i++;
									goto here02;
								}
							}
							break;
						}
						Flyer.Version = "大型农业运载无人机";
						Flyer.Avail = "在岗";
						Flyer.Note = "无";
						goto End666;
					}
					Flyer.Name = FV[rand() % (int(FV.size()))].Name;
					Flyer.Version = FV[rand() * 11 % (int(FV.size()))].Version;
					for (int i = 11;; i++) {
here01:
						srand(time(0)*i + i);
						Flyer.ID = to_string(rand() % 1000001);
						for (int j = 0; j < int(FV.size()); j++) {
							if (Flyer.ID == FV[j].ID) {
								i++;
								goto here01;
							}
						}
						break;
					}
					Flyer.Avail = "在岗";
					Flyer.Note = "无";
					FV.push_back(Flyer);
End666:
					;
					break;
				}
				char move;
				while (1) {
					fflush(stdin);
					move = getch();
					if (islower(move)) {
						move = toupper(move);
					}
					if (move == 'W' || move == 'A' || move == 'D' || move == 'S' || move == 'P' || move == 'L' || move == 'E')
						break;
				}
				switch (move) {
					case 'W': {
						FARR.Walk();
						break;
					}
					case 'A': {
						FARR.TLeft();
						break;
					}
					case 'D': {
						FARR.TRight();
						break;
					}
					case 'S': {
						FARR.TBehind();
						break;
					}
					case 'P': {
						FARR.WUp();
						break;
					}
					case 'L': {
						FARR.WDown();
						break;
					}
					case 'E': {
						cout << "\n\n>>中途结束游戏！" << endl;
						Sleep(1000);
						goto Begin_moni;
						break;
					}
					default:
						break;
				}
			}
			cout << "\n\n>>回车或换行返回游戏菜单" << endl;
			while (1) {
				fflush(stdin);
				char a = getch();
				if (a == '\n' || a == '\r')
					break;
			}
			break;
		}
		default:
			break;
	}
	goto Begin_FAG;
END654:
	;
}

void SLKM() {
	//SLKM: System_Lock_Key_Management系统密令管理
	system("cls");
	cout << Head_Str
	     << "【系统密令管理】" << endl
	     << "0.返回主菜单" << endl
	     << "1.更改系统密令" << endl
	     << "2.关闭系统密令" << endl
	     << "3.启用系统密令" << endl;
	cout << Please_Str;
	int Model = GMr(48, 51);
	switch (Model) {
		case 0:
			break;
		case 1: {
			system("cls");
			cout << Head_Str
			     << "【更改系统密令】" << endl;

			if (!int(Right.size())) {
				cout << "\n>>系统密令未启用，无法更改系统密令！" << endl;
				OE();
				break;
			}

			cout << "\n>>请输入原密令:" << endl;
			string key;
			fflush(stdin);
			cin >> key;
			if (Lockor(Locker(key))) {
				cout << "\n密令正确！" << endl
				     << "\n请输入新密令(请勿包含空格):" << endl;
				string _New;
				fflush(stdin);
				cin >> _New;
				cout << "\n>>回车或换行确认更改，按其他键取消更改" << endl;
				fflush(stdin);
				char a = getch();
				if (a == '\n' || a == '\r') {
					Right = Locker(_New);
					cout << "\n>>更改成功！" << endl;
					OE();
					break;
				}
				cout << "\n>>取消更改！" << endl;
				OE();
				break;
			} else {
				cout << "\n>>密令错误！" << endl;
				OE();
				break;
			}
			break;
		}
		case 2: {
			system("cls");
			cout << Head_Str
			     << "【关闭系统密令】" << endl;

			if (!int(Right.size())) {
				cout << "\n>>系统密令已经关闭，请勿重复关闭！" << endl;
				OE();
				break;
			}

			cout << "\n>>请输入原密令:" << endl;
			string Key;
			fflush(stdin);
			cin >> Key;
			if (Lockor(Locker(Key))) {
				cout << "\n>>密令正确！" << endl
				     << "\n>>已关闭系统密令！" << endl;
				Right = "";
				Sleep(500);
				OE();
				break;
			} else {
				cout << "\n>>密令错误！" << endl;
				Sleep(500);
				OE();
				break;
			}
			break;
		}
		case 3: {
			system("cls");
			cout << Head_Str
			     << "【启用系统密令】" << endl;

			if (int(Right.size())) {
				cout << "\n>>系统密令已经启用，请勿重复启用！" << endl;
				OE();
				break;
			}

			cout << "\n>>请输入启用密令:" << endl;
			string Key;
			fflush(stdin);
			cin >> Key;
			cout << "\n>>回车或换行确认启用，按其他键取消启用" << endl;
			fflush(stdin);
			char a = getch();
			if (a == '\n' || a == '\r') {
				cout << "\n>>启用成功！" << endl;
				Right = Locker(Key);
				Sleep(500);
				OE();
				break;
			}
			cout << "\n>>取消启用！" << endl;
			Sleep(500);
			OE();
			break;
		}
	}
}

void MF() {
	//More_Function 更多功能
	system("cls");
	cout << Head_Str
	     << "【更多功能】" << endl
	     << "0.返回主菜单" << endl
	     << "1.设置窗口颜色" << endl
	     << "2.关于开发者、获取开源、彩蛋" << endl;
	cout << Please_Str;
	int Model = GMr(48, 50);
	switch (Model) {
		case 0: {
			break;
		}
		case 1: {
			system("cls");
			cout << Head_Str
			     << "【更多功能·设置窗口颜色】" << endl
			     << "0.返回主菜单" << endl
			     << "1.默认配色" << endl
			     << "2.自定义" << endl;
			cout << Please_Str;
			int Model = GMr(48, 50);
			switch (Model) {
				case 0: {
					break;
				}
				case 1: {
					_Backcolor = 15;
					_Fontcolor = 9;
					SetConsoleTextAttribute(Winptr, 15 * 16 + 9);
					break;
				}
				case 2: {
					system("cls");
					cout << Head_Str
					     << "【更多功能·设置当前窗口颜色·自定义·背景色】" << endl << endl;
					cout << "0.黑色    8.灰色" << endl
					     << "1.蓝色    9.淡蓝色" << endl
					     << "2.绿色    A.淡绿色" << endl
					     << "3.浅绿色  B.淡浅绿色" << endl
					     << "4.红色    C.淡红色" << endl
					     << "5.紫色    D.淡紫色" << endl
					     << "6.黄色    E.淡黄色" << endl
					     << "7.白色    F.亮白色" << endl
					     << endl;
					cout << ">>请选择背景色:" << endl;
					while (1) {
						char ch = getch();
						if (islower(ch)) {
							ch = toupper(ch);
						}
						if ((ch >= 48 && ch <= 57)) {
							_Backcolor = (ch - 48);
							SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Fontcolor);
							break;
						} else if (ch >= 65 && ch <= 70) {
							_Backcolor = (ch - 55);
							SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Fontcolor);
							break;
						}
					}

					system("cls");
					cout << Head_Str
					     << "【更多功能·设置当前窗口颜色·自定义·字体色】" << endl << endl;
					cout << "0.黑色    8.灰色" << endl
					     << "1.蓝色    9.淡蓝色" << endl
					     << "2.绿色    A.淡绿色" << endl
					     << "3.浅绿色  B.淡浅绿色" << endl
					     << "4.红色    C.淡红色" << endl
					     << "5.紫色    D.淡紫色" << endl
					     << "6.黄色    E.淡黄色" << endl
					     << "7.白色    F.亮白色" << endl
					     << endl;
					cout << ">>请选择字体色(异于背景色):" << endl;
					while (1) {
						char ch = getch();
						if (islower(ch)) {
							ch = toupper(ch);
						}
						if ((ch >= 48 && ch <= 57) && (ch - 48 != _Backcolor)) {
							_Fontcolor = (ch - 48);
							SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Fontcolor);
							break;
						} else if ((ch >= 65 && ch <= 70) && (ch - 55 != _Backcolor)) {
							_Fontcolor = (ch - 55);
							SetConsoleTextAttribute(Winptr, _Backcolor * 16 + _Fontcolor);
							break;
						}
					}
					break;
				}
				default: {
					break;
				}
			}
			break;
		}
		case 2: {
			system("cls");
			cout << Head_Str
			     << "【关于开发者·获取开源·彩蛋】" << endl;
			system("start https://JularDepick.github.io/Yuameng/Expand/Page/About_Designer_Page_for_UAV_MAS/");
			Sleep(500);
			cout << endl << "尝试访问:";
			string src = "https://JularDepick.github.io/Yuameng/Expand/Page/About_Designer_Page_for_UAV_MAS/";
			for (int i = 0; i < int(src.size()); i++) {
				cout << src[i];
				Sleep(10);
			}
			cout << endl;
			OE();
			break;
		}
		default: {
			break;
		}
	}
}

bool Lockor(string value) {
	return (value == Right);
}

string Locker(string value) {
	const static char Base64[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz&%"; //加密字典(基于Base64)
	string encode;
	vector<int> k;

	//字节展开求值,加入其他操作后对应加密字典
	for (int i = 0; i < int(value.size()); i++) {
		byte bt = value[i];  //byte类型包含在Windows头文件中(为了处理中文)
		int a = abs(int(bt));
		int s[8] = {};
		for (int i = a, pos = 7; i > 0; i /= 2, pos--) {
			//展开为二进制数
			s[pos] = i % 2;
		}
		for (int i = 0; i < 8; i++) {
			k.push_back(s[i]);
		}
	}

	for (int i = 0; i < int(k.size()); i++) {
		//按位取反
		k[i] = (!k[i]);
	}

	for (int i = 0, posb = 0; i < int(k.size()) / 6; i++) {
		//按六位匹配解析
		int l[6] = {};
		for (int posa = 0; posa < 6; posa++, posb++) {
			l[posa] = k[posb];
		}
		int ans = l[0] * 32 + l[1] * 16 + l[2] * 8 + l[3] * 4 + l[4] * 2 + l[5];
		encode.push_back(Base64[ans]);
	}
	if (int(k.size()) / 6 * 6 != int(k.size())) {
		//处理可能的剩余位
		int ls[6] = {};
		for (int i = int(k.size()) - 1, j = 5; i >= int(k.size()) / 6 * 6; j--, i--) {
			ls[j] = k[i];
		}
		int ans = ls[0] * 32 + ls[1] * 16 + ls[2] * 8 + ls[3] * 4 + ls[4] * 2 + ls[5];
		encode.push_back(Base64[ans]);
	}

	//拉锁式打乱
	string sa, sb;
	if (encode.size() % 2) {
		for (int i = 0; i < int(encode.size()); i++) {
			if (i <= int(encode.size()) / 2) {
				sa.push_back(encode[i]);
			} else {
				sb.push_back(encode[i]);
			}
		}
	} else {
		for (int i = 0; i < int(encode.size()); i++) {
			if (i < int(encode.size()) / 2) {
				sa.push_back(encode[i]);
			} else {
				sb.push_back(encode[i]);
			}
		}
	}
	encode.clear();
	for (int i = 0; i < int(sb.size()); i++) {
		encode.push_back(sa[i]);
		encode.push_back(sb[i]);
	}
	if (sa.size() > sb.size()) {
		encode.push_back(*(sa.end() - 1));
	}

	return encode;
}

string tostring(int a) {
	string s;
	for (; a > 0; a /= 10) {
		s.push_back(char(a % 10 + 48));
	}
	reverse(s.begin(), s.end());

	return s;
}

int GMr(int a, int b) {
	//GMr: Get_Modeler获取模式
	char Model;
	while (1) {
		fflush(stdin);
		Model = getch();
		if (Model >= a && Model <= b)
			break;
	}
	return int(Model - 48);
}

void OE() {
	//OE: Over_End执行结束
	cout << "\n\n>>回车或换行返回主菜单\n";
	while (1) {
		fflush(stdin);
		char a = getch();
		if (a == '\n' || a == '\r')
			break;
	}
}
