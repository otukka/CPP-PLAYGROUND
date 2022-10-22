/*
Derivative from https://stackoverflow.com/a/16075550
*/


#ifndef _MESSAGE_QUEUE_HPP_
#define _MESSAGE_QUEUE_HPP_

#include <queue>
#include <mutex>
#include <condition_variable>


template <class T>
class MessageQueue
{
public:
  MessageQueue(void) : queue(), mutex(), condition_variable(){}

  ~MessageQueue(void){}

  void push_back(T t)
  {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(t);
    condition_variable.notify_one();
  }


  T pop_front(void)
  {
    std::unique_lock<std::mutex> lock(mutex);
    while(queue.empty())
    {

      condition_variable.wait(lock);
    }
    T val = queue.front();
    queue.pop();
    return val;
  }

  size_t size(void)
  {
    return queue.size(); 
  }

private:
  std::queue<T> queue;
  mutable std::mutex mutex;
  std::condition_variable condition_variable;
};

#endif // _MESSAGE_QUEUE_HPP_