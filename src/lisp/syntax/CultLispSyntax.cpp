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



void CultLispSyntax::parse(std::string const& s, PSyntax::ParseOptions const* opts)
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

std::string_view CultLispSyntax::getSource(PSyntax::SourceLocation const& sl) const
{
	return std::string_view(_source.c_str() + sl.pos, sl.length);
}

PSyntax::SourceLocation CultLispSyntax::getSourceLocation(size_t const& start_pos, size_t const& end_pos) const
{
	if (_source.empty())
		return { 0, 0, 0, 0, 0, 0 };

	size_t start_line_count = 0;
	size_t start_col_offset = 0;
	for (size_t i =0; i < start_pos; ++i)
	{
		if (_source[i] == '\n')
		{
			start_line_count += 1;
			start_col_offset = i;
		}
	}

	size_t end_line_count = start_line_count;
	size_t end_col_offset = start_col_offset;
	for (auto i = start_pos; i < end_pos; ++i)
	{
		if (_source[i] == '\n')
		{
			end_line_count += 1;
			end_col_offset = i;
		}
	}

	return {
		(uint32_t)start_pos, (uint32_t)(end_pos == 0 ? 1 : end_pos - start_pos),
		(uint32_t)start_line_count, (uint32_t)(start_pos - start_col_offset),
		(uint32_t)end_line_count, (uint32_t)(end_pos - end_col_offset)
	};
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

std::string_view CultLispSyntaxProvider::source_toString(instance<> syntax, SourceLocation const& sl) const
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
std::string_view CultLispSyntaxProvider::node_toString(instance<> syntax_node) const
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
