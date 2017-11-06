#ifndef INCLUDE_ASQ_ASQ_NEOPIXEL_H_
#define INCLUDE_ASQ_ASQ_NEOPIXEL_H_

#include <asq.h>
#include <Adafruit_NeoPixel.h>

namespace asq {

using uint32_array_t = array_t<uint32_t>;

class NeoPixelBounceSequencer : public Sequencer {
  public:
    NeoPixelBounceSequencer(Adafruit_NeoPixel& strip,
                            const uint8_array_t& mask,
                            const uint32_array_t& color_sequence)
      : strip_(strip), mask_(mask), color_sequence_(color_sequence),
        dir_(1), position_(0), current_color_(0) {
          
    }

    bool IsRunning() const override { return true; }
    void Reset() override {}
    void NextStep() override {
      strip_.clear();
      strip_.setPixelColor(position_, color_sequence_[current_color_]);
      strip_.show();
      if (dir_ > 0) {
        if (++position_ >= strip_.numPixels()) {
          --position_;
          dir_ = -1;
          if (++current_color_ >= color_sequence_.size()) {
            current_color_ = 0;
          }
        }
      } else {
        if (position_ > 0) {
          --position_;
        } else {
          position_ = 0;
          dir_ = 1;
          if (++current_color_ >= color_sequence_.size()) {
            current_color_ = 0;
          }
        }
      }
    }

  private:
    Adafruit_NeoPixel& strip_;
    const uint8_array_t& mask_;
    const uint32_array_t& color_sequence_;
    int8_t dir_;
    size_t position_;
    size_t current_color_;
};

}  // namespace asq


#endif  // INCLUDE_ASQ_ASQ_NEOPIXEL_H_