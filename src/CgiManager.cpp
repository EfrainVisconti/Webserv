#include "../inc/Response.hpp"
#include <sys/wait.h>

/* En esta primera funcion voy a checkear el acceso a la ruta de cgi pero sin tener
en cuenta el query_string que en el caso de lo que esta implementado es el nombre.*/

void    Response::parseCgi(){
    std::string cgiPath = _real_location;
    size_t pos = cgiPath.find('?');
    if (pos != std::string::npos) {
        cgiPath = cgiPath.substr(0, pos);
    }
    if (!cgiPath.empty() && cgiPath[0] == '/') {
        cgiPath.erase(0, 1);
    }
    if (access(cgiPath.c_str(), X_OK) == -1 || cgiPath.size() <= 14){
        throw Response::ResponseErrorException(404);
    }
}

/* Esta es la funcion principal y se divide en 2 partes:
 - Crea variables de entorno interactivas con el archivo a ejecutar
 - similar a la ejecucion de comandos en terminal se ejecuta el script
 redirigiendo la salida al body de la respuesta http.
 
 en caso de cualquier error de funciones se lanza error 500*/

void Response::HandleCgi() {
    std::stringstream ss;
    ss << _content_length;
    setenv("REQUEST_METHOD", _req_method.c_str(), 1);
    setenv("CONTENT_LENGTH", ss.str().c_str(), 1);
    setenv("SCRIPT_NAME", _real_location.c_str(), 1);
    setenv("SCRIPT_FILENAME", _real_location.c_str(), 1);
    setenv("QUERY_STRING", getQueryString().c_str(), 1);
    if (_req_method == "POST"){
        std::string body(_request->getBody().begin(), _request->getBody().end());
        setenv("CONTENT_BODY", body.c_str(), 1);
    }
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        throw ResponseErrorException(500);
    }
    pid_t pid = fork();
    if (pid == -1) {
        throw ResponseErrorException(500);
    }
    if (pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        std::string location_without_query = _real_location;
        size_t pos = location_without_query.find('?');
        if (pos != std::string::npos) {
            location_without_query = location_without_query.substr(0, pos);
        }
        if (!location_without_query.empty() && location_without_query[0] == '/') {
            location_without_query.erase(0, 1);
        }
        char* argv[] = { const_cast<char*>(location_without_query.c_str()), NULL };
        extern char** environ;
        execve(location_without_query.c_str(), argv, environ);
        throw Response::ResponseErrorException(500);
    } else {
        close(pipefd[1]);
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
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
            close(pipefd[0]);
            throw ResponseErrorException(500);
        }

        close(pipefd[0]);
    }
}

std::string Response::getQueryString() {
    std::string querystring = _real_location;
    size_t pos = querystring.find('?');
    if (pos != std::string::npos) {
        return querystring.substr(pos + 1);
    }
    return _real_location;
}