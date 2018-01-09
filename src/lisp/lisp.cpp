#include "lisp/common.h"
#include "lisp/lisp.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_PROVIDER_DEFINE(PSubroutine);
CRAFT_ASPECT_DEFINE(SBinding);
CRAFT_ASPECT_DEFINE(SScope);
CRAFT_ASPECT_DEFINE(SFrame);