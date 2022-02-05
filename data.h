#ifndef DATA_H
#define DATA_H

#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

#define MESSAGE_LENGTH 100

class Data {
 public:
 // инициализация переменных
  Data() : message{""}, isEmpty{true}, isResult{false}{}

  // мьютекс переменная для ограниченной блокировки
  boost::interprocess::interprocess_mutex mutex;
  
  // условные переменные
  boost::interprocess::interprocess_condition b_turn;
  boost::interprocess::interprocess_condition c_turn;
  boost::interprocess::interprocess_condition d_turn;

  // строка, в которой хранится выражение
  char message[MESSAGE_LENGTH];

  // логические переменные для попеременного доступа к памяти
  bool isEmpty;
  bool isResult;
};

#endif  // DATA_H
