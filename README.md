# Задание

Мы предлагаем реализовать ThreadPool, который может исполнять любую работу в одном из заранее созданных рабочих потоков. И консольное приложение, которое демонстрирует работу ThreadPool. Если пользователь ввёл restart, то существующий ThreadPool уничтожается и создаётся новый с большим количеством рабочих потоков. Если пользователь ввёл exit, то приложение завершается. Если пользователь ввёл два числа, то приложение отправляет работу в ThreadPool по вычислению всех простых делителей первого числа (uint64), а второе число нужно для задания приоритета этой задаче. После завершения работы нужно вывести результат в консоль.

Мы хотим получить программу на современном C++, ведь в современном C++ есть потоки, примитивы синхронизации и много всего хорошего. Для сборки лучше всего использовать CMake. Ещё мы хотим получить текст, который описывает детали реализации.

Уверены, вы блестяще справитесь с заданием!
