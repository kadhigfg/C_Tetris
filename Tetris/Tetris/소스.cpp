#include <Windows.h>
#include <time.h>
#include <stdio.h>

#define MapHeight 30
#define MapWidth 20
#define BlockAni 4
#define BlockType 7

#pragma region Enum
enum Color
{
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHTGRAY,
	DARKGRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE,

};
enum Direction
{
	Dir_LEFT,
	Dir_RIGHT,
	Dir_DOWN,
};
#pragma endregion

#pragma region Struct
typedef struct Obj
{
	bool act;
	int x;
	int y;
	int blockShape;
	int rotationNum;
	Color color;
	Direction dir;
	const char* shape[BlockType][BlockAni][4];
}*Player;
struct Line
{
	Color color;
};
#pragma endregion

#pragma region Map
char map[MapHeight][MapWidth] =
{
	{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1}, // x = 20, y = 2
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1}, // x 0 ~ 20
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1}, // y 0 ~ 30
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	//                 0
};

#pragma endregion

#pragma region DoubleBuffer
#define BufferWidth 100
#define BufferHeight 60

HANDLE hBuffer[2];
int screenIndex;

void InitBuffer();
void WriteBuffer(int x, int y, const char* shape, int color);
void FlipBuffer();
void ClearBuffer();
void DestroyBuffer();
#pragma endregion

#pragma region Game
void Initialize();
void Progress();
void Render();
void Release();
#pragma endregion

void Collision(Obj* obj, Direction dir);
void RemoveLine();
void RotationBlock(Obj* obj);
void CreatNewBlock();
bool CheckBlock1(Obj* obj, int blockShape);
void ChangeColor(Obj* obj, int blockShape);
void StackBlock(Obj* obj, int blockShape);
void ExpectedLocation();

Player player[3];
int timeCount = 0;
int stackCount = 0;

int main()
{
	srand((unsigned)time(NULL));

	InitBuffer();
	Initialize();

	int tickCount = GetTickCount64();

	while (true)
	{
		if (GetTickCount64() >= tickCount + 40)
		{
			Progress();
			Render();

			FlipBuffer();
			ClearBuffer();

			tickCount = GetTickCount64();
		}

		if (GetAsyncKeyState(VK_RETURN)) break;
		
	}

	Release();
	DestroyBuffer();

	return 0;

}

