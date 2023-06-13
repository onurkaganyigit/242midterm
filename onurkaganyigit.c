#include "main.h"
#include "math.h"
#include "usbd_cdc_if.h"

#define INPUT_SIZE 3 // girenlerin size'ı

float inputs[INPUT_SIZE];
float weights[INPUT_SIZE];
float bias; //esik deger - bias

// ADC read
void ADC_Read(float* inputs) {

}

// PWM write
void PWM_Write(float output) {

}

// gelen bir değere sigmoid aktivasyon fonksiyonunu uygular ve sonucunu döndürür.
float sigmoid(float x) {
    return 1 / (1 + exp(-x));
}

// girişleri ve ağırlıkları kullanarak perceptron hesaplar.
void Perceptron() {
    float sum = 0.0f;
    for (int i = 0; i < INPUT_SIZE; i++) {
        sum += inputs[i] * weights[i];
    }
    sum += bias;
    float output = sigmoid(sum);
    PWM_Write(output); // çıkan sonuç write fonksiyonuna gelir
}

void VeriDuzenleyici(uint8_t* Buf, uint32_t *Len) {
    char* token = strtok((char*)Buf, ","); //strok verileri virgülle ayrıştırır
    for(int i = 0; i < INPUT_SIZE; i++) {
        weights[i] = atof(token); //atof float'a dönüştürür
        token = strtok(NULL, ",");
    }
    bias = atof(token);
}

// Bu kısmı internetten buldum kodum error veriyordu.
void Error_Handler(void) {
    // Add your error handling code here
    while (1) {
        // Loop indefinitely or take appropriate action
    }
}

//USB'den veri alır
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len) {
    ParseAndUpdateValues(Buf, Len);
    return (USBD_OK);
}

int main(void) {

    uint8_t data[64];  // 64 bit sınırlı integer verdim

    USBD_HandleTypeDef hUsbDeviceFS;

    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, data);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);

    while(1) { //sonsuz döngüde çalışır
        ADC_Read(inputs); // inputları okur
        Perceptron(); // kodu çalıştırır output çıkarır
    }
}
