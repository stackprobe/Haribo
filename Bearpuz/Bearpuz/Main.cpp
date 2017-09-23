#include "all.h"

static void Logo(void)
{
	int frmidx;
	double frmpct;

	frameloop(frmidx, frmpct, 40)
	{
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);

		SetAlpha(frmpct);
		SimpleDraw(Dc->PicLogo, 0, 0, 0);
		ResetAlpha();

		SwapFrame();
	}
//	PlaySe(Dc->SeRucca);

	frameloop(frmidx, frmpct, 80)
	{
		SimpleDraw(Dc->PicLogo, 0, 0, 0);
		SwapFrame();
	}
	frameloop(frmidx, frmpct, 40)
	{
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);

		SetAlpha(1.0 - frmpct);
		SimpleDraw(Dc->PicLogo, 0, 0, 0);
		ResetAlpha();

		SwapFrame();
	}
}

typedef struct PT_Info_st
{
	double X;
	double Y;
	double Rot;
	double Zoom;
	int Color;

	double CurrX;
	double CurrY;
	double CurrRot;
	double CurrZoom;
	int SleepFrmCnt;
	double A;

	int FrmCnt;
}
PT_Info_t;

#define PT_FADEIN_FRMCNT 10
#define PT_CRASH_FRMCNT 45

static int PT_Disable;

static int PT_Task(PT_Info_t *i)
{
	if(PT_Disable)
		return 0;

	if(i->SleepFrmCnt)
	{
		m_countDown(i->SleepFrmCnt);
		return 1;
	}
	int currFrm = i->FrmCnt;
	i->FrmCnt++;

	nearize_adj(i->CurrX, i->X, 0.9, 0.0);
	nearize_adj(i->CurrY, i->Y, 0.9, 0.0);
	nearize_adj(i->CurrRot, i->Rot, 0.9, 0.0);
	nearize_adj(i->CurrZoom, i->Zoom, 0.9, 0.0);

	SetAntiAlias(1);

	if(currFrm < PT_FADEIN_FRMCNT)
		SetAlpha(currFrm / (double)PT_FADEIN_FRMCNT);
	else
		SetAlpha(i->A);

	DppInit(Dc->PicBlocks[i->Color], i->CurrX, i->CurrY, 1);
	DppRotate(i->CurrRot);
	DppZoom(i->CurrZoom);
	DppDraw();

	ResetAlpha();
	SetAntiAlias(0);

	if(PT_CRASH_FRMCNT <= currFrm)
	{
		if(currFrm == PT_CRASH_FRMCNT)
		{
			nearize_adj(i->CurrX, i->X, 0.6, 0.0);
			nearize_adj(i->CurrY, i->Y, 0.6, 0.0);
			nearize_adj(i->CurrRot, i->Rot, 0.6, 0.0);

			for(int c = 0; c < 9; c++)
			{
				double idoR = rnd() * 2.0 * PI;

				CommonEffect_t *ce = AddCommonEffect(
					Dc->PicFlowers[i->Color],
					0,
					i->X,
					i->Y,
					rnd() * 2.0 * PI,
					1.0,
					0.8,
					cos(idoR) * 9.0,
					sin(idoR) * 9.0,
					rndpm() * 0.05,
					0.0,
					0.0,
					0.0,
					0.3
					);

				ce->LifeCount = 60;
			}
			for(int c = 0; c < 7; c++)
			{
				double idoR = rnd() * 2.0 * PI;

				CommonEffect_t *ce = AddCommonEffect(
					Dc->PicFlowers[i->Color],
					0,
					i->X,
					i->Y,
					rnd() * 2.0 * PI,
					3.0,
					0.6,
					cos(idoR) * 6.0,
					sin(idoR) * 6.0,
					rndpm() * 0.05
					);

				ce->LifeCount = 30;
				ce->KasokuRate = 0.97;
			}
			for(int c = 0; c < 10; c++)
			{
				double idoR = rnd() * 2.0 * PI;

				CommonEffect_t *ce = AddCommonEffect(
					Dc->PicEffeHoshi,
					0,
					i->X,
					i->Y,
					rnd() * 2.0 * PI,
					0.1,
					0.9,
					cos(idoR) * 10.0,
					sin(idoR) * 10.0,
					rndpm() * 0.05,
					0.0,
					1.0 / -30
					);

				ce->KasokuRate = 0.997;
			}
		}
		i->A -= 1.0 / 150;
	}
	return 0.0 <= i->A;
}
static void PT_AddBlock(double x, double y, double r, double z, int color, int sfc)
{
	PT_Disable = 0;

	PT_Info_t *i = (PT_Info_t *)memAlloc(sizeof(PT_Info_t));

	memset(i, 0x00, sizeof(PT_Info_t));

	i->X = x;
	i->Y = y;
	i->Rot = r;
	i->Zoom = z;
	i->Color = color;

	i->CurrX = rnd() * 800.0;
	i->CurrY = rnd() * 600.0;
	i->CurrRot = (rnd() * 100.0 + 50.0) * rndp1m1();
	i->CurrZoom = rnd() * 10.0 + 10.0;
	i->SleepFrmCnt = sfc; // Get_Rnd(45);
	i->A = 1.0;

	AddFrameTask((int (*)(void *))PT_Task, (void *)i);
}
static void PreTitle_Orig(void)
{
	const int sfcnum = 9;
	int sfcList[sfcnum];

	for(int c = 0; c < sfcnum; c++)
		sfcList[c] = (int)((45.0 * c) / sfcnum);

	Shuffle(sfcList, sfcnum);

	PT_AddBlock(405.0, -10.0,  0.12, 1.00, 2, sfcList[0]);
	PT_AddBlock(308.5,  32.5, -0.51, 2.07, 1, sfcList[1]);
	PT_AddBlock(785.5, 115.0,  0.47, 2.50, 3, sfcList[2]);
	PT_AddBlock(703.5, 245.0,  0.47, 1.79, 5, sfcList[3]);
	PT_AddBlock(688.0, 518.5, -0.28, 1.80, 2, sfcList[4]);
	PT_AddBlock(200.0, 415.5, -0.40, 2.15, 2, sfcList[5]);
	PT_AddBlock(  0.0, 392.0,  0.47, 2.16, 4, sfcList[6]);
	PT_AddBlock( 87.0, 479.5,  0.00, 1.65, 5, sfcList[7]);
	PT_AddBlock( 40.5, 557.0, -0.43, 1.00, 4, sfcList[8]);

	int frmidx;
	double frmpct;

	frameloop(frmidx, frmpct, 90)
	{
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);
		SwapFrame();
	}
	double z = 1.2;

	frameloop(frmidx, frmpct, 30)
	{
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);

		SetAlpha(frmpct);
		SetAntiAlias(1);
		DppInit(GetPicTitle(), 400.0, 300.0, 1);
		DppZoom(z);
		DppDraw();
