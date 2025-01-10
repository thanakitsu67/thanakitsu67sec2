#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD address to 0x27 for a 16 chars and 2-line display

int menu = 0;

int sw_next = 15; // Switch to go to the next menu
int sw_ok = 2;    // Switch to confirm selection

int leds[] = {23, 19, 18, 5, 17, 16, 4, 0}; // LED pins
const int num_leds = sizeof(leds) / sizeof(leds[0]);

// Function prototypes
void executeMenu(int menu);
void turnOnAllLEDs();
void blinkAlternateLEDs();
void rotateSingleLED();
void blinkInPairs();
void sequentialOnOff();
void blinkAllLEDs();
void turnOffAllLEDs();

// Function to execute the menu actions
void setup() {
    lcd.init(); // Initialize the LCD
    lcd.backlight();

    pinMode(sw_next, INPUT_PULLUP);
    pinMode(sw_ok, INPUT_PULLUP);

    // Initialize LED pins
    for (int i = 0; i < num_leds; i++) {
        pinMode(leds[i], OUTPUT);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Menu: 0");
}

void loop() {
    static int last_next_state = HIGH;
    static int current_menu = 0;
    static unsigned long last_action_time = 0;

    int current_next_state = digitalRead(sw_next);

    // Handle menu navigation
    if (current_next_state == LOW && last_next_state == HIGH) {
        menu++;
        if (menu > 6) {
            menu = 0;
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Menu: ");
        lcd.setCursor(6, 0);
        lcd.print(menu);

        // Update current menu immediately
        current_menu = menu;
        last_action_time = millis(); // Reset the timer for debounce
    }

    // Execute the current menu
    if (current_menu == 0) {
        turnOffAllLEDs();
    } else {
        executeMenu(current_menu);
    }

    last_next_state = current_next_state;
}

void executeMenu(int menu) {
    switch (menu) {
        case 1:
            turnOnAllLEDs(); // LED ทั้ง 8 หลอดเปิดพร้อมกัน
            break;
        case 2:
            blinkAlternateLEDs(); // LED กระพริบสลับกัน (คู่-คี่)
            break;
        case 3:
            rotateSingleLED(); // LED สว่างทีละหลอดวนไปเรื่อย ๆ
            break;
        case 4:
            blinkInPairs(); // LED ติดและดับทีละคู่ (2 หลอดในเวลาเดียวกัน)
            break;
        case 5:
            sequentialOnOff(); // LED สว่างเรียงลำดับจากหลอดแรกถึงหลอดสุดท้ายและดับกลับทีละหลอด
            break;
        case 6:
            blinkAllLEDs(); // LED กระพริบทั้งหมดพร้อมกัน
            break;
        default:
            turnOffAllLEDs();
            break;
    }
}

void turnOnAllLEDs() {
    for (int i = 0; i < num_leds; i++) {
        digitalWrite(leds[i], HIGH);
    }
}

void turnOffAllLEDs() {
    for (int i = 0; i < num_leds; i++) {
        digitalWrite(leds[i], LOW);
    }
}

void blinkAlternateLEDs() {
    static unsigned long last_time = 0;
    static bool state = false;

    if (millis() - last_time >= 300) {
        last_time = millis();
        state = !state;
        for (int i = 0; i < num_leds; i++) {
            digitalWrite(leds[i], (i % 2 == 0) == state ? HIGH : LOW);
        }
    }
}

void rotateSingleLED() {
    static int current_led = 0;
    static unsigned long last_time = 0;

    if (millis() - last_time >= 200) {
        last_time = millis();
        turnOffAllLEDs();
        digitalWrite(leds[current_led], HIGH);
        current_led = (current_led + 1) % num_leds;
    }
}

void blinkInPairs() {
    static unsigned long last_time = 0;
    static int current_pair = 0;
    static bool state = false;

    if (millis() - last_time >= 500) {
        last_time = millis();
        state = !state;

        if (state) {
            // Turn on LEDs in pairs (2 at a time)
            for (int i = 0; i < 8; i += 2) {
                if (i / 2 == current_pair) {
                    digitalWrite(leds[i], HIGH);
                    if (i + 1 < 8) {
                        digitalWrite(leds[i + 1], HIGH);
                    }
                }
            }
            current_pair++;
            if (current_pair >= num_leds / 2) {
                current_pair = 0;
            }
        } else {
            turnOffAllLEDs();
        }
    }
}

void sequentialOnOff() {
    static int current_led = 0;
    static bool ascending = true;
    static unsigned long last_time = 0;

    if (millis() - last_time >= 200) {
        last_time = millis();
        turnOffAllLEDs();
        digitalWrite(leds[current_led], HIGH);

        if (ascending) {
            current_led++;
            if (current_led >= num_leds) {
                current_led = num_leds - 1;
                ascending = false;
            }
        } else {
            current_led--;
            if (current_led < 0) {
                current_led = 0;
                ascending = true;
            }
        }
    }
}

void blinkAllLEDs() {
    static unsigned long last_time = 0;
    static bool state = false;

    if (millis() - last_time >= 500) {
        last_time = millis();
        state = !state;
        if (state) {
            turnOnAllLEDs();
        } else {
            turnOffAllLEDs();
        }
    }
}

