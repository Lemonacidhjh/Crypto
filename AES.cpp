#include <stdio.h>
#define N 4
static unsigned char Sbox[16*16] =	// S盒
{
		   //0   1     2     3     4     5     6     7     8     9     a     b     c     d     e     f 
	/*0*/  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	/*1*/  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	/*2*/  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	/*3*/  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	/*4*/  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	/*5*/  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	/*6*/  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	/*7*/  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	/*8*/  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	/*9*/  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	/*a*/  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	/*b*/  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	/*c*/  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	/*d*/  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	/*e*/  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	/*f*/  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static unsigned char RoundKey[N][N] =	// 轮密钥
{
	0xa0, 0x88, 0x23, 0x2a,
	0xfa, 0x54, 0xa3, 0x6c,
	0xfe, 0x2c, 0x39, 0x76,
	0x17, 0xb1, 0x39, 0x05
};

void SubBytes(unsigned char State[N][N])	// 字节替代
{
	int i, j;
	char high;
	char low;
	for (i = 0; i < N; i ++)
	{
		for (j = 0; j < N; j ++)
		{			
				low  = State[i][j] & 0x0F;	// 取低4位
				high = (State[i][j] >> 4)&0x0f;	// 取高4位
				State[i][j] = Sbox[16*high + low];
		}
	}
}

void ShiftRows(unsigned char State[N][N])	// 行移位
{
	int i, j, k;
	int shiftnum = 0;
	char tmp;
			
	for (i = 0; i < N; i ++)
	{
		for (j = 0; j < shiftnum; j ++)		// 循环左移一次	
		{ 
			tmp = State[i][0];
			for (k = 0; k < N-1; k ++)
			{
				State[i][k] = State[i][k+1];
			}
			State[i][k] = tmp;
		}
		shiftnum ++;                       // 移位次数+1
	}
}

unsigned char gfmultby01(unsigned char b)
{
	return b;
}

unsigned char gfmultby02(unsigned char b)
{
	if (b < 0x80)
		return (unsigned char)(int)(b <<1);
	else
		return (unsigned char)( (int)(b << 1) ^ (int)(0x1b) );
}

unsigned char gfmultby03(unsigned char b)
{
	return (unsigned char) ( (int)gfmultby02(b) ^ (int)b );
}

unsigned char choose(unsigned char a, unsigned char b)
{
	if (a == 0x01)
	{
		return gfmultby01(b);
	}
	if (a == 0x02)
	{
		return gfmultby02(b);
	}
	if (a == 0x03)
	{
		return gfmultby03(b);
	} 
	return b;
}

void MixColumns(unsigned char State[N][N])	// 列混合
{
	unsigned char mix[N][N] = 
	{
		0x02, 0x03, 0x01, 0x01,
		0x01, 0x02, 0x03, 0x01,
		0x01, 0x01, 0x02, 0x03,
		0x03, 0x01, 0x01, 0x02
	};
	unsigned char tmp[N];
	unsigned char end[N][N];
	int i, j, k;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			for (k = 0; k < N; k++)
			{
				tmp[k] = choose(mix[i][k], State[k][j]);
			}
			end[i][j] = tmp[0] ^ tmp[1] ^ tmp[2] ^ tmp[3];
		}
	}
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			State[i][j] = end[i][j];
		}
	}

}

void AddRoundKey(unsigned char State[N][N], unsigned char RoundKey[N][N])
{
	int i,j;
	for (j = 0; j < N; j ++)
	{
		for (i = 0; i < N; i ++)
			State[i][j] = State[i][j] ^ RoundKey[i][j];
	}
}


int main()
{
	int i, j, tmp;
	unsigned char State[N][N];
	
	printf("请输入数据：");
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			scanf("%x", &State[i][j]); //0x19 0x3d 0xe3 0xbe 0xa0 0xf4 0xe2 0x2b 0x9a 0xc6 0x8d 0x2a 0xe9 0xf8 0x48 0x08
		}
	}

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < i+1; j++)
		{
			tmp = State[i][j];
			State[i][j] = State[j][i];
			State[j][i] = tmp;
		}
	}

	printf("\n原始数据为：(列优先)\n");
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			printf("%0x ", State[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	SubBytes(State);
	printf("字节替代后：\n");
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			printf("%0x ", State[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	ShiftRows(State);
	printf("行移位后：\n");
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			printf("%0x ", State[i][j]);
		}
		printf("\n");
	}	
	printf("\n");

	MixColumns(State);
	printf("列混合后：\n");
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			printf("%0x ", State[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	AddRoundKey(State, RoundKey);
	printf("轮密钥加后：\n");
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			printf("%0x ", State[i][j]);
		}
		printf("\n");
	}
	return 0;
}