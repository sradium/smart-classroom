# SISTEMA DE CONTROL PARA UN SALON INTELIGENTE

Sistema de control de un salón de la Universidad Metropolitana que regula la iluminación de forma inteligente para proporcionar un ambiente confortable, aisla el ruido externo que interumpa las clases, provee un sistema de seguridad contra incendios y registra la temperatura, humedad relativa y niveles de CO2 en el salón (Requeridos para un futuro sistema de climatización).

## LIBRERIAS REQUERIDAS

* [SPI](https://www.arduino.cc/en/reference/SPI): Librería que gestiona la comunicación entre dispositivos por el protocolo SPI, con el Arduino como dispositivo maestro.
* [SD](https://www.arduino.cc/en/Reference/SD): Librería que permite leer y escribir sobre tarjetas SD que soporta un sistema de archivos **FAT16** o **FAT32**. La comunicación entre el Arduino y el módulo de lectura tiene que ser por el protocolo **SPI**.
* [Ethernet](https://www.arduino.cc/en/Reference/Ethernet): Librerías diseñada para gestionar la conexión a internet con shields basados en los integrados **Wiznet W5100/W5200/W5500**.
* [PubSubClient](https://pubsubclient.knolleary.net/): Esta librería provee un cliente para la publicación y suscripción de mensajes a un Broker [MQTT](https://mqtt.org/).

## EJEMPLO DE CONFIGURACION 

Con la finalidad de adaptar el sistema a las distintas características de los salones, se utiliza un lector de tarjetas micro SD por medio de una interfaz SPI para cargar la configuración específica de un salón. Para conocer el diagrama de conexión dirígase a la sección de [descripción del hardware](#DESCRIPCION-DEL-HARDWARE). La micro tarjeta SD contiene un archivo de configuración `configuration.json` que posee la siguiente estructura:

```json
{
    "server":{
        "ip":[192, 168, 0, 135],
        "dns": [200, 44, 32, 12],
        "mac": [222, 237, 186, 254, 254, 237],
        "broker":"broker.hivemq.com",
        "client_id":"A1314",
        "intervarl_publish_min": 5,
        "port": 1883
    },
    "illumination":{
        "timer_to_off_min": 15,
        "scenes":{
            "setpoint": 300,
            "highest": 100,
            "high": 70,
            "medium": 45,
            "low": 10
        }
    },
    "FireEquipment":[
        {
            "name": "emergency lever",
            "type": "sensor",
            "mode": "NO",
            "terminal": "FI1",
            "debounce_seg": 0.5
        },
        {
            "name": "smoke detector",
            "type": "sensor",
            "mode": "NO",
            "terminal": "FI2",
            "debounce_seg": 5
        },
        {
            "name": "smoke detector 2",
            "type": "sensor",
            "mode": "NO",
            "terminal": "FI3",
            "debounce_seg": 5
        },
        {
            "name": "Siren",
            "type": "actuator",
            "mode": "NO",
            "terminal": "FO1"
        }
    ]
}
```

TOKEN JWT con 256 bit KEY: cRfUjXn2r5u8x/A?D(G-KaPdSgVkYp3s and payload: {"sub": "pub/sub", "name": "A1314", "iat": 1516239022}
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiJwdWIvc3ViIiwibmFtZSI6IkExMzE0IiwiaWF0IjoxNTE2MjM5MDIyfQ.CTdgo-ZmodY8TIcQIePZGJghjgsLlFoUiOajBfGfo8o

# COMUNICACION CON EL BMS

Entre los objetivos del proyecto, el control inteligente de los salones pretende ayudar a que la edificación sea certificable por [LEED](https://www.usgbc.org/leed) en la categoría [BD+C: Building Design and Construction](https://www.usgbc.org/sites/default/files/LEED%20v4%20BDC_07.25.19_current.pdf). Por lo tanto, es requerido que el sistema tenga la capacidad de integrarse con otros sistemas de la edificación y sea escalable. 

Para ello, se planteó la siguiente arquitectura: cada salón que conforma este proyecto cada minuto reportara las condiciones del salón, por medio de un Broker MQTT, con un Building Management System (BMS). Este servidor se encarga de la integración con los demás subsistemas y la generación de reportes que contribuyen en el proceso de certificación.

## Mensajes de lecturas de sensores

## Estado del salón

Publicación del estado del salón que rige el funcionamiento del sistema. Cuando la variable `occupied` esté en `true` las luces estarán encendidas en la escena automatica o la seleccionada por el usuario por detectarse presencia en el salón, de caso contrario será `false`. Cuando la variable `fire_alarm` esté en `true` los detectores de humo percibieron un incendio o se activo la palanca de emergencia. La variable `projection_mode` determina si el video beam está activado y la escena de proyección está activa, en este modo las persionas estaran abajo, las luminarias marcadas en el archivo de configuración estarán apagadas y las que no estarán encendidas para aportar 100 lux en el salón.

**TOPIC** : `A1314/OUT/STATUS`

**QoS** : 0

**Message** : 
```json
{
    "occupied": true,
    "fire_alarm": false,
    "projection_mode": false
}
```

## Iluminación

Publicación de los niveles de iluminación de cada lazo de control de las luminarias del salón.

**TOPIC** : `A1314/OUT/ILLUM`

**QoS** : 0

**Message** : 
```json
{
    "unit": "lux",
    "loop1": {
        "value": 302
    },
    "loop2": {
        "value": 301
    },
    "loop3": {
        "value": 300
    },
    "loop4": {
        "value": 300
    },
    "loop5": {
        "value": 302
    },
    "loop6": {
        "value": 301
    },
    "loop7": {
        "value": 299
    },
    "loop8": {
        "value": 300
    }
}
```

## Temperatura

Publicación de temperatura promedio del salón y la registrada por cada sensor.

**TOPIC** : `A1314/OUT/TEMP`

**QoS** : 0

**Message** : 
```json
{
    "value": 23,
    "unit": "c",
    "s1": {
        "value": 23.1
    },
    "s2": {
        "value": 23
    },
    "s3": {
        "value": 23
    },
    "s4": {
          "value": 23
    },
    "s5": {
        "value": 22.9
    },
    "s6": {
        "value": 22.9
    }
}
```

## Nivel de CO2

Publicación de nivel de CO2 promedio del salón y el registrado por cada sensor.

**TOPIC** : `A1314/OUT/CO2`

**QoS** : 0

**Message** : 
```json
{
    "value": 200,
    "unit": "ppm",
    "s1": {
        "value": 201
    },
    "s2": {
        "value": 200
    },
    "s3": {
        "value": 200
    },
    "s4": {
          "value": 199
    },
    "s5": {
        "value": 200
    },
    "s6": {
        "value": 200
    }
}
```

## Humedad relativa

Publicación de nivel de humedad relativa promedio del salón y el registrado por cada sensor.

**TOPIC** : `A1314/OUT/HUM`

**QoS** : 0

**Message** : 
```json
{
    "value": 65,
    "unit": "percent",
    "s1": {
        "value": 65.2
    },
    "s2": {
        "value": 65
    },
    "s3": {
        "value": 64.9
    },
    "s4": {
          "value": 65
    },
    "s5": {
        "value": 64.9
    },
    "s6": {
        "value": 65
    }
}
```

## Mensajes de configuración

## Configuración de escenas de iluminación

Todos los salones cuentan con ocho escenas de iluminación. La primera es la escena de regulación automática según la luz diurna presente en la habitación hasta llegar al setpoint establecido, para cambiar este valor se tiene que enviar la cantidad de lux que se quiere mantener en el salón dentro del rango de 200 a 800 lux. 

Las otras escenas permiten al usuario escoger de forma manual cuatro niveles de regulación según el porcentaje de potencia de la lumninaria dentro del rango 0 a 100 %. Es importante respetar el orden descendente establecido, es decir si en la escena `high` tiene el valor de 70 y se quiere cambiar el valor de la escena `medium` a 80 el mensaje de configuración será ignorado.

**TOPIC** : `A1314/CFG/ILLUM/SCENE`

**QoS** : 1

**Message** : 
```json
{
    "setpoint": 300,
    "highest": 100,
    "high": 70,
    "medium": 45,
    "low": 10
}
```

## Configuración del tiempo de apagado

En el momento en el que **todos** los sensores de presencia dejen de estar activos, se iniciará el timer para el apagado de las luminarias. Si se detecta movimiento antes de que se haya acabado el tiempo este se reinicia. 

**TOPIC** : `A1314/CFG/ILLUM/DELAY`

**QoS** : 1

**Message** : 
```json
{
    "timer_to_off_min": 15,
}
```

# DESCRIPCION DEL HARDWARE


## LICENCIA 

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Licencia Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />Esta obra está bajo una <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Licencia Creative Commons Atribución-CompartirIgual 4.0 Internacional</a>.