#ifndef GUI_EVENT_LOOP_H
#define GUI_EVENT_LOOP_H

#include <list>
#include <SDL_mutex.h>
#include <tgf.h>
#include <tgfclient.h>

template <class T>
class GuiEventLoop
{
protected:
    GuiEventLoop() :
        mutex(SDL_CreateMutex()),
        cb(GfuiApp().eventLoop().setRecomputeCB(recompute, this))
    {
        if (!mutex)
            throw std::runtime_error("SDL_CreateMutex failed");

        if (!cb)
            throw std::runtime_error("GfEventLoop::registerRecomputeCB failed");
    }

    virtual ~GuiEventLoop()
    {
        //GfuiApp().eventLoop().unregisterRecomputeCB(cb);
        SDL_DestroyMutex(mutex);
    }

    int push_event(const T &ev)
    {
        if (SDL_LockMutex(mutex))
        {
            GfLogError("%s::%s: SDL_LockMutex failed\n", name, __func__);
            return -1;
        }

        events.push_back(ev);

        if (SDL_UnlockMutex(mutex))
        {
            GfLogError("%s::%s: SDL_LockMutex failed\n", name, __func__);
            return -1;
        }

        return 0;
    }

    virtual int exec_event(const T &ev)
    {
        GfLogError("%s::%s: derived class must override this function.\n",
            name, __func__);
        return -1;
    }

private:
    static void recompute(void *arg)
    {
        static_cast<GuiEventLoop *>(arg)->recompute_impl();
    }

    void recompute_impl()
    {
        if (SDL_LockMutex(mutex))
        {
            GfLogError("%s::%s: SDL_LockMutex failed\n", name, __func__);
            return;
        }

        for (auto it = events.begin(), i = 0u; it != events.end(); it++, i++)
            exec_event(*it);

        events.clear();

        if (SDL_UnlockMutex(mutex))
        {
            GfLogError("%s::%s: SDL_LockMutex failed\n", name, __func__);
            return;
        }
    }

    SDL_mutex *mutex;
    std::list<T> events;
    void *cb;
    static const char *const name;
};

template <class T>
const char *const GuiEventLoop<T>::name{"GuiEventLoop"};

#endif /* GUI_EVENT_LOOP_H */
