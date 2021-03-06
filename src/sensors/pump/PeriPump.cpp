#include "sensors/pump/PeriPump.hpp"

using namespace Sensors::Pump;

PeriPump::PeriPump(MillisProvider& millisProvider, int pinOut1, int pinOut2, int pinPWM, int pinStandby) :
_millisProvider(millisProvider),
_timer(millisProvider, 0, Units::MILLISECONDS, false),
_pinOut1(pinOut1), 
_pinOut2(pinOut2), 
_pinPWM(pinPWM), 
_pinStandby(pinStandby),
_stopTime(0), 
_flowRate(1.2l) {
  pinMode(_pinOut1, OUTPUT);
  pinMode(_pinOut2, OUTPUT);
  pinMode(_pinStandby, OUTPUT);
  enable();
  stop();
}

double PeriPump::getFlowRate() {
  return _flowRate;
}

void PeriPump::setFlowRate(double flowRate) {
  _flowRate = flowRate;
}

void PeriPump::runPump(int speed, Directions dir) {
  if (dir == Forward)
  {
    digitalWrite(_pinOut1, HIGH);
    digitalWrite(_pinOut2, LOW);
  }
  else {
    digitalWrite(_pinOut1, LOW);
    digitalWrite(_pinOut2, HIGH);
  }
  int pwmVal = (speed * PWMRANGE) / 100;
  analogWrite(_pinPWM, pwmVal);
}

void PeriPump::stop() {
  analogWrite(_pinPWM, 0);
  digitalWrite(_pinOut1, HIGH);
  digitalWrite(_pinOut2, HIGH);
}

void PeriPump::setVolume(int vol) {
  unsigned long secs = (((vol / _flowRate) * _speed) / 100);
  _timer.reset(secs, Units::SECONDS, false);
}

void PeriPump::loop() {
  if (dispenseDone()) {
    stop();
  }
  else {
    runPump(_speed, Forward);
  }
}

bool PeriPump::dispenseDone() {
  return _timer.isExpired();
}

void PeriPump::enable() {
  digitalWrite(_pinStandby, HIGH);
}
