namespace vorb {
    namespace script {
        template<typename... Args>
        void Function::call(Args... args) const {
            EnvironmentHandle hnd = m_env->getHandle();

            impl::pushToTop(hnd, *this);
            impl::pushArgs(hnd, args...);
            
            int error=impl::call(hnd, sizeof...(Args), 0);
            
            if(error!=0)
            {
                const cString errorMsg=impl::popValue<const cString>(hnd);
                printf("Error: \"%s\"\n", errorMsg);

                return;
            }

            impl::popStack(hnd);
        }
        template<typename Ret, typename... Args>
        void Function::rcall(OUT Ret* retValue, Args... args) const {
            EnvironmentHandle hnd = m_env->getHandle();

            impl::pushToTop(hnd, *this);
            impl::pushArgs(hnd, args...);
            int error=impl::call(hnd, sizeof...(Args), ScriptValueSender<Ret>::getNumValues());

            if(error!=0)
            {
                const cString errorMsg=impl::popValue<const cString>(hnd);
                printf("Error: \"%s\"\n", errorMsg);

                return;
            }
            *retValue = impl::popValue<Ret>(hnd);
            impl::popStack(hnd);
        }
    }
}