# Сервер на Apache + php
1. Узнать ip компьютера с помощью ipconfig. Этот адрес вставить в код всех версий прошивки (6 строка), залить на плату по usb v1.0.0.
2. Установить XAMPP. В папку ...\xampp\htdocs скопировать php-файлы. Собрать файл прошивки v1.1.0 и поместить бинарный файл ...\firmware_v1.1.0\build\esp8266.esp8266.nodemcuv2\firmware_v1.1.0.ino.bin в папку ...\xampp\htdocs\firmware. Запустить Apache-сервер.
3. В браузере можете открыть http://localhost/check_update.php. Эта страница отобразит последнюю версию прошивки. Подождать пока плата обновит прошивку.

# Различия в прошивках
## v1.0.0
Диод мигает один раз
## v1.1.0
Диод мигает 2 раза