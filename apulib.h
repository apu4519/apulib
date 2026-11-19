//APULIB - A C macro library for data structures and algorithms
#ifndef APULIB_FULL_LIB
#define APULIB_FULL_LIB

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//APULIB - Helper macros
#define apulib_realloc(pointer,size)\
    do{\
        pointer = realloc(pointer,size);\
        while(!pointer)\
        {\
            printf("Memory allocation failure! Exiting with code 1");\
            exit(1);\
        }\
    }while(0)

#define apulib_malloc(pointer,size)\
    do{\
        pointer = malloc(size);\
        while(!pointer)\
        {\
            printf("Memory allocation failure! Exiting with code 1");\
            exit(1);\
        }\
    }while(0)

#define apulib_true_cond(a) (1)
#define apulib_return_1(a,b) (1)
#define apulib_sum_helper(a,b) ((a) + (b))
#define apulib_max_helper(a,b) (((a) > (b)) ? (a) : (b))
#define apulib_min_helper(a,b) (((a) < (b)) ? (a) : (b))
#define apulib_swap(a,b)\
    do{\
        typeof(a) _tmp_=a;\
        a = b;\
        b = _tmp_;\
    }while(0)

#ifndef APULIB_EPSILON_ENV
#define APULIB_EPSILON_ENV 0.0000000005
#endif

#define apulib_epsilon_close(a, b)\
    _Generic((a) + (b),\
        float: fabsf((float)(a) - (float)(b)) < APULIB_EPSILON_ENV,\
        double: fabs((double)(a) - (double)(b)) < APULIB_EPSILON_ENV,\
        long double: fabsl((long double)(a) - (long double)(b)) < APULIB_EPSILON_ENV,\
        default: (a) == (b)\
    )

#ifdef APULIB_DA_IMPL

//Create a dynamic array type for {T} type and with the name {typename}
#define da_declare(T,typename) typedef struct { T* data; size_t size; size_t capacity; } typename
//Creates a dynamic array of {typename} struct type and {name} name of capacity 1.
#define da_create(typename,name) typename name = {NULL, 0, 1}
//Appends a {val} value to the end of {arr} dynamic array
#define da_append(arr,val)\
    do{\
        if(arr.data == NULL) apulib_malloc(arr.data,sizeof(arr.data[0]));\
        if(arr.size >= arr.capacity) {\
            arr.capacity *= 2;\
            apulib_realloc(arr.data,sizeof(arr.data[0]) * arr.capacity);\
        }\
        arr.data[arr.size++] = val;\
    }while(0)

//Gets the value at {index} index of {arr} dynamic array
#define da_get(arr,index) arr.data[index]
//Clears the values of {arr} dynamic array, and also frees the data
#define da_clear(arr)\
    do{\
        arr.size=0;\
        arr.capacity=1;\
        free(arr.data);\
        arr.data = NULL;\
    }while(0)
//Inserts {val} value into {arr} dynamic array to array position {where}
#define da_insert(arr,val,where)\
    do{\
        if(arr.data != NULL)\
        {\
            if(arr.size >= arr.capacity) {\
                arr.capacity *= 2;\
                apulib_realloc(arr.data, sizeof(arr.data[0]) * arr.capacity);\
            }\
            if (where < arr.size) {\
                memmove(&arr.data[where + 1], &arr.data[where], sizeof(arr.data[0]) * (arr.size - where));\
            }\
            arr.data[where] = val;\
            arr.size++;\
        }\
        else\
        {\
            da_append(arr,val);\
        }\
    }while(0)
//Deletes value {val} from {arr} dynamic array
#define da_delete(arr, val) \
    do { \
        if (arr.data != NULL) { \
            for (size_t _i_ = 0; _i_ < arr.size; _i_++) { \
                if (apulib_epsilon_close(arr.data[_i_],val)) { \
                    size_t _c_ = arr.size - 1 - _i_; \
                    if (_c_ > 0) { \
                        memmove(&arr.data[_i_], &arr.data[_i_ + 1], _c_ * sizeof(arr.data[0])); \
                    } \
                    arr.size--; \
                    if (arr.size == 0) { \
                        free(arr.data); \
                        arr.data = NULL; \
                        arr.capacity = 1; \
                    } \
                    break;\
                } \
            } \
        } \
    } while(0)
//Prints the values of {arr} in {form} form character
#define da_print(arr,form)\
    do{\
        printf("[");\
        for(size_t _i_=0;_i_<arr.size;_i_++) {\
            printf(form,arr.data[_i_]);\
            if(_i_!=arr.size-1)printf(", ");\
        }\
        printf("]\n");\
    }while(0)
//{out} is a copy of {arr} dynamic array
#define da_copy(arr,out)\
    do{\
        da_clear(out);\
        if(arr.data !=NULL)\
        {\
            out.size = arr.size;\
            out.capacity = arr.capacity;\
            apulib_realloc(out.data,sizeof(arr.data[0]) * out.capacity);\
            for(size_t _i_=0;_i_<arr.size;_i_++) out.data[_i_] = arr.data[_i_];\
        }\
    }while(0)
//Reduces {arr} dynamic array into {accumulator} variable using {func} function, if {cond} condition is true, with {start} starting value.
//If the condition is never true, then the {out_bool} variable is 0, else it is 1
#define da_reduce(arr,func,cond,start,accumulator,out_bool)\
    do{\
        accumulator = start;\
        out_bool = 0;\
        for(size_t _i_=0;_i_<arr.size;_i_++)\
            if(cond(arr.data[_i_]))\
            {\
                if(out_bool)\
                    accumulator = func(accumulator,arr.data[_i_]);\
                else \
                    accumulator = arr.data[_i_];\
                out_bool = 1;\
            }\
    }while(0)

//Adds the values of {arr} dynamic array into {out} variable
#define da_sum(arr,out) do{char _b_;da_reduce(arr,apulib_sum_helper,apulib_true_cond,0,out,_b_);}while(0)

