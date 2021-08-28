using System;
using Tizen.NUI.BaseComponents;
using Tizen.Peripheral.Gpio;


namespace SmartLight
{
    public partial class Scene1Page : View
    {
        private const int SENSOR_LED_GPIO_NUMBER = 24;
        private const int RGBLED_R_GPIO_NUMBER = 17;
        private const int RGBLED_B_GPIO_NUMBER = 27;
        private const int RGBLED_G_GPIO_NUMBER = 22;

        private GpioPin ledGpio;
        private RGBMixer ledPwmR, ledPwmB, ledPwmG;
        private RESTClient restClient;

        public Scene1Page()
        {
            InitializeComponent();

            ledGpio = new GpioPin(SENSOR_LED_GPIO_NUMBER, GpioPinDriveMode.OutputInitiallyLow);
            ledPwmR = new RGBMixer(RGBLED_R_GPIO_NUMBER);
            ledPwmB = new RGBMixer(RGBLED_B_GPIO_NUMBER);
            ledPwmG = new RGBMixer(RGBLED_G_GPIO_NUMBER);
            restClient = new RESTClient();
            restClient.Read();
        }

        private void ChangeLEDLight(object sender, PinUpdatedEventArgs e)
        {
            if (ledGpio.Read() == GpioPinValue.High)
            {
                ledGpio.Write(GpioPinValue.Low);
                restClient.Write("low");
            }
            else if (ledGpio.Read() == GpioPinValue.Low)
            {
                ledGpio.Write(GpioPinValue.High);
                restClient.Write("high");
            }
        }

        public void Stop()
        {
            ledGpio.Dispose();
        }

        private void OnRValueChanged(object sender, Tizen.NUI.Components.SliderValueChangedEventArgs e)
        {
            Console.WriteLine("R value" + e.CurrentValue);

            ledPwmR.AnalogWrite((int)e.CurrentValue);
        }

        private void OnGValueChanged(object sender, Tizen.NUI.Components.SliderValueChangedEventArgs e)
        {
            Console.WriteLine("G value" + e.CurrentValue);

            ledPwmG.AnalogWrite((int)e.CurrentValue);
        }

        private void OnBValueChanged(object sender, Tizen.NUI.Components.SliderValueChangedEventArgs e)
        {
            Console.WriteLine("B value" + e.CurrentValue);

            ledPwmB.AnalogWrite((int)e.CurrentValue);
        }
    }
}
