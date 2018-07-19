// *****************************************************************************
// *****************************************************************************
// Voice.hpp
//
// Author: Jason Tost
// Date:   5.16.2018
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "Component.hpp"
#include "ComponentFactory.hpp"
#include "Timer.hpp"

class Voice : public Component, public TimerListener
{

public:

   // Constructor.
   Voice(
      // A unique identifier for this component.
      const String& id,
      // The pin for triggering the voice to play.
      const int& pin);

   // Constructor.
   Voice(
      // A message containing the parameters to use in creating the component.
      MessagePtr message);

   // Destructor.
   virtual ~Voice();

   void play(
      const int& duration);

   void stop();

   // This operation should be called on startup to prepare the sensor for polling/updating.
   virtual void setup();

   // This operation handles a message directed to this component.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   // This operation handles a timeout causing the voice module to stop playing.
   virtual void timeout(
      Timer* timer);

private:

   int pin;
};

REGISTER(Voice, voice)

