#ifndef LOCALTIME_H
#define LOCALTIME_H

#include <SDL_mutex.h>
#include <time.h>

// localtime(3) is not thread-safe, so this class provides a
// thread-safe (but not re-entrant) portable interface.
class LocalTime
{
public:
    static int get(struct tm &tm, const time_t &time_p = time(NULL));

private:
    LocalTime();
    ~LocalTime();
    void operator=(const LocalTime &);
    static LocalTime &instance();
    SDL_mutex *mutex;
    static const char *const name;
};

#endif /* LOCALTIME_H */
