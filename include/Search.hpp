#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include "InvertedIndex.h"
#include "SearchServer.h"
#include "ConverterJSON.h"
#include "Version.h"

void search() {
    std::cout << "Starting " << PROJECT_NAME << " " << PROJECT_VERSION << "\n";
    ConverterJSON json;
    std::cout << "Get text documents\n";
    auto textDocuments = json.GetTextDocuments();
    std::cout << "Get requests\n";
    auto requests = json.GetRequests();
    std::vector<std::string> docs(textDocuments.size());

    std::cout << "Reading text from files\n";
    //чтение текста из файлов
    for(int i = 0; i < docs.size(); i++){
        std::ifstream file(textDocuments[i], std::ios::binary | std::ios::ate); //открыть файл с конца
        if(!file.is_open()) {
            std::cout << textDocuments[i] << " not find\n";
            continue;
        }
        docs[i].resize(file.tellg());   //выделить место под текст файла
        file.seekg(0);  //вернуться в начало файла
        file.read((char*)docs[i].c_str(), docs[i].length());
        file.close();
    }

    InvertedIndex index;
    std::cout << "Text indexing\n";
    index.UpdateDocumentBase(docs); //индексация текста
    SearchServer server(index);
    std::cout << "Search\n";
    auto search_results = server.search(requests);  //поиск
    std::cout << "Put answers\n";
    json.putAnswers(search_results);    //запись результатов
}