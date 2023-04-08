// -*- coding: utf-8 -*-

/*
Checks the Lines of Code of a Python project and also works with Single files

license: MIT License
version: 1.0
author:  ucx15 @ https://github.com/ucx15
usage:   slocc [path]
*/

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>


#define UNDER_COUNT 50
#define VERSION 1.0f


namespace fs = std::filesystem;


std::string *get_files(std::string pathName, int *filecount) {

    for (const auto &entry : fs::directory_iterator(pathName)) {
        if (!entry.is_directory()) {
            if (entry.path().extension() == std::string(".py")) {
                (*filecount)++;
            }
        }
    }

    std::string *fileList = new std::string[*filecount];

    int i = 0;
    for (const auto &entry : fs::directory_iterator(pathName)) {
        if (!entry.is_directory()) {
            if (entry.path().extension() != std::string(".py")) {continue;}

            fileList[i] = entry.path().string();
            i++;
        }
    }

    return fileList;
}


void parse_file(std::string f_name, int *sloc, int *loc, int *cmts, int *blank) {
    std::ifstream file;
    file.open(f_name, std::ios_base::in);

    if (file.bad()) {
        std::cout << "Error opening file\n";
        (*sloc) = -1;
        (*loc) = -1;
        return;
    }

    std::string line;
    bool  is_line = true, is_cmt = false, is_blnk;

    while (std::getline(file, line)) {

        char c = line[0];
        
        is_blnk = false;
        if (!is_cmt) { is_line = true; }

        // Empty Lines and New Lines
        if ((c == 0) || (c == '\n') || (c == 13)) {
            is_line = false, is_blnk = true;
            (*blank)++;
        }

        // Comments
        else if (c == '#') {
            is_line = false;
            (*cmts)++;
        }

        // Tabbed/Spaced Comments
        else if ((c == '\t') || (c == ' ')) {
            for (auto l : line) {
                if (l == '#') {
                    is_line = false;
                    (*cmts)++;
                    break;
                }

                else if ((l != '\t') && (l != ' ')) { break;}
            }
        }


        // Multi-line Comments and Doc-Strings
        else if ((c == '\'') || (c == '"')) {
            if (((line[1] == '\'') || (line[1] == '"')) &&
                ((line[2] == '\'') || (line[2] == '"'))) {
                is_line = false;
                is_cmt = 1 - is_cmt;
                (*cmts)++;
            }
        }

        // Marking line SLOC or not
        if ((is_line) && (!is_cmt)) {
            (*sloc)++;
        }

        if ((!is_line) && (!is_blnk) && is_cmt) {
            (*cmts)++;
        }

        // Marking line LOC
        (*loc)++;
    }

    file.close();
}



int main(int argc, char **argv) {

    std::string path = "./";

    // Parsing Flags
    if (argc == 2){
        std::string flag = std::string( *(argv+1) );

        if (flag[0] == '-') {
            if (
                (flag == (std::string)"-V") ||
                (flag == (std::string)"--version")) {
                    std::cout << VERSION << '\n';
            }

            else if (
                (flag == (std::string)"--help") ||
                (flag == (std::string)"-h")) {
                std::cout << "\t\t\tslocc - Source Lines of Code (using C++)\nAuthor: https://www.github.com/ucx15 \n\nUsage:\n\tslocc.exe [options] [path]\n\n\tCounts lines of code with comments and blank lines in a Directory.\n\tExamples:\tslocc Src/\tslocc\n";
            }
            
            else {
                std::cout << "invalid Flag\n";
                exit(1);
            }
            
            exit(0);
        }

        else {path = flag;}
    }


    // Main Interface
    std::cout << "Checking: " << path << '\n';

    // Get a list of files and number of files
    int fileCount = 0;
    std::string *fileNames = get_files(path, &fileCount);


    // Parse the files One by one
    std::cout << "Parsing " << fileCount << " Files\n\n";
    std::cout << std::string(UNDER_COUNT, '_') << '\n';
    std::cout << "Lines\tcmnts\tBlanks\tSLOC\tFile\n";
    std::cout << std::string(UNDER_COUNT, '_') << '\n';


    int sloc_f, loc_f, cmnts_f, blnks_f;
    int sloc, loc, cmnts, blnks;

    sloc = loc = cmnts = blnks = 0;


    auto t1 = std::chrono::high_resolution_clock::now();
    
    if (fileCount) {
        for (int i = 0; i < fileCount; i++) {
            std::string fName = fileNames[i];

            sloc_f = loc_f = cmnts_f = blnks_f = 0;
            parse_file(fName, &sloc_f, &loc_f, &cmnts_f, &blnks_f);

            sloc  += sloc_f;
            loc   += loc_f;
            cmnts += cmnts_f;
            blnks += blnks_f;

            // Out Results
            std::cout << loc_f << '\t' << cmnts_f << '\t' << blnks_f << '\t' << sloc_f << '\t' << fName << '\n';
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    
    double elapsed_time_ms = std::chrono::duration<double, std::milli>(t2 - t1).count();
    double elapsed_time = elapsed_time_ms/1000;


    std::cout << std::string(UNDER_COUNT, '_') << '\n';
    std::cout << loc << '\t' << cmnts << '\t' << blnks << '\t' << sloc << '\t' << "TOTAL" << '\n';
    std::cout << std::string(UNDER_COUNT, '_') << '\n';

    std::cout << "\nTook\t" << elapsed_time_ms << " ms @ (" << int(fileCount*loc/elapsed_time) << " lines/s)\n";

    return 0;
}
