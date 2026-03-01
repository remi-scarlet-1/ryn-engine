#pragma once

#include "Defines.h"
import Ryntime.Core;

#if !defined(RYN_RUNTIME)
ryn::Box<ryn::IApplication> createApplication();

int main(const int argc, char** argv) { ryn::IApplication::bootstrap(argc, argv, createApplication()); }
#endif