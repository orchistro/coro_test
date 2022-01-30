#include CORO_HDR

#include <string>
#include <memory>
#include <iostream>

using namespace CORO_USING;

struct resumable_thing
{
    struct promise_type;

    coroutine_handle<promise_type> mCoroutine = nullptr;

    resumable_thing() = default;

    explicit resumable_thing(coroutine_handle<promise_type> aCoroutine) : mCoroutine{aCoroutine}
    {
    }
    resumable_thing(const resumable_thing&) = delete;
    resumable_thing& operator=(const resumable_thing&) = delete;

    resumable_thing(resumable_thing&& aOther) : mCoroutine{aOther.mCoroutine}
    {
        aOther.mCoroutine = nullptr;
    }
    resumable_thing& operator=(resumable_thing&& aOther)
    {
        if (&aOther != this)
        {
            mCoroutine = aOther.mCoroutine;
            aOther.mCoroutine = nullptr;
        }

        return *this;
    }

    ~resumable_thing()
    {
        if (mCoroutine)
        {
            mCoroutine.destroy();
        }
    }

    struct promise_type
    {
        resumable_thing get_return_object()
        {
            return resumable_thing(coroutine_handle<promise_type>::from_promise(*this));
        }

        auto initial_suspend()
        {
            return suspend_never{};
        }
        auto final_suspend() noexcept
        {
            return suspend_never{};
        }
        void return_void()
        {
        }
        void unhandled_exception()
        {
        }
    };

    void resume()
    {
        mCoroutine.resume();
    }
};

resumable_thing counter(int aName)
{
    std::cout << "counter " << aName << "\n";

    for (int i = 0;; i++)
    {
        co_await suspend_always{};
        std::cout << "resumed(" << aName << ") : i = " << i << "\n";
    }
}

int main()
{
    auto a = counter(100);
    auto b = counter(200);
    a.resume();
    a.resume();
    b.resume();
    a.resume();
    b.resume();

    return 0;
}

