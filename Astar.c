/*
 * author:	Atom
* date:		2012/12/03
* file:		Astar.c
*/
#include "Astar.h"

#define SPEED	10

long euclidean_distance(int, int, int, int);		/* 欧氏距离 */
long manhattan_distance(int, int, int, int);		/* 曼哈顿距离 */
long chebyshew_distance(int, int, int, int);		/* 切比雪夫距离 */

int main(int argc, char* argv[])
{
	struct tile_map tmap;
	tmap.row = 35;
	tmap.column = 35;
	
	printf("euclidean distance:\n");
	init_map(&tmap);
	gen_wall(&tmap);
	astar(&tmap, 2 ,1 , 30, 30, euclidean_distance);
	destory_map(&tmap);

	printf("manhattan distance:\n");
	init_map(&tmap);
	gen_wall(&tmap);
	astar(&tmap, 3 ,3 , 30, 30, manhattan_distance);
	destory_map(&tmap);
	
	printf("chebyshew distance:\n");
	init_map(&tmap);
	gen_wall(&tmap);
	astar(&tmap, 3 ,3 , 30, 30, chebyshew_distance);
	destory_map(&tmap);

	return (0);
}

/* 搜索路径 */
void astar(struct tile_map* tmap, int st_x, int st_y, int end_x,
																 int end_y, distance_t distance)
{
	struct Bheap *o_heap = NULL, *c_heap = NULL;
	struct map_node *fnode = NULL;
	struct Bheap_node *inode = NULL, *onode = NULL;
	struct map_node *omnode = NULL;
	int fx = 0, fy = 0;
	
	if ((NULL == tmap) || (st_x <= 0) || (st_y <= 0) || (end_x <= 0) || (end_y <= 0))
		return;
	
	if (!is_reachable(tmap, st_x, st_y) || !is_reachable(tmap, end_x, end_y))
	{
		printf("开始节点或结束节点错误，无法到达!\n");
		return;
	}
	o_heap = Bheap_create(128, BHEAP_TYPE_SMALL);
	c_heap = Bheap_create(128, BHEAP_TYPE_SMALL);
	Bheap_init(o_heap);
	Bheap_init(c_heap);
	
	tmap->map[st_x][st_y] = START;
	tmap->map[end_x][end_y] = END;
	
	if (NULL == (fnode = MALLOC(struct map_node, 1)))
	{
		fprintf(stderr, "malloc fnode error!\n");
		return;
	}
	if (NULL == (inode = MALLOC(struct Bheap_node, 1)))
	{
		fprintf(stderr, "malloc inode error!\n");
		return;
	}
	
	memset(fnode, 0x00, sizeof(struct map_node));
	memset(fnode, 0x00, sizeof(struct Bheap_node));
	
	fnode->x = st_x;
	fnode->y = st_y;
	fnode->g = 0;
	fnode->h = distance(st_x, st_y, end_x, end_y);
	fnode->f = fnode->g + fnode->h;
	fnode->parent = NULL;
	
	inode->value = fnode;
	Bheap_push(o_heap, inode, _comp);

#if 0
	print_map(tmap);
#endif

	for ( ; ; )
	{
		omnode = NULL;
		if (NULL == (onode = Bheap_pop(o_heap, _comp)))
		{
			break;
		}
		else
		{
			omnode = (struct map_node*)onode->value;
			if (is_arrived(tmap, omnode))
					break;
			Bheap_push(c_heap, onode, _comp);
			
			/*上*/
			fx = omnode->x;
			fy = omnode->y - 1;
			if (is_reachable(tmap, fx, fy))
			{
				if(1 == deal_child(tmap, o_heap, c_heap, fx, fy,
																		 omnode, distance, end_x, end_y))
					continue;
			}
			/*右上*/
			fx = omnode->x + 1;
			fy = omnode->y - 1;
			if (is_reachable(tmap, fx, fy))
			{	
				if(1 == deal_child(tmap, o_heap, c_heap, fx, fy,
																		 omnode, distance, end_x, end_y))
					continue;
			}
			
			/*右*/
			fx = omnode->x + 1;
			fy = omnode->y;
			if (is_reachable(tmap, fx, fy))
			{
				if(1 == deal_child(tmap, o_heap, c_heap, fx, fy,
																		 omnode, distance, end_x, end_y))
					continue;
			}
			
			/*右下*/
			fx = omnode->x + 1;
			fy = omnode->y + 1;
			if (is_reachable(tmap, fx, fy))
			{
				if(1 == deal_child(tmap, o_heap, c_heap, fx, fy,
																		 omnode, distance, end_x, end_y))
					continue;
			}
			
			/*下*/
			fx = omnode->x;
			fy = omnode->y + 1;
			if (is_reachable(tmap, fx, fy))
			{
				if(1 == deal_child(tmap, o_heap, c_heap, fx, fy,
																		 omnode, distance, end_x, end_y))
					continue;
			}
			
			/*左下*/
			fx = omnode->x - 1;
			fy = omnode->y + 1;
			if (is_reachable(tmap, fx, fy))
			{	
				if(1 == deal_child(tmap, o_heap, c_heap, fx, fy,
																		 omnode, distance, end_x, end_y))
					continue;
			}
			
			/*左*/
			fx = omnode->x - 1;
			fy = omnode->y;
			if (is_reachable(tmap, fx, fy))
			{	
				if(1 == deal_child(tmap, o_heap, c_heap, fx, fy,
																		 omnode, distance, end_x, end_y))
					continue;
			}
			
			/*左上*/
			fx = omnode->x - 1;
			fy = omnode->y - 1;
			if (is_reachable(tmap, fx, fy))
			{
				if(1 == deal_child(tmap, o_heap, c_heap, fx, fy,
																		 omnode, distance, end_x, end_y))
					continue;
			}
		}
	}

	if (NULL == omnode)
	{
		printf("没有找到可行的路径!\n");
	}
	else
	{
		while(NULL != omnode)
		{
			if ((START!= tmap->map[omnode->x][omnode->y])
					 && (END != tmap->map[omnode->x][omnode->y]))
				tmap->map[omnode->x][omnode->y] = ROAD;
			omnode = omnode->parent;
		}
		print_map(tmap);
	}

	Bheap_destory(&o_heap, 1, free_map_node);
	Bheap_destory(&c_heap, 1, free_map_node);

}

