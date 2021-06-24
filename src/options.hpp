#ifndef GBEMU_OPTIONS_HPP
#define GBEMU_OPTIONS_HPP

/// Command line options.
struct Options {
    bool trace                 = false;
    bool disable_logs          = false;
    bool headless              = false;
    bool show_full_framebuffer = false;
    bool exit_on_infinite_jr   = false;
    bool print_serial          = false;
};

#endif  // GBEMU_OPTIONS_HPP
