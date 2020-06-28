#ifndef CPPJIEBA_KEYWORD_EXTRACTOR_H
#define CPPJIEBA_KEYWORD_EXTRACTOR_H

#include <cmath>
#include <set>
#include "MixSegment.hpp"
#include "./util.h"

namespace cppjieba {

using namespace limonp;
using namespace std;
/*utf8*/
class KeywordExtractor {
 public:
  struct Word {
    string word;
    vector<size_t> offsets;
    double weight;
  }; // struct Word

  KeywordExtractor(
    const string& dictContent, 
    const string& hmmContent, 
    const string& idfContent, 
    const string& stopWordContent, 
    const string& userDict = ""
  ) : segment_(dictContent, hmmContent, userDict) {
    LoadIdfDict(idfContent);
    LoadStopWordDict(stopWordContent);
  }
  
  KeywordExtractor(
    const DictTrie* dictTrie, 
    const HMMModel* model,
    const string& idfContent, 
    const string& stopWordContent
  ) : segment_(dictTrie, model) {
    LoadIdfDict(idfContent);
    LoadStopWordDict(stopWordContent);
  }

  ~KeywordExtractor() {}

  void Extract(const string& sentence, vector<string>& keywords, size_t topN) const {
    vector<Word> topWords;
    Extract(sentence, topWords, topN);
    for (size_t i = 0; i < topWords.size(); i++) {
      keywords.push_back(topWords[i].word);
    }
  }

  void Extract(const string& sentence, vector<pair<string, double> >& keywords, size_t topN) const {
    vector<Word> topWords;
    Extract(sentence, topWords, topN);
    for (size_t i = 0; i < topWords.size(); i++) {
      keywords.push_back(pair<string, double>(topWords[i].word, topWords[i].weight));
    }
  }

  void Extract(const string& sentence, vector<Word>& keywords, size_t topN) const {
    vector<string> words;
    segment_.Cut(sentence, words);

    map<string, Word> wordmap;
    size_t offset = 0;
    for (size_t i = 0; i < words.size(); ++i) {
      size_t t = offset;
      offset += words[i].size();
      if (IsSingleWord(words[i]) || stopWords_.find(words[i]) != stopWords_.end()) {
        continue;
      }
      wordmap[words[i]].offsets.push_back(t);
      wordmap[words[i]].weight += 1.0;
    }
    if (offset != sentence.size()) {
      XLOG(ERROR) << "words illegal";
      return;
    }

    keywords.clear();
    keywords.reserve(wordmap.size());
    for (map<string, Word>::iterator itr = wordmap.begin(); itr != wordmap.end(); ++itr) {
      unordered_map<string, double>::const_iterator cit = idfMap_.find(itr->first);
      if (cit != idfMap_.end()) {
        itr->second.weight *= cit->second;
      } else {
        itr->second.weight *= idfAverage_;
      }
      itr->second.word = itr->first;
      keywords.push_back(itr->second);
    }
    topN = min(topN, keywords.size());
    partial_sort(keywords.begin(), keywords.begin() + topN, keywords.end(), Compare);
    keywords.resize(topN);
  }
 private:
  void LoadIdfDict(const string& idfContent) {
    double idfSum = 0.0;
    int lineno = 0;

    auto addIdfLine = [&idfSum, this](string line) {
      vector<string> buf;
      double idf;
      Split(line, buf, " ");

      if (buf.size() == 2) {
        idf = atof(buf[1].c_str());
        idfMap_[buf[0]] = idf;
        idfSum += idf;
      }
    };

    getSplitLine(idfContent, [&addIdfLine, &lineno, this](int originLineno, std::string line) {
      addIdfLine(line);
      lineno += 1;
    });

    idfAverage_ = idfSum / lineno;
  }

  void LoadStopWordDict(const string& stopWordContent) {
    getSplitLine(stopWordContent, [this](int originLineno, std::string line) {
      stopWords_.insert(line);
    });
  
    assert(stopWords_.size());
  }

  static bool Compare(const Word& lhs, const Word& rhs) {
    return lhs.weight > rhs.weight;
  }

  MixSegment segment_;
  unordered_map<string, double> idfMap_;
  double idfAverage_;

  unordered_set<string> stopWords_;
}; // class KeywordExtractor

inline ostream& operator << (ostream& os, const KeywordExtractor::Word& word) {
  return os << "{\"word\": \"" << word.word << "\", \"offset\": " << word.offsets << ", \"weight\": " << word.weight << "}"; 
}

} // namespace cppjieba

#endif


