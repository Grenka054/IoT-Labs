![Подключение](/sources/4.jpg)

# 2 платы
1. На первую плату загрузить серверный код, скопировать адрес для подключения.
2. Этот адрес вставить в клиентский код, раскомментировать 40 строку.
3. Загрузить клиентский код на вторую плату.

# 1 плата и ПК
## Плата-клиент
1. Узнать ip компьютера с помощью ipconfig. Этот адрес вставить в клиентский код, раскомментировать 39 строку, залить на плату.
2. Установить XAMPP. В папку ...\xampp\htdocs скопировать php-файлы. Запустить Apache-сервер.
3. В браузере открыть http://localhost/state_visual.php. Это визуализация серверного светофора (не обновялется при остуствии GET-запросов, но можно спамить через Runner в интерфейсе Postman).

## Плата-сервер
1. На плату загрузить серверный код, скопировать адрес для подключения.
2. Обращаться по этому адресу через Postman, например, http://192.168.1.103:80/state. Внизу будет отображаться полученный JSON.

