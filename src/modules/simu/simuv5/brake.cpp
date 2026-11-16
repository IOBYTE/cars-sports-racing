/***************************************************************************

    file                 : brake.cpp
    created              : Sun Mar 19 00:05:26 CET 2000
    copyright            : (C) 2000 by Eric Espie
    email                : torcs@free.fr
    version              : $Id: brake.cpp 3948 2011-10-08 07:27:25Z wdbee $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sim.h"

static bool updated = false;
static tdble brksyst;

void
SimBrakeConfig(void *hdle, const char *section, tBrake *brake)
{
    tdble diam, area, width, mu, maxtemp;

    diam     = GfParmGetNum(hdle, section, PRM_BRKDIAM, (char*)NULL, 0.2f);
    area     = GfParmGetNum(hdle, section, PRM_BRKAREA, (char*)NULL, 0.002f);
    width    = GfParmGetNum(hdle, section, PRM_BRKWIDTH, (char*)NULL, 25.00f);
    mu       = GfParmGetNum(hdle, section, PRM_MU, (char*)NULL, 0.30f);
    maxtemp  = GfParmGetNum(hdle, section, PRM_BRKMAXTEMP, (char*)NULL, 500.0f);

    // Option TCL ...
    //if (car->features & FEAT_TCLINSIMU)
    {
        brake->TCL = 0.0f;
    }
    // ... Option TCL
    // Option ABS ...
    //if (car->features & FEAT_ABSINSIMU)
    {
        brake->ABS = 1.0f;
        brake->EnableABS = GfParmGetNum(hdle, section, PRM_ABSINSIMU, (char*)NULL, 0.0f) > 0;
    }

    // ... Option ABS
    brake->coeff = (tdble) (diam * 0.5 * area * mu);
    brake->width = (tdble) width;
    brake->initwidth = (tdble) width;
    brake->I = GfParmGetNum(hdle, section, PRM_INERTIA, (char*)NULL, 0.13f);
    brake->radius = diam / 2.0f;
    brake->temp = 0.0f;
    brake->disktemp = Tair;
    brake->maxtemp = maxtemp;
    brake->tempcoeff = GfParmGetNum(hdle, section, PRM_BRAKTEMPCOEFF, (char*)NULL, 0.00000000030f);

    if ((strcmp(pTrack, "aalborg") == 0) || (strcmp(pTrack, "peters") == 0) || (strcmp(pTrack, "atlanta") == 0))
        brake->tempcoeff = GfParmGetNum(hdle, section, PRM_BRAKTEMPCOEFF2, (char*)NULL, 0.00000000030f);

    GfLogDebug("# Simu track name = %s - brake coefficient = %.10f\n", pTrack, brake->tempcoeff);
}

void
SimBrakeUpdate(tCar *car, tWheel *wheel, tBrake *brake)
{
    brake->Tq = brake->coeff * brake->pressure;

    // Option ABS ...
    if (car->features & FEAT_ABSINSIMU)
    {
        if (brake->EnableABS)
            brake->Tq *= brake->ABS;
    }
    // ... Option ABS

    // Option TCL ...
    if (car->features & FEAT_TCLINSIMU)
    {
        // Brake most spinning wheel
        //tdble TCL_BrakeScale = 125.0f;	// Make it be a parameter later
        brake->Tq += (tdble) MAX(0.0,MIN(5000.0, car->TCL_BrakeScale * brake->TCL)); // Sanity check
        brake->TCL = 0.0; // Reset for next timestep
    }
    // ... Option TCL

    brake->temp -= (tdble) (fabs(car->DynGC.vel.x) * 0.0001 + 0.0002);

    if (brake->temp < 0 )
        brake->temp = 0;

    brake->temp += (tdble) (brake->pressure * brake->radius * fabs(wheel->spinVel) * 0.0000000005);

    if (brake->temp > 1)
        brake->temp = 0;

    if (car->options->brake_damage)
    {
        double air = 38.0 - K2C(Tair);
        brake->disktemp -= (tdble) (fabs(car->DynGC.vel.x) * 0.00008 + (air / 2.0) / 100000.0);

        if (brake->disktemp < 0.0 )
            brake->disktemp = 0.0;

        brake->disktemp += (tdble) (brake->pressure * brake->radius * fabs(wheel->spinVel)
                                    * (brake->tempcoeff));

        if (brake->disktemp < brake->maxtemp)
            brake->width -=(tdble)(brake->pressure * brake->radius * 0.00000000001f);
        else
            brake->width -=(tdble)(brake->pressure * brake->radius * 0.00000000003f);

        if (brake->disktemp > brake->maxtemp + 100.0)
        {
            brake->disktemp = brake->maxtemp + 100.0;
        }

        //GfLogDebug("Brake Temperature = %.2f max temperature = %.2f - brake disk width = %.5f\n", brake->disktemp, brake->maxtemp, brake->width);
    }
}

void
SimBrakeSystemConfig(tCar *car)
{
    void *hdle = car->params;
    tCarSetupItem *setupBrkRep = &(car->carElt->setup.brakeRepartition);
    tCarSetupItem *setupBrkPress = &(car->carElt->setup.brakePressure);

    setupBrkRep->desired_value = setupBrkRep->min = setupBrkRep->max = 0.5;
    GfParmGetNumWithLimits(hdle, SECT_BRKSYST, PRM_BRKREP, (char*)NULL, &(setupBrkRep->desired_value), &(setupBrkRep->min), &(setupBrkRep->max));
    setupBrkRep->changed = true;
    setupBrkRep->stepsize = 0.005f;

    setupBrkPress->desired_value = setupBrkPress->min = setupBrkPress->max = 1000000;
    GfParmGetNumWithLimits(hdle, SECT_BRKSYST, PRM_BRKPRESS, (char*)NULL, &(setupBrkPress->desired_value), &(setupBrkPress->min), &(setupBrkPress->max));
    setupBrkPress->changed = true;
    setupBrkPress->stepsize = 1000;

    car->brkSyst.ebrake_pressure = GfParmGetNum(hdle, SECT_BRKSYST, PRM_EBRKPRESS, (char*)NULL, 0.0);
}

void
SimBrakeSystemReConfig(tCar *car)
{
    /* called by SimCarReConfig() in car.cpp */
    tCarSetupItem *setupBrkRep = &(car->carElt->setup.brakeRepartition);
    tCarSetupItem *setupBrkPress = &(car->carElt->setup.brakePressure);
    GfLogDebug("#Simu brake repartition reconfig value   = %.3f\n", setupBrkRep->value);
    GfLogDebug("#Simu repartition reconfig desired value = %.3f\n", setupBrkRep->desired_value);
    GfLogDebug("#Simu repartition reconfig changed       = %u\n", setupBrkRep->changed);
    GfLogDebug("#Simu repartition reconfig min           = %.3f\n", setupBrkRep->min);
    GfLogDebug("#Simu repartition reconfig max           = %.3f\n", setupBrkRep->max);
    GfLogDebug("#Simu brake Press reconfig value         = %.3f\n", setupBrkPress->value);
    GfLogDebug("#Simu brake Press reconfig desired value = %.3f\n", setupBrkPress->desired_value);
    GfLogDebug("#Simu brake Press reconfig changed       = %u\n", setupBrkPress->changed);
    GfLogDebug("#Simu brake Press reconfig min           = %.3f\n", setupBrkPress->min);
    GfLogDebug("#Simu brake Press reconfig max           = %.3f\n", setupBrkPress->max);

    if (setupBrkRep->changed)
    {
        car->brkSyst.rep = MIN(setupBrkRep->max, MAX(setupBrkRep->min, setupBrkRep->desired_value));
        setupBrkRep->value = car->brkSyst.rep;
        setupBrkRep->changed = FALSE;
        GfLogDebug("# Simu Brake = %.2f\n", setupBrkRep->desired_value);
    }

    if (setupBrkPress->changed)
    {
        car->brkSyst.coeff = MIN(setupBrkPress->max, MAX(setupBrkPress->min, setupBrkPress->desired_value));
        setupBrkPress->value = car->brkSyst.coeff;
        brksyst = car->brkSyst.coeff;
        setupBrkPress->changed = FALSE;
    }
}

