#ifndef INCLUDE_ASQ_H_
#define INCLUDE_ASQ_H_

namespace asq {

using uint8 = unsigned char;

class uint8_array {
  public:
    typedef unsigned char byte;

    uint8_array(const uint8 data) : size_(1) {
      data_ = new uint8[1];
      data_[0] = data;
    }

    uint8_array(const size_t size, const uint8 data[]) {
      size_ = size;
      data_ = new uint8[size_];
      memcpy(data_, data, size_);
    }

    uint8_array(const uint8_array& other) {
      size_ = other.size_;
      data_ = new uint8[size_];
      memcpy(data_, other.data_, size_);      
    }

    ~uint8_array() { delete data_; }

    size_t size() const { return size_; }

    const uint8& operator[](const size_t index) const { return data_[index]; }
    
  private:
    size_t size_;
    uint8* data_;
};


class Sequencer {
  public:
    typedef uint8 index_t;
    typedef uint8 step_t;

    struct Entry {
      step_t step_count;
      uint8_array action;
    };

    static const index_t kIndexStopped = 255;
    static const uint8 kActionDelay = 255;

    Sequencer(index_t sequence_size, const Entry* sequence, bool should_loop)
      : sequence_size_(sequence_size),
        sequence_(sequence),
        should_loop_(should_loop),
        current_index_(kIndexStopped),
        current_step_(0) {}

    bool IsRunning() const {
      return current_index_ != kIndexStopped;
    }

    void Start() {
      current_index_ = 0;
    }

    void NextStep() {
      if (!IsRunning()) {
        return;
      }
      if (current_index_ >= sequence_size_) {
        if (!should_loop_) {
          current_index_ = kIndexStopped;
          return;
        } else {
          current_index_ = 0;
          current_step_ = 0;
        }
      }
      ExecuteAction(sequence_[current_index_].action);
      if (++current_step_ >= sequence_[current_index_].step_count) {
        ++current_index_;
        current_step_ = 0;
      }
    }

    virtual void ExecuteAction(const uint8_array& action) {}

  protected:
    const index_t sequence_size_;
    const Entry* sequence_;
    const bool should_loop_;

    index_t current_index_;
    step_t current_step_;
};


class AnalogOutputSequencer : public Sequencer {
  public:
    AnalogOutputSequencer(const uint8_array& output_pins, index_t sequence_size, const Entry* sequence, bool should_loop)
      : Sequencer(sequence_size, sequence, should_loop),
        output_pins_(output_pins) {
    }

    virtual void ExecuteAction(const uint8_array& action) {
      for (unsigned i = 0; i < output_pins_.size(); ++i) {
        const int pin = int(output_pins_[i]);
        Serial.println(action[0]);
        analogWrite(pin, action[0] / 2);
      }
    }

  private:
    const uint8_array output_pins_;
};

class NeoPixelSequencer : public Sequencer {

};


}  // namespace asq

#endif INCLUDE_ASQ_H_
