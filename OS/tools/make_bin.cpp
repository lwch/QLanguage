#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if(argc > 2)
	{
		FILE* fOut = fopen(argv[argc - 1], "wb+");
		if(fOut == NULL) return 0;
		
		for(int i = 1; i < argc - 1; ++i)
		{
			FILE* fp = fopen(argv[i], "rb");
			
			if(fp == NULL) continue;
			
			fseek(fp, 0L, SEEK_END);
			long len = ftell(fp);
			fseek(fp, 0L, SEEK_SET);
			
			char* buffer = (char*)malloc(len);
			if(buffer == NULL) continue;
			fread(buffer, sizeof(char), len, fp);
			fwrite(buffer, sizeof(char), len, fOut);
			free(buffer);
			
			fclose(fp);
			
			printf("append %s successed\n", argv[i]);
		}
		fclose(fOut);
		
		printf("make %s successed", argv[argc - 1]);
	}
	return 0;
}