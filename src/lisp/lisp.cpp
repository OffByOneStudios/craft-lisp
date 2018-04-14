#include "lisp/common.h"
#include "lisp/lisp.h"

using namespace craft;
using namespace craft::lisp;

CRAFT_DEFINE(PSyntax) { _.defaults(); }
CRAFT_DEFINE(PSemantics) { _.defaults(); }
CRAFT_DEFINE(PBackend) { _.defaults(); }
CRAFT_DEFINE(PExecutor) { _.defaults(); }
CRAFT_DEFINE(PCompiler) { _.defaults(); }
CRAFT_DEFINE(SFrame) { _.defaults(); }
