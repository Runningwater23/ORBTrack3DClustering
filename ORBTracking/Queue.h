#pragma once

 /* 函数结果状态代码 */
 #define TRUE 1
 #define FALSE 0
 #define OK 1
 #define ERROR 0
 #define INFEASIBLE -1
 typedef int Status;
 typedef struct
 {
	 int r;
	 int c;
	 double value;
 }QElemType;
 typedef struct QNode
 {
   QElemType data;
   struct QNode *next;
 }QNode,*QueuePtr;

 typedef struct
 {
   QueuePtr front,rear; /* 队头、队尾指针 */
 }LinkQueue;

 Status InitQueue(LinkQueue *Q);

 Status DestroyQueue(LinkQueue *Q);

 Status ClearQueue(LinkQueue *Q);

 Status QueueEmpty(LinkQueue Q);

 int QueueLength(LinkQueue Q);

 Status GetHead_Q(LinkQueue Q,QElemType *e);

 Status EnQueue(LinkQueue *Q,QElemType e);

 Status EnMaxQueue(LinkQueue *Q,QElemType e);//按value值降序排列队列

 Status EnMinQueue(LinkQueue *Q,QElemType e);//按value值升序排列队列

 Status DeQueue(LinkQueue *Q,QElemType *e);

 Status QueueTraverse(LinkQueue Q,void(*vi)(QElemType));

