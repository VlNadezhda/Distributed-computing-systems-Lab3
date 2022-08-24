# Distributed-computing-systems-Lab3

Входные данные
time_interval = интервал времени через который будет генерироваться сообщение.      

Пояснение к решению задачи.

Потоки могут помещать сообщения в очередь(message queue), а также удалять сообщения       
из очереди. Каждое сообщение также может иметь приоритет, так что сообщения с более       
высоким приоритетом читаются перед сообщениями с более низким приоритетом.       

При создании очереди сообщений пользователь должен указать максимальный размер сообщения         
и максимальное количество сообщений, которое может хранить очередь сообщений.        

Shared memory(совместно используемая память) - это самый быстрый механизм межпроцессного взаимодействия.        
Операционная система отображает сегмент памяти в адресное пространство нескольких процессов,         
так что несколько процессов могут читать и писать в этом сегменте памяти без вызова функций операционной системы.       

Когда создаётся объект разделяемой памяти, его размер равен 0. Чтобы установить размер разделяемой памяти,         
пользователь должен использовать вызов функции truncate:        
shm_obj.truncate(10000);        

Пользователь может отобразить всю разделяемую память или только её часть.        
Процесс отображения выполняется с использованием класса mapped_region.         
Mutexes гарантируют, что только один поток может заблокировать данный мьютекс.         

Мы не можем использовать мьютекс для эффективного ожидания момента выполнения условия для продолжения.        
Для этого существует condition variable(переменная условия) которая может:      

wait : поток блокируется до тех пор, пока какой-либо другой поток не уведомит о том,        
что он может продолжить, потому что условие, приводящее к ожиданию, исчезло.   

notify : поток отправляет сигнал одному заблокированному потоку или всем заблокированным потокам,         
чтобы сообщить им, что условие, вызвавшее их ожидание, исчезло.
