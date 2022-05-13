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
    auto max_responses = this->GetResponsesLimit();
    JSON answers_json;

    for(int answer = 0; answer < answers.size(); answer++){
        char request[sizeof("request000")];
        sprintf(request, "request%03i", answer);

        if(answers[answer].front() == RelativeIndex()){  //не найдено
            answers_json["answers"][request]["result"] = false;
            continue;
        } else{
            answers_json["answers"][request]["result"] = true;
        }

        for(int doc = 0; doc < answers[answer].size() && doc < max_responses; doc++){
            JSON relevance;
            relevance["docid"] = answers[answer][doc].doc_id;
            relevance["rank"] = answers[answer][doc].rank;
            answers_json["answers"][request]["relevance"].push_back(relevance);
        }
    }

    if(answers_json.empty()) answers_json["answers"] = {};  //отсутствуют запросы

    {
        std::ifstream file("answers.json");
        if(!file.is_open()) throw Exception("answers file is missing");
        file.close();
    }

    std::ofstream file("answers.json");
    file << answers_json.dump(1, '\t');
    file.close();
}