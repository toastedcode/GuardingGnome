#pragma once

#include "Component\Motor.hpp"
#include "Pattern.hpp"

class MotorPattern : public Pattern
{

public:

   MotorPattern(
      const String& id,
      Motor* motor,
      const String& pattern,
      const int& updatePeriod);

   virtual ~MotorPattern();

   virtual void onUpdate(
         const char& token);

   virtual void stop();

private:

   Motor* motor;
};
