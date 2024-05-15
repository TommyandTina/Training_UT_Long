#ifndef osal_test_framework_helper
#define osal_test_framework_helper

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "rcar-xos/osal/r_osal.h"

#define LOG_INFO(...) {printf(__VA_ARGS__);}
#define LOG_ERROR(...) {printf("Error:"); printf(__VA_ARGS__);}
#define STRING_MAX_SIZE 1000

namespace OsalFWHelper
{
    // TODO: this map only support the key as C string type. Need to support other types in future
    template<class T1, class T2> class multimap
    {
    public:
        multimap()
        {
            // reserve an element for end() 
            p_map = (map_info*)malloc(sizeof(map_info));
            if(NULL == p_map)
            {
                LOG_ERROR("Map allocation failed\n");
            }
            else
            {
                map_size++;
            }
        }
        ~multimap()
        {
            if(NULL != p_map)
            {
                free(p_map);
                p_map = NULL;
            }
        }

        struct map_data
        {
            T1 key;
            T2 value;
        };

        struct map_info
        {
            int index;
            T1 first;
            T2 second;
        };

        using iterator = map_info*;

        map_info* p_map = NULL;
        size_t map_size = 0;
        size_t elements_count = 0;

        multimap& operator=(const multimap<T1, T2>& sp)
        {
            map_info* tmp_map = NULL;
            tmp_map = (map_info*)realloc((void*)p_map, sp.map_size * sizeof(map_info));
            if(NULL == tmp_map)
            {
                LOG_ERROR("Assignment operator failed\n");
            }
            else
            {
                p_map = tmp_map;
                memcpy(p_map, sp.p_map, sizeof(map_info) * sp.map_size);
                map_size = sp.map_size;
                elements_count = sp.elements_count;
            }
            return *this;
        }

        multimap(const multimap<T1, T2>& sp)
        {
            map_info* tmp_map = NULL;
            tmp_map = (map_info*)realloc((void*)p_map, sp.map_size * sizeof(map_info));
            if(NULL == tmp_map)
            {
                LOG_ERROR("Copy constructor failed\n");
            }
            else
            {
                p_map = tmp_map;
                memcpy(p_map, sp.p_map, sizeof(map_info) * sp.map_size);
                map_size = sp.map_size;
                elements_count = sp.elements_count;
            }
        }

        map_info* begin()
        {
            return &p_map[0];
        }

        map_info* end()
        {
            return &p_map[elements_count];
        }

        static int map_sort_compare(const void * a, const void * b)
        {
            map_info *ptr_a = (map_info *)a;
            map_info *ptr_b = (map_info *)b;
            int ret = strcmp(ptr_a->first, ptr_b->first);
            if(0 == ret)
            {
                if (ptr_a->index < ptr_b->index)
                {
                    ret = -1;
                }
                else if (ptr_a->index > ptr_b->index)
                {
                    ret = 1;
                }
                else
                {
                    ret = 0;
                }
            }
            return ret;
        }

        void insert(map_data new_element)
        {
            map_info* tmp_map = NULL;
            tmp_map = (map_info*)realloc((void*)p_map, (map_size + 1) * sizeof(map_info));
            if(NULL == tmp_map)
            {
                LOG_ERROR("Adding new element failed\n");
            }
            else
            {
                p_map = tmp_map;
                p_map[elements_count].first = new_element.key;
                p_map[elements_count].second = new_element.value;
                p_map[elements_count].index = elements_count;
                map_size++;
                elements_count++;
                if(elements_count > 1)
                {
                    qsort(p_map, elements_count, sizeof(map_info), map_sort_compare);
                }
            }
        }

        int first_element(map_info* arr, T1 key, int num)
        {
            int low = 0, high = num - 1, res = -1;
            while (low <= high)
            {
                int mid = (low + high) / 2;
                if (strcmp(arr[mid].first, key) > 0)
                {
                    high = mid - 1;
                }
                else if (strcmp(arr[mid].first, key) < 0)
                {
                    low = mid + 1;
                } 
                else
                {
                    res = mid;
                    high = mid - 1;
                }
            }
            return res;
        }

        int last_element(map_info* arr, T1 key, int num)
        {
            int low = 0, high = num - 1, res = -1;
            while (low <= high)
            {
                int mid = (low + high) / 2;
                if (strcmp(arr[mid].first, key) > 0)
                {
                    high = mid - 1;
                }
                else if (strcmp(arr[mid].first, key) < 0)
                {
                    low = mid + 1;
                } 
                else
                {
                    res = mid;
                    low = mid + 1;
                }
            }
            return res;
        }

        map_info* lower_bound(T1 key)
        {
            map_info* tmp = NULL;
            int index = first_element(p_map, key, elements_count);
            if(index == -1)
            {
                LOG_ERROR("Key was not found\n");
            }
            else
            {
                tmp = &p_map[index];
            }
            return tmp;
        }

        map_info* upper_bound(T1 key)
        {
            map_info* tmp = NULL;
            int index = last_element(p_map, key, elements_count);
            if(index == -1)
            {
                LOG_ERROR("Key was not found\n");
            }
            else
            {
                tmp = &p_map[index+1];
            }
            return tmp;
        }

        size_t size()
        {
            return elements_count;
        }

        void clear()
        {
            map_info* tmp_map = NULL;

            //clear all elements in map except the reserved element for end(), will be freed in destructor.
            tmp_map = (map_info*)realloc((void*)p_map, sizeof(map_info));
            if(NULL == tmp_map)
            {
                LOG_ERROR("Cleaning map failed\n");
            }
            else
            {
                p_map = tmp_map;
                elements_count = 0;
                map_size = 1;
            }
        }
    };

