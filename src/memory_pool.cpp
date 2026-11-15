#include <cstdlib>
#include <cstddef>
#include <new>

#include "simple_pool.h"

// helper function
namespace {

    bool is_pow2(size_t num){
        return num != 0 && (num & (num - 1)) == 0;
    }
    
    void* platform_aligned_alloc(std::size_t size, std::size_t alignment) {
        if (alignment <= alignof(std::max_align_t)) {
        return std::malloc(size);
    }
    #if defined(_MSC_VER)
        return _aligned_malloc(size, alignment);
    #else
    void* p = nullptr;
    if (posix_memalign(&p, alignment, size) != 0) return nullptr;
        return p;
    #endif
    }

void platform_aligned_free(void* p, std::size_t alignment) {
    if (!p) return;
    if (alignment <= alignof(std::max_align_t)) {
        std::free(p);
    }
    #if defined(_MSC_VER)
    else {
        _aligned_free(p);
    }
    #else
    else {
        std::free(p);
    }
    #endif
    }
}

class SimplePool {
public:
    SimplePool(std::size_t block_size, std::size_t num_blocks, std::size_t aligned) : 
    block_size_(block_size), num_blocks_(num_blocks), alignment_(aligned) ,
    free_list_(nullptr), pool_(nullptr) {
        if (block_size < sizeof(void*)) block_size_ = sizeof(void*); // Ensure that at least one pointer is placed in each block

        block_size_ = (block_size_ + alignment_ - 1) / alignment_ * alignment_;
        pool_ = platform_aligned_alloc(block_size_ * num_blocks_, alignment_);
        if (!pool_) throw std::bad_alloc();
        
        char* start = static_cast<char*>(pool_);
        for(std::size_t i = 0; i < num_blocks_; i++) {
            char* block = start + i * block_size_;
            *reinterpret_cast<Block**>(block) = free_list_;
            free_list_ = reinterpret_cast<Block*>(block);
        }

    }

    ~SimplePool() {
        std::free(pool_);
    }

    SimplePool(const SimplePool&) = delete;
    SimplePool& operator=(const SimplePool&) = delete;

    void* alloc(){
        if (!free_list_) 
            return nullptr;   
        Block* ptr = free_list_;
        free_list_ = free_list_->next;
        return ptr;
    }

    void dealloc(void* ptr){
        if (!ptr) return;
        static_cast<Block*>(ptr)->next = free_list_;
        free_list_ = static_cast<Block*>(ptr);

    }

private:
    struct Block {
        Block* next;
    };
    
    std::size_t block_size_;
    std::size_t num_blocks_;
    std::size_t alignment_;
    Block* free_list_;
    void* pool_;
};


extern "C"{
    SimplePool* simple_pool_init_alignment(size_t block_size, size_t num_blocks, size_t alignment){
        if(block_size == 0 || num_blocks == 0) return nullptr;
        if (alignment < sizeof(void*) || !is_pow2(alignment)) return nullptr;
        try {
            return new SimplePool(block_size, num_blocks, alignment);
        } catch (...) {
            return nullptr;
        }
    }

    SimplePool* simple_pool_init(size_t block_size, size_t num_blocks){
        return simple_pool_init_alignment(block_size, num_blocks, DEFAULT_ALIGNMENT);
    }

    void simple_pool_destroy(SimplePool *pool){
        delete pool;
    }

    void* simple_pool_alloc(SimplePool* pool){
        if(!pool) return nullptr;
        return pool->alloc();
    }

    void simple_pool_dealloc(SimplePool* pool, void *ptr){
        if (!pool) return;
        pool->dealloc(ptr);
    }
}