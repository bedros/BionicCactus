#include <SoilRunLoop.hpp>

SoilRunLoop::SoilRunLoop(Pump* pump, SoilSensor* sensor, Clock& clock) : _pump(pump), _sensor(sensor), _clock(clock), _state(Dispersing) {
  _disperseStarted = _clock.getMillis();
}

void SoilRunLoop::loop() {
  switch (_state) {
    case CheckMoisture:
      checkMoisture();
      break;
    case Pumping:
      pumping();
      break;
    case Dispersing:
      dispersing();
      break;
    case Drying:
      drying();
      break;
  }
}

void SoilRunLoop::checkMoisture() {
  unsigned long now = _clock.getMillis();
  if ((now - _moistureStarted) >=_timeAtMoisture) {
    _dryStarted = now;
    _state = Drying;
    return;
  }
  int current = _sensor->getPercent();
  int diff = _setPoint - current;
  if (diff > _tolerance) {
    int vol = _mlPerPercent * diff;
    if (vol > _maxDispense) {
      vol = _maxDispense;
    }
    Serial.println(vol);
    _pump->setVolume(vol);
    _state = Pumping;
  }
}

void SoilRunLoop::pumping() {
  _pump->loop();
  if (_pump->dispenseDone()) {
    _disperseStarted = _clock.getMillis();
    _state = Dispersing;
    Serial.println("pump done");
  }
}

void SoilRunLoop::dispersing() {
  unsigned long now = _clock.getMillis();
  if ((now - _disperseStarted) >= _disperseTime) {
    _state = CheckMoisture;
  }
}

void SoilRunLoop::drying() {
  unsigned long now = _clock.getMillis();
  if ((now - _dryStarted) >= _timeAtDry) {
    _moistureStarted = now;
    _state = CheckMoisture;
  }
}