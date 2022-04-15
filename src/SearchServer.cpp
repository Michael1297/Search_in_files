#include "SearchServer.h"
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <map>

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> result(queries_input.size());

    for(int i = 0; i < queries_input.size(); i++){
        std::vector<std::string> words;
        std::stringstream text;
        text << queries_input[i];
        while(true){
            std::string word;
            text >> word;
            if(word.empty()) break;
            words.push_back(word);
        }
        std::sort(words.begin(), words.end());
        words.erase(std::unique(words.begin(), words.end()), words.end());  //удалить дубликаты слов

        //отсортировать элементы по количеству файлов, в которых находятся эти слова
        std::sort(words.begin(), words.end(),[this](std::string& first, std::string& second){
            return _index.GetWordCount(first).size() < _index.GetWordCount(second).size();
        });

        std::unordered_set<size_t> doc_id; // id документов редкого слова
        for(auto& index : _index.GetWordCount(words.front())){
            doc_id.insert(index.doc_id);
        }

        //создание списка совпадений
        for(int j = 1; j < words.size(); j++){
            std::unordered_set<size_t> another_doc_id; //id документов других слов
            for(auto& index : _index.GetWordCount(words[j])){
                another_doc_id.insert(index.doc_id);
            }
            for(auto& id : doc_id){
                if(!another_doc_id.count(id)) doc_id.erase(id); //в других документах нет id из списка редкого слова
            }
        }

        std::map<size_t, size_t> relevance; //релевантность документа
        for(auto& word : words){
            for(auto& index : _index.GetWordCount(word)){
                if(doc_id.count(index.doc_id) && index.count){
                    relevance[index.doc_id] += index.count;
                }
            }
        }

        size_t max_relevance = 0; //максимальная релевантность для всех документов
        for(auto& doc : relevance){
            if(doc.second > max_relevance) max_relevance = doc.second;
        }

        for(auto& doc : relevance){     //вставка результатов поиска
            result[i].push_back({doc.first, (float)doc.second / (float)max_relevance});
        }

        if(result[i].empty()) {
            result[i].push_back ({});  //если ничего не найдено
        }

        //сортировка результатов поиска
        std::sort(result[i].begin(), result[i].end(), [](RelativeIndex& first, RelativeIndex& second){
            return first.rank > second.rank;
        });
    }
    return result;
}