#pragma region Game
void Initialize()
{
	for (int i = 0; i < 3; i++)
	{
		player[i] = (Player)malloc(sizeof(Obj));
		player[i]->rotationNum = 0;
		player[i]->blockShape = rand() % 7;

		{
			player[i]->shape[0][0][0] = "　　■　";
			player[i]->shape[0][0][1] = "　■■■";
			player[i]->shape[0][0][2] = "　　　　";
			player[i]->shape[0][0][3] = "　　　　";

			player[i]->shape[0][1][0] = "　　■　";
			player[i]->shape[0][1][1] = "　　■■";
			player[i]->shape[0][1][2] = "　　■　";
			player[i]->shape[0][1][3] = "　　　　";

			player[i]->shape[0][2][0] = "　　　　";
			player[i]->shape[0][2][1] = "　■■■";
			player[i]->shape[0][2][2] = "　　■　";
			player[i]->shape[0][2][3] = "　　　　";

			player[i]->shape[0][3][0] = "　　■　";
			player[i]->shape[0][3][1] = "　■■　";
			player[i]->shape[0][3][2] = "　　■　";
			player[i]->shape[0][3][3] = "　　　　";



			player[i]->shape[1][0][0] = "　■　　";
			player[i]->shape[1][0][1] = "　■■■";
			player[i]->shape[1][0][2] = "　　　　";
			player[i]->shape[1][0][3] = "　　　　";

			player[i]->shape[1][1][0] = "　　■■";
			player[i]->shape[1][1][1] = "　　■　";
			player[i]->shape[1][1][2] = "　　■　";
			player[i]->shape[1][1][3] = "　　　　";

			player[i]->shape[1][2][0] = "　　　　";
			player[i]->shape[1][2][1] = "　■■■";
			player[i]->shape[1][2][2] = "　　　■";
			player[i]->shape[1][2][3] = "　　　　";

			player[i]->shape[1][3][0] = "　　■　";
			player[i]->shape[1][3][1] = "　　■　";
			player[i]->shape[1][3][2] = "　■■　";
			player[i]->shape[1][3][3] = "　　　　";



			player[i]->shape[2][0][0] = "　　　■";
			player[i]->shape[2][0][1] = "　■■■";
			player[i]->shape[2][0][2] = "　　　　";
			player[i]->shape[2][0][3] = "　　　　";

			player[i]->shape[2][1][0] = "　　■　";
			player[i]->shape[2][1][1] = "　　■　";
			player[i]->shape[2][1][2] = "　　■■";
			player[i]->shape[2][1][3] = "　　　　";

			player[i]->shape[2][2][0] = "　　　　";
			player[i]->shape[2][2][1] = "　■■■";
			player[i]->shape[2][2][2] = "　■　　";
			player[i]->shape[2][2][3] = "　　　　";

			player[i]->shape[2][3][0] = "　■■　";
			player[i]->shape[2][3][1] = "　　■　";
			player[i]->shape[2][3][2] = "　　■　";
			player[i]->shape[2][3][3] = "　　　　";



			player[i]->shape[3][0][0] = "　　　　";
			player[i]->shape[3][0][1] = "　　■■";
			player[i]->shape[3][0][2] = "　■■　";
			player[i]->shape[3][0][3] = "　　　　";

			player[i]->shape[3][1][0] = "　　■　";
			player[i]->shape[3][1][1] = "　　■■";
			player[i]->shape[3][1][2] = "　　　■";
			player[i]->shape[3][1][3] = "　　　　";

			player[i]->shape[3][2][0] = "　　　　";
			player[i]->shape[3][2][1] = "　　■■";
			player[i]->shape[3][2][2] = "　■■　";
			player[i]->shape[3][2][3] = "　　　　";

			player[i]->shape[3][3][0] = "　　■　";
			player[i]->shape[3][3][1] = "　　■■";
			player[i]->shape[3][3][2] = "　　　■";
			player[i]->shape[3][3][3] = "　　　　";



			player[i]->shape[4][0][0] = "　　　　";
			player[i]->shape[4][0][1] = "　■■　";
			player[i]->shape[4][0][2] = "　　■■";
			player[i]->shape[4][0][3] = "　　　　";

			player[i]->shape[4][1][0] = "　　　■";
			player[i]->shape[4][1][1] = "　　■■";
			player[i]->shape[4][1][2] = "　　■　";
			player[i]->shape[4][1][3] = "　　　　";

			player[i]->shape[4][2][0] = "　　　　";
			player[i]->shape[4][2][1] = "　■■　";
			player[i]->shape[4][2][2] = "　　■■";
			player[i]->shape[4][2][3] = "　　　　";

			player[i]->shape[4][3][0] = "　　　■";
			player[i]->shape[4][3][1] = "　　■■";
			player[i]->shape[4][3][2] = "　　■　";
			player[i]->shape[4][3][3] = "　　　　";



			player[i]->shape[5][0][0] = "　　　　";
			player[i]->shape[5][0][1] = "　■■　";
			player[i]->shape[5][0][2] = "　■■　";
			player[i]->shape[5][0][3] = "　　　　";

			player[i]->shape[5][1][0] = "　　　　";
			player[i]->shape[5][1][1] = "　■■　";
			player[i]->shape[5][1][2] = "　■■　";
			player[i]->shape[5][1][3] = "　　　　";

			player[i]->shape[5][2][0] = "　　　　";
			player[i]->shape[5][2][1] = "　■■　";
			player[i]->shape[5][2][2] = "　■■　";
			player[i]->shape[5][2][3] = "　　　　";

			player[i]->shape[5][3][0] = "　　　　";
			player[i]->shape[5][3][1] = "　■■　";
			player[i]->shape[5][3][2] = "　■■　";
			player[i]->shape[5][3][3] = "　　　　";



			player[i]->shape[6][0][0] = "　　　　";
			player[i]->shape[6][0][1] = "■■■■";
			player[i]->shape[6][0][2] = "　　　　";
			player[i]->shape[6][0][3] = "　　　　";

			player[i]->shape[6][1][0] = "　　■　";
			player[i]->shape[6][1][1] = "　　■　";
			player[i]->shape[6][1][2] = "　　■　";
			player[i]->shape[6][1][3] = "　　■　";

			player[i]->shape[6][2][0] = "　　　　";
			player[i]->shape[6][2][1] = "■■■■";
			player[i]->shape[6][2][2] = "　　　　";
			player[i]->shape[6][2][3] = "　　　　";

			player[i]->shape[6][3][0] = "　　■　";
			player[i]->shape[6][3][1] = "　　■　";
			player[i]->shape[6][3][2] = "　　■　";
			player[i]->shape[6][3][3] = "　　■　";
		}
	}
	player[0]->act = true;
	player[0]->rotationNum = 0;
	player[0]->x = 5;
	player[0]->y = 2;
	ChangeColor(player[0], player[0]->blockShape);

	player[1]->rotationNum = 0;
	player[1]->x = 14;
	player[1]->y = 2;
	ChangeColor(player[1], player[1]->blockShape);

	player[2]->act = true;
	player[2]->rotationNum = player[0]->rotationNum;
	player[2]->x = player[0]->x;
	player[2]->y = 2;
	player[2]->blockShape = player[0]->blockShape;
	player[2]->color = DARKGRAY;

}

