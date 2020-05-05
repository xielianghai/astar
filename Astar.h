/*
* author:	Atom
* date:		2012/12/03
* file:		Astar.h
*/
#ifndef ASTAR_H
#define ASTAR_H

#include <stdio.h>
#include <math.h>
#include "bheap.h"

#define MALLOC(type,n)  (type *)malloc((n)*sizeof(type))

#define MAX(a,b) ((a)>(b))?(a):(b)

#define START		1
#define END			-1
#define EMPTY		0
#define WALL		9
#define ROAD		8

struct tile_map
{
	int** map;
	int row;
	int column;
};

struct map_node
{
	int x;
	int y;
	long f;											/*最终路径长度*/
	long g;											/*起点到该点的已知长度*/
	long h;											/*该点到终点的估计长度*/
	struct map_node* parent;
};

typedef long (* distance_t)(int, int, int, int);

int init_map(struct tile_map*);
void gen_wall(struct tile_map*);
void destory_map(struct tile_map*);
void astar(struct tile_map*, int, int, int, int, distance_t);
int _comp(struct Bheap_node*, struct Bheap_node*);
int _eq(struct Bheap_node*, struct Bheap_node*);
int is_reachable(struct tile_map*, int, int);
int is_arrived(struct tile_map*, struct map_node*);
void free_map_node(struct Bheap_node*);
int deal_child(struct tile_map*, struct Bheap*, struct Bheap*, int, int,
 struct map_node*, distance_t, int, int);
long point_distance(int, int, int, int);

static void print_map(struct tile_map* tmap);
static void print_point(struct map_node*, char );
static void print_heap(struct Bheap*);


#endif		/*ASTAR_H*/
