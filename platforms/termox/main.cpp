#include <chrono>
#include <fstream>
#include <mutex>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <termox/termox.hpp>

#include "../../src/gameboy.h"
#include "../../src/input.h"
#include "../../src/options.h"
#include "../../src/util/files.h"
#include "../cli/cli.h"

namespace {

constexpr auto gb_width  = 160;
constexpr auto gb_height = 144;

auto load_state(std::string const& save_name) -> std::vector<u8>
{
    auto const filename = save_name + ".sav";
    auto const ifs      = std::ifstream{filename};
    if (ifs.good()) {
        auto save_data = read_bytes(filename);
        return save_data;
    }
    else
        return {};
}

}  // namespace

namespace oxgb {

class Gameboy_widget
    : public ox::
          Color_graph_static_bounds<int, 0, gb_width - 1, gb_height - 1, 0> {
   private:
    static constexpr auto display_width  = gb_width;
    static constexpr auto display_height = gb_height / 2;

    using Base_t =
        Color_graph_static_bounds<int, 0, gb_width - 1, gb_height - 1, 0>;

   public:
    Gameboy_widget(std::vector<u8> rom_data,
                   Options& options,
                   std::vector<u8> save_data)
        : emulator_{std::move(rom_data), options, std::move(save_data)}
    {
        using namespace ox::pipe;

        ox::Terminal::set_palette(ox::gameboy::palette);
        *this | fixed_width(display_width) | fixed_height(display_height) |
            strong_focus() | on_resize([this](auto area, auto) {
                too_small_ =
                    area.width < display_width || area.height < display_height;
            });

        emulator_.register_draw_callback(
            [this](FrameBuffer const& buf) { next_buffer_ = buf; });

        loop_.run_async([this](auto& queue) {
            {
                auto const guard = std::lock_guard{button_mtx_};
                if (!last_keypress.has_value() && button_.has_value()) {
                    emulator_.button_pressed(*button_);
                    last_keypress = std::chrono::high_resolution_clock::now();
                }
                else if (last_keypress.has_value() && button_.has_value() &&
                         (std::chrono::high_resolution_clock::now() -
                          *last_keypress) >= std::chrono::milliseconds{25}) {
                    // no key release events in the terminal so use timeout.
                    // not perfect but it is as good as it'll get for now.
                    emulator_.button_released(*button_);
                    button_       = std::nullopt;
                    last_keypress = std::nullopt;
                }
            }
            emulator_.tick();  // This can assign to next_buffer_.
            if (next_buffer_.has_value()) {
                queue.append(
                    ox::Custom_event{[this, buf = std::move(*next_buffer_)] {
                        this->handle_next_frame(std::move(buf));
                    }});
                next_buffer_ = std::nullopt;
            }
        });
    }

   protected:
    auto paint_event(ox::Painter& p) -> bool override
    {
        if (too_small_) {
            p.put(U"Display is too small.", {0, 0});
            p.put(U"Make the font size smaller", {0, 1});
            p.put(U"Or expand the terminal window.", {0, 2});
            return true;
        }
        else
            return Base_t::paint_event(p);
    }

    auto key_press_event(ox::Key k) -> bool override
    {
        auto b = std::optional<::GbButton>{std::nullopt};
        switch (k) {
            using ox::Key;
            case Key::Arrow_up: b = ::GbButton::Up; break;
            case Key::Arrow_down: b = ::GbButton::Down; break;
            case Key::Arrow_left: b = ::GbButton::Left; break;
            case Key::Arrow_right: b = ::GbButton::Right; break;
            case Key::z: b = ::GbButton::A; break;
            case Key::x: b = ::GbButton::B; break;
            case Key::Enter: b = ::GbButton::Start; break;
            case Key::Backspace: b = ::GbButton::Select; break;
            default: break;
        }
        {
            auto const guard = std::lock_guard{button_mtx_};
            button_          = b;
        }
        return Base_t::key_press_event(k);
    }

   private:
    bool too_small_ = true;
    Gameboy emulator_;
    ox::Event_loop loop_;
    std::optional<::FrameBuffer> next_buffer_ = std::nullopt;
    std::optional<::GbButton> button_         = std::nullopt;
    std::mutex button_mtx_;
    std::optional<std::chrono::high_resolution_clock::time_point> last_keypress;

   private:
    void handle_next_frame(FrameBuffer buf)
    {
        this->Base_t::reset(translate_to_pairs(buf));
    }

    [[nodiscard]] static auto translate_to_pairs(FrameBuffer const& buf)
        -> std::vector<std::pair<Base_t::Coordinate, ox::Color>>
    {
        auto result = std::vector<std::pair<Base_t::Coordinate, ox::Color>>{};
        for (auto x = 0; x < gb_width; ++x) {
            for (auto y = 0; y < gb_height; ++y) {
                result.push_back({{x, Base_t::boundary().north - y},
                                  to_color(buf.get_pixel(x, y))});
            }
        }
        return result;
    }

    [[nodiscard]] static auto to_color(::Color c) -> ox::Color
    {
        switch (c) {
            case ::Color::White: return ox::gameboy::Green_4;
            case ::Color::LightGray: return ox::gameboy::Green_3;
            case ::Color::DarkGray: return ox::gameboy::Green_2;
            case ::Color::Black: return ox::gameboy::Green_1;
        }
    }
};

}  // namespace oxgb

int main(int argc, char* argv[])
{
    auto cli_options = get_cli_options(argc, argv);
    auto rom_data    = read_bytes(cli_options.filename);
    auto save_data   = load_state(cli_options.filename);

    return ox::System{}.run<ox::Float_2d<oxgb::Gameboy_widget>>(
        std::move(rom_data), cli_options.options, std::move(save_data));
}
