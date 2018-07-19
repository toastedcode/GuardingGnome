#include "Board.hpp"
#include "Pattern.hpp"

Pattern::Pattern(
   const String& id,
   const String& pattern,
   const int& updatePeriod) :
      Component(id),
      pattern(pattern),
      updatePeriod(updatePeriod),
      patternIndex(0),
      isActive(false),
      updateTime(0)
{
   // Nothing to do here.
}

Pattern::~Pattern()
{
   // Nothing to do here.
}

void Pattern::setPattern(
   const String& pattern)
{
   this->pattern = pattern;
   patternIndex = 0;
}

void Pattern::setUpdatePeriod(
   const int& updatePeriod)
{
   this->updatePeriod = updatePeriod;
}

void Pattern::start()
{
   isActive = true;

   patternIndex = 0;

   updateTime = Board::getBoard()->systemTime() + updatePeriod;

   if (pattern.length() > 0)
   {
      onUpdate(pattern.charAt(patternIndex));
   }
}

void Pattern::stop()
{
   isActive = false;
}

void Pattern::loop()
{
   if (isActive && (pattern.length() > 0))
   {
      long systemTime = Board::getBoard()->systemTime();

      if (systemTime >= updateTime)
      {
         onUpdate(getNext());

         updateTime = Board::getBoard()->systemTime() + updatePeriod;
      }
   }
}

char Pattern::getNext()
{
   patternIndex++;

   if (patternIndex >= pattern.length())
   {
      patternIndex = 0;
   }

   return (pattern.charAt(patternIndex));;
}