    template<class T> T* next(T* ptr, size_t n)
    {
        return (ptr+n);
    }

    template<class T1, class T2> struct pair
    {
        T1 first;
        T2 second;
    };

    template<class T> class vector
    {
    public:
        vector()
        {
            // reserve for the element which follow the last element
            p_data = (T*)malloc(sizeof(T));
            if(NULL == p_data)
            {
                LOG_ERROR("Vector allocation failed\n");
            }
            else
            {
                vector_size++;
            }
        }
        ~vector()
        {
            if(NULL != p_data)
            {
                free(p_data);
                p_data = NULL;
            }
        }

        T* p_data = NULL;
        size_t vector_size = 0;
        size_t elements_count = 0;

        void push_back(const T& value)
        {
            T* tmp = NULL;
            if((elements_count + 1) >= vector_size)
            {
                tmp = (T*)realloc((void*)p_data, (vector_size + 1) * sizeof(T));
                if(NULL == tmp)
                {
                    LOG_ERROR("Vector allocation failed\n");
                }
                else
                {
                    p_data = tmp;
                    vector_size++;
                }
            }
            p_data[elements_count] = value;
            elements_count++;
        }

        T* begin()
        {
            return &p_data[0];
        }

        T* begin() const
        {
            return &p_data[0];
        }

        T* end()
        {
            return &p_data[elements_count];
        }

        T* end() const
        {
            return &p_data[elements_count];
        }

        size_t size()
        {
            return elements_count;
        }

        void clear()
        {
            memset(p_data, 0, vector_size * sizeof(T));
            elements_count = 0;
        }

        bool empty()
        {
            return (elements_count == 0) ? true : false;
        }

        vector& operator=(const vector<T>& sp)
        {
            T* tmp_vector = NULL;
            tmp_vector = (T*)realloc((void*)p_data, sizeof(T) * sp.vector_size);
            if(NULL == tmp_vector)
            {
                LOG_ERROR("Assignment operator failed\n");
            }
            else
            {
                p_data = tmp_vector;
                memcpy(p_data, sp.p_data, sizeof(T) * sp.vector_size);
                vector_size = sp.vector_size;
                elements_count = sp.elements_count;
            }
            return *this;
        }

        vector(const vector<T>& sp)
        {
            T* tmp_vector = NULL;
            tmp_vector = (T*)realloc((void*)p_data, sizeof(T) * sp.vector_size);
            if(NULL == tmp_vector)
            {
                LOG_ERROR("Copy constructor failed\n");
            }
            else
            {
                p_data = tmp_vector;
                memcpy(p_data, sp.p_data, sizeof(T) * sp.vector_size);
                vector_size = sp.vector_size;
                elements_count = sp.elements_count;
            }
        }

        void resize(size_t new_size, T value)
        {
            T* tmp_vector = NULL;

            if(elements_count < new_size)
            {
                tmp_vector = (T*)realloc((void*)p_data, sizeof(T) * (new_size + 1)); // plus 1 is reserve for the element which follow the last element
                if(NULL == tmp_vector)
                {
                    LOG_ERROR("resize vector failed\n");
                }
                else
                {
                    p_data = tmp_vector;
                    for(size_t i = elements_count; i < new_size; i++)
                    {
                        p_data[i] = value;
                    }
                    vector_size = new_size + 1;
                }
            }
            elements_count = new_size;
        }

        void resize(size_t new_size)
        {
            this->resize(new_size, 0);
        }

        void shrink_to_fit()
        {
            T* tmp_vector = NULL;

            if(elements_count != (vector_size - 1))
            {
                tmp_vector = (T*)realloc((void*)p_data, sizeof(T) * (elements_count + 1)); // plus 1 is reserve for the element which follow the last element
                if(NULL == tmp_vector)
                {
                    LOG_ERROR("Cleaning vector failed\n");
                }
                else
                {
                    p_data = tmp_vector;
                    vector_size = elements_count + 1;
                }
            }
        }

        size_t capacity()
        {
            return (vector_size - 1);
        }

        T& at(size_t index)
        {
            return p_data[index];
        }

        void erase(T* pos)
        {
            T* tmp_vector = NULL;

            memmove(pos, pos + 1, sizeof(T) * (&p_data[elements_count] - pos));
            tmp_vector = (T*)realloc((void*)p_data, sizeof(T) * (vector_size - 1));
            if(NULL == tmp_vector)
            {
                LOG_ERROR("erase vector failed\n");
            }
            else
            {
                p_data = tmp_vector;
                vector_size--;
                elements_count--;
            }
        }

        T& operator[](size_t index)
        {
            return p_data[index];
        }
    };

