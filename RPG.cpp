#include "Game.h"

#define GameName "RPG游戏"

GameData GD;

LRESULT CALLBACK SubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDPROC DefaultWinProc = nullptr;

jmp_buf jmp_UITitle;

signed main() {
	//Copyright
	//printf("程序作者:纳米2班 姜春旺\r\n\r\n");

	// BASIC Help
	printf("游戏基本介绍:\r\n");
	printf("游戏内通过WSAD或上下左右箭头进行移动，\r\n");
	printf("游戏内通过回车键(Enter)与人物、事件进行互动，\r\n");
	printf("游戏内按ESC键打开游戏内菜单\r\n");
	printf("按回车键开始游戏\r\n");

	(void)getchar();

	HWND GameUI = initgraph(frameW, frameH);
	SetWindowText(GameUI, TEXT("RPG游戏"));
	BASS_Init(-1, 44100, 0, GameUI, 0);
	DefaultWinProc = (WNDPROC)GetWindowLongPtr(GameUI, GWLP_WNDPROC);
	SetWindowLongPtr(GameUI, GWLP_WNDPROC, (LONG_PTR)SubclassProc);

	_setjmp(jmp_UITitle);

	setbkmode(TRANSPARENT);
	settextstyle(32, 0, _T("思源宋体"));
	int select = UITitle();
	GDDefaultGenerator(&GD);
	if (select == 1) {
		bool success = false;
		UILoad(&GD, &success);
		if (!success) {
			longjmp(jmp_UITitle,0);
		}
		//IOLoad(&GD, "SAVE000.SAV");
	}
	GameRender(&GD);
	closegraph();
	BASS_Free();
}

static LRESULT CALLBACK SubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// 如果是 WM_CLOSE 消息，则执行关闭操作
	if (uMsg == WM_CLOSE) {
		printf("\r\n游戏窗口被手动关闭，游戏程序即将退出...\r\n");
		PostQuitMessage(0);
		exit(0);
	}
	return CallWindowProc(DefaultWinProc, hwnd, uMsg, wParam, lParam);
}