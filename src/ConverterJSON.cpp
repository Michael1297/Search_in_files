#include "ConverterJSON.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Exception.h"
#include "Version.h"
#include "InvertedIndex.h"

using JSON = nlohmann::json;

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    JSON config;
    std::ifstream file("config.json");
    if(!file.is_open()) throw Exception("config file is missing");
    file >> config;
    file.close();
    if(config["config"]["version"] != PROJECT_VERSION) throw Exception("config.json has incorrect file version");
    std::cout << "Starting " << config["config"]["name"] << " " << PROJECT_VERSION << "\n";
    return config["files"];
}

std::vector<std::string> ConverterJSON::GetRequests() {
    JSON requests;
    std::ifstream file("requests.json");
    if(!file.is_open()) throw Exception("requests file is missing");
    file >> requests;
    file.close();
    return requests["requests"];
}

int ConverterJSON::GetResponsesLimit() {
    JSON config;
    std::ifstream file("config.json");
    if(!file.is_open()) throw Exception("config file is missing");
    file >> config;
    file.close();
    return config["config"]["max_responses"];
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers) {
    this->clearAnswers();
    JSON answers_json;
    for(int i = 0; i < answers.size(); i++){
        char request_command[sizeof("request000")];
        sprintf(request_command, "request%03i", i);
        auto& request = answers_json["answers"][request_command];

        if(answers[i].front() == RelativeIndex()){  //не найдено
            request["result"] = false;
            continue;
        } else{
            request["result"] = true;
        }

        for(int j = 0; j < answers[i].size() && j < this->GetResponsesLimit(); j++){
            JSON relevance;
            relevance["docid"] = answers[i][j].doc_id;
            relevance["rank"] = answers[i][j].rank;
            request["relevance"].push_back(relevance);
        }
    }

    std::ofstream file("answers.json");
    file << answers_json.dump(1, '\t');
    file.close();
}

void ConverterJSON::clearAnswers() {
    std::ofstream answers("answers.json", std::ios::trunc);
    if(answers.is_open()) answers.close();
}

void ConverterJSON::search() {
    auto textDocuments = this->GetTextDocuments();
    auto requests = this->GetRequests();
    std::vector<std::string> docs(textDocuments.size());
    //чтение текста из файлов
    for(int i = 0; i < docs.size(); i++){
        std::ifstream file(textDocuments[i], std::ios::binary | std::ios::ate);
        if(!file.is_open()) {
            std::cout << textDocuments[i] << " not find\n";
            continue;
        }
        docs[i].resize(file.tellg());
        file.seekg(0);
        file.read((char*)docs[i].c_str(), docs[i].size());
        file.close();
    }

    InvertedIndex index;
    index.UpdateDocumentBase(docs); //индексация текста
    SearchServer searchServer(index);
    this->putAnswers(searchServer.search(requests));    //поиск и запись результата
}