#include "hash.h"
#include "superblock.h"
#include <stdlib.h>
#include <string.h>

/**
 * FNV-1a hash function implementation for filesystem path hashing
 * Provides good distribution and is fast for string hashing
 * Used for efficient path lookups in directory structures
 */
uint64_t path_hash(const char *path) {
    uint64_t hash = 0xcbf29ce484222325ULL; // FNV-1a offset basis (64-bit)
    
    // Process each character in the path string
    for (; *path; ++path) {
        hash ^= (uint64_t)(unsigned char)(*path);  // XOR with current byte
        hash *= 0x100000001b3ULL; // Multiply by FNV prime (64-bit)
    }
    
    return hash;
}

/**
 * This function takes a given absolute path and returns the corresponding
 * inode and, if a directory, new B-Tree root as found in the B-Tree search process.
 */
InodeBtreePair * item_search(DiskInterface* disk, cache *cache, const char *path)
{
    InodeBtreePair *pair = malloc(sizeof(struct InodeBtreePair));
    const char delimiter[] = "/";
    Superblock sb;
    BTreeNode node;
    char path[PATH_MAX];
    
    superblock_read(disk, cache, &sb);
    
    btree_node_read(disk, cache, sb.btree_root, &node);
    if (!strcmp("/", path))
    {
        pair->inode_number = node.value;
        pair->block_number = node.block_number;
        goto return_pair;
    }
    
    char *token = strtok(path, delimiter);
    uint64_t node_block;
    
    while (token != NULL) {
        node_block = btree_search(disk, cache, node.block_number, path_hash(token));
        printf("%s\n", token);
        token = strtok(NULL, delimiter);
    }
    
return_pair:
    return pair;
}

