const emModule = require('./jieba.js')
const path = require("path")
const fs = require("fs")
emModule.onRuntimeInitialized = function() {
    const DEFAULT_DICT = path.join(__dirname, "../dict/jieba.dict.utf8")
    const DEFAULT_HMM_DICT = path.join(__dirname, "../dict/hmm_model.utf8")
    const DEFAULT_USER_DICT = path.join(__dirname, "../dict/user.dict.utf8")
    const DEFAULT_IDF_DICT = path.join(__dirname, "../dict/idf.utf8")
    const DEFAULT_STOP_WORD_DICT = path.join(__dirname, "../dict/stop_words.utf8")
    console.log("=========")
    const loadRes = emModule.load(DEFAULT_DICT, DEFAULT_HMM_DICT, DEFAULT_USER_DICT, DEFAULT_IDF_DICT, DEFAULT_STOP_WORD_DICT)
    console.log(loadRes)
}