void Progress()
{
	if (GetAsyncKeyState(VK_LEFT))
	{
		player[0]->x--;
		Collision(player[0], Dir_LEFT);
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		player[0]->x++;
		Collision(player[0], Dir_RIGHT);
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		player[0]->y++;
		Collision(player[0], Dir_DOWN);
	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		player[0]->y = player[2]->y;
		stackCount = 4;
	}

	if (player[0]->act)
	{
		timeCount++;
		{
			if (timeCount > 2)
			{
				player[0]->y++;
				timeCount = 0;
			}

		}
	}
	if (CheckBlock1(player[0], player[0]->blockShape))
	{
		player[0]->y--;
		stackCount++;
		if (stackCount > 3)
		{
			player[0]->act = false;
			StackBlock(player[0], player[0]->blockShape);
			stackCount = 0;
		}

	}
	if (!player[0]->act)
	{
		player[0]->act = true;
		player[0]->x = 5;
		player[0]->y = 2;
		player[0]->blockShape = player[1]->blockShape;
		player[0]->rotationNum = player[1]->rotationNum;
		player[0]->color = player[1]->color;
		CreatNewBlock();
	}

	RotationBlock(player[0]);
	RemoveLine();
	ExpectedLocation();

	if (GetAsyncKeyState(VK_RETURN))
	{
		system("PAUSE");
	}
}

void Render()
{
	
	// 블럭 도우미
	for (int y = 0; y < 4; y++)
	{
		WriteBuffer(player[2]->x, player[2]->y + y, player[2]->shape[player[2]->blockShape][player[2]->rotationNum][y], player[2]->color);
	}
	// block 출력
	for (int i = 0; i < BlockAni; i++)
	{
		for (int y = 0; y < 4; y++)
		{
			WriteBuffer(player[0]->x, player[0]->y + y, player[0]->shape[player[0]->blockShape][player[0]->rotationNum][y], player[0]->color);
		}
	}
	// newblock 출력
	for (int y = 0; y < 4; y++)
	{
		WriteBuffer(player[1]->x, player[1]->y + y, player[1]->shape[player[1]->blockShape][0][y], player[1]->color);
	}

	//map 출력
	for (int y = 0; y < MapHeight; y++)
	{
		for (int x = 0; x < MapWidth; x++)
		{
			switch (map[y][x])
			{
			case 0:
				break;
			case 1:
				WriteBuffer(x, y, "▣", WHITE);
				break;
			case 2:
				WriteBuffer(x, y, "■", LIGHTMAGENTA);
				break;
			case 3:
				WriteBuffer(x, y, "■", LIGHTBLUE);
				break;
			case 4:
				WriteBuffer(x, y, "■", LIGHTCYAN);
				break;
			case 5:
				WriteBuffer(x, y, "■", LIGHTGREEN);
				break;
			case 6:
				WriteBuffer(x, y, "■", LIGHTRED);
				break;
			case 7:
				WriteBuffer(x, y, "■", YELLOW);
				break;
			case 8:
				WriteBuffer(x, y, "■", WHITE);
				break;
			default:
				break;
			}
		}
	}
}

