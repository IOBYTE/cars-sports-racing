#include <string>
#include <stdexcept>
#include "netdriver.h"
#include <robot.h>
#include <playerpref.h>

NetDriver::NetDriver(int index)
{
    void *params = GfParmReadFileLocal(HM_DRV_FILE, GFPARM_RMODE_STD);

    if (!params)
        throw std::invalid_argument("GfParmReadFileLocal(" HM_DRV_FILE ") failed");

    std::string s;

    s += "Robots/index/";
    s += std::to_string(index);

    const char *section = s.c_str();

    if (!GfParmExistsSection(params, section))
    {
        std::string err;

        err += "Expected section ";
        err += s;
        err += " in " HM_DRV_FILE;
        throw std::invalid_argument(err);
    }

    name = GfParmGetStr(params, section, ROB_ATTR_NAME, "");
    sname = GfParmGetStr(params, section, ROB_ATTR_SNAME, "");
    code = GfParmGetStr(params, section, ROB_ATTR_CODE, "");
    car = GfParmGetStr(params, section, ROB_ATTR_CAR, "");
    type = GfParmGetStr(params, section, ROB_ATTR_TYPE, "");
    skill_level = GfParmGetStr(params, section, ROB_ATTR_LEVEL, "");

    race_number = GfParmGetNum(params, section, ROB_ATTR_RACENUM, nullptr, 0);

    r = GfParmGetNum(params, section, ROB_ATTR_RED, nullptr, 1);
    g = GfParmGetNum(params, section, ROB_ATTR_GREEN, nullptr, 1);
    b = GfParmGetNum(params, section, ROB_ATTR_BLUE, nullptr, 1);

    GfParmReleaseHandle(params);
}
