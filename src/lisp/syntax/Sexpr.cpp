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
	auto ret = instance<Sexpr>::make();
	ret->cells = parse_lisp(s);
	return ret;
}

std::string Sexpr::toString() const
{
	// TODO, use an external recursive pretty printer
	std::vector<std::string> child_strs;
	std::transform(std::begin(cells), std::end(cells), std::back_inserter(child_strs),
		[](instance<> const& cell) { return cell.toString(); });

	return "(" + stdext::join(' ', std::begin(child_strs), std::end(child_strs)) + ")";
}

std::string Sexpr::toRepr() const
{
	// TODO, use an external recursive pretty printer
	std::vector<std::string> child_strs;
	std::transform(std::begin(cells), std::end(cells), std::back_inserter(child_strs),
		[](instance<> const& cell)
		{
			if (cell.isType<Sexpr>())
				return cell.asType<Sexpr>()->toRepr();
			if (cell.isType<Symbol>())
				return cell.asType<Symbol>()->getDisplay();
			if (cell.isType<std::string>())
				return fmt::format("\"{0}\"", *cell.asType<std::string>());
			return cell.toString();
		});

	return "(" + stdext::join(' ', std::begin(child_strs), std::end(child_strs)) + ")";
}