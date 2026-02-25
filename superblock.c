#include "cache.h"
#include "superblock.h"
#include <string.h>
#include "config.h"
#include <stdint.h>

int superblock_initialize(DiskInterface* disk, cache *cache, const char* volume_name)
{
    if (strlen(volume_name) > 31) return -1;
    block_type_t *block_type = (block_type_t*)get_block(disk, cache, 0, 0);
    *block_type = BLOCK_TYPE_SUPER;
    Superblock* superblock = (Superblock*) ( block_type + 1 );
    printf("Size of superblock: %llu\n", sizeof(Superblock));
    superblock->magic_number = 0x4E425452534653;
    superblock->block_size = BLOCK_SIZE;
    superblock->total_blocks = disk->total_blocks;
    printf("Total blocks: %lu\n", superblock->total_blocks);
    uint32_t bitmap_space = (superblock->total_blocks % USABLE_BLOCK_SIZE) ? ( (superblock->total_blocks / USABLE_BLOCK_SIZE) + 1 ) : (superblock->total_blocks / USABLE_BLOCK_SIZE);
    bitmap_space += (superblock->total_blocks % (4*USABLE_BLOCK_SIZE)) ? ( (superblock->total_blocks / (4*USABLE_BLOCK_SIZE) ) + 1 ) : (superblock->total_blocks / (4*USABLE_BLOCK_SIZE) );
    printf("Number of blocks needed for block bitmap: %llu\n", (superblock->total_blocks % USABLE_BLOCK_SIZE) ? ( (superblock->total_blocks / USABLE_BLOCK_SIZE) + 1 ) : (superblock->total_blocks / USABLE_BLOCK_SIZE) );
    printf("Number of blocks needed for inode bitmap: %llu\n", (superblock->total_blocks % (4*USABLE_BLOCK_SIZE)) ? ( (superblock->total_blocks / (4*USABLE_BLOCK_SIZE) ) + 1 ) : (superblock->total_blocks / (4*USABLE_BLOCK_SIZE) ) );
    printf("Total number of blocks needed for bitmaps: %lu\n", bitmap_space);
}