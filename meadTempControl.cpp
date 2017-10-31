#include "mbed.h"

void wakeUp();

int main()
{
    AnalogIn analog_value(A0);
    DigitalOut relay(D0);
    Ticker timer;
    
    const uint32_t analogScalar = 3300;     //0-3.3V Range
    const uint32_t tempHigh = 700;          //68F
    const uint32_t tempLow = 683;           //64.94F
    const uint32_t sleepTime = 5;           //Time in seconds
    
    float meas_r;
    float meas_v;
    enum state  {
        on = 1,
        off = 0
    };
    enum state fridgeState;
    fridgeState = off;
    relay = fridgeState;
    timer.attach(&wakeUp,sleepTime);

    while(1)
    {
        meas_r = analog_value.read(); // Read the analog input value (value from 0.0 to 1.0 = full ADC conversion range)
        meas_v = meas_r * analogScalar; // Converts value in the 0V-3.3V range
        
        // Display values
        printf("measure = %f = %.0f mV\n", meas_r, meas_v);

        //FSM implements hysteresis so that fridge doesn't toggle incessantly
        switch (fridgeState)   {
            case on:
                if (meas_v <= tempLow)
                {
                    fridgeState = off;
                    sleep();
                }
                break;
            case off:
                if (meas_v >= tempHigh)
                {
                    fridgeState = on;
                }
                else
                {
                    sleep();
                }
                break;
            default:
                fridgeState = off;
        }
        relay = fridgeState;
        //Definitely need to tune TEMPLIMIT based on the device and desired temp

        wait(0.5); // 0.5 second
    }
}

void wakeUp()
{
    printf("Waking up from sleep.\n");
}
