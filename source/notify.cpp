// SPDX-License-Identifier: MIT

#include <atomic>

#include <notifications/notifications.h>

#include "notify.hpp"

#include "cfg.hpp"
#include "logging.hpp"


namespace notify {

    std::atomic_uint refs = 0;


    void
    initialize()
    {
        // don't initialize again if refs was already positive
        if (refs++)
            return;

        NotificationModule_InitLibrary();
    }


    void
    finalize()
    {
        if (!refs)
            return;

        // don't finalize if refs is still positive
        if (--refs)
            return;

        NotificationModule_DeInitLibrary();
    }


    void
    error(const std::string& arg)
    {
        logging::printf("ERROR: %s", arg.c_str());

        if (!cfg::notify)
            return;

        std::string msg = "[" PLUGIN_NAME "] " + arg;
        NotificationModule_AddErrorNotificationEx(msg.c_str(),
                                                  cfg::msg_duration,
                                                  1,
                                                  {255, 255, 255, 255},
                                                  {160, 32, 32, 255},
                                                  nullptr,
                                                  nullptr,
                                                  true);
    }


    void
    info(const std::string& arg)
    {
        logging::printf("INFO: %s", arg.c_str());

        if (!cfg::notify)
            return;

        std::string msg = "[" PLUGIN_NAME "] " + arg;
        NotificationModule_AddInfoNotificationEx(msg.c_str(),
                                                 cfg::msg_duration,
                                                 {255, 255, 255, 255},
                                                 {32, 32, 160, 255},
                                                 nullptr,
                                                 nullptr,
                                                 true);
    }


    void
    success(const std::string& arg)
    {
        logging::printf("SUCCESS: %s", arg.c_str());

        if (!cfg::notify)
            return;

        std::string msg = "[" PLUGIN_NAME "] " + arg;
        NotificationModule_AddInfoNotificationEx(msg.c_str(),
                                                 cfg::msg_duration,
                                                 {255, 255, 255, 255},
                                                 {32, 160, 32, 255},
                                                 nullptr,
                                                 nullptr,
                                                 true);
    }



    guard::guard(bool init) :
        must_finalize{init}
    {
        initialize();
    }


    guard::~guard()
    {
        if (must_finalize)
            finalize();
    }


    void
    guard::release()
    {
        must_finalize = false;
    }


}