#pragma once

#include <array>
#include "log.h"

namespace SimX {
class Accelerometer : public Loggable<Accelerometer> {
   public:
    Accelerometer() = default;
    ~Accelerometer() = default;

    std::array<double, 3> get_acc(void) { return acc_; }

    void show(void) {
        info("Accelerometer: {} {} {}", acc_[0], acc_[1], acc_[2]);
    }

   private:
    std::array<double, 3> acc_ = {0.0, 0.0, 0.0};  // 初始化默认值
};
}  // namespace SimX
