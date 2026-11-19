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
        typeof(a) tmp=a;\
        a = b;\
        b = tmp;\
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
            for (size_t i = 0; i < arr.size; i++) { \
                if (apulib_epsilon_close(arr.data[i],val)) { \
                    size_t c = arr.size - 1 - i; \
                    if (c > 0) { \
                        memmove(&arr.data[i], &arr.data[i + 1], c * sizeof(arr.data[0])); \
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
        for(size_t i=0;i<arr.size;i++) {\
            printf(form,arr.data[i]);\
            if(i!=arr.size-1)printf(", ");\
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
            for(size_t i=0;i<arr.size;i++) out.data[i] = arr.data[i];\
        }\
    }while(0)
//Reduces {arr} dynamic array into {accumulator} variable using {func} function, if {cond} condition is true, with {start} starting value.
//If the condition is never true, then the {out_bool} variable is 0, else it is 1
#define da_reduce(arr,func,cond,start,accumulator,out_bool)\
    do{\
        accumulator = start;\
        out_bool = 0;\
        for(size_t i=0;i<arr.size;i++)\
            if(cond(arr.data[i]))\
            {\
                if(out_bool)\
                    accumulator = func(accumulator,arr.data[i]);\
                else \
                    accumulator = arr.data[i];\
                out_bool = 1;\
            }\
    }while(0)

//Adds the values of {arr} dynamic array into {out} variable
#define da_sum(arr,out) do{char b;da_reduce(arr,apulib_sum_helper,apulib_true_cond,0,out,b);}while(0)

//Finds the maximum value within {arr} dynamic array and outputs it into {out} variable
#define da_max(arr,out) do{char b;if(arr.data!= NULL) da_reduce(arr,apulib_max_helper,apulib_true_cond,arr.data[0],out,b);}while(0)

//Finds the minimum value within {arr} dynamic array and outputs it into {out} variable
#define da_min(arr,out) do{char b;if(arr.data!= NULL) da_reduce(arr,apulib_min_helper,apulib_true_cond,arr.data[0],out,b);}while(0)

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
        for(size_t i=0;i<arr.size;i++) if(cond(arr.data[i])) out++;\
    }while(0)

//{out} is true if {val} value is inside {arr} dynamic array
#define da_contains(arr,val,out)\
    do{\
        out = 0;\
        for(size_t i=0;i<arr.size;i++) if(apulib_epsilon_close(arr.data[i],val))\
        {\
            out = 1;\
            break;\
        }\
    }while(0)

//{out} is the index of {val} value inside {arr} dynamic array, if it isn't inside, it returns -1
#define da_find(arr,val,out)\
    do{\
        out = -1;\
        for(size_t i=0;i<arr.size;i++) if(apulib_epsilon_close(arr.data[i],val))\
        {\
            out = i;\
            break;\
        }\
    }while(0)

//{out} is a dynamic array of the values of {arr} each mutated by the {func} function
#define da_map(arr,func,out)\
    do{\
        da_clear(out);\
        for(size_t i=0;i<arr.size;i++) da_append(out,func(arr.data[i]));\
    }while(0)

//{out} is a dynamic array of the values of {arr}, if {cond} condition is true for them
#define da_filter(arr,cond,out)\
    do{\
        da_clear(out);\
        for(size_t i=0;i<arr.size;i++) if(cond(arr.data[i])) da_append(out,arr.data[i]);\
    }while(0)

//Mutates the values of {arr} dynamic array, each mutated by the {func} function
#define da_map_self(arr,func)\
    do{\
        for(size_t i=0;i<arr.size;i++) arr.data[i] = func(arr.data[i]);\
    }while(0)

//Only keeps the values of {arr} dynamic array, if {cond} condition is true for them
#define da_filter_self(arr, cond)\
    do{\
        if(arr.data)\
        {\
            size_t ind = 0;\
            for(size_t i = 0; i < arr.size; i++)\
            {\
                if(cond(arr.data[i]))\
                {\
                    arr.data[ind++] = arr.data[i];\
                }\
            }\
            arr.size = ind;\
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
        for(size_t i=0;i<arr.size;i++) if(cond(arr.data[i]))\
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
        for(size_t i=0;i<arr.size;i++) if(!(cond(arr.data[i])))\
        {\
            out = 0;\
            break;\
        }\
    }while(0)
//Allows you to create for loops for {arr} dynamic array with {indexer_name} index name
#define da_forEach(arr,indexer_name) for(size_t indexer_name=0;indexer_name<arr.size;indexer_name++)

#define da_heap_sort_sink(arr,k,n)\
    do{\
        size_t i=k,j=2*k+1;\
        char b = 1;\
        while(j < n && b)\
        {\
            if(j+1<n && arr.data[j+1] > arr.data[j]) j++;\
            if(arr.data[i] < arr.data[j])\
            {\
                apulib_swap(arr.data[i],arr.data[j]);\
                i=j;\
                j = (j * 2)+1;\
            }\
            else b = 0;\
        }\
    }while(0)

#define da_build_max_heap(arr) \
    for (size_t k = (arr.size) / 2;k > 0;) \
    {\
        k--;\
        da_heap_sort_sink(arr, k, arr.size);\
    }

//Uses heap_sort algorithm on {arr} dynamic array
#define da_unstable_sort(arr)\
    do{\
        da_build_max_heap(arr);\
        size_t m = arr.size;\
        while(m>1)\
        {\
            m--;\
            apulib_swap(arr.data[0],arr.data[m]);\
            da_heap_sort_sink(arr,0,m);\
        }\
    }while(0)

//Uses merge_sort algorithm on {arr} dynamic array
#define da_stable_sort(arr)\
    do{\
        if(arr.size>1)\
        {\
            size_t c = 1;\
            typeof(arr.data) buffer;\
            apulib_malloc(buffer,sizeof(arr.data[0]) * arr.capacity);\
            while(c < arr.size)\
            {\
                size_t l = 0;\
                while(l < arr.size - 1)\
                {\
                    size_t m = apulib_min_helper(l + c - 1,arr.size-1);\
                    size_t re = apulib_min_helper(l + 2 * c - 1, arr.size-1);\
                    \
                    size_t i=l,j=m+1,k=l;\
                    while(i <= m && j <= re)\
                        if(arr.data[i] < arr.data[j]) buffer[k++] = arr.data[i++];\
                        else buffer[k++] = arr.data[j++];\
                    \
                    if(i <= m) for(;i<=m;i++) buffer[k++] = arr.data[i];\
                    else for(;j<=re;j++) buffer[k++] = arr.data[j];\
                    for(size_t index=l;index<=re;index++) arr.data[index] = buffer[index];\
                    \
                    l += 2 * c;\
                }\
                c *= 2;\
            }\
            free(buffer);\
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
        typeof(list) ll_list = list;\
        out = 0;\
        while(ll_list)\
        {\
            out++;\
            ll_list=ll_list->next;\
        }\
    }while(0)

//Appends {val} value to the end of {list} Linked List, with the type {typename}
#define ll_append(typename,list,val)\
    do{\
        if(list)\
        {\
            typename* ll_list = list;\
            while(ll_list && ll_list->next)\
            {\
                ll_list=ll_list->next;\
            }\
            ll_create(typename,ll_new,val);\
            ll_list->next = ll_new;\
            ll_new->prev = ll_list;\
        }\
        else\
        {\
            ll_create(typename,ll_list_helper,val);\
            list = ll_list_helper;\
        }\
    }while(0)

//Prints the elements of {list} Linked List, with the printf form {form}
#define ll_print(list,form)\
    do{\
        typeof(list) ll_list = list;\
        printf("[ ");\
        while(ll_list)\
        {\
            printf(form" ",ll_list->value);\
            ll_list = ll_list->next;\
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
                ll_create(typename,ll_new,val);\
                ll_new->next = list;\
                list->prev = ll_new;\
                list = ll_new;\
            }\
            else\
            {\
                typename* ll_list = list;\
                for(size_t i=0;i<where-1 && ll_list->next;i++)\
                {\
                    ll_list= ll_list->next;\
                }\
                if(!ll_list->next)\
                {\
                    ll_create(typename,ll_new,val);\
                    ll_list->next = ll_new;\
                    ll_new->prev = ll_list;\
                }\
                else\
                {\
                    ll_create(typename,ll_new,val);\
                    typename* ll_next = ll_list->next;\
                    ll_list->next = ll_new;\
                    ll_new->prev = ll_list;\
                    ll_new->next = ll_next;\
                    ll_next->prev = ll_new;\
                }\
            }\
        }\
        else ll_append(typename,list,val);\
    }while(0)

//Deletes {val} value from {list} Linked List, if the value is not found, the function doesn't do anything
#define ll_delete(list,val)\
    do{\
        typeof(list) ll_list = list;\
        while(ll_list && !(apulib_epsilon_close(ll_list-> value, val)))\
        {\
            ll_list = ll_list->next;\
        }\
        if(ll_list)\
        {\
            if(ll_list->prev && ll_list->next)\
            {\
                typeof(list) prev = ll_list->prev;\
                typeof(list) next = ll_list->next;\
                prev->next = next;\
                next->prev = prev;\
                free(ll_list);\
            }\
            else if(ll_list->prev)\
            {\
                typeof(list) prev = ll_list->prev;\
                prev->next = NULL;\
                free(ll_list);\
            }\
            else if(ll_list->next)\
            {\
                typeof(list) next = ll_list->next;\
                next->prev = NULL;\
                free(ll_list);\
                list = next;\
            }\
            else\
            {\
                free(ll_list);\
                list = NULL;\
            }\
        }\
    }while(0)

//Clears {list} Linked List, used as free as well
#define ll_clear(list)\
    do{\
        while(list)\
        {\
            typeof(list) curr = list;\
            list = list->next;\
            free(curr);\
        }\
    }while(0)

//Gets the value at {index} index inside {list} Linked List, and copies the value into {out} variable
//If the index is out of bounds the function doesn't do anything
#define ll_get(list,index,out)\
    do{\
        typeof(list) ll_list = list;\
        size_t i = 0;\
        while(ll_list && i != index)\
        {\
            ll_list = ll_list->next;\
            i++;\
        }\
        if(ll_list) out = ll_list->value;\
    }while(0)

//Copies {list} Linked List into {out} Linked List both of {typename} type
#define ll_copy(typename,list,out)\
    do{\
        ll_clear(out);\
        if(list)\
        {\
            ll_create(typename,ll_new_list,list->value);\
            typename* ll_list = list->next;\
            typename* ll_new_list_second = ll_new_list;\
            while(ll_list)\
            {\
                ll_create(typename,ll_new_element,ll_list->value);\
                ll_new_list_second-> next = ll_new_element;\
                ll_new_element->prev = ll_new_list_second;\
                ll_new_list_second = ll_new_element;\
                ll_list= ll_list->next;\
            }\
            out = ll_new_list;\
        }\
    }while(0)

//Reduces {list} Linked List into {accumulator} variable using {func} function, if {cond} condition is true, with {start} starting value.
//If the condition is never true, then the {out_bool} variable is 0, else it is 1
#define ll_reduce(list,func,cond,start,accumulator,out_bool)\
    do{\
        typeof(list) ll_list = list;\
        accumulator = start;\
        out_bool = 0;\
        while(ll_list)\
        {\
            if(cond(ll_list->value))\
            {\
                if(out_bool)\
                    accumulator = func(accumulator,ll_list->value);\
                else \
                    accumulator = ll_list->value;\
                out_bool = 1;\
            }\
            ll_list= ll_list->next;\
        }\
    }while(0)

//Adds the values of {list} Linked List into {out} variable
#define ll_sum(list,out) do{char b;ll_reduce(list,apulib_sum_helper,apulib_true_cond,0,out,b);}while(0)

//Finds the maximum value within {list} Linked List and outputs it into {out} variable
#define ll_max(list,out) do{char b;if(list) ll_reduce(list,apulib_max_helper,apulib_true_cond,list->value,out,b);}while(0)

//Finds the minimum value within {list} Linked List and outputs it into {out} variable
#define ll_min(list,out) do{char b;if(list) ll_reduce(list,apulib_min_helper,apulib_true_cond,list->value,out,b);}while(0)

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
        typeof(list) ll_list = list;\
        out = 0;\
        while(ll_list)\
        {\
            if(cond(ll_list->value)) out++;\
            ll_list = ll_list->next;\
        }\
    }while(0)

//Return true or false to {out_bool} if {list} Linked List contains {val} value
#define ll_contains(list,val,out_bool)\
    do{\
        typeof(list) ll_list = list;\
        out_bool = 0;\
        while(ll_list && !(apulib_epsilon_close(ll_list->value,val)))\
            ll_list = ll_list->next;\
        if(ll_list) out_bool = 1;\
    }while(0)

//Uses {func} function on all elements on a copy of {list} Linked List of {typename} type, and returns it to {out} Linked List
#define ll_map(typename,list,func,out)\
    do{\
        ll_clear(out);\
        if(list)\
        {\
            ll_create(typename,ll_new_list,func(list->value));\
            typename* ll_list = list->next;\
            typename* ll_new_list_second = ll_new_list;\
            while(ll_list)\
            {\
                ll_create(typename,ll_new_element,func(ll_list->value));\
                ll_new_list_second-> next = ll_new_element;\
                ll_new_element->prev = ll_new_list_second;\
                ll_new_list_second = ll_new_element;\
                ll_list= ll_list->next;\
            }\
            out = ll_new_list;\
        }\
    }while(0)

//Keeps the elements that return true with {cond} condition
//on all elements on a copy of {list} Linked List of {typename} type, and returns it to {out} Linked List
#define ll_filter(typename,list,cond,out)\
    do{\
        ll_clear(out);\
        if(list)\
        {\
            typename* ll_new_list = NULL;\
            typename* ll_list = list;\
            typename* ll_new_list_second = NULL;\
            while(ll_list)\
            {\
                if(cond(ll_list->value))\
                {\
                    if(!ll_new_list)\
                    {\
                        ll_create(typename,ll_list_helper,ll_list->value);\
                        ll_new_list = ll_list_helper;\
                        ll_new_list_second = ll_new_list;\
                    }\
                    else\
                    {\
                        ll_create(typename,ll_new_element,ll_list->value);\
                        ll_new_list_second->next = ll_new_element;\
                        ll_new_element->prev = ll_new_list_second;\
                        ll_new_list_second = ll_new_element;\
                    }\
                }\
                ll_list= ll_list->next;\
            }\
            out = ll_new_list;\
        }\
    }while(0)

//Uses {func} function on all elements on {list} Linked List of {typename} type
#define ll_map_self(typename,list,func)\
    do{\
        if(list)\
        {\
            typename* ll_list = list;\
            while(ll_list)\
            {\
                ll_list->value = func(ll_list->value);\
                ll_list=ll_list->next;\
            }\
        }\
    }while(0)

//Keeps the elements that return true with {cond} condition on all elements on {list} Linked List of {typename} type
#define ll_filter_self(typename, list, cond)\
    do{\
        typename* ll_list = list;\
        while(ll_list)\
        {\
            typename* next_node = ll_list->next;\
            if(!cond(ll_list->value))\
            {\
                if(ll_list->prev) ll_list->prev->next = ll_list->next;\
                else list = ll_list->next;\
                if(ll_list->next) ll_list->next->prev = ll_list->prev;\
                free(ll_list);\
            }\
            ll_list = next_node;\
        }\
    }while(0)

//Return true or false to {out_bool} if at least one element of {list} return true to {cond} condition 
#define ll_any(list,cond,out_bool)\
    do{\
        typeof(list) ll_list = list;\
        out_bool = 0;\
        while(ll_list && !(cond(ll_list->value)))\
            ll_list = ll_list->next;\
        if(ll_list) out_bool = 1;\
    }while(0)

//Return true or false to {out_bool} if all elements of {list} return true to {cond} condition
#define ll_all(list,cond,out_bool)\
    do{\
        typeof(list) ll_list = list;\
        out_bool = 1;\
        while(ll_list && cond(ll_list->value))\
            ll_list = ll_list->next;\
        if(ll_list) out_bool = 0;\
    }while(0)

//Uses Merge Sort on {list} Linked List of type {typename}
#define ll_sort(typename,list)\
    do{\
        if(list)\
        {\
            size_t c=1;\
            size_t size=0;\
            ll_size(list,size);\
            while(c < size)\
            {\
                size_t l = 0;\
                while(l < size - 1)\
                {\
                    size_t m = apulib_min_helper(l + c - 1,size - 1);\
                    size_t re = apulib_min_helper(l + 2 * c - 1, size - 1);\
                    typeof(list) left = list, right = list;\
                    typeof(list->value) value;\
                    size_t i = 0;\
                    while(i++ < l) left = left->next;\
                    i=0;\
                    while(i++ < m+1) right = right->next;\
                    typeof(list) list_helper = left;\
                    size_t j=l,k=m+1;\
                    if(left->value < right->value)\
                    {\
                        value = left->value;\
                        left=left->next;\
                        j++;\
                    }\
                    else\
                    {\
                        value = right->value;\
                        right = right->next;\
                        k++;\
                    }\
                    ll_create(typename,buffer,value);\
                    while(left && right && j<=m && k<=re)\
                    {\
                        if(left->value < right->value)\
                        {\
                            ll_append(typename,buffer,left->value);\
                            left = left->next;\
                            j++;\
                        }\
                        else\
                        {\
                            ll_append(typename,buffer,right->value);\
                            right = right->next;\
                            k++;\
                        }\
                    }\
                    while(left && list_helper && j<=m)\
                    {\
                        ll_append(typename,buffer,left->value);\
                        left = left->next;\
                        j++;\
                    }\
                    while(right && list_helper && k<=re)\
                    {\
                        ll_append(typename,buffer,right->value);\
                        right= right->next;\
                        k++;\
                    }\
                    typeof(buffer) buffer_start = buffer;\
                    while(buffer && list_helper)\
                    {\
                        list_helper->value = buffer->value;\
                        list_helper = list_helper->next;\
                        buffer = buffer->next;\
                    }\
                    ll_clear(buffer_start);\
                    l += 2 * c;\
                }\
                c *= 2;\
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
            size_t l=0,r=set.size;\
            while(l < r)\
            {\
                size_t m = l + (r - l) / 2;\
                if(set.data[m] < val) l = m+1;\
                else r=m;\
            }\
            if(set.size == l || !(apulib_epsilon_close(set.data[l], val)))\
            {\
                if(set.size != l)\
                    memmove(&set.data[l+1],&set.data[l],sizeof(set.data[0]) * (set.size-l));\
                set.data[l] = val;\
                set.size++;\
            }\
        }\
    }while(0)

//Prints out {set} set with the form character {form}
#define set_print(set,form)\
    do{\
        printf("{");\
        for(size_t i=0;i<set.size;i++) {\
            printf(form,set.data[i]);\
            if(i!=set.size-1)printf(", ");\
        }\
        printf("}\n");\
    }while(0)

//Deletes {val} value from {set} set if present
#define set_delete(set,val)\
    do{\
        if(set.data!=NULL)\
        {\
            size_t l=0,r=set.size;\
            while(l < r)\
            {\
                size_t m = l + (r - l) / 2;\
                if(set.data[m] < val) l = m+1;\
                else r=m;\
            }\
            if(l < set.size && apulib_epsilon_close(set.data[l],val))\
            {\
                if(set.size != l)\
                {\
                    memmove(&set.data[l],&set.data[l+1],sizeof(set.data[0]) * (set.size-l-1));\
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
            size_t l=0,r=set.size;\
            while(l < r)\
            {\
                size_t m = l + (r - l) / 2;\
                if(set.data[m] < val) l = m+1;\
                else r=m;\
            }\
            if(l < set.size && apulib_epsilon_close(set.data[l],val))\
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
            for(size_t i=0;i<set.size;i++) out.data[i] = set.data[i];\
        }\
    }while(0)

//Reduces the values of {set} set into {accumulator} variable, if {out_bool} is true
//The reduction is done with {func} function, if {cond} condition is true, with the start value of {start}
#define set_reduce(set,func,cond,start,accumulator,out_bool)\
    do{\
        accumulator = start;\
        out_bool = 0;\
        for(size_t i=0;i<set.size;i++)\
            if(cond(set.data[i]))\
            {\
                if(out_bool)\
                    accumulator = func(accumulator,set.data[i]);\
                else \
                    accumulator = set.data[i];\
                out_bool = 1;\
            }\
    }while(0)

//Adds the values of {set} set into {out} variable
#define set_sum(set,out) do{char b;set_reduce(set,apulib_sum_helper,apulib_true_cond,0,out,b);}while(0)

//Finds the maximum value within {set} set and outputs it into {out} variable
#define set_max(set,out) do{char b;if(set.data!= NULL) set_reduce(set,apulib_max_helper,apulib_true_cond,set.data[0],out,b);}while(0)

//Finds the minimum value within {set} set and outputs it into {out} variable
#define set_min(set,out) do{char b;if(set.data!= NULL) set_reduce(set,apulib_min_helper,apulib_true_cond,set.data[0],out,b);}while(0)

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
        for(size_t i=0;i<set.size;i++) if(cond(set.data[i])) out++;\
    }while(0)

//{out} is a set of the values of {set} each mutated by the {func} function
#define set_map(set,func,out)\
    do{\
        set_clear(out);\
        for(size_t i=0;i<set.size;i++) set_insert(out,func(set.data[i]));\
    }while(0)

//{out} is a set of the values of {set}, if {cond} condition is true for them
#define set_filter(set,cond,out)\
    do{\
        set_clear(out);\
        for(size_t i=0;i<set.size;i++) if(cond(set.data[i])) set_insert(out,set.data[i]);\
    }while(0)

//Mutates the values of {set} set, each mutated by the {func} function
//WARNING: THE {func} FUNCTION CAN RUIN THE SORTED NATURE OF THE SET.
//IF YOU USE A FUNCTION THAT ISN'T MONOTONIC, THEN PLEASE SORT THE SET WITH DA_STABLE_SORT
#define set_map_self(set,func)\
    do{\
        for(size_t i=0;i<set.size;i++) set.data[i] = func(set.data[i]);\
    }while(0)

//Only keeps the values of {set} set, if {cond} condition is true for them
#define set_filter_self(set, cond)\
    do{\
        if(set.data)\
        {\
            size_t ind = 0;\
            for(size_t i = 0; i < set.size; i++)\
            {\
                if(cond(set.data[i]))\
                {\
                    set.data[ind++] = set.data[i];\
                }\
            }\
            set.size = ind;\
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
        for(size_t i=0;i<set.size;i++) if(cond(set.data[i]))\
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
        for(size_t i=0;i<set.size;i++) if(!(cond(set.data[i])))\
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
            size_t l=0,r=map.size;\
            while(l < r)\
            {\
                size_t m = l + (r - l) / 2;\
                if(map.keys[m] < key) l = m+1;\
                else r=m;\
            }\
            if(map.size == l || !(apulib_epsilon_close(map.keys[l], key)))\
            {\
                if(map.size != l)\
                {\
                    memmove(&map.keys[l+1],&map.keys[l],sizeof(map.keys[0]) * (map.size-l));\
                    memmove(&map.data[l+1],&map.data[l],sizeof(map.data[0]) * (map.size-l));\
                }\
                map.keys[l] = key;\
                map.data[l] = val;\
                map.size++;\
            }\
        }\
    }while(0)

//Prints out {map} map, with the form characters: {keyform},{valform}
#define map_print(map,keyform,valform)\
    do{\
        printf("{");\
        for(size_t i=0;i<map.size;i++) {\
            printf(keyform": "valform,map.keys[i],map.data[i]);\
            if(i!=map.size-1)printf(", ");\
        }\
        printf("}\n");\
    }while(0)

//Deletes a {key},value pair from {map} map
#define map_delete(map,key)\
    do{\
        if(map.keys != NULL)\
        {\
            size_t l=0,r=map.size;\
            while(l < r)\
            {\
                size_t m = l + (r - l) / 2;\
                if(map.keys[m] < key) l = m+1;\
                else r=m;\
            }\
            if(l < map.size && apulib_epsilon_close(map.keys[l], key))\
            {\
                memmove(&map.keys[l],&map.keys[l+1],sizeof(map.keys[0]) * (map.size-l-1));\
                memmove(&map.data[l],&map.data[l+1],sizeof(map.data[0]) * (map.size-l-1));\
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
            size_t l=0,r=map.size;\
            while(l < r)\
            {\
                size_t m = l + (r - l) / 2;\
                if(map.keys[m] < key) l = m+1;\
                else r=m;\
            }\
            if(l < map.size && apulib_epsilon_close(map.keys[l], key)) out = map.data[l];\
        }\
    }while(0)

//Checks if {map} map contains {key} key, and outputs the result into {out} variable
#define map_contains_key(map,key,out)\
    do{\
        out = 0;\
        if(map.data)\
        {\
            size_t l=0,r=map.size;\
            while(l < r)\
            {\
                size_t m = l + (r - l) / 2;\
                if(map.keys[m] < key) l = m+1;\
                else r=m;\
            }\
            if(l < map.size && apulib_epsilon_close(map.keys[l], key))\
            {\
                out = 1;\
            }\
        }\
    }while(0)

//Checks if {map} map contains {val} value, and outputs the result into {out} variable
#define map_contains_val(map,val,out)\
    do{\
        out = 0;\
        for(size_t i=0;i<map.size;i++) if(apulib_epsilon_close(map.data[i],val))\
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
            for(size_t i=0;i<map.size;i++)\
            {\
                out.data[i] = map.data[i];\
                out.keys[i] = map.keys[i];\
            }\
        }\
    }while(0)

//Reduces the values of {map} map into {accumulator} variable, if {out_bool} is true
//The reduction is done with {func} function, if {cond} condition is true, with the start value of {start}
#define map_reduce(map,func,cond,start,accumulator,out_bool)\
    do{\
        accumulator = start;\
        out_bool = 0;\
        for(size_t i=0;i<map.size;i++)\
            if(cond(map.data[i]))\
            {\
                if(out_bool)\
                    accumulator = func(accumulator,map.data[i]);\
                else \
                    accumulator = map.data[i];\
                out_bool = 1;\
            }\
    }while(0)

//Adds the values of {map} map into {out} variable
#define map_sum(map,out) do{char b;map_reduce(map,apulib_sum_helper,apulib_true_cond,0,out,b);}while(0)

//Finds the maximum value within {map} map and outputs it into {out} variable
#define map_max(map,out) do{char b;if(map.data!= NULL) map_reduce(map,apulib_max_helper,apulib_true_cond,map.data[0],out,b);}while(0)

//Finds the minimum value within {map} map and outputs it into {out} variable
#define map_min(map,out) do{char b;if(map.data!= NULL) map_reduce(map,apulib_min_helper,apulib_true_cond,map.data[0],out,b);}while(0)

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
        for(size_t i=0;i<map.size;i++) if(cond(map.data[i])) out++;\
    }while(0)

//{out} is a map of the values of {map}, if {cond} condition is true for them
#define map_filter(map,cond,out)\
    do{\
        map_clear(out);\
        for(size_t i=0;i<map.size;i++) if(cond(map.data[i])) map_insert(out,map.keys[i],map.data[i]);\
    }while(0)

//Only keeps the values of {map} map, if {cond} condition is true for them
#define map_filter_self(map, cond)\
    do{\
        if(map.data)\
        {\
            size_t ind = 0;\
            for(size_t i = 0; i < map.size; i++)\
            {\
                if(cond(map.data[i]))\
                {\
                    map.data[ind] = map.data[i];\
                    map.keys[ind++] = map.keys[i];\
                }\
            }\
            map.size = ind;\
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
        for(size_t i=0;i<map.size;i++) if(cond(map.data[i]))\
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
        for(size_t i=0;i<map.size;i++) if(!(cond(map.data[i])))\
        {\
            out = 0;\
            break;\
        }\
    }while(0)

#endif

#endif
