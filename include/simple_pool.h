#ifndef SIMPLE_POOL_H
#define SIMPLE_POOL_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct SimplePool;

/** 
 * @brief Initializes the memory pool with the specified size.
 * @param block_size The size of each block in the pool.
 * @param num_blocks The number of each block in the pool.
 */
SimplePool* simple_pool_init(size_t block_size, size_t num_blocks);

/**
 * @brief Destroys the memory pool and frees all allocated memory.
 * @param pool A pointer to memory pool to be destroyed.
 */
void simple_pool_destroy(SimplePool* pool);

/**
 * @brief Allocates a block of memory from the pool.
 * @param pool A pointer to the memory pool from which to allocate.
 * @return A pointer to the allocated block of memory, or NULL if the pool is full.
 */
void* simple_pool_alloc(SimplePool* pool);

/**
 * @brief Frees a block of memory back to the pool.
 * @param pool The pool of block to be freed.
 * @param ptr A pointer to the block of memory to be freed.
 */
void simple_pool_dealloc(SimplePool* pool, void* ptr);


#ifdef __cplusplus
}
#endif

#endif // SIMPLE_POOL_H