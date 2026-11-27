const unsigned long OPEN_MS = 10000;      // how long the blind takes to open

bool running = false;                     // button toggles this (automatic mode)
unsigned long startTime = 0;
String wakeupTime = "";
String blindStatus = "";

void setupComponents() {
  setupServo();
  setupLeds();
  setupButton();
  setupLightSensor(); // not working
  setupMotionSensor();
  setupRainSensor();
  setupDisplay();
}

void setup() {
  Serial.begin(115200);
  unsigned long start = millis();
  while (!Serial && millis() - start < 5000) delay(10);

  setupComponents();
  setupApiClient();

  pinMode(LED_BUILTIN, OUTPUT);
}

// ---------- helper functions ----------

// get status and wakeup time from the api
void fetchApiData() {
  static unsigned long lastStatus = 0;
  static unsigned long lastWakeup = 0;

  if (lastStatus == 0 || millis() - lastStatus >= 5000) {
    lastStatus = millis();
    String s = getBlindStatus();
    s.trim();
    if (s != "") blindStatus = s;     // keep old value if the request failed
  }

  if (lastWakeup == 0 || millis() - lastWakeup >= 30000) {
    lastWakeup = millis();
    String w = getWakeupTime();
    w.trim();
    if (w != "") wakeupTime = w;
  }
}

// how far along the opening is, 0.0 to 1.0
float openProgress() {
  float p = (float)(millis() - startTime) / OPEN_MS;
  if (p > 1.0) p = 1.0;
  return p;
}

// moves the servo and sets the lights, returns the servo angle
int updateBlinds(bool pressed) {
  if (blindStatus == "open") {
    running = false;
    setLeds(255, 255, 255);
    return 90;
  }

  if (blindStatus == "closed") {
    running = false;
    setLeds(0, 0, 0);
    return 0;
  }

  // automatic: wait for the button
  if (pressed) {
    running = !running;
    startTime = millis();
  }

  if (running) {
    float p = openProgress();
    setLeds(255 * p, 180 * p, 80 * p);   // warm white slowly turning on
    return 90 * p;                       // servo slowly opening
  }

  setLeds(0, 0, 0);
  return 0;
}

// ---------- loop ----------

void loop() {
  fetchApiData();

  // sensors
  long lux = readLux();
  if (lux < 0) lux = 0;
  int rain = readRain();
  bool motion = readMotion();

  // button (detect a new press)
  static bool lastBtn = false;
  bool btn = buttonPressed();
  bool pressed = btn && !lastBtn;
  lastBtn = btn;

  // servo + lights
  int angle = updateBlinds(pressed);
  writeServo(angle);

  // display (every 500 ms)
  static unsigned long lastShow = 0;
  if (millis() - lastShow >= 500) {
    lastShow = millis();
    displayLines("Wake: " + wakeupTime,
        "Status: " + blindStatus,
        "Motion: " + String(motion ? "yes" : "no") + " Light: " + String(lux),
        "Rain: " + String(rain) + " %");
  }

  // send to the api (every 10 s)
  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 10000) {
    lastSend = millis();
    int result = sendSensorDataToApi(angle, lux, rain);   // motor, light, rain
    Serial.println(result == 0 ? "sent sensor data" : "sending failed");
  }

  // heartbeat
  digitalWrite(LED_BUILTIN, (millis() / 500) % 2);

  delay(50);
}
