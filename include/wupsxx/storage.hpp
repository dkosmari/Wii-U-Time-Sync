// SPDX-License-Identifier: MIT

#ifndef WUPSXX_STORAGE_HPP
#define WUPSXX_STORAGE_HPP

#include <expected>
#include <string>

#include <wups/storage.h>

#include "storage_error.hpp"


namespace wups::storage {

    template<typename T>
    std::expected<T, storage_error>
    load(const std::string& key)
    {
        T value;
        auto status = WUPSStorageAPI::GetEx(nullptr,
                                            key,
                                            value,
                                            WUPSStorageAPI::GetOptions::RESIZE_EXISTING_BUFFER);
        if (status != WUPS_STORAGE_ERROR_SUCCESS)
            return std::unexpected{storage_error{"error loading key \"" + key + "\"",
                                                 status}};
        return value;
    }


    template<typename T>
    void
    store(const std::string& key, const T& value)
    {
        auto status = WUPSStorageAPI::StoreEx(nullptr, key, value);
        if (status != WUPS_STORAGE_ERROR_SUCCESS)
            throw storage_error{"error storing key \"" + key + "\"",
                                status};
    }


    void save();

    void reload();

} // namespace wups::storage

#endif