//Finds the maximum value within {arr} dynamic array and outputs it into {out} variable
#define da_max(arr,out) do{char _b_;if(arr.data!= NULL) da_reduce(arr,apulib_max_helper,apulib_true_cond,arr.data[0],out,_b_);}while(0)

//Finds the minimum value within {arr} dynamic array and outputs it into {out} variable
#define da_min(arr,out) do{char _b_;if(arr.data!= NULL) da_reduce(arr,apulib_min_helper,apulib_true_cond,arr.data[0],out,_b_);}while(0)

//Adds the values of {arr} dynamic array, for elements that {cond} condition is true on to {out} variable if {out_bool} is true
#define da_sumif(arr,cond,out,out_bool) da_reduce(arr,apulib_sum_helper,cond,0,out,out_bool)

//Finds max value of {arr} dynamic array, for elements that {cond} condition is true on to {out} variable if {out_bool} is true
#define da_maxif(arr,cond,out,out_bool) do{out_bool=0;if(arr.data!= NULL) da_reduce(arr,apulib_max_helper,cond,arr.data[0],out,out_bool);}while(0)

//Finds the min value of {arr} dynamic array, for elements that {cond} condition is true on to {out} variable if {out_bool} is true
#define da_minif(arr,cond,out,out_bool) do{out_bool=0;if(arr.data!= NULL) da_reduce(arr,apulib_min_helper,cond,arr.data[0],out,out_bool);}while(0)

//Returns the amount of elements of {arr} dynamic array, that return true to {cond} condition, to {out} variable
#define da_count(arr,cond,out)\
    do{\
        out = 0;\
        for(size_t _i_=0;_i_<arr.size;_i_++) if(cond(arr.data[_i_])) out++;\
    }while(0)

//{out} is true if {val} value is inside {arr} dynamic array
#define da_contains(arr,val,out)\
    do{\
        out = 0;\
        for(size_t _i_=0;_i_<arr.size;_i_++) if(apulib_epsilon_close(arr.data[_i_],val))\
        {\
            out = 1;\
            break;\
        }\
    }while(0)

//{out} is the index of {val} value inside {arr} dynamic array, if it isn't inside, it returns -1
#define da_find(arr,val,out)\
    do{\
        out = -1;\
        for(size_t _i_=0;_i_<arr.size;_i_++) if(apulib_epsilon_close(arr.data[_i_],val))\
        {\
            out = _i_;\
            break;\
        }\
    }while(0)

//{out} is a dynamic array of the values of {arr} each mutated by the {func} function
#define da_map(arr,func,out)\
    do{\
        da_clear(out);\
        for(size_t _i_=0;_i_<arr.size;_i_++) da_append(out,func(arr.data[_i_]));\
    }while(0)

//{out} is a dynamic array of the values of {arr}, if {cond} condition is true for them
#define da_filter(arr,cond,out)\
    do{\
        da_clear(out);\
        for(size_t _i_=0;_i_<arr.size;_i_++) if(cond(arr.data[_i_])) da_append(out,arr.data[_i_]);\
    }while(0)

//Mutates the values of {arr} dynamic array, each mutated by the {func} function
#define da_map_self(arr,func)\
    do{\
        for(size_t _i_=0;_i_<arr.size;_i_++) arr.data[_i_] = func(arr.data[_i_]);\
    }while(0)

//Only keeps the values of {arr} dynamic array, if {cond} condition is true for them
#define da_filter_self(arr, cond)\
    do{\
        if(arr.data)\
        {\
            size_t _ind_ = 0;\
            for(size_t _i_ = 0; _i_ < arr.size; _i_++)\
            {\
                if(cond(arr.data[_i_]))\
                {\
                    arr.data[_ind_++] = arr.data[_i_];\
                }\
            }\
            arr.size = _ind_;\
            if(arr.size == 0)\
            {\
                da_clear(arr);\
            }\
        }\
    } while(0)

//Checks if any of the elements of {arr} dynamic array give true to {cond} condition
//Returns 0 if false, 1 if true into {out} parameter 
#define da_any(arr,cond,out)\
    do{\
        out = 0;\
        for(size_t _i_=0;_i_<arr.size;_i_++) if(cond(arr.data[_i_]))\
        {\
            out = 1;\
            break;\
        }\
    }while(0)
//Checks if all of the elements of {arr} dynamic array give true to {cond} condition
//Returns 0 if false, 1 if true into {out} parameter 
#define da_all(arr,cond,out)\
    do{\
        out = 1;\
        for(size_t _i_=0;_i_<arr.size;_i_++) if(!(cond(arr.data[_i_])))\
        {\
            out = 0;\
            break;\
        }\
    }while(0)
//Allows you to create for loops for {arr} dynamic array with {indexer_name} index name
#define da_forEach(arr,indexer_name) for(size_t indexer_name=0;indexer_name<arr.size;indexer_name++)

#define da_heap_sort_sink(arr,k,n)\
    do{\
        size_t _i_=k,_j_=2*k+1;\
        char _b_ = 1;\
        while(_j_ < n && _b_)\
        {\
            if(_j_+1<n && arr.data[_j_+1] > arr.data[_j_]) _j_++;\
            if(arr.data[_i_] < arr.data[_j_])\
            {\
                apulib_swap(arr.data[_i_],arr.data[_j_]);\
                _i_=_j_;\
                _j_ = (_j_ * 2)+1;\
            }\
            else _b_ = 0;\
        }\
    }while(0)

#define da_build_max_heap(arr) \
    for (size_t _k_ = (arr.size) / 2;_k_ > 0;) \
    {\
        _k_--;\
        da_heap_sort_sink(arr, _k_, arr.size);\
    }

//Uses heap_sort algorithm on {arr} dynamic array
#define da_unstable_sort(arr)\
    do{\
        da_build_max_heap(arr);\
        size_t _m_ = arr.size;\
        while(_m_>1)\
        {\
            _m_--;\
            apulib_swap(arr.data[0],arr.data[_m_]);\
            da_heap_sort_sink(arr,0,_m_);\
        }\
    }while(0)

