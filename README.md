# Webserv

### HTTP
Es el protocolo de comunicación que permite las transferencias de información a través de archivos en la web. Está orientado a transacciones y sigue el esquema petición-respuesta entre un cliente y un servidor. El cliente (por ejemplo, un **navegador web**) realiza una petición enviando un mensaje, con un formato específico al servidor. El servidor (comúnmente llamado **servidor web**) le envía un mensaje de respuesta.


### Métodos de petición HTTP implementados

##### GET
El método GET solicita que el recurso de destino transfiera una representación de su estado. Las solicitudes que usan GET solo deben recuperar datos y no deben tener ningún otro efecto.
	
##### POST
El método POST solicita que el recurso de destino procese la representación contenida en la petición. Está orientado a crear un nuevo recurso. Se puede usar, por ejemplo, para publicar un mensaje en un foro de Internet o completar una compra en línea.

##### DELETE
Borra el recurso especificado.


### Códigos de estado HTTP

En el RFC 7231 se establecen los significados de los códigos de respuesta:

    1xx: Informativo
    2xx: Éxito (e.g., 200 OK)
    3xx: Redirección (e.g., 301 Moved Permanently)
    4xx: Error del cliente (e.g., 404 Not Found)
    5xx: Error del servidor (e.g., 500 Internal Server Error)


### Ejemplo de diálogo HTTP

Para obtener un recurso con el URL http://www.example.com/index.html

- Se abre una conexión en el puerto 80 del host www.example.com. El puerto 80 es el puerto predefinido para HTTP. Si se quisiera utilizar el puerto XXXX, habría que codificarlo en la URL de la forma http://www.example.com:XXXX/index.html.

- Se envía un mensaje en el estilo siguiente:

		GET /index.html HTTP/1.1
		Host: www.example.com
		User-Agent: Mozilla/5.0
		Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
		Accept-Language: en-GB,en;q=0.5
		Accept-Encoding: gzip, deflate, br
		Connection: keep-alive
		[Línea en blanco]

- La respuesta del servidor está formada por encabezados seguidos del recurso solicitado, en el caso de una página web:

		HTTP/1.1 200 OK
		Date: Mon, 23 May 2005 22:38:34 GMT
		Content-Type: text/html; charset=UTF-8
		Content-Length: 155
		Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT
		Server: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)
		ETag: "3f80f-1b6-3e1cb03b"
		Accept-Ranges: bytes
		Connection: close

		<html>
		<head>
			<title>An Example Page</title>
		</head>
		<body>
			<p>Hello World, this is a very simple HTML document.</p>
		</body>
		</html>

### API de Sockets

![Diagrama](https://media.geeksforgeeks.org/wp-content/uploads/20220330131350/StatediagramforserverandclientmodelofSocketdrawio2-448x660.png)

(fuente: geeksforgeeks.org)

### Caso Server

1. Creacion del Socket.

		int sockfd = socket(domain, type, protocol)

	- sockfd: descriptor de socket (como un identificador de archivo).
	- domain: dominio de comunicación. AF_LOCAL para la comunicación entre procesos en el mismo host. AF_INET para la comunicación entre procesos en diferentes hosts conectados por IPv4.
	- type: tipo de comunicación. SOCK_STREAM: TCP (confiable, orientado a la conexión). SOCK_DGRAM: UDP (no confiable, sin conexión).
	- protocol: valor de protocolo para el Protocolo de Internet (IP), que es 0.

2. Enlazar el socket

		int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)

	Vincula el socket a la dirección y el número de puerto especificados en addr (estructura de datos personalizada). Si vinculamos el servidor al host local, usamos INADDR_ANY para especificar la dirección IP.

		addr
		– struct sockaddr_in para IPv4
		– cast (struct sockaddr_in*) a (struct sockaddr*)
		struct sockaddr_in
		{
			short sin_family; // ejemplo AF_INET
			unsigned short sin_port; // ejemplo htons(3490)
			struct in_addr sin_addr; // ver struct in_addr
			char sin_zero[8];
		};
		struct in_addr
		{
			unsigned long s_addr;
		};

	Se usa htons() (Host TO Network Short), convierte el número de puerto de orden de bytes del host a orden de bytes de red (Big Endian). Es necesario porque diferentes arquitecturas pueden almacenar enteros en diferentes ordenamientos (endianness), y la red usa Big Endian por convención. Por otro lado, no se usa htonl(INADDR_ANY) porque es innecesario; INADDR_ANY ya está en el orden de bytes correcto.

3. Escuchar conexiones

		int listen(int sockfd, int backlog)

	Pone el socket en modo pasivo, donde espera a que el cliente establezca una conexión. Backlog define la longitud máxima hasta la cual puede crecer la cola de conexiones pendientes para sockfd.

4. Aceptar conexiones

		int new_socket = accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)

	Extrae la primera solicitud de conexión en la cola de conexiones pendientes para el socket de escucha y crea un nuevo socket conectado (devolviendo su descriptor de archivo). En este punto, se establece la conexión cliente-servidor y están listos para transferir datos.

### Multiplexación I/O

Es la capacidad de realizar operaciones de E/S en múltiples descriptores de archivos. Las operaciones de entrada como read() o accept() se bloquean cuando no hay datos entrantes. Para evitar esto, se proporcionan llamadas de multiplexación de E/S como select() o poll(). En nuestro caso usamos poll().

Un proceso se bloquea en una llamada de poll(), cuando regresa esta llamada, el proceso recibe un conjunto de descriptores de archivos que están listos para E/S. De esta forma se pueden realizar operaciones E/S en estos descriptores de archivos antes de pasar a la siguiente iteración.

	int poll(struct pollfd *fds, nfds_t nfds, int timeout)

Especifica un vector de nfds estructuras del tipo struct pollfd y un tiempo límite timeout en milisegundos.

	struct pollfd {
        int fd;         /* Descriptor de archivo */
        short events;   /* Eventos a monitorear */
    	short revents;  /* Eventos ocurridos (se llena cuando poll() retorna) */
	};

Los bits posibles en las máscaras (events y revents) están definidos por:

	#define POLLIN    0x0001  /* Hay datos que leer */
    #define POLLPRI   0x0002  /* Hay datos urgentes que leer */
    #define POLLOUT   0x0004  /* Se puede escribir sin bloquear */
    #define POLLERR   0x0008  /* Condición de error */
	#define POLLHUP   0x0010  /* Colgado */
	#define POLLNVAL  0x0020  /* Petición inválida: fd no está abierto */

Las funciones como poll() permiten que nuestro programa espere eventos en uno o varios sockets de red y actúe en consecuencia. Por ejemplo, un servidor HTTP necesita saber cuándo:
- Hay datos disponibles para leer de un cliente.
- Puede escribir datos a un cliente sin bloquear.
- Una conexión se cierra.


### Referencias
- https://www.youtube.com/watch?v=XXfdzwEsxFk
- https://developer.mozilla.org/es/docs/Web/HTTP/Overview
- https://manpages.ubuntu.com/manpages/focal/es/man2/poll.2.html