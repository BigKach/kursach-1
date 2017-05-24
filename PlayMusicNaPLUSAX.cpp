#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include "bass.h"
#pragma comment (lib, "bass.lib")

using namespace std;
int main(void)
{
	if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
		cout << "Ошибка версии BASS.";
		return 1;
	}
	if (!BASS_Init(-1, 22050, NULL, 0, NULL)) {
		cout << "Не удалось инициализировать BASS.";
		return 1;
	}
	HSAMPLE Sample; HCHANNEL Channel;
	Sample = BASS_SampleLoad(FALSE, "C:\\Users\\GIGABYTE\\Documents\\Visual Studio 2015\\Projects\\Проект1\\Music.mp3", 0, 0, 2, BASS_SAMPLE_LOOP);
	if (!Sample)
	{
		cout << "error";
		return 1;
	}
	Channel = BASS_SampleGetChannel(Sample, TRUE);
	if (!Channel)
	{
		cout << "error2";
		return 1;
	}
	if (!BASS_ChannelPlay(Channel, TRUE))
	{
		cout << "error3";
		return 1;
	}
	system("pause");
	BASS_ChannelStop(Channel);
	BASS_SampleFree(Sample);
	BASS_Free();
	system("pause");
	return 0;
}