//Uses merge_sort algorithm on {arr} dynamic array
#define da_stable_sort(arr)\
    do{\
        if(arr.size>1)\
        {\
            size_t _c_ = 1;\
            typeof(arr.data) _buffer_;\
            apulib_malloc(_buffer_,sizeof(arr.data[0]) * arr.capacity);\
            while(_c_ < arr.size)\
            {\
                size_t _l_ = 0;\
                while(_l_ < arr.size - 1)\
                {\
                    size_t _m_ = apulib_min_helper(_l_ + _c_ - 1,arr.size-1);\
                    size_t _re_ = apulib_min_helper(_l_ + 2 * _c_ - 1, arr.size-1);\
                    \
                    size_t _i_=_l_,_j_=_m_+1,_k_=_l_;\
                    while(_i_ <= _m_ && _j_ <= _re_)\
                        if(arr.data[_i_] < arr.data[_j_]) _buffer_[_k_++] = arr.data[_i_++];\
                        else _buffer_[_k_++] = arr.data[_j_++];\
                    \
                    if(_i_ <= _m_) for(;_i_<=_m_;_i_++) _buffer_[_k_++] = arr.data[_i_];\
                    else for(;_j_<=_re_;_j_++) _buffer_[_k_++] = arr.data[_j_];\
                    for(size_t _index_=_l_;_index_<=_re_;_index_++) arr.data[_index_] = _buffer_[_index_];\
                    \
                    _l_ += 2 * _c_;\
                }\
                _c_ *= 2;\
            }\
            free(_buffer_);\
         }\
    }while(0)

//Returns the last element of {arr} dynamic array, and removes it from it
#define da_pop(arr) arr.data[--arr.size]

//Returns the first element into {out} variable of {arr} dynamic array and removes it from it
#define da_shift(arr,out)\
    do{\
        if(arr.data)\
        {\
            out = arr.data[0];\
            memmove(&arr.data[0], &arr.data[1], (--arr.size) * sizeof(arr.data[0]));\
            if(arr.size==0) da_clear(arr);\
        }\
    }while(0)

#endif

#ifdef APULIB_LL_IMPL

//Declares a Linked List of {T} type, with the name {typename}
#define ll_declare(T, typename)\
    typedef struct typename{\
        T value;\
        struct typename* next;\
        struct typename* prev;\
    } typename;

//Creates a linked list with the name {name} of type {typename}, with the starting value of {start}
#define ll_create(typename,name,start)\
    typename* name = NULL;\
    apulib_malloc(name,sizeof(typename));\
    name->value = start;\
    name->next = NULL;\
    name->prev = NULL

#define ll_size(list,out)\
    do{\
        typeof(list) _ll_list_ = list;\
        out = 0;\
        while(_ll_list_)\
        {\
            out++;\
            _ll_list_=_ll_list_->next;\
        }\
    }while(0)

//Appends {val} value to the end of {list} Linked List, with the type {typename}
#define ll_append(typename,list,val)\
    do{\
        if(list)\
        {\
            typename* _ll_list_ = list;\
            while(_ll_list_ && _ll_list_->next)\
            {\
                _ll_list_=_ll_list_->next;\
            }\
            ll_create(typename,_ll_new_,val);\
            _ll_list_->next = _ll_new_;\
            _ll_new_->prev = _ll_list_;\
        }\
        else\
        {\
            ll_create(typename,_ll_list_helper_,val);\
            list = _ll_list_helper_;\
        }\
    }while(0)

//Prints the elements of {list} Linked List, with the printf form {form}
#define ll_print(list,form)\
    do{\
        typeof(list) _ll_list_ = list;\
        printf("[ ");\
        while(_ll_list_)\
        {\
            printf(form" ",_ll_list_->value);\
            _ll_list_ = _ll_list_->next;\
        }\
        printf("]\n");\
    }while(0)

//Inserts an element with {val} value into {list} list with type {typename} to {where} index
//If the list's size is less than {where}, then it will append {val} to {list}
#define ll_insert(typename,list,val,where)\
    do{\
        if(list)\
        {\
            if(where==0)\
            {\
                ll_create(typename,_ll_new_,val);\
                _ll_new_->next = list;\
                list->prev = _ll_new_;\
                list = _ll_new_;\
            }\
            else\
            {\
                typename* _ll_list_ = list;\
                for(size_t _i_=0;_i_<where-1 && _ll_list_->next;_i_++)\
                {\
                    _ll_list_= _ll_list_->next;\
                }\
                if(!_ll_list_->next)\
                {\
                    ll_create(typename,_ll_new_,val);\
                    _ll_list_->next = _ll_new_;\
                    _ll_new_->prev = _ll_list_;\
                }\
                else\
                {\
                    ll_create(typename,_ll_new_,val);\
                    typename* _ll_next_ = _ll_list_->next;\
                    _ll_list_->next = _ll_new_;\
                    _ll_new_->prev = _ll_list_;\
                    _ll_new_->next = _ll_next_;\
                    _ll_next_->prev = _ll_new_;\
                }\
            }\
        }\
        else ll_append(typename,list,val);\
    }while(0)

//Deletes {val} value from {list} Linked List, if the value is not found, the function doesn't do anything
#define ll_delete(list,val)\
    do{\
        typeof(list) _ll_list_ = list;\
        while(_ll_list_ && !(apulib_epsilon_close(_ll_list_-> value, val)))\
        {\
            _ll_list_ = _ll_list_->next;\
        }\
        if(_ll_list_)\
        {\
            if(_ll_list_->prev && _ll_list_->next)\
            {\
                typeof(list) _prev_ = _ll_list_->prev;\
                typeof(list) _next_ = _ll_list_->next;\
                _prev_->next = _next_;\
                _next_->prev = _prev_;\
                free(_ll_list_);\
            }\
            else if(_ll_list_->prev)\
            {\
                typeof(list) _prev_ = _ll_list_->prev;\
                _prev_->next = NULL;\
                free(_ll_list_);\
            }\
            else if(_ll_list_->next)\
            {\
                typeof(list) _next_ = _ll_list_->next;\
                _next_->prev = NULL;\
                free(_ll_list_);\
                list = _next_;\
            }\
            else\
            {\
                free(_ll_list_);\
                list = NULL;\
            }\
        }\
    }while(0)

