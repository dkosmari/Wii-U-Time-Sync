// SPDX-License-Identifier: MIT

#include <cstdio>
#include <stdexcept>

#include "wupsxx/item.hpp"

#include "wupsxx/config_error.hpp"


namespace wups::config {

    namespace dispatchers {

        int32_t
        get_display(void* ctx, char* buf, int32_t size)
        {
            auto i = static_cast<const item*>(ctx);
            return i->get_display(buf, size);
        }

        int32_t
        get_selected_display(void* ctx, char* buf, int32_t size)
        {
            auto i = static_cast<const item*>(ctx);
            return i->get_selected_display(buf, size);
        }

        void
        on_selected(void* ctx, bool is_selected)
        {
            auto i = static_cast<item*>(ctx);
            i->on_selected(is_selected);
        }

        void
        restore_default(void* ctx)
        {
            auto i = static_cast<item*>(ctx);
            i->restore();
        }

        bool
        is_movement_allowed(void* ctx)
        {
            auto i = static_cast<const item*>(ctx);
            return i->is_movement_allowed();
        }

        void
        on_close(void* ctx)
        {
            auto i = static_cast<item*>(ctx);
            i->on_close();
        }

        void
        on_input(void* ctx, WUPSConfigSimplePadData input)
        {
            auto i = static_cast<item*>(ctx);
            i->on_input(input);
        }

        void
        on_input_ex(void* ctx, WUPSConfigComplexPadData input)
        {
            auto i = static_cast<item*>(ctx);
            i->on_input(input);
        }

        void
        on_delete(void* ctx)
        {
            auto i = static_cast<item*>(ctx);
            i->release(); // don't destroy the handle, it's already happening
            delete i;
        }
    }


    item::item(const std::optional<std::string>& key,
               const std::string& name) :
        key{key},
        name{name}
    {
        WUPSConfigAPIItemOptionsV2 options {
            .displayName = name.c_str(),
            .context = this,
            .callbacks = {
                .getCurrentValueDisplay = dispatchers::get_display,
                .getCurrentValueSelectedDisplay = dispatchers::get_selected_display,
                .onSelected = dispatchers::on_selected,
                .restoreDefault = dispatchers::restore_default,
                .isMovementAllowed = dispatchers::is_movement_allowed,
                .onCloseCallback = dispatchers::on_close,
                .onInput = dispatchers::on_input,
                .onInputEx = dispatchers::on_input_ex,
                .onDelete = dispatchers::on_delete,
            }
        };

        auto status = WUPSConfigAPI_Item_Create(options, &handle);
        if (status != WUPSCONFIG_API_RESULT_SUCCESS)
            throw config_error{"could not create config item \"" + name + "\"", status};
    }


    item::~item()
    {
        if (handle.handle)
            WUPSConfigAPI_Item_Destroy(handle);
    }


    void
    item::release()
    {
        handle = {};
    }


    int
    item::get_display(char* buf,
                      std::size_t size)
        const
    {
        std::snprintf(buf, size, "NOT IMPLEMENTED");
        return 0;
    }


    int
    item::get_selected_display(char* buf,
                               std::size_t size)
        const
    {
        return get_display(buf, size);
    }


    void
    item::on_selected(bool)
    {}


    void
    item::restore()
    {}


    bool
    item::is_movement_allowed()
        const
    {
        return true;
    }


    void
    item::on_close()
    {}


    void
    item::on_input(WUPSConfigSimplePadData input)
    {
        if (input.buttons_d & WUPS_CONFIG_BUTTON_X)
            restore();
    }


    void
    item::on_input(WUPSConfigComplexPadData /*input*/)
    {}


} // namespace wups::config