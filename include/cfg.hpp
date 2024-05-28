// SPDX-License-Identifier: MIT

#ifndef CFG_HPP
#define CFG_HPP

#include <chrono>
#include <string>


namespace cfg {

    namespace key {
        extern const char* auto_tz;
        extern const char* msg_duration;
        extern const char* notify;
        extern const char* server;
        extern const char* sync;
        extern const char* threads;
        extern const char* tolerance;
        extern const char* utc_offset;
    }


    namespace label {
        extern const char* auto_tz;
        extern const char* msg_duration;
        extern const char* notify;
        extern const char* server;
        extern const char* sync;
        extern const char* threads;
        extern const char* tolerance;
        extern const char* utc_offset;
    }


    extern bool                 auto_tz;
    extern std::chrono::minutes utc_offset;
    extern int                  msg_duration;
    extern bool                 notify;
    extern std::string          server;
    extern bool                 sync;
    extern int                  threads;
    extern int                  tolerance;


    void load();
    void reload();
    void save();
    void migrate_old_config();

    std::chrono::minutes get_utc_offset();
    void set_utc_offset(std::chrono::minutes tz_offset);

} // namespace cfg

#endif
