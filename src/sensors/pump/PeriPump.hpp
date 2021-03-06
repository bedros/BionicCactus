#ifndef PERI_PUMP_H
#define PERI_PUMP_H

#include "Pump.hpp"
#include "time/MillisProvider.hpp"
#include "time/Timer.hpp"
#include <Arduino.h>

using namespace Time;

namespace Sensors{
  namespace Pump {

    class PeriPump : public Pump {
    public:
      PeriPump(MillisProvider& millisProvider, int pinOut1, int pinOut2, int pinPWM, int pinStandby);
      enum Directions { Forward, Backward };
      virtual void setVolume(int vol);
      virtual bool dispenseDone();
      virtual void loop();
      virtual void prime() {
        runPump(100, Forward);
      };
      virtual void empty() {
        runPump(100, Backward);
      }
      void setSpeed(int speed) {
        _speed = speed;
      };
      void setFlowRate(double flowRate);
      double getFlowRate();
      void runPump(int speed, Directions dir);
      void stop();
    private:
      MillisProvider& _millisProvider;
      Timer _timer;
      int _pinOut1;
      int _pinOut2;
      int _pinPWM;
      int _pinStandby;
      int _speed = 100;
      double _flowRate;
      unsigned long _startTime;
      unsigned long _stopTime;
      void enable();
    };

  }
}

#endif
