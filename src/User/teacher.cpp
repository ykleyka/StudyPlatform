#include "teacher.h"

QString Teacher::toFileString() const {
    return NamedUser::toFileString() + ";" + subject;
}
