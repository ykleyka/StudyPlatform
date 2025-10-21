#include "nameduser.h"

QString NamedUser::toFileString() const {
    return this->getRole() + ';' + login + ';' + password + ';' + name;
};
