/** 
 * abstract_linear_objpool.h
 * created 07/17/24 by frank collebrusco 
 * 
 * push differently sized subclasses stored in place to a stack of memory
 * iterate through them via ptr to base class
 * there is no removing elements
 */
#ifndef ABSTRACT_LINEAR_OBJPOOL_H
#define ABSTRACT_LINEAR_OBJPOOL_H
#include <stdlib.h>
#include <stddef.h>
#include <cstring>
#include "util/misc.h"

#include <stdio.h>
template <typename Base, size_t size>
class abstract_linear_objpool {
static_assert(IS_POW2(size));
public:
    abstract_linear_objpool() {
        memset(buf, 0, size); 
        top = 0;
    }
    ~abstract_linear_objpool() {
        this->destroy();
    }
    NO_COPY_OR_MOVE(abstract_linear_objpool);

    template <typename Sub, typename ...Args>
    void push(Args... args) {
        buf_entry_t* base = (buf_entry_t*)(buf + top);
        uint16_t jump = sizeof(Sub) + sizeof(base->next_size);
        top += jump;
        if (top > size) {
            printf("[ERROR alop]: overflow\n");
        }
        base->next_size = (uint16_t)jump;
        new (&base->entry) Sub(args...);
    }

    void clear() {
        this->destroy();
        memset(buf, 0, size);
    }

    class alop_it {
        friend class abstract_linear_objpool;
        unsigned int i;
        abstract_linear_objpool& alop;
        inline alop_it(abstract_linear_objpool* home, unsigned int idx = 0) : i(idx), alop(*home) {}
    public:
        inline Base* operator*() {return (i==(~(0U))) ? 0 : (Base*) ( &(( (abstract_linear_objpool::buf_entry_t*)(alop.buf + i) )->entry) );}
        inline bool operator==(alop_it const& other) const {return other.i == this->i;}
        inline bool operator!=(alop_it const& other) const {return other.i != this->i;}
        alop_it& operator++() {
            i += (((abstract_linear_objpool::buf_entry_t*)(alop.buf + i))->next_size);
            if ((((abstract_linear_objpool::buf_entry_t*)(alop.buf + i))->entry) == 0)
                i = (~(0U));
            return *this;
        }
    };

    alop_it begin() {
        if (((buf_entry_t*)(buf))->entry == 0) {
            return this->end();
        }
        return alop_it(this);
    }
    inline alop_it end() {return alop_it(this, (~(0U)));}

    inline unsigned int get_top() const {return top;}
    inline unsigned int get_size() const {return size;}

private:
    void destroy() {
        top = 0;
        for (Base* obj : *this) {
            obj->~Base();
        }
    }
    char buf[size];
    unsigned int top;
    /* TODO could add a pop by keeping a u16 for size of top of stack. dk if needed */
    struct __attribute__((packed)) buf_entry_t {
        uint16_t next_size;
        uint32_t entry;
    };
};

template <typename T, size_t s>
using alop_t = abstract_linear_objpool<T, s>;


template <typename Base, size_t size>
class heap_abstract_linear_objpool {
static_assert(IS_POW2(size));
public:
    heap_abstract_linear_objpool() {
        buf = (char*)malloc(size); 
        memset(buf, 0, size); 
        top = 0;
    }
    ~heap_abstract_linear_objpool() {
        this->destroy();
        free(buf);
    }
    NO_COPY_OR_MOVE(heap_abstract_linear_objpool);

    template <typename Sub, typename ...Args>
    void push(Args... args) {
        buf_entry_t* base = (buf_entry_t*)(buf + top);
        uint16_t jump = sizeof(Sub) + sizeof(base->next_size);
        top += jump;
        if (top > size) {
            printf("[ERROR alop]: overflow\n");
        }
        base->next_size = (uint16_t)jump;
        new (&base->entry) Sub(args...);
    }

    void clear() {
        this->destroy();
        memset(buf, 0, size);
    }

    class alop_it {
        friend class heap_abstract_linear_objpool;
        unsigned int i;
        heap_abstract_linear_objpool& alop;
        inline alop_it(heap_abstract_linear_objpool* home, unsigned int idx = 0) : i(idx), alop(*home) {}
    public:
        inline Base* operator*() {return (i==(~(0U))) ? 0 : (Base*) ( &(( (heap_abstract_linear_objpool::buf_entry_t*)(alop.buf + i) )->entry) );}
        inline bool operator==(alop_it const& other) const {return other.i == this->i;}
        inline bool operator!=(alop_it const& other) const {return other.i != this->i;}
        alop_it& operator++() {
            i += (((heap_abstract_linear_objpool::buf_entry_t*)(alop.buf + i))->next_size);
            if ((((heap_abstract_linear_objpool::buf_entry_t*)(alop.buf + i))->entry) == 0)
                i = (~(0U));
            return *this;
        }
    };

    alop_it begin() {
        if (((buf_entry_t*)(buf))->entry == 0) {
            return this->end();
        }
        return alop_it(this);
    }
    inline alop_it end() {return alop_it(this, (~(0U)));}

    inline unsigned int get_top() const {return top;}
    inline unsigned int get_size() const {return size;}

private:
    void destroy() {
        top = 0;
        for (Base* obj : *this) {
            obj->~Base();
        }
    }
    char* buf;
    unsigned int top;
    /* TODO could add a pop by keeping a u16 for size of top of stack. dk if needed */
    struct __attribute__((packed)) buf_entry_t {
        uint16_t next_size;
        uint32_t entry;
    };
};

template <typename T, size_t s>
using h_alop_t = heap_abstract_linear_objpool<T, s>;

#endif /* ABSTRACT_LINEAR_OBJPOOL_H */
