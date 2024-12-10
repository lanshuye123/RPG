#include "Game.h"

#include <Windows.h>

#define GameName "RPG：勇者赞歌"

struct GameData GD;

LRESULT CALLBACK SubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDPROC DefaultWinProc = nullptr;

signed main() {
	//Copyright
	printf("程序作者:纳米2班 姜春旺\r\n");
	HWND GameUI = initgraph(frameW, frameH);
	SetWindowText(GameUI, TEXT("R.P.G 勇者赞歌 作者:纳米2班姜春旺"));

	DefaultWinProc = (WNDPROC)GetWindowLongPtr(GameUI, GWLP_WNDPROC);
	SetWindowLongPtr(GameUI, GWLP_WNDPROC, (LONG_PTR)SubclassProc);

	setbkmode(TRANSPARENT);
	settextstyle(32, 0, _T("思源宋体"));
	int select = UITitle();
	while (select == 1) {
		UIAlert(TEXT("\r\n[提示]\r\n读档功能尚未制作"));
		select = UITitle();
	}
	GDDefaultGenerator(&GD);
	GameRender(&GD);
}

//生成默认游戏数据
void GDDefaultGenerator(GameData* GD) {
	GD->Mapid = 0;
	GD->Player.Level = 1;
	GD->PlayerPos = { 0,0 };
	GD->Player.HP = 100;
	GD->Player.MP = 100;
	GD->Player.Item.Weapon = 0;
	GD->Player.Item.Wear[0] = 1;
	GD->Player.Item.Wear[1] = 2;
	GD->Player.Item.Wear[2] = 3;
	GD->Player.Item.Wear[3] = 4;
}

LRESULT CALLBACK SubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// 如果是 WM_CLOSE 消息，则执行特定操作
	if (uMsg == WM_CLOSE) {
		printf("\r\n游戏窗口被手动关闭，游戏程序即将退出...\r\n");
		PostQuitMessage(0);
		exit(0);
	}
	return CallWindowProc(DefaultWinProc, hwnd, uMsg, wParam, lParam);
}