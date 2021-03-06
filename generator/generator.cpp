#include "generator.h"

//STATIC init
boost::program_options::options_description Generator::arguments = boost::program_options::options_description();
boost::program_options::variables_map Generator::argument_variables = boost::program_options::variables_map();


Generator::Generator()
{
  BOOST_LOG_TRIVIAL(info) << "ROS runtime monitor generator. Made by Bence Gazder";
  arguments.add_options()
    ("help", "See available options.")
    ("wait", "Wait for a key press before termination.")
    ("source-path", boost::program_options::value<std::string>()->required(), "Root directory of the monitor frame source.")
    ("output-path", boost::program_options::value<std::string>()->required(), "Root directory, of the generated monitor.")
    ("debug-output", boost::program_options::value<std::string>(), "Directory of the log files, and generation information.")
    ("input-expression", boost::program_options::value<std::string>()->required(), "Expression that defines, the monitor behavior.")
    ("monitor-name", boost::program_options::value<std::string>()->required(), "Name of the generated monitor.")
    ("true-command", boost::program_options::value<std::string>()->required(), "System call if the result is TRUE")
    ("false-command", boost::program_options::value<std::string>()->required(), "System call for the result is FALSE")
    ;

  //add the wanted filenames for generation
  gen_files["gen_commands.h"] = std::make_tuple("", "", true);
  gen_files["gen_blocks.h"] = std::make_tuple("", "", true);
  gen_files["gen_events.h"] = std::make_tuple("", "", true);
  gen_files["subscriber.h"] = std::make_tuple("", "", false);
  gen_files["CMakeLists.txt"] = std::make_tuple("", "", true);
  gen_files["package.xml"] = std::make_tuple("", "", true);
}

Generator::Generator(int argc, char* argv[]) :Generator()
{
  for (int i = 0; i < argc; i++)
  {
    std::string argument(argv[i]);
    program_arguments.push_back(argument);
  }

  parseProgramArguments(argc, argv);
}

Generator::~Generator()
{
}

void Generator::run()
{
    try
    {
      /*
       * Ex.:
       *  - Linux: /home/user/projects/monitor_generator/monitor
       *  - Windows: D:\Projects\monitor_generator\monitor
       * */
      std::string monitor_source_path = argument_variables["source-path"].as<std::string>();
      /*
       * Ex.:
       *  - Linux:
       *  - Windows:
       * */
      std::string monitor_destination_path = argument_variables["output-path"].as<std::string>();

      /*
       * Ex.: std::string input = "G (((event1 | event2) ^ 4) U (event3 & event4))";
       */
      std::string input = argument_variables["input-expression"].as<std::string>();

      setMonitorDestinationPath(monitor_destination_path);
      setMonitorSourcePath(monitor_source_path);
      setExpressionInput(input);
      setRoot(parseInput(getExpressionInput()));

      auto tree_root = getRoot();
      AstOptimizer::optimizeAst(tree_root);   //remove the unnecessary parts of the AST
      auto clonedTree = converter.convertToConnectionNormalForm(root);
      block_generator.setAstRootNode(clonedTree);
      block_generator.createBlocks();

      /* if you want to write out the optimised AST with the blocks
      #include "syntx/ast_draw.h"
      ast_draw<decltype(clonedTree)>  drawer(clonedTree);
      drawer.to_formatted_string();
      terminate();
      */

      generateMonitor();
      BOOST_LOG_TRIVIAL(info) << "Generation completed!";
    }
    catch (std::exception& e) {
      BOOST_LOG_TRIVIAL(fatal) << e.what();
    }
    catch (...) {
      //Unknown exception happened. Use some hack, to see what is it.
      std::exception_ptr eptr = std::current_exception();
      [&]() -> auto
      {
        try
        {
          if (eptr)
          {
            std::rethrow_exception(eptr);
          }
        }
        catch (const std::exception& e) {
          std::cout << e.what();
        }
      }();
    }
    monitor_generator::terminate();
}

void Generator::setExpressionInput(std::string expression_input)
{
  BOOST_LOG_TRIVIAL(info) << "Expression set to: " << expression_input;
  this->expression_input = expression_input;
}

std::string Generator::getExpressionInput()
{
  return expression_input;
}

void Generator::setMonitorSourcePath(std::string monitor_source_path)
{
  BOOST_LOG_TRIVIAL(info) << "Monitor source path set to: " << monitor_source_path;
  this->monitor_source_path = monitor_source_path;
}

