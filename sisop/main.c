#include <stdio.h>
#define BOOL int
#define TRUE 1;
#define FALSE 0;


int mv_start(int npages, int nframes);
int mv(int page, int *frame);
int mv_stop(void);


int number_pages_in_memory=0;
int max_memory;
int max_pages;
pagelist * memory = NULL;
pagelist * disk = NULL;


typedef struct pagelist
{
	int page;
	int frame;
	struct pagelist * next;
} pagelist;

pagelist * insert(pagelist* list, int _page, int _frame) // insere um elemento na �ltima posi��o da lista
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

void print(pagelist* list) // imprime a lista
{
	pagelist * p1 = list;
	while(p1!=NULL)
	{
		printf("%d\n",p1->page);
		p1 = p1->next;
	}
}

pagelist * _remove(pagelist * list, int * page_number, int * frame_number) // remove o primeiro elemento da lista
{
    pagelist * p1;
	*page_number = list->page;
	*frame_number = list->frame;
	p1 = list->next;
	free(list);
	return p1;
}

pagelist * free_(pagelist * list) // desaloca o ultimo elemento da lista
{
    pagelist * p1 = NULL;
    pagelist * p2 = list;
    while(p2!=NULL)
    {
        p1 = p2;
        p2 = p1->next;
    }
    free(p1);
}

void free_all(pagelist * list) // desaloca todos os elementos da lista
{
    while(list!=NULL)
    {
        list = free_(list);
    }
}

int get_frame(int page) // retorna o a frame que cont�m a p�gina de n�mero page
{
    pagelist * p1 = memory;
    while(p1)
    {
        if (p1->page == page)
            return p1->frame;
    }
}

BOOL is_in_list(pagelist * list, int page_number) // testa se a p�gina de n�mero page_number est� na lista
{

	BOOL retvalue = FALSE;
	pagelist * p1 = list;
	while(p1!=NULL)
	{
		if (p1->page == page_number)
			retvalue = TRUE;
		p1 = p1->next;
	}
}


BOOL is_in_memory(int _page) // testa se a p�gina de n�mero _page est� na lista
{
    return is_in_list(memory,_page);
}

BOOL is_memory_full() // testa se a mem�ria est� cheia
{
    if(number_pages_in_memory < max_memory)
        return FALSE;
    else
        return TRUE;
}

void insert_in_memory_in_frame(int page, int frame) // insere a p�gina de n�mero page no frame;
{
    pagelist * p1 = memory;
    while(p1!=NULL)
    {
        if(p1->frame == frame)
            p1->page = page;
    }
}

void insert_in_disk(int page) // insere a p�gina de n�mero page no disco
{
    disk = insert(disk,page,-1);
}

void remove_from_disk(int page) // remove a p�gina de n�mero page do disco
{
    pagelist * p1 = NULL;
    pagelist * p2 = disk;
    while(p2->page != page)
    {
        p1 = p2;
        p2 = p2->next;
    }
    p1->next = p2->next;
}

int insert_in_memory(int _page) // se a mem�ria n�o est� cheia, coloca a pagina na proxima frame livre, caso a mem�ria esteja cheia, faz o swap
{
    int frame;
    if (!is_memory_full())
    {
        remove_from_disk(_page);
        memory = insert(memory, _page, number_pages_in_memory);
        frame = number_pages_in_memory;
        number_pages_in_memory++;

    }
    else
    {
        int pn,fn;
        memory = _remove(memory, &pn, &fn);
        memory = insert(memory,_page,fn);
        remove_from_disk(_page);
        insert_in_disk(pn);
        frame = fn;
    }
    return frame;

}

int mv_start(int npages, int nframes)
{
    int i=0;
    for (i=0 ; i<npages ; i++)
        disk = insert_in_disk(i);
    max_memory = nframes;
}

int mv(int page, int *frame)
{
        if(is_in_memory(page))
            *frame = get_frame(page);
        else
            *frame = insert_in_memory(page);
        return 0;
}

int mv_stop(void)
{
    free_all(memory);
    free_all(disk);
}

int main()
{



}


