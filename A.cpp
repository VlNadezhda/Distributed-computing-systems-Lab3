#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

#include <boost/interprocess/ipc/message_queue.hpp>
#include <string>
#include<iostream>

using namespace boost::interprocess;

static pid_t pid;


std::string gen_message() {
  std::string buf;

  srand(time(NULL));
  buf += std::to_string(2 + rand() % 10);

    //рандомим операцию
    switch (rand() % 4) {
      case 0:
        buf += "+";
        break;
      case 1:
        buf += "-";
        break;
      case 2:
        buf += "*";
        break;
      case 3:
        buf += "/";
        break;
    }

  //добавляем еще одно число в конец 
  buf += std::to_string(2 + rand() % 10);

  return buf;
}

//функция, выполняющаяся при закрытии программы
void handler(int signum) {
  if (signum == SIGINT) {
    kill(pid, SIGINT);
    wait(nullptr);
    message_queue::remove("queue");
    exit(1);
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr<<"wrong arguments!\n";
    return 0;
  }

  char* pathName = "B";

  message_queue::remove("message_queue");
  int time_interval = atoi(argv[1]);
  // создание очереди сообщений с именем queue, максимальным
  // количеством сообщений 1, и его размером 100
  message_queue queue(create_only, "message_queue", 1, 100);

  // форкаем, чтобы создать процесс В
  pid = fork();
  if (pid == 0) {
    //  загружаем и запускаем В
      execl(pathName, pathName, NULL);
    _exit(1);
  }

  // обработчик сигнала, при завершении из терминала
  signal(SIGINT, handler);
  while (true) {
    std::string buf = gen_message();
    printf("%s\n", buf.c_str());

    queue.send(buf.c_str(), buf.size(), 1);
    sleep(time_interval);
  }
}

