#include "test.h"

Test::Test(TestInfo* info) {
    testId = info->getId();
    title = info->getTitle();
    subject = info->getSubject();
    teacherLogin = info->getTeacherLogin();
}