//		SimpleDraw(GetPicTitle(), 0, 0, 1);
		SetAntiAlias(0);
		ResetAlpha();

		SwapFrame();

		nearize(z, 1.0, 0.85);
	}
}
static void PreTitle(void)
{
	// TODO 何か入れたい。

	int frmidx;
	double frmpct;
	double z = 1.2;
	double x = 100.0;

	frameloop(frmidx, frmpct, 30)
	{
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);

		SetAlpha(frmpct);
		SetAntiAlias(1);
		DppInit(GetPicTitle(), 400.0 + x, 300.0, 1);
		DppZoom(z);
		DppDraw();
//		SimpleDraw(GetPicTitle(), 0, 0, 1);
		SetAntiAlias(0);
		ResetAlpha();

		SwapFrame();

		nearize(z, 1.0, 0.85);
		nearize(x, 0.0, 0.75);
	}
}

#define TITLE_ITEM_NUM 4

static int TitleIndex;
static char *DefaultScenario = "プロローグ";

static void Title(void)
{
	// [][CursorX, CursorY, MusX1, MusY1, MusX2, MuxY2]
	int locs[TITLE_ITEM_NUM][6] =
	{
		{ 260, 200, 50, 200, 250, 250, },
		{ 260, 260, 50, 260, 250, 310, },
		{ 260, 320, 50, 320, 250, 370, },
		{ 260, 380, 50, 380, 250, 430, },
	};
	int frmidx;
	double frmpct;

#ifdef CHEAT_MODE
restart:
#endif
	PreTitle();

	int cheat_scnrIndex = -1;
	int kirara_frmcnt = 0;

	double box_x = -400.0;
	double box_y = -300.0;
	double box_r = 0.0;
	double box_z = 1.0;

	for(; ; )
	{
		if(kirara_frmcnt) kirara_frmcnt--;

		int mx;
		int my;
		GetMusPos(&mx, &my);

		int currIndex = -1; // -1 - (TITLE_ITEM_NUM - 1)

		for(int i = 0; i < TITLE_ITEM_NUM; i++)
		{
			if(isinside(mx, my, locs[i][2], locs[i][3], locs[i][4], locs[i][5]))
			{
				currIndex = i;
			}
		}
//		int trueAfterInside = nearthan(((double)mx), ((double)my), 690.0, 290.0, 90.0); // True-After クリック位置 botsu
		int extraInside     = nearthan(((double)mx), ((double)my), 690.0, 490.0, 90.0); // EXTRA クリック位置

#ifdef CHEAT_MODE
		if(IsHit(KEY_INPUT_1))
		{
			cheat_scnrIndex++;
			cheat_scnrIndex %= Dc->ResrcScnrCount;

			DefaultScenario = Dc->ResrcScnrList[cheat_scnrIndex];
		}
		if(IsHit(KEY_INPUT_2))
		{
//			SetWindowSizeExtendRate(1.5);
			PlaySe(Dc->SeIkuyo);
		}
		if(IsHit(KEY_INPUT_3))
		{
//			SetWindowSizeExtendRate(1.6);
			PlaySe(Dc->SeRen7);
		}
		if(IsHit(KEY_INPUT_4))
		{
			// todo
//			Ending("E_IP");
			Ending("E_PJ");
		}
		if(IsRenda(KEY_INPUT_H)) m_incDenom(box_x, -2.0);
		if(IsRenda(KEY_INPUT_J)) m_incDenom(box_y, -2.0);
		if(IsRenda(KEY_INPUT_K)) m_incDenom(box_y,  2.0);
		if(IsRenda(KEY_INPUT_L)) m_incDenom(box_x,  2.0);
		if(IsRenda(KEY_INPUT_Y)) m_incDenom(box_r, -100.0);
		if(IsRenda(KEY_INPUT_U)) m_incDenom(box_r,  100.0);
		if(IsRenda(KEY_INPUT_I)) m_incDenom(box_z, -100.0);
		if(IsRenda(KEY_INPUT_O)) m_incDenom(box_z,  100.0);
		if(IsRenda(KEY_INPUT_P)) { box_x *= -1.0; box_y *= -1.0; }
		if(IsHit(KEY_INPUT_R)) goto restart;

		printfDx(
			"%03d,%03d Scnr:%s %d\n"
			"%0.6f %0.6f %0.6f %0.6f"
			,mx
			,my
			,DefaultScenario
			,CheckSoundMem(Dc->SeIkuyo)
			,box_x
			,box_y
			,box_r
			,box_z
			);
#endif
		int moveIndex = 0; // -1 - 1
		if(GetHit(INP_DIR_6))
		{
			SetMusPos(715, 280);
		}
		if(GetHit(INP_DIR_4) || GetHit(INP_DIR_8))
		{
			moveIndex = -1;
//			if(1 <= currIndex) moveIndex = -1;
		}
		if(GetHit(INP_DIR_2))
		{
			moveIndex = 1;
//			if(currIndex < TITLE_ITEM_NUM - 1) moveIndex = 1;
		}
		if(moveIndex)
		{
			currIndex += moveIndex;
			m_range(currIndex, 0, TITLE_ITEM_NUM - 1);

			SetMusPos(locs[currIndex][4] - 1, locs[currIndex][5] - 1);
		}
		int decide = GetClick() & 1 || GetHit(INP_ROT_DECIDE);

		if(decide)
		{
			if(extraInside && IsExtraReleased())
			{
				if(kirara_frmcnt == 0)
				{
					PzefAddStarPon(600.0, 400.0, 35);
					kirara_frmcnt = 35;
				}
				TestTaisen();
			}
			if(currIndex != -1)
			{
				TitleIndex = currIndex;
				break;
			}
		}

		SimpleDraw(GetPicTitle(), 0, 0, 0);

		if(
			0.0 <= box_x && box_x <= 800.0 ||
			0.0 <= box_y && box_y <= 600.0
			)
		{
			DppInit(Dc->PicBlocks[0], box_x, box_y, 1);
			DppRotate(box_r);
			DppZoom(box_z);
			DppDraw();
		}

		for(int i = 0; i < TITLE_ITEM_NUM; i++)
		{
			if(i == currIndex)
			{
				SimpleDraw(Dc->PicTitleCursor, locs[i][0], locs[i][1], 1);
			}
		}

#if 0 // _Orig
		// 'EXTRA'
		{
			double ex_x[5];
			double ex_y[5];
			double ex_z[5];

			for(int c = 0; c < 5; c++)
			{
				ex_x[c] = 15.0 * c - 30.0;
				ex_y[c] = sin(ProcFrmCnt / 30.0 - c / 5.0) * 10.0;
				ex_z[c] = 1.0;
			}
			if(extraInside)
			{
				for(int c = 0; c < 5; c++)
				{
					const double RATE = 1.5;

					ex_x[c] *= RATE;
					ex_y[c] *= RATE;
					ex_z[c] *= RATE;
				}
			}
			for(int c = 0; c < 5; c++)
			{
				ex_x[c] += 703.0;
				ex_y[c] += 245.0;

				SetAntiAlias(1);
				DppInit(Dc->PicAscii["EXTRA"[c]], ex_x[c], ex_y[c], 1);
				DppZoom(ex_z[c]);
				DppDraw();
				SetAntiAlias(0);
			}
		}
#endif

		SwapFrame();

		// test
		/*
		if(GetHit(INP_DIR_8))
		{
			PlaySe(Dc->SeIkuyo);
//			PlaySe(Dc->SeFlowerLot);
		}
		*/
	}
	PT_Disable = 1;

	frameloop(frmidx, frmpct, 20)
	{
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);

		SetAlpha(1.0 - frmpct);
		SimpleDraw(GetPicTitle(), 0, 0, 0);
		ResetAlpha();

		SwapFrame();
	}
}

