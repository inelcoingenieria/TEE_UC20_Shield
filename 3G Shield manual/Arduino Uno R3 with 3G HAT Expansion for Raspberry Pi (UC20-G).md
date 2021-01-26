ตัวอย่างการใช้งานบอร์ด Arduino Uno R3 ร่วมกับ  3G HAT Expansion for Raspberry Pi (UC20-G)


อุปกรณ์ที่ใช้
1. (EADN014) บอร์ด Arduino Uno R3 https://www.thaieasyelec.com/product/73/arduino-uno-r3-%E0%B8%9A%E0%B8%AD%E0%B8%A3%E0%B9%8C%E0%B8%94%E0%B9%80%E0%B9%80%E0%B8%97%E0%B9%89-100
2. (ETEE064) 3G HAT Expansion for Raspberry Pi (UC20-G) ***On Sale*** https://www.thaieasyelec.com/product/1096/3g-hat-expansion-for-raspberry-pi-uc20-g
3. (EETT067) ET-Mini Logic Level Bi สำหรับแปลงสัญญาณระหว่าง 3.3-5V https://www.thaieasyelec.com/product/195/et-mini-logic-level-bi
4. (ELNK054) 20 cm Male to Female Cable Jumper Wire 40 Pins https://www.thaieasyelec.com/product/742/20-cm-male-to-female-cable-jumper-wire-40-pins-%E0%B8%9C%E0%B8%B9%E0%B9%89-%E0%B9%80%E0%B8%A1%E0%B8%B5%E0%B8%A2


การต่อวงจรดังนี้ 

![Capture](https://user-images.githubusercontent.com/8803501/105672079-64cce180-5f16-11eb-97bb-f9cb0b266c2f.JPG)


หลังจากที่ต่อวงจรเสร็จเรียบร้อยแล้วจะต้องทำการตั้งค่า Baudrate ของตัว 3G HAT เป็น 9600 bps (โดย default โมดูลจะตั้ง Baudrate เป็น 115200 bps) 
- ให้ทำการกดเปิดโมดูลโดยการกดปุ่ม PWRKEY ค้างไว้ประมาณ 1-2 วินาที เมื่อโมดูลเปิดจะมี LED สีเหลืองติดขึ้นมา 


1. ให้ติดตั้ง Library TEE_UC20_Shield โดย Download จาก Lbrary manager โดยไปที่เมนู Sketch >> Include Library >> Mangae Libraries.. ทำการค้นหา TEE_UC20_Shield และติดตั้งดังภาพ

![image](https://user-images.githubusercontent.com/8803501/105673919-4f0ceb80-5f19-11eb-94b1-946f3156957c.png)



2. รันโปรแกรม ATCommand_serial เพื่อใช้งานโหมด AT command โดยไปที่เมนู File >> Examples >> TEE_UC20_Shield >> ATCommand_serial 

![image](https://user-images.githubusercontent.com/8803501/105674613-5e406900-5f1a-11eb-8d9d-d8cefa0bf52b.png)



3. ภายในโปรแกรม ATCommand_serial แก้ไข mySerial.begin(9600); เป็น mySerial.begin(115200); และทำการ upload 

![image](https://user-images.githubusercontent.com/8803501/105675177-43babf80-5f1b-11eb-9504-ac27ec182f5e.png)



4. เปิด Serial monitor โดยไปที่แถบเมนู Tools >> Serial monitor โดยตั้ง baudrate บน Serial monitor เป็น 9600 bps และเลือกเป็น Both NL & CR ดังภาไ จากนั้นพิมพ์คำสั่ง AT กด Enter จากนั้นตัวโมดูลจะตอบกลับมาว่า OK ซึ่งหมายถึงบอร์ด Arduino สามารถติดต่อกับตัวโมดูลได้

![image](https://user-images.githubusercontent.com/8803501/105795687-cdbc6400-5fbf-11eb-92d4-4999b3ed97dd.png)

![image](https://user-images.githubusercontent.com/8803501/105676107-a2346d80-5f1c-11eb-9255-8db14a6242cb.png)



5. ทำการตั้งค่า URC Port ให้ออกทาง UART โดยใชคำสั่ง AT+QURCCFG="urcport","uart1" จากนั้นตัวโมดูลจะตอบกลับมาว่า OK ซึ่งหมายถึงได้ทำการเปลี่ยนให้ URC Port ออกทาง UART แล้ว

![image](https://user-images.githubusercontent.com/8803501/105678007-356ea280-5f1f-11eb-81a1-211707cd2b70.png)



6. ส่งคำสั่งเพื่อทำการเปลี่ยน baudrate เป็น 9600 bps โดยใชำคำสั่ง AT+IPR=9600;&W หากตัวโมดูลตอบกลับมาว่า OK หมายถึงได้ทำการเปลี่ยน baudrate เรียบร้อยแล้ว

![image](https://user-images.githubusercontent.com/8803501/105676669-6cdc4f80-5f1d-11eb-9f08-042a43fcba1b.png)



7. ทำการทดสอบโดยรันโปรแกรม NTP ไปที่ เมนู File >> Examples >> TEE_UC20_Shield >> TCP >> NTP จากนั้นทำการอัพโหลด และติดตามผลการเชื่อมต่อ NTP server เป็นอันเรียบร้อย 

![image](https://user-images.githubusercontent.com/8803501/105677213-29ceac00-5f1e-11eb-93da-9d94ae7a927d.png)



***หากตัวโมดูลยังไม่ตอบกลับ ให้กลับไปเช็คการต่อวงจรอีกครั้งว่าถูกต้องหรือไม่

