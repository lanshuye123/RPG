#include "Game.h"

// 0: �л���ͼ
// 1: �����Ի�
// 2: ����ս��

//����Ի�
void ActiveTalk(int TalkID) {
	switch (TalkID) {
		case 0:
		default:
			UITalk(L"����", L"����Ի���û����");
			break;
		case 1:
			UITalk(L"����", L"����������?");
			UITalk(L"����", L"��Ϊʲô��������?");
			UITalk(L"����", L"���Ƿ�����ʲô��?");
			break;
		case 2:
			UITalk(L"����", L"�����˰�? ���ߡ�");
			UITalk(L"����", L"����? ����˵����");
			UITalk(L"����", L"�԰���Ԥ��˵���ںڰ���Ű��ʱ��������һλ���ߴ�������������������ַ�ħ����");
			UITalk(L"����", L"�����Ǵ�������ģ���ô����ǻ���������Ǵ�ħ������λ���ߡ�");
			UITalk(L"����", L"��һͷ��ˮ���ⶼʲô��ʲô����");
			break;
		case 3:
			UITalk(L"����", L"��ô���Ҫ��������");
			UITalk(L"����", L"�ƺ��ǰɡ�");
			UITalk(L"����", L"���ߣ�Ҫ���Ͱ���");
			break;
		case 4:
			UITalk(L"ʿ��", L"��������ߣ���");
			UITalk(L"ʿ��", L"��ǰ�ַ�ħ���ľ��ӱ������ˡ�");
			UITalk(L"ʿ��", L"�������ǵ�ϣ�����������ϡ�");
			UITalk(L"����", L"�аɡ�");
			break;
		case 5:
			UITalk(L"����", L"��Ҫ����������");
			UITalk(L"����", L"�ȹ�����أ�");
			UITalk(L"����", L"��������ɣ�");
			UITalk(L"����", L"�������ߵ������۷����Ҿ��Ǹ�������ƴʲô��������ȥ�ɡ�");
			UITalk(L"����", L"����͹�ȥ����");
			break;
		case 6:
			UITalk(L"ħ��", L"���������ˣ����ߡ�");
			UITalk(L"����", L"������");
			UITalk(L"ħ��", L"������");
			UITalk(L"ħ��", L"�ϻ�����˵�ˣ���ս�ɡ�");
			UITalk(L"����", L"<���ڼ��������ƣ�ս������û��>");
			UITalk(L"����", L"�͵�������Ӯ�˰ɡ�");
			UITalk(L"����", L"���У�");
			UITalk(L"ħ��", L"�ǣ����Ȼ�������ң�");
			break;
	}
}

void ActiveEvent(GameEvent* Ev, GameData* GD) {
	switch (Ev->EventCode) {
		case 0:
			int fv1, fv2, fv3;
			fv1 = (Ev->Flags >> 16) & 0xFFFF;
			fv2 = (Ev->Flags >> 8) & 0xFF;
			fv3 = (Ev->Flags) & 0xFF;
			GD->Mapid = fv1;
			GD->PlayerPos.X = fv2;
			GD->PlayerPos.Y = fv3;
			break;
		case 1:
			ActiveTalk(Ev->Flags);
			break;
		case 2:

			break;
	}
}