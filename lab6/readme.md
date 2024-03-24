[Подключение](/sources/4.jpg)

4 лаба, но с другим протоколом, который даже проще отлаживать.

Подписка на топик:
```
mosquitto_sub -h mqtt.by -p 1883 -u login -P password -t user/login/trafficligth -i 112
```

Отправка сообщения:
```
mosquitto_pub -h mqtt.by -p 1883 -u login -P password -t user/login/trafficligth -m '{"green":0,"yellow":1,"red":0}'
```