// SPDX-License-Identifier: MIT

#ifndef WUPSXX_BOOL_ITEM_HPP
#define WUPSXX_BOOL_ITEM_HPP

#include <memory>
#include <string>

#include "base_item.hpp"


namespace wups::config {

    struct bool_item : base_item {

        bool& variable;
        bool default_value;
        std::string true_str = "true";
        std::string false_str = "false";


        bool_item(const std::optional<std::string>& key,
                  const std::string& name,
                  bool& variable);


        static
        std::unique_ptr<bool_item>
        create(const std::optional<std::string>& key,
               const std::string& name,
               bool& variable);


        virtual int get_display(char* buf, std::size_t size) const override;

        virtual int get_selected_display(char* buf, std::size_t size) const override;

        virtual void restore() override;

        virtual void on_input(WUPSConfigSimplePadData input) override;


        void on_changed();
    };

} // namespace wups::config

#endif