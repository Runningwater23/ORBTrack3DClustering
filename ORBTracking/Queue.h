#pragma once

 /* �������״̬���� */
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
   QueuePtr front,rear; /* ��ͷ����βָ�� */
 }LinkQueue;

 Status InitQueue(LinkQueue *Q);

 Status DestroyQueue(LinkQueue *Q);

 Status ClearQueue(LinkQueue *Q);

 Status QueueEmpty(LinkQueue Q);

 int QueueLength(LinkQueue Q);

 Status GetHead_Q(LinkQueue Q,QElemType *e);

 Status EnQueue(LinkQueue *Q,QElemType e);

 Status EnMaxQueue(LinkQueue *Q,QElemType e);//��valueֵ�������ж���

 Status EnMinQueue(LinkQueue *Q,QElemType e);//��valueֵ�������ж���

 Status DeQueue(LinkQueue *Q,QElemType *e);

 Status QueueTraverse(LinkQueue Q,void(*vi)(QElemType));

