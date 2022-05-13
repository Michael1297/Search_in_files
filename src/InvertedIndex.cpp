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
                auto entry = std::find_if(freq_dictionary[word].begin(), freq_dictionary[word].end(), [&id](Entry& element){
                    return element.doc_id == id;
                });

                if(entry != freq_dictionary[word].end()){   //слово есть в словаре
                    entry->count++;
                } else{
                    freq_dictionary[word].push_back({id, 1});    //добавить слово в словарь
                }
                mutex.unlock();
            }
        });
    }
    for(auto& thread : threads) thread.join();
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    try{
        auto wordCount = freq_dictionary.at(word);
        std::sort(wordCount.begin(), wordCount.end(), [](Entry& first, Entry& second){  //отсортировать по возрастанию doc_id
            return first.doc_id < second.doc_id;
        });
        return wordCount;
    }
    catch (...) {   //если слово отсутствует
        return {{}};
    }
}