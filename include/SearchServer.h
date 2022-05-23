#pragma once
#include "InvertedIndex.h"

#define EmptyRelativeIndex RelativeIndex()

struct RelativeIndex{
    size_t doc_id;
    float rank;

    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }

    bool operator !=(const RelativeIndex& other) const {
        return (doc_id != other.doc_id || rank != other.rank);
    }

    RelativeIndex() = default;

    RelativeIndex(size_t _doc_id, float _rank): doc_id(_doc_id), rank(_rank){}
};


class SearchServer {
public:
/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
    SearchServer(InvertedIndex& idx) : _index(idx){ };
/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
private:
    InvertedIndex _index;
};