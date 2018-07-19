#pragma once

#include "Component.hpp"
#include "Component\Motor.hpp"
#include "Component\Sensor.hpp"
#include "Messaging.hpp"
#include "MotorPattern.hpp"
#include "Timer.hpp"
#include "Voice.hpp"

class Gnome : public Component, public TimerListener
{
  
public:

   enum State
   {
      FIRST,
      WATCHING = FIRST,
      TRIGGERED,
      RESTING,
      LAST,
      COUNT = LAST - FIRST
   };
  
   // Constructor.
   Gnome(
      const String& id,
      Motor* motor,
      Sensor* sensor,
      Voice* voice);

   // Destructor.
   virtual ~Gnome();

   virtual void setup();

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);
  
   virtual void timeout(
      Timer* timer);

   void setWatchPattern(
      const String& pattern,
      const int& updatePeriod);

   void setTriggeredPattern(
      const String& pattern,
      const int& updatePeriod);

protected:

   void setState(
     const State& state);

   void onSensorUpdate(
      int value);

   void onWatchingTimeout();

   void onTriggeredTimeout();

   void onRestingTimeout();

   void reportDetection();

   void smsNotification();

private:

   String toString(
      const State& state);

   static const int DEFAULT_TRIGGERED_PERIOD = 5000;  // millis

   static const int DEFAULT_RESTING_PERIOD = 5000;  // millis

   Motor* motor;

   Sensor* sensor;

   Voice* voice;

   MotorPattern* watchPattern;

   MotorPattern* triggeredPattern;

   State state;

   int triggeredPeriod;

   int restingPeriod;
};
