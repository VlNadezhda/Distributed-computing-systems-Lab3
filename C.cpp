#include <unistd.h>
#include <signal.h>

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <fstream>
#include <string>

#include "Calculate.h"
#include "data.h"

using namespace boost::interprocess;

static pid_t pid;

void handler(int signum) {
  if (signum == SIGINT) {
    kill(pid, SIGINT);
    exit(1);
  }
}

int main(int argc, char **argv) {
  shared_memory_object shm(open_only, "SharedMemory", read_write);

  mapped_region region(shm, read_write);

  void* addr = region.get_address();

  //т.к. структура уже есть в памяти,
  // просто читаем ее в переменную data
  Data* data = static_cast<Data*>(addr);
  
  //форкаем для создания процесса D
  pid = fork();
  if (pid == 0) {
    //создаем его
      execl("D", "D", NULL);
    _exit(1);
  }

  signal(SIGINT, handler);
  while (true) {
    scoped_lock<interprocess_mutex> lock(data->mutex);
    // если в памяти нет выражения или есть результат, ждем
    if (data->isEmpty || data->isResult) {
      data->c_turn.wait(lock);
    }

    Calculate c;
    double result = c.calculate(data->message);
    printf(" %f\n", result);

      //записываем результат в память
    std::sprintf(data->message, "%f", std::move(result));
    //помечаем логические переменные на то,
    // что в памяти нет выражения, но есть результат

    data->isEmpty = true;
    data->isResult = true;

    //пробуждаем процесс D
    data->d_turn.notify_one();
  }
  return 0;
}


