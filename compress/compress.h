#pragma once

#ifdef __cplusplus
extern "C"
{
#endif


int compress(void *src, unsigned src_len, void *dst);
int decompress(void *src, unsigned src_len, void *dst);

/*
struct TimeLapse
{
	TimeLapse()
	{
		gettimeofday(&dwStart, NULL);
	}

	void ReStart(const char* chStr)
	{
		gettimeofday(&dwEnd, NULL);
		unsigned long dwTime = 1000 * (dwEnd.tv_sec - dwStart.tv_sec) + (dwEnd.tv_usec - dwStart.tv_usec) / 1000;
		gettimeofday(&dwStart, NULL);
		printf("%s Time Take [%ld]ms \n", chStr, dwTime);
	}

	~TimeLapse()
	{
		gettimeofday(&dwEnd, NULL);
		unsigned long dwTime = 1000 * (dwEnd.tv_sec - dwStart.tv_sec) + (dwEnd.tv_usec - dwStart.tv_usec) / 1000;
		printf("Time Take [%ld]ms \n", dwTime);
	}

	struct timeval dwStart, dwEnd;
};

int main(int argc, char* argv[])
{
	FILE* fInFile = fopen("Font.1b", "r+");
	FILE* fOutFile = fopen("Fonts.lz", "wb");
	FILE* fDecFile = fopen("Fonts.dep", "wb");

	if (fInFile == NULL || fOutFile == NULL || fDecFile == NULL)
	{
		printf("file open error \n");
		return -1;
	}

	TimeLapse tLapse;

	long curpos;
	curpos = ftell(fInFile);
	fseek(fInFile, 0L, SEEK_END);
	int iFileSize = ftell(fInFile);
	fseek(fInFile, curpos, SEEK_SET);

	printf("read file to compress size [%d]B \n", iFileSize);

	int iInBufsize = iFileSize * sizeof(char) + 1;
	char* pInBuf = (char*)malloc(iInBufsize);
	if (pInBuf == NULL)
	{
		printf("malloc error \n");
	}

	fread(pInBuf, iFileSize, 1, fInFile);
	char* pOutBuf = (char*)malloc(2 * iFileSize);
	if (pOutBuf == NULL)
	{
		printf("malloc error \n");
	}

	tLapse.ReStart("read file");
	int iOutPutSize = compress(pInBuf, iFileSize, pOutBuf);
	tLapse.ReStart("compress");

	printf("out file for compress size [%d]B \n", iOutPutSize);

	memset(pInBuf, 0, iInBufsize);
	tLapse.ReStart("deprecess file");
	int iDepressSize = decompress(pOutBuf, iOutPutSize, pInBuf);
	tLapse.ReStart("deprecess");

	printf("out file for decompress size [%d]B \n", iDepressSize);

	printf("strtol [%ld]\n", strtol("-0x20", 0, 0));

	fwrite(pOutBuf, iOutPutSize, 1, fOutFile);
	fwrite(pInBuf, iDepressSize, 1, fDecFile);

	free(pInBuf);
	free(pOutBuf);

	fclose(fInFile);
	fclose(fOutFile);

	return 0;
}*/
#ifdef __cplusplus
}
#endif