    class Counter
    {
    public:
        // Constructor
        Counter() : m_counter(0){};
    
        Counter(const Counter&) = delete;
        Counter& operator=(const Counter&) = delete;
    
        // Destructor
        ~Counter() {}
    
        void reset()
        {
            m_counter = 0;
        }
    
        unsigned int get()
        {
            return m_counter;
        }
    
        // Overload post/pre increment
        void operator++()
        {
            m_counter++;
        }
    
        void operator++(int)
        {
            m_counter++;
        }
    
        // Overload post/pre decrement
        void operator--()
        {
            m_counter--;
        }
        void operator--(int)
        {
            m_counter--;
        }
    private:
        unsigned int m_counter{};
    };

    template<class T> class shared_ptr
    {
    public:
        // constuctor
        explicit shared_ptr(T* new_ptr = NULL)
        {
            ptr = new_ptr;
            p_counter = new Counter();
            (*p_counter)++;
        }

        // copy constructor
        shared_ptr(const shared_ptr<T>& sp)
        {
            ptr = sp.ptr;
            p_counter = sp.p_counter;
            (*p_counter)++;
        }

        shared_ptr& operator=(const shared_ptr<T>& sp)
        {
            ptr = sp.ptr;
            p_counter = sp.p_counter;
            (*p_counter)++;
            return *this;
        }

        unsigned int use_count()
        {
            return p_counter->get();
        }
    
        T* get()
        {
            return ptr;
        }

        T& operator*()
        {
            return *ptr;
        }

        T* operator->()
        {
            return ptr;
        }

        ~shared_ptr()
        {
            (*p_counter)--;
            if (p_counter->get() == 0)
            {
                if(p_counter != NULL)
                {
                    delete p_counter;
                }
                    
                if(ptr != NULL)
                {
                    delete ptr;
                }
            }
        }
    private:
        T* ptr = NULL;
        Counter* p_counter = NULL;
    };

    template<class T1, class T2> bool equal(T1* first1, T1* last1, T2* first2)
    {
        size_t range = last1 - first1;
        for(auto i = 0; i < range; i++)
        {
            if(*(first1 + i) != *(first2 + i))
            {
                return false;
            }
        }

        return true;
    }

