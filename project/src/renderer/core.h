#pragma once
#ifdef _JN_ENGINE
#define JNAPI _declspec(dllexport)
#else	
#define JNAPI _declspec(dllimport)
#endif