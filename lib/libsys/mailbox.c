/*
mailbox.c
*/

#include "syslib.h"
#include <minix/sysutil.h>

/*===========================================================================*
 *				deposit					     *
 *===========================================================================*/
PUBLIC void deposit(int receipent, message m)
{
	int r;
	message m;

	m.m_type= BUSC_PCI_IDS;
	m.m1_i1= devind;

	r= send(MS, &m);
	
	printf("pci_ids: %04x/%04x\n", *vidp, *didp);
}



