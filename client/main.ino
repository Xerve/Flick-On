#define DOWN 70
#define UP 155
#define SERVO_PIN A0
#define TOGGLE_BUTTON D0
#define STATUS_LED D7

Servo servo;
int servo_position = DOWN; // starting position for the servo
int on = 0; // if the switch is on or not
int on_direc = 0; // which direction is on, 1 is up, 0 is down

int turn(String command) {
    if (command == "on" || command == "On" || command == "ON") {
        if (!on) { // if the light switch is off
            on = 1;
            if (on_direc == 1) { // if the switch's on position is up
                servo.write(DOWN);
            } else { // if the switch's on position is down
                servo.write(UP);
            }
            
            return 0;
        } else {
            return 1;
        }
    } else if (command == "off" || command == "Off" || command == "OFF") {
        if (on) { // if the ligh switch is on
            on = 0;
            if (on_direc == 1) { // if the switch's on position is up
                servo.write(UP);
            } else { // if the switch's on position is down
                servo.write(DOWN);
            }
            
            return 0;
        } else {
            return 1;
        }
    }
    
    return 2;
}

int localToggle() {
    if (!on) {
        Spark.publish("turned on");
        turn("on");
    } else {
        Spark.publish("turned off");
        turn("off");
    }
    
    return 200;
}

void setup() {
    servo.attach(SERVO_PIN);
    
    Spark.variable("on", &on, INT);
    Spark.variable("on_direc", &on_direc, INT);
    Spark.function("turn", turn);
    
    pinMode(TOGGLE_BUTTON, INPUT_PULLDOWN);
    pinMode(STATUS_LED, OUTPUT);
    
    digitalWrite(STATUS_LED, HIGH);
}

void loop() {
    digitalWrite(STATUS_LED, HIGH);
    if (digitalRead(TOGGLE_BUTTON)) {
        delay(10);
        if (digitalRead(TOGGLE_BUTTON)) {
            digitalWrite(STATUS_LED, LOW);
            delay(350);
            if (digitalRead(TOGGLE_BUTTON)) {
                delay(4000);
                if (digitalRead(TOGGLE_BUTTON)) {
                    on_direc = !on_direc;
                    on = !on;
                    digitalWrite(STATUS_LED, HIGH);
                    Spark.publish("direction changed");
                    while(digitalRead(TOGGLE_BUTTON));
                }
            } else {
                localToggle();
            }
        }
    }
}