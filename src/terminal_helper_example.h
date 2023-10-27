//
// Created by Kasper de Bruin on 27/10/2023.
//

#ifndef IMGUI_MAC_APP_TERMINAL_HELPER_EXAMPLE_H
#define IMGUI_MAC_APP_TERMINAL_HELPER_EXAMPLE_H

#include <array>
#include <string>
#include <vector>
#include <mutex>

#include "../vendor/i
#include "imterm/terminal_helpers.hpp"

using namespace ImTerm;

class terminal_helper_example : basic_terminal_helper<terminal_helper_example, void> {
public:
    static std::vector<std::string> no_completion(std::string_view) { return {}; }

    // clears the logging screen. argument_type is aliased in ImTerm::basic_terminal_helper
    static void clear(argument_type& arg) {
        arg.term.clear();
    }

    // prints the text passed as parameter to the logging screen. argument_type is aliased in ImTerm::basic_terminal_helper
    static void echo(argument_type& arg) {
        if (arg.command_line.size() < 2) {
            return;
        }
        std::string str = std::move(arg.command_line[1]);
        for (auto it = std::next(arg.command_line.begin(), 2) ; it != arg.command_line.end() ; ++it) {
            str += " " + std::move(*it);
        }
        message msg;
        msg.value = std::move(str);
        msg.color_beg = msg.color_end = 0; // color is disabled when color_beg == color_end
        // other parameters are ignored
        arg.term.add_message(std::move(msg));
    }

    terminal_helper_example() {
        add_command_({"clear", "clear the screen", clear, no_completion});
        add_command_({"echo", "echoes your text", echo, no_completion});
    }
};

#endif //IMGUI_MAC_APP_TERMINAL_HELPER_EXAMPLE_H
