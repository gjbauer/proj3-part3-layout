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
    Superblock superblock;

    superblock_initialize(disk, cache, "UNTITLED");
    superblock_read(disk, cache, &superblock);
    printf("Setting block types to bitmaps for bitmaps...");
    block_type_t *block_type;
    for (int i=1; i < superblock.inode_bitmap+calculate_inode_bitmap_size(&superblock); i++ )
    {
        block_type = (block_type_t*)get_block(disk, cache, 0, i);
        *block_type = BLOCK_TYPE_BITMAP;
    }
    printf("Usable block size / inode size : %lu\n", USABLE_BLOCK_SIZE/sizeof(Inode));
    printf("Allocating pages for superblock, bitmaps, and inode table...\n");
    printf("%llu\n", (superblock.inode_bitmap+calculate_inode_bitmap_size(&superblock)+calculate_inode_table_size(&superblock)));
    for (int i=0; i < (superblock.inode_bitmap+calculate_inode_bitmap_size(&superblock)+calculate_inode_table_size(&superblock)) ; i++) alloc_page(disk, cache);
}