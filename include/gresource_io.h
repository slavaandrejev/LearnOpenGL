#pragma once

#include <algorithm>
#include <list>

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

#include <glib/glib.hpp>
#include <gnamespaces.h>

class AssimpLikeGStream : public Assimp::IOStream
{
public:
    AssimpLikeGStream(GLib::Bytes bytes, std::string path)
      : bytes(bytes)
      , path(std::move(path))
    {}

private:
    size_t Read(void *pvBuffer, size_t pSize, size_t pCount) override {
        if (0 == pSize || 0 == pCount) {
            return 0;
        }

        auto nLeft       = (bytes.get_size() - curPos) / pSize;
        auto countToRead = std::min(nLeft, pCount);
        if (0 != countToRead) {
            auto bytesToRead   = countToRead * pSize;
            memcpy(pvBuffer, static_cast<const uint8_t *>(bytes.get_data().data()) + curPos, bytesToRead);
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
                if (bytes.get_size() > pOffset) {
                    curPos = pOffset;
                    return aiReturn_SUCCESS;
                }
                break;
            }
            case aiOrigin_CUR: {
                if (bytes.get_size() > curPos + pOffset) {
                    curPos += pOffset;
                    return aiReturn_SUCCESS;
                }
                break;
            }
            case aiOrigin_END: {
                if (bytes.get_size() > pOffset) {
                    curPos = bytes.get_size() - (pOffset + 1);
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
        return const_cast<GLib::Bytes *>(&bytes)->get_size();
    }

    void Flush() override {}

    GLib::Bytes bytes;
    size_t      curPos{};
    std::string path;
};

class GResourceIO : public Assimp::IOSystem
{
    bool Exists(const char *pFile) const override {
        return Gio::resources_get_info(pFile, Gio::ResourceLookupFlags::NONE_, nullptr, nullptr);
    }

    char getOsSeparator() const override {
        return '/';
    }

    AssimpLikeGStream * Open(const char *pFile, const char *pMode = "rb") override {
        auto stream = std::make_unique<AssimpLikeGStream>(Gio::resources_lookup_data(pFile, Gio::ResourceLookupFlags::NONE_), pFile);

        return stream.release();
    }

    void Close(Assimp::IOStream *pFile) override {
    }
};
