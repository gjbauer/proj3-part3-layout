#include "inode.h"

int inode_read(DiskInterface* disk, uint64_t inode_number, Inode* inode)
{
}

int inode_write(DiskInterface* disk, const Inode* inode)
{
}

uint64_t inode_allocate(DiskInterface* disk, FileType type)
{
}

int inode_free(DiskInterface* disk, uint64_t inode_number)
{
}

int inode_get_block(DiskInterface* disk, Inode* inode, uint64_t block_index, uint64_t* physical_block)
{
}

int inode_set_block(DiskInterface* disk, Inode* inode, uint64_t block_index, uint64_t physical_block)
{
}