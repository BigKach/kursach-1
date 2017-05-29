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
	char way[size];
} TrackInfo;
typedef struct DL_ITEM {
	TrackInfo TI;
	DL_ITEM *next;
	DL_ITEM *prev;
} DL_ITEM;
typedef struct DL_LIST{
	DL_ITEM *head;
	DL_ITEM *tail;
	DL_ITEM *current;
	size_t DL_size;
} DL_LIST;

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
//Ã¤Ã®Ã¡Ã Ã¢Ã«Ã¥Ã­Ã¨Ã¥ Ã½Ã«Ã¥Ã¬Ã¥Ã­Ã²Ã  Ã¯Ã®Ã±Ã«Ã¥
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
//Ã¤Ã®Ã¡Ã Ã¢Ã«Ã¥Ã­Ã¨Ã¥ Ã½Ã«Ã¥Ã¬Ã¥Ã­Ã²Ã  Ã¤Ã®
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
//Ã¢Ã±Ã²Ã¢Ã Ã¢ÃªÃ   Ã¢ Ã­Ã³Ã¦Ã­Ã®Ã¥ Ã¬Ã¥Ã±Ã²Ã®
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

int get_info(TrackInfo *TI) {
	FILE *fp;
	int i;
	char *res;
	fp = fopen(TI->way, "rb");
	if (fp == NULL)
		return CAN_NOT_OPEN_FILE;
	fclose(fp);
	if (res = strstr(TI->way, ".mp3")) {
		while (*res != '\\' && res != TI->way) {
			res--;
		}
		res++;
		for (i = 0; *res !='.'; i++) {
			TI->fullname[i] = *res;
			res++;
		}
		TI->fullname[i] = '\0';
	}
	else
		return NO_MP3;


}
int insert_track(DL_LIST *list,char *way) {
	TrackInfo NewData;
	strcpy(NewData.way, way);
	if (get_info(&NewData)) {
		insert(list, &NewData);
		return SUCCESS;
	}
	else
		return NO_MP3;
}
//Ã¤Ã¢Ã¨Ã¦Ã¥Ã­Ã¨Ã¥ Ã­Ã  1 Ã¯Ã®Ã§Ã¨Ã¶Ã¨Ã¾ Ã¢Ã¯Ã°Ã Ã¢Ã®
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
//Ã¤Ã¢Ã¨Ã¦Ã¥Ã­Ã¨Ã¥ Ã­Ã  1 Ã¯Ã®Ã§Ã¨Ã¶Ã¨Ã¾ Ã¢Ã«Ã¥Ã¢Ã®
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
//Ã³Ã¤Ã Ã«Ã¥Ã­Ã¨Ã¥ Ã½Ã«Ã¥Ã¬Ã¥Ã­Ã²Ã 
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
//Ã³Ã­Ã¨Ã·Ã²Ã®Ã¦Ã¥Ã­Ã¨Ã¥ Ã±Ã¯Ã¨Ã±ÃªÃ 
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
//Ã¢Ã»Ã¢Ã®Ã¤ Ã±Ã¯Ã¨Ã±ÃªÃ  Ã­Ã  Ã½ÃªÃ°Ã Ã­(Ã¤Ã«Ã¿ Ã²Ã¥Ã±Ã²Ã®Ã¢ Ã¢Ã®Ã§Ã¬Ã®Ã¦Ã­Ã® Ã¨Ã±Ã¯Ã®Ã«Ã¼Ã§Ã®Ã¢Ã Ã²Ã¼Ã±Ã¿ Ã­Ã¥ Ã¡Ã³Ã¤Ã¥Ã²)
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
//Ã¯Ã®Ã¨Ã±Ãª Ã¯Ã® Ã±Ã¯Ã¨Ã±ÃªÃ³
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
//Ã±Ã®ÃµÃ°Ã Ã­Ã¨Ã²Ã¼ Ã²Ã°Ã¥ÃªÃ«Ã¨Ã±Ã²
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
//Ã³Ã¤Ã Ã«Ã¥Ã­Ã¨Ã¥ '/n'
void delete_n(char *name) {
	int i = 0;
	while (name[i] != '\n'&& i<size)
		i++;
	name[i] = '\0';
}
//Ã§Ã Ã£Ã°Ã³Ã§ÃªÃ 
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


int show_text(DL_LIST *list) {
	FILE *fp;
	char *res;
	char nametxt[size],text[size];
	strcpy(nametxt, list->current->TI.way);
	if (res = strstr(nametxt, ".mp3")) {
		res++;
		*res = 't';
		res++;
		*res = 'x';
		res++;
		*res = 't';
	}
	else
		return NO_MP3;
	fp = fopen(nametxt, "r");
	if (fp == NULL)
		return CAN_NOT_OPEN_FILE;
	while (fgets(text, size, fp)!=NULL) {
		puts(text);
	}
	return SUCCESS;
}

