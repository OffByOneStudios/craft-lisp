#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/syntax.h"
#include "CultLispSyntax.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

/******************************************************************************
** CultLispSyntax
******************************************************************************/

CRAFT_DEFINE(CultLispSyntax)
{
	_.use<PSyntax>().singleton<CultLispSyntaxProvider>();

	_.defaults();
}


CultLispSyntax::CultLispSyntax(instance<lisp::Module> forModule)
{
	_module = forModule;
}



void CultLispSyntax::parse(std::string const& s, PSyntax::ParseOptions const* opts = nullptr)
{
	static PSyntax::ParseOptions defaultOpts = {
		true, // keep_source;
		false, // no_source_locations;
		false, // no_macros;
	};
	if (opts == nullptr) opts = &defaultOpts;

	if (opts->keep_source)
		_source = s;

	_rootForms = parse_lisp(s, !opts->no_source_locations);
}

// TODO string_view
std::string const& CultLispSyntax::getSource(PSyntax::SourceLocation const& sl) const
{
	return _source.substr(sl.pos, sl.length);
}

PSyntax::SourceLocation const& CultLispSyntax::getSourceLocation(size_t const&) const
{

}

/******************************************************************************
** CultLispSyntaxProvider
******************************************************************************/

instance<lisp::Module> CultLispSyntaxProvider::getModule(instance<> syntax) const
{
	return syntax.asType<CultLispSyntax>()->getModule();
}

instance<> CultLispSyntaxProvider::parse(std::string const& s, instance<lisp::Module> into, ParseOptions const* opts) const
{
	auto building = instance<CultLispSyntax>::make(into);
	building->parse(s, opts);

	return building;
}

std::string CultLispSyntaxProvider::source_toString(instance<> syntax, SourceLocation const& sl) const
{
	return syntax.asType<CultLispSyntax>()->getSource(sl);
}

std::vector<instance<>> CultLispSyntaxProvider::node_children(instance<> syntax_node) const
{
	if (syntax_node.isType<Sexpr>())
		return syntax_node.asType<Sexpr>()->cells;
	else if (syntax_node.isType<CultLispSyntax>())
		return syntax_node.asType<CultLispSyntax>()->getRootForms();
	else
		return{};
}
std::string CultLispSyntaxProvider::node_toString(instance<> syntax_node) const
{
	return "";
}
std::string CultLispSyntaxProvider::node_toVerboseString(instance<> syntax_node) const
{
	return "";
}

PSyntax::SourceLocation CultLispSyntaxProvider::node_source(instance<> syntax_node) const
{
	return {};
}
