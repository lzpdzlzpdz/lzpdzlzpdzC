/* memcmp example */
#include <stdio.h>
#include <string.h>
 
#define MAX_PCELL_NUM 10

typedef struct
{
	unsigned int ucPcellInvalid;
	unsigned int ucPcellLocalCellId;
}PCELL_INFO;

typedef struct
{
	int cellnum;
	PCELL_INFO stPcellInfo[MAX_PCELL_NUM];
}PCELL_LIST_INFO;

PCELL_LIST_INFO gstpcelllist = {0};

void fun_add_pcell(unsigned int ulLoop)
{
	if(gstpcelllist.cellnum >= MAX_PCELL_NUM)
	{
		printf("fun_add_pcell err\n");
		return;
	}
	gstpcelllist.stPcellInfo[gstpcelllist.cellnum].ucPcellInvalid = 1;
	gstpcelllist.stPcellInfo[gstpcelllist.cellnum].ucPcellLocalCellId = ulLoop;
	
	printf("[add]ucPcellLocalCellId = %d\n",gstpcelllist.stPcellInfo[gstpcelllist.cellnum].ucPcellLocalCellId);

	gstpcelllist.cellnum ++;
}

void fun_del_pcell(unsigned int ulPcellId)
{
	unsigned int ulLoop;
	for(ulLoop = 0; ulLoop < MAX_PCELL_NUM-1; ulLoop++)
	{
		if(ulPcellId == gstpcelllist.stPcellInfo[ulLoop].ucPcellLocalCellId)
		{
			printf("[del]ucPcellLocalCellId = %d\n",gstpcelllist.stPcellInfo[ulLoop].ucPcellLocalCellId);
			memmove(&gstpcelllist.stPcellInfo[ulLoop], &gstpcelllist.stPcellInfo[ulLoop+1], sizeof(PCELL_INFO));
			break;
		}
	}

	gstpcelllist.cellnum --;
}

void fun_upt_pcell(unsigned int ulPcellId, unsigned int ucPcellInvalid)
{
	unsigned int ulLoop;
	for(ulLoop = 0; ulLoop < MAX_PCELL_NUM-1; ulLoop++)
	{
		if(ulPcellId == gstpcelllist.stPcellInfo[ulLoop].ucPcellLocalCellId)
		{
			printf("[old]ucPcellInvalid = %d\n",gstpcelllist.stPcellInfo[ulLoop].ucPcellInvalid);
			gstpcelllist.stPcellInfo[ulLoop].ucPcellLocalCellId = ulPcellId;
		    gstpcelllist.stPcellInfo[ulLoop].ucPcellInvalid = ucPcellInvalid;
			printf("[new]ucPcellInvalid = %d\n",gstpcelllist.stPcellInfo[ulLoop].ucPcellInvalid);
			break;
		}
	}
}

void main()
{
	unsigned int ulLoop;
	printf("hello\n");

	for(ulLoop = 0; ulLoop <10; ulLoop++)
	{
		fun_add_pcell(ulLoop);
	}
    printf("gstpcelllist.cellnum = %d\n",gstpcelllist.cellnum);


	for(ulLoop = 0; ulLoop <8; ulLoop++)
	{
		fun_del_pcell(ulLoop);
	}
    printf("gstpcelllist.cellnum = %d\n",gstpcelllist.cellnum);

	for(ulLoop = 10; ulLoop <20; ulLoop++)
	{
		fun_add_pcell(ulLoop);
	}
    printf("gstpcelllist.cellnum = %d\n",gstpcelllist.cellnum);


	for(ulLoop = 10; ulLoop <20; ulLoop++)
	{
		fun_upt_pcell(ulLoop, 0);
	}
    printf("gstpcelllist.cellnum = %d\n",gstpcelllist.cellnum);
}