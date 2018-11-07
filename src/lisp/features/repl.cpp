#include "lisp/common.h"

#include "repl.h"


#include "lisp/semantics/cult/cult.h"
#include "lisp/library/system/prelude.h"


using namespace craft;
using namespace craft::lisp;
using namespace craft::lisp::features;


namespace _impl {
	using cl = replxx::Replxx::Color;
	std::vector<std::pair<std::string, cl>> regex_color{
		// single chars
		{ "\\`", cl::BRIGHTCYAN },
	{ "\\'", cl::BRIGHTBLUE },
	{ "\\\"", cl::BRIGHTBLUE },
	{ "\\.", cl::BRIGHTMAGENTA },
	{ "\\(", cl::BRIGHTMAGENTA },
	{ "\\)", cl::BRIGHTMAGENTA },
	{ "\\[", cl::BRIGHTMAGENTA },
	{ "\\]", cl::BRIGHTMAGENTA },
	{ "\\{", cl::BRIGHTMAGENTA },
	{ "\\}", cl::BRIGHTMAGENTA },

	// numbers
	{ "[\\-|+]{0,1}[0-9]+", cl::YELLOW }, // integers
	{ "[\\-|+]{0,1}[0-9]*\\.[0-9]+", cl::YELLOW }, // decimals
	{ "[\\-|+]{0,1}[0-9]+e[\\-|+]{0,1}[0-9]+", cl::YELLOW }, // scientific notation

															 // strings
	{ "\".*?\"", cl::BRIGHTGREEN }, // double quotes
	{ "\"\"\".*?\"\"\"", cl::BROWN }, // double quotes
	};
}


CRAFT_DEFINE(LispRepl)
{
	_.defaults();
}

LispRepl::LispRepl(std::function<void(instance<Module>)> init)
{
	auto logger = spdlog::stdout_color_mt("environment");
	logger->set_level(spdlog::level::trace);

	_env = instance<Environment>::make(logger);

	_live_module = _env->importModule(instance<>(), "repl:console");
	if (init) init(_live_module);
	_live_module->initialize();

	using Replxx = replxx::Replxx;

	
	_rx.install_window_change_handler();
	_history_file = std::string(path::join(path::system_data_path(), "culthistory.cult"));
	_rx.history_load(_history_file);
	// set the max history size
	_rx.set_max_history_size(12);
	// set the max input line size
	_rx.set_max_line_size(128);
	// set the max number of hint rows to show
	_rx.set_max_hint_rows(8);
	_rx.set_beep_on_ambiguous_completion(false);
	_rx.set_word_break_characters("() ");

	//rx.set_special_prefixes("()");

	/*Replxx::completions_t hook_completion(std::string const& context, int index, void* user_data);
	Replxx::hints_t hook_hint(std::string const& context, int index, Replxx::Color& color, void* user_data);
	void hook_color(std::string const& str, Replxx::colors_t& colors, void* user_data);*/
	std::function<Replxx::completions_t(std::string const& context, int index, void* user_data)> complete = [](std::string const& context, int index, void* user_data)->Replxx::hints_t {
		auto module = *static_cast<instance<Module>*>(user_data);
		Replxx::completions_t completions;

		instance<> query = (instance<>)instance<std::string>::make(context);
		instance<> ind = (instance<>)instance<int64_t>::make(index);

		instance<library::List> res = module->exec("repl/completion", { module, query, ind });

		for (auto& d : res->data())
		{
			completions.emplace_back(d.asFeature<std::string>()->c_str());
		}

		return completions;
	};
	_rx.set_completion_callback(complete, &_live_module);

	std::function<void(std::string const&, Replxx::colors_t&, void*)> colors = [](std::string const& context, Replxx::colors_t& colors, void* user_data)
	{
		for (auto const& e : _impl::regex_color) {
			size_t pos{ 0 };
			std::string str = context;
			std::smatch match;

			while (std::regex_search(str, match, std::regex(e.first))) {
				std::string c{ match[0] };
				pos += std::string(match.prefix()).size();

				for (size_t i = 0; i < c.size(); ++i) {
					colors.at(pos + i) = e.second;
				}

				pos += c.size();
				str = match.suffix();
			}
		}
	};
	_rx.set_highlighter_callback(colors, &_live_module);
}


instance<> LispRepl::invoke(instance<std::string> input, bool record)
{
	if (record)
	{
		_rx.history_add(*input);
		_rx.history_save(_history_file);
	}

	instance<Module> statement;
	try
	{
		auto statement_loader = instance<AnonLoader>::make();
		statement_loader->setContent(input);
		statement_loader->setModule(_live_module);
		statement = _env->importModule(_live_module, "anon:repl", statement_loader);
	}
	catch (std::exception const& ex)
	{
		throw ReplParseException(ex.what());
	}

	if (statement)
	{
		_live_module->appendModule(statement);
		return _live_module->lastExecutedResult();
	}

	return instance<>();
}
instance<> LispRepl::step()
{
	// display the prompt and retrieve input from the user
	#ifndef _win32
	auto cinp = _rx.input("Cult>");
	#else
	auto cinp = _rx.input("CμλΤ>");
	#endif
	if (cinp == nullptr) throw ReplExitException();


	auto input = instance<std::string>::make(cinp);

	return invoke(input, true);
}

