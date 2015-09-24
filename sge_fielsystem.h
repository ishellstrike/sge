/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef FIELSYSTEM_H
#define FIELSYSTEM_H
#include "logger.h"
#include <vector>
#include "helper.h"
#include <fstream>
#include <sstream>

inline std::string LoadTextFile(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::stringstream ss;
    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            ss << line << std::endl;
        }
        file.close();
    } else {
        LOG(fatal) << sge::string_format("Failed to open file %s", filename.c_str());
    }

    return ss.str();
}

inline void SaveTextFile(const std::string &filename, const std::string &content)
{
    std::ofstream file(filename);
    if (file.is_open()) {
        file << content;
        file.close();
    } else {
        LOG(fatal) << sge::string_format("Failed to save file %s", filename.c_str());
    }
}

inline int GetLastPatternedFilenameNubmer(const std::string &filename, const std::string &ext)
{
    bool cont = true;
    int i = 0;

    std::ifstream file(sge::string_format("%s%d%s", filename.c_str(), 0, ext.c_str()));
    if(!file.is_open()) {file.close(); return -1;}
    file.close();

    while(cont)
    {
        std::ifstream file(sge::string_format("%s%d%s", filename.c_str(), i, ext.c_str()));
        if(!file.is_open()) cont = false;
        else i++;
        file.close();
    }
    return i;
}

#ifdef _WIN32
    #include <windows.h>

    inline void getFiles(const std::string &dir, std::vector<std::string> &files)
    {
        files.clear();
        WIN32_FIND_DATA f;


        std::wstring s = L"./" + sge::string_to_wstring(dir) + L"*";
        HANDLE h = FindFirstFile(s.c_str(), &f);
        if(h != INVALID_HANDLE_VALUE)
        {
            do
            {
                std::string fn = sge::wstring_to_string(f.cFileName);
                if(fn == "." || fn == "..")
                    continue;

                files.push_back(fn);
            } while(FindNextFile(h, &f));
        }
        else
            LOG(error) << "Error opening directory";
    }

#endif
#ifdef __unix__
#include <sys/types.h>
#include <dirent.h>

    inline void getFiles(const std::string &dir, std::vector<std::string> &files)
    {
        files.clear();
        DIR *direct = opendir(("./"+dir).c_str());
        if(direct)
        {
            struct dirent *ent;
            while((ent = readdir(direct)) != nullptr)
            {
                std::string fn = ent->d_name;
                if(fn == "." || fn == "..")
                    continue;

                files.push_back(fn);
            }
        }
        else
            LOG(error) << "Error opening directory";
    }
#endif

#endif // FIELSYSTEM_H

