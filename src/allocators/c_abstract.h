/** 
 * c_abstract.h
 * created 08/03/24 by frank collebrusco 
 */
#ifndef C_ABSTRACT_H
#define C_ABSTRACT_H
#include <stddef.h>
#include <type_traits>

/** 
 * in-place allocator for derivatives of a base class
 * literally just a buffer with a max size aligned as a base type with helpers to construct, destruct, access, downcast, etc derivatives of the type in this memory
 * TODO right now this is duplicated in the ecs repo. i realize its really a generic allocator if you can call it that, i.e. a simple way to alloc variable size subclasses of a base in place.
 */

template <class Base, size_t max_size>
struct c_abstract {
static_assert(sizeof(Base) <= max_size);
static_assert(( !((max_size) & ((max_size)-1)) )); /* power of 2 */

    inline c_abstract() {memset(data, 0x00, max_size);}
    virtual ~c_abstract() = default;

    c_abstract(c_abstract const& other) = delete;
    c_abstract(c_abstract && other) = delete;
    c_abstract& operator=(c_abstract const& other) = delete;
    c_abstract& operator=(c_abstract && other) = delete;

    Base& get() {
        return (*((Base*)data));
    }

    template <typename Sub>
    Sub& downcast() {
static_assert(sizeof(Sub) <= max_size);
static_assert(std::is_base_of<Base, Sub>::value);
        return (*((Sub*)((Base*)data)));
    }

    template <typename Sub, typename ...Args>
    Sub& emplace(Args... args) {
static_assert(sizeof(Sub) <= max_size);
static_assert(std::is_base_of<Base, Sub>::value);
        return *(new (data) Sub(args...));
    }

    void destroy() {
        this->get().~Base();
    }

    template <typename Sub, typename ...Args>
    Sub& replace(Args... args) {
        this->destroy();
        return this->emplace(args...);
    }

private:
    alignas(Base) char data[max_size];
};

#endif /* C_ABSTRACT_H */
