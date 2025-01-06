---

# DHT22 Sensor with ThingSpeak Integration

โปรเจคนี้ใช้เพื่ออ่านข้อมูลอุณหภูมิและความชื้นจากเซ็นเซอร์ DHT22 และส่งข้อมูลเหล่านี้ไปยัง ThingSpeak เพื่อการตรวจสอบและบันทึกข้อมูลแบบเรียลไทม์ โดยใช้โมดูล Wi-Fi ESP8266 เพื่อเชื่อมต่ออินเทอร์เน็ตและส่งข้อมูลไปยัง ThingSpeak

## เนื้อหาภายใน

- [บทนำ](#บทนำ)
- [อุปกรณ์ที่ต้องการ](#อุปกรณ์ที่ต้องการ)
- [ซอฟต์แวร์ที่ต้องการ](#ซอฟต์แวร์ที่ต้องการ)
- [การตั้งค่าและการกำหนดค่า](#การตั้งค่าและการกำหนดค่า)
- [คำอธิบายโค้ด](#คำอธิบายโค้ด)
- [ข้อควรระวัง](#ข้อควรระวัง)
- [ลิขสิทธิ์](#ลิขสิทธิ์)

## บทนำ

โปรเจคนี้แสดงวิธีการเชื่อมต่อเซ็นเซอร์ DHT22 กับไมโครคอนโทรลเลอร์ ESP8266 และส่งข้อมูลอุณหภูมิและความชื้นไปยัง ThingSpeak โดยใช้ Wi-Fi โมดูล ESP8266 เพื่ออัปโหลดข้อมูลไปยังช่องของ ThingSpeak สำหรับการวิเคราะห์หรือแสดงผลข้อมูล

## อุปกรณ์ที่ต้องการ

- **ESP8266 (เช่น NodeMCU, Wemos D1 mini)**
- **เซ็นเซอร์ DHT22 (สำหรับวัดอุณหภูมิและความชื้น)**
- **สายจัมเปอร์**
- **แผงบอร์ด (อุปกรณ์เสริม)**

### การเชื่อมต่อ

1. เชื่อมต่อขา **VCC** ของเซ็นเซอร์ DHT22 กับขา **3.3V** ของ ESP8266
2. เชื่อมต่อขา **GND** ของเซ็นเซอร์ DHT22 กับขา **GND** ของ ESP8266
3. เชื่อมต่อขา **DATA** ของเซ็นเซอร์ DHT22 กับขา **D2** ของ ESP8266
4. หากต้องการสามารถใช้ **ตัวต้านทาน pull-up 10kΩ** ระหว่างขา **DATA** และ **VCC**

## ซอฟต์แวร์ที่ต้องการ

- **Arduino IDE**: โค้ดนี้เขียนสำหรับสภาพแวดล้อม Arduino โดยใช้แพลตฟอร์ม ESP8266
- **ไลบรารี**:
  - `ESP8266WiFi`: สำหรับเชื่อมต่อ Wi-Fi กับ ESP8266
  - `DHT`: สำหรับอ่านข้อมูลจากเซ็นเซอร์ DHT22
  - `WiFiClient`: สำหรับจัดการคำขอ HTTP ไปยัง ThingSpeak

เพื่อให้ใช้งานไลบรารีที่ต้องการ:
1. เปิดโปรแกรม Arduino IDE
2. ไปที่ **Sketch > Include Library > Manage Libraries...**
3. ค้นหา `DHT sensor library` และติดตั้ง
4. ค้นหา `ESP8266` และติดตั้งแพลตฟอร์ม ESP8266

## การตั้งค่าและการกำหนดค่า

### ขั้นตอนที่ 1: กำหนดค่าการเชื่อมต่อ Wi-Fi

ในโค้ด ให้ปรับค่าต่อไปนี้ให้ตรงกับข้อมูลการเชื่อมต่อ Wi-Fi ของคุณ:

```cpp
const char* ssid = "YOUR_WIFI_SSID";           // เปลี่ยนเป็นชื่อ WiFi ของคุณ
const char* password = "YOUR_WIFI_PASSWORD";   // เปลี่ยนเป็นรหัสผ่าน WiFi ของคุณ
```

### ขั้นตอนที่ 2: กำหนดค่า ThingSpeak

สร้างบัญชีฟรีที่ [ThingSpeak](https://thingspeak.com/) แล้วสร้างช่องสำหรับข้อมูลอุณหภูมิและความชื้น

ในโค้ด ให้แทนที่ค่าต่อไปนี้ด้วย **API Key** ของคุณจาก ThingSpeak:

```cpp
String apiKey = "YOUR_THINGSPEAK_API_KEY";   // เปลี่ยนเป็น API Key สำหรับการเขียนข้อมูลในช่องของคุณ
```

อย่าลืมบันทึก **Field IDs** ที่เกี่ยวข้องกับอุณหภูมิและความชื้นในช่องของคุณ

## คำอธิบายโค้ด

### การตั้งค่าการเชื่อมต่อ Wi-Fi

ในฟังก์ชัน `setup()` ESP8266 จะเชื่อมต่อกับ Wi-Fi โดยใช้ข้อมูลที่กำหนดไว้ในโค้ด และแสดงสถานะการเชื่อมต่อไปยัง Serial Monitor

```cpp
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
}
Serial.println("WiFi connected");
```

### การอ่านข้อมูลจากเซ็นเซอร์

ในฟังก์ชัน `loop()` โค้ดจะอ่านข้อมูลอุณหภูมิและความชื้นจากเซ็นเซอร์ DHT22 ทุกๆ 2 วินาที:

```cpp
float humidity = dht.readHumidity();
float temperature = dht.readTemperature();  // ค่าอุณหภูมิในเซลเซียส
```

ถ้าเซ็นเซอร์ไม่สามารถอ่านข้อมูลได้ จะมีการแสดงข้อความผิดพลาดใน Serial Monitor:

```cpp
if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
}
```

### การส่งข้อมูลไปยัง ThingSpeak

ข้อมูลที่ได้จากเซ็นเซอร์จะถูกส่งไปยัง ThingSpeak ผ่านการเชื่อมต่อ HTTP:

```cpp
if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temperature);
    postStr += "&field2=";
    postStr += String(humidity);
    postStr += "\r\n\r\n";

    // ส่งคำขอ HTTP ไปยัง ThingSpeak
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.println("Data sent to ThingSpeak");

    // ปิดการเชื่อมต่อ
    client.stop();
}
```

### การหน่วงเวลา

หลังจากส่งข้อมูลไปยัง ThingSpeak แล้ว โค้ดจะหน่วงเวลา 20 วินาที เพื่อให้ตรงกับข้อจำกัดของอัตราการส่งข้อมูลของ ThingSpeak (ขั้นต่ำ 15 วินาที):

```cpp
delay(20000);
```

## ข้อควรระวัง

- ตรวจสอบให้แน่ใจว่า API Key และข้อมูล Wi-Fi ถูกต้อง
- เซ็นเซอร์ DHT22 อาจใช้เวลาในการตอบสนองเมื่อทำการอ่านข้อมูล หากเซ็นเซอร์ไม่สามารถอ่านค่าได้ ให้ตรวจสอบการเชื่อมต่อและสัญญาณ

## ลิขสิทธิ์

โปรเจคนี้เปิดเผยให้ใช้งานได้ฟรีภายใต้ลิขสิทธิ์ MIT

---

![คำอธิบายรูปภาพ](https://raw.githubusercontent.com/Useless007/hello_thingspeak/refs/heads/main/%E0%B8%A3%E0%B8%B9%E0%B8%9B%E0%B8%9B%E0%B8%A3%E0%B8%B0%E0%B8%81%E0%B8%AD%E0%B8%9A.jpg)
