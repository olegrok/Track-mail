
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <cstdlib>
#include <atomic>
#include <system_error>
#include <string>

enum class DataBaseErr : int {
  ok = 7000,
  can_not_create = 7001,
  can_not_open = 7002,
  wrong_request = 7003,
  can_not_insert = 7004,
  can_not_delete = 7005
};

namespace std{
  template<> struct is_error_code_enum<DataBaseErr> : public true_type{};
}

class DataBase_category_impl : public std::error_category {
public:
  virtual const char* name() const noexcept { return "DataBase"; };
  virtual std::string message(int ev) const;
};

std::string DataBase_category_impl::message(int ev) const
{
  switch (ev) {
    case (int)DataBaseErr::ok : return "OK";
    case (int)DataBaseErr::can_not_create: return "Unable to create a table";
    case (int)DataBaseErr::can_not_open: return "Unable to open data base";
    case (int)DataBaseErr::wrong_request: return "Wrong request";
    case (int)DataBaseErr::can_not_insert: return "Unable to make insert opeation";
    case (int)DataBaseErr::can_not_delete: return "Unable to make delete opeation";
    default: return "Unknown Data Base error";
  }
}

const std::error_category& DataBase_category(){
  static DataBase_category_impl instance;
  return instance;
}



std::error_code make_error_code(DataBaseErr e){
  return std::error_code(static_cast<int>(e), DataBase_category());
}

std::error_condition make_error_condition(DataBaseErr e){
  return std::error_condition(static_cast<int>(e), DataBase_category());
}


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
  static void _slave(std::mutex& mutex, std::condition_variable& cond, std::vector<job>& v,
    std::atomic<int>& _active, std::atomic<bool>& _end);
  std::atomic<int> _active;
  std::atomic<bool> _end;
};

thread_pool::thread_pool(int thread_num) :
  _thread_num(thread_num),
  _active(thread_num),
  _end(false)
{
  if(thread_num > 0)
    for(int i = 0; i < thread_num; i++)
      slaves.push_back(std::thread(_slave, std::ref(tasksLock), std::ref(cv), std::ref(_tasks), std::ref(_active), std::ref(_end)));
    while(_active != 0);
}

thread_pool::~thread_pool(){
  while(_active != 0);
  _end = true;
  cv.notify_all();
  for(auto& thread : slaves){
    std::cout << "join" << std::endl;
    thread.join();
  }
}

void thread_pool::post(Task task, void* taskData){
  tasksLock.lock();
  _tasks.push_back(job(task, taskData));
  tasksLock.unlock();
  cv.notify_one();
}

void thread_pool::_slave(std::mutex& mutex, std::condition_variable& cond, std::vector<job>& v,
  std::atomic<int>& _active, std::atomic<bool>& _end){
  Task task;
  void* arg;

  _active--;
  while(_active != 0);
  std::cout << _active << std::endl;

  mutex.lock();
  std::cout << "thread " << std::this_thread::get_id() << " created" << std::endl;
  mutex.unlock();

  std::unique_lock<std::mutex> locker(mutex);

  while(_end == false){
     cond.wait(locker, [&](){
       bool empty = !v.empty();
       return (empty || _end);
     });
     if(_end == true){
//        std::cout << "notified" << std::endl;
        break;
      }

     _active++;
    task = v.back()._task;
    arg = v.back()._arg;
    v.pop_back();
    mutex.unlock();
    task(arg);
    _active--;
  }
  mutex.unlock();
  std::cout << "joined " << std::this_thread::get_id() << std::endl;
}

int main(){
  /*std::error_code e;
  e = DataBaseErr::can_not_open;
  std::cout << e.message() << std::endl;*/
  int a = 0;
  thread_pool pool(5);
  pool.post(test, NULL);
  pool.post(test, NULL);
  //std::cin >> a;
  pool.post(test, NULL);
  pool.post(test, NULL);
  pool.post(test, NULL);
  std::cin >> a;
  return 0;
}
