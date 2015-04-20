#ifndef NVM_HASH_H
#define NVM_HASH_H

#include <asm/page_types.h>
#include <config/printk.h>
#include <asm/io.h>
#include <linux/mm_types.h>
#include <linux/mmzone.h>
#include <linux/mm.h>

#define HASH_SHIFT PAGE_SHIFT-3
#define HASH_SIZE PAGE_SIZE/8

extern unsigned long fixed_hash_start;			//in address --- the start of the hash store
extern unsigned long variable_hash_start;			//in address --- the start of the page hash in hash store
extern unsigned long external_page_start;			//in address --- bedayt el page
extern unsigned long command_space_start;		//in address --- 

extern int normalpages;
extern int zeropages;

static inline void NVMHash_COW(unsigned long pfn)
{
	unsigned int sets[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1
	,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2
	,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3
	,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3
	,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2
	,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4
	,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};
	int i = 0;
	int j = 0;
	unsigned char tmp = 0;
	char *Data;
	char *Hash;

	//printk(KERN_CRIT "Calculating hash for COW page for pfn %lu" , pfn);
	Data = (char *) phys_to_virt(pfn << PAGE_SHIFT);
	Hash = (char *) phys_to_virt(variable_hash_start);

	for(i = 0; i < PAGE_SIZE; i += 8)
	{
		for(j = 0; j < 8; j++)
		{
			tmp = *(Data + i + j);
			if(sets[tmp] > 4)
			{
				Hash[i] |= 1 << j; 
			}
			else
			{
				Hash[i] &= ~(1 << j);
			}
		}
	}
}

static inline void NVMHash_incoming(void)
{
	unsigned int sets[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1
	,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2
	,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3
	,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3
	,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2
	,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4
	,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};
	int i = 0;
	int j = 0;
	unsigned char tmp = 0;
	char *Data;
	char *Hash;

	//printk(KERN_CRIT "Calculating hash for incoming page from hard disk");
	Data = (char *) phys_to_virt(external_page_start);
	Hash = (char *) phys_to_virt(variable_hash_start);

	for(i = 0; i < PAGE_SIZE; i+=8)
	{
		for(j = 0; j < 8; j++)
		{
			tmp =*(Data+i+j);
			if(sets[tmp] > 4)
			{
				Hash[i] |= 1 << j; 
			}
			else
			{
				Hash[i] &= ~(1 << j);
			}
		}
	}
}


static inline void NVMHash_existing(unsigned long pfn)
{
	unsigned int sets[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1
	,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2
	,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3
	,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3
	,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2
	,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4
	,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};

	int i = 0;
	int j = 0;
	unsigned char tmp = 0;
	char *Data;

	char *Hash;
	//printk(KERN_DEBUG "Hashing Existing Page  for pfn %lu" , pfn);	

	Data=(char *) phys_to_virt(pfn << PAGE_SHIFT);
	Hash=(char *) phys_to_virt(fixed_hash_start + (pfn - 4096) * HASH_SIZE);

	for(i = 0; i < PAGE_SIZE; i+=8)
	{
		for(j = 0; j < 8; j++)
		{
			tmp = *(Data + i + j);
			if(sets[tmp] > 4)
			{
				Hash[i] |= 1 << j; 
			}
			else
			{
				Hash[i] &= ~(1 << j);
			}
		}
	}
	
}


static inline void NVMHash_zeroHash(void)
{
	int i = 0;
	char* Hash;

	//printk(KERN_CRIT "Zero Hash for a new page\n");
	Hash = (char*) phys_to_virt(variable_hash_start);
	
	for(i = 0; i<external_page_start-variable_hash_start; i++)
		Hash[i] = 0;
}

/*
@inhash the incoming hash pointer to find the distance of
@comphash the original hash to find the distance to
@order the order of pages to fin the hash
*/
static inline unsigned long GetDistance(char * inhash, char * comphash, int order)
{
	unsigned long i;
	unsigned long dist = 0;
	unsigned char sets[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1
	,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2
	,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3
	,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3
	,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2
	,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4
	,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};
		
	
	for (i = 0; i < ((2 ^ order) * PAGE_SIZE); i++)
		dist += sets[inhash[i] ^ comphash[i]];
	return dist;
}

static inline struct page *FindSuitablePage(struct zone *zone, struct list_head *list , gfp_t gfp_flags,
											int cold, int order)
{
	struct page * page = NULL;
	struct list_head *current_head = NULL;
	unsigned long pfn;
	char *Hash;
	unsigned long distance = 0 ;
	unsigned long mindistance = 4194304 ;
	unsigned long minpfn;

	// printk(KERN_CRIT "Finding best fit\n");
	// if((gfp_flags & __GFP_ZERO)){	
	// 	zeropages++;
	// 	printk(KERN_DEBUG "zeropages  = %d " , zeropages);	

	// }
	// normalpages++;
	// printk(KERN_DEBUG "normalpages = %d" , normalpages);
	

	if(order == 0 )
	{
		list_for_each(current_head,list)
		{
			page = list_entry(current_head, struct page, lru);
			pfn = page_to_pfn(page);
			Hash = (char *) phys_to_virt(fixed_hash_start + (pfn-4096) * HASH_SIZE);
			distance = GetDistance(Hash, phys_to_virt(variable_hash_start), 0);
			if(distance <= mindistance)
			{
				mindistance=distance;
				minpfn=pfn;
			}
		}
	}
	// if(pfn < zone->zone_start_pfn)printk(KERN_DEBUG "WRONG PFN AT pfn= %lu" , pfn);
	// if(pfn ==0)printk(KERN_CRIT "A7EH PFN BE ZERO LEEH!!\n");
	// if(pfn >= zone->zone_start_pfn + zone->spanned_pages)printk(KERN_DEBUG "WRONG PFN AT pfn= %lu" , pfn);

	page = pfn_to_page(minpfn);
	
	return page;
} 

#endif /*NVM_HASH_H*/
