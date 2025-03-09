#!/usr/bin/env python3

import os
import cgi

# Función para leer el cuerpo de la solicitud POST
def get_post_data():
    # Obtener el valor de CONTENT_LENGTH
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    if content_length > 0:
        # Leer el cuerpo del mensaje
        post_data = sys.stdin.read(content_length)
        return post_data
    return ""

# Mostrar los encabezados HTTP para la respuesta
print("Content-Type: text/html; charset=UTF-8")
print()  # Línea en blanco para separar los encabezados del cuerpo de la respuesta

# Mostrar el HTML
print("<html><head><title>CGI POST Example</title></head><body>")
print("<h1>Detalles de la Solicitud POST</h1>")

# Mostrar las variables de entorno
print("<h2>Variables de Entorno:</h2>")
print(f"<p>REQUEST_METHOD: {os.environ.get('REQUEST_METHOD')}</p>")
print(f"<p>CONTENT_LENGTH: {os.environ.get('CONTENT_LENGTH')}</p>")
print(f"<p>SCRIPT_NAME: {os.environ.get('SCRIPT_NAME')}</p>")
print(f"<p>SCRIPT_FILENAME: {os.environ.get('SCRIPT_FILENAME')}</p>")
print(f"<p>QUERY_STRING: {os.environ.get('QUERY_STRING')}</p>")

# Si el método es POST, mostrar el contenido del cuerpo
if os.environ.get('REQUEST_METHOD') == 'POST':
    print("<h2>Cuerpo de la Solicitud POST:</h2>")
    body = os.environ.get('CONTENT_BODY')
    print(f"<pre>{body}</pre>")

print("</body></html>")
