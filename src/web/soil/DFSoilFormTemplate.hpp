#ifndef DFSOIL_FORM_TEMPLATE_H
#define DFSOIL_FORM_TEMPLATE_H

#include "web/SettingsFormTemplate.hpp"
#include "constants/DFSoilConstants.hpp"
#include "sensors/soil/DFSoil.hpp"
#include <Arduino.h>

using namespace Constants;
using namespace Sensors::Soil;

namespace Web {

    class DFSoilFormTemplate : public SettingsFormTemplate {
    public:
        DFSoilFormTemplate(const char *postUri, DFSoil &soil);
        virtual int getForm(char *out, size_t len) override;
    private:
        const char *_postUri;
        DFSoil &_soil;
    };

}

#endif