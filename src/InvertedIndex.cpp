#include "InvertedIndex.h"
#include <fstream>
#include <thread>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = input_docs;
    std::vector<std::thread> threads;
    threads.reserve(input_docs.size());

    for(int i = 0; i < input_docs.size(); i++){
        threads.emplace_back([&](){
            std::ifstream file(input_docs[i]);
            if(!file.is_open()) return;
            while (!file.eof()){
                std::string word;
                file >> word;
                if(!word.empty()) {
                    try{
                        freq_dictionary[word].at(i).count++;
                    }
                    catch (...){
                        freq_dictionary[word].resize(i + 1);
                        freq_dictionary[word][i].doc_id = i;
                        freq_dictionary[word][i].count++;
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