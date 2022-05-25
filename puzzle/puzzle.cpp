#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <ctime>
#include <conio.h>

static int g_nScreenIndex;
static HANDLE g_hScreen[2];
int g_numofFPS;
clock_t CurTime, OldTime;
ULONGLONG newTime, prevTime;
char* FPSTextInfo;


void ScreenInit()
{
	// 이 함수를 실행 시켜야 하는 이유는 
	// 게임에서 만들어내는 메세지를 다 표현 및 표시 해주기 위해 초기 설정으로 셋팅한다.
	system("mode con:cols=100 lines=45");

	CONSOLE_CURSOR_INFO cci;

	// 화면 버퍼 2개를 만든다.
	g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// 커서를 숨긴다.
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(g_hScreen[0], &cci);
	SetConsoleCursorInfo(g_hScreen[1], &cci);
}

void ScreenFlipping()
{
	SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
	g_nScreenIndex = !g_nScreenIndex;
}

void ScreenClear()
{
	COORD Coor = { 0, 0 };
	DWORD dw;

	// 80 * 50 은 화면을 다 지우고 다시 출력하는 크기이다.
	FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', 80 * 50, Coor, &dw);
}

void ScreenRelease()
{
	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);
}

void ScreenPrint(int x, int y, char* string)
{
	DWORD dw;
	COORD CursorPosition = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);
}



char c;

void KeyInut()
{
	if (_kbhit())  //키보드 입력 여부 확인
	{
		c = _getch(); //kbhit 함수 내용 입력을 기다렸다가 출력해줌.
	}
}


void Update(double StartTime) //게임 업데이트 되는 데이터 모두 처리
{
	KeyInut();
}

clock_t UpdateCurTime;
clock_t UpdateOldTime;
char GameText[256 * 256];

const int PUZZLE_WIDTH = 9;
const int PUZZLE_HEIGHT = 9;
class puzzleData
{
	puzzleData()
	{
		for (int x = 0; x < PUZZLE_WIDTH; ++x)
		{
			for (int y = 0; y < PUZZLE_HEIGHT; ++y)
			{
				//■
				tile[x][y] = '□';
			}
		}
	}
	char tile[PUZZLE_WIDTH][PUZZLE_HEIGHT];
};

void Render(double StartTime)
{
	ScreenClear();

	if (CurTime - OldTime >= 1000)
	{
		sprintf(FPSTextInfo, "FPS : %d", g_numofFPS);

		// 게임에 사용하는 텍스트를 매번 리셋해서 사용한다.(텍스트가 더럽혀 진다고...??? -_-)
		memset(GameText, 0, 256 * 256);  //memset 함수: 메모리의 내용값을 원하는 특정값으로 세팅할 수 있음. 메모리를 초기화 해줌.
		char tempString[256] = { 0, };
		sprintf(GameText, "게임정보\n");

		OldTime = CurTime; //일반적으로 FPS가 60 아래로 나오면 게임 플레이가 원활하지 못함. 적절히 조절
		g_numofFPS = 0;
	}



	g_numofFPS++;
	ScreenPrint(0, 0, FPSTextInfo);
	ScreenPrint(0, 1, GameText);

	ScreenFlipping();
}

void Release()
{
	delete[] FPSTextInfo;
}

int main()
{
	// 현재시간- 이전에 출력한 시간  
	int differentTime = 0;
	int elapedTime = 0;
	//프로그램이 동작한 횟수 
	newTime = GetTickCount64();
	prevTime = GetTickCount64();

	g_numofFPS = 0;
	FPSTextInfo = new char[128];
	memset(FPSTextInfo, '\0', 128);

	ScreenInit();

	srand(time(NULL));

	UpdateOldTime = OldTime = clock(); // 시간을 측정한다. 1초마다 갱신한다.

	double start = 0.0f;
	double end = 0.0f;
	start = (double)clock() / CLOCKS_PER_SEC;

	while (1)
	{
		// 지금 시간을 받아온다
		newTime = GetTickCount64();
		// 지나간 시간 = 지금 시간 - 이전에 화면을 출력한 시간
		elapedTime = newTime - prevTime;

		// 화면을 출력한 시간 = 지금 시간
		prevTime = newTime;

		UpdateCurTime = CurTime = clock();

		int GamePlayTimeSecond = (end - start);
		int GamePlayTimeMin = GamePlayTimeSecond / 60;

		// 분단위 종료 코드
		bool GameEndTime = (15 <= GamePlayTimeMin);

		if (GameEndTime)
		{
			// 게임 종료
		}
		else
		{
			end = (((double)clock()) / CLOCKS_PER_SEC);

			// 게임 중
			//이 코드가 없으면 일정하게 비율증가나 게임진행을 하기 어렵다.
			float TargetFrame = 1000.0f / 60.0f;
			if (UpdateCurTime - UpdateOldTime >= TargetFrame) // 1초=1000이다. 최소 TargetFrame은 60프레임이다. 1초에 60번 규칙적으로 업데이트하기 위해 씀
			{
				UpdateOldTime = UpdateCurTime;
				Update(start);
			}
		}


		Render(start);
	}
	Release();
	ScreenRelease();

	return 0;
}