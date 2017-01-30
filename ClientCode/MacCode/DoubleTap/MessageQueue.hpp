//
//  MessageQueue.hpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 10/01/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//  Original design: https://vichargrave.github.io/articles/2013-01/multithreaded-work-queue-in-cpp

#ifndef MessageQueue_hpp
#define MessageQueue_hpp

//#include <stdio.h>
#include <iostream>
#include <thread>
#include <list>

// TODO- rewrite as on blackboard
template <typename T> class MessageQueue
{
public:
    MessageQueue(){}
//    MessageQueue(const MessageQueue& cpy)
//    {
//        //std::unique_lock<std::mutex> l_lock(m_mutex, std::defer_lock);
//        //std::unique_lock<std::mutex> l_cpyLock(cpy.m_mutex, std::defer_lock);
//        //std::lock(l_lock, l_cpyLock);
//        m_queue = cpy.m_queue;
//        m_MaxMessages = cpy.m_MaxMessages;
//        m_Count = cpy.m_Count;
//
//        //m_NotFull = cpy.m_NotFull;
//        //m_NotEmpty = cpy.m_NotEmpty;
//        
//    }
    ~MessageQueue(){}
    void SetLimit(int p_capacity)
    {
        //std::cout<<&"MaxMessages was: " [ m_MaxMessages]<<std::endl;
        m_MaxMessages = p_capacity;
        //std::cout<<&"MaxMessage set to:" [ m_MaxMessages]<<std::endl;
    }
    void Add(T item)
    {
        std::unique_lock<std::mutex> l_lock(m_mutex);
        
        m_NotFull.wait(l_lock, [&](){return m_queue.size() != m_MaxMessages;});

        m_queue.push_back(item);
        m_Count++;

        m_NotEmpty.notify_one();
        
        std::cout<<item<<std::endl;

    }
    T Retrieve()
    {
        // If nothing in queue return immediatly without waiting
        // Check if main thread, if so dont block
        
        std::unique_lock<std::mutex> l_lock(m_mutex);
        
        if(0 == m_Count)
        {
            T l_tempT;
            return l_tempT;
        }
        
        m_NotEmpty.wait(l_lock, [&](){return m_Count != 0;});
        
        T l_item = m_queue.front();
        m_queue.pop_front();
        m_Count--;
        std::cout<<m_Count<<std::endl;
        
        m_NotFull.notify_one();
        return l_item;
    }
    
//    int Size()
//    {
//        std::unique_lock<std::mutex> l_lock(m_mutex);
//
//        int l_size = m_queue.size();
//
//        return l_size;
//    }
    
private:
    std::list<T> m_queue;
    int m_MaxMessages =69;
    int m_Count = 0;
    std::mutex m_mutex;
    std::condition_variable m_NotFull;
    std::condition_variable m_NotEmpty;
};


#endif /* MessageQueue_hpp */
