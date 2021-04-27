#include "utility/time.hpp"
#include "utility/log.hpp"
#include <cstdint>
#include "Bus.hpp"

Bus bus1;

void ISR() 
{
  LOG_INFO("Key detected");
  bus1.IOPoll();
}


int main()
{
   
  while (1)
  {
    LOG_INFO("No keypress detected...");
    sjsu::Delay(100ms);
    bus1.IOPoll();
  }

  return 0;
}





















/*
To write to memory:
        RD, WR should be high be default
        previous state unknown, but m/io must be set to high before rd/wr
operation write_enable previous state unknown

        set direction write_enable to high
        put address on a/d lines
        ale high, pause, ale low
                -address now latched
        m/io must be set to high for mem select
        put data on a/d lines
        set write control line to low
        pause
        set write control line to high
                -data has been written to memory

******************************************************************************************

To read from memory:
        RD, WR should be high be default
        previous state unknown, but m/io must be set to high before rd/wr
operation write_enable previous state unknown

    set direction write_enable to high
    put address on a/d lines
    ale high, pause, ale low
            -address now latched
    m/io must be set to high for mem select
        set direction write_enable to low
        set RD control to low
                data will now be on the a/d lines
        pause
        fetch data from a/d lines
        set RD to high
*/

/*
**************************************************************************************************************
**************************************************************************************************************
This comment contains various mains from various labs. some won't work anymore since functions or classes may
have been renamed or modified. 
**************************************************************************************************************
**************************************************************************************************************

//Lab ?

do  // do while loop to indicate the code unique to this lab demo.
{
uint8_t out = 0;
LOG_INFO("Joshua Pabst's Lab 7");

address = 0;
data    = 135;
bus.MemWrite(address, data);

address = 64;
data    = 95;
bus.MemWrite(address, data);

address = 0;
out     = bus.MemRead(address);
LOG_INFO("A: %d D: %d", address, out);

address = 64;
out     = bus.MemRead(address);
LOG_INFO("A: %d D: %d", address, out);

} while (0);

*******************************************************************************************************************

// this loop is for lab 9: IO with 82c55

//Lab 9

while (1)
{
// setup 82c55 here:
address = 3;
data    = 131;  // set mode
bus.IOWrite(address, data);
LOG_INFO("setting up 82C55...");

    data    = 110;
    address = 0;
bus.IOWrite(address, data);
    LOG_INFO("write to a...");
  
    sjsu::Delay(1000ms);

    address = 1;
data = bus.IORead(address);
LOG_INFO("in from b: %d", data);

    sjsu::Delay(1000ms);
}
**************************************************************************************************************


//Lab 11

while(1)
{
bus.IOPoll();
}

**************************************************************************************************************

//Lab 13

Bus bus1;


void ISR()
{
  LOG_INFO("Key detected");
  bus1.IOPoll();
}



int main()
{
  bus1.Initialize();

  sjsu::lpc40xx::Gpio interrupt = sjsu::lpc40xx::Gpio(0, 11);
  interrupt.GetPin().SetPull(sjsu::Pin::Resistor::kPullUp);
  interrupt.AttachInterrupt(ISR, sjsu::Gpio::Edge::kEdgeRising);
   
  while (1)
  {
    LOG_INFO("No keypress detected...");
    sjsu::Delay(1000ms);
    //bus1.IOPoll();
  }

  return 0;
}

**************************************************************************************************************
*/

