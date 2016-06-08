#include "stdafx.h"

#include"Queue.h"
#include<malloc.h>
#include<stdio.h> 

Status InitQueue(LinkQueue *Q)
{ /* ����һ���ն���Q */
	(*Q).front = (*Q).rear = (QueuePtr)malloc(sizeof(QNode));
	// if(!(*Q).front)
	// exit(OVERFLOW);
	(*Q).front->next = NULL;
	return OK;
}

Status DestroyQueue(LinkQueue *Q)
{ /* ���ٶ���Q(���ۿշ����) */
	while ((*Q).front)
	{
		(*Q).rear = (*Q).front->next;
		free((*Q).front);
		(*Q).front = (*Q).rear;
	}
	return OK;
}

Status ClearQueue(LinkQueue *Q)
{ /* ��Q��Ϊ�ն��� */
	QueuePtr p, q;
	(*Q).rear = (*Q).front;
	p = (*Q).front->next;
	(*Q).front->next = NULL;
	while (p)
	{
		q = p;
		p = p->next;
		free(q);
	}
	return OK;
}

Status QueueEmpty(LinkQueue Q)
{ /* ��QΪ�ն���,�򷵻�TRUE,���򷵻�FALSE */
	if (Q.front == Q.rear)
		return TRUE;
	else
		return FALSE;
}

int QueueLength(LinkQueue Q)
{ /* ����еĳ��� */
	int i = 0;
	QueuePtr p;
	p = Q.front;
	while (Q.rear != p)
	{
		i++;
		p = p->next;
	}
	return i;
}

Status GetHead_Q(LinkQueue Q, QElemType *e)
{ /* �����в���,����e����Q�Ķ�ͷԪ��,������OK,���򷵻�ERROR */
	QueuePtr p;
	if (Q.front == Q.rear)
		return ERROR;
	p = Q.front->next;
	*e = p->data;
	return OK;
}

Status EnQueue(LinkQueue *Q, QElemType e)
{ /* ����Ԫ��eΪQ���µĶ�βԪ�� */
	QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
	p->data = e;
	p->next = NULL;
	(*Q).rear->next = p;
	(*Q).rear = p;
	return OK;
}

Status EnMaxQueue(LinkQueue *Q, QElemType e)
{ /* ����Ԫ��eΪQ���µ�Ԫ�أ�Q�����нڵ�valueֵ�������� */
	QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
	p->data = e;
	p->next = NULL;
	if ((*Q).front == (*Q).rear)//����Ϊ�գ�ֱ�Ӳ���
	{
		(*Q).rear->next = p;
		(*Q).rear = p;
	}
	else{//��������ڵ�����������Ӧλ��
		QueuePtr temp = (*Q).front;
		if (temp->next->data.value > e.value)
		{
			temp = temp->next;
		}
		else{
			p->next = temp->next;
			temp->next = p;
		}
	}
	return OK;
}

Status EnMinQueue(LinkQueue *Q, QElemType e)
{  /* ����Ԫ��eΪQ���µ�Ԫ�أ�Q�����нڵ�valueֵ�������� */
	QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
	p->data = e;
	p->next = NULL;
	if ((*Q).front == (*Q).rear)//����Ϊ�գ�ֱ�Ӳ���
	{
		(*Q).rear->next = p;
		(*Q).rear = p;
	}
	else{//��������ڵ�����������Ӧλ��
		QueuePtr temp = (*Q).front;
		if (temp->next->data.value < e.value)
		{
			temp = temp->next;
		}
		else{
			p->next = temp->next;
			temp->next = p;
		}
	}
	return OK;
}

Status DeQueue(LinkQueue *Q, QElemType *e)
{ /* �����в���,ɾ��Q�Ķ�ͷԪ��,��e������ֵ,������OK,���򷵻�ERROR */
	QueuePtr p;
	if ((*Q).front == (*Q).rear)
		return ERROR;
	p = (*Q).front->next;
	*e = p->data;
	(*Q).front->next = p->next;
	if ((*Q).rear == p)
		(*Q).rear = (*Q).front;
	free(p);
	return OK;
}

Status QueueTraverse(LinkQueue Q, void(*vi)(QElemType))
{ /* �Ӷ�ͷ����β���ζԶ���Q��ÿ��Ԫ�ص��ú���vi()��һ��viʧ��,�����ʧ�� */
	QueuePtr p;
	p = Q.front->next;
	while (p)
	{
		vi(p->data);
		p = p->next;
	}
	printf("\n");
	return OK;
}