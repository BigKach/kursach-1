#include <stdlib.h>

typedef struct TrackInfo {
	char name[31];
	char artist[31];
	char album[31];
	char year[5];
	char comment[31];
};
typedef struct CL_ITEM {
	TrackInfo TI;
	CL_ITEM *next;
	CL_ITEM *prev;
};
typedef struct CL_LIST{
	CL_ITEM *current;
	size_t CL_size;
};

CL_ITEM* CL_create(TrackInfo NewElem){
	CL_ITEM *NewItem;
	NewItem = (CL_ITEM*)malloc(sizeof(CL_ITEM));
	if (NewItem == NULL)
		return NULL;
	NewItem->next = NewItem;
	NewItem->prev = NewItem;
	NewItem->TI = NewElem;
	return NewItem;
}

int CL_add_item(CL_LIST *list,TrackInfo NewElem) {
	CL_ITEM *NewItem;
	NewItem=CL_create(NewElem);
	if (NewItem == NULL)
		return 0;
	++list->CL_size;
	if (list->current == NULL) {
		list->current = NewItem;
	}
	else {
		NewItem->next = list->current->next; 
		list->current->next->prev = NewItem; 
		list->current->next = NewItem;
		NewItem->prev = list->current;
	}
	return 1;
}
