extern void UITalk(char* ,char*)

void ZwActiveTalk(int TalkID) {
	switch (TalkID) {
		case 0:
		default:
			UITalk(L"作者", L"这个对话还没做完");
			break;
		case 1:
			UITalk(L"勇者", L"这里是哪里?");
			UITalk(L"勇者", L"我为什么会在这里?");
			UITalk(L"勇者", L"这是发生了什么啊?");
			break;
		case 2:
			UITalk(L"村民", L"你醒了啊? 勇者。");
			UITalk(L"勇者", L"勇者? 是在说我吗？");
			UITalk(L"村民", L"对啊，预言说，在黑暗肆虐的时代，会有一位勇者从天而降，征服恶龙，讨伐魔王。");
			UITalk(L"村民", L"而你是从天而降的，那么你就是会带领着我们打倒魔王的那位勇者。");
			UITalk(L"勇者", L"（一头雾水）这都什么和什么啊！");
			break;
		case 3:
			UITalk(L"村民", L"这么快就要出发了吗？");
			UITalk(L"勇者", L"似乎是吧。");
			UITalk(L"村民", L"勇者，要加油啊！");
			break;
		case 4:
			UITalk(L"士兵", L"你就是勇者！？");
			UITalk(L"士兵", L"此前讨伐魔王的军队被击败了。");
			UITalk(L"士兵", L"现在我们的希望都在你身上。");
			UITalk(L"勇者", L"行吧。");
			break;
		case 5:
			UITalk(L"门卫", L"想要进入王宫？");
			UITalk(L"门卫", L"先过我这关！");
			UITalk(L"勇者", L"放马过来吧！");
			UITalk(L"门卫", L"（被勇者的气势折服）我就是个门卫，拼什么命啊，进去吧。");
			UITalk(L"勇者", L"？这就过去了吗？");
			break;
		case 6:
			UITalk(L"魔王", L"你终于来了，勇者。");
			UITalk(L"勇者", L"。。。");
			UITalk(L"魔王", L"。。。");
			UITalk(L"魔王", L"废话不多说了，开战吧。");
			UITalk(L"作者", L"<鉴于技术力限制，战斗功能没做>");
			UITalk(L"作者", L"就当是勇者赢了吧。");
			UITalk(L"勇者", L"看招！");
			UITalk(L"魔王", L"呵！你居然击败了我！");
			break;
	}
}