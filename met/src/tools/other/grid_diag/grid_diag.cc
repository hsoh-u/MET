// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2019
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

////////////////////////////////////////////////////////////////////////
//
//   Filename:   grid_diag.cc
//
//   Description:
//
//   Mod#   Date      Name            Description
//   ----   ----      ----            -----------
//   000    10/01/19  Fillmore        New
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <ctype.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "grid_diag.h"
#include "series_data.h"

#include "vx_statistics.h"
#include "vx_nc_util.h"
#include "vx_regrid.h"
#include "vx_log.h"

////////////////////////////////////////////////////////////////////////

static void process_command_line(int, char **);
static void process_series(void);

static Met2dDataFile *get_mtddf(
    const StringArray &, const GrdFileType);

static void setup_var_hists(void);
static void setup_nc_file(void);
static void clean_up();

static void usage();
static void set_data_files(const StringArray &);
static void set_out_file(const StringArray &);
static void set_config_file(const StringArray &);
static void set_log_file(const StringArray &);
static void set_verbosity(const StringArray &);
static void set_compress(const StringArray &);

////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

    // Set handler to be called for memory allocation error
    set_new_handler(oom);

    // Process the command line arguments
    process_command_line(argc, argv);

    // Close files and deallocate memory
    clean_up();

    return(0);
}

////////////////////////////////////////////////////////////////////////

void process_command_line(int argc, char **argv) {
    CommandLine cline;
    ConcatString default_config_file;

    // Check for zero arguments
    if(argc == 1) usage();

    // Parse the command line into tokens
    cline.set(argc, argv);

    // Set the usage function
    cline.set_usage(usage);

    // Add the options function calls
    cline.add(set_data_files,  "-data",  -1);
    cline.add(set_config_file, "-config", 1);
    cline.add(set_out_file,    "-out",    1);
    cline.add(set_log_file,    "-log",    1);
    cline.add(set_verbosity,   "-v",      1);
    cline.add(set_compress,    "-compress", 1);

    // Parse the command line
    cline.parse();

    // Check for error, there should be zero arguments left
    if(cline.n() != 0) usage();

    // Check that the required arguments have been set.
    if(data_files.n_elements() == 0) {
        mlog << Error << "\nprocess_command_line() -> "
             << "the data file list must be set using the "
             << "\"-data option.\n\n";
        usage();
    }
    if(config_file.length() == 0) {
        mlog << Error << "\nprocess_command_line() -> "
             << "the configuration file must be set using the "
             << "\"-config\" option.\n\n";
        usage();
    }
    if(out_file.length() == 0) {
        mlog << Error << "\nprocess_command_line() -> "
             << "the output NetCDF file must be set using the "
             << "\"-out\" option.\n\n";
        usage();
    }

    // Create the default config file name
    default_config_file = replace_path(default_config_filename);

    // List the config files
    mlog << Debug(1)
         << "Default Config File: " << default_config_file << "\n"
         << "User Config File: "    << config_file << "\n";

    mlog << Debug(2) << "Read config files.\n";

    // Read the config files
    conf_info.read_config(
        default_config_file.c_str(), config_file.c_str());

    // Get the data file type from config, if present
    dtype = parse_conf_file_type(
        conf_info.conf.lookup_dictionary(conf_key_data));

    // Parse the data file lists
    data_files = parse_file_list(data_files, dtype);

    // Get mtddf
    data_mtddf = get_mtddf(data_files, dtype);

    // Store the input data file types
    dtype = data_mtddf->file_type();

    mlog << Debug(2) << "Process configuration.\n";

    // Process the configuration
    conf_info.process_config(dtype);

    // Determine the verification grid
    grid = parse_vx_grid(conf_info.data_info[0]->regrid(),
        &(data_mtddf->grid()), &(data_mtddf->grid()));

    // Process masking regions
    conf_info.process_masks(grid);

    // Setup variable histograms
    setup_var_hists();

    // Process series
    process_series();
}

////////////////////////////////////////////////////////////////////////

void setup_var_hists(void) {

    for(int i_var = 0; i_var < conf_info.get_n_data(); i_var++) {
        VarInfo* data_info = conf_info.data_info[i_var];
        mlog << Debug(2) << "Initializing "
             << data_info->magic_str() << " histogram\n"; 
        var_hists[data_info->magic_str()] = vector<int>();
    }
}

