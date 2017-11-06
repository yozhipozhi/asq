#ifndef INCLUDE_ASQ_ASQ_ACTION_H_
#define INCLUDE_ASQ_ASQ_ACTION_H_

#include <asq.h>

namespace asq {

template <size_t ACTION_SIZE = 1>
class ActionSequencer : public Sequencer {
  public:
    struct Action {
      uint8_t step_count;
      uint8_t action[ACTION_SIZE];
    };

    ActionSequencer(const array_t<Action>& sequence,
                    const size_t start_index = 0,
                    const bool should_loop = true)
      : sequence_(sequence),
        should_loop_(should_loop),
        current_index_(start_index),
        current_step_(0) {}

    bool IsRunning() const override {
      return current_index_ < sequence_.size();
    }

    void Reset() override {
      current_index_ = 0;
      current_step_ = 0;
    }

    void NextStep() override {
      if (current_index_ >= sequence_.size()) {
        if (!should_loop_) {
          return;
        } else {
          Reset();
        }
      }
      ExecuteAction(sequence_[current_index_].action);
      if (++current_step_ >= sequence_[current_index_].step_count) {
        ++current_index_;
        current_step_ = 0;
      }
    }

    virtual void ExecuteAction(const uint8_t action[ACTION_SIZE]) = 0;

  protected:
    const array_t<Action>& sequence_;
    const bool should_loop_;

    uint8_t current_index_;
    uint8_t current_step_;
};


class AnalogOutputSequencer : public ActionSequencer<1> {
  public:
    AnalogOutputSequencer(const uint8_array_t& output_pins,
                          const array_t<Action>& sequence,
                          const size_t start_index = 0,
                          const bool should_loop = true)
      : ActionSequencer(sequence, start_index, should_loop),
        output_pins_(output_pins) {
    }

    void ExecuteAction(const uint8_t action[1]) override {
      for (size_t i = 0; i < output_pins_.size(); ++i) {
        const int pin = int(output_pins_[i]);
        analogWrite(pin, action[0]);
      }
    }

  private:
    const uint8_array_t& output_pins_;
};

}  // namespace asq

#endif  INCLUDE_ASQ_ASQ_ACTION_H_