void Release()
{
	for (int i = 0; i < 2; i++)
	{
		if (player[i] == nullptr)
		{
			free(player[i]);
			player[i] = nullptr;
		}
	}
}

#pragma endregion

void Collision(Obj* obj, Direction dir)
{
	while (CheckBlock1(obj, obj->blockShape))
	{
		if (dir == Dir_LEFT)
		{
			switch (obj->blockShape)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				obj->x++;
				break;
			default:
				break;
			}
		}
		else if (dir == Dir_RIGHT)
		{
			switch (obj->blockShape)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				obj->x--;
				break;
			default:
				break;
			}
		}
		else
		{
			obj->y--;
		}

	}
}

void RemoveLine()
{
	for (int y = 1; y < MapHeight - 1; y++)
	{
		int lineCount = 0;

		for (int x = 2; x <= 11; x++)
		{
			if (map[y][x] != 0)
			{
				lineCount++;
			}
		}
		if (lineCount >= 10)
		{
			for (int i = 0; i - y < -1; i++)
			{
				for (int x = 2; x <= 11; x++)
				{
					if (y - i - 1 > 0)
					{
						map[y - i][x] = map[y - i - 1][x];
					}
					else
					{
						map[y - i][x] = 0;
					}
				}
			}
		}

	}

}

void RotationBlock(Obj* obj)
{
	if (GetAsyncKeyState(VK_UP))
	{
		obj->rotationNum++;

		if (obj->rotationNum > 3)
		{
			obj->rotationNum = 0;
		}
	}
}

void CreatNewBlock()
{
	player[1]->x = 14;
	player[1]->y = 2;
	player[1]->rotationNum = 0;
	player[1]->blockShape = rand() % 7;
	ChangeColor(player[1], player[1]->blockShape);
}

