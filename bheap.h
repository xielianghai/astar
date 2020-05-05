/*
*	author:	Atom
* date:		2012/12/02
* file:		bheap.h
*/
#ifndef _BHEAP_H
#define _BHEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEF_SIZE			16				/* 默认heap大小 */
#define INC_RATE			1.5				/* heap自动增长速度 */

enum Bheap_type
{
	BHEAP_TYPE_BIG,								/* 大根二叉堆 */
	BHEAP_TYPE_SMALL							/* 小根二叉堆 */
};

struct Bheap_node
{
	void* value;
};

struct Bheap
{
	struct Bheap_node**	head;
	size_t							size;				/* 当前元素个数 */
	size_t							max_size;		/* 堆容量 */
	enum Bheap_type			Btype;			/* 堆类型 */
};

/*		condition		result
*			n1 > n2				1
*			n1 == n2			0
*			n1 < n2				-1
*/
typedef int (*Bheap_compare_t)(struct Bheap_node* n1, struct Bheap_node* n2);

/*		condition		result
*			n1 == n2			1
*			n1 != n2			0
*/
typedef int (*Bheap_equal_t)(struct Bheap_node* n1, struct Bheap_node* n2);

/* free 节点,包括bn->value */
typedef void (*Bheap_free_node_t)(struct Bheap_node* bn);

/*根据 Bheap_type 创建二叉堆,如果未指定堆大小或指定的堆大小 小于等于 1,则初始化堆容量为16*/
static struct Bheap* Bheap_create(size_t size, enum Bheap_type Btype)
{
	struct Bheap* heap = NULL;

	if (NULL == (heap = (struct Bheap*)malloc(sizeof(struct Bheap))))
	{
		fprintf(stderr, "Bheap malloc error!\n");
		return (NULL);
	}
	memset(heap, 0x00, sizeof(struct Bheap));
		
	if (size > 1)
	{
		heap->head = (struct Bheap_node**)malloc(sizeof(struct Bheap_node*) * (size + 1));
		if (NULL == heap->head)
		{
			fprintf(stderr, "Bheap.head malloc error!\n");
			return (NULL);
		}
		heap->max_size = size;
	}
	else
	{
		heap->head = (struct Bheap_node**)malloc(sizeof(struct Bheap_node*) * ((DEF_SIZE) + 1));
		if (NULL == heap->head)
		{
			fprintf(stderr, "Bheap.head malloc error!\n");
			return (NULL);
		}
		heap->max_size = DEF_SIZE;
	}
	
	heap->Btype = Btype;
	
	return (heap);
}

/* 初始化二叉堆 */
static void Bheap_init(struct Bheap* heap)
{
	if (NULL == heap)
		return;
	
	memset(heap->head, 0x00, (sizeof(struct Bheap_node*) * (heap->size + 1)));
	
	heap->size = 0;
}

/* 增长二叉堆容量 */
static size_t __inc_Bheap_size(struct Bheap* heap)
{
	if (NULL == heap)
		return (-1);

	heap->max_size = (size_t)((INC_RATE) * heap->max_size);
	
	heap->head = (struct Bheap_node**)realloc(heap->head, sizeof(struct Bheap_node*) * (heap->max_size + 1));
	
	if (NULL == heap->head)
		return (-1);

	return (0);
}

/* 交换节点内容 */
static void __swap_Bheap_node(struct Bheap_node** n1, struct Bheap_node** n2)
{
	struct Bheap_node* temp;
	
	if (*n1 == *n2)
		return;

	temp = *n1;
	*n1 = *n2;
	*n2 = temp;
}

/* 入堆操作 */
static int Bheap_push(struct Bheap* heap, struct Bheap_node* node, Bheap_compare_t comp)
{
	int n = 0, c;
	if ((NULL == heap) || (NULL == node) || (NULL == comp))
		return	(-1);
	
	if ((heap->size == heap->max_size) && (-1 == __inc_Bheap_size(heap)))
	{
		fprintf(stderr, "increase heap size error!\n");
		return (-1);
	}
	heap->size++;
	n = heap->size;
	heap->head[heap->size] = node;
	
	/* 冗余代码,减少循环中的判断 */
	if (BHEAP_TYPE_SMALL == heap->Btype)
	{
		/* 小根堆 */
		for ( ; ; )
		{
			if (1 == n)
				break;
			c = n;
			n = n>>1;
			if (1 == comp(heap->head[n], heap->head[c]))
				__swap_Bheap_node(&(heap->head[n]), &(heap->head[c]));
			else
				break;
		}
	}
	else
	{
		/* 大根堆 */
		for ( ; ; )
		{
			if (1 == n)
				break;
			c = n;
			n = n>>1;
			if (-1 == comp(heap->head[n], heap->head[c]))
				__swap_Bheap_node(&(heap->head[n]), &(heap->head[c]));
			else
				break;
		}
	}
	
	return (0);
}

