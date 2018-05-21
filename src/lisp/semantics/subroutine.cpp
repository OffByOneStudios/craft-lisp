#include "lisp/common.h"
#include "lisp/lisp.h"
#include "subroutine.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_DEFINE(PSubroutine) { _.defaults(); }


/******************************************************************************
** SubroutineClosure
******************************************************************************/

#include "lisp/backend/BootstrapInterpreter.h"

namespace craft {
namespace lisp {
	class SubroutineClosureSubroutineProvider
		: public types::Implements<PSubroutine>::For<SubroutineClosure>
	{
		static ExpressionStore MultiMethodAny;

		virtual types::Function function(instance<> _) const override
		{
			throw stdext::exception("Needs to support closures....");
		}
		virtual types::ExpressionStore expression(instance<> _) const override
		{
			instance<SubroutineClosure> cl = _;

			return cl->subroutine->expression(cl->subroutine);
		}

		//
		// Performs a full call on the subroutine, including pushing to the current execution
		//
		virtual instance<> execute(instance<> _, types::GenericInvoke const& call) const override
		{
			instance<SubroutineClosure> cl = _;

			// TODO make this generic
			auto frame = InterpreterFrame::ensureCurrent(Execution::getCurrent()->getNamespace()->get<BootstrapInterpreter>());

			frame->getExecution()->getNamespace()->getEnvironment()->log()->warn("Subroutine executed with poor frame state, crash incomming...");
			return frame->interp_call(cl->subroutine, call /*, (InterpreterFrame::SubFrame*)cl->scope_frame */);
		}
	};
}}

CRAFT_DEFINE(SubroutineClosure)
{
	_.use<PSubroutine>().singleton<SubroutineClosureSubroutineProvider>();

	_.defaults();
};

SubroutineClosure::SubroutineClosure(void* frame, instance<PSubroutine> subroutine)
	: scope_frame(frame), subroutine(subroutine)
{

}
