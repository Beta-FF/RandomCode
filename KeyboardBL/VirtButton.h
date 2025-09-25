#pragma once
#include <Arduino.h>

class EasyButton {
    public:
        bool check(bool newSate) {
            if (newSate && !_flag) {  // нажатие
                _flag = true;
                return false;
            }
            if (!newSate && _flag) {  // отпускание
                _flag = false;
                return true; //срабатывание по отпусканию
            }
            return false;
        }

    private:
        bool _flag = false;
};