/* 移除节点 */
static struct Bheap_node* Bheap_remove(struct Bheap* heap, size_t idx, Bheap_compare_t comp)
{
	int n = 0, c;
	if ((NULL == heap) || (idx < 1) || (idx > heap->size) || (NULL == comp))
		return (NULL);
		__swap_Bheap_node(&(heap->head[idx]), &(heap->head[heap->size]));
	
	n = idx;	
	heap->size--;
	
	/* 冗余代码,减少循环中的判断 */
	if (BHEAP_TYPE_SMALL == heap->Btype)
	{
		/* 小根堆 */
		for ( ; ; )
		{
			c = n;
			n = n<<1;
			if (n > heap->size)
				break;
			
			if ((n + 1) > heap->size)
			{
				if (1 == comp(heap->head[c], heap->head[n]))
					__swap_Bheap_node(&(heap->head[c]), &(heap->head[n]));
				else
					break;
			}
			else
			{
				if (1 == comp(heap->head[n], heap->head[n + 1]))
				{
					if (1 == comp(heap->head[c], heap->head[n + 1]))
					{
						__swap_Bheap_node(&(heap->head[c]), &(heap->head[n + 1]));
						n += 1;
					}
					else
						break;
				}
				else 
				{
					if (1 == comp(heap->head[c], heap->head[n]))
						__swap_Bheap_node(&(heap->head[c]), &(heap->head[n]));
					else
						break;
				}
			}
		}/* end for */
	}
	else
	{
		/* 大根堆 */
		for ( ; ; )
		{
			c = n;
			n = n<<1;
			if (n > heap->size)
				break;
			
			if ((n + 1) > heap->size)
			{
				if (-1 == comp(heap->head[c], heap->head[n]))
					__swap_Bheap_node(&(heap->head[c]), &(heap->head[n]));
				else
					break;
			}
			else
			{
				if (-1 == comp(heap->head[n], heap->head[n + 1]))
				{
					if (-1 == comp(heap->head[c], heap->head[n + 1]))
					{
						__swap_Bheap_node(&(heap->head[c]), &(heap->head[n + 1]));
						n += 1;
					}
					else
						break;
				}
				else 
				{
					if (-1 == comp(heap->head[c], heap->head[n]))
						__swap_Bheap_node(&(heap->head[c]), &(heap->head[n]));
					else
						break;
				}
			}
		}/* end for */
	}
	
	return (heap->head[heap->size + 1]);
}

/* 出堆操作 */
static struct Bheap_node* Bheap_pop(struct Bheap* heap, Bheap_compare_t comp)
{
	if ((NULL == heap) || (NULL == comp) || (0 == heap->size))
		return (NULL);
	
	return Bheap_remove(heap, 1, comp);
}

/* 判断堆中是否包含某节点 */
static size_t is_Bheap_contain(struct Bheap* heap, struct Bheap_node* node, Bheap_equal_t eq)
{
	int n = 1;
	if ((NULL == heap) || (node == NULL) || (NULL == eq))
		return (-1);
	
	for ( ; n <= heap->size; n++)
	{
		if (1 == eq(heap->head[n], node))
			return (n);
	}
	
	return (-1);
}

/* 获取某节点 */
static struct Bheap_node* Bheap_get(struct Bheap* heap, size_t idx)
{
	if ((NULL == heap) || (idx < 1) || (idx > heap->max_size))
		return (NULL);
	
	return (heap->head[idx]);
}

/* 堆销毁,如果需要销毁节点，
*	is_need_free_node 传入非零值(建议传1) 并且指定自己实现的Bheap_free_node_t,
* 如不需要销毁堆中节点 则is_need_free_node传入0,Bheap_free_node_t为NULL
*/
static void Bheap_destory(struct Bheap** heap, int is_need_free_value, Bheap_free_node_t free_node)
{
	size_t o_idx = 1;
	if ((NULL == *heap))
		return;

	if (is_need_free_value && (NULL != free_node))
		for ( ; o_idx <= (*heap)->size; o_idx++)
		{
			if (NULL != ((*heap)->head[o_idx])->value)
			{
				if (NULL != (*heap)->head[o_idx])
				{
					free_node((*heap)->head[o_idx]);
					(*heap)->head[o_idx] = NULL;
				}
			}
		}
	if (NULL != (*heap)->head)
	{
		free((*heap)->head);
		(*heap)->head = NULL;
	}
	free(*heap);
	*heap = NULL;
}

#endif	/*_BHEAP_H*/
