# Sincronización de  clientes

Este ejemplo explica cómo varios clientes pueden sincronizar sus estados (un número aleatorio que cambia de vez en cuando) a través de un servidor que simplemente reenvía mensajes entre ellos. Ejecuta el servidor y varios clientes para observar cómo sincronizan sus estados.


# Cómo jecutar los ejemplos de client/server

Vamos a suponer que tu proyecto se llama ``TPV2`` y que está en la carpeta ``C:\hlocal\TPV2``. Añade un ejemplo a tu proyecto (se puede añadir sólo uno a la vez poruqe todos tienen un método ``main``).

- Para ejecutar el servidor, abre una consola (ver abajo cómo) y escribe las siguientes instrucciones

```
  cd c:
  cd c:\hlocal\TPV2\bin
  DebugTPV2.exe server 2000
```

- Para ejecutar un cliente, abre una consola (ver abajo cómo) y escribe las siguientes instrucciones
  
```
  cd c:
  cd c:\hlocal\TPV2\bin
  DebugTPV2.exe client localhost 2000
```
  
- Para más información sobre cómo ejecutar el programa, abre una consola (ver abajo cómo) y escribe las siguientes instrucciones

```
  cd c:
  cd c:\hlocal\TPV2\bin
  DebugTPV2.exe client localhost 2000
```


# Cómo abrir una consola

- Pulsa la tecla ``Windows+R`` (o abre el menú ``Start`` y haz click en la opción ``Run``)
- Escribe en el recuadro ``cmd`` y haz clic en ``OK`` (o pulsar ``ENTER``)  


Mas información : [https://www.howtogeek.com/235101/10-ways-to-open-the-command-prompt-in-windows-10/](https://www.howtogeek.com/235101/10-ways-to-open-the-command-prompt-in-windows-10/).

