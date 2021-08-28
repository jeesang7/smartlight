using System;
using System.Threading;
using Tizen.Peripheral.Gpio;


namespace SmartLight
{
    class RGBMixer
    {
        GpioPin gpioPwm;
        int dutyCycle = 0;
        Thread thread;

        public RGBMixer(int pinnum)
        {
            gpioPwm = new GpioPin(pinnum, GpioPinDriveMode.OutputInitiallyHigh);
            gpioPwm.Write(GpioPinValue.High);
            thread = new Thread(new ThreadStart(Run));
        }

        public void AnalogWrite(int duty)
        {
            dutyCycle = duty;

            if (!thread.IsAlive)
                thread.Start();
        }

        void Run()
        {
            Console.WriteLine("Thread#{0}: Begin", Thread.CurrentThread.ManagedThreadId);

            while (true)
            {
                // 50Hz
                gpioPwm.Write(GpioPinValue.Low);
                Thread.Sleep(dutyCycle / 5);
                gpioPwm.Write(GpioPinValue.High);
                Thread.Sleep((100 - dutyCycle) / 5);
            }
        }
    }
}
