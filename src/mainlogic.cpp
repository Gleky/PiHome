#include "mainlogic.h"

#include "isensor.h"
#include "idevice.h"


MainLogic::MainLogic(Sensor *temperatureSensor, Sensor *humiditySensor, Device *light, Alarm *alarm)
    : _temperatureSensor(*temperatureSensor),
      _humiditySensor(*humiditySensor),
      _light(*light),
      _alarm(*alarm)
{
    auto modelUpdated = [this] () {this->_view->modelUpdated();};
    _alarm.setOnUpdated(modelUpdated);
    _updateTimer.setInterval(600000);
    _updateTimer.start();
    QObject::connect( &_updateTimer, &QTimer::timeout, modelUpdated );
}

State MainLogic::currentState()
{
    _updateTimer.start();
    State result;

    result.temperatureConnected = _temperatureSensor.isConnected();
    result.temperature = _temperatureSensor.value();

    result.humidityConnected = _humiditySensor.isConnected();
    result.humidity = _humiditySensor.value();

    result.lightConnected = _light.isConnected();
    result.lightIsOn = _light.isOn();

    result.alarmConnected = _alarm.isConnected();
    result.alarmTime = _alarm.time();

    return result;
}

void MainLogic::tempOrHumidPressed()
{
    _view->modelUpdated();
}

void MainLogic::toggleLight()
{
    _light.toggle();
}

void MainLogic::setAlarmTime(Time newTime)
{
    _alarm.setTime(newTime);
}

void MainLogic::stopAlarm()
{
    _alarm.stop();
}
