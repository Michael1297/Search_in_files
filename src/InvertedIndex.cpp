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

    for(size_t id = 0; id < input_docs.size(); id++){
        threads.emplace_back([this, &input_docs, id, &mutex](){
            std::stringstream text(input_docs[id]);     //парсинг текста
            while (true){
                std::string word;
                text >> word;
                if(word.empty()) return;    //текст закончился - выход из лямбды

                mutex.lock();
                //поиск элемента с нужным doc_id
                auto element = std::find_if(freq_dictionary[word].begin(), freq_dictionary[word].end(), [&id](Entry& element){
                    return element.doc_id == id;
                });

                if(element != freq_dictionary[word].end()){   //слово есть в словаре
                    element->count++;
                } else{
                    freq_dictionary[word].push_back({id, 1});    //добавить слово в словарь
                }
                mutex.unlock();
            }
        });
    }
    for(auto& thread : threads) thread.join();

    //отсортировать freq_dictionary по возрастанию doc_id
    for(auto& freq : freq_dictionary){
        std::sort(freq.second.begin(), freq.second.end(), [](Entry& first, Entry& second){
            return first.doc_id < second.doc_id;
        });
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    try{
        return freq_dictionary.at(word);
    }
    catch (...) {   //если слово отсутствует
        return {{}};
    }
}