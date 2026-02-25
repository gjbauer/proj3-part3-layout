#include <stdio.h>
#include <stdlib.h>
#include "disk.h"
#include "cache.h"
#include "btr.h"
#include "superblock.h"

int main()
{
	DiskInterface* disk = disk_open("my.img");
	
	cache *cache = NULL;

    superblock_initialize(disk, cache, "UNTITLED");
}