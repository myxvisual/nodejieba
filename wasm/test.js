const emModule = require('./jieba.js')
const path = require("path")
const fs = require("fs")

function vectorToArray(vec) {
    const size = vec.size();
    let arr = [];
    for (let i = 0; i < size; i++) {
        const v = vec.get(i)
        arr.push(v)
    }

    return arr
}
function mapToObject(map) {
    const keys = map.keys();
    const keySize = keys.size()
    const obj = {}
    
    for (let i = 0; i < keySize; i++) {
        const key = keys.get(i)
        const value = map.get(key)
        obj[key] = value
    }

    return obj
}

emModule.onRuntimeInitialized = function() {
    // emModule['FS_createDataFile']("/tmp", "test.txt", "test-content", true, true)
    // emModule.testReadFile();
    const DEFAULT_DICT = fs.readFileSync(path.join(__dirname, "../dict/jieba.dict.utf8"), "utf-8")
    const DEFAULT_HMM_DICT = fs.readFileSync(path.join(__dirname, "../dict/hmm_model.utf8"), "utf-8")
    const DEFAULT_USER_DICT = fs.readFileSync(path.join(__dirname, "../dict/user.dict.utf8"), "utf-8")
    const DEFAULT_IDF_DICT = fs.readFileSync(path.join(__dirname, "../dict/idf.utf8"), "utf-8")
    const DEFAULT_STOP_WORD_DICT = fs.readFileSync(path.join(__dirname, "../dict/stop_words.utf8"), "utf-8")

    const now = Date.now()
    const loadRes = emModule.load(DEFAULT_DICT, DEFAULT_HMM_DICT, DEFAULT_USER_DICT, DEFAULT_IDF_DICT, DEFAULT_STOP_WORD_DICT)
    console.log(loadRes)
    console.log(`${Date.now() - now}ms`)
    // ~3616 ms --> ~2789ms

    const cutVec = emModule.cut("南京市长江大桥", false);
    const cutArr = vectorToArray(cutVec)
    console.log(cutArr)
    const tagMap = emModule.tag("红掌拨清波")
    const tagObj = mapToObject(tagMap)
    console.log(tagObj)
}