//Clears {list} Linked List, used as free as well
#define ll_clear(list)\
    do{\
        while(list)\
        {\
            typeof(list) _curr_ = list;\
            list = list->next;\
            free(_curr_);\
        }\
    }while(0)

//Gets the value at {index} index inside {list} Linked List, and copies the value into {out} variable
//If the index is out of bounds the function doesn't do anything
#define ll_get(list,index,out)\
    do{\
        typeof(list) _ll_list_ = list;\
        size_t _i_ = 0;\
        while(_ll_list_ && _i_ != index)\
        {\
            _ll_list_ = _ll_list_->next;\
            _i_++;\
        }\
        if(_ll_list_) out = _ll_list_->value;\
    }while(0)

//Copies {list} Linked List into {out} Linked List both of {typename} type
#define ll_copy(typename,list,out)\
    do{\
        ll_clear(out);\
        if(list)\
        {\
            ll_create(typename,_ll_new_list_,list->value);\
            typename* _ll_list_ = list->next;\
            typename* _ll_new_list_second_ = _ll_new_list_;\
            while(_ll_list_)\
            {\
                ll_create(typename,_ll_new_element_,_ll_list_->value);\
                _ll_new_list_second_-> next = _ll_new_element_;\
                _ll_new_element_->prev = _ll_new_list_second_;\
                _ll_new_list_second_ = _ll_new_element_;\
                _ll_list_= _ll_list_->next;\
            }\
            out = _ll_new_list_;\
        }\
    }while(0)

//Reduces {list} Linked List into {accumulator} variable using {func} function, if {cond} condition is true, with {start} starting value.
//If the condition is never true, then the {out_bool} variable is 0, else it is 1
#define ll_reduce(list,func,cond,start,accumulator,out_bool)\
    do{\
        typeof(list) _ll_list_ = list;\
        accumulator = start;\
        out_bool = 0;\
        while(_ll_list_)\
        {\
            if(cond(_ll_list_->value))\
            {\
                if(out_bool)\
                    accumulator = func(accumulator,_ll_list_->value);\
                else \
                    accumulator = _ll_list_->value;\
                out_bool = 1;\
            }\
            _ll_list_= _ll_list_->next;\
        }\
    }while(0)

//Adds the values of {list} Linked List into {out} variable
#define ll_sum(list,out) do{char _b_;ll_reduce(list,apulib_sum_helper,apulib_true_cond,0,out,_b_);}while(0)

//Finds the maximum value within {list} Linked List and outputs it into {out} variable
#define ll_max(list,out) do{char _b_;if(list) ll_reduce(list,apulib_max_helper,apulib_true_cond,list->value,out,_b_);}while(0)

//Finds the minimum value within {list} Linked List and outputs it into {out} variable
#define ll_min(list,out) do{char _b_;if(list) ll_reduce(list,apulib_min_helper,apulib_true_cond,list->value,out,_b_);}while(0)

//Adds the values of {list} Linked List, for elements that {cond} condition is true on to {out} variable
//If the condition is never true, then the {out_bool} variable is 0, else it is 1
#define ll_sumif(list,cond,out,out_bool) ll_reduce(list,apulib_sum_helper,cond,0,out,out_bool)

//Finds max value of {list} Linked List, for elements that {cond} condition is true on to {out} variable
//If the condition is never true, then the {out_bool} variable is 0, else it is 1
#define ll_maxif(list,cond,out,out_bool) ll_reduce(list,apulib_max_helper,cond,0,out,out_bool)

//Finds the min value of {list} Linked List, for elements that {cond} condition is true on to {out} variable
//If the condition is never true, then the {out_bool} variable is 0, else it is 1
#define ll_minif(list,cond,out,out_bool) ll_reduce(list,apulib_min_helper,cond,0,out,out_bool)

//Counts the elements of {list} Linked List into {out} variable, that {cond} condition gives true to
#define ll_count(list,cond,out)\
    do{\
        typeof(list) _ll_list_ = list;\
        out = 0;\
        while(_ll_list_)\
        {\
            if(cond(_ll_list_->value)) out++;\
            _ll_list_ = _ll_list_->next;\
        }\
    }while(0)

//Return true or false to {out_bool} if {list} Linked List contains {val} value
#define ll_contains(list,val,out_bool)\
    do{\
        typeof(list) _ll_list_ = list;\
        out_bool = 0;\
        while(_ll_list_ && !(apulib_epsilon_close(_ll_list_->value,val)))\
            _ll_list_ = _ll_list_->next;\
        if(_ll_list_) out_bool = 1;\
    }while(0)

//Uses {func} function on all elements on a copy of {list} Linked List of {typename} type, and returns it to {out} Linked List
#define ll_map(typename,list,func,out)\
    do{\
        ll_clear(out);\
        if(list)\
        {\
            ll_create(typename,_ll_new_list_,func(list->value));\
            typename* _ll_list_ = list->next;\
            typename* _ll_new_list_second_ = _ll_new_list_;\
            while(_ll_list_)\
            {\
                ll_create(typename,_ll_new_element_,func(_ll_list_->value));\
                _ll_new_list_second_-> next = _ll_new_element_;\
                _ll_new_element_->prev = _ll_new_list_second_;\
                _ll_new_list_second_ = _ll_new_element_;\
                _ll_list_= _ll_list_->next;\
            }\
            out = _ll_new_list_;\
        }\
    }while(0)

