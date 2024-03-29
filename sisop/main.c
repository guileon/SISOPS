#include <stdio.h>
#define BOOL int
#define TRUE 1;
#define FALSE 0;


int mv_start(int npages, int nframes);
int mv(int page, int *frame);
int mv_stop(void);


typedef struct pagelist
{
	int page;
	int frame;
	struct pagelist * next;
} pagelist;

int number_pages_in_memory=0;
int max_memory;
int max_pages;
int count=0;
pagelist * memory;
pagelist * disk;




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
	printf("\nImprimindo Lista:\n");
	while(p1!=NULL)
	{
		printf("%d\n",p1->page);
		p1 = p1->next;
	}
	printf("\n...Fim da Impressao\n");
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

void free_all(pagelist * list) // desaloca o ultimo elemento da lista
{
    pagelist * p1 = NULL;
    pagelist * p2 = list;
    while(p2!=NULL)
    {

        p1 = p2;
        p2 = p2->next;
        if(p1)
            free(p1);
    }
    free(p1);

}



int get_frame(int page) // retorna o a frame que cont�m a p�gina de n�mero page
{
    pagelist * p1 = memory;
    while(p1!=NULL)
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
	return(retvalue);
}


BOOL is_in_memory(int _page) // testa se a p�gina de n�mero _page est� na lista
{
    return is_in_list(memory,_page);
}

BOOL is_memory_full() // testa se a mem�ria est� cheia
{
    BOOL retvalue;
    if(number_pages_in_memory < max_memory)
    {
        retvalue = FALSE;
    }else
        retvalue = TRUE;

    return retvalue;
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
    free(p2);
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
    disk = NULL;
    memory = NULL;
    int i=0;
    for (i=0 ; i<npages ; i++)
        insert_in_disk(i);
    max_memory = nframes;
    return 0;
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
    return 0;
}

int main()
{
    int f;
    mv_start(5, 3);
    print(disk);
    print(memory);

    mv(2,&f);
    printf("%d\n",f);
    mv(1,&f);
    printf("%d\n",f);
    mv(2,&f);
    printf("%d\n",f);
    mv(3,&f);
    printf("%d\n",f);

    print(disk); // 0 2
    print(memory); // 1 4 3
    mv_stop();
    getchar();

}


