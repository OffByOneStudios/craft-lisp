#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/Sexpr.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

/******************************************************************************
** Sexpr
******************************************************************************/

CRAFT_DEFINE(Sexpr)
{
	_.use<PClone>().singleton<DefaultCopyConstructor>();
	_.use<PParse>().singleton<FunctionalParse>(&Sexpr::_fromString);
	_.use<PStringer>().singleton<FunctionalStringer>(&Sexpr::_toString);

	_.defaults();
}

std::string Sexpr::_toString(instance<> inst)
{
	return inst.asType<Sexpr>()->toString();
}
instance<> Sexpr::_fromString(std::string s)
{
	return parse_lisp(s);
}

std::string Sexpr::toString() const
{
	// TODO, use an external recursive pretty printer
	std::vector<std::string> child_strs;
	std::transform(std::begin(cells), std::end(cells), std::back_inserter(child_strs),
		[](instance<> const& cell) { return cell.toString(); });

	return "(" + stdext::join(' ', std::begin(child_strs), std::end(child_strs)) + ")";
}