/* 处理↑、↗、→、↘、↓、↙、←、↖方向上的子节点 */
int deal_child(struct tile_map* tmap, struct Bheap *o_heap, struct Bheap *c_heap,
	 int fx, int fy, struct map_node *omnode, distance_t distance, int end_x, int end_y)
{
	struct map_node *fnode = NULL;
	struct Bheap_node *inode = NULL;
	struct Bheap_node *exist_node = NULL;
	size_t idx = 0;
	
	if (NULL == (fnode = MALLOC(struct map_node, 1)))
	{
		fprintf(stderr, "malloc map_node error!\n");
		return (-1);
	}
	if (NULL == (inode = MALLOC(struct Bheap_node, 1)))
	{
		fprintf(stderr, "malloc map_node error!\n");
		return (-1);
	}
	memset(fnode, 0x00, sizeof(struct map_node));
	memset(inode, 0x00, sizeof(struct Bheap_node));
	
	fnode->x = fx;
	fnode->y = fy;
	inode->value = fnode;
	
	fnode->g = omnode->g + point_distance(omnode->x, omnode->y, fnode->x, fnode->y);
	fnode->h = distance(fnode->x, fnode->y, end_x, end_y);
	fnode->f = fnode->g + fnode->h;
	fnode->parent = omnode;
	
	/* 即不在open heap 也不在closed head */
	if (-1 == is_Bheap_contain(o_heap, inode, _eq) 
		&& -1 == is_Bheap_contain(c_heap, inode, _eq))
	{
		Bheap_push(o_heap, inode, _comp);
		if (is_arrived(tmap, fnode))
			return (1);
	}
	/* 在open heap*/
	else if (-1 != (idx = is_Bheap_contain(o_heap, inode, _eq)))
	{
		if (NULL != (exist_node = Bheap_get(o_heap, idx)))
		{
			if (fnode->f < ((struct map_node*)(exist_node->value))->f)
			{
				((struct map_node*)(exist_node->value))->f = fnode->f;
				((struct map_node*)(exist_node->value))->parent = fnode->parent;
			}
		}

		free(fnode);
		free(inode);
	}
	/* 在closed heap */
	else
	{
		free(fnode);
		free(inode);
	}
	return (0);
}

void free_map_node(struct Bheap_node* bn)
{
	free(bn->value);
	free(bn);
}

/* 欧氏距离 */
long euclidean_distance(int x1, int y1, int x2, int y2)
{
	long distance = 0;
	distance = (long)sqrt((long)(pow((x1 - x2) * (SPEED) , 2)
	 + pow((y1 - y2) * (SPEED), 2)));

	return distance;
}

/* 曼哈顿距离 */
long manhattan_distance(int x1, int y1, int x2, int y2)
{
	long distance = 0;
	distance = (abs(x1 - x2) + abs(y1 - y2)) * (SPEED);
	
	return distance;
}

