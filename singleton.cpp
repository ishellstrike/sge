#include "singleton.h"

template<typename Typename>
Typename *Singleton<Typename>::m_instance = nullptr;