void Generator::setMonitorDestinationPath(std::string monitor_destination_path)
{
  BOOST_LOG_TRIVIAL(info) << "Generated monitor destination folder set to: " << monitor_destination_path;
  this->monitor_destination_path = monitor_destination_path;
}

void Generator::generateMonitor()
{
  //copy the code skeleton to the destination directory
  if (copyDir(boost::filesystem::path(monitor_source_path), boost::filesystem::path(monitor_destination_path)))
    BOOST_LOG_TRIVIAL(info) << monitor_source_path + " directory successfully copied to " + monitor_destination_path;
  else {
    BOOST_LOG_TRIVIAL(fatal) << "Error during source code directory copy. From: " 
      + monitor_source_path + " to " + monitor_destination_path;
    monitor_generator::terminate();
  }

  //modify the gen_* file content
  BOOST_LOG_TRIVIAL(info) << "Processing package.xml...";
  string_replace_all(std::get<1>(gen_files["package.xml"]), 
    "--monitor_name--",
    argument_variables["monitor-name"].as<std::string>()
  );

  BOOST_LOG_TRIVIAL(info) << "Processing CMakeLists.txt...";
  string_replace_all(std::get<1>(gen_files["CMakeLists.txt"]),
    "--monitor_name--",
    argument_variables["monitor-name"].as<std::string>()
  );

  BOOST_LOG_TRIVIAL(info) << "Processing gen_blocks.h...";
  string_replace_all(std::get<1>(gen_files["gen_blocks.h"]),
    "//--DECLARATIONS--",
    block_generator.getFunctionDeclarations()
  );
  string_replace_all(std::get<1>(gen_files["gen_blocks.h"]),
    "//--CONSTRUCTFUNCTIONS--",
    block_generator.getConstructFunctions()
  );
  string_replace_all(std::get<1>(gen_files["gen_blocks.h"]),
    "//--EVALFUNCTIONS--", 
    block_generator.getFunctions()
  );
  
  BOOST_LOG_TRIVIAL(info) << "Processing gen_commands.h...";
  string_replace_all(std::get<1>(gen_files["gen_commands.h"]), 
    "--true_command--", 
    argument_variables["true-command"].as<std::string>()
  );
  string_replace_all(std::get<1>(gen_files["gen_commands.h"]),
    "--false_command--", 
    argument_variables["false-command"].as<std::string>()
  );

  BOOST_LOG_TRIVIAL(info) << "Processing gen_events.h...";
  string_replace_all(std::get<1>(gen_files["gen_events.h"]),
    "//--EVENTS--",
    block_generator.getEventsWithCodes()
  );

  //write out the files
  for (auto& entry : gen_files) {
      std::ofstream file_out(std::get<0>(entry.second));
      if (file_out.is_open())
        BOOST_LOG_TRIVIAL(info) << entry.first + " file opened for generation";
      else
        BOOST_LOG_TRIVIAL(fatal) <<entry.first + " failed to open for writing";
      file_out.write(std::get<1>(entry.second).c_str(), std::get<1>(entry.second).size());
      file_out.close();
  }
}

std::shared_ptr<base_rule::node> Generator::parseInput(std::string expression_input)
{
  std::shared_ptr<base_rule::node> result_root;
  base_rule::set_build_ast(true);
  base_rule::match_range context(expression_input.cbegin(), expression_input.cend());
  base_rule::match_range result_range;

  BOOST_LOG_TRIVIAL(info) << "Parsing the given expression...";
  
  try
  {
    bool match_result = ltl().match(context, result_range, result_root);  //doesn't always return the good value
    std::string match_error_string = base_rule::get_error_message(context);

    if (match_result && (match_error_string.empty() 
      //bruteforce. On no error, the message contains two new line characters...
      || (match_error_string.find("\n\n") != std::string::npos)  
      )) {
      BOOST_LOG_TRIVIAL(info) << "Expression parsed!";
    }
    else {
      BOOST_LOG_TRIVIAL(fatal) << match_error_string;
      monitor_generator::terminate();
    }
  }
  catch (rule::undefined_rule ex)
  {
    BOOST_LOG_TRIVIAL(error) << "Error during the expression parsing: " << ex.what();
    BOOST_LOG_TRIVIAL(error) <<  base_rule::get_error_message(context);
  }

  return result_root;
}


