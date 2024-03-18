#include <iostream> 
#include <vector> 
#include <algorithm>	// Для std::for_each 
#include <functional>	// Для std::function

// Базовый класс Processor с виртуальной функцией process для обработки данных 
class Processor 
{ 
public: 
	// Функция для обработки данных, принимает вектор и лямбда-выражение 
	virtual void process(std::vector<int>& data, std::function<void(int&)> func) = 0; 
};

// Производный класс LambdaProcessor, наследующий от Processor 
class LambdaProcessor : public Processor 
{ 
public: 
	// Переопределение функции process с использованием лямбда-выражения в качестве параметра 
	void process(std::vector<int>& data, std::function<void(int&)> func) override { 
		// Применение лямбда-выражения к каждому элементу вектора 
		std::for_each(data.begin(), data.end(), func); 
	} 
};

int main() 
{
	std::vector<int> data = { 1, 2, 3, 4, 5 }; // Исходные данные

	LambdaProcessor processor; // Создание экземпляра производного класса

	// Вызов функции обработки данных с лямбда-выражением для увеличения каждого элемента на 1 
	processor.process(data, [](int& val) { val += 1; }); 
	// Вывод результатов после первой обработки 
	for (int val : data) 
	{ 
		std::cout << val << " "; // Должно вывести 2 3 4 5 6 
	} 
	std::cout << std::endl;

	// Вызов функции обработки данных с другим лямбда-выражением для умножения каждого элемента на 2 
	processor.process(data, [](int& val) { val *= 2; }); 
	// Вывод результатов после второй обработки 
	for (int val : data) 
	{ 
		std::cout << val << " "; // Должно вывести 4 6 8 10 12 
	} 
	std::cout << std::endl;

	return 0;
}