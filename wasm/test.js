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
    const DEFAULT_DICT_PATH = "jieba.dict.utf8"
    const DEFAULT_HMM_DICT_PATH = "hmm_model.utf8"
    const DEFAULT_USER_DICT_PATH = "user.dict.utf8"
    const DEFAULT_IDF_DICT_PATH = "idf.utf8"
    const DEFAULT_STOP_WORD_DICT_PATH = "stop_words.utf8"
    
    const filePaths = [DEFAULT_DICT_PATH, DEFAULT_HMM_DICT_PATH, DEFAULT_USER_DICT_PATH, DEFAULT_IDF_DICT_PATH, DEFAULT_STOP_WORD_DICT_PATH]
    for (const filePath of filePaths) {
        const fileContent = fs.readFileSync(path.join(__dirname, "../dict", filePath));
        emModule['FS_createDataFile']("/tmp", filePath, fileContent, true, true)
    }
    const now = Date.now()
    const loadRes = emModule.load(...filePaths.map(filePath => `/tmp/${filePath}`))
    console.log(`${Date.now() - now}ms`)
    console.log(loadRes)

    const cutVec = emModule.cut("南京市长江大桥", false);
    const cutArr = vectorToArray(cutVec)
    console.log(cutArr)
    const tagMap = emModule.tag("红掌拨清波")
    const tagObj = mapToObject(tagMap)
    console.log(tagObj)
}
