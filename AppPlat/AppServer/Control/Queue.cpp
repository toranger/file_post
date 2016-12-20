#include "Queue.h"
#include <iostream>
using namespace std;
//init
Queue* init_que(){
	Queue* queue = (Queue*)malloc(sizeof(Queue));	
	if(queue != NULL){
		queue->front = NULL;
		queue->rear = NULL;
		queue->num = 0;
	}
	return queue;
}
//destory
void destroy_que(Queue* pqueue){
	if(pqueue != NULL){
		free(pqueue);
	}
	return ;
}
//clear
void clear_que(Queue* pqueue){
	if(pqueue == NULL)
		return ;
	int i = pqueue->num;
	PNode node = pqueue->front;
	while(i--){
		free(node);	
		node = node->next;
	}
	pqueue->num = 0;
	return ;
}
//is empty
int is_empty(Queue* pqueue){
	if(pqueue != NULL && pqueue->num == 0)
		return 0;
	return -1;
}
//size of que
int get_size(Queue* pqueue){
	if(pqueue != NULL)
		return pqueue->num;
	return -1;
}
//return front
PNode get_front(Queue* pqueue,Item* pitem){
	if(pqueue == NULL || pqueue->num == 0)	
		return NULL;
	pitem = &(pqueue->front->num);
	return pqueue->front;
}
//return rear
PNode get_rear(Queue* pqueue,Item* pitem){
	if(pqueue == NULL || pqueue->num == 0)
		return NULL;
	pitem = &(pqueue->rear->num);
	return pqueue->rear;
}
//enqueue
PNode enqueue(Queue* pqueue,Item item){
	if(pqueue == NULL)
		return NULL;
	PNode node = (PNode)malloc(sizeof(Node));	
	node->data = item;
	if(pqueue->num != 0){
		pqueue->rear->next = node;
		pqueue->rear = node;
	}else if(pqueue->num == 0){
		pqueue->front = node;		
		pqueue->rear = node;
	}
	pqueue->num++;
	node->next = NULL;
	return node;
}
//dequeue
PNode dequeue(Queue* pqueue,Item *pitem){
	if(pqueue == NULL || pqueue->num == 0)	
		return NULL;
	pitem = &(pqueue->front->data);
	PNode node = pqueue->front;
	if(node->next != NULL){
		pqueue->front = node->next;
	}else{
		pqueue->front = NULL;
		pqueue->rear = NULL;
	}
	free(node);
	pqueue->num--;
	return pqueue->front;
}
//traverse
void traverse_que(Queue *pqueue,void (*visit)(int)){
	int i = pqueue->num;
	PNode node = pqueue->front;
	while(i--){
		visit(node->num);
		node = node->next;
	}
	return ;
}