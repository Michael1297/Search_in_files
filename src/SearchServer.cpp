#include "SearchServer.h"

#include <cmath>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <map>

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> result(queries_input.size());

    for(int query = 0; query < queries_input.size(); query++){
        std::unordered_set<std::string> words;  //список слов
        std::stringstream text(queries_input[query]);
        while(true){
            std::string word;
            text >> word;
            if(word.empty()) break;
            words.insert(word);
        }

        if(words.empty()){      //пустой запрос
            result[query].emplace_back();
            continue;
        }

        std::map<size_t, size_t> relevance; //релевантность документов
        for(auto& word : words){
            for(auto& word_count : _index.GetWordCount(word)){
                if(word_count != EmptyEntry) relevance[word_count.doc_id] += word_count.count;
            }
        }

        size_t max_relevance = 0; //максимальная релевантность для всех документов
        for(auto& doc_relevance : relevance){
            if(doc_relevance.second > max_relevance) max_relevance = doc_relevance.second;
        }

        for(auto& doc_relevance : relevance){     //вставка результатов поиска
            float rank = (float)doc_relevance.second / (float)max_relevance;  //ранг документа
            rank = std::round(rank * 100.f) / 100.f;  //округление до сотых
            result[query].push_back({doc_relevance.first, rank});
        }

        if(result[query].empty()) result[query].emplace_back();  //если ничего не найдено

        //сортировка результатов поиска
        std::sort(result[query].begin(), result[query].end(), [](RelativeIndex& first, RelativeIndex& second){
            return first.rank > second.rank || (first.rank == second.rank && first.doc_id < second.doc_id);
        });
    }
    return result;
}