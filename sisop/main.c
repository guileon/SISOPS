#include <stdio.h>


typedef struct pagelist
{
	int page;
	int frame;
	struct pagelist * next;
} pagelist;

pagelist * insert(pagelist* list, int _page, int _frame)
{

	pagelist * new = (pagelist*)malloc(sizeof(pagelist));
	new->page = _page;
	new->frame = _frame;
	new->next = NULL;

	if(list == NULL)
		return new;
	else
	{
		pagelist * p1 = list;
		pagelist * p2;

		while(p1!=NULL)
		{
			p2 = p1;
			p1 = p1->next;
		}
		p2->next = new;
		return list;
	}


}

void print(pagelist* list)
{
	pagelist * p1 = list;
	while(p1!=NULL)
	{
		printf("%d\n",p1->page);
		p1 = p1->next;
	}
}

pagelist * _remove(pagelist * list, int * page_number, int * frame_number)
{
	*page_number = list->page;
	*frame_number = list->frame;
	return list->next;
}

int is_in_list(pagelist * list, int page_number)
{

	int retvalue = 0;
	pagelist * p1 = list;
	while(p1!=NULL)
	{
		if (p1->page == page_number)
			retvalue = 1;
		p1 = p1->next;
	}
}



int main()
{
	pagelist * memory = NULL;
	pagelist * disk = NULL;


}


