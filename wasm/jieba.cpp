#include <string>
#include <vector>
#include "./cppjieba/Jieba.hpp"
#include "./cppjieba/KeywordExtractor.hpp"
#include <fstream>

#include <emscripten/bind.h>

using emscripten::function;
using emscripten::register_vector;
using emscripten::register_map;

using std::vector;
using std::map;
using std::string;
using std::pair;

cppjieba::Jieba* global_jieba_handle;

bool load(
  const string& dictContent,
  const string& modelContent,
  const string& userDictContent,
  const string& idfContent,
  const string& stopWordsContent
) {
  delete global_jieba_handle;
  global_jieba_handle = new cppjieba::Jieba(
    dictContent,
    modelContent,
    userDictContent,
    idfContent,
    stopWordsContent
  );
  return true;
}

bool insertWord(string word, string tag = "x") {
  assert(global_jieba_handle);
  return global_jieba_handle->InsertUserWord(word, tag);
}

vector<string> cut(string sentence, bool useHMM = false) {
  vector<string> words;
  global_jieba_handle->Cut(sentence, words, useHMM);
  return words;
}

vector<string> cutHMM(string sentence) {
  vector<string> words;
  global_jieba_handle->CutHMM(sentence, words); 
  return words;
}

vector<string> cutAll(string sentence) {
  vector<string> words;
  global_jieba_handle->CutAll(sentence, words); 
  return words;
}

vector<string> cutForSearch(string sentence, bool useHMM = false) {
  vector<string> words;
  global_jieba_handle->CutForSearch(sentence, words, useHMM); 
  return words;
}

vector<string> cutSmall(string sentence, size_t word_len_limit) {
  vector<string> words;
  global_jieba_handle->CutSmall(sentence, words, word_len_limit); 
  return words;
}

map<string, string> tag(string sentence) {
  vector<pair<string, string>> words;
  global_jieba_handle->Tag(sentence, words); 

  
  map<string, string> m;
  for (auto item: words) {
    m.insert(item);
  }
  return m;
}

map<string, double> extract(string sentence, size_t topN) {
  vector<pair<string, double>> words;
  global_jieba_handle->extractor.Extract(sentence, words, topN); 

  map<string, double> m;
  for (auto item: words) {
    m.insert(item);
  }
  return m;
}

vector<string> returnVectorData() {
  vector<string> v;
  v.push_back("10");
  v.push_back("11");
  return v;
}

void testReadFile() {
  std::ifstream fileStream("/tmp/test.txt");
  for(std::string line; getline(fileStream, line);) {
    std::cout << line << std::endl;
  }
}

EMSCRIPTEN_BINDINGS(module) {
    // register bindings
    register_vector<int>("vector<int>");
    register_vector<string>("vector<string>");
    register_map<string, double>("map<string, double>");
    register_map<string, string>("map<string, string>");

    function("load", &load);
    function("insertWord", &insertWord);
    function("cut", &cut);
    function("cutHMM", &cutHMM);
    function("cutAll", &cutAll);
    function("cutForSearch", &cutForSearch);
    function("cutSmall", &cutSmall);
    function("tag", &tag);
    function("extract", &extract);
  
    function("returnVectorData", &returnVectorData);
    function("testReadFile", &testReadFile);
}