static int GOS_ExitGame;
static int GOP_ExitGame;

static char *GameOneScenario(char *scnrTitle, int actionIndex) // ret : Grip, 最後に実行したシナリオ
{
	int reload = 0;
	static char rl_scnr[2048];
	int rl_aidx = 0;

	GOS_ExitGame = 0; // 2bs

	strcpy(rl_scnr, scnrTitle);
	rl_aidx = actionIndex;

restart:
	ScInit();

	Scdc->ScnrTitle = rl_scnr;
	Scdc->JumpActionIndex = rl_aidx;

	ScMain();
	GOS_ExitGame = Scdc->ReturnBackToTitle;

	reload = 0;
//	rl_scnr[0] = '\0'; // 消しちゃ駄目だろjk
	rl_aidx = 0;

	if(Scdc->DoReload)
	{
		reload = 1;
		strcpy(rl_scnr, SettInfo.SaveDataList[SLLoadIndex].Scenario);
		rl_aidx = SettInfo.SaveDataList[SLLoadIndex].ActionIndex;
	}
	ScFnlz();

	if(reload)
		goto restart;

	return rl_scnr;
}
static void GameOnePuzzle(char *puzzTitle)
{
	int maketa;
	int chuudan;

restart:
	maketa  = 0;
	chuudan = 0;

	PzInit();

	Scc_SetPuzzleParams(puzzTitle);

	PzMain();

	maketa  = PzdcRet.GameOver && PzdcRet.Winner == 1;
	chuudan = PzdcRet.GameOver == 0;

	PzFnlz();

	// init ret
	GOP_ExitGame = 0;

	if(PzdcRet.PauseSelExitApp)
	{
		GameTermination();
	}
	if(maketa)
	{
		int ribenji = 0;

		int suruX = 233;
		int suruY = 209;
		int shinaiX = 395;
		int shinaiY = 270;

		int suruXSize;
		int suruYSize;
		int shinaiXSize;
		int shinaiYSize;

		GetGraphSize(Dc->PicContinueSuru, &suruXSize, &suruYSize);
		GetGraphSize(Dc->PicContinueShinai, &shinaiXSize, &shinaiYSize);

		int suruPic   = Dc->PicDummy;
		int shinaiPic = Dc->PicDummy;

		Dc->BlackCurtain = 0.0;
		Dc->BCPct = 1.0;

		for(; ; )
		{
			int mx;
			int my;
			GetMusPos(&mx, &my);

			ribenji = 0;

			suruPic   = Dc->PicContinueSuru;
			shinaiPic = Dc->PicContinueShinai;

			int focused = 0;
			if(isinside(mx, my, suruX, suruY, suruX + suruXSize, suruY + suruYSize))
			{
				suruPic = Dc->PicContinueSuruFocus;
				ribenji = 1;
				focused = 1;
			}
			if(isinside(mx, my, shinaiX, shinaiY, shinaiX + shinaiXSize, shinaiY + shinaiYSize))
			{
				shinaiPic = Dc->PicContinueShinaiFocus;
				focused = 1;
			}

			SimpleDraw(Dc->PicBlackWall, 0, 0, 0);

			SimpleDraw(suruPic, suruX, suruY, 1);
			SimpleDraw(shinaiPic, shinaiX, shinaiY, 1);

			SwapFrame();

			int cStatus = GetClick();

			if(GetHit(INP_ROT_DECIDE) || cStatus == 1)
			{
				if(focused)
				{
					break;
				}
			}
			if(GetHit(INP_ROT_CANCEL) || cStatus == 2)
			{
				ribenji = 0;

				suruPic   = Dc->PicContinueSuru;
				shinaiPic = Dc->PicContinueShinai;

				break;
			}
			if(GetHit(INP_DIR_2) || GetHit(INP_DIR_4) || GetHit(INP_DIR_6) || GetHit(INP_DIR_8))
			{
				if(ribenji)
				{
					mx = shinaiX + shinaiXSize - 1;
					my = shinaiY + shinaiYSize - 1;
				}
				else
				{
					mx = suruX + suruXSize - 1;
					my = suruY + suruYSize - 1;
				}
				SetMusPos(mx, my);
			}
		}

		Dc->BlackCurtain = 1.0;
		while(Dc->BlackCurtain != Dc->BCPct)
		{
			SimpleDraw(Dc->PicBlackWall, 0, 0, 0);

			SimpleDraw(suruPic, suruX, suruY, 1);
			SimpleDraw(shinaiPic, shinaiX, shinaiY, 1);

			SwapFrame();
		}
		Dc->BlackCurtain = 0.0;
		Dc->BCPct = 0.0;

		if(ribenji)
			goto restart;

		GOP_ExitGame = 1;
	}
	if(chuudan)
	{
		GOP_ExitGame = 1;
	}
}

