﻿#include "lisp/common.h"
#include "lisp/lisp.h"

#include <stack>
#include <queue>

#include "replxx/replxx.hxx"
#include "lisp/semantics/cult/cult.h"
#include "lisp/library/system/prelude.h"


using namespace craft;
using namespace craft::lisp;


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


int main(int argc, char** argv)
{
	craft::types::boot();

	instance<Environment> global_env = instance<Environment>::make(spdlog::stdout_color_mt("environment"));
	instance<Namespace> ns = global_env->ns_user;
	
	if (argc != 1)
	{
		std::string f;
		try
		{
			f = fs::read<std::string>(path::normalize(argv[1]), fs::string_read).get();
		}
		catch (...)
		{
			global_env->log()->info("No Such File: {0}", argv[1]);
		}
		try
		{
			auto live_module = ns->requireModule(fmt::format("file:{0}", f));
			live_module->initialize();
		}
		catch (stdext::exception e)
		{
			global_env->log()->error(e.what());
			return -1;
		}
		catch (std::exception e)
		{
			global_env->log()->error(e.what());
			return -1;
		}
	}
	else
	{
		instance<Module> live_module = global_env->ns_user->requireModule("repl:console");
		live_module->initialize();

		using Replxx = replxx::Replxx;

		Replxx rx;
		rx.install_window_change_handler();
		std::string history_file(path::join(path::system_data_path(), "culthistory.cult"));
		rx.history_load(history_file);
		// set the max history size
		rx.set_max_history_size(12);
		// set the max input line size
		rx.set_max_line_size(128);
		// set the max number of hint rows to show
		rx.set_max_hint_rows(8);
		rx.set_beep_on_ambiguous_completion(false);
		rx.set_word_break_characters("() ");
		
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
		rx.set_completion_callback(complete, &live_module);

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
		rx.set_highlighter_callback(colors, &live_module);


		for (;;)
		{
			// display the prompt and retrieve input from the user
			auto cinp = rx.input("CμλΤ>");
			if (cinp == nullptr) break;

			auto input = instance<std::string>::make(cinp);

			instance<Module> statement;
			try
			{
				auto statement_loader = instance<AnonLoader>::make();
				statement_loader->setContent(input);
				statement_loader->setModule(live_module);
				statement = ns->requireModule("anon:repl", statement_loader);
			}
			catch (std::exception const& ex)
			{
				std::cout << "parser: " << ex.what() << '\n';
			}

			try
			{
				if (statement)
				{
					live_module->appendModule(statement);
					instance<> res = live_module->lastExecutedResult();

					std::cout << res.toString() << '\n';
				}
			}
			catch (std::exception const& e)
			{
				std::cout << e.what() << '\n';
			}

			rx.history_add(*input);
			rx.history_save(history_file);
		}
		// the path to the history file
		
	}

	return 0;
	
	
	//add_globals(global_env);
	
}
