#include <exception>
#include <string>

class ScopException : public std::exception {
protected:
    std::string _msg;
public:
    ScopException(const std::string& msg) : _msg(msg) {}
    virtual const char* what() const noexcept override {
        return _msg.c_str();
    }
};

class FileNotFoundException : public ScopException {
public:
    FileNotFoundException(const std::string& filepath)
        : ScopException("Erreur Fichier : Impossible d'ouvrir le fichier '" + filepath + "'") {}
};

class InvalidExtensionException : public ScopException {
public:
    InvalidExtensionException(const std::string& filepath)
        : ScopException("Erreur Format : Le fichier '" + filepath + "' n'a pas l'extension .obj") {}
};

class ParsingException : public ScopException {
public:
    ParsingException(const std::string& detail)
        : ScopException("Erreur Parsing : " + detail) {}
};