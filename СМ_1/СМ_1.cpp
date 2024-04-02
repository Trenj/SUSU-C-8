#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex mtx; // глобальный мьютекс для синхронизации доступа к файлу результатов

class DataHandler 
{
public:
    virtual void process(std::ifstream& in, std::ofstream& out) = 0;
};

class WordCounter : public DataHandler 
{
public:
    void process(std::ifstream& in, std::ofstream& out) override 
    {
        std::string word;
        int count = 0;

        while (in >> word) 
        {
            ++count;
        }

        std::lock_guard<std::mutex> guard(mtx); // Обеспечение безопасной записи
        out << "Number of words: " << count << std::endl;
    }
};

int main() 
{
    std::vector<std::thread> threads;

    std::vector<std::string> fileNames = { "file1.txt", "file2.txt" };
    std::string resultFileName = "result.txt";

    std::ofstream resultFile(resultFileName);

    for (const auto& fileName : fileNames) {
        std::ifstream infile(fileName);

        threads.emplace_back([&, fileName]() {
            std::ifstream in(fileName);  // Перемещение ifstream внутрь потока
            WordCounter counter;
            counter.process(in, resultFile);
            });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    resultFile.close();

    return 0;
}


/*
Самостоятельная задача: Параллельная обработка текстовых файлов

Создайте систему на C++, которая читает текст из нескольких файлов, 
обрабатывает данные параллельно с использованием лямбда-выражений и многопоточности, 
а затем сохраняет результат в новых файлах. Каждый поток должен обрабатывать текст из своего файла, 
например, выполняя подсчет слов или модифицируя текст определенным образом. 
Используйте наследование для создания общего интерфейса обработчиков данных и мьютексы для синхронизации доступа к ресурсам,
если это необходимо. Результатом работы каждого потока должен быть новый файл, в котором содержится обработанный текст. 
Это задание поможет вам понять, как эффективно использовать многопоточность и лямбда-выражения для выполнения параллельной 
обработки данных, а также даст практику в работе с файлами в C++.

*/