/* 切比雪夫距离 */
long chebyshew_distance(int x1, int y1, int x2, int y2)
{
	long distance = 0;
	distance = MAX(abs(x1 - x2) * (SPEED),
									 abs(y1 - y2)* (SPEED));
	
	return distance;
}

/* 实际两点距离(使用欧氏距离计算) */
long point_distance(int x1, int y1, int x2, int y2)
{
	return euclidean_distance(x1, y1, x2, y2);
}

/* 判断点是否可达 */
int is_reachable(struct tile_map* tmap, int x, int y)
{
	if ((x >= (tmap->row - 1)) || (y >= (tmap->column - 1)) 
		|| (x < 1) || (y < 1) || (WALL == tmap->map[x][y]))
		return (0);

	return (1);
	
}

/* 判断是否到达终点 */
int is_arrived(struct tile_map* tmap, struct map_node* map_node)
{
	if (is_reachable(tmap, map_node->x, map_node->y) 
		&& (END == tmap->map[map_node->x][map_node->y]))
		return (1);
	else
		return (0);
}

/* Bheap_compare_t 函数实现 */
int _comp(struct Bheap_node* n1, struct Bheap_node* n2)
{
	struct map_node *mn1 = NULL, *mn2 = NULL;

	if ((NULL != n1) && (NULL != n2))
	{
		mn1 = (struct map_node*)n1->value;
		mn2 = (struct map_node*)n2->value;

		if (mn1->f > mn2->f)
			return (1);
		else if(mn1->f == mn2->f)
			return (0);
		else
			return (-1);
	}
	else
		return (0);
}

/* Bheap_equal_t 函数实现 */
int _eq(struct Bheap_node* n1, struct Bheap_node* n2)
{
	struct map_node *mn1 = NULL, *mn2 = NULL;

	if ((NULL != n1) && (NULL != n2))
	{
		mn1 = (struct map_node*)n1->value;
		mn2 = (struct map_node*)n2->value;
		return ((mn1->x == mn2->x) && (mn1->y ==mn2->y));
	}
	else
		return (0);
}

/* 初始化map */
int init_map(struct tile_map* tmap)
{
	int o_idx;
	int i ,j;
	if (NULL == tmap)
		return (-1);
	
	tmap->map = MALLOC(int*, tmap->row);
	memset(tmap->map, 0x00, sizeof(int*) * tmap->row);
	
	for (o_idx = 0; o_idx < tmap->row; o_idx++)
	{
		tmap->map[o_idx] = MALLOC(int, tmap->column);
		memset(tmap->map[o_idx], 0x00, sizeof(int) * tmap->column);
	}
}

/*  */
void gen_wall(struct tile_map* tmap)
{
	if (NULL == tmap)
		return;
#if 1
	tmap->map[2][2] = WALL;
	tmap->map[2][4] = WALL;
	tmap->map[3][4] = WALL;
	tmap->map[4][4] = WALL;
	tmap->map[4][3] = WALL;
	tmap->map[3][2] = WALL;
	
	tmap->map[29][29] = WALL;
	tmap->map[29][30] = WALL;
	tmap->map[29][31] = WALL;
	tmap->map[30][31] = WALL;
	tmap->map[31][30] = WALL;
	tmap->map[31][29] = WALL;
	tmap->map[30][29] = WALL;
#endif
}

/* 销毁map */
void destory_map(struct tile_map* tmap)
{
	int o_idx;
	if (NULL == tmap)
		return;
	
	for (o_idx = 0; o_idx < tmap->row; o_idx++)
		free(tmap->map[o_idx]);
	
	free(tmap->map);
	tmap->map = NULL;
}

/* 打印map */
static void print_map(struct tile_map* tmap)
{
	int o_idx, i_idx;
	if (NULL == tmap)
		return;
	
	for (o_idx = 0; o_idx < tmap->row; o_idx++)
	{
		for (i_idx = 0; i_idx < tmap->column; i_idx++)
		{
			if (0 == o_idx || (tmap->row - 1 == o_idx))
				printf("--");
			else if (0 == i_idx || (tmap->column - 1 == i_idx))
				printf("| ");
			else if(START == tmap->map[o_idx][i_idx])
				printf("S ");
			else if (END == tmap->map[o_idx][i_idx])
				printf("E ");
			else if (ROAD == tmap->map[o_idx][i_idx])
				printf("0 ");
			else if (WALL == tmap->map[o_idx][i_idx])
				printf("W ");
			else
				printf("  ");
		}
		printf("\n");
	}
}
