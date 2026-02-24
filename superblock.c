#include "cache.h"
#include "superblock.h"
#include <string.h>
#include "config.h"

int superblock_initialize(DiskInterface* disk, cache *cache, const char* volume_name)
{
    if (strlen(volume_name) > 31) return -1;
    Superblock* superblock = get_block(disk, cache, 0, 0);
    superblock->magic_number = 0x4E425452534653;
    superblock->block_size = BLOCK_SIZE;
    superblock->total_blocks = disk->total_blocks;
}