//Keeps the elements that return true with {cond} condition
//on all elements on a copy of {list} Linked List of {typename} type, and returns it to {out} Linked List
#define ll_filter(typename,list,cond,out)\
    do{\
        ll_clear(out);\
        if(list)\
        {\
            typename* _ll_new_list_ = NULL;\
            typename* _ll_list_ = list;\
            typename* _ll_new_list_second_ = NULL;\
            while(_ll_list_)\
            {\
                if(cond(_ll_list_->value))\
                {\
                    if(!_ll_new_list_)\
                    {\
                        ll_create(typename,_ll_list_helper_,_ll_list_->value);\
                        _ll_new_list_ = _ll_list_helper_;\
                        _ll_new_list_second_ = _ll_new_list_;\
                    }\
                    else\
                    {\
                        ll_create(typename,_ll_new_element_,_ll_list_->value);\
                        _ll_new_list_second_->next = _ll_new_element_;\
                        _ll_new_element_->prev = _ll_new_list_second_;\
                        _ll_new_list_second_ = _ll_new_element_;\
                    }\
                }\
                _ll_list_= _ll_list_->next;\
            }\
            out = _ll_new_list_;\
        }\
    }while(0)

//Uses {func} function on all elements on {list} Linked List of {typename} type
#define ll_map_self(typename,list,func)\
    do{\
        if(list)\
        {\
            typename* _ll_list_ = list;\
            while(_ll_list_)\
            {\
                _ll_list_->value = func(_ll_list_->value);\
                _ll_list_=_ll_list_->next;\
            }\
        }\
    }while(0)

//Keeps the elements that return true with {cond} condition on all elements on {list} Linked List of {typename} type
#define ll_filter_self(typename, list, cond)\
    do{\
        typename* _ll_list_ = list;\
        while(_ll_list_)\
        {\
            typename* _next_node_ = _ll_list_->next;\
            if(!cond(_ll_list_->value))\
            {\
                if(_ll_list_->prev) _ll_list_->prev->next = _ll_list_->next;\
                else list = _ll_list_->next;\
                if(_ll_list_->next) _ll_list_->next->prev = _ll_list_->prev;\
                free(_ll_list_);\
            }\
            _ll_list_ = _next_node_;\
        }\
    }while(0)

//Return true or false to {out_bool} if at least one element of {list} return true to {cond} condition 
#define ll_any(list,cond,out_bool)\
    do{\
        typeof(list) _ll_list_ = list;\
        out_bool = 0;\
        while(_ll_list_ && !(cond(_ll_list_->value)))\
            _ll_list_ = _ll_list_->next;\
        if(_ll_list_) out_bool = 1;\
    }while(0)

//Return true or false to {out_bool} if all elements of {list} return true to {cond} condition
#define ll_all(list,cond,out_bool)\
    do{\
        typeof(list) _ll_list_ = list;\
        out_bool = 1;\
        while(_ll_list_ && cond(_ll_list_->value))\
            _ll_list_ = _ll_list_->next;\
        if(_ll_list_) out_bool = 0;\
    }while(0)

//Uses Merge Sort on {list} Linked List of type {typename}
#define ll_sort(typename,list)\
    do{\
        if(list)\
        {\
            size_t _c_=1;\
            size_t _size_=0;\
            ll_size(list,_size_);\
            while(_c_ < _size_)\
            {\
                size_t _l_ = 0;\
                while(_l_ < _size_ - 1)\
                {\
                    size_t _m_ = apulib_min_helper(_l_ + _c_ - 1,_size_ - 1);\
                    size_t _re_ = apulib_min_helper(_l_ + 2 * _c_ - 1, _size_ - 1);\
                    typeof(list) _left_ = list, _right_ = list;\
                    typeof(list->value) _value_;\
                    size_t _i_ = 0;\
                    while(_i_++ < _l_) _left_ = _left_->next;\
                    _i_=0;\
                    while(_i_++ < _m_+1) _right_ = _right_->next;\
                    typeof(list) _list_helper_ = _left_;\
                    size_t _j_=_l_,_k_=_m_+1;\
                    if(_left_->value < _right_->value)\
                    {\
                        _value_ = _left_->value;\
                        _left_=_left_->next;\
                        _j_++;\
                    }\
                    else\
                    {\
                        _value_ = _right_->value;\
                        _right_ = _right_->next;\
                        _k_++;\
                    }\
                    ll_create(typename,_buffer_,_value_);\
                    while(_left_ && _right_ && _j_<=_m_ && _k_<=_re_)\
                    {\
                        if(_left_->value < _right_->value)\
                        {\
                            ll_append(typename,_buffer_,_left_->value);\
                            _left_ = _left_->next;\
                            _j_++;\
                        }\
                        else\
                        {\
                            ll_append(typename,_buffer_,_right_->value);\
                            _right_ = _right_->next;\
                            _k_++;\
                        }\
                    }\
                    while(_left_ && _list_helper_ && _j_<=_m_)\
                    {\
                        ll_append(typename,_buffer_,_left_->value);\
                        _left_ = _left_->next;\
                        _j_++;\
                    }\
                    while(_right_ && _list_helper_ && _k_<=_re_)\
                    {\
                        ll_append(typename,_buffer_,_right_->value);\
                        _right_= _right_->next;\
                        _k_++;\
                    }\
                    typeof(_buffer_) _buffer_start_ = _buffer_;\
                    while(_buffer_ && _list_helper_)\
                    {\
                        _list_helper_->value = _buffer_->value;\
                        _list_helper_ = _list_helper_->next;\
                        _buffer_ = _buffer_->next;\
                    }\
                    ll_clear(_buffer_start_);\
                    _l_ += 2 * _c_;\
                }\
                _c_ *= 2;\
            }\
        }\
    }while(0)

#endif

#ifdef APULIB_SET_IMPL

//Declares a set of {T} type, with the name {typename}
#define set_declare(T,typename)\
    typedef struct typename\
    {\
        T* data;\
        size_t size;\
        size_t capacity;\
    } typename

//Creates a new set with the type {typename} and with the name {name}
#define set_create(typename,name)\
    typename name;\
    name.size = 0;\
    name.capacity = 1;\
    name.data = NULL

