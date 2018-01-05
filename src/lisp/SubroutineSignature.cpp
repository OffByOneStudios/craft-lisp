#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/SubroutineSignature.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_OBJECT_DEFINE(Argument)
{
	_.defaults();
}

Argument::Argument()
{
	this->flags = (Flags)0;
}


CRAFT_OBJECT_DEFINE(SubroutineSignature)
{
	_.defaults();
}

SubroutineSignature::SubroutineSignature()
{

}

void SubroutineSignature::complete()
{
	this->required = 0;
	this->singular = 0;
	this->variadic = instance<>();
	this->keywords = instance<>();

	bool hit_optional = false;
	bool hit_collect = false;
	size_t i = 0;
	for (auto arg : arguments)
	{
		if (arg->flags & Argument::Optional)
		{
			hit_optional = true;
		}
		else
		{
			if (hit_optional)
				throw stdext::exception("signature[{1} :{0}]: Positional argument may not follow optional arguments.", arg->name, i);
		}

		if (arg->flags & Argument::VariadicCollect)
		{
			if (this->variadic)
				throw stdext::exception("signature[{1} :{0}]: Variadic collection argument already defined.", arg->name, i);
			this->variadic = arg;

			hit_collect = true;
		}
		else if (arg->flags & Argument::KeywordCollect)
		{
			if (this->keywords)
				throw stdext::exception("signature[{1} :{0}]: Keyword collection argument already defined.", arg->name, i);
			this->keywords = arg;

			hit_collect = true;
		}
		else
		{
			if (hit_collect)
				throw stdext::exception("signature[{1} :{0}]: Singular arguments may not follow collection arguments.", arg->name, i);
		}

		i += 1;

		if (!hit_optional) required = i;
		if (!hit_collect) singular = i;
	}
}

instance<SScope> SubroutineSignature::read_frame(instance<SScope> const& scope)
{
	auto new_scope = instance<Scope>::make(scope);

	for (auto signature : arguments)
	{
		new_scope->define(instance<Binding>::make(signature->name, instance<>()));
	}

	_scope = new_scope;
	return new_scope;
}

instance<SScope> SubroutineSignature::eval_frame(instance<SScope> const& scope, std::vector<instance<>> const& args)
{
	check(args);

	auto new_scope = _scope;

	size_t i = 0;
	for (auto actual : args)
	{
		if (i > singular)
		{
			
			// TODO collect
		}
		else
		{
			auto signature = arguments[i];
			new_scope->lookup(signature->name)->setValue(actual);
		}
		i += 1;
	}

	return new_scope;
}

void SubroutineSignature::check(std::vector<instance<>> const& args)
{
	if (args.size() < required)
		throw stdext::exception("signature: requires at least {0} arguments, given {1}.", required, args.size());

	// required arguments
	for (size_t i = 0; i < required; ++i)
	{ 
		auto actual = args[i];
		auto signature = arguments[i];

		if (signature->type != types::None
			&& signature->type != actual.typeId())
			throw stdext::exception("signature: argument {0} is not type {1}", i, signature->type.toString());
	}

	// optional arguments
	size_t optional_max = std::min(args.size(), singular);
	for (size_t i = required; i < optional_max; ++i)
	{
		auto actual = args[i];
		auto signature = arguments[i];

		if (signature->type != types::None
			&& signature->type != actual.typeId())
			throw stdext::exception("signature: argument {0} is not type {1}", i, signature->type.toString());
	}

	// collection arguments
	if (optional_max > singular)
	{
		for (size_t i = singular; i < args.size(); ++i)
		{
			auto actual = args[i];
			auto signature = variadic;

			if (signature->type != types::None
				&& signature->type != actual.typeId())
				throw stdext::exception("signature: argument {0} is not type {1}", i, signature->type.toString());
		}
	}
}

