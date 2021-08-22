using Tizen.NUI;
using Tizen.NUI.BaseComponents;
using Tizen.Peripheral.Gpio;

namespace SmartLight
{
    public partial class Scene1Page : View
    {
        private const int SENSOR_LED_GPIO_NUMBER = 24;
        private GpioPin ledGpio;

        public Scene1Page()
        {
            InitializeComponent();

            ledGpio = new GpioPin(SENSOR_LED_GPIO_NUMBER, GpioPinDriveMode.OutputInitiallyLow);
        }

        private void ChangeLEDLight(object sender, PinUpdatedEventArgs e)
        {
            if (ledGpio.Read() == GpioPinValue.High)
            {
                ledGpio.Write(GpioPinValue.Low);
            }
            else if (ledGpio.Read() == GpioPinValue.Low)
            {
                ledGpio.Write(GpioPinValue.High);
            }
        }

        public void Stop()
        {
            ledGpio.Dispose();
        }
    }
}
