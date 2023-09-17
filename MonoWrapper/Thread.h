#pragma once

#include "NonOwningPtr.h"
#include <mono/metadata/threads.h>

namespace Mono {
    class AppDomain;

    class Thread {
    public:
        Thread() = default;

        explicit Thread(non_owning_ptr<MonoThread> thread);

        static const AppDomain &getDomain();

        static void setDomain(const AppDomain &domain);

        static Thread currentThread();

        static Thread attach(const AppDomain &domain);

        /**
         * @brief Get raw MonoThread pointer.
         * @return Raw MonoThread pointer.
         */
        non_owning_ptr<MonoThread> get() const;

        void detach();

    protected:
        non_owning_ptr<MonoThread> _thread = nullptr;
    };
}