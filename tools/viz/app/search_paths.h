#ifndef VIZ_APP_SEARCH_PATHS_H
#define VIZ_APP_SEARCH_PATHS_H

#include "filesystem/path.h"
#include <fstream>

namespace viz {
    namespace app {
        class SearchPaths {
           public:
            SearchPaths() {}
            void AddSearchPath(filesystem::path path) { paths_.push_back(path); }
            filesystem::path FindFile(filesystem::path file_name)
            {
                if (file_name.is_file()) {
                    return file_name;
                }

                for (const auto& path : paths_) {
                    auto full_path = path / file_name;
                    if (full_path.is_file()) {
                        return full_path;
                    }
                }
                return filesystem::path();
            }

            std::string LoadFile(filesystem::path file_name)
            {
                return ReadFile(FindFile(file_name));
            }

           private:
            static std::string ReadFile(filesystem::path file_name)
            {
                std::ifstream ifs(file_name.str());
                ifs.seekg(0, std::ios::end);
                auto length = ifs.tellg();
                ifs.seekg(std::ios::beg);

                auto text = std::string();
                text.reserve(length);
                text.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
                return text;
            }

            std::vector<filesystem::path> paths_;
        };
    }
}

#endif