void
SimBrakeSystemUpdate(tCar *car)
{
    tBrakeSyst	*brkSyst = &(car->brkSyst);

    if (car->options->brake_damage)
    {
        for (int i = 0; i < 4; i++)
        {
            if (car->wheel[i].brake.width < 20.00f)
            {
                brkSyst->coeff = brkSyst->coeff / 100.0f;
                //GfLogDebug("Brake system coefficient / 100 = %.2f\n", coeff);
            }
            else if (car->wheel[i].brake.disktemp < car->wheel[i].brake.maxtemp)
            {
                brkSyst->coeff = brksyst;
                updated = false;

                //GfLogDebug("Brake system coefficient normal = %.2f\n", coeff);
            }
            else if (car->wheel[i].brake.disktemp > car->wheel[i].brake.maxtemp && updated == false)
            {
                brkSyst->coeff = brkSyst->coeff / 20.0f;
                updated = true;
                //GfLogDebug("Brake system coefficient Alert = %.2f\n", coeff);
            }

        }
    }

    tdble coeff = brkSyst->coeff;

    // Option ESP ...
    if (car->features & FEAT_ESPINSIMU)
    {
        tCarElt	*carElt = car->carElt;
        tdble driftAngle = atan2(carElt->_speed_Y, carElt->_speed_X) - carElt->_yaw;
        FLOAT_NORM_PI_PI(driftAngle);
        tdble absDriftAngle = fabs(driftAngle);

        // Make it be parameters later
        tdble driftAngleLimit = (tdble) (7.5 * PI / 180);       // 7.5 deg activation level
        tdble brakeSide = 0.0025f * driftAngle/driftAngleLimit; // Car side brake command
        tdble brakeBalance = 0.005f;                            // Front/Rear brake command

        if (absDriftAngle > driftAngleLimit)
        {
            car->ctrl->brakeFrontRightCmd -= brakeSide;
            car->ctrl->brakeFrontLeftCmd += brakeSide;
            car->ctrl->brakeRearRightCmd -= brakeBalance + brakeSide;
            car->ctrl->brakeRearLeftCmd -= brakeBalance - brakeSide;
        }

        if (car->ctrl->singleWheelBrakeMode == 1)
        {
            // Sanity check needed
            car->ctrl->brakeFrontRightCmd = (tdble) MIN(1.0,MAX(0.0, car->ctrl->brakeFrontRightCmd));
            car->ctrl->brakeFrontLeftCmd = (tdble) MIN(1.0,MAX(0.0,car->ctrl->brakeFrontLeftCmd));
            car->ctrl->brakeRearRightCmd = (tdble) MIN(1.0,MAX(0.0,car->ctrl->brakeRearRightCmd));
            car->ctrl->brakeRearLeftCmd = (tdble) MIN(1.0,MAX(0.0,car->ctrl->brakeRearRightCmd));

            car->wheel[FRNT_RGT].brake.pressure = coeff * car->ctrl->brakeFrontRightCmd;
            car->wheel[FRNT_LFT].brake.pressure = coeff * car->ctrl->brakeFrontLeftCmd;
            car->wheel[REAR_RGT].brake.pressure = coeff * car->ctrl->brakeRearRightCmd;
            car->wheel[REAR_LFT].brake.pressure = coeff * car->ctrl->brakeRearLeftCmd;
        }
        else
        {
            tdble	ctrl = car->ctrl->brakeCmd;

            if (absDriftAngle > driftAngleLimit)
            {
                // Sanity check needed
                car->wheel[FRNT_RGT].brake.pressure = (tdble) MIN(1.0,MAX(0.0,ctrl - brakeSide));
                car->wheel[FRNT_LFT].brake.pressure = (tdble) MIN(1.0,MAX(0.0,ctrl + brakeSide));
                car->wheel[REAR_RGT].brake.pressure = (tdble) MIN(1.0,MAX(0.0,ctrl - brakeSide - brakeBalance));
                car->wheel[REAR_LFT].brake.pressure = (tdble) MIN(1.0,MAX(0.0,ctrl + brakeSide - brakeBalance));

                car->wheel[FRNT_RGT].brake.pressure *= coeff * brkSyst->rep;
                car->wheel[FRNT_LFT].brake.pressure *= coeff * brkSyst->rep;
                car->wheel[REAR_RGT].brake.pressure *= coeff * (1 - brkSyst->rep);
                car->wheel[REAR_LFT].brake.pressure *= coeff * (1 - brkSyst->rep);
            }
            else
            {
                ctrl *= brkSyst->coeff;
                car->wheel[FRNT_RGT].brake.pressure = car->wheel[FRNT_LFT].brake.pressure = ctrl * brkSyst->rep;
                car->wheel[REAR_RGT].brake.pressure = car->wheel[REAR_LFT].brake.pressure = ctrl * (1 - brkSyst->rep);
            }
        }
    } // ... Option ESP
    else if (car->ctrl->singleWheelBrakeMode == 1)
    {
        car->wheel[FRNT_RGT].brake.pressure = coeff * car->ctrl->brakeFrontRightCmd;
        car->wheel[FRNT_LFT].brake.pressure = coeff * car->ctrl->brakeFrontLeftCmd;
        car->wheel[REAR_RGT].brake.pressure = coeff * car->ctrl->brakeRearRightCmd;
        car->wheel[REAR_LFT].brake.pressure = coeff * car->ctrl->brakeRearLeftCmd;
    }
    else
    {
        tdble	ctrl = car->ctrl->brakeCmd;
        ctrl *= brkSyst->coeff;
        car->wheel[FRNT_RGT].brake.pressure = car->wheel[FRNT_LFT].brake.pressure = ctrl * brkSyst->rep;
        car->wheel[REAR_RGT].brake.pressure = car->wheel[REAR_LFT].brake.pressure = ctrl * (1 - brkSyst->rep);
    }

    if ( (car->ctrl->ebrakeCmd > 0) && (car->wheel[REAR_RGT].brake.pressure < brkSyst->ebrake_pressure) )
    {
        car->wheel[REAR_RGT].brake.pressure = car->wheel[REAR_LFT].brake.pressure = brkSyst->ebrake_pressure;
    }
}
