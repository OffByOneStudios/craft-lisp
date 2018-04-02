#include "lisp/common.h"
#include "lisp/lisp.h"
#include "SubroutineSignature.h"

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

instance<lisp::types::SType> SubroutineSignature::asType()
{
	if (arguments.size() == 1
		&& arguments[0]->flags == Argument::Flags::VariadicCollect
		&& arguments[0]->type.typeId().isType<types::Special>()
		&& arguments[0]->type.asType<types::Special>()->kind == types::Special::Any)
		return arguments[0]->type;

	auto tuple = instance<lisp::types::Tuple>::make();
	tuple->cells.reserve(this->required);

	for (auto i = 0; i < required; ++i)
	{
		tuple->cells.push_back(arguments[i]->type);
	}

	return tuple;
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
	assert(_scope.isNull()); // A function should never be read more than once without rebuilding it.

	auto new_scope = instance<Block>::make(scope);

	for (auto signature : arguments)
	{
		signature->binding = new_scope->define(signature->name, instance<>());
	}

	_scope = new_scope;
	return new_scope;
}

instance<SFrame> SubroutineSignature::push_frame()
{
	return instance<Frame>::make(_scope);
}

instance<SFrame> SubroutineSignature::set_frame(instance<SFrame> const& frame, std::vector<instance<>> const& args)
{
	check(frame->getNamespace()->environment(), args);

	instance<Frame> call_frame = frame.asType<Frame>();

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

			if (signature->binding.typeId().isType<BlockBinding>())
			{
				auto signature_blockBinding = signature->binding.asType<BlockBinding>();
				call_frame->set(signature_blockBinding->position(), actual);
			}
			else
				throw stdext::exception("Can only call functions using blocks.");
		}
		i += 1;
	}

	return call_frame;
}

void SubroutineSignature::check(instance<Environment> env, std::vector<instance<>> const& args)
{
	if (args.size() < required)
		throw stdext::exception("signature: requires at least {0} arguments, given {1}.", required, args.size());

	// required arguments
	for (size_t i = 0; i < required; ++i)
	{ 
		auto actual = args[i];
		auto signature = arguments[i];

		if (!types::is_subtype(env, instance<types::CraftType>::make(actual.typeId()), signature->type))
			throw stdext::exception("signature: argument {0} is not type {1}", i, signature->type.toString());
	}

	// optional arguments
	size_t optional_max = std::min(args.size(), singular);
	for (size_t i = required; i < optional_max; ++i)
	{
		auto actual = args[i];
		auto signature = arguments[i];

		if (!types::is_subtype(env, instance<types::CraftType>::make(actual.typeId()), signature->type))
			throw stdext::exception("signature: argument {0} is not type {1}", i, signature->type.toString());
	}

	// collection arguments
	if (optional_max > singular)
	{
		for (size_t i = singular; i < args.size(); ++i)
		{
			auto actual = args[i];
			auto signature = variadic;

			if (!types::is_subtype(env, instance<types::CraftType>::make(actual.typeId()), signature->type))
				throw stdext::exception("signature: argument {0} is not type {1}", i, signature->type.toString());
		}
	}
}