//Inserts a {val} value into {set} set
#define set_insert(set,val)\
    do{\
        if(set.data == NULL)\
        {\
            apulib_malloc(set.data,sizeof(set.data[0]));\
            set.data[0] = val;\
            set.size++;\
        }\
        else\
        {\
            if(set.size>=set.capacity)\
            {\
                set.capacity *=2;\
                apulib_realloc(set.data,sizeof(set.data[0]) * set.capacity);\
            }\
            size_t _l_=0,_r_=set.size;\
            while(_l_ < _r_)\
            {\
                size_t _m_ = _l_ + (_r_ - _l_) / 2;\
                if(set.data[_m_] < val) _l_ = _m_+1;\
                else _r_=_m_;\
            }\
            if(set.size == _l_ || !(apulib_epsilon_close(set.data[_l_], val)))\
            {\
                if(set.size != _l_)\
                    memmove(&set.data[_l_+1],&set.data[_l_],sizeof(set.data[0]) * (set.size-_l_));\
                set.data[_l_] = val;\
                set.size++;\
            }\
        }\
    }while(0)

//Prints out {set} set with the form character {form}
#define set_print(set,form)\
    do{\
        printf("{");\
        for(size_t _i_=0;_i_<set.size;_i_++) {\
            printf(form,set.data[_i_]);\
            if(_i_!=set.size-1)printf(", ");\
        }\
        printf("}\n");\
    }while(0)

//Deletes {val} value from {set} set if present
#define set_delete(set,val)\
    do{\
        if(set.data!=NULL)\
        {\
            size_t _l_=0,_r_=set.size;\
            while(_l_ < _r_)\
            {\
                size_t _m_ = _l_ + (_r_ - _l_) / 2;\
                if(set.data[_m_] < val) _l_ = _m_+1;\
                else _r_=_m_;\
            }\
            if(_l_ < set.size && apulib_epsilon_close(set.data[_l_],val))\
            {\
                if(set.size != _l_)\
                {\
                    memmove(&set.data[_l_],&set.data[_l_+1],sizeof(set.data[0]) * (set.size-_l_-1));\
                    set.size--;\
                    if(set.size==0)\
                    {\
                        set_clear(set);\
                    }\
                }\
            }\
        }\
    }while(0)

//Checks if a {val} value is inside {set} set, and returns the bool value into {out} variable
#define set_contains(set,val,out)\
    do{\
        out = 0;\
        if(set.data)\
        {\
            size_t _l_=0,_r_=set.size;\
            while(_l_ < _r_)\
            {\
                size_t _m_ = _l_ + (_r_ - _l_) / 2;\
                if(set.data[_m_] < val) _l_ = _m_+1;\
                else _r_=_m_;\
            }\
            if(_l_ < set.size && apulib_epsilon_close(set.data[_l_],val))\
            {\
                out = 1;\
            }\
        }\
    }while(0)

//Clears {set} set, and frees all the data
#define set_clear(set)\
    do{\
        set.size=0;\
        set.capacity=1;\
        free(set.data);\
        set.data = NULL;\
    }while(0)

//Copies the values of {set} set into {out} set
#define set_copy(set,out)\
    do{\
        set_clear(out);\
        if(set.data !=NULL)\
        {\
            out.size = set.size;\
            out.capacity = set.capacity;\
            apulib_realloc(out.data,sizeof(set.data[0]) * out.capacity);\
            for(size_t _i_=0;_i_<set.size;_i_++) out.data[_i_] = set.data[_i_];\
        }\
    }while(0)

//Reduces the values of {set} set into {accumulator} variable, if {out_bool} is true
//The reduction is done with {func} function, if {cond} condition is true, with the start value of {start}
#define set_reduce(set,func,cond,start,accumulator,out_bool)\
    do{\
        accumulator = start;\
        out_bool = 0;\
        for(size_t _i_=0;_i_<set.size;_i_++)\
            if(cond(set.data[_i_]))\
            {\
                if(out_bool)\
                    accumulator = func(accumulator,set.data[_i_]);\
                else \
                    accumulator = set.data[_i_];\
                out_bool = 1;\
            }\
    }while(0)

//Adds the values of {set} set into {out} variable
#define set_sum(set,out) do{char _b_;set_reduce(set,apulib_sum_helper,apulib_true_cond,0,out,_b_);}while(0)

//Finds the maximum value within {set} set and outputs it into {out} variable
#define set_max(set,out) do{char _b_;if(set.data!= NULL) set_reduce(set,apulib_max_helper,apulib_true_cond,set.data[0],out,_b_);}while(0)

//Finds the minimum value within {set} set and outputs it into {out} variable
#define set_min(set,out) do{char _b_;if(set.data!= NULL) set_reduce(set,apulib_min_helper,apulib_true_cond,set.data[0],out,_b_);}while(0)

//Adds the values of {set} set, for elements that {cond} condition is true on to {out} variable if {out_bool} is true
#define set_sumif(set,cond,out,out_bool) set_reduce(set,apulib_sum_helper,cond,0,out,out_bool)

//Finds max value of {set} set, for elements that {cond} condition is true on to {out} variable if {out_bool} is true
#define set_maxif(set,cond,out,out_bool) do{out_bool=0;if(set.data!= NULL) set_reduce(set,apulib_max_helper,cond,set.data[0],out,out_bool);}while(0)

//Finds the min value of {set} set, for elements that {cond} condition is true on to {out} variable if {out_bool} is true
#define set_minif(set,cond,out,out_bool) do{out_bool=0;if(set.data!= NULL) set_reduce(set,apulib_min_helper,cond,set.data[0],out,out_bool);}while(0)

//Returns the amount of elements of {set} set, that return true to {cond} condition, to {out} variable
#define set_count(set,cond,out)\
    do{\
        out = 0;\
        for(size_t _i_=0;_i_<set.size;_i_++) if(cond(set.data[_i_])) out++;\
    }while(0)

//{out} is a set of the values of {set} each mutated by the {func} function
#define set_map(set,func,out)\
    do{\
        set_clear(out);\
        for(size_t _i_=0;_i_<set.size;_i_++) set_insert(out,func(set.data[_i_]));\
    }while(0)

