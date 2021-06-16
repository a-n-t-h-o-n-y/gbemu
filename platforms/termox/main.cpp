#include <termox/termox.hpp>

namespace oxgb {

class Gameboy_widget : public ox::Widget {
   private:
    static constexpr auto width  = 160;
    static constexpr auto height = 144;

   public:
    Gameboy_widget()
    {
        using namespace ox::pipe;
        *this | fixed_width(width) | fixed_height(height / 2) |
            bg(ox::Color::Blue);
    }
};

}  // namespace oxgb

int main() { return ox::System{}.run<ox::Float_2d<oxgb::Gameboy_widget>>(); }
