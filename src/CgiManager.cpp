#include "../inc/Response.hpp"
#include <sys/wait.h>

/* En esta primera funcion voy a checkear el acceso a la ruta de cgi pero sin tener
en cuenta el query_string que en el caso de lo que esta implementado es el nombre.*/

int    Response::parseCgi(){
    std::string cgiPath = _real_location;
    size_t pos = cgiPath.find('?');
    if (pos != std::string::npos) {
        cgiPath = cgiPath.substr(0, pos);
    }
    if (access(cgiPath.c_str(), X_OK) == -1){
        throw Response::ResponseErrorException(404);
    }
    return 1;
}

void Response::timeout_handler(int sig){
    if (sig)
        throw Response::ResponseErrorException(504);
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
    exit(1);
	} else {
		close(pipefd[1]);

		int status;
		int elapsed_time = 0;
		const int timeout = 5;

		while (elapsed_time < timeout) {
			int ret = waitpid(pid, &status, WNOHANG);

			if (ret == -1) {
				kill(pid, SIGKILL);
				throw Response::ResponseErrorException(505);
			}
			if (ret > 0) {
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
					_status_message = "OK";
					_content_type = "text/html";
					SetResponse(true);
					close(pipefd[0]);
					return;
				} else {
					kill(pid, SIGKILL);
					throw Response::ResponseErrorException(500);
				}
			}

			sleep(1);
			elapsed_time++;
		}

		kill(pid, SIGKILL);
		throw Response::ResponseErrorException(504);

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

bool    Response::validExt(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}
