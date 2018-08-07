namespace vorb {
    namespace script {
        template<typename... Args>
        void Function::call(Args... args) const {
            EnvironmentHandle hnd = m_env->getHandle();

            impl::pushToTop(hnd, *this);
            impl::pushArgs(hnd, args...);
            impl::call(hnd, sizeof...(Args), 0);
            impl::popStack(hnd);
        }
        template<typename Ret, typename... Args>
        void Function::rcall(OUT Ret* retValue, Args... args) const {
            EnvironmentHandle hnd = m_env->getHandle();

            impl::pushToTop(hnd, *this);
            impl::pushArgs(hnd, args...);
            impl::call(hnd, sizeof...(Args), ScriptValueSender<Ret>::getNumValues());
            *retValue = impl::popValue<Ret>(hnd);
            impl::popStack(hnd);
        }
    }
}