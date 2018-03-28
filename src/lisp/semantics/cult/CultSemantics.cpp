#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult_semantics.h"
#include "CultSemantics.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

/******************************************************************************
** CultSemantics
******************************************************************************/

CRAFT_DEFINE(CultSemantics)
{
	_.use<PSemantics>().singleton<CultSemanticsProvider>();

	_.defaults();
}


CultSemantics::CultSemantics(instance<lisp::Module> forModule)
{
	_module = forModule;
}

void CultSemantics::read(instance<CultLispSyntax> syntax, PSemantics::ReadOptions const* opts = nullptr)
{

}

/******************************************************************************
** CultSemanticsProvider
******************************************************************************/

instance<lisp::Module> CultSemanticsProvider::getModule(instance<> semantics) const
{
	semantics.asType<CultSemantics>()->getModule();
}

std::vector<TypeId> CultSemanticsProvider::readsFrom() const
{
	return{ cpptype<CultLispSyntax>::typeDesc() };
}
instance<> CultSemanticsProvider::read(instance<> syntax, instance<lisp::Module> into, ReadOptions const* opts) const
{
	auto building = instance<CultSemantics>::make(into);
	building->read(syntax, opts);

	return building;
}

std::vector<TypeId> CultSemanticsProvider::transformsFrom() const
{
	return{};
}
instance<> CultSemanticsProvider::transform(instance<> semantics, instance<lisp::Module> into, instance<> transformationOptions = instance<>()) const
{

}

instance<> CultSemanticsProvider::lookup(instance<> semantics, std::string const&) const
{

}
