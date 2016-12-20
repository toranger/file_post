#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef int Item;
typedef node* PNode;
typedef struct node{
	Item data;
	PNode next;
}Node;
typedef struct{
	PNode front;
	PNode rear;
	int num;
}Queue;
//init
Queue* init_que();
//destory
void destroy_que(Queue* pqueue);
//clear
void clear_que(Queue* pqueue);
//is empty
int is_empty(Queue* pqueue);
//size of que
int get_size(Queue* pqueue);
//return front
PNode get_front(Queue* pqueue,Item* pitem);
//return rear
PNode get_rear(Queue* pqueue,Item* pitem);
//enqueue
PNode enqueue(Queue* pqueue,Item item);
//dequeue
PNode dequeue(Queue* pqueue,Item *pitem);
//traverse
void traverse_que(Queue *pqueue,void (*visit)(int));

#endif