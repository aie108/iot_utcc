#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <DHT.h>

#define DHTTYPE DHT22     // DHT 22  (AM2302), AM2321

// Wireless Network & Authentication
WiFiClient client; 

char* ssid     = "UTCC_CPE2";
char* password = "kn2hyn6e01";

// MySQL Server
IPAddress server_addr(xxx,xxx,xxx,xxx);
char user[] = "xxx";
char dbPassword[] = "xxx";
char query[128];
MySQL_Connection dbConnector((Client *)&client);

const char INSERT_DATA[] = "INSERT INTO MySimpleIoT.measured_data VALUEs (NULL, %d, %f, %f, NULL)";
int location_id = 1;

// DHT Sensor
const int DHTPin = 4;
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char humidityTemp[7];

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Initialize the DHT sensor
  Serial.print("\n\nInitialize temperature sensor\n");
  dht.begin();

  // Connect to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Database Connection
  while (!dbConnector.connect(server_addr, 3306, user, dbPassword)) {
    Serial.print(".");
    delay(500);
  }
   
  Serial.println("Database successfully connected.");  
}

void loop() {
  // Read humidity
  float h = dht.readHumidity();

  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    strcpy(celsiusTemp, "-404");
    strcpy(humidityTemp, "-1");
  } else {
    // Computes temperature values in Celsius + Fahrenheit and Humidity
    float hic = dht.computeHeatIndex(t, h, false);
    dtostrf(hic, 6, 2, celsiusTemp);
    dtostrf(h, 6, 2, humidityTemp);
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t Temperature: ");
  Serial.println(t);
  delay(1000);

  // Insert new data into DB
  sprintf(query, INSERT_DATA, location_id, t, h);
  //Serial.println("Inserting reading data into database.");
  Serial.println(query);

  MySQL_Cursor *cursor = new MySQL_Cursor(&dbConnector);
  cursor->execute(query);
  delete cursor;
  delay(60000);
}
