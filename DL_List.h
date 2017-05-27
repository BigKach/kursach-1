#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define size 255
#define CAN_NOT_OPEN_FILE 0
#define NO_MEMORY 0
#define SUCCESS 1
#define INVALID_DATA 0
#define NO_MP3 0

typedef struct TrackInfo {
	char fullname[62];
	char name[31];
	char artist[31];
	char album[31];
	char year[5];
	char comment[31];
	char way[size];
};
typedef struct DL_ITEM {
	TrackInfo TI;
	DL_ITEM *next;
	DL_ITEM *prev;
};
typedef struct DL_LIST{
	DL_ITEM *head;
	DL_ITEM *tail;
	DL_ITEM *current;
	size_t DL_size;
};

//считывание информации о треке
int InfoMP3(TrackInfo *track) {
	int i;
	char  strt[4];
	FILE *fp;
	fp = fopen(track->way, "rb");
	if (fp == NULL)
		return 0;
	fseek(fp, -128, SEEK_END);
	for (i = 0; i <= 2; i++)
		strt[i] = getc(fp);
	strt[3] = '\0';
	if (!strcmp(strt, "TAG")) {
		for (i = 0; i < 30; i++)
			track->name[i] = getc(fp);
		for (i = 0; i < 30; i++)
			track->artist[i] = getc(fp);
		for (i = 0; i < 30; i++)
			track->album[i] = getc(fp);
		for (i = 0; i < 4; i++)
			track->year[i] = getc(fp);
		for (i = 0; i < 30; i++)
			track->comment[i] = getc(fp);
		track->name[30] = '\0';
		track->artist[30] = '\0';
		track->album[30] = '\0';
		track->year[4] = '\0';
		track->comment[30] = '\0';
		strcpy(track->fullname, track->artist);
		strcat(track->fullname, track->name);
		fclose(fp);
		return 1;
	}
	else {
		fclose(fp);
		return NO_MP3;
	}
}
//ñîçäàíèå 1-îãî ýëåìåíòà ñïèñêà
DL_ITEM* DL_create(TrackInfo NewElem){
	DL_ITEM *NewItem;
	NewItem = (DL_ITEM*)malloc(sizeof(DL_ITEM));
	if (NewItem == NULL)
		return NULL;
	NewItem->next = NULL;
	NewItem->prev = NULL;
	NewItem->TI = NewElem;
	return NewItem;
}
//äîáàâëåíèå ýëåìåíòà ïîñëå
int DL_add_after(DL_LIST *list,TrackInfo NewElem) {
	DL_ITEM *NewItem;
	
	NewItem=DL_create(NewElem);
	if (NewItem == NULL)
		return NO_MEMORY;
	
	++list->DL_size;
	
	if (list->current == NULL) {
		list->current = NewItem;
		list->head = NewItem;
		list->tail = NewItem;
		return SUCCESS;
	}
	NewItem->next = list->current->next;
	NewItem->prev = list->current;
	list->current->next = NewItem;
	
	if (NewItem->next != NULL)
		NewItem->next->prev = NewItem;
	else
		list->tail = NewItem;
	
	return SUCCESS;
}
//äîáàâëåíèå ýëåìåíòà äî
int DL_add_before(DL_LIST *list, TrackInfo NewElem){
	DL_ITEM *NewItem;
	NewItem = DL_create(NewElem);
	if (NewItem == NULL)
		return NO_MEMORY;
	
	++list->DL_size;

	if (list->current == NULL) {
		list->current = NewItem;
		list->head = NewItem;
		list->tail = NewItem;
		return SUCCESS;

	}

	NewItem->next = list->current;
	NewItem->prev = list->current->prev;
	list->current->prev = NewItem;
	
	if (NewItem->prev != NULL)
		NewItem->prev->next = NewItem;
	else
		list->head = NewItem;

	return SUCCESS;
}
//âñòâàâêà  â íóæíîå ìåñòî
int insert(DL_LIST *list, TrackInfo *NewData) {
	list->current = list->head;
	if (list == NULL)
		return INVALID_DATA;
	if (list->current == NULL) {
		DL_add_after(list, *NewData);
		return SUCCESS;
	}
	while (strcmp(list->current->TI.fullname, NewData->fullname) < 0 && list->current->next) {
		list->current = list->current->next;
	}

	if (strcmp(list->current->TI.fullname, NewData->fullname) > 0)
		DL_add_before(list, *NewData);
	else
		DL_add_after(list, *NewData); 
	return SUCCESS;
}
//âñòàâêà òðåêà è çàïîëíåíèå èíôîðìàöèè
int insert_track(DL_LIST *list,char *way) {
	TrackInfo NewData;
	strcpy(NewData.way, way);
	if (InfoMP3(&NewData)) {
		insert(list, &NewData);
		return SUCCESS;
	}
	else
		return NO_MP3;
}
//äâèæåíèå íà 1 ïîçèöèþ âïðàâî
void DL_movR(DL_LIST *list) {
	if (list == NULL)
		return;
	if (list->current == NULL)
		return;

	if (list->current->next == NULL)
		if (list->DL_size == 1)
			return;
		else
			list->current = list->head;

	list->current = list->current->next;

}
//äâèæåíèå íà 1 ïîçèöèþ âëåâî
void DL_movL(DL_LIST *list) {
	if (list == NULL)
		return;
	if (list->current == NULL)
		return;

	if (list->current->prev == NULL)
		if (list->DL_size == 1)
			return;
		else
			list->current = list->tail;

	list->current = list->current->prev;
}
//óäàëåíèå ýëåìåíòà
void DL_delete(DL_LIST *list) {
	DL_ITEM *tmp;
	if (list->current == NULL)
		return;
	--list->DL_size;
	if ((list->DL_size + 1) == 1) {
		free(list->current);
		list->current = NULL;
		list->head = NULL;
		list->tail = NULL;
		return;
	}
	if (list->current == list->head) {
		list->current->next->prev = NULL;
		list->head = list->current->next;
		free(list->current);
		list->current = list->head;
		return;
	}
	if (list->current == list->tail) {
		list->current->prev->next = NULL;
		list->tail = list->current->prev;
		free(list->current);
		list->current = list->tail;
		return;
	}
	list->current->next->prev = list->current->prev;
	list->current->prev->next = list->current->next;
	tmp = list->current->next;
	free(list->current);
	list->current = tmp;
	
	return;
}
//óíè÷òîæåíèå ñïèñêà
void DL_Destroy(DL_LIST *list) {
	DL_ITEM *tmp;
	list->current = list->head;
	while (list->current->next != NULL) {
		tmp = list->current->next;
		free(list->current);
		list->current = tmp;
	}
	free(list->current);
	list->current = NULL;
	list->head = NULL;
	list->tail = NULL;
	list->DL_size = 0;
}
//âûâîä ñïèñêà íà ýêðàí(äëÿ òåñòîâ âîçìîæíî èñïîëüçîâàòüñÿ íå áóäåò)
void DL_show(DL_LIST *list) {
	if (list == NULL)
		return;
	if (list->current == NULL)
		return;
	list->current = list->head;
	while (list->current->next != NULL) {
		printf("%s\n", list->current->TI.fullname);
		list->current = list->current->next;
	}
	printf("%s\n", list->current->TI.fullname);
	
}
//ïîèñê ïî ñïèñêó
DL_ITEM* DL_search(DL_LIST *list, char *key) {
	list->current = list->head;
	while (list->current->next != NULL) {
		if (strstr(list->current->TI.fullname, key))
			return list->current;
		list->current = list->current->next;
	}
	if (strstr(list->current->TI.fullname, key))
		return list->current;
	else
		return NULL;
}
//ñîõðàíèòü òðåêëèñò
void save(DL_LIST *list) {
	FILE *fp;
	fp = fopen("SPP_TL.txt", "w");
	if (fp == NULL)
		return;
	if (list->current == NULL)
		return;
	list->current = list->head;
	while (list->current->next) {
		fprintf(fp, "%s\n", list->current->TI.way);
		list->current = list->current->next;
	}
	fprintf(fp, "%s\n", list->current->TI.way);
	fclose(fp);
}
//óäàëåíèå '/n'
void delete_n(char *name) {
	int i = 0;
	while (name[i] != '\n'&& i<size)
		i++;
	name[i] = '\0';
}
//çàãðóçêà
void load(DL_LIST *list) {
	FILE *fp;
	char way[size];
	fp = fopen("SPP_TL.txt", "r");
	while (fgets(way, size, fp)!=NULL) {
		delete_n(way);
		insert_track(list, way);
	}
	fclose(fp);
}
