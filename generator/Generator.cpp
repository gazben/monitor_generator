#include "Generator.h"

void Generator::Generate(std::string sourcePath_ /*= "D:\\Projects\\R5-COP-Verification\\monitor\\"*/, std::string generatedPath_ /*= "D:\\Projects\\R5-COP-Verification\\Generated\\"*/)
{
  copyDir(boost::filesystem::path(sourcePath_), boost::filesystem::path(generatedPath_));

  ifstream tempFile(PropertyFilePath);
  std::string str((std::istreambuf_iterator<char>(tempFile)), std::istreambuf_iterator<char>());
  propertyFileString = move(str);

  std::string functionDeclarations = blockGenerator->getFunctionDeclarations();
  std::string constructFunctionsString = blockGenerator->getConstructFunctionStrings();
  std::string evalFunctionsString = blockGenerator->getFunctionStrings();

  str_replace(propertyFileString, "//--DECLARATIONS--", functionDeclarations);
  str_replace(propertyFileString, "//--CONSTRUCTFUNCTIONS--", constructFunctionsString);
  str_replace(propertyFileString, "//--EVALFUNCTIONS--", evalFunctionsString);

  ofstream propertyFile(PropertyFilePath);
  propertyFile.write(propertyFileString.c_str(), propertyFileString.size());
  propertyFile.close();
}

Generator::Generator(BlockGenerator* _generator) :blockGenerator(_generator)
{
}

bool Generator::str_replace(std::string& str, const std::string& from, const std::string& to)
{
  unsigned int start_pos = str.find(from);
  if (start_pos == std::string::npos)
    return false;
  str.replace(start_pos, from.length(), to);
  return true;
}

bool Generator::copyDir(boost::filesystem::path const & source, boost::filesystem::path const & destination)
{
  namespace fs = boost::filesystem;
  try
  {
    // Check whether the function call is valid
    if (fs::exists(destination))
    {
      std::cerr << "Destination directory " << destination.string()
        << " already exists. Overriding..." << '\n'
        ;
      //return false;
    }
    else
      // Create the destination directory
      if (!fs::create_directory(destination))
      {
        std::cerr << "Unable to create destination directory"
          << destination.string() << '\n'
          ;
        return false;
      }
  }
  catch (fs::filesystem_error const & e)
  {
    std::cerr << e.what() << '\n';
    return false;
  }
  // Iterate through the source directory
  for (
    fs::directory_iterator file(source);
    file != fs::directory_iterator(); ++file
    )
  {
    try
    {
      fs::path current(file->path());
      if (fs::is_directory(current))
      {
        // Found directory: Recursion
        if (
          !copyDir(
            current,
            destination / current.filename()
            )
          )
        {
          return false;
        }
      }
      else
      {
        // Found file: Copy
        if (current.filename() == "Property.cpp") {
          PropertyFilePath = std::string((destination / current.filename()).string());
        }
        fs::copy_file(
          current,
          destination / current.filename(), copy_option::overwrite_if_exists
          );
      }
    }
    catch (fs::filesystem_error const & e)
    {
      std::cerr << e.what() << '\n';
    }
  }
  return true;
}