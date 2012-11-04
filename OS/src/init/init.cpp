// 

void memcpy(char* dest, const char* src, int size);

int kernel_init()
{
	memcpy((char*)0x2000, (const char*)0x1000, 1024);

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