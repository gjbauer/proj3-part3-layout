#include "hash.h"
#include "superblock.h"
#include <stdlib.h>
#include <string.h>
#include "btr.h"
#include <limits.h>

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
    char curr_path[PATH_MAX];
    
    pair->inode_number = 0;
    pair->btree_block = 0;
    superblock_read(disk, cache, &sb);
    btree_node_read(disk, cache, sb.btree_root, &node);
    
    if (!strcmp("/", path))
    {
        pair->inode_number = node.value;
        pair->btree_block = node.block_number;
        goto return_pair;
    }
    
    char *token = strtok((char*)path, delimiter);
    uint64_t node_block;
    
    while (token != NULL) {
        printf("Searching for %s\n", token);
        node_block = btree_search(disk, cache, node.block_number, path_hash(token));
        if (node_block)
        {
            snprintf(curr_path, sizeof(curr_path), "%s/%s", curr_path, token);
            btree_node_read(disk, cache, node_block, &node);
            node_block = node.value;
            btree_node_read(disk, cache, node_block, &node);
            if (!strcmp(path, curr_path))
            {
                pair->inode_number = node.value;
                pair->btree_block = node.block_number;
                goto return_pair;
            }
        }
        token = strtok(NULL, delimiter);
    }
    
    fprintf(stderr, "ERROR: Path not found!!\n");
return_pair:
    return pair;
}

