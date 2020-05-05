/**
* Author	:	Atom
* Date		:	2012/12/03
* File		:	slist.h
* Describe:	头插法单链表
**/
#ifndef SLIST_H
#define SLIST_H

#define MALLOC(type,n)	(type *)malloc(sizeof(type)* n)

typedef int (*slist_compare_t)(void* n1, void* n2);

typedef void (* deltonull)(void* node);

struct slist
{
	void *cur;
	struct slist *next;
};

struct slist* slist_create()
{
	struct slist* _list;
	
	if (NULL == (_list = MALLOC(struct slist, 1)))
	{
		fprintf(stderr, "malloc shashmap list error!\n");
		return (NULL);
	}
	
	memset(_list, 0x00, sizeof(struct slist));
	
	return (_list);
}

void* slist_find(struct slist* list, void* node, slist_compare_t _comp)
{
	struct slist* p = NULL;

	if ((NULL == list) || (NULL == list->next) || (NULL == node) || (NULL == _comp))
		return (NULL);
	
	p = list->next;
	
	while (NULL != p)
	{
		if (0 == _comp(p->cur, node))
			return (p->cur);
		
		p = p->next;
	}
	
	return (NULL);
}

int slist_insert(struct slist* list, void* node, slist_compare_t _comp)
{
	struct slist* list_node = NULL;
	
	if ((NULL == list) || (NULL == node))
		return (-1);
	
	if(NULL == (list_node = MALLOC(struct slist, 1)))
	{
		fprintf(stderr, "malloc shashmap list!\n");
		return (-1);
	}
	list_node->cur = node;
	list_node->next = NULL;

	/* 还需要处理key相同,替换的情况 */
	
	if (NULL == list->next)
		list->next = list_node;
	else
	{
		list_node->next = list->next->next;
		list->next = list_node;
	}
	
	return (0);
}

void slist_destory(struct slist* list, deltonull del)
{
	struct slist *p = NULL, *q = NULL;
	
	if (NULL == list)
		return;
	
	p = list;
	q = list->next;
	
	while (NULL != q)
	{
		if (NULL != p->cur)
			del(p->cur);
		if (NULL != p)
			free(p);
		p = q;
		q = p->next;
	}

	if (NULL != p)
		free(p);
}

#endif		/* SLIST_H */
