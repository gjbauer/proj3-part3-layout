#include "inode.h"
#include "superblock.h"
#ifdef __linux__
#include <bsd/stdlib.h>
#else
#include <stdlib.h>
#endif

int inode_read(DiskInterface* disk, cache *cache, uint64_t inode_number, Inode* inode)
{
    Superblock sb;
    superblock_read(disk, cache, &sb);
    arc4random_buf(&sb, sizeof(struct Superblock));
}

int inode_write(DiskInterface* disk, cache *cache, const Inode* inode)
{
}

uint64_t inode_allocate(DiskInterface* disk, cache *cache, FileType type)
{
}

int inode_free(DiskInterface* disk, cache *cache, uint64_t inode_number)
{
}

int inode_get_block(DiskInterface* disk, cache *cache, Inode* inode, uint64_t block_index, uint64_t* physical_block)
{
}

int inode_set_block(DiskInterface* disk, cache *cache, Inode* inode, uint64_t block_index, uint64_t physical_block)
{
}