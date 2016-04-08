
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <cstdlib>

typedef void (*Task)(void*);

void test(void* abc){
  std::cout << "hello:" << std::endl;
}

struct job {
  job(Task task, void* arg){
      _task = task;
      _arg = arg;
  }
  Task _task;
  void* _arg;
};

class thread_pool{
public:
  thread_pool(int thread_num);
  ~thread_pool();
  void post(Task task, void* taskData);

private:
  int _thread_num;
  std::vector<job> _tasks;
  std::vector<std::thread> slaves;
  std::mutex tasksLock;
  std::condition_variable cv;
  static void _slave(std::mutex& mutex, std::condition_variable& cond, std::vector<job>& v);
};

thread_pool::thread_pool(int thread_num) :
  _thread_num(thread_num)
{
  if(thread_num > 0)
    for(int i = 0; i < thread_num; i++)
      slaves.push_back(std::thread(_slave, std::ref(tasksLock), std::ref(cv), std::ref(_tasks)));
}

thread_pool::~thread_pool(){
}

void thread_pool::post(Task task, void* taskData){
  tasksLock.lock();
  _tasks.push_back(job(task, taskData));
  tasksLock.unlock();
  cv.notify_one();
}

void thread_pool::_slave(std::mutex& mutex, std::condition_variable& cond, std::vector<job>& v){
  Task task;
  void* arg;
  mutex.lock();
  std::cout << "thread " << std::this_thread::get_id() << " created" << std::endl;
  mutex.unlock();

  std::unique_lock<std::mutex> locker(mutex);
  while(true){
     cond.wait(locker, [&](){
       return !v.empty();
     });
    task = v.back()._task;
    arg = v.back()._arg;
    v.pop_back();
    mutex.unlock();
    task(arg);
  }
}

int main(){
  int a = 0;
  thread_pool pool(5);
  pool.post(test, NULL);
  pool.post(test, NULL);
  std::cin >> a;
  pool.post(test, NULL);
  pool.post(test, NULL);
  pool.post(test, NULL);
  while(true);
  return 0;
}
