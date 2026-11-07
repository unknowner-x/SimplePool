#include <cstdlib>
#include <cstddef>
#include <new>

#include <simple_pool.h>

constexpr static std::size_t Align = alignof(std::max_align_t);

class SimplePool {
public:
    SimplePool(std::size_t block_size, std::size_t num_blocks) : 
    block_size_(block_size), num_blocks_(num_blocks), free_list_(nullptr) {
        if (block_size < sizeof(void*)) block_size_ = sizeof(void*); // Ensure that at least one pointer is placed in each block

        block_size_ = (block_size_ + Align - 1) / Align * Align;
        pool_ = std::malloc(block_size_ * num_blocks_);
        if (!pool_) throw std::bad_alloc();
        
        char* start = static_cast<char*>(pool_);
        for(std::size_t i = 0; i < num_blocks_; i++) {
            char* block = start + i * block_size_;
            *reinterpret_cast<void**>(block) = free_list_;
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
    Block* free_list_;
    void* pool_;
};

extern "C"{
    SimplePool* simple_pool_init(size_t block_size, size_t num_blocks){
        if(block_size == 0 || num_blocks == 0) return nullptr;

        try {
            return new SimplePool(block_size, num_blocks);
        } catch (...) {
            return nullptr;
        }
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