#include "utility/time.hpp"
#include "utility/log.hpp"
#include <cstdint>
#include "L1_Peripheral/lpc40xx/gpio.hpp"

class Bus
{
 public:
  void Initialize()
  {
    // Setting each pin as Open Drain
    for (int i = 0; i < 8; i+=1)
    {
      ad[i].GetPin().SetAsOpenDrain();
	}


    //
    write_enable.SetAsOutput();  // controls direction of transciever
    write_enable.SetHigh();      // set to a safe default value
    ale.SetAsOutput();           // ALE: address latch enable
    ale.SetLow();                // set to a safe default value
    m_io.SetAsOutput();          // M/IO
    m_io.SetHigh();              // set to a safe default value
    write.SetAsOutput();         // write control for SRAM
    write.SetHigh();             // set to a safe default value
    read.SetAsOutput();          // read control for SRAM
    read.SetHigh();              // set to a safe default value
    interrupt.SetAsInput();      // interrupt input

    IOWrite(3, 131);
    LOG_INFO("setting up 82C55...");
    IOWrite(0, 15);
  }

  void pins(uint8_t dat)
  {
    for (int i = 0; i < 8; i++)
    {
      ad[i].SetAsOutput();
      if (dat & (1 << i))
        ad[i].SetHigh();
      else
        ad[i].SetLow();
    }
  }

  void MemWrite(uint8_t address, uint8_t data)
  {
    latch(address);
    m_wr(data);
  }

  void IOWrite(uint8_t address, uint8_t data)
  {
    latch(address);
    io_wr(data);
  }

  uint8_t MemRead(uint8_t address)
  {
    latch(address);
    return m_rd();
  }

  uint8_t IORead(uint8_t address)
  {
    latch(address);
    return io_rd();
  }

  void IOPoll()
  {
    uint8_t PortA = 0x00;
    uint8_t PortB = 0x01;
    uint8_t data  = 0x00;

    IOWrite(PortA, 1);
    data = IORead(PortB);
    switch (data)
    {
      case 1:
        LOG_INFO("1");
        while (data == 1) data = IORead(PortB);
        break;
      case 2:
        LOG_INFO("2");
        while (data == 2) data = IORead(PortB);
        break;
      case 4:
        LOG_INFO("3");
        while (data == 4) data = IORead(PortB);
        break;
      case 8:
        LOG_INFO("A");
        while (data == 8) data = IORead(PortB);
        break;
    }

    IOWrite(PortA, 2);
    data = IORead(PortB);
    switch (data)
    {
      case 1:
        LOG_INFO("4");
        while (data == 1) data = IORead(PortB);
        break;
      case 2:
        LOG_INFO("5");
        while (data == 2) data = IORead(PortB);
        break;
      case 4:
        LOG_INFO("6");
        while (data == 4) data = IORead(PortB);
        break;
      case 8:
        LOG_INFO("B");
        while (data == 8) data = IORead(PortB);
        break;
    }

    IOWrite(PortA, 4);
    data = IORead(PortB);
    switch (data)
    {
      case 1:
        LOG_INFO("7");
        while (data == 1) data = IORead(PortB);
        break;
      case 2:
        LOG_INFO("8");
        while (data == 2) data = IORead(PortB);
        break;
      case 4:
        LOG_INFO("9");
        while (data == 4) data = IORead(PortB);
        break;
      case 8:
        LOG_INFO("C");
        while (data == 8) data = IORead(PortB);
        break;
    }

    IOWrite(PortA, 8);
    data = IORead(PortB);
    switch (data)
    {
      case 1:
        LOG_INFO("*");
        while (data == 1) data = IORead(PortB);
        break;
      case 2:
        LOG_INFO("0");
        while (data == 2) data = IORead(PortB);
        break;
      case 4:
        LOG_INFO("#");
        while (data == 4) data = IORead(PortB);
        break;
      case 8:
        LOG_INFO("D");
        while (data == 8) data = IORead(PortB);
        break;
    }
  }

  void latch(uint8_t add)
  {
    write_enable.SetHigh();
    read.SetHigh();
    write.SetHigh();
    for (int i = 0; i < 8; i+=1)  // Setting Address bits
    {
      ad[i].SetAsOutput();
      if (add & (1 << i))
        ad[i].SetHigh();
      else
        ad[i].SetLow();
    }
    ale.SetHigh();
    sjsu::Delay(1ns);
    ale.SetLow();
    sjsu::Delay(1ns);
  }

  void m_wr(uint8_t dat)
  {
    m_io.SetHigh();
    write.SetLow();
    for (int i = 0; i < 8; i+=1)
    {
      ad[i].SetAsOutput();
      if (dat & (1 << i))
        ad[i].SetHigh();
      else
        ad[i].SetLow();
    }
    sjsu::Delay(1ns);
    write.SetHigh();
    sjsu::Delay(1ns);
  }

  void io_wr(uint8_t dat)
  {
    m_io.SetLow();
    write.SetLow();
    write_enable.SetHigh();
    for (int i = 0; i < 8; i+=1)
    {
      ad[i].SetAsOutput();
      if (dat & (1 << i))
        ad[i].SetHigh();
      else
        ad[i].SetLow();
    }
    sjsu::Delay(1ns);
    write.SetHigh();
    sjsu::Delay(1ns);
    m_io.SetHigh();
  }

  uint8_t m_rd()
  {
    uint8_t data = 0;
    m_io.SetHigh();
    write_enable.SetLow();
    read.SetLow();
    sjsu::Delay(1ns);

    for (int i = 7; i >= 0; i-=1)
    {
      ad[i].SetAsInput();
      data = (data << 1);
      if (ad[i].Read())
        data = data + 1;
    }

    read.SetHigh();
    write_enable.SetHigh();
    return data;
  }

  uint8_t io_rd()
  {
    uint8_t data = 0;
    m_io.SetLow();
    write_enable.SetLow();
    read.SetLow();
    sjsu::Delay(1ns);

    for (int i = 7; i >= 0; i-=1)
    {
      ad[i].SetAsInput();
      data = (data << 1);
      if (ad[i].Read())
        data = data + 1;
    }

    read.SetHigh();
    write_enable.SetHigh();
    m_io.SetHigh();
    return data;
  }

 private:
  sjsu::lpc40xx::Gpio ad[8] = {
    sjsu::lpc40xx::Gpio(2, 2),  sjsu::lpc40xx::Gpio(2, 5),
    sjsu::lpc40xx::Gpio(2, 7),  sjsu::lpc40xx::Gpio(2, 9),
    sjsu::lpc40xx::Gpio(0, 15), sjsu::lpc40xx::Gpio(0, 18),
    sjsu::lpc40xx::Gpio(0, 1),  sjsu::lpc40xx::Gpio(0, 10),
  };

  sjsu::lpc40xx::Gpio write_enable = sjsu::lpc40xx::Gpio(0, 17);
  sjsu::lpc40xx::Gpio ale          = sjsu::lpc40xx::Gpio(0, 22);
  sjsu::lpc40xx::Gpio m_io         = sjsu::lpc40xx::Gpio(0, 0);
  sjsu::lpc40xx::Gpio interrupt    = sjsu::lpc40xx::Gpio(0, 11);
  sjsu::lpc40xx::Gpio write        = sjsu::lpc40xx::Gpio(0, 16);
  sjsu::lpc40xx::Gpio read         = sjsu::lpc40xx::Gpio(2, 8);
};