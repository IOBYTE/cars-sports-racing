#include "localtime.h"
#include <SDL_mutex.h>
#include <tgf.h>
#include <time.h>
#include <stdexcept>

const char *const LocalTime::name{"LocalTime"};

LocalTime::LocalTime() :
    mutex(SDL_CreateMutex())
{
    if (!mutex)
        throw std::runtime_error("SDL_CreateMutex failed");
}

LocalTime::~LocalTime()
{
    SDL_DestroyMutex(mutex);
}

LocalTime &
LocalTime::instance()
{
    static LocalTime lt;

    return lt;
}

int
LocalTime::get(struct tm &tm, const time_t &time_p)
{
    LocalTime &lt = instance();

    if (time_p == (time_t)-1)
    {
        GfLogError("%s::%s: invalid time_t.\n", name, __func__);
        return -1;
    }

    if (SDL_LockMutex(lt.mutex))
    {
        GfLogError("%s::%s: SDL_LockMutex failed\n", name, __func__);
        return -1;
    }

    struct tm *res = localtime(&time_p);

    if (!res)
    {
        GfLogError("%s::%s: localtime failed\n", name, __func__);
        return -1;
    }

    tm = *res;

    if (SDL_UnlockMutex(lt.mutex))
    {
        GfLogError("%s::%s: SDL_UnlockMutex failed\n", name, __func__);
        return -1;
    }

    return 0;
}
