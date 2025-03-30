#pragma once

#include <algorithm>
#include <list>

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

#include <giomm/inputstream.h>
#include <glibmm/refptr.h>

class AssimpLikeGStream : public Assimp::IOStream
{
public:
    AssimpLikeGStream(const Glib::RefPtr<const Glib::Bytes> &bytes, std::string path)
      : bytes(bytes)
      , path(std::move(path))
    {}

private:
    size_t Read(void *pvBuffer, size_t pSize, size_t pCount) override {
        if (0 == pSize || 0 == pCount) {
            return 0;
        }

        auto nLeft       = (bytes->get_size() - curPos) / pSize;
        auto countToRead = std::min(nLeft, pCount);
        if (0 != countToRead) {
            auto bytesToRead   = countToRead * pSize;
            auto bytesReturned = bytes->get_size();
            memcpy(pvBuffer, static_cast<const uint8_t *>(bytes->get_data(bytesReturned)) + curPos, bytesToRead);
            curPos += bytesToRead;
        }

        return countToRead;
    }

    size_t Write(const void *pvBuffer, size_t pSize, size_t pCount) override {
        return {};
    }

    aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override {
        switch (pOrigin) {
            case aiOrigin_SET: {
                if (bytes->get_size() > pOffset) {
                    curPos = pOffset;
                    return aiReturn_SUCCESS;
                }
                break;
            }
            case aiOrigin_CUR: {
                if (bytes->get_size() > curPos + pOffset) {
                    curPos += pOffset;
                    return aiReturn_SUCCESS;
                }
                break;
            }
            case aiOrigin_END: {
                if (bytes->get_size() > pOffset) {
                    curPos = bytes->get_size() - (pOffset + 1);
                    return aiReturn_SUCCESS;
                }
                break;
            }
            default:
                break;
        }
        return aiReturn_FAILURE;
    }

    size_t Tell() const override {
        return curPos;
    }

    size_t FileSize() const override {
        return bytes->get_size();
    }

    void Flush() override {}

    Glib::RefPtr<const Glib::Bytes> bytes;
    size_t                          curPos{};
    std::string                     path;
};

class GResourceIO : public Assimp::IOSystem
{
    bool Exists(const char *pFile) const override {
        return Gio::Resource::get_file_exists_global_nothrow(pFile);
    }

    char getOsSeparator() const override {
        return '/';
    }

    AssimpLikeGStream * Open(const char *pFile, const char *pMode = "rb") override {
        openFiles.push_back(AssimpLikeGStream{Gio::Resource::lookup_data_global(pFile), pFile});

        return &openFiles.back();
    }

    void Close(Assimp::IOStream *pFile) override {
        auto aiStrm = dynamic_cast<AssimpLikeGStream *>(pFile);
        if (nullptr != aiStrm) {
            auto it = std::find_if(openFiles.begin(), openFiles.end(), [&](auto &&f) {
                return &f == aiStrm;
            });
            if (openFiles.end() != it) {
                openFiles.erase(it);
            }
        }
    }

    std::list<AssimpLikeGStream> openFiles;
};
