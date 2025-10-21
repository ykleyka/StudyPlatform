#include "user.h"

bool User::checkPassword(const QString& inPassword) const {
    return inPassword == password;
}