bool CheckBlock1(Obj* obj, int blockShape)
{
	switch (blockShape)
	{
	case 0:
		if (obj->rotationNum == 0)
		{
			for (int i = 0; i <= 2; i++)
			{
				if (map[obj->y][obj->x + 2] != 0 || map[obj->y + 1][obj->x + 1 + i] != 0)
				{
					return true;
				}
			}
		}
		else if (obj->rotationNum == 1)
		{
			for (int i = 0; i <= 2; i++)
			{
				if (map[obj->y + i][obj->x + 2] != 0 || map[obj->y + 1][obj->x + 3] != 0)
				{
					return true;
				}
			}
		}
		else if (obj->rotationNum == 2)
		{
			for (int i = 0; i <= 2; i++)
			{
				if (map[obj->y + 1][obj->x + 1 + i] != 0 || map[obj->y + 2][obj->x + 2] != 0)
				{
					return true;
				}
			}
		}
		else if (obj->rotationNum == 3)
		{
			for (int i = 0; i <= 2; i++)
			{
				if (map[obj->y + 1][obj->x + 1] != 0 || map[obj->y + i][obj->x + 2] != 0)
				{
					return true;
				}
			}
		}
		break;
	case 1:
		if (obj->rotationNum == 0)
		{
			for (int i = 0; i <= 2; i++)
			{
				if (map[obj->y][obj->x + 1] != 0 || map[obj->y + 1][obj->x + 1 + i] != 0)
				{
					return true;
				}
			}
		}
		else if (obj->rotationNum == 1)
		{
			for (int i = 0; i <= 2; i++)
			{
				if (map[obj->y + i][obj->x + 2] != 0 || map[obj->y][obj->x + 3] != 0)
				{
					return true;;
				}
			}
		}
		else if (obj->rotationNum == 2)
		{
			for (int i = 0; i <= 2; i++)
			{
				if (map[obj->y + 1][obj->x + 1 + i] != 0 || map[obj->y + 2][obj->x + 3] != 0)
				{
					return true;
				}
			}
		}
		else if (obj->rotationNum == 3)
		{
			for (int i = 0; i <= 2; i++)
			{
				if (map[obj->y + 2][obj->x + 1] != 0 || map[obj->y + i][obj->x + 2] != 0)
				{
					return true;
				}
			}
		}
		break;
	case 2:
		if (obj->rotationNum == 0)
		{
			for (int i = 0; i <= 2; i++)
			{
				if (map[obj->y][obj->x + 3] != 0 || map[obj->y + 1][obj->x + 1 + i] != 0)
				{
					return true;
				}

			}
		}
		else if (obj->rotationNum == 1)
		{
			for (int i = 0; i <= 2; i++)
			{
				if (map[obj->y + i][obj->x + 2] != 0 || map[obj->y + 2][obj->x + 3] != 0)
				{
					return true;
				}
			}
		}
		else if (obj->rotationNum == 2)
		{
			for (int i = 0; i <= 2; i++)
			{
				if (map[obj->y + 1][obj->x + 1 + i] != 0 || map[obj->y + 2][obj->x + 1] != 0)
				{
					return true;
				}
			}
		}
		else if (obj->rotationNum == 3)
		{
			for (int i = 0; i <= 2; i++)
			{
				if (map[obj->y][obj->x + 1] != 0 || map[obj->y + i][obj->x + 2] != 0)
				{
					return true;
				}
			}
		}
		break;
	case 3:
		if (obj->rotationNum == 0 || obj->rotationNum == 2)
		{
			for (int i = 1; i <= 2; i++)
			{
				if (map[obj->y + 1][obj->x + 1 + i] != 0 || map[obj->y + 2][obj->x + i])
				{
					return true;
				}
			}
		}
		else if (obj->rotationNum == 1 || obj->rotationNum == 3)
		{
			for (int i = 0; i <= 1; i++)
			{
				if (map[obj->y + i][obj->x + 2] != 0 || map[obj->y + 1 + i][obj->x + 3])
				{
					return true;
				}
			}
		}
		break;
	case 4:
		if (obj->rotationNum == 0 || obj->rotationNum == 2)
		{
			for (int i = 1; i <= 2; i++)
			{
				if (map[obj->y + 1][obj->x + i] != 0 || map[obj->y + 2][obj->x + 1 + i])
				{
					return true;
				}
			}
		}
		else if (obj->rotationNum == 1 || obj->rotationNum == 3)
		{
			for (int i = 0; i <= 1; i++)
			{
				if (map[obj->y + i][obj->x + 3] != 0 || map[obj->y + 1 + i][obj->x + 2])
				{
					return true;
				}
			}
		}
		break;
	case 5:
		if (obj->rotationNum == 0 || obj->rotationNum == 2 ||
			obj->rotationNum == 1 || obj->rotationNum == 3)
		{
			for (int i = 1; i <= 2; i++)
			{
				for (int j = 1; j <= 2; j++)
				{
					if (map[obj->y + i][obj->x + j] != 0)
					{
						return true;
					}
				}
			}
		}
		break;
	case 6:
		if (obj->rotationNum == 0 || obj->rotationNum == 2)
		{
			for (int i = 0; i <= 3; i++)
			{
				if (map[obj->y + 1][obj->x + i] != 0)
				{
					return true;
				}
			}
		}
		else if (obj->rotationNum == 3 || obj->rotationNum == 1)
		{
			for (int i = 0; i <= 3; i++)
			{
				if (map[obj->y + i][obj->x + 2] != 0)
				{
					return true;
				}
			}
		}
		break;
	default:
		break;
	}

	return false;
}

