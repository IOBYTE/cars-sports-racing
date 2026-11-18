/***************************************************************************

    file                 : pit.cpp
    created              : Thu Aug 31 01:21:49 UTC 2006
    copyright            : (C) 2006 Daniel Schellhammer

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "pit.h"

#include "Utils.h"

#include <algorithm>
#include <cmath>

namespace
{
// Width of a (main, border or side) segment at a fraction of its length.
// RtTrackGetWidth wants the distance into the segment in the segment's own
// units: meters on straights, radians on turns.
double segWidthAt(const tTrackSeg* seg, double frac)
{
    double toStart = frac * (seg->type == TR_STR ? seg->length : seg->arc);

    return RtTrackGetWidth(const_cast<tTrackSeg*>(seg), (tdble)toStart);
}

// Zero-slope Hermite blend used by Spline between two points.
double smoothStep(double t)
{
    return t * t * (3.0 - 2.0 * t);
}

// Distance from start wrapped into [0, tracklen). The race engine can report
// a negative _distFromStartLine just before the line; that is a position near
// the end of the lap, not near its start (fabs() would mirror it).
double lapPos(double fromstart, double tracklen)
{
    double d = std::fmod(fromstart, tracklen);

    if (d < 0.0)
    {
        d += tracklen;
    }

    return d;
}

// True when fromstart is at or after 'start' and less than 'len' metres past
// it along the lap, handling the start / finish line wrap ('start' may be
// negative or past the end of the lap).
bool inLapWindow(double fromstart, double start, double len, double tracklen)
{
    return lapPos(fromstart - start, tracklen) < len;
}
}

Pit::Pit() :
    mTrack(NULL),
    mMyCar(NULL),
    mCar(NULL),
    mTeamCar(NULL),
    mPit(NULL),
    mPitInfo(NULL),
    mAvgFuelPerLap(10.0),
    mAvgFuelPerMeter(0.00007),
    mTiresChangeTime(30.0),
    mMaxDamage(7000.0),
    mMaxDamageDist(30000.0),
    mPreEntryMargin(100.0)
{
}

Pit::~Pit()
{
}

void Pit::init(const tTrack* track, tSituation* situation, CarParams* car,
               int pitdamage, double pitgripfactor, double pitentrymargin, int rain,
               double pitleft, double pitright)
{
    if (pitdamage)
    {
        mMaxDamage = pitdamage;
    }
    else
    {
        mMaxDamage = 5000;
    }

    if (pitgripfactor)
    {
        mPitGripFactor = pitgripfactor;
    }
    else
    {
        mPitGripFactor = 0.80;
    }

    mRain = rain;

    mEntryMargin = pitentrymargin;
    mTrack = track;
    mMyCar = car;
    mCar = (tCarElt*)mMyCar->car();
    mPit = mCar->_pit;
    mPitInfo = &mTrack->pits;
    mPitstop = mInPitLane = mDriveThrough = false;
    mFuelChecked = false;
    mStopChecked = false;
    mTotalFuel = 0.0;
    mFuelLapsCounted = 0;
    mMaxFuel = mMyCar->MaxFuel();
    mAvgFuelPerLap = mMyCar->avgFuelPerMeter() * mTrack->length;
    mLastPitFuel = 0.0;
    mLastFuel = 0.0;
    mPenalty = 0;
    mPitLeft = pitleft;
    mPitRight = pitright;
    mSituation = situation;

    // Get teammates car

    for (int i = 0; i < situation->_ncars; i++)
    {
        if (situation->cars[i] != mCar)
        {
            if (!strncmp(mCar->_teamname, situation->cars[i]->_teamname, 10))
            {
                mTeamCar = situation->cars[i];
            }
        }
    }

    if (mPit != NULL)
    {
        mSpeedLimit = mPitInfo->speedLimit - 0.5;
        // Compute pit spline points along the track
        mPitp[3].x = mPit->pos.seg->lgfromstart + mPit->pos.toStart;
        mPitp[2].x = mPitp[3].x - (mPitInfo->len);
        mPitp[4].x = mPitp[3].x + mPitInfo->len;
        mPitp[0].x = mPitInfo->pitEntry->lgfromstart;
        mPitp[1].x = mPitInfo->pitStart->lgfromstart - mPitInfo->len;
        mPitp[5].x = mPitInfo->pitEnd->lgfromstart + mPitInfo->pitEnd->length + mPitInfo->len;
        //mPitp[5].x = mPitInfo->pitEnd->lgfromstart + mPitInfo->nPitSeg * mPitInfo->len;
        mPitp[6].x = mPitInfo->pitExit->lgfromstart + mPitInfo->pitExit->length;
        //mPitp[6].x = mPitInfo->pitExit->lgfromstart;
        // Drive through: entry, end of entry, pit lane start, pit lane end,
        // start of exit, exit. The extra lane points keep the car in the lane
        // when the end of entry / start of exit get pulled in off the grass.
        mDtp[0].x = mPitp[0].x;
        mDtp[1].x = mPitp[1].x;
        mDtp[2].x = mPitInfo->pitStart->lgfromstart;
        mDtp[3].x = mPitInfo->pitEnd->lgfromstart + mPitInfo->pitEnd->length;
        mDtp[4].x = mPitp[5].x;
        mDtp[5].x = mPitp[6].x;
        mPitEntry = mPitp[0].x;
        mPitExit = mPitp[6].x;
        mLimitEntry = mPitp[1].x;
        mLimitExit = mPitp[5].x;
        // Normalizing spline segments to <= 0.0

        for (int i = 0; i < PITPOINTS; i++)
        {
            mPitp[i].s = 0.0;
            mPitp[i].x = toSplineCoord(mPitp[i].x);
        }

        for (int i = 0; i < DTPOINTS; i++)
        {
            mDtp[i].s = 0.0;
            mDtp[i].x = toSplineCoord(mDtp[i].x);
        }

        mPitp[1].x = std::min(mPitp[1].x, mPitp[2].x);
        mPitp[5].x = std::max(mPitp[4].x, mPitp[5].x);

        // Broken pit exit: the start of the exit is at or past the end of
        // the pit exit segment. Keep point 6 at the end of the pit exit and
        // move point 5 back between its neighbours so the car still blends
        // back to the track edge. If there is no room (last pit box and a
        // very short pit exit segment), fall back to collapsing point 6.
        if (mPitp[6].x <= mPitp[5].x)
        {
            if (mPitp[4].x < mPitp[6].x)
            {
                mPitp[5].x = (mPitp[4].x + mPitp[6].x) / 2.0;
            }
            else
            {
                mPitp[6].x = mPitp[5].x;
            }
        }

        if (mDtp[5].x <= mDtp[4].x)
        {
            if (mDtp[3].x < mDtp[5].x)
            {
                mDtp[4].x = (mDtp[3].x + mDtp[5].x) / 2.0;
            }
            else
            {
                mDtp[5].x = mDtp[4].x;
            }
        }

        // End of the drive through entry wraps to almost a full lap when the
        // pit entry is less than a pit length before the pit start. Put it
        // back between its neighbours so the spline points stay in order.
        if (mDtp[1].x >= mDtp[2].x)
        {
            mDtp[1].x = (mDtp[0].x + mDtp[2].x) / 2.0;
        }

        mPitOnLeftSide = mPitInfo->side == TR_LFT;
        double sign = mPitOnLeftSide ? 1.0 : -1.0;
        // 2 m inside the edge of the actual segment at the pit entry start and
        // the pit exit end, not the track's default width.
        mPitp[0].y = sign * (segWidthAt(mPitInfo->pitEntry, 0.0) / 2.0 - 2.0);
        mPitp[6].y = sign * (segWidthAt(mPitInfo->pitExit, 1.0) / 2.0 - 2.0);
        //mPitp[0].y = 0.0;
        //mPitp[6].y = 0.0;
        mDtp[0].y = mPitp[0].y;
        mDtp[DTPOINTS - 1].y = mPitp[6].y;

        for (int i = 1; i < PITPOINTS - 1; i++)
        {
            mPitp[i].y = sign * (fabs(mPitInfo->driversPits->pos.toMiddle) - mPitInfo->width + mPitLeft);
            GfLogDebug("#mPitp[%i].y = %.3f\n", i, mPitp[i].y);
        }

        for (int i = 1; i < DTPOINTS - 1; i++)
        {
            mDtp[i].y = sign * (fabs(mPitInfo->driversPits->pos.toMiddle) - mPitInfo->width + mPitLeft);
            GfLogDebug("#mDtp[%i].y = %.3f\n", i, mDtp[i].y);
        }

        mPitp[3].y = sign * (fabs(mPitInfo->driversPits->pos.toMiddle) + mPitRight);
        GfLogDebug("#mPitp[3].y = %.3f\n", mPitp[3].y);

        // Keep the entry and exit transitions on the paved part of the track.
        // The lateral target at the end of the pit entry and the start of the
        // pit exit is limited so the whole blend from / to the track edge
        // stays inside the drivable width (no grass, sand or gravel).
        double margin = mCar->_dimension_y / 2.0 + 0.5;
        double edge = fabs(mPitp[0].y);

        double a = limitTransition(mPitp[0].x, edge, mPitp[1].x, fabs(mPitp[1].y), margin);
        mPitp[1].y = sign * a;
        a = limitTransition(mPitp[6].x, fabs(mPitp[6].y), mPitp[5].x, fabs(mPitp[5].y), margin);
        mPitp[5].y = sign * a;

        a = limitTransition(mDtp[0].x, edge, mDtp[1].x, fabs(mDtp[1].y), margin);
        mDtp[1].y = sign * a;
        a = limitTransition(mDtp[5].x, fabs(mDtp[5].y), mDtp[4].x, fabs(mDtp[4].y), margin);
        mDtp[4].y = sign * a;

        GfLogDebug("#Pit drivable limits: mPitp[1].y = %.3f mPitp[5].y = %.3f mDtp[1].y = %.3f mDtp[4].y = %.3f\n",
                   mPitp[1].y, mPitp[5].y, mDtp[1].y, mDtp[4].y);

        mPitSpline.init(PITPOINTS, mPitp);
        mDtSpline.init(DTPOINTS, mDtp);
    }
}

// Transforms track coordinates to spline parameter coordinates
double Pit::toSplineCoord(double x) const
{
    x -= mPitEntry;

    while (x < 0.0)
    {
        x += mTrack->length;
    }

    return x;
}

// Finds the main segment at a distance from start and the distance into it.
const tTrackSeg* Pit::segAt(double fromstart, double& toStart) const
{
    while (fromstart < 0.0)
    {
        fromstart += mTrack->length;
    }

    while (fromstart >= mTrack->length)
    {
        fromstart -= mTrack->length;
    }

    const tTrackSeg* seg = mTrack->seg;

    for (int i = 0; i < mTrack->nseg; i++)
    {
        if (fromstart >= seg->lgfromstart && fromstart < seg->lgfromstart + seg->length)
        {
            toStart = fromstart - seg->lgfromstart;
            return seg;
        }

        seg = seg->next;
    }

    toStart = 0.0;
    return mTrack->seg;
}

// Distance from the track middle to the outer edge of the drivable surface
// on the pit side: the main track plus any border / side that is paved,
// stopping at the first grass, sand or gravel. A wall border does not end
// the drivable area: a pit lane can be walled off from the track and the
// car drives on the side between the walls, so the wall counts only if
// there is pavement beyond it.
double Pit::drivableToMiddle(double fromstart) const
{
    double toStart = 0.0;
    const tTrackSeg* seg = segAt(fromstart, toStart);
    double frac = seg->length > 0.0 ? Utils::clip(toStart / seg->length, 0.0, 1.0) : 0.0;
    int side = mPitOnLeftSide ? TR_SIDE_LFT : TR_SIDE_RGT;

    double extent = segWidthAt(seg, frac) / 2.0;
    double wallWidth = 0.0;
    const tTrackSeg* s = seg->side[side];

    // At most a border and a side.
    for (int i = 0; i < 2 && s != NULL; i++)
    {
        double w = segWidthAt(s, frac);
        bool border = (s->type2 == TR_LBORDER || s->type2 == TR_RBORDER);

        if (border && s->style == TR_WALL)
        {
            wallWidth += w;
        }
        // Same off-track rule as Driver::stateOfftrack (friction below 1.0),
        // using the dry value so wet asphalt still counts as drivable.
        else if (s->surface->kFrictionDry < 1.0)
        {
            break;
        }
        else
        {
            extent += wallWidth + w;
            wallWidth = 0.0;
        }

        s = s->side[side];
    }

    return extent;
}

// Returns the largest outward offset (<= aFree) for the spline point at xFree
// such that the blend between the fixed point (xFixed, aFixed) and it stays
// at least 'margin' inside the drivable edge everywhere along the way.
// x values are spline coordinates, a values are distances from the middle
// towards the pit side.
double Pit::limitTransition(double xFixed, double aFixed, double xFree, double aFree, double margin) const
{
    double len = fabs(xFree - xFixed);
    int n = std::max(2, (int)ceil(len / 0.5));
    double limit = aFree;

    for (int i = 1; i <= n; i++)
    {
        double t = (double)i / n;
        double fs = xFixed + (xFree - xFixed) * t + mPitEntry;
        double maxa = drivableToMiddle(fs) - margin;
        double w = smoothStep(t);

        if (w > 0.001)
        {
            limit = std::min(limit, (maxa - aFixed * (1.0 - w)) / w);
        }
    }

    // Never pull the point in past the fixed (on track) point.
    return std::max(limit, std::min(aFree, aFixed));
}

// Computes offset to track middle for trajectory
double Pit::pathToMiddle(double fromstart) const
{
    if (mPit != NULL)
    {
        if (mInPitLane || ((mPitstop || mPenalty == RM_PENALTY_DRIVETHROUGH) && isBetween(fromstart)))
        {
            fromstart = toSplineCoord(fromstart);

            if (mPenalty == RM_PENALTY_DRIVETHROUGH)
            {
                return mDtSpline.evaluate(fromstart);   
            }
            else
            {
                return mPitSpline.evaluate(fromstart);
            }
        }
        else if ((mPitstop || mPenalty == RM_PENALTY_DRIVETHROUGH) && isBetween(fromstart + mEntryMargin))
        {
            return mPitp[0].y;
        }
    }

    return 0.0;
}

void Pit::setPitstop(bool pitstop)
{
    if (mPit == NULL)
    {
        return;
    }

    if (!pitstop)
    {
        // Also withdraw the pit request. The race engine only clears it when
        // the car stops in its box, so after a drive through (no stop) it would
        // stay set and keep the teammate from pitting.
        mPitstop = false;
        mCar->_raceCmd &= ~RM_CMD_PIT_ASKED;
        return;
    }

    if (!isBetween(mCar->_distFromStartLine) && !isBetween(mCar->_distFromStartLine + mEntryMargin + mPreEntryMargin))
    {
        if (mTeamCar != NULL && !(mTeamCar->_state & RM_CAR_STATE_OUT))
        {
            if ((mTeamCar->_raceCmd & RM_CMD_PIT_ASKED) || (mTeamCar->_state & RM_CAR_STATE_PIT))
            {
                return;
            }
        }

        mCar->_raceCmd |= RM_CMD_PIT_ASKED;
        mPitstop = true;
    }
}

bool Pit::isBetween(double fromstart) const
{
    fromstart = lapPos(fromstart, mTrack->length);

    if (mPitEntry <= mPitExit)
    {
        if (fromstart >= mPitEntry && fromstart <= mPitExit)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if ((fromstart >= 0.0 && fromstart <= mPitExit) || (fromstart >= mPitEntry && fromstart <= mTrack->length))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool Pit::isPitLimit(double fromstart)
{
    if (mLimitEntry <= mLimitExit)
    {
        if (fromstart >= mLimitEntry && fromstart <= mLimitExit)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if ((fromstart >= 0.0 && fromstart <= mLimitExit) || (fromstart >= mLimitEntry && fromstart <= mTrack->length))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void Pit::update()
{
    if (mPit == NULL)
    {
        return;
    }

    double fs = lapPos(mCar->_distFromStartLine, mTrack->length); // _distFromStartLine can be negative
    updateInPitLane(fs);
    updateFuel(fs);
    int remaininglaps = mCar->_remainingLaps - mCar->_lapsBehindLeader;

    if (mSituation->_totTime > mSituation->currentTime)
    {
        // In practice / qualifying a lap that hit a wall or cut a corner is not
        // kept as best lap, so _bestLapTime can still be 0 here : fall back to
        // the last lap time, and with neither, treat it like the first two laps.
        double laptime = mCar->_bestLapTime > 0.0 ? mCar->_bestLapTime : mCar->_lastLapTime;

        if (mCar->_laps > 2 && laptime > 0.0)
            remaininglaps += ( mSituation->_totTime - mSituation->currentTime ) / laptime;
        else
        {
            // It has a pit stop in the first two laps. Normally we don't want to refuel.
            // This will cause for at least sufficient fuel.
            remaininglaps += 1;
        }
    }

    if (mPitstop || remaininglaps == 0)
    {
        return;
    }

    mPenalty = 0; // fuel, damage and tires served before penalty
    bool pittyres = false, pitfuel = false, pitdamage = false, enginetemperature = false;
    // Check for fuel, damage and tyres
    pitfuel = (mCar->_fuel < (mAvgFuelPerLap * 1.2) || mCar->_fuel < (mAvgFuelPerMeter * mTrack->length * 1.2));
    pitdamage = (mCar->_dammage > mMaxDamage && remaininglaps * mTrack->length > mMaxDamageDist && mLastFuel > 15.0) || (mCar->_dammage > mMaxDamage);
    enginetemperature = (mCar->_engineTempWater > mCar->_engineMaxTempWater + 2.0f);

    // Teammates share one pit box and setPitstop() refuses a stop while the
    // teammate has asked for or is in the pit. If both run short of fuel on the
    // same lap and are close together on track, the second car is refused and
    // can run dry before its next chance. So when both will need fuel within
    // about two laps, the teammate isn't pitting yet, and they are closer in
    // time than one stop blocks the box, go a lap early: whoever gets here
    // first pits first, the other one pits next lap with the box free again.
    bool pitfuelearly = false;

    if (!pitfuel && mTeamCar != NULL && !(mTeamCar->_state & RM_CAR_STATE_OUT))
    {
        const double soonfuel = mAvgFuelPerLap * 2.2;
        const int materemaininglaps = mTeamCar->_remainingLaps - mTeamCar->_lapsBehindLeader;
        const bool ineedsoon = (mCar->_fuel < soonfuel) && (mCar->_fuel < remaininglaps * mAvgFuelPerLap);
        const bool mateneedsoon = (mTeamCar->_fuel < soonfuel) && (mTeamCar->_fuel < materemaininglaps * mAvgFuelPerLap);
        const bool matepitting = (mTeamCar->_raceCmd & RM_CMD_PIT_ASKED) || (mTeamCar->_state & RM_CAR_STATE_PIT);

        if (ineedsoon && mateneedsoon && !matepitting)
        {
            // Gap to the teammate along the lap, ahead or behind, in seconds.
            double gapdist = lapPos(mTeamCar->_distFromStartLine - mCar->_distFromStartLine, mTrack->length);
            gapdist = std::min(gapdist, mTrack->length - gapdist);
            double laptime = mCar->_bestLapTime > 0.0 ? mCar->_bestLapTime : mTeamCar->_bestLapTime;
            double gaptime = laptime > 0.0 ? gapdist / mTrack->length * laptime : 0.0; // no lap time yet: assume close

            // How long one stop keeps the box busy: decision point to the box at
            // the pit speed limit, plus the service time (same estimate as
            // pitBeforeTeammate()), plus a margin.
            double approachtime = (mEntryMargin + mPreEntryMargin + mPitp[3].x) / std::max(mSpeedLimit, 1.0);
            double damage = std::max(mCar->_dammage, mTeamCar->_dammage);
            double blocktime = approachtime + 15.0 + 0.007 * damage + mTiresChangeTime + 10.0;

            pitfuelearly = gaptime < blocktime;
        }
    }
    double tires = ((mMyCar->wheels()->TyreTreadDepth() / 100) / mMyCar->wheels()->avgWearPerMeter());
    double dist = mTrack->length * (remaininglaps + 1);

    if(mMyCar->wheels()->TyreTreadDepth() < 10)
    {
        if(tires < dist)
            pittyres = true;
        else
            pittyres = false;
    }

    LogAXIOM.debug(" # Tyre depth = %.2f Pit Tyres change = %i\n", mMyCar->wheels()->TyreTreadDepth(), pittyres);

    // Decide 3 m before (pit entry - entry margin - pre entry margin) and re-arm
    // at (pit entry - entry margin). Both points can fall before the start line
    // when the pit entry is close after it, so wrap them around the lap.
    const double decisionPoint = mPitEntry - mEntryMargin - mPreEntryMargin;
    const double rearmPoint = mPitEntry - mEntryMargin;

    if (inLapWindow(fs, decisionPoint - 3.0, 3.0, mTrack->length) && !mStopChecked)
    {
        if (pitBeforeTeammate(remaininglaps))
        {
            setPitstop(true);
            LogAXIOM.debug(" # Strategy pitbefore Team Mate !\n");
        }
        else if ((mCar->race.laps > 1) && (pitfuel || pitfuelearly || pitdamage || pittyres || enginetemperature))
        {
            setPitstop(true);

            if (!mPitstop)
            {
                // Refused: teammate has asked for or is in the shared pit.
                LogAXIOM.info(" # %s : pit stop refused, teammate using the pit (fuel %.2f, %.2f per lap)\n",
                              mCar->_name, mCar->_fuel, mAvgFuelPerLap);
            }

            LogAXIOM.debug(" # Strategy pit stop asked (early for teammate = %i)\n", pitfuelearly);
            LogAXIOM.debug(" # Tyre depth = %.2f Pit Tyres change = %i\n", mMyCar->wheels()->TyreTreadDepth(), pittyres);
            LogAXIOM.debug(" # Pit Damage = %i\n", pitdamage);
            LogAXIOM.debug(" # Pit Fuel = %.2f - Fuel Per Meters = %.5f - Fuel per Lap = %.2f - Remaning Lap = %i - AVG Fuel = %.2f\n",
                          mAvgFuelPerLap + 2.0, mAvgFuelPerMeter, mAvgFuelPerLap, mCar->_remainingLaps, mAvgFuelPerLap * mCar->_remainingLaps * 1.1);
        }
        else if (pitForPenalty())
        {
            setPitstop(true);
        }

        mStopChecked = true;
    }
    else if (inLapWindow(fs, rearmPoint, 3.0, mTrack->length))
    {
        mStopChecked = false;
    }
}

bool Pit::pitBeforeTeammate(int remaininglaps) const
{
    bool teamcarrunning = (mTeamCar != NULL) && !(mTeamCar->_state & RM_CAR_STATE_OUT) && (mTeamCar->_bestLapTime > 0.0);
    double teamcarfuel;

    if (teamcarrunning)
    {
        teamcarfuel = mTeamCar->_fuel;
    }

    if (teamcarrunning && (mCar->_fuel < teamcarfuel) && (mCar->_fuel < remaininglaps * mAvgFuelPerLap))
    {
        double matelaps = teamcarfuel / mAvgFuelPerLap;
        // Distance the teammate still has to drive to reach (pit entry - entry
        // margin), wrapped around the start line.
        double matetoentry = (mPitEntry - mEntryMargin) - mTeamCar->_distFromStartLine;

        if (matetoentry < 0.0)
        {
            matetoentry += mTrack->length;
        }
        else if (matetoentry >= mTrack->length)
        {
            matetoentry -= mTrack->length;
        }

        double matefulllaps = floor(matelaps - matetoentry / mTrack->length);
        double matetime = (matefulllaps + matetoentry / mTrack->length) * mTeamCar->_bestLapTime;
        double maxstoptime = 15.0 + 0.007 * mCar->_dammage + mTiresChangeTime;
        double mytime = mCar->_bestLapTime + maxstoptime + 60.0;
        bool matepitasked = (mTeamCar->_raceCmd & RM_CMD_PIT_ASKED) != 0;
        bool matestop = mTeamCar->_state & RM_CAR_STATE_PIT;

        if (matetime < mytime && !(matepitasked || matestop))
        {
            return true;
        }
    }

    return false;
}

bool Pit::pitForPenalty()
{
    // Do we need to serve a penalty
    tCarPenalty *penalty = GF_TAILQ_FIRST(&(mCar->_penaltyList));

    if (penalty)
    {
        if (penalty->penalty == RM_PENALTY_DRIVETHROUGH || penalty->penalty == RM_PENALTY_STOPANDGO)
        {
            // Rudimentary strategy here - always serving the penalty straightaway.
            // there's almost certainly more clever ways of doing this, as there's
            // five laps after the penalty in which to serve it before the car is
            // eliminated.
            mPenalty = penalty->penalty;
            mDriveThrough = mCar->_penaltyDrivethrough;
            LogAXIOM.debug("# Penalty type = %i\n", mPenalty);

            return true;
        }
    }

    return false;
}

void Pit::updateDriveThrough()
{
    double fs = lapPos(mCar->_distFromStartLine, mTrack->length);
    mDriveThrough = mCar->_penaltyDrivethrough;

    if((fs > mPitExit - 30.0 && fs < mPitExit - 29.0) && mDriveThrough == true)
    {
        setPitstop(true);
        LogAXIOM.debug("# Update DriveThrough penalty = %i\n", mCar->_penaltyDrivethrough);
    }
    else if((fs > mPitExit - 30.0 && fs < mPitExit - 29.0) && mDriveThrough == false)
    {
        setPitstop(false);
    }
}

void Pit::updateInPitLane(double fromstart)
{
    if (isBetween(fromstart))
    {
        if (mPitstop || mPenalty == RM_PENALTY_DRIVETHROUGH)
        {
            mInPitLane = true;
        }
    }
    else
    {
        mInPitLane = false;
    }
}

void Pit::updateFuel(double fromstart)
{
    if (fromstart <= 3.0 && !mFuelChecked)
    {
        if (mCar->race.laps > 1)
        {
            mTotalFuel += mLastFuel + mLastPitFuel - mCar->priv.fuel;
            mFuelLapsCounted++;
            mAvgFuelPerLap = mTotalFuel / mFuelLapsCounted;
            mAvgFuelPerMeter = mAvgFuelPerLap / mTrack->length;
            LogAXIOM.debug("# AVG Fuel per lap = %.6f - Fuel per meter = %.8f\n", mAvgFuelPerLap, mAvgFuelPerMeter);
        }

        mLastFuel = mCar->priv.fuel;
        mLastPitFuel = 0.0;
        mFuelChecked = true;
    }
    else if (fromstart > 3.0 && fromstart < 6.0)
    {
        mFuelChecked = false;
    }
}

double Pit::calcRefuel()
{
    double laps = mCar->_remainingLaps + (mTrack->length - mCar->_distFromStartLine) / mTrack->length;

    if (mSituation->_totTime > mSituation->currentTime)
    {
        // Same lap time fallback as in update().
        double laptime = mCar->_bestLapTime > 0.0 ? mCar->_bestLapTime : mCar->_lastLapTime;

        if (mCar->_laps > 2 && laptime > 0.0)
            laps += ( mSituation->_totTime - mSituation->currentTime ) / laptime;
        else
        {
            // It has a pit stop in the first two laps. Normally we don't want to refuel.
            // This will cause for at least sufficient fuel.
            laps += 1;
        }
    }

    // Calc fuel pitstops
    double fueltoend = (laps - mCar->_lapsBehindLeader) * mAvgFuelPerLap;
    int fuelpitstops = int(floor(fueltoend / mCar->_tank));
    LogAXIOM.debug("# PIT STRATEGY Fuel pit stop = %i\n", fuelpitstops);

    // Calc tires pitstops
    double disttoend = (laps - mCar->_lapsBehindLeader) * mTrack->length;
    double tiresstintdist = 1.0 / mMyCar->wheels()->avgWearPerMeter();
    int tirespitstops = int(floor(disttoend / tiresstintdist));
    LogAXIOM.debug("# PIT STRATEGY tires pit stop = %i\n", tirespitstops);

    // Need fuel or tires stops?
    int pitstops = std::max(fuelpitstops, tirespitstops);
    LogAXIOM.debug("Pit Stop = %i\n", pitstops);

    // Calc the stint fuel : split the fuel still needed evenly over this stint
    // and the stops still to come.
    double stintfuel = fueltoend / (pitstops + 1);
    LogAXIOM.debug("# PIT STRATEGY Fuel stintfuel = %.2f\n", stintfuel);

    if (mMaxFuel < stintfuel)
        stintfuel = mMaxFuel;

    if (pitstops > 0)
    {
        // Stay out a little longer, if possible
        stintfuel += 0.5 * mAvgFuelPerLap;
    }

    // Safety margin first, then limit to what the tank can hold.
    stintfuel += 3.0;
    stintfuel = Utils::clip(stintfuel, 0.0, (double)(mCar->_tank));

    // Tire strategy
    double tiresdist = ((mMyCar->wheels()->TyreTreadDepth() / 100) / mMyCar->wheels()->avgWearPerMeter());
    double stintdist = stintfuel * (mTrack->length / mAvgFuelPerLap);

    if (tiresdist < stintdist)
    {
        mTireChange = true;
    }
    else
    {
        mTireChange = false;
    }

    // Print infos
    LogAXIOM.debug("AXIOM Fuel pitstops %i\n", fuelpitstops);
    LogAXIOM.debug("AXIOM Fuel per meter %.7f\n", mAvgFuelPerLap / mTrack->length);
    LogAXIOM.debug("AXIOM Tire pitstops %i\n", tirespitstops);
    LogAXIOM.debug("AXIOM Tire wear per meter %.9f\n", mMyCar->wheels()->avgWearPerMeter());

    // Never request a negative amount (the simu ignores it but the race engine
    // still charges fabs(_pitFuel) as refuel time), nor more than fits in the
    // tank : the request is also used as "fuel added" by updateFuel().
    double refuel = stintfuel - mCar->_fuel;
    return Utils::clip(refuel, 0.0, std::max(0.0, (double)(mCar->_tank - mCar->_fuel)));
}

int Pit::calcRepair() const
{
    if ((mCar->_remainingLaps - mCar->_lapsBehindLeader) * mTrack->length < mMaxDamageDist)
    {
        return (int)(0.5 * mCar->_dammage);
    }

    return mCar->_dammage;
}

double Pit::dist()
{
    if (mPitstop)
    {
        float dl, dw;
        RtDistToPit(mCar, (tTrack*)mTrack, &dl, &dw);

        return dl;
    }

    return DBL_MAX;
}

// Callback from driver
void Pit::pitCommand()
{
    if (mPenalty == RM_PENALTY_STOPANDGO)
    {
        // No repairs or fuel as we're serving a penalty
        mCar->pitcmd.stopType = RM_PIT_STOPANDGO;
    }
    else
    {
        mCar->_pitRepair = calcRepair();
        mLastPitFuel = calcRefuel();
        mCar->_pitFuel = (tdble) mLastPitFuel;

        if(mMyCar->wheels()->TyreTreadDepth() < 15.0)
            mTireChange = true;

        if (mTireChange)
        {
            mCar->pitcmd.tireChange = (tCarPitCmd::TireChange::ALL);

            if(mMyCar->HASCPD)
            {
                    int	remainingLaps = mCar->race.remainingLaps + 1;

                    if (remainingLaps <= 10 && mRain < 1)
                    {
                        mCar->pitcmd.tiresetChange = tCarPitCmd::SOFT;
                        mMyCar->setTireMu(1);
                        LogAXIOM.debug("Change Tire SOFT !\n");
                    }
                    else if (remainingLaps <= 25 && mRain < 2)
                    {
                        mCar->pitcmd.tiresetChange = tCarPitCmd::MEDIUM;
                        mMyCar->setTireMu(2);
                        LogAXIOM.debug("Change Tire MEDIUM !\n");
                    }
                    else if(mRain < 2)
                    {
                        mCar->pitcmd.tiresetChange = tCarPitCmd::HARD;
                        mMyCar->setTireMu(3);
                        LogAXIOM.debug("Change Tire HARD !\n");
                    }
                    else if (mRain < 3)
                    {
                        mCar->pitcmd.tiresetChange = tCarPitCmd::WET;
                        mMyCar->setTireMu(4);
                        LogAXIOM.debug("Change Tire WET !\n");
                    }
                    else
                    {
                        mCar->pitcmd.tiresetChange = tCarPitCmd::EXTREM_WET;
                        mMyCar->setTireMu(5);
                        LogAXIOM.debug("Change Tire EXTREM WET !\n");
                    }
            }
        }
        else
        {
            mCar->pitcmd.tireChange = (tCarPitCmd::TireChange::NONE);
            LogAXIOM.debug(" #AXIOM no tyre changes !!!\n");
        }

        mCar->pitcmd.stopType = RM_PIT_REPAIR;
    }

    setPitstop(false);
}
