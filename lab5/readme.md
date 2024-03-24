# 1. Mosquitto

Для начала прочитать: https://mosquitto.org/man/mqtt-7.html

Добавить Mosquitto в переменные среды Path.

Откройте командную строку и введите следующую команду:

net start mosquitto

Это запустит Mosquitto Broker в фоновом режиме.

В новом окне командной строки введите следующую команду (замените “your_topic” на имя вашего топика):
```
mosquitto_sub -t "your_topic"
mosquitto_sub -h localhost -t test/topic
```

Это подпишет вас на указанный топик и вы будете получать все сообщения, отправленные в этот топик.

В еще одном новом окне командной строки введите следующую команду (замените “your_topic” на имя вашего топика и “your_message” на ваше сообщение):
```
mosquitto_pub -t "your_topic" -m "your_message"
mosquitto_pub -h localhost -t test/topic -m "Hello, MQTT!"
```

Это отправит ваше сообщение в указанный топик.

Вернитесь к окну командной строки, где вы подписались на топик, и вы должны увидеть ваше сообщение.

# 2. mqtt.by

При регистрации не прошли домены gmail.com, yandex.ru. На mail.ru завелся.

P.S. На yandex.ru все же пришло письмо, но уже поздно.

Заходим в профиль. Видим информацию для подключения:
```
Сервер: mqtt.by
Порты: 1883, 1885
Логин: login
Пароль: password
Префикс топика: user/login/ или /user/login/
Например: user/login/your/temp или /user/login/your/temp
```

Подписка на топик:
```
mosquitto_sub -h mqtt.by -p 1883 -u login -P password -t user/login/test -i 666
```

Нужно указать id клиента - любое число, чтобы получать сообщения по подписке.

Отправка сообщения:
```
mosquitto_pub -h mqtt.by -p 1883 -u login -P password -t user/login/test -m "<your_message>"
```

Если будут проблемы с получением сообщений дополнительно можно указать QoS (от 0 до 2), например так:
```
-q 1
```

# 3. Mobile + local

Есть проблема, связанная с обновление mosquitto до второй версии.
В папке с исполняемым файлов mosquitto отредактировать mosquitto.conf, нужно добавить строки:
```
allow_anonymus true
listener 1883
```
и презапустить mosquitto:
```
net stop mosquitto
net start mosquitto
```
В крайнем случае, попробуйте Linux.

Приложение, которое точно работает: *MQTT Dashboard Client*

# 4. Mobile + mqtt.by

Почти то же самое, проблем быть не должно.