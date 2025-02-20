#include "../inc/Response.hpp"
#include <sys/wait.h>

/* En esta primera funcion voy a checkear el acceso a la ruta de cgi pero sin tener
en cuenta el query_string que en el caso de lo que esta implementado es el nombre.*/

bool    Response::parseCgi(){
    std::string location_without_query = _real_location;
    size_t pos = location_without_query.find('?');
    if (pos != std::string::npos)
        location_without_query = location_without_query.substr(0, pos);
    if (access(location_without_query.c_str(), X_OK) == 0){
        return true;
    }
    return false;
}

/* Esta es la funcion principal y se divide en 2 partes:
 - Crea variables de entorno interactivas con el archivo a ejecutar
 - similar a la ejecucion de comandos en terminal se ejecuta el script
 redirigiendo la salida al body de la respuesta http.
 
 en caso de cualquier error de funciones se lanza error 500*/

void    Response::HandleCgi(){
    std::stringstream ss;
    ss << _content_length;
    setenv("REQUEST_METHOD", _req_method.c_str(), 1);
    setenv("CONTENT_LENGTH", ss.str().c_str(), 1);
    setenv("SCRIPT_NAME", _real_location.c_str(), 1);
    setenv("SCRIPT_FILENAME", _real_location.c_str(), 1);
    setenv("QUERY_STRING", getQueryString().c_str(), 1);
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        throw ResponseErrorException(500);
    }
    pid_t pid = fork();
    if (pid == -1){
        throw ResponseErrorException(500);
    }
    if (pid == 0){
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        std::string location_without_query = _real_location;
        size_t pos = location_without_query.find('?');
        if (pos != std::string::npos)
            location_without_query = location_without_query.substr(0, pos);
        execlp(location_without_query.c_str(), location_without_query.c_str(), (char*)NULL);
    } else {
        close(pipefd[1]);
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            char buffer[1024];
            std::string response_body;
            ssize_t bytes_read;
            while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
                response_body.append(buffer, bytes_read);
            }
            _body = response_body;
            _content_length = _body.size();
            _status_code = 200;
            _status_message = "CGI";
            _content_type = "text/html";
            SetResponse(true);
        } else {
            throw ResponseErrorException(500);
        }
        close(pipefd[0]);
    }
}

std::string Response::getQueryString() {
    std::string querystring = _real_location;
    size_t pos = querystring.find('?');
    if (pos != std::string::npos) {
        return querystring.substr(pos + 1); // Retorna todo lo que está después del '?'
    }
    return _real_location;
}