#include <iostream> 
#include <vector> 
#include <thread> 
#include <functional> 
#include <mutex> 
#include <chrono> 
#include <locale.h>

// **Мьютекс для синхронизации доступа к стандартному выводу (cout)** 
std::mutex coutMutex;

// **Базовый класс для обработки данных** 
// 
// Абстрактный метод `process` должен быть переопределен в производных классах 
// для реализации конкретной логики обработки данных. 
class DataProcessor 
{ 
public: 
	virtual void process(std::vector<int>& data, std::function<void(int&)> func) = 0; 
};

// **Производный класс, реализующий параллельную обработку данных** 
class ParallelDataProcessor : public DataProcessor 
{ 
public: 
	// **Переопределение метода `process` из базового класса** 
	void process(std::vector<int>& data, std::function<void(int&)> func) override { 
		// **Создание контейнера для потоков** 
		std::vector<std::thread> threads;

		// **Вычисление размера части данных для каждого потока** 
		// (Предполагается, что размер данных кратен 4) 
		int partSize = data.size() / 4;

		// **Запуск потоков для обработки каждой части данных** 
		for (int i = 0; i < 4; ++i) 
		{ 
			// Создание нового потока с использованием лямбда-выражения: 
			// * [&]: Захват внешних переменных по ссылке (для доступа к coutMutex, data и др.). 
			// * [i, func]: Захват `i` (индекс части данных) и `func` (функции обработки) по значению 
			// * для использования внутри лямбда-функции.

			threads.emplace_back([&, i, func]() { 
				// **Определение диапазона обрабатываемых данных для текущего потока** 
				int start = i * partSize; 
				int end = (i + 1) * partSize;

				// **Обработка данных в цикле** 
				for (int j = start; j < end; ++j) 
				{ 
					// **Имитация задержки для демонстрации чередования потоков** 
					std::this_thread::sleep_for(std::chrono::milliseconds(50 * (j - start + 1)));

					// **Выполнение функции `func` для каждого элемента данных** 
					func(data[j]);

					// **Вывод сообщения о выполнении операции**
					std::lock_guard<std::mutex> guard(coutMutex); 
					std::cout << "Поток " << i << " обработал элемент " << j << std::endl; 
				} 
				}); 
		}

		// **Ожидание завершения всех потоков** 
		for (auto& t : threads) 
		{ 
			t.join(); 
		} 
	} 
};

int main() 
{
	setlocale(LC_ALL, "Russian");

	// **Пример данных** 
	std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 8 };

	// **Создание объекта класса ParallelDataProcessor** 
	ParallelDataProcessor processor;

	// **Обработка данных с помощью лямбда-выражения, увеличивающего каждый элемент на 1** 
	processor.process(data, [](int& n) { n += 1; });

	// **Вывод результатов обработки** 
	std::lock_guard<std::mutex> guard(coutMutex); 
	for (int n : data) 
	{ 
		std::cout << n << " "; 
	} 
	std::cout << std::endl;

	return 0;
}