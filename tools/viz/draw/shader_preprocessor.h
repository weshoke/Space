#ifndef SPACE_VIZ_DRAW_SHADER_PREPROCESS_H
#define SPACE_VIZ_DRAW_SHADER_PREPROCESS_H

#include <algorithm>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

namespace viz {
    namespace draw {
        // TODO: make string manipulation more efficient
        class ShaderSourceMap {
           public:
            ShaderSourceMap(const std::string &file_name)
            : sources_{file_name}
            {
            }

            void Append(const std::string &line) { lines_.push_back(line); }
            void Append(const ShaderSourceMap &source_map)
            {
                auto source_offset = sources_.size();
                sources_.insert(
                    sources_.end(), source_map.sources_.begin(), source_map.sources_.end());
                lines_.push_back("#line 1 " + std::to_string(source_offset) + "\n");
                for (const auto &line : source_map.lines_) {
                    if (line.find("#line") == 0) {
                        lines_.push_back(OffsetLineDirective(line, source_offset));
                    }
                    else {
                        lines_.push_back(line);
                    }
                }
            }

            std::string Error(const std::string &err_log)
            {
                const auto source_file_name = [&](auto &ss) {
                    if (ss.peek() == ':') {
                        return sources_.front();
                    }
                    else {
                        uint32_t source_idx;
                        ss >> source_idx;
                        return sources_.size() > source_idx ? sources_[source_idx] : "<unknown>";
                    }
                };

                auto ss = std::stringstream(err_log);
                ss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
                auto source_pos = ss.tellg();
                return err_log.substr(0, source_pos) + source_file_name(ss) +
                       err_log.substr(ss.tellg());
            }

            const std::vector<std::string> &lines() const { return lines_; }
           private:
            std::string OffsetLineDirective(const std::string &line, uint32_t source_offset)
            {
                auto ss = std::stringstream(line);
                ss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
                uint32_t line_idx, source_idx;
                ss >> line_idx >> source_idx;
                return "#line " + std::to_string(line_idx) + " " +
                       std::to_string(source_idx + source_offset) + "\n";
            }

            std::vector<std::string> sources_;
            std::vector<std::string> lines_;
        };

        template <class F>
        ShaderSourceMap Preprocess(const std::string &file_name, F &&load_file)
        {
            auto source = ShaderSourceMap(file_name);

            auto text = load_file(file_name);
            std::istringstream ss(text);
            auto line_text = std::string();
            auto line = 1u;
            while (!ss.eof() && !ss.bad()) {
                std::getline(ss, line_text);
                if (line_text.find("#include") == 0) {
                    // Fold into a trim-to function
                    auto sidx = 8;
                    auto eidx = line_text.size() - 1;
                    while (sidx < eidx && line_text[sidx] != '"') {
                        ++sidx;
                    }
                    while (eidx > sidx && line_text[eidx] != '"') {
                        --eidx;
                    }

                    if (sidx >= eidx) {
                        throw std::runtime_error("Malformed include statement");
                    }

                    auto file_name = line_text.substr(sidx + 1, eidx - sidx - 1);
                    source.Append(Preprocess(file_name, load_file));
                    // Offset line since we're replacing the #include with #line and
                    // the line number represents the next line in the source file
                    source.Append("#line " + std::to_string(line + 1) + " 0\n");
                }
                else {
                    source.Append(line_text + "\n");
                }
                ++line;
            }
            return source;
        }
    }
}

#endif
