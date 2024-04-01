#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>

class DataHandler {
public:
    virtual void process(std::ifstream& in, std::ofstream& out) = 0;
};

class WordCounter : public DataHandler {
public:
    void process(std::ifstream& in, std::ofstream& out) override {
        std::string word;
        int count = 0;
        while (in >> word) {
            ++count;
        }
        out << "Number of words: " << count << std::endl;
    }
};

int main() {
    std::vector<std::thread> threads;
    std::mutex mtx;

    // Замените эти имена на имена ваших файлов
    std::vector<std::string> fileNames = { "file1.txt", "file2.txt", "file3.txt" };

    for (const auto& fileName : fileNames) {
        std::ifstream in(fileName);
        std::ofstream out(fileName + ".processed");

        WordCounter counter;
        threads.push_back(std::thread([&] {
            counter.process(in, out);
            }));
    }

    for (auto& thread : threads) {
        thread.join();
    }

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