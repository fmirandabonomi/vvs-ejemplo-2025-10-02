# Spot WiFi - Historias de usuario

El Spot WiFi es una luminaria directiva (patrón puntual o spot) con control de dirección y encendido/apagado de luz mediante una interfaz de control web sobre una red WiFi local.

## Personas

Usuario: Usuario del Spot WiFi

## Historias (H#) y Criterios de Aceptación (C#)

H1. Como Usuario, quiero que haya una interfaz web para comandar en forma remota mi *Spot WiFi* desde mi móvil. Ref. UC1

- CA1.1 El *Spot WiFi* puede conectarse a la red WiFi local.
- CA1.2 El *Spot WiFi* provee un servidor http.
- CA1.3 El servidor sirve al menos un *panel de control web* del Spot.
- CA1.4 El *panel de control web* debe incluir al menos la identidad del Spot.

H2. Como Usuario, quiero una dirección web tipo reflector_&lt;id&gt;.local poder acceder en forma simple a la interfaz web de mi Spot WiFi. Ref. UC1

- CA2.1 El *Spot WiFi* debe responder a peticiones de mDNS, permitiendo resolver una dirección de tipo *reflector_&lt;id&gt;.local*
- CA2.2 La dirección mDNS debe ser visible en el cuerpo del Spot.
- CA2.3 Navegando a la dirección resuelta se accede al *panel de control web* del *Spot WiFi*.

H3. Como Usuario, quiero un control de luz en la interfaz web de mi Spot para poder ver si está encendido, encenderlo y apagarlo. Ref. UC2

- CA3.1 El *panel de control web* debe incluir una *sección de luz*
- CA3.2 La *sección de luz* debe contar con un indicador de estado *encendida*/*apagada*
- CA3.3 La *sección de luz* debe contar con controles que permitan *encender* y *apagar* la luz.
- CA3.4 Si la luz está *apagada*, al usar el control *encender* su estado pasará a *encendida*.
- CA3.5 Si la luz está *encendida*, al usar el control *encender* su estado pasará a *apagado*.
- CA3.6 El estado de *encendida* o *apagada* mostrado en la sección de luz debe ser un fiel reflejo del estado de la luz del Spot.

H4. Como Usuario, quiero un control de dirección en la interfaz web de mi Spot para poder dirigir su luz a donde la necesite. Ref. UC3

- CA4.1 El *panel de control web* debe incluir una *sección de dirección*.
- CA4.2 La *seccion de dirección* debe contar con un *indicador de dirección*
- CA4.3 La *sección de dirección* debe contar con un *control de dirección* que permita modificar la dirección del Spot.
- CA4.4 Los cambios ordenados por el *control de dirección* deben reflejarse en el *indicador de dirección*.
- CA4.5 La dirección indicada por el *indicador de dirección* debe coincidir con la verdadera dirección del Spot.
