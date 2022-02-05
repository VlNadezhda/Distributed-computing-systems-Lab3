#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <fstream>
#include <string>

#include "data.h"
#include <iostream>
using namespace boost::interprocess;

static pid_t pid;

void handler(int signum) {
  if (signum == SIGINT) {
    kill(pid, SIGINT);
    wait(nullptr);
    shared_memory_object::remove("SharedMemory");
    exit(1);
  }
}

int main(int argc, char **argv) {

    //очищаем файл
  std::fstream o("result.txt", std::fstream::out);
  o.close();
  //открываем очередь сообщений
  message_queue queue(open_only, "message_queue");
  char buf[100];
  //количество считываем символов из очереди
  std::size_t record_n;
  unsigned int priority;


    //создаем общую память
  shared_memory_object shm(open_or_create, "SharedMemory", read_write);
  //выделяем место для общей памяти
  shm.truncate(sizeof(Data));

  // мапим всю общую память для получения адреса
  mapped_region region(shm, read_write);
  // получаем адрес
  void* address = region.get_address();

  //по полученному адресу записываем
  // структуру Data в общую память
  Data* data = new (address) Data;

  pid = fork();
  if (pid == 0) {
      execl("C", "C", NULL);
    _exit(1);
  }

  signal(SIGINT, handler);
  while (true) {
    //получаем сообщение из очереди сообщений
    queue.receive(buf, 100, record_n, priority);
    buf[record_n] = '\0';
    //записываем полученной выражение в файл
    std::fstream out("result.txt",std::fstream::in
    | std::fstream::out | std::fstream::app);
    out << buf << " ";
    out.close();

    //ограниченная блокировка(гарантированно, всегда разблокируется)
    scoped_lock<interprocess_mutex> lock(data->mutex);
    // если в памяти уже есть выражение или результат, ждем
    if (!data->isEmpty || data->isResult) {
      data->b_turn.wait(lock);
    }

    //записываем в общую память выражение
    std::sprintf(data->message, "%s", buf);
    //помечаем логическую переменную, что в памяти есть выражение
    data->isEmpty = false;
    //даем понять процессу С, что сейчас его очередь
    data->c_turn.notify_one();
  }
  return 0;
}
