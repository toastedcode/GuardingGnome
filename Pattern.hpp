#pragma once

#include "Component.hpp"

class Pattern : public Component
{

public:

   Pattern(
      const String& id,
      const String& pattern,
      const int& updatePeriod);

   virtual ~Pattern();

   void setPattern(
      const String& pattern);

   void setUpdatePeriod(
    const int& updatePeriod);

   void start();

   void stop();

   virtual void onUpdate(
         const char& token) = 0;

   virtual void loop();

protected:

   char getNext();

private:

   String pattern;

   int updatePeriod;

   int patternIndex;

   bool isActive;

   long updateTime;
};
