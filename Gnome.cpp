#include "Board.h"
#include "Logger.h"
#include "Pattern.hpp"
#include "Properties.h"
#include "Gnome.hpp"
#include "ToastBot.h"
//#include "TwilioProtocol.hpp"

Gnome::Gnome(
   const String& id,
   Motor* motor,
   Sensor* sensor,
   Voice* voice) :
      Component(id),
      motor(motor),
      sensor(sensor),
      voice(voice),
      state(WATCHING),
      triggeredPeriod(DEFAULT_TRIGGERED_PERIOD),
      restingPeriod(DEFAULT_RESTING_PERIOD),
      watchPattern(0),
      triggeredPattern(0)
{
   // Nothing to do here.
}

Gnome::~Gnome()
{
   // Nothing to do here.
}

void Gnome::setup()
{
   Messaging::subscribe(this, "sensorChange");
  
   setState(WATCHING);
}

void Gnome::handleMessage(
   MessagePtr message)
{
   Logger::logDebug("Gnome::handleMessage: %s", message->getMessageId().c_str());

   //  ping
   if (message->getMessageId() == "ping")
   {
      Message* reply = Messaging::newMessage();
      
      if (reply)
      {
         reply->setMessageId("pong");
         reply->setSource(getId());
         reply->setDestination(message->getSource());
         reply->set("deviceId", ToastBot::getId());

         if (WifiBoard::getBoard())
         {
           unsigned char mac[6] = {0, 0, 0, 0, 0, 0};
           WifiBoard::getBoard()->getMacAddress(mac);
           char macAddress[18];
           sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

           reply->set("macAddress", macAddress);
           reply->set("ipAddress", WifiBoard::getBoard()->getIpAddress());
         }

         Messaging::send(reply);

         Messaging::freeMessage(message);
      }
   }
   else if (message->getMessageId() == "reset")
   {
      Logger::logDebug("Resetting ...");
      Board::getBoard()->reset();

      Messaging::freeMessage(message);
   }
   else if (message->getMessageId() == "properties")
   {
      Properties& properties = ToastBot::getProperties();
    
      Logger::logDebug("Gnome::handleMessage: Properties:");
      properties.log();
   }
   else if (message->getMessageId() == "setLogLevel")
   {
      LogLevel logLevel = fromString(message->getString("logLevel"));  // defaults to SEVERE

      //Logger::logDebug("Gnome::handleMessage: setLogLevel(%s)", toString(logLevel).c_str());

      Logger::setLogLevel(logLevel);

      Messaging::freeMessage(message);
   }   
   else if (message->getMessageId() == "sensorReading")
   {
      int value = message->getInt("value");
      
      Logger::logDebug("Gnome::handleMessage: sensor = %d", value);

      onSensorUpdate(value);

      Messaging::freeMessage(message);
   }
   else if (message->getMessageId() == "pattern")
   {
      String pattern = message->getString("pattern");
      int updatePeriod = message->getInt("updatePeriod");
      bool start = message->getBool("start");
      bool stop = message->getBool("stop");

      Pattern* motorPattern = static_cast<Pattern*>(ToastBot::getComponent("motorPattern"));

      if (pattern.length() > 0)
      {
         motorPattern->setPattern(pattern);
      }

      if (updatePeriod > 0)
      {
         motorPattern->setUpdatePeriod(updatePeriod);
      }      

      if (start == true)
      {
         motorPattern->start();
      }
      else if (stop == true)
      {
         motorPattern->stop();
      }

      Messaging::freeMessage(message);
   }
   else
   {
      Component::handleMessage(message);
   }
}

void Gnome::timeout(
   Timer* timer)
{
   if (timer->getId() == "watchTimer")
   {
      onWatchingTimeout();
   }
   else if (timer->getId() == "triggeredTimer")
   {
      onTriggeredTimeout();
   }
   else if (timer->getId() == "restingTimer")
   {
      onRestingTimeout();
   }
}

void Gnome::setWatchPattern(
   const String& pattern,
   const int& updatePeriod)
 {
    if (watchPattern)
    {
       ToastBot::removeComponent(watchPattern);
       delete (watchPattern);
    }

    watchPattern = new MotorPattern("watchPattern", motor, pattern, updatePeriod);
    ToastBot::addComponent(watchPattern);
 }

void Gnome::setTriggeredPattern(
   const String& pattern,
   const int& updatePeriod)
{
   if (triggeredPattern)
   {
      ToastBot::removeComponent(triggeredPattern);
      delete (triggeredPattern);
   }

   triggeredPattern = new MotorPattern("triggeredPattern", motor, pattern, updatePeriod);
   ToastBot::addComponent(triggeredPattern);
}
 
void Gnome::setState(
  const State& state)
{
  this->state = state;

  Logger::logDebug("Gnome::setState: %s", toString(state).c_str());

   switch (state)
   {
      case TRIGGERED:
      {
         Timer* timer = Timer::newTimer("triggeredTimer", triggeredPeriod, Timer::ONE_SHOT, this);
         timer->start();

         if (triggeredPattern)
         {
            triggeredPattern->start();
         }

         if (voice)
         {
            voice->play(triggeredPeriod);
         }
         break;
      }

      case RESTING:
      { 
         Timer* timer = Timer::newTimer("restingTimer", restingPeriod, Timer::ONE_SHOT, this);
         timer->start();
   
         if (triggeredPattern)
         {
            triggeredPattern->stop();
         }         
      }

      case WATCHING:
      default:
      {
         break;
      }
   }
}   

void Gnome::onSensorUpdate(
   int value)
{
   if (state == WATCHING)
   {
      setState(TRIGGERED);
      reportDetection();
   }
}

void Gnome::onWatchingTimeout()
{
   if (state == WATCHING)
   {
      // TODO
   }
}

void Gnome::onTriggeredTimeout()
{
   if (state == TRIGGERED)
   {
      setState(RESTING);
   }
}

void Gnome::onRestingTimeout()
{
   if (state == RESTING)
   {
      if (sensor->read() == 1)
      {
         setState(TRIGGERED);
      }
      else
      {
         setState(WATCHING);
      }
   }
}


void Gnome::reportDetection()
{
   Properties& properties = ToastBot::getProperties();

   if (properties.isSet("server.url"))
   {
      String url = properties.getString("server.url");
      
      MessagePtr message = Messaging::newMessage();
      if (message)
      {
        message->setMessageId("detection");
        message->setSource(ToastBot::getId());
        message->setDestination("httpAdapter");
        message->set("url", url);
        message->set("action", "detection");
        message->set("gnomeId", getId());
        Messaging::send(message);
      }
   }
}

void Gnome::smsNotification()
{
   /*
   Properties& properties = ToastBot::getProperties();

   if (properties.getBool("sms.enabled") &&
       properties.isSet("sms.toNumber"))
   {
      MessagePtr message = Messaging::newMessage();
      if (message)
      {
        message->setMessageId("detection");
        message->setSource(ToastBot::getId());
        message->setDestination("smsAdapter");
        TwilioProtocol::setToNumber(message, properties.getString("sms.toNumber"));
        TwilioProtocol::setBody(message, "Garden Gnome says, \"Intrusion detected!!!\"");
      }
   }
   */
}

String Gnome::toString(
   const State& state)
{
   static const char* STRING_VALUES[] = 
   {
      "WATCHING",
      "TRIGGERED",
      "RESTING"
   };
   
   return (String(STRING_VALUES[state - FIRST]));
}

