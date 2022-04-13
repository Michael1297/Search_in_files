#include "InvertedIndex.h"
#include <thread>
#include <sstream>
#include <algorithm>
#include <mutex>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = input_docs;
    freq_dictionary.clear();    //удалить старые значения
    std::mutex mutex;
    std::vector<std::thread> threads;
    threads.reserve(input_docs.size());

    for(size_t i = 0; i < input_docs.size(); i++){
        threads.emplace_back([&](){
            std::stringstream text;
            text << input_docs[i];  //парсинг текста
            while (true){
                std::string word;
                text >> word;
                if(word.empty()) return;    //текст закончился - выход из лямбды

                //поиск элементы с нужным doc_id
                auto entry = std::find_if(freq_dictionary[word].begin(), freq_dictionary[word].end(), [&i](Entry& element){
                    return element.doc_id == i;
                });

                if(entry != freq_dictionary[word].end()){   //слово есть в словаре
                    mutex.lock();
                    entry->count++;
                    mutex.unlock();
                } else{
                    mutex.lock();
                    freq_dictionary[word].push_back({i, 1});    //добавить слово в словарь
                    mutex.unlock();
                }
            }
        });
    }
    for(auto& thread : threads) thread.join();
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    return freq_dictionary[word];
}