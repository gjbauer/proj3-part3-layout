#include <stdio.h>
#include <stdlib.h>
#include "disk.h"
#include "cache.h"
#include "btr.h"
#include "superblock.h"
#include "inode.h"

int main()
{
	DiskInterface* disk = disk_open("my.img");
	
	cache *cache = NULL;

    superblock_initialize(disk, cache, "UNTITLED");
    printf("Usable block size / Inode size : %lu", USABLE_BLOCK_SIZE/sizeof(Inode));
}