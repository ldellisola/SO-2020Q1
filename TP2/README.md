# Read me para entrega parcial:
  Para esta entrega desde terminal se puede llamar a:
 
 #### Como built-ins : 
  * ps 
  * help
  * nice [id] [prioridad] //Contamos con 3 prioridades, 0,1,2 a mayor numero, mayor prioridad
  * block [id] //se llama para bloquear o desbloquear el proceso
  * mem
  * kill [id]
 
 #### Como procesos (es decir se los puede llamar con & en caso de querer tratarlos en background):
  * testMem [*&] 
  * loop  [*&]
  
Aún cuando están implementadas las 2 memorias en el Kernel, para probar el background es necesario implementar el Buddy, dado que la memManager genera un problema en ese caso. Para habilitarlo, debe dejarse el #define ENABLE_BUDDY que se encuentra en el TP2/Kernel/include/MemManager.h.

Como recomendación por como está implementada la impresión en pantalla, la terminal suele ponerse lenta al tener mucho impreso en ella, por lo que se recomienda llamar a "clear".

Por ultimo, el gran parte del codigo de este TP viene de nuestra presentacion del TP final de Arquitectura de Computadoras, una entrega donde tuvimos bastantes problemas y por esto gran parte del codigo puede estar comentado o el estilo del codigo no puede ser el mejor. De la misma forma, esta entrega tiene muchos warnings que tenemos planeado solucionar para la entrega final, pero no tuvimos el tiempo de ponernos a arreglar para esta entrega.
