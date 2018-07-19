#include "Board.hpp"
#include "Logger.hpp"
#include "Voice.hpp"

Voice::Voice(
   const String& id,
   const int& pin) :
      Component(id),
      pin(pin)
{
   // Nothing to do here.
}

Voice::Voice(
   MessagePtr message) :
       Component(message)
{
   pin = message->isSet("pin") ? message->getInt("pin") : 0;
}

Voice::~Voice()
{
   // Nothing to do here.
}

void Voice::play(
   const int& duration)
{
   Logger::logDebug("Voice::play: ");
   
   Board::getBoard()->digitalWrite(pin, HIGH);
   
   Timer* timer = Timer::newTimer("voiceTimer", duration, Timer::ONE_SHOT, this);
   timer->start();
}

void Voice::stop()
{
   Logger::logDebug("Voice::stop: ");

   Board::getBoard()->digitalWrite(pin, LOW);
}

void Voice::setup()
{
   Board::getBoard()->pinMode(pin, OUTPUT);
}

void Voice::handleMessage(
   MessagePtr message)
{
   Component::handleMessage(message);
}

void Voice::timeout(
   Timer* timer)
{
   stop();
}

