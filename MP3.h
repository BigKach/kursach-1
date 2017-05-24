#include <stdio.h>
#include <string.h>

typedef struct TrackInfo {
	char name[31];
	char artist[31];
	char album[31];
	char year[5];
	char comment[31];
};

TrackInfo InfoMP3(FILE *fp) {
	TrackInfo track;
	int i;
	char  strt[4];
	fseek(fp, -128, SEEK_END);
	for (i = 0; i <= 2; i++)
		strt[i] = getc(fp);
	strt[3] = '\0';
	if (!strcmp(strt, "TAG")) {
		for (i = 0; i < 30; i++)
			track.name[i] = getc(fp);
		for (i = 0; i < 30; i++)
			track.artist[i] = getc(fp);
		for (i = 0; i < 30; i++)
			track.album[i] = getc(fp);
		for (i = 0; i < 4; i++)
			track.year[i] = getc(fp);
		for (i = 0; i < 30; i++)
			track.comment[i] = getc(fp);
		track.name[30] = '\0';
		track.artist[30] = '\0';
		track.album[30] = '\0';
		track.year[4] = '\0';
		track.comment[30] = '\0';
	}
}
