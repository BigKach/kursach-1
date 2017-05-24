#define _CRT_SECURE_NO_WARNINGS
#include "CL_List.h"
#include "MP3.h"
#include <stdio.h>

int main(void) {
	CL_LIST list{ NULL,0};
	TrackInfo newData;
	FILE *fp;
	char mp3[255];
	fp = fopen("Billy Talent - Man Alive!.mp3", "rb");
	while (!feof(fp)) {
		fgets(mp3, 256, fp);
		printf("%s\n",mp3);
	}

	InfoMP3(fp);
	system("pause");
	return 1;
};