static int GS_ReturnToLogo;

static void GameScenario(char *scnrTitle, int actionIndex)
{
	int currIndex;

	// init ret
	GS_ReturnToLogo = 0;

restart:
	scnrTitle = GameOneScenario(scnrTitle, actionIndex);

	if(GOS_ExitGame)
	{
		return;
	}
	currIndex = Scc_GetEventIndex(scnrTitle) + 1;

	for(; ; )
	{
		char *eventLine = Scc_GetEvent(currIndex);

		if(eventLine[0] == '\0' || eventLine[0] == ';') // 空行 || コメント
		{
			1; // noop
		}
		else if(eventLine[0] == 'B') // 分岐
		{
			char lnum[30];

			sprintf(lnum, "%d", ScProcDc.RouteSelected);
			currIndex = Scc_GetEventIndex(lnum);
		}
		else if(eventLine[0] == 'P') // パズル
		{
			GameOnePuzzle(eventLine);

			if(GOP_ExitGame)
			{
				return;
			}
		}
		else if(eventLine[0] == 'E') // エンディング
		{
			Ending(eventLine);
		}
		else if(eventLine[0] == 'R') // ルート開放
		{
			// _Orig
			/*
			     if(!strcmp(eventLine, "R_Route_A")) SettInfo.RouteOpenList[ROL_A] = 1;
			else if(!strcmp(eventLine, "R_Route_B")) SettInfo.RouteOpenList[ROL_B] = 1;
			else if(!strcmp(eventLine, "R_Route_C")) SettInfo.RouteOpenList[ROL_C] = 1;
			else                                     error();
			*/

			SaveSettings();
		}
		else if(eventLine[0] == 'L') // Return to Logo
		{
			GS_ReturnToLogo = 1;
			return;
		}
		else // シナリオ
		{
			scnrTitle = eventLine;
			actionIndex = 0;
			goto restart;
		}
		currIndex++;
	}
}
static void Bearpuz(void)
{
	SetBGMVolume(Dc->BGMVolume);
	SetSeVolume(Dc->SeVolume);

start_logo:
	DispMusCursor(false);
//	Logo(); // 廃止
	PlayDouga(ResourcePath("opening.ogv"));
//	PlayMovie(ResourcePath("opening.ogv"), 1, DX_MOVIEPLAYTYPE_BCANCEL); // old
	DispMusCursor(true);

start_title:
	PlayBGM(Dc->BGM_Title);
	Title();

	switch(TitleIndex)
	{
	case 0:
		GameScenario(DefaultScenario, 0);
		goto endGameScenrio;

	case 1:
		{
			SLSaveMode = 0;
			SLSaveFunc = NULL;

			SaveLoad();

			if(SLDoLoad)
			{
				GameScenario(
					SettInfo.SaveDataList[SLLoadIndex].Scenario,
					SettInfo.SaveDataList[SLLoadIndex].ActionIndex
					);
endGameScenrio:
				if(GS_ReturnToLogo)
				{
					goto start_logo;
				}
			}
		}
		goto start_title;

	case 2:
		CfgMain();
		goto start_title;

	case 3:
		goto end_program;

	default:
		error();
	}
end_program:;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if(!ProcMutexLock())
		return 0;

	GndInit();

	char *ltmp;

	SetOutApplicationLogValidFlag(false);
	SetDXArchiveKeyString(ltmp = xcout("%c%s-%d", 'B', "earpuz", 8493)); // "Bearpuz-8493"
	memFree(ltmp);

	InitSettings();
	LoadSettings();

	SetGraphMode(800, 600, 32);
//	SetWindowSizeExtendRate(1.6); // 拡大
	SetAlwaysRunFlag(true); // フォーカス外したら、true: 動く, false: 止まる。
	ChangeWindowMode(SettInfo.FullScreenMode == 0); // true: Window, false: Full Screen

	SetMainWindowText(ltmp = xcout("Bearpuz blt: %s", GetBuiltStamp()));
	memFree(ltmp);

	DispMusCursor(true); // これ必要？

	SetWindowIconID(101); // ウィンドウ左上のアイコン

	if(DxLib_Init())
	{
		return 1;
	}
	Init_Rnd((int)time(NULL));

	DispMusCursor(true); // 再設定？、ここで設定しないと出ないっぽい？

	SetDrawScreen(DX_SCREEN_BACK); // これをやらないと 60 hz にならなず、何故か 43 hz くらいになる。
	SetWaitVSyncFlag(true);

	/*
		ウィンドウの右下をドラッグで、ウィンドウ伸縮 true: 可, false: 不可
		可にすると SetWindowSizeExtendRate() の最大サイズに影響する。
	*/
//	SetWindowSizeChangeEnableFlag(true); // -> ScreenZoomControl()

	// ---- クレジット_表示 ----

	ClearDrawScreen();
	int loadchuu = LoadGraph(ResourcePath("画像\\ロゴ画面.png"));
	errorCase(loadchuu == -1);

	int frmidx;
	double frmpct;

	frameloop(frmidx, frmpct, 10)
	{
		Pub_ScreenZoomControl();
		ScreenFlip();
	}
	frameloop(frmidx, frmpct, 40)
	{
//		SimpleDraw(Dc->PicBlackWall, 0, 0, 0); // 未ロード
		ClearDrawScreen();

		SetAlpha(frmpct);
		DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, loadchuu, 0);
		ResetAlpha();

//		SwapFrame(); // たぶんまだ使えない。
		Pub_ScreenZoomControl();
		ScreenFlip();
	}
	clsDx();
	printfDx("[DEBUG]ロード中");
	ScreenFlip();

	uint64 logoStartedTime = GetTickCount64();

	// ----

	DcInit();
	SettInfo2Other();

	GC_ProcInit();
	ScProcInit();
	ScRotSenProcInit();
	Scc_ProcInit();
	NtProcInit();
	Taisen_ProcInit();

	// ---- クレジット_消す ----

	clsDx();

	while(GetTickCount() < logoStartedTime + 2000)
//	frameloop(frmidx, frmpct, 10)
	{
		SwapFrame();
	}
	frameloop(frmidx, frmpct, 40)
	{
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);

		SetAlpha(1.0 - frmpct);
		SimpleDraw(loadchuu, 0, 0, 0);
		ResetAlpha();

		SwapFrame();
	}
	DeleteGraph(loadchuu);

	frameloop(frmidx, frmpct, 10)
	{
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);
		SwapFrame();
	}

	// ----

	Bearpuz();

	Taisen_ProcFnlz();
	NtProcFnlz();
	Scc_ProcFnlz();
	ScRotSenProcFnlz();
	ScProcFnlz();
	GC_ProcFnlz();

	DcFnlz();

	termination(0);
	return 0; // dummy
}
