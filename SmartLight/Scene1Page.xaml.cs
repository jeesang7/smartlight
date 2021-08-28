using Tizen.NUI.BaseComponents;
using Tizen.Peripheral.Gpio;

namespace SmartLight
{
    public partial class Scene1Page : View
    {
        private const int SENSOR_LED_GPIO_NUMBER = 24;
        private GpioPin ledGpio;
        private RESTClient restClient;

        public Scene1Page()
        {
            InitializeComponent();

            ledGpio = new GpioPin(SENSOR_LED_GPIO_NUMBER, GpioPinDriveMode.OutputInitiallyLow);
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
    }
}
