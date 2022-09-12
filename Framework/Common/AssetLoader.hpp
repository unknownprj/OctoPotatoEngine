#pragma once

#include <cstdio>
#include <string>
#include <utility>
#include <vector>
#include "IRuntimeModule.hpp"
#include "Buffer.hpp"

namespace OctoPotato {
    class AssetLoader : implements IRuntimeModule {
    public:
        virtual ~AssetLoader() {};
        virtual int initialize();
        virtual void finalize();
        virtual void tick();

        typedef void *AssetFilePtr;

        enum AssetOpenMode {
            ASSET_OPEN_TEXT     = 0,
            ASSET_OPEN_BINARY   = 1
        };

        enum AssetSeekBase {
            ASSET_SEEK_SET = 0,
            ASSET_SEEK_CUR,
            ASSET_SEEK_END
        };

        bool addSearchPath(const char *path);

        bool removeSearchPath(const char *path);

        bool fileExists(const char *filePath);

        AssetFilePtr openFile(const char *name, AssetOpenMode mode);

        Buffer &syncOpenAndReadText(const char *filePath);

        size_t syncRead(const AssetFilePtr &fp, Buffer &buf);

        void closeFile(AssetFilePtr &fp);

        size_t getSize(const AssetFilePtr &fp);

        int32_t seek(AssetFilePtr fp, long offset, AssetSeekBase where);

        std::string syncOpenAndReadTextFileToString(const char *fileName) {
            std::string result;
            Buffer buffer = syncOpenAndReadText(fileName);
            char *content = reinterpret_cast<char*>(buffer.data());

            if(content) {
                result = std::string(std::move(content));
            }

            return result;
        }
    private:
        std::vector<std::string> m_searchPaths;
    };
}