std::shared_ptr<base_rule::node> Generator::getRoot()
{
  return root;
}

void Generator::setRoot(std::shared_ptr<base_rule::node> root)
{
  this->root = root;
}

void Generator::parseProgramArguments(int argc, char* argv[])
{
  //wait flag check, before the parsing can fail
  for (auto& entry : program_arguments)
  {
    if (entry == "--wait")
      monitor_generator::wait_for_key_on_exit = true;
  }

  try {
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, arguments), argument_variables);

  if (!argument_variables["help"].empty() || argument_variables.empty())
  {
    BOOST_LOG_TRIVIAL(info) << "Displaying help options.";
    std::cout << arguments;
    monitor_generator::terminate();
  }

    boost::program_options::notify(argument_variables);
  }
  catch(boost::program_options::error& e){
    BOOST_LOG_TRIVIAL(fatal) << "Error during the command line argument parsing, " << e.what();
    monitor_generator::terminate(1);
  }

  //if the parsing is done, see if the wait flag is added (double check)
  if(!monitor_generator::wait_for_key_on_exit)
    monitor_generator::wait_for_key_on_exit = (argument_variables.count("wait") > 0) ? true : false;

}

int Generator::string_replace_all(std::string& str, const std::string& from, const std::string& to)
{
  int replace_count = 0;
  unsigned long start_pos = str.find(from);

  size_t pos = 0;
  while ((pos = str.find(from, pos)) != std::string::npos) {
    str.replace(pos, from.length(), to);
    pos += to.length();
    replace_count++;
  }

  BOOST_LOG_TRIVIAL(info) << from << " replaced " + std::to_string(replace_count) + " times with " + to.substr(0, 20) + "...";
  return replace_count;
}

bool Generator::copyDir(boost::filesystem::path const & source, boost::filesystem::path const & destination)
{
  namespace fs = boost::filesystem;
  try
  {
    // Check and create the directory
    if (fs::exists(destination))
    {
      BOOST_LOG_TRIVIAL(info) << "Destination directory "
        << destination.string() << " already exists. Overriding...";
    }
    else
    {
      if (!fs::create_directory(destination))
      {
        BOOST_LOG_TRIVIAL(error) << "Unable to create destination directory "
          << destination.string();
        return false;
      }
    }
  }
  catch (fs::filesystem_error const & e)
  {
    BOOST_LOG_TRIVIAL(error) << e.what();
    return false;
  }

  // Check and copy the files
  for (fs::directory_iterator file(source); file != fs::directory_iterator(); ++file)
  {
    try {
      fs::path current(file->path());
      if (fs::is_directory(current))
      {
        if (!copyDir(current, destination / current.filename()))
        {
          return false;
        }
      }
      else
      {
        //get the wanted files path and content
        for (auto& entry : gen_files) {
          if (current.filename() == std::get<0>(entry))
          {
            //path
            std::get<0>(entry.second) = std::string((destination / current.filename()).string());
            BOOST_LOG_TRIVIAL(info) << entry.first + " found! Path: " << std::get<0>(entry.second);
            //content
            std::ifstream file_in(current.c_str());
            if (file_in.is_open())
              BOOST_LOG_TRIVIAL(info) << current.generic_string() + " is opened";
            else
              BOOST_LOG_TRIVIAL(fatal) << std::get<0>(entry.second) + " file opening failed";
            std::get<1>(entry.second) = std::string((std::istreambuf_iterator<char>(file_in)), std::istreambuf_iterator<char>());
            file_in.close();
          }
        }
        //copy the files
        if ((gen_files.count(current.filename().generic_string()) > 0)
            && (std::get<2>(gen_files[current.filename().generic_string()]) == false)) {
              BOOST_LOG_TRIVIAL(info) << current.filename().generic_string() + " copied without override";
              fs::copy_file(current, destination / current.filename(), boost::filesystem::copy_option::none);
        }else{
          BOOST_LOG_TRIVIAL(info) << current.filename().generic_string() + " copied with override";
          fs::copy_file(current, destination / current.filename(), boost::filesystem::copy_option::overwrite_if_exists);
        }
      }
    }
    catch (fs::filesystem_error const & e)
    {
      BOOST_LOG_TRIVIAL(error) << e.what();
    }
  }
  return true;
}