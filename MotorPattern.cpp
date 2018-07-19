#include "Logger.hpp"
#include "MotorPattern.hpp"

MotorPattern::MotorPattern(
   const String& id,
   Motor* motor,
   const String& pattern,
   const int& updatePeriod) :
      Pattern(id, pattern, updatePeriod),
      motor(motor)
{
   // Nothing to do here.
}

MotorPattern::~MotorPattern()
{
   // Nothing to do here.
}

void MotorPattern::stop()
{
   Pattern::stop();

   motor->setSpeed(Motor::NO_SPEED);
}

void MotorPattern::onUpdate(
   const char& token)
{
   switch (token)
   {
      case 'R':
      {
         motor->setSpeed(Motor::MAX_SPEED);
         break;
      }

      case 'r':
      {
         motor->setSpeed(Motor::MAX_SPEED * 0.50);
         break;
      }

      case 'L':
      {
         motor->setSpeed(Motor::MAX_SPEED * -1);
         break;
      }

      case 'l':
      {
         motor->setSpeed(Motor::MAX_SPEED * -0.50);
         break;
      }

      case ' ':
      default:
      {
         motor->setSpeed(Motor::NO_SPEED);
         break;
      }
   }
}
