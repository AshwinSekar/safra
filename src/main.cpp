/**
 * Reads Buechi Automata and create Rabin Automata
 *
 * Takes a variety of output formats, see usage
 */

#include "automata/buechi.hpp"
#include "automata/rabin.hpp"

#include <string>
#include <string.h>
#include <unistd.h>
#include <fstream>

// Output format
enum OUTPUT {
  PLAIN_TEXT,
  DOT,
  PDF
};

// Holds program options
struct options {
  std::string input_filename;
  std::string output_filename;
  std::string input_out_filename;
  OUTPUT format;
  bool use_letters;
};

/*
 * Prints program usage
 */
void usage(const char* prog_name) {
  std::cout << "Usage: " << prog_name
    << " input_buechi.aut [-o output_file]"
    << "[-i input_out_file] [-f format] [-u]" << std::endl;

  std::cout << std::endl << "Arguments" << std::endl;
  std::cout << "    input_buechi.aut" << std::endl;
  std::cout << "        The input Buechi Automat to determinize" << std::endl;
  std::cout << "    -o output_file" << std::endl;
  std::cout << "        The output file to store the Rabin Automata, defaults to standard out"
    << std::endl;
  std::cout << "    -i input_out_file" << std::endl;
  std::cout << "        With this option the input Buechi Auotmata is also converted to the" \
    "specified format and outputed to file" << std::endl;
  std::cout << "    -f format" << std::endl;
  std::cout << "        The output format, one of {txt, pdf, dot} defaults to txt" << std::endl;
  std::cout << "    -u This argument signifies that letters should be used as the alphabet," \
    "fails if alphabet size is over 26" << std::endl;
}

/*
 * Parses input arguments
 */
bool parse_args(options* opts, int argc, char* argv[]) {
  if (argc < 2) {
    usage(argv[0]);
    return false;
  }

  opts->input_filename = argv[1];
  opts->output_filename = "";
  opts->input_out_filename = "";
  opts->format = PLAIN_TEXT;

  int opt;
  while ((opt = getopt(argc, argv, "o:i:f:u")) != -1) {
    switch (opt) {
      case 'o':
        opts->output_filename = optarg;
        break;
      case 'i':
        opts->input_out_filename = optarg;
        break;
      case 'f':
        if (strcmp(optarg, "txt") == 0) {
          opts->format = PLAIN_TEXT;
        } else if(strcmp(optarg, "pdf") == 0) {
          opts->format = PDF;
        } else if(strcmp(optarg, "dot") == 0) {
          opts->format = DOT;
        } else {
          std::cerr << "Unkown format: " << optarg << std::endl;
          return false;
        }
        break;
      case 'u':
        opts->use_letters = true;
        break;
      case '?':
        std::cerr << "Unkown option: " << char(optopt) << std::endl;
        return false;
    }
  }
  return true;
}

int main(int argc, char* argv[]) {
  options opts;

  if (parse_args(&opts, argc, argv)) {
    safra::Buechi buechi(opts.input_filename, opts.use_letters);
    safra::Rabin rab = buechi.to_rabin();

    std::streambuf* buf;
    std::ofstream of;
    if (!opts.output_filename.empty() && opts.format != PDF) {
      of.open(opts.output_filename);
      buf = of.rdbuf();
    } else {
      buf = std::cout.rdbuf();
    }
    std::ostream out(buf);

    switch (opts.format) {
      case PLAIN_TEXT:
        out << rab << std::endl;
        break;
      case PDF:
        rab.to_pdf(opts.output_filename);
        if (!opts.input_out_filename.empty()) {
          buechi.to_pdf(opts.input_out_filename);
        }
        break;
      case DOT:
        rab.to_graphviz(out);
        if (!opts.input_out_filename.empty()) {
          std:: ofstream in_out(opts.input_out_filename);
          buechi.to_graphviz(in_out);
        }
        break;
    }
    return 0;
  }
  return 1;
}
