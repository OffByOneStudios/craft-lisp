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
			auto frame = InterpreterFrameSection::ensureCurrent(Execution::getCurrent()->getNamespace()->get<BootstrapInterpreter>());

			return frame->interp_call(cl->subroutine, call, cl->scope_frame.asType<InterpreterFrame>());
		}
	};
}}

CRAFT_DEFINE(SubroutineClosure)
{
	_.use<PSubroutine>().singleton<SubroutineClosureSubroutineProvider>();

	_.defaults();
};

SubroutineClosure::SubroutineClosure(instance<> frame, instance<PSubroutine> subroutine)
	: scope_frame(frame), subroutine(subroutine)
{

}
