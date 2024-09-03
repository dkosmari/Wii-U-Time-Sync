/*
 * Time Sync - A NTP client plugin for the Wii U.
 *
 * Copyright (C) 2024  Daniel K. O.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TIMEZONE_OFFSET_ITEM_HPP
#define TIMEZONE_OFFSET_ITEM_HPP

#include <chrono>
#include <memory>

#include <wupsxx/var_item.hpp>


struct timezone_offset_item : wups::config::var_item<std::chrono::minutes> {

    timezone_offset_item(const std::string& label,
                         std::chrono::minutes& variable);

    static
    std::unique_ptr<timezone_offset_item>
    create(const std::string& label,
           std::chrono::minutes& variable);

    virtual void get_display(char* buf, std::size_t size) const override;

    virtual void get_focused_display(char* buf, std::size_t size) const override;

    virtual
    wups::config::focus_status
    on_input(const wups::config::simple_pad_data& input) override;


private:

    void on_changed();

};


#endif
