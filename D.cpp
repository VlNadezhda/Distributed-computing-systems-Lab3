#include <signal.h>

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <fstream>
#include <string>

#include "data.h"

using namespace boost::interprocess;

void handler(int signum) {
  if (signum == SIGINT) exit(1);
}

int main(int argc, char **argv) {
  shared_memory_object shm(
          open_only,"SharedMemory", read_write);

  mapped_region region(shm, read_write);

  void* addr = region.get_address();

  Data* data = static_cast<Data*>(addr);
  
  signal(SIGUSR1, handler);

  while (true) {
    scoped_lock<interprocess_mutex> lock(data->mutex);
    // если в памяти есть выражение или нет результата, ждем
    if (!data->isEmpty || !data->isResult) {
      data->d_turn.wait(lock);
    }

    //пишем результат в файл
    std::fstream out("result.txt",std::fstream::in
    | std::fstream::out | std::fstream::app);
    out << " = " << data->message << "\n";
    out.close();
    
    //ставим логическую переменную на то,
    // что в памяти уже нет результата
    data->isResult = false;

    //пробуждаем процесс В
    data->d_turn.notify_one();
  }
  
  return 0;
}

