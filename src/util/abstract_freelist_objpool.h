/** 
 * abstract_freelist_objpool.h
 * created 07/22/24 by frank collebrusco 
 */
#ifndef ABSTRACT_FREELIST_OBJPOOL_H
#define ABSTRACT_FREELIST_OBJPOOL_H
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util/misc.h"

/**
 * this structure stores a pool of objects inheriting from the same base.
 * they are stored in place, each object 'slot' is of a fixed size.
 * then an iterator is provided to loop through and get base class ptrs.
 * subclasses can be of varying size as long as they're less than the slot size
 * now, this is weird. we determine if a slot is being used by looking at the last byte
 * since we want slots to be a power of 2, slot_size MUST BE A POWER OF 2 MINUS ONE
 * which is silly but hey. This is enforced with a static assert so don't worry
 */
template <typename Base, uint16_t slot_size, uint16_t slots>
struct abstract_freelist_objpool {
    abstract_freelist_objpool() {
        static_assert(IS_POW2(slot_size + 1));
        static_assert(slot_size >= 3);
        static_assert(slots > 1);
        data = (char*)malloc(size_bytes());
        memset(data, 0, size_bytes());
        top = 0;
    }
    ~abstract_freelist_objpool() {
        for (Base* ent : *this) {
            // printf("dest(i:%d,v:%d): %p\n", get_i(ent), is_valid(get_i(ent)), ent);
            ent->~Base();
            set_valid(get_i(ent), 0);
        }
        printf("out of dest of alop\n");
        free(data);
    }
    NO_COPY_OR_MOVE(abstract_freelist_objpool);

    inline uint32_t size_bytes() const {return (slot_size * slots) + slots;}
    /** unprotected, just buffer + offset */
    inline Base* at(uint16_t i) {
        if (i >= slots) return 0; 
        return (Base*)(&data[i * (slot_size + 1)]);
    }
    inline bool is_valid(uint16_t i) {
        return (i < slots) && data[(i * (slot_size + 1)) + slot_size];
    }
    inline void set_valid(uint16_t i, bool v) {
        if (!(i < slots)) return;
        data[(i * (slot_size + 1)) + slot_size] = v;
    }
    inline uint16_t get_i(Base* ent) const {
        uintptr_t off = (uintptr_t)ent - (uintptr_t)data;
        return off / (slot_size + 1);
    }

    template <typename Sub, typename ...Args>
    Base* push(Args ...args) {
        static_assert(sizeof(Sub) <= slot_size);
        uint16_t i;
        if (freelist.top) {
            i = freelist.data[--freelist.top];
        } else {
            i = top++;
        }
        set_valid(i, true);
        new (at(i)) Sub(args...);
        return at(i);
    }

    void remove(Base* entry) {
        uint16_t i = get_i(entry);
        if (i > slots) {printf("[ERROR afop]: remove: i = %d out of bounds\n", i);}
        entry->~Base();
        set_valid(i, false);
        freelist.data[freelist.top++] = i;
    }

    class afop_it;
    afop_it begin() {return is_valid(0) ? afop_it(this) : this->end();}
    afop_it end() {return afop_it(this, 0xFFFF);}

    class afop_it {
    friend struct abstract_freelist_objpool;
        abstract_freelist_objpool& afop;
        uint16_t i{0};
        afop_it(abstract_freelist_objpool* home, uint16_t _i = 0) : afop(*home), i(_i) {}
    public:
        bool operator==(afop_it const& other) const {return this->i == other.i;}
        bool operator!=(afop_it const& other) const {
            return this->i != other.i;}
        afop_it const& operator++() {
            if (i >= slots) return *this;
            do {
                printf("i = %d to %d\n", i, i+1);
                i++;
                if (i >= slots) {
                    printf("returnin (oob)\n");
                    i = 0xFFFF; return *this;
                }
            } while (!afop.is_valid(i));
            return *this;
        }
        inline Base* operator*() const {return ((i == 0xFFFF) && afop.is_valid(i)) ? 0 : afop.at(i);}
    };

#ifndef UNITTEST
private:
#endif
    char* data;
    uint16_t top;
    struct {
        uint16_t data[slots];
        uint16_t top;
    } freelist = {{0}, {0}};
};


#endif /* ABSTRACT_FREELIST_OBJPOOL_H */
