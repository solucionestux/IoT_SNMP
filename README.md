# IoT_SNMP [![Build Status](https://travis-ci.org/Greencorecr/IoT_SNMP.svg?branch=master)](https://travis-ci.org/Greencorecr/IoT_SNMP)
Dispositivos con The Things Network para conectar a SNMP

## [Conector con MQTT](https://github.com/Greencorecr/IoT_SNMP/tree/master/software/ttn2db)

Se mantiene un daemon escuchando en la plataforma de MQTT de TheThingsNetwork, escuchando mensajes de los diferentes sensores, y guardando en una base de datos de InfluxDB en formato JSON, los valores decodificados que recibe, así como un timestamp del momento en el que recibe el mensaje.

## [Conector con SNMP](https://github.com/Greencorecr/IoT_SNMP/tree/master/software/SNMP)

Dependiendo del OID consultado, snmpd llama al conector que saca de la base de el valor del sensor, así como verificando que el timestamp no sea más viejo que 5 minutos, lo que quiere decir que el sensor ha dejado de comunicarse, en el caso en que retorna "0" como valor del sensor. Se utiliza la técnica de este [ejemplo](https://github.com/fede2cr/raspberry-pi_snmp/blob/master/snmpd-example/snmpd.conf)

## Sensores

![Case con TTGO](https://github.com/Greencorecr/IoT_SNMP/raw/master/doc/img/Case%2BTTGO.jpg "Case con TTGO")

![Case con PCB](https://github.com/Greencorecr/IoT_SNMP/raw/master/doc/img/Case+PCB.jpg "Case con TTGO")

- [Sensor de Consumo eléctrico](https://github.com/Greencorecr/IoT_SNMP/tree/master/firmware/Amperaje)
- [Sensor de Caída eléctrica](https://github.com/Greencorecr/IoT_SNMP/tree/master/firmware/Caida)
- [Sensor de Gotas](https://github.com/Greencorecr/IoT_SNMP/tree/master/firmware/Gotas)
- [Sensor de Temperatura y Humedad](https://github.com/Greencorecr/IoT_SNMP/tree/master/firmware/TempHum)
- [Sensor de Puerta](https://github.com/Greencorecr/IoT_SNMP/tree/master/firmware/Puerta)


## Arduino IDE

### Dependencias:

- Paquetes necesarios para Ubuntu

```
sudo apt update
sudo apt install -y python2 python2-pip
sudo python2 -m pip install pyserial
```

### Librerías:
- Instalar IBM LMIC framework by IBM Version 1.5.0+arduino-2

  Se debe modificar el archivo ``Arduino/libraries/IBM_LMIC_framework/src/lmic/config.h``, comentando la primera línea y descomentando la segunda:

```
  //#define CFG_eu868 1
  #define CFG_us915 1
```
- Instalar DHT sensor library for ESPx by beegee_tokyo Version 1.0.9
- Instalar U8g2 by oliver Version 2.25.10
- Instalar [Arduino_SNMP](https://github.com/fede2cr/Arduino_SNMP) en su carpera de liberías de Arduino, como puede ser ``Arduino/libraries/``
- Instalar EasyButto by Evert Arias, Version 1.0.1

### Tarjetas/Boards

- Incluir en File -> Preferences -> Additional Boards Manager URLs: ``https://dl.espressif.com/dl/package_esp32_index.json``

- Instalar en Tool -> Board -> Board Manager: esp32 by Espressif Systems
