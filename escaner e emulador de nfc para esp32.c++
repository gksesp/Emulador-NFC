#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_PN532.h>

// Configurações do Display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Configurações do Módulo NFC
#define SDA_PIN 21
#define SCL_PIN 22
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup() {
  Serial.begin(115200);
  
  // Inicializa o display OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Erro ao iniciar o display OLED"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Inicializando NFC...");
  display.display();

  // Inicializa o módulo NFC
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("Não foi possível detectar o módulo PN532");
    while (1);
  }
  nfc.SAMConfig();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("NFC pronto!");
  display.display();
  delay(1000);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Aproxime o cartão NFC");
  display.display();

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  // Escaneia a tag NFC
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Cartao Detectado!");
    display.setCursor(0, 20);
    display.print("UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      display.print(uid[i], HEX);
      display.print(" ");
    }
    display.display();
    delay(2000);

    // Emula o código NFC (exemplo simples)
    emulateNFC(uid, uidLength);
  }
}

void emulateNFC(uint8_t *uid, uint8_t uidLength) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Emulando UID...");
  display.display();

  // Exemplo básico para emular um UID específico
  uint8_t emulatedUID[] = { 0xDE, 0xAD, 0xBE, 0xEF };
  for (int i = 0; i < uidLength; i++) {
    uid[i] = emulatedUID[i];
  }

  display.setCursor(0, 20);
  display.print("Novo UID:");
  for (uint8_t i = 0; i < uidLength; i++) {
    display.print(emulatedUID[i], HEX);
    display.print(" ");
  }
  display.display();
  delay(3000);
}
