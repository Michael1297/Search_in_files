#include "SearchServer.h"
#include <sstream>
#include <algorithm>

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
        words.erase(std::unique(words.begin(), words.end()), words.end());  //удалить дубликаты

        //отсортировать элементы по количеству файлов, в которых находятся эти слова
        std::sort(words.begin(), words.end(),[this](std::string& first, std::string& second){
            return _index.GetWordCount(first).size() < _index.GetWordCount(second).size();
        });

        auto first_request = _index.GetWordCount(words.front());    //самое редкое слово
        std::sort(first_request.begin(), first_request.end(), [](Entry& first, Entry& second){
            return first.doc_id < second.doc_id;
        });

        //поиск совпадений
        for(int j = 1; j < words.size(); j++){
            auto request = _index.GetWordCount(words[j]);
            std::sort(request.begin(), request.end(), [](Entry& first, Entry& second){
                return first.doc_id < second.doc_id;
            });
            std::vector<Entry> temp_buffer; //хранение совпадений
            std::set_intersection(first_request.begin(), first_request.end(),   //добавление совпадений в temp_buffer
                                  request.begin(), request.end(),
                                  std::back_inserter(temp_buffer),
                                  [](Entry& first, Entry& second){
                return first.doc_id < second.doc_id;
            });
            std::swap(first_request, temp_buffer);
        }

        size_t total_relevance = 0; //релевантность для всех документов
        for(auto& req : first_request) total_relevance += req.count;

        for(auto& req : first_request){ //выдача результатов поиска
            result[i].push_back({req.doc_id, (float)req.count / (float)total_relevance});
        }

        //сортировка результатов поиска
        std::sort(result[i].begin(), result[i].end(), [](RelativeIndex& first, RelativeIndex& second){
            return first.rank > second.rank;
        });

    }
    return result;
}