#include "AssetLoader.hpp"

using namespace OctoPotato;

int AssetLoader::initialize() { return 0; }

void AssetLoader::finalize() { m_searchPaths.clear(); }

void AssetLoader::tick() {}

bool AssetLoader::addSearchPath(const char* path) {
    std::vector<std::string>::iterator src = m_searchPaths.begin();

    while (src != m_searchPaths.end()) {
        if (!(*src).compare(path))
            return true;
        ++src;
    }

    m_searchPaths.push_back(path);
    return true;
}

bool AssetLoader::removeSearchPath(const char* path) {
    auto src = m_searchPaths.begin();

    while (src != m_searchPaths.end()) {
        if (!(*src).compare(path)) {
            m_searchPaths.erase(src);
            return true;
        }
        ++src;
    }

    return true;
}

bool AssetLoader::fileExists(const char* filePath) {
    AssetFilePtr fp = openFile(filePath, ASSET_OPEN_BINARY);
    if (fp != nullptr) {
        closeFile(fp);
        return true;
    }
    return false;
}

AssetLoader::AssetFilePtr AssetLoader::openFile(const char* name, AssetOpenMode mode) {
    FILE* fp = nullptr;

    std::string upPath;
    std::string fullPath;
    for (int32_t i = 0; i < 5; i++) {
        std::vector<std::string>::iterator src = m_searchPaths.begin();
        bool looping = true;
        while (looping) {
            fullPath.assign(upPath);
            if (src != m_searchPaths.end()) {
                fullPath.append(*src);
                fullPath.append("/Assets/");
                ++src;
            }
            else {
                fullPath.append("Assets/");
                looping = false;
            }
            fullPath.append(name);
#ifdef DEBUG
            fprintf(stderr, "Trying to open file: %s\n", fullPath.c_str());
#endif
            switch (mode) {
            case ASSET_OPEN_TEXT:
                fp = fopen(fullPath.c_str(), "r");
                break;
            case ASSET_OPEN_BINARY:
                fp = fopen(fullPath.c_str(), "rb");
                break;
            }

            if (fp)
                return static_cast<AssetFilePtr>(fp);
        }

        upPath.append("../");
    }

    return nullptr;
}

const std::string AssetLoader::searchFile(const char* name) {
    FILE* fp = nullptr;

    std::string upPath;
    std::string fullPath;
    for (int32_t i = 0; i < 5; i++) {
        std::vector<std::string>::iterator src = m_searchPaths.begin();
        bool looping = true;
        while (looping) {
            fullPath.assign(upPath);
            if (src != m_searchPaths.end()) {
                fullPath.append(*src);
                fullPath.append("/Assets/");
                ++src;
            }
            else {
                fullPath.append("Assets/");
                looping = false;
            }
            fullPath.append(name);
#ifdef DEBUG
            fprintf(stderr, "Trying to open file: %s\n", fullPath.c_str());
#endif

            fp = fopen(fullPath.c_str(), "r");

            if (fp) {
                std::clog << "file found at: " << fullPath << std::endl;
                return fullPath;
            }
        }

        upPath.append("../");
    }
    std::clog << "file" << name << "not found" << std::endl;

    return "";
}

Buffer& AssetLoader::syncOpenAndReadText(const char* filePath) {
    AssetFilePtr fp = openFile(filePath, ASSET_OPEN_TEXT);
    Buffer* pBuff = nullptr;

    if (fp) {
        size_t length = getSize(fp);

        pBuff = new Buffer(length + 1);
        fread(pBuff->data(), length, 1, static_cast<FILE*>(fp));
        (pBuff->data())[length] = '\0';

        closeFile(fp);
    }
    else {
        fprintf(stderr, "Failed to opening file: %s\n", filePath);
        pBuff = new Buffer();
    }

#ifdef DEBUG
    fprintf(stderr, "Reading file: %s, %d bytes\n", filePath, length);
#endif

    return *pBuff;
}

void AssetLoader::closeFile(AssetFilePtr& fp) {
    fclose((FILE*)fp);
    fp = nullptr;
}

size_t AssetLoader::getSize(const AssetFilePtr& fp) {
    FILE* _fp = static_cast<FILE*>(fp);

    long pos = ftell(_fp);
    fseek(_fp, 0, SEEK_END);
    size_t length = ftell(_fp);
    fseek(_fp, pos, SEEK_SET);

    return length;
}

size_t AssetLoader::syncRead(const AssetFilePtr& fp, Buffer& buf) {
    size_t sz;

    if (!fp) {
        fprintf(stderr, "[AssetLoader::syncRead] Null file discriptor\n");
        return 0;
    }

    sz = fread(buf.data(), buf.size(), 1, static_cast<FILE*>(fp));

#ifdef DEBUG
    fprintf(stderr, "Reading %d bytes\n", sz);
#endif

    return sz;
}

int32_t AssetLoader::seek(AssetFilePtr fp, long offset, AssetSeekBase where) {
    return fseek(static_cast<FILE*>(fp), offset, static_cast<int>(where));
}