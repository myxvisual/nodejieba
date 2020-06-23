#include <string>
#include <vector>
#include "./cppjieba/Jieba.hpp"
#include "./cppjieba/KeywordExtractor.hpp"

#include <emscripten/bind.h>

using emscripten::function;

using std::vector;
using std::string;
using std::pair;

cppjieba::Jieba* global_jieba_handle;

bool load(
  string dictPath,
  string modelContent,
  string userDictPath,
  string idfPath,
  string stopWordsPath
) {
  delete global_jieba_handle;
  global_jieba_handle = new cppjieba::Jieba(
    dictPath,
    modelContent,
    userDictPath,
    idfPath,
    stopWordsPath
  );
  return true;
}

bool insertWord(string word, string tag = "x") {
  assert(global_jieba_handle);
  return global_jieba_handle->InsertUserWord(word, tag);
}

vector<string> cut(const std::string &sentence, bool useHMM = false) {
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

vector<pair<string, string>> tag(string sentence) {
  vector<pair<string, string>> words;
  global_jieba_handle->Tag(sentence, words); 
  return words;
}

vector<pair<string, double>> extract(string sentence, size_t topN) {
  vector<pair<string, double>> words;
  global_jieba_handle->extractor.Extract(sentence, words, topN); 
  return words;
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("load", &load);
    function("insertWord", &insertWord);
    function("cut", &cut);
    function("cutHMM", &cutHMM);
    function("cutAll", &cutAll);
    function("cutForSearch", &cutForSearch);
    function("cutSmall", &cutSmall);
    function("tag", &tag);
    function("extract", &extract);
}