void ChangeColor(Obj* obj, int blockShape)
{
	switch (blockShape)
	{
	case 0:
		obj->color = LIGHTMAGENTA;
		break;
	case 1:
		obj->color = LIGHTBLUE;
		break;
	case 2:
		obj->color = LIGHTCYAN;
		break;
	case 3:
		obj->color = LIGHTGREEN;
		break;
	case 4:
		obj->color = LIGHTRED;
		break;
	case 5:
		obj->color = YELLOW;
		break;
	case 6:
		obj->color = WHITE;
		break;
	default:
		break;
	}
}

void StackBlock(Obj* obj, int blockShape)
{
	switch (blockShape)
	{
	case 0:
		if (obj->rotationNum == 0)
		{
			for (int i = 0; i <= 2; i++)
			{
				map[obj->y][obj->x + 2] = 2;
				map[obj->y + 1][obj->x + 1 + i] = 2;
			}
		}
		else if (obj->rotationNum == 1)
		{
			for (int i = 0; i <= 2; i++)
			{
				map[obj->y + i][obj->x + 2] = 2;
				map[obj->y + 1][obj->x + 3] = 2;

			}
		}
		else if (obj->rotationNum == 2)
		{
			for (int i = 0; i <= 2; i++)
			{
				map[obj->y + 1][obj->x + 1 + i] = 2;
				map[obj->y + 2][obj->x + 2] = 2;
			}
		}
		else if (obj->rotationNum == 3)
		{
			for (int i = 0; i <= 2; i++)
			{
				map[obj->y + 1][obj->x + 1] = 2;
				map[obj->y + i][obj->x + 2] = 2;
			}
		}
		break;
	case 1:
		if (obj->rotationNum == 0)
		{
			for (int i = 0; i <= 2; i++)
			{
				map[obj->y][obj->x + 1] = 3;
				map[obj->y + 1][obj->x + 1 + i] = 3;
			}
		}
		else if (obj->rotationNum == 1)
		{
			for (int i = 0; i <= 2; i++)
			{
				map[obj->y + i][obj->x + 2] = 3;
				map[obj->y][obj->x + 3] = 3;
			}
		}
		else if (obj->rotationNum == 2)
		{
			for (int i = 0; i <= 2; i++)
			{
				map[obj->y + 1][obj->x + 1 + i] = 3;
				map[obj->y + 2][obj->x + 3] = 3;
			}
		}
		else if (obj->rotationNum == 3)
		{
			for (int i = 0; i <= 2; i++)
			{
				map[obj->y + 2][obj->x + 1] = 3;
				map[obj->y + i][obj->x + 2] = 3;

			}
		}
		break;
	case 2:
		if (obj->rotationNum == 0)
		{
			for (int i = 0; i <= 2; i++)
			{
				map[obj->y][obj->x + 3] = 4;
				map[obj->y + 1][obj->x + 1 + i] = 4;
			}
		}
		else if (obj->rotationNum == 1)
		{
			for (int i = 0; i <= 2; i++)
			{
				map[obj->y + i][obj->x + 2] = 4;
				map[obj->y + 2][obj->x + 3] = 4;
			}
		}
		else if (obj->rotationNum == 2)
		{
			for (int i = 0; i <= 2; i++)
			{
				map[obj->y + 1][obj->x + 1 + i] = 4;
				map[obj->y + 2][obj->x + 1] = 4;
			}
		}
		else if (obj->rotationNum == 3)
		{
			for (int i = 0; i <= 2; i++)
			{
				map[obj->y][obj->x + 1] = 4;
				map[obj->y + i][obj->x + 2] = 4;
			}
		}
		break;
	case 3:
		if (obj->rotationNum == 0 || obj->rotationNum == 2)
		{
			for (int i = 1; i <= 2; i++)
			{
				map[obj->y + 1][obj->x + 1 + i] = 5;
				map[obj->y + 2][obj->x + i] = 5;
			}
		}
		else if (obj->rotationNum == 1 || obj->rotationNum == 3)
		{
			for (int i = 0; i <= 1; i++)
			{
				map[obj->y + i][obj->x + 2] = 5;
				map[obj->y + 1 + i][obj->x + 3] = 5;
			}
		}
		break;
	case 4:
		if (obj->rotationNum == 0 || obj->rotationNum == 2)
		{
			for (int i = 1; i <= 2; i++)
			{
				map[obj->y + 1][obj->x + i] = 6;
				map[obj->y + 2][obj->x + 1 + i] = 6;
			}
		}
		else if (obj->rotationNum == 1 || obj->rotationNum == 3)
		{
			for (int i = 0; i <= 1; i++)
			{
				map[obj->y + i][obj->x + 3] = 6;
				map[obj->y + 1 + i][obj->x + 2] = 6;
			}
		}
		break;
	case 5:
		if (obj->rotationNum == 0 || obj->rotationNum == 2 ||
			obj->rotationNum == 1 || obj->rotationNum == 3)
		{
			for (int i = 1; i <= 2; i++)
			{
				for (int j = 1; j <= 2; j++)
				{
					map[obj->y + i][obj->x + j] = 7;
				}
			}
		}
		break;
	case 6:
		if (obj->rotationNum == 0 || obj->rotationNum == 2)
		{
			for (int i = 0; i <= 3; i++)
			{
				map[obj->y + 1][obj->x + i] = 8;
			}
		}
		else if (obj->rotationNum == 3 || obj->rotationNum == 1)
		{
			for (int i = 0; i <= 3; i++)
			{
				map[obj->y + i][obj->x + 2] = 8;
			}
		}
		break;
	default:
		break;
	}

}

