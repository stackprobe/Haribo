#define SC_CMD_MAX 32
#define SC_ACT_MAX 1024

enum
{
	SC_CMD_DISP_WALL,
	SC_CMD_DISP_CHARA,
	SC_CMD_EFFECT_CHARA,
	SC_CMD_PLAY_BGM,
	SC_CMD_PLAY_SE,
	SC_CMD_INTERVAL,
	SC_CMD_WHITEOUT,
	SC_CMD_ROUTE_SENTAKU,
};

typedef struct ScCommand_st
{
	char *LCommand; // not ""
	int Command;
	int PosNo;   // -1: no-data, 0 - 3: 立ち位置
	char *Extra; // not ""
}
ScCommand_t;

typedef struct ScAction_st
{
	ScCommand_t CmdLst[SC_CMD_MAX];
	int CmdCnt; // 0〜

	char *CharaName; // "" あり, 
	char *Message;   // "" あり, 改行は "\n" とする。
}
ScAction_t;

typedef struct ScScenario_st
{
	ScAction_t ActLst[SC_ACT_MAX];
	int ActCnt; // 1〜
}
ScScenario_t;

void ScLoadScenario(char *file);
void ScUnloadScenario(void);
