/* queue.h - firstid, firstkey, isempty, lastkey, nonempty		*/

/* Queue structure declarations, constants, and inline functions	*/

/* Default # of queue entries: 1 per process plus 2 for ready list plus	*/
/*			2 for sleep list plus 2 per semaphore		*/
#ifndef NQENT
/* ayush edit for LAB3
 * Now it's:
 *   1 per process +
 *   2 per Time Sharing Level +
 *   2 for higher priority process readylist (e.g. daemon)
 *   2 for RT readylist
 *   2 for sleep list
 *   2 per semaphore
 */
#define NQENT	(NPROC + TS_LEVELS + TS_LEVELS + 2 + 4 + NSEM + NSEM)
//#define NQENT	(NPROC + 4 + NSEM + NSEM)
#endif

#define	EMPTY	(-1)		/* Null value for qnext or qprev index	*/
#define	MAXKEY	0x7FFFFFFF	/* Max key that can be stored in queue	*/
#define	MINKEY	0x80000000	/* Min key that can be stored in queue	*/

struct	qentry	{		/* One per process plus two per list	*/
	int32	qkey;		/* Key on which the queue is ordered	*/
	qid16	qnext;		/* Index of next process or tail	*/
	qid16	qprev;		/* Index of previous process or head	*/
};

extern	struct qentry	queuetab[];

/* Inline queue manipulation functions */

#define	queuehead(q)	(q)
#define	queuetail(q)	((q) + 1)
#define	firstid(q)	(queuetab[queuehead(q)].qnext)
#define	lastid(q)	(queuetab[queuetail(q)].qprev)
#define	isempty(q)	(firstid(q) >= NPROC)
#define	nonempty(q)	(firstid(q) <  NPROC)
#define	firstkey(q)	(queuetab[firstid(q)].qkey)
#define	lastkey(q)	(queuetab[ lastid(q)].qkey)

/* Inline to check queue id assumes interrupts are disabled */

#define	isbadqid(x)	(((int32)(x) < 0) || (int32)(x) >= NQENT-1)