void ExpectedLocation()
{
	
	player[2]->x = player[0]->x;
	player[2]->y = player[0]->y;
	player[2]->blockShape = player[0]->blockShape;
	player[2]->rotationNum = player[0]->rotationNum;

	while (!CheckBlock1(player[2], player[2]->blockShape))
	{
		player[2]->y++;
	}
	if (CheckBlock1(player[2], player[2]->blockShape))
	{
		player[2]->y--;
	}
}

#pragma region DoubleBuffer
void InitBuffer()
{
	screenIndex = 0;

	COORD size = { BufferWidth, BufferHeight };
	SMALL_RECT rect = { 0, 0, BufferWidth, BufferHeight };

	hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(hBuffer[0], size);
	SetConsoleWindowInfo(hBuffer[0], TRUE, &rect);

	hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(hBuffer[1], size);
	SetConsoleWindowInfo(hBuffer[1], TRUE, &rect);

	CONSOLE_CURSOR_INFO Info;
	Info.dwSize = 1;
	Info.bVisible = FALSE;

	SetConsoleCursorInfo(hBuffer[0], &Info);
	SetConsoleCursorInfo(hBuffer[1], &Info);


}

void WriteBuffer(int x, int y, const char* shape, int color)
{
	COORD pos = { x * 2, y };

	SetConsoleCursorPosition(hBuffer[screenIndex], pos);
	SetConsoleTextAttribute(hBuffer[screenIndex], color);

	DWORD dw;
	WriteFile(hBuffer[screenIndex], shape, strlen(shape), &dw, NULL);


}

void FlipBuffer()
{
	SetConsoleActiveScreenBuffer(hBuffer[screenIndex]);
	screenIndex = !screenIndex;
}


void ClearBuffer()
{
	COORD pos = { 0,0 };
	DWORD dw;
	FillConsoleOutputCharacter(hBuffer[screenIndex], ' ', BufferWidth * BufferHeight, pos, &dw);
}

void DestroyBuffer()
{
	CloseHandle(hBuffer[0]);
	CloseHandle(hBuffer[1]);
}

#pragma endregion