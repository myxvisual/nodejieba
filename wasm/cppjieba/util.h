#ifndef JIEBA_UTILS_H_HEADR
#define JIEBA_UTILS_H_HEADR
#include <iostream>

template <typename Functor>
inline void getSplitLine(std::string content, Functor cb) {
    auto contentSize = content.size();
    auto n = "\n";
    auto r = "\r";
    auto rn = "\r\n";
    int lineno = 0;
    size_t pos = 0;

    for (size_t i = 0; i < contentSize; ++i) {
        auto signleChar = &content[i];
        bool isN = false;
        bool isR = strncmp(r, signleChar, 1) == 0;
        bool isRN = false;

        if (!isR) {
            isN = strncmp(n, signleChar, 1) == 0;
        }
        if (isR) {
            auto nextChar = &content[i + 1];
            isRN = strncmp(n, nextChar, 1) == 0;
        }

        bool isNewLine = isN || isR;
        if (isNewLine) {
            auto line = content.substr(pos, i - pos);
            pos = i + (isRN ? 2 : 1);
            cb(lineno, line);
            lineno += 1;
        }
    }

    auto line = content.substr(pos, contentSize - pos);
    cb(lineno, line);
    lineno += 1;
}

#endif