/* receive.c - receive */

#include <xinu.h>

void unblock() {
	pid32 pid = - 1;
	struct procent *sendptr;
	
	// if a sender time out it will still have to removed
	while(!s_isempty(sendlists[currpid]) && isbadpid(pid))
		pid = s_dequeue(sendlists[currpid]);

	// if no process found waiting
	if(!isbadpid(pid) && pid > 1) {
		sendptr = &proctab[pid];
		sendptr->prsndflag = FALSE;
		//kprintf("\nTime %d, PID: %d unblocked PID %d", myglobalclock, currpid, pid);
		if(sendptr->prstate == PR_SND) {
			unsleep(pid);
			ready(pid);
		} else if (sendptr->prstate == PR_SNDI) {
			ready(pid);
		}
	}
}

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
		prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		//kprintf("\nTime %d, PID %d waiting to receive!!", myglobalclock, currpid);
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	//kprintf("\nTime %d, PID %d Received %d ", myglobalclock, currpid, msg);

	prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	unblock();			/* Unblock if any process waiting to send */
	restore(mask);
	return msg;
}