//{out} is a set of the values of {set}, if {cond} condition is true for them
#define set_filter(set,cond,out)\
    do{\
        set_clear(out);\
        for(size_t _i_=0;_i_<set.size;_i_++) if(cond(set.data[_i_])) set_insert(out,set.data[_i_]);\
    }while(0)

//Mutates the values of {set} set, each mutated by the {func} function
//WARNING: THE {func} FUNCTION CAN RUIN THE SORTED NATURE OF THE SET.
//IF YOU USE A FUNCTION THAT ISN'T MONOTONIC, THEN PLEASE SORT THE SET WITH DA_STABLE_SORT
#define set_map_self(set,func)\
    do{\
        for(size_t _i_=0;_i_<set.size;_i_++) set.data[_i_] = func(set.data[_i_]);\
    }while(0)

//Only keeps the values of {set} set, if {cond} condition is true for them
#define set_filter_self(set, cond)\
    do{\
        if(set.data)\
        {\
            size_t _ind_ = 0;\
            for(size_t _i_ = 0; _i_ < set.size; _i_++)\
            {\
                if(cond(set.data[_i_]))\
                {\
                    set.data[_ind_++] = set.data[_i_];\
                }\
            }\
            set.size = _ind_;\
            if(set.size == 0)\
            {\
                set_clear(set);\
            }\
        }\
    } while(0)

//Checks if any of the elements of {set} set give true to {cond} condition
//Returns 0 if false, 1 if true into {out} parameter 
#define set_any(set,cond,out)\
    do{\
        out = 0;\
        for(size_t _i_=0;_i_<set.size;_i_++) if(cond(set.data[_i_]))\
        {\
            out = 1;\
            break;\
        }\
    }while(0)
//Checks if all of the elements of {set} set give true to {cond} condition
//Returns 0 if false, 1 if true into {out} parameter 
#define set_all(set,cond,out)\
    do{\
        out = 1;\
        for(size_t _i_=0;_i_<set.size;_i_++) if(!(cond(set.data[_i_])))\
        {\
            out = 0;\
            break;\
        }\
    }while(0)
//Allows you to create for loops for {set} set with {indexer_name} index name
#define set_forEach(set,indexer_name) for(size_t indexer_name=0;indexer_name<set.size;indexer_name++)

#endif

#ifdef APULIB_MAP_IMPL

//Declares a map with {K} key type, {V} value type and with the name {typename}
#define map_declare(K,V,typename)\
    typedef struct typename\
    {\
        K* keys;\
        V* data;\
        size_t size;\
        size_t capacity;\
    } typename

//Creates an instance of map with {name} name of type {typename}
#define map_create(typename,name)\
    typename name;\
    name.size = 0;\
    name.capacity = 1;\
    name.data = NULL;\
    name.keys = NULL

//Inserts the {key},{val} pair into {map} map, if the key is not present
#define map_insert(map,key,val)\
    do{\
        if(map.keys == NULL)\
        {\
            apulib_malloc(map.keys,sizeof(map.keys[0]));\
            apulib_malloc(map.data,sizeof(map.data[0]));\
            map.keys[0] = key;\
            map.data[0] = val;\
            map.size++;\
        }\
        else\
        {\
            if(map.size>=map.capacity)\
            {\
                map.capacity *=2;\
                apulib_realloc(map.keys,sizeof(map.keys[0]) * map.capacity);\
                apulib_realloc(map.data,sizeof(map.data[0]) * map.capacity);\
            }\
            size_t _l_=0,_r_=map.size;\
            while(_l_ < _r_)\
            {\
                size_t _m_ = _l_ + (_r_ - _l_) / 2;\
                if(map.keys[_m_] < key) _l_ = _m_+1;\
                else _r_=_m_;\
            }\
            if(map.size == _l_ || !(apulib_epsilon_close(map.keys[_l_], key)))\
            {\
                if(map.size != _l_)\
                {\
                    memmove(&map.keys[_l_+1],&map.keys[_l_],sizeof(map.keys[0]) * (map.size-_l_));\
                    memmove(&map.data[_l_+1],&map.data[_l_],sizeof(map.data[0]) * (map.size-_l_));\
                }\
                map.keys[_l_] = key;\
                map.data[_l_] = val;\
                map.size++;\
            }\
        }\
    }while(0)

//Prints out {map} map, with the form characters: {keyform},{valform}
#define map_print(map,keyform,valform)\
    do{\
        printf("{");\
        for(size_t _i_=0;_i_<map.size;_i_++) {\
            printf(keyform": "valform,map.keys[_i_],map.data[_i_]);\
            if(_i_!=map.size-1)printf(", ");\
        }\
        printf("}\n");\
    }while(0)

//Deletes a {key},value pair from {map} map
#define map_delete(map,key)\
    do{\
        if(map.keys != NULL)\
        {\
            size_t _l_=0,_r_=map.size;\
            while(_l_ < _r_)\
            {\
                size_t _m_ = _l_ + (_r_ - _l_) / 2;\
                if(map.keys[_m_] < key) _l_ = _m_+1;\
                else _r_=_m_;\
            }\
            if(_l_ < map.size && apulib_epsilon_close(map.keys[_l_], key))\
            {\
                memmove(&map.keys[_l_],&map.keys[_l_+1],sizeof(map.keys[0]) * (map.size-_l_-1));\
                memmove(&map.data[_l_],&map.data[_l_+1],sizeof(map.data[0]) * (map.size-_l_-1));\
                map.size--;\
                if(map.size==0)\
                {\
                    map_clear(map);\
                }\
            }\
        }\
    }while(0)

//Gets a value of {map} map, of key {key}, if the key is present, into {out} variable
#define map_get(map,key,out)\
    do{\
        if(map.keys != NULL)\
        {\
            size_t _l_=0,_r_=map.size;\
            while(_l_ < _r_)\
            {\
                size_t _m_ = _l_ + (_r_ - _l_) / 2;\
                if(map.keys[_m_] < key) _l_ = _m_+1;\
                else _r_=_m_;\
            }\
            if(_l_ < map.size && apulib_epsilon_close(map.keys[_l_], key)) out = map.data[_l_];\
        }\
    }while(0)