    template<class T> T* begin(T& container)
    {
        return container.begin();
    }

    template<class T, size_t N> T* begin(T(&arr)[N])
    {
        return &arr[0];
    }

    template<class T> T* end(T& container)
    {
        return container.end();
    }

    template<class T, size_t N> T* end(T(&arr)[N])
    {
        return arr + N;
    }

    template<class T> T* find(T* first, T* last, const T& val)
    {
        int start = -1;
        size_t range = last - first;

        if(0 == range)
        {
            return last;
        }

        // Traverse from beginning to find
        // first occurrence
        for (int i = 0; i < range; i++)
        {
            if (*(first+i) == val)
            {
                start = i;
                break;
            }
        }

        if (start == -1)
        {
            // Val not present in array
            return last;
        }

        return (first + start);
    }

    template<class T> void iota(T* first, T* last, T value)
    {
        size_t range = last - first;
        for(auto i = 0; i < range; i++)
        {
            *(first + i) = value;
            value++;
        }
    }

    template<class T> size_t count(T* first, T* last, T value)
    {
        size_t counter = 0;
        size_t range = last - first;
        for(auto i = 0; i < range; i++)
        {
            if(*(first + i) == value)
            {
                counter++;
            }
        }

        return counter;
    }

    class string
    {
    public:
        string(const char* str)
        {
            strcpy(data, str);
        }

        string(){};

        string& operator=(const string& sp)
        {
            strcpy(data, sp.data);
            return *this;
        }

        string(const string& sp)
        {
            strcpy(data, sp.data);
        }

        const char* c_str()
        {
            return data;
        }
        char data[STRING_MAX_SIZE];
    };

    constexpr unsigned long long int operator"" _KB(unsigned long long int val)
    {
        return val * 1024U;
    }

    constexpr unsigned long long int operator"" _MB(unsigned long long int val)
    {
        return val * 1024U * 1024U;
    }

    constexpr unsigned long long int operator"" _GB(unsigned long long int val)
    {
        return val * 1024U * 1024U * 1024U;
    }

    template<class T1, class T2>
    inline T1 saturate(T1 val, T2 lower_limit, T2 upper_limit)
    {
        T1 ret;
        if (lower_limit > upper_limit)
        {
            LOG_ERROR("lower_limit > upper_limit\n");
        }
        else if (upper_limit < lower_limit)
        {
            LOG_ERROR("upper_limit < lower_limit\n");
        }
        if (val >= SIZE_MAX) // use the fixed maximum value of size_t because osal functional test only use size_t
        {
            val = SIZE_MAX;
        }
        ret = val < lower_limit ? lower_limit : val;
        ret = ret > upper_limit ? upper_limit : ret;
        return ret;
    }

    namespace utils
    {
        template<class T>
        struct cb_wrapper;

        template<class T, class Ret, class... Args>
        struct cb_wrapper<Ret (T::*)(Args...)>
        {
            typedef Ret (*cb_t)(void* user_arg, Args... args);
            typedef Ret (*cbe_t)(Args... args, void* user_arg);
            template<Ret (T::*func)(Args...)>
            constexpr static cb_t make_cb(void)
            {
                return [](void* user_arg, Args... args) -> Ret {
                    T& handlerClass = *static_cast<T*>(user_arg);
                    return (handlerClass.*func)(args...);
                };
            }
        };

        template<class T, class Ret, class... Args>
        struct cb_wrapper<Ret (T::*)(Args...) const>
        {
            typedef Ret (*cb_t)(const void* user_arg, Args... args);
            typedef Ret (*cbe_t)(Args... args, const void* user_arg);
            template<Ret (T::*func)(Args...) const>
            constexpr static cb_t make_cb()
            {
                return [](const void* user_arg, Args... args) -> Ret {
                    const T& handlerClass = *static_cast<const T*>(user_arg);
                    return (handlerClass.*func)(args...);
                };
            }
        };

    } // namespace utils

    static constexpr st_osal_mmngr_config_t MMNGR_CONFIG_INIT_VALUE()
    {
        return {OSAL_MMNGR_ALLOC_MODE_INVALID, 0, 0, 0};
    }
}

#endif
