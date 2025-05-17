/** 
 * c_abstract.h
 * created 08/03/24 by frank collebrusco 
 */
#ifndef C_ABSTRACT_H
#define C_ABSTRACT_H
#include <stddef.h>
#include <type_traits>

/** 
 * this component is useful when you want an abstract ECS component
 * so e.g. a component of Actors where the component can be any override of Actor
 * this is pretty against the ECS philosophy, but it still allocates the subclasses in place in the component array, 
 * and it's just so useful to use a liiiittle bit of OOP on your components.
 * 
 * Usage:
 * say you have an abstract interface like so
 *  class Actor {virtual void taketurn() = 0;};
 * you can create a component which will hold subclasses in place 
 * and let you access them through the base class like so
 *  class c_Actor : public c_abstract<Actor, MAX_ACTOR_SIZE> {}; // or use a typedef
 * now you can use this object as a component with the ECS.
 * emplace an implementation:
 *  comp.emplace<Subclass>(Subclass constructor args...);
 * get the object as a Base reference
 *  Actor& a = comp.get();
 * downcast to a subclass
 *  Subclass s = comp.downcast<Subclass>();
 * destroy, replace methods act as expected.
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
