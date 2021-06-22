#ifndef GBEMU_CLI_HPP
#define GBEMU_CLI_HPP

#include <string>
#include <vector>

#include "definitions.h"
#include "options.h"

struct CliOptions {
    Options options;
    std::string filename;
};

inline auto get_cli_options(int argc, char* argv[]) -> CliOptions
{
    if (argc < 2) {
        fatal_error("Please provide a ROM file to run.");
    }

    CliOptions cliOptions;
    cliOptions.filename = argv[1];

    std::vector<std::string> flags(argv + 2, argv + argc);

    for (std::string& flag : flags) {
        if (flag == "--debug") {
            cliOptions.options.debugger = true;
        }
        else if (flag == "--trace") {
            cliOptions.options.trace = true;
        }
        else if (flag == "--silent") {
            cliOptions.options.disable_logs = true;
        }
        else if (flag == "--headless") {
            cliOptions.options.headless = true;
        }
        else if (flag == "--whole-framebuffer") {
            cliOptions.options.show_full_framebuffer = true;
        }
        else if (flag == "--exit-on-infinite-jr") {
            cliOptions.options.exit_on_infinite_jr = true;
        }
        else if (flag == "--print-serial") {
            cliOptions.options.print_serial = true;
        }
        else {
            fatal_error("Unknown flag: %s", flag.c_str());
        }
    }

    return cliOptions;
}

#endif  // GBEMU_CLI_HPP
