/**
 * \addtogroup osal_wrapper
 * @{
 */

#ifndef MESSAGE_QUEUE_HPP
#define MESSAGE_QUEUE_HPP

//system includes
#include <condition_variable>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

template<typename T>   //pointers are not allowed! (no free mechanism and not supported by destructor)
class message_queue {
    static_assert(std::is_pointer<T>::value == false, "pointers are not allowed! (no free mechanism and not supported by destructor)");
private:
    std::queue<T>               m_queue;
    const std::size_t           msg_sz;
    const std::size_t           msg_max;
    const std::size_t           q_sz;
    std::recursive_timed_mutex  mutex;
    std::condition_variable_any cond;
    
    bool                send_wait;          /*!< Send wait flag */
    bool                receive_wait;       /*!< Receive wait flag */

public:
    message_queue(std::size_t sz, std::size_t max)
        : msg_sz(sz)
        , msg_max(max)
        , q_sz(msg_max * msg_sz)
    {
        std::unique_lock<std::recursive_timed_mutex> lck(mutex);   //unlocks when destructed
        send_wait = false;
        receive_wait = false;
    }
    ~message_queue() 
    {
        std::unique_lock<std::recursive_timed_mutex> lck(mutex);   //unlocks when destructed
        send_wait = false;
        receive_wait = false;
    }

    bool full(void)
    {
        std::unique_lock<std::recursive_timed_mutex> lck(mutex);   //unlocks when destructed
        return m_queue.size() >= msg_max;                          //size returns the number of elements and not the actual memory size
    }
    bool empty(void)
    {
        std::unique_lock<std::recursive_timed_mutex> lck(mutex);   //unlocks when destructed
        return m_queue.empty();
    }
    void reset()
    {
        std::unique_lock<std::recursive_timed_mutex> lck(mutex);   //unlocks when destructed
        while (!empty())
        {
            m_queue.pop();   //also calls object destructor
        }
        cond.notify_all();
    }

    bool try_send(T message)
    {
        std::unique_lock<std::recursive_timed_mutex> lck(mutex);
        if (!full())
        {
            m_queue.push(message);
            cond.notify_all();
            return true;
        }
        return false;
    }

    template<class Clock = std::chrono::high_resolution_clock, class Duration = typename Clock::duration>
    bool try_send_until(T message, std::chrono::time_point<Clock, Duration> timepoint)
    {
        std::unique_lock<std::recursive_timed_mutex> lck(mutex);   //unlocks when destructed
        if (!try_send(message))
        {
            send_wait = true;
            while (std::cv_status::no_timeout == cond.wait_until(lck, timepoint))
            {
                if (try_send(message))
                {
                    send_wait = false;
                    return true;
                }
            }
            send_wait = false;
            return false;
        }
        return true;

        //while (timepoint < Clock::now())
        //{
        //    if (try_send(message))
        //    {
        //        return true;
        //    }
        //    std::this_thread::yield();
        //}
        //return false;
    }

    template<class Clock = std::chrono::high_resolution_clock, class Duration = typename Clock::duration>
    bool try_send_for(T message, Duration duration)
    {
        return try_send_until(message, Clock::now() + duration);
    }

    bool try_receive(T& buffer)
    {
        std::unique_lock<std::recursive_timed_mutex> lck(mutex);
        if (!empty())
        {
            buffer = m_queue.front();
            m_queue.pop();
            cond.notify_all();
            return true;
        }
        return false;
    }

    template<class Clock = std::chrono::high_resolution_clock, class Duration = typename Clock::duration>
    bool try_receive_until(T& buffer, std::chrono::time_point<Clock, Duration> timepoint)
    {
        std::unique_lock<std::recursive_timed_mutex> lck(mutex);   //unlocks when destructed
        if (!try_receive(buffer))
        {
            receive_wait = true;
            while (std::cv_status::no_timeout == cond.wait_until(lck, timepoint))
            {
                if (try_receive(buffer))
                {
                    receive_wait = false;
                    return true;
                }
            }
            receive_wait = false;
            return false;
        }
        return true;
    }

    template<class Clock = std::chrono::high_resolution_clock, class Duration = typename Clock::duration>
    bool try_receive_for(T& buffer, Duration duration)
    {
        return try_receive_until(buffer, Clock::now() + duration);
    }

    void get_conf(std::size_t& sz, int64_t& max)
    {
        sz  = msg_sz;
        max = msg_max;
    }

    bool is_busy(void)
    {
        std::unique_lock<std::recursive_timed_mutex> lck(mutex);
        if ((false != send_wait) || (false != receive_wait))
        {
            return true;
        }
        if (!empty())
        {
            return true;
        }
        
        return false;
    }
};

class osal_message_t {
private:
    std::shared_ptr<void> m_data;
    std::size_t           m_sz;

public:
    osal_message_t(std::size_t size = 0)
        : m_sz(size)
    {
        m_data = std::shared_ptr<void>(std::malloc(size), std::free);
    }

    bool set(const void* const source, std::size_t size)
    {
        if (!m_data || nullptr == source || size != m_sz)
        {
            return false;
        }
        std::memcpy(m_data.get(), source, m_sz);

        return true;
    }

    bool get(void* const buffer, std::size_t size)
    {
        if (!m_data || nullptr == buffer || size != m_sz)
        {
            return false;
        }
        std::memcpy(buffer, m_data.get(), m_sz);

        return true;
    }
};

#endif   //MESSAGE_QUEUE_HPP

/**
 * @}
 */