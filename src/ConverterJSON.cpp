#include "ConverterJSON.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "Exception.h"
#include "Version.h"
#include "InvertedIndex.h"

#define REQUEST_LENGTH sizeof("request000")

using JSON = nlohmann::json;

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    JSON config;
    std::ifstream file("config.json");
    if(!file.is_open()) throw Exception("config file is missing");
    file >> config;
    file.close();
    if(config["config"]["version"] != PROJECT_VERSION) throw Exception("config.json has incorrect file version");
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
    auto max_responses = this->GetResponsesLimit();
    JSON result;

    for(int answer = 0; answer < answers.size(); answer++){
        char request[REQUEST_LENGTH];
        sprintf(request, "request%03i", answer);

        if(answers[answer].front() != EmptyRelativeIndex){
            result["answers"][request]["result"] = true;
        } else{
            result["answers"][request]["result"] = false;     //не найдено
            continue;
        }

        for(int doc = 0; doc < answers[answer].size() && doc < max_responses; doc++){
            JSON relevance;
            relevance["docid"] = answers[answer][doc].doc_id;
            relevance["rank"] = answers[answer][doc].rank;
            result["answers"][request]["relevance"].push_back(relevance);
        }
    }

    if(result.empty()) result["answers"] = {};  //отсутствуют запросы

    std::ofstream file("answers.json");
    if(!file.is_open()) throw Exception("answers file is not open");
    file << result.dump(1, '\t');
    file.close();
}

ConverterJSON::ConverterJSON() {
    std::ofstream file("answers.json", std::ios::trunc);
    if(!file.is_open()) throw Exception("answers file is not open");
    file.close();
}