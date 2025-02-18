# UART_fpga_esp32
FPGA interface with ESP32 or ESP8266 via UART protocol, connect to Thingsboard.

Button và đèn LED kết nối phía ESP32 (hoặc dùng LED và Button có sẵn trên ESP8266).

Khi nhấn BTN trên Zuboard thì gửi lệnh Zuboard -> ESP32 qua UART, bật tắt đèn trên ESP32, đồng thời hiển thị Led Indicator trên Thingsboard.

Khi nhấn button trên ESP32, hoặc khi toggle switch trên Thingsboard thì gửi lệnh ESP32 -> Zuboard qua UART, bật tắt led trên Zuboard.

https://github.com/user-attachments/assets/7471a146-a458-4281-b12c-d379d0f1d4b4