////////////////////////////////////////////////////////////////////////

void setup_nc_file(void) {

}

////////////////////////////////////////////////////////////////////////

void process_series(void) {
    DataPlane data_dp;

    // List the lengths of the series options
    mlog << Debug(1)
         << "Length of configuration \"data.field\" = "
         << conf_info.get_n_data() << "\n"
         << "Length of data file list         = "
         << data_files.n_elements() << "\n";

    series_type = SeriesType_Data_Files;
    n_series = data_files.n_elements();
    mlog << Debug(1)
          << "Series defined by the data file list of length "
          << n_series << ".\n";

    // Process series variables
    for(int i_series = 0; i_series < n_series; i_series++) {
        for(int i_var = 0; i_var < conf_info.get_n_data(); i_var++) {
            get_series_entry(i_series, conf_info.data_info[i_var],
                data_files, dtype, found_data_files, data_dp, grid);
        }
    }

    return;
}

////////////////////////////////////////////////////////////////////////

Met2dDataFile *get_mtddf(const StringArray &file_list,
    const GrdFileType type) {

    Met2dDataFile *mtddf = (Met2dDataFile *) 0;

    mlog << "Enter get_mtddf.\n";

    int i;
    // Find the first file that actually exists
    for(i = 0; i < file_list.n_elements(); i++) {
        if(file_exists(file_list[i].c_str())) break;
    }

    // Check for no valid files
    if(i == data_files.n_elements()) {
        mlog << Error << "\nTrouble reading data files\n\n";
        exit(1);
    }

    // Read first valid file
    if(!(mtddf = mtddf_factory.new_met_2d_data_file(
        file_list[i].c_str(), type))) {
        mlog << Error << "\nTrouble reading data file \""
             << file_list[i] << "\"\n\n";
        exit(1);
    }

    return(mtddf);
}

////////////////////////////////////////////////////////////////////////

void clean_up() {

    // Close the output NetCDF file
    if(nc_out) {

        // List the NetCDF file after it is finished
        mlog << Debug(1) << "Output file: " << out_file << "\n";

        delete nc_out;
        nc_out = (NcFile *) 0;
    }

    // Deallocate memory for data files
    if(data_mtddf) {
        delete data_mtddf;
        data_mtddf = (Met2dDataFile *) 0;
    }

    return;
}

////////////////////////////////////////////////////////////////////////

void usage() {

    cout << "\n*** Model Evaluation Tools (MET" << met_version
         << ") ***\n\n"

         << "Usage: " << program_name << "\n"
         << "\t-data  file_1 ... file_n | data_file_list\n"
         << "\t-out file\n"
         << "\t-config file\n"
         << "\t[-log file]\n"
         << "\t[-v level]\n"
         << "\t[-compress level]\n\n"

         << "\twhere\t\"-data file_1 ... file_n\" are the gridded "
         << "data files to be used (required).\n"

         << "\t\t\"-data data_file_list\" is an ASCII file containing "
         << "a list of gridded data files to be used (required).\n"

         << "\t\t\"-out file\" is the NetCDF output file containing "
         << "computed statistics (required).\n"

         << "\t\t\"-config file\" is a GridDiagConfig file "
         << "containing the configuration settings (required).\n"

         << "\t\t\"-log file\" outputs log messages to the specified "
         << "file (optional).\n"

         << "\t\t\"-v level\" overrides the default level of logging ("
         << mlog.verbosity_level() << ") (optional).\n"

         << flush;

    exit(1);
}

////////////////////////////////////////////////////////////////////////

void set_data_files(const StringArray & a) {
    data_files = a;
}

////////////////////////////////////////////////////////////////////////

void set_out_file(const StringArray & a) {
    out_file = a[0];
}

////////////////////////////////////////////////////////////////////////

void set_config_file(const StringArray & a) {
    config_file = a[0];
}

////////////////////////////////////////////////////////////////////////

void set_log_file(const StringArray & a) {
    ConcatString filename;

    filename = a[0];

    mlog.open_log_file(filename);
}

////////////////////////////////////////////////////////////////////////

void set_verbosity(const StringArray & a) {
    mlog.set_verbosity_level(atoi(a[0].c_str()));
}

////////////////////////////////////////////////////////////////////////

void set_compress(const StringArray & a) {
    compress_level = atoi(a[0].c_str());
}

////////////////////////////////////////////////////////////////////////
