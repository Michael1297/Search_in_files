#include "ConverterJSON.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Exception.h"
#include "Version.h"

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

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    JSON answers_json;
    for(int i = 0; i < answers.size(); i++){
        auto& request = answers_json["answers"]["request" + std::to_string(i)];
        if(!answers[i].empty()){
            request["result"] = true;
        } else{
            request["result"] = false;
            continue;
        }

        for(auto& j : answers[i]){
            JSON relevance;
            relevance["docid"] = j.first;
            relevance["rank"] = j.second;
            request["relevance"].push_back(relevance);
        }
    }

    std::ofstream file("answers.json");
    file << answers_json;
    file.close();
}

void ConverterJSON::clearAnswers() {
    std::ofstream answers("answers.json", std::ios::trunc);
    if(answers.is_open()) answers.close();
}
