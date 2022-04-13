#include "InvertedIndex.h"
#include <fstream>
#include <thread>
#include <algorithm>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = input_docs;
    std::vector<std::thread> threads;
    threads.reserve(input_docs.size());

    for(size_t i = 0; i < input_docs.size(); i++){
        threads.emplace_back([&](){
            std::ifstream file(input_docs[i]);
            if(!file.is_open()) return;
            while (!file.eof()){
                std::string word;
                file >> word;
                if(!word.empty()) {
                    auto find = std::find_if(freq_dictionary[word].begin(), freq_dictionary[word].end(), [&i](Entry& element){
                        return element.doc_id == i;
                    });
                    if(find != freq_dictionary[word].end()){
                        find->count++;
                    } else{
                        freq_dictionary[word].push_back({i, 1});
                    }
                }
            }
            file.close();
        });
    }
    for(auto& thread : threads) thread.join();
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    return freq_dictionary[word];
}