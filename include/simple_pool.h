#ifndef SIMPLE_POOL_H
#define SIMPLE_POOL_H

#include <stddef.h>

/** 
 *  @def SP_EXPORT
 *  The sysmbol export macor.
 *  When building a library, it is expanded into exported attributes.
 */
#ifdef _MSC_VER
    #ifdef BUILD_LIB
        #define SP_EXPORT __declspec(dllexport) 
    #else
        #define SP_EXPORT __declspec(dllimport)
    #endif
#elif defined(__GNUC__) || defined(__clang__)
    #ifdef BUILD_LIB 
        #define SP_EXPORT __attribute__((visibility("default")))
    #else
        #define SP_EXPORT
    #endif
#else
    #define SP_EXPORT
#endif

/**  
 * @def DEFAULT_ALIGNMENT
 * The default aligned.
 */
#define DEFAULT_ALIGNMENT sizeof(void*)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SimplePool SimplePool;

/** 
 * @brief Initialize a memory pool with the specified size and aligned to the default byte(DEFAULT_ALIGNMENT).
 * @param block_size The size of each block in the pool.
 * @param num_blocks The number of each block in the pool.
 * @return A SimplePool handle.
 *
 * @see simple_pool_init_alignment(size_t block_size, size_t num_blocks, size_t alignment)
 * @see DEFAULT_ALIGNMENT
 */
SP_EXPORT SimplePool* simple_pool_init(size_t block_size, size_t num_blocks);

/** 
 * @brief Initialize a memory pool with a specified size and aligned according to a specific byte alignment.
 * @param block_size The size of each block in the pool.
 * @param num_blocks The number of each block in the pool.
 * @param alignment specified alignment.
 * @return A SimplePool handle.
 *
 * @see simple_pool_init(size_t block_size, size_t num_blocks)
 * @see DEFAULT_ALIGNMENT
 */
SP_EXPORT SimplePool* simple_pool_init_alignment(size_t block_size, size_t num_blocks, size_t alignment);

/**
 * @brief Destroys the memory pool and frees all allocated memory.
 * @param pool A pointer to memory pool to be destroyed.
 */
SP_EXPORT void simple_pool_destroy(SimplePool* pool);

/**
 * @brief Allocates a block of memory from the pool.
 * @param pool A pointer to the memory pool from which to allocate.
 * @return A pointer to the allocated block of memory, or NULL if the pool is full.
 */
SP_EXPORT void* simple_pool_alloc(SimplePool* pool);

/**
 * @brief Frees a block of memory back to the pool.
 * @param pool The pool of block to be freed.
 * @param ptr A pointer to the block of memory to be freed.
 */
SP_EXPORT void simple_pool_dealloc(SimplePool* pool, void* ptr);


#ifdef __cplusplus
}
#endif

#endif // SIMPLE_POOL_H