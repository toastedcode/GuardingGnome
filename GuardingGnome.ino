#include <Board.h>
//#include <SMS.h>
#include <ToastBot.h>

#include "MotorPattern.hpp"
#include "Pattern.hpp"
#include "PirSensor.hpp"
#include "PropertiesPage.hpp"
#include "Gnome.hpp"
#include "Voice.hpp"
#include "WebServer.hpp"

WebServer webServer(80);

// *****************************************************************************
//                                  Arduino
// *****************************************************************************

void setup()
{
   ToastBot::setup(new Esp8266Board());

   Properties& properties = ToastBot::getProperties();
   
   String triggeredPattern = properties.getString("triggeredPattern.pattern");
   int updatePeriod = properties.getInt("triggeredPattern.updatePeriod");

   Sensor* sensor = static_cast<PirSensor*>(ToastBot::getComponent("sensor1"));
   Motor* motor = static_cast<Motor*>(ToastBot::getComponent("motor1"));
   Voice* voice = static_cast<Voice*>(ToastBot::getComponent("voice1"));
   Gnome* gnome = new Gnome("gnome", motor, sensor, voice);
   gnome->setTriggeredPattern(triggeredPattern, updatePeriod);

   Adapter* httpAdapter = new HttpClientAdapter("httpAdapter", new RestfulProtocol());

   /*
   Adapter* smsAdapter = new SMSAdapter("smsAdapter",                                                         // id
                                        "api.twilio.com",                                                     // host
                                        443,                                                                  // port
                                        "REDACTED",                                 // account sid
                                        "REDACTED",                                   // auth token
                                        "REDACTED",  // api.twilio.com SHA1 fingerprint, this one was valid as of May 2018.
                                        "REDACTED");                                                      // from phone number
   */

   ToastBot::addComponent(gnome, true);  // <-- default handler
   ToastBot::addComponent(sensor);
   ToastBot::addComponent(httpAdapter);
   //ToastBot::addComponent(smsAdapter);
                                           
   webServer.setup();
   webServer.addPage(new PropertiesPage());
}

void loop()
{
   ToastBot::loop();

   webServer.loop();
}