//Checks if {map} map contains {key} key, and outputs the result into {out} variable
#define map_contains_key(map,key,out)\
    do{\
        out = 0;\
        if(map.data)\
        {\
            size_t _l_=0,_r_=map.size;\
            while(_l_ < _r_)\
            {\
                size_t _m_ = _l_ + (_r_ - _l_) / 2;\
                if(map.keys[_m_] < key) _l_ = _m_+1;\
                else _r_=_m_;\
            }\
            if(_l_ < map.size && apulib_epsilon_close(map.keys[_l_], key))\
            {\
                out = 1;\
            }\
        }\
    }while(0)

//Checks if {map} map contains {val} value, and outputs the result into {out} variable
#define map_contains_val(map,val,out)\
    do{\
        out = 0;\
        for(size_t _i_=0;_i_<map.size;_i_++) if(apulib_epsilon_close(map.data[_i_],val))\
        {\
            out = 1;\
            break;\
        }\
    }while(0)

//Clears the elements of {map} map, and frees all resources
#define map_clear(map)\
    do{\
        map.size=0;\
        map.capacity=1;\
        free(map.data);\
        free(map.keys);\
        map.data = NULL;\
        map.keys = NULL;\
    }while(0)

//Copies {map} map, into {out} map
#define map_copy(map,out)\
    do{\
        map_clear(out);\
        if(map.data !=NULL)\
        {\
            out.size = map.size;\
            out.capacity = map.capacity;\
            apulib_realloc(out.data,sizeof(map.data[0]) * map.capacity);\
            apulib_realloc(out.keys,sizeof(map.keys[0]) * map.capacity);\
            for(size_t _i_=0;_i_<map.size;_i_++)\
            {\
                out.data[_i_] = map.data[_i_];\
                out.keys[_i_] = map.keys[_i_];\
            }\
        }\
    }while(0)

//Reduces the values of {map} map into {accumulator} variable, if {out_bool} is true
//The reduction is done with {func} function, if {cond} condition is true, with the start value of {start}
#define map_reduce(map,func,cond,start,accumulator,out_bool)\
    do{\
        accumulator = start;\
        out_bool = 0;\
        for(size_t _i_=0;_i_<map.size;_i_++)\
            if(cond(map.data[_i_]))\
            {\
                if(out_bool)\
                    accumulator = func(accumulator,map.data[_i_]);\
                else \
                    accumulator = map.data[_i_];\
                out_bool = 1;\
            }\
    }while(0)

//Adds the values of {map} map into {out} variable
#define map_sum(map,out) do{char _b_;map_reduce(map,apulib_sum_helper,apulib_true_cond,0,out,_b_);}while(0)

//Finds the maximum value within {map} map and outputs it into {out} variable
#define map_max(map,out) do{char _b_;if(map.data!= NULL) map_reduce(map,apulib_max_helper,apulib_true_cond,map.data[0],out,_b_);}while(0)

//Finds the minimum value within {map} map and outputs it into {out} variable
#define map_min(map,out) do{char _b_;if(map.data!= NULL) map_reduce(map,apulib_min_helper,apulib_true_cond,map.data[0],out,_b_);}while(0)

//Adds the values of {map} map, for elements that {cond} condition is true on to {out} variable if {out_bool} is true
#define map_sumif(map,cond,out,out_bool) map_reduce(map,apulib_sum_helper,cond,0,out,out_bool)

//Finds max value of {map} map, for elements that {cond} condition is true on to {out} variable if {out_bool} is true
#define map_maxif(map,cond,out,out_bool) do{out_bool=0;if(map.data!= NULL) map_reduce(map,apulib_max_helper,cond,map.data[0],out,out_bool);}while(0)

//Finds the min value of {map} map, for elements that {cond} condition is true on to {out} variable if {out_bool} is true
#define map_minif(map,cond,out,out_bool) do{out_bool=0;if(map.data!= NULL) map_reduce(map,apulib_min_helper,cond,map.data[0],out,out_bool);}while(0)

//Returns the amount of elements of {map} map, that return true to {cond} condition, to {out} variable
#define map_count(map,cond,out)\
    do{\
        out = 0;\
        for(size_t _i_=0;_i_<map.size;_i_++) if(cond(map.data[_i_])) out++;\
    }while(0)

//{out} is a map of the values of {map}, if {cond} condition is true for them
#define map_filter(map,cond,out)\
    do{\
        map_clear(out);\
        for(size_t _i_=0;_i_<map.size;_i_++) if(cond(map.data[_i_])) map_insert(out,map.keys[_i_],map.data[_i_]);\
    }while(0)

//Only keeps the values of {map} map, if {cond} condition is true for them
#define map_filter_self(map, cond)\
    do{\
        if(map.data)\
        {\
            size_t _ind_ = 0;\
            for(size_t _i_ = 0; _i_ < map.size; _i_++)\
            {\
                if(cond(map.data[_i_]))\
                {\
                    map.data[_ind_] = map.data[_i_];\
                    map.keys[_ind_++] = map.keys[_i_];\
                }\
            }\
            map.size = _ind_;\
            if(map.size == 0)\
            {\
                map_clear(map);\
            }\
        }\
    } while(0)

//Checks if any of the elements of {map} map give true to {cond} condition
//Returns 0 if false, 1 if true into {out} parameter 
#define map_any(map,cond,out)\
    do{\
        out = 0;\
        for(size_t _i_=0;_i_<map.size;_i_++) if(cond(map.data[_i_]))\
        {\
            out = 1;\
            break;\
        }\
    }while(0)
//Checks if all of the elements of {map} map give true to {cond} condition
//Returns 0 if false, 1 if true into {out} parameter 
#define map_all(map,cond,out)\
    do{\
        out = 1;\
        for(size_t _i_=0;_i_<map.size;_i_++) if(!(cond(map.data[_i_])))\
        {\
            out = 0;\
            break;\
        }\
    }while(0)

#endif

#endif
