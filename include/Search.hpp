#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include "InvertedIndex.h"
#include "SearchServer.h"
#include "ConverterJSON.h"

void search() {
    ConverterJSON json;
    auto textDocuments = json.GetTextDocuments();
    auto requests = json.GetRequests();
    std::vector<std::string> docs(textDocuments.size());

    //чтение текста из файлов
    for(int i = 0; i < docs.size(); i++){
        std::ifstream file(textDocuments[i], std::ios::binary | std::ios::ate); //открыть файл с конца
        if(!file.is_open()) {
            std::cout << textDocuments[i] << " not find\n";
            continue;
        }
        docs[i].resize(file.tellg());   //выделить место под текст файла
        file.seekg(0);  //вернуться в начало файла
        file.read((char*)docs[i].c_str(), docs[i].size());
        file.close();
    }

    InvertedIndex index;
    index.UpdateDocumentBase(docs); //индексация текста
    SearchServer search_server(index);
    json.putAnswers(search_server.search(requests));    //поиск и запись результата
}