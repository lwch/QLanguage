// 

void memcpy(char* dest, const char* src, int size);
void print_char(char c);

static const char word_H[16][9] = {
	"00000000",
	"11100111",
	"01000010",
	"01000010",
	"01000010",
	"01000010",
	"01000010",
	"01111110",
	"01000010",
	"01000010",
	"01000010",
	"01000010",
	"01000010",
	"11100111",
	"00000000",
	"00000000"
};

static void* words[255] = {0};

int kernel_init()
{
	memcpy((char*)0x2000, (const char*)0x1000, 1024);
	
	words['H'] = (void*)word_H;
	
	print_char('H');

	while(true);
	
	return 0;
}

void memcpy(char* dest, const char* src, int size)
{
	while(size--)
	{
		*(dest++) = *(src++);
	}
}

void print_char(char c)
{
	char* video = (char*)0xA0000;
	char* word = (char*)words[(int)c];
	
	for(int i = 0; i < 16; ++i)
	{
		for(int j = 0; j < 9; ++j)
		{
			if(*(word + i * 9 + j) == '1') video[i * 320 + j] = 15;
		}
	}
}