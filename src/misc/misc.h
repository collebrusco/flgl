/** 
 * misc.h 
 * flgl
 * created 04/20/25 by frank collebrusco
 */
#ifndef MISC_H
#define MISC_H

#define NO_COPY(class_type) \
    class_type(class_type const& other) = delete; \
    class_type& operator=(class_type const& other) = delete; \

#define NO_MOVE(class_type) \
    class_type(class_type && other) = delete; \
    class_type& operator=(class_type && other) = delete;

#define NO_COPY_OR_MOVE(class_type) \
    class_type(class_type const& other) = delete; \
    class_type(class_type && other) = delete; \
    class_type& operator=(class_type const& other) = delete; \
    class_type& operator=(class_type && other) = delete;


#define IS_POW2(num) ( !((num) & ((num)-1)) )


#endif /* MISC_H */
