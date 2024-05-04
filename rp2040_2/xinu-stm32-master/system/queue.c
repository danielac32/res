/* queue.c - enqueue, dequeue */

#include <xinu.h>

struct qentry	queuetab[NQENT];	/* Table of process queues	*/

/*------------------------------------------------------------------------
 *  enqueue  -  Insert a process at the tail of a queue
 *------------------------------------------------------------------------
 */
pid32	enqueue(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q		/* ID of queue to use		*/
	)
{
	qid16	tail, prev;		/* Tail & previous node indexes	*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	tail = queuetail(q);
	prev = queuetab[tail].qprev;

	queuetab[pid].qnext  = tail;	/* Insert just before tail node	*/
	queuetab[pid].qprev  = prev;
	queuetab[prev].qnext = pid;
	queuetab[tail].qprev = pid;
	return pid;
}

/*------------------------------------------------------------------------
 *  dequeue  -  Remove and return the first process on a list
 *------------------------------------------------------------------------
 */
pid32	dequeue(
	  qid16		q		/* ID of queue to use		*/
	)
{
	pid32	pid;			/* ID of process removed	*/

	if (isbadqid(q)) {
		return SYSERR;
	} else if (isempty(q)) {
		return EMPTY;
	}

	pid = getfirst(q);
	queuetab[pid].qprev = EMPTY;
	queuetab[pid].qnext = EMPTY;
	return pid;
}


void queue_init(queue_t *q) {
  q->first = 0;
  q->last = 0;
}

 void queue_push(queue_t *q, task_t *t) {
  if (q->first == 0) {
    t->next = 0;
    q->first = t;
    q->last = t;
  } else {
    t->next = 0;
    q->last->next = t;
    q->last = t;
  }
}

 void queue_pushsort(queue_t *q, task_t *t) {
  task_t *tmp;

  if (q->first == 0) {
    q->first = t;
    q->last = t;
    t->next = 0;
  } else {
    tmp = q->first;
    if (t->param < tmp->param) {
      t->next = tmp;
      q->first = t;
    } else {
      while (tmp->next && (t->param > tmp->next->param))
        tmp = tmp->next;

      if (tmp->next == 0) {
        tmp->next = t;
        t->next = 0;
        q->last = t;
      } else {
        t->next = tmp->next;
        tmp->next = t;
      }
    }
  }
}

 task_t* queue_pop(queue_t *q) {
  task_t *tmp = q->first;
  if (!tmp)
    return 0;

  q->first = tmp->next;
  if (!q->first)
    q->last = 0;

  return tmp;
}

 task_t* queue_peek(queue_t *q) {
  return q->first;
}
 