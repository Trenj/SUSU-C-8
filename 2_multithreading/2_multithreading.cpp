#include <iostream> 
#include <thread> 
#include <mutex> 
#include <locale.h>

// **Мьютекс для синхронизации доступа к стандартному выводу (cout)** 
std::mutex coutMutex;

// **Функция, демонстрирующая работу потока с определенной частью данных** 
// 
// @param threadNum - Номер потока 
// @param start - Начальный индекс обрабатываемых данных 
// @param end - Конечный индекс обрабатываемых данных 
void threadWork(int threadNum, int start, int end) 
{ 
	for (int i = start; i < end; ++i) 
	{ 
		// **Блокировка мьютекса для обеспечения синхронного доступа к cout** 
		{ 
			std::lock_guard<std::mutex> guard(coutMutex); 
			// **Вывод сообщения о том, что поток работает над определенным элементом** 
			std::cout << "Поток " << threadNum << " обрабатывает элемент " << i << std::endl; 
		} 
		// **Имитация работы потока путем введения небольшой задержки** 
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
	} 
}

int main() {
	setlocale(LC_ALL, "Russian");

	// **Запуск двух потоков, каждый из которых обрабатывает свою часть данных** 
	std::thread t1(threadWork, 1, 0, 5); 
	std::thread t2(threadWork, 2, 5, 10);

	// **Ожидание завершения работы потоков** 
	t1.join(); 
	t2.join();

	return 0;
}