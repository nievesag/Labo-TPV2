# Entity-Component-System 

* ecs_0: solo EC, con grupos V1 (solo bitset de groups en Entity)
* ecs_1: only EC, con groups V2 (como V1 per el Manager mantiene listas de groups)
* ecs_2: only EC, con groups V3 (cada Entity pertenece a un solo grupo, el Manager mantiene listas de groups)
* ecs_3: como ecs_2, pero los métodos de Entity están en el Manager, y usa entity_t para Entity*
* ecs_4: como ecs_3, pero con sistemas y comunicación usando mensajes.

## Como usar

* copiar la carpeta *ecs* a la carpeta a la raiz de la carpeta **src**
* copiar el contenido de un ejemplo de uso a la raiz de la carpeta **src**

La estructura tiene que ser así:

     src/ecs
     src/components
     src/systems
     src/game
     src/utils
     src/json
     src/sdlutils
     main.cpp

 