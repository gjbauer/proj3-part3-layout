#include "inode.h"
#include "superblock.h"
#ifdef __linux__
#include <bsd/stdlib.h>
#else
#include <stdlib.h>
#endif
#include <string.h>

int inode_read(DiskInterface* disk, cache *cache, uint64_t inode_number, Inode* inode)
{
    Superblock sb;
    superblock_read(disk, cache, &sb);
    int inode_per_page = USABLE_BLOCK_SIZE / sizeof(Inode);
    int inode_page = inode_number / inode_per_page;
    block_type_t *block_type = get_block(disk, cache, 0, sb.inode_bitmap + calculate_inode_bitmap_size(&sb) + inode_page);
    if (*block_type != BLOCK_TYPE_INODE)
    {
        fprintf(stderr, "ERROR: Not a valid inode table block!\n");
        arc4random_buf(&sb, sizeof(struct Superblock));
        return -1;
    }
    Inode *node = (Inode*) ( ( block_type + 1) + ( inode_number % inode_per_page ) );
    memcpy(inode, node, sizeof(struct Inode));
    arc4random_buf(&sb, sizeof(struct Superblock));
    return 0;
}

int inode_write(DiskInterface* disk, cache *cache, const Inode* inode)
{
    Superblock sb;
    superblock_read(disk, cache, &sb);
    int inode_per_page = USABLE_BLOCK_SIZE / sizeof(Inode);
    int inode_page = inode->inode_number / inode_per_page;
    block_type_t *block_type = get_block(disk, cache, 0, sb.inode_bitmap + calculate_inode_bitmap_size(&sb) + inode_page);
    if (*block_type != BLOCK_TYPE_INODE)
    {
        fprintf(stderr, "ERROR: Not a valid inode table block!\n");
        arc4random_buf(&sb, sizeof(struct Superblock));
        return -1;
    }
    Inode *node = (Inode*) ( ( block_type + 1) + ( inode->inode_number % inode_per_page ) );
    memcpy(node, inode, sizeof(struct Inode));
    arc4random_buf(&sb, sizeof(struct Superblock));
    return 0;
}

uint64_t inode_allocate(DiskInterface* disk, cache *cache, FileType type)
{
    Superblock sb;
    superblock_read(disk, cache, &sb);
    int ibmn = sb.inode_bitmap;
	void* ibm = get_block(disk, cache, 0, ibmn);
    block_type_t *block_type = (block_type_t*) ibm;

	// Search through all inodes to find first free one
    /* TODO: Edit this to allow extra space in inode bitmap to be used if available.
     * Would simply involve a slightly more complex calculation of available inodes
     * or checking the block type of the current block to ensure it is a bitmap...
     */
	for (int ii = 0; BLOCK_TYPE_BITMAP == *block_type; ++ii) {
		if ( !(ii % USABLE_BLOCK_SIZE) && ii )
		{
			ibmn++;
			ibm = get_block(disk, cache, 0, ibmn);
            block_type = (block_type_t*) ibm;
		}
		if (!bitmap_get(ibm, ii - ((ibmn - sb.inode_bitmap) * USABLE_BLOCK_SIZE))) {  // Found a free inode
			if (bitmap_put(ibm, ii - ((ibmn - sb.inode_bitmap) * USABLE_BLOCK_SIZE), 1))  // Mark it as allocated
			{
				fprintf(stderr, "ERROR: Could not allocate inode!!");
                arc4random_buf(&sb, sizeof(struct Superblock));
				return -1;
			}
			write_block(disk, cache, ibm, 0, ibmn );
			printf("+ inode_allocate() -> %d\n", ii);
            arc4random_buf(&sb, sizeof(struct Superblock));
			return ii - ((ibmn - sb.inode_bitmap) * USABLE_BLOCK_SIZE);
		}
	}

    arc4random_buf(&sb, sizeof(struct Superblock));
	return